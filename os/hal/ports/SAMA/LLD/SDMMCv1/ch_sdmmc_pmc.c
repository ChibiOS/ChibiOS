#include "hal.h"
#include "sama_sdmmc_lld.h"
#include "ch_sdmmc_pmc.h"



struct _pmc_main_osc {
	uint32_t rc_freq;
	uint32_t crystal_freq;
};



enum _slowclock_domain {
	SLOWCLOCK_DOMAIN_DEFAULT, /* Default slow clock, used as input for peripherals */
#ifdef CONFIG_HAVE_SLOWCLOCK_TIMING_DOMAIN
	SLOWCLOCK_DOMAIN_TIMING,  /* Timing Domain slow clock (RTC, RTT) */
#endif
};

static uint32_t _pmc_mck = 0;
static struct _pmc_main_osc _pmc_main_oscillators = {
	.rc_freq = MAIN_CLOCK_INT_OSC,
};

uint32_t pmc_get_slow_clock(void);
void pmc_disable_gck(uint32_t id);
int pmc_select_external_osc(bool bypass);
void pmc_enable_internal_osc(void);
void pmc_switch_mck_to_slck(void);
void pmc_select_internal_osc(void);
void pmc_disable_external_osc(void);
void pmc_disable_internal_osc(void);

static uint16_t _pmc_measure_main_osc_freq(bool external_xt)
{
	volatile uint32_t timeout = MAINFRDY_TIMEOUT;

#ifdef CKGR_MCFR_CCSS
	PMC->CKGR_MCFR = external_xt ? CKGR_MCFR_CCSS : 0;
#endif

#ifdef CKGR_MCFR_RCMEAS
	PMC->CKGR_MCFR |= CKGR_MCFR_RCMEAS;
#endif
	timeout = MAINFRDY_TIMEOUT;
	while (!(PMC->CKGR_MCFR & CKGR_MCFR_MAINFRDY) && --timeout > 0);

	return (timeout ?
		((PMC->CKGR_MCFR & CKGR_MCFR_MAINF_Msk) >> CKGR_MCFR_MAINF_Pos) :
		0u);
}

void pmc_switch_mck_to_new_source(uint32_t mckr_css)
{
	uint32_t mckr = PMC->PMC_MCKR;
	uint32_t mask = PMC_MCKR_CSS_Msk;

	if ((mckr ^ mckr_css) & mask) {
		PMC->PMC_MCKR = (mckr & ~mask) | (mckr_css & mask);
		while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
	}

	_pmc_mck = 0;
}


uint32_t pmc_set_main_oscillator_freq(uint32_t freq)
{
	uint32_t mor, mckr, mckr_mask;
	uint16_t mainf_rc, mainf_xt = 0;

	_pmc_main_oscillators.crystal_freq = freq;

	if (freq > 0)
		return freq;
#if 1
	/*
	 * Save the current value of the CKGR_MCKR register then swith to
	 * the slow clock.
	 */
	mckr = PMC->PMC_MCKR;
	pmc_switch_mck_to_slck();
	mckr_mask = PMC_MCKR_MDIV_Msk | PMC_MCKR_PRES_Msk;
	PMC->PMC_MCKR &= ~mckr_mask;

	/* Save the current value of the CKGR_MOR register. */
	mor = PMC->CKGR_MOR;

	/* Measure the 12MHz RC frequency. */
	pmc_select_internal_osc();
	mainf_rc = _pmc_measure_main_osc_freq(false);

	/* Measure the crystal or by-pass frequency. */

	/* Try by-pass first. */
	if (pmc_select_external_osc(true) == 0)
		mainf_xt = _pmc_measure_main_osc_freq(true);

	/* Then try external crytal if no by-pass. */
	if (!mainf_xt) {
		if (pmc_select_external_osc(false) == 0)
			mainf_xt = _pmc_measure_main_osc_freq(true);
	}

	/* Switch back to internal 12MHz RC if it was selected initially */
	if (!(mor & CKGR_MOR_MOSCSEL))
		pmc_select_internal_osc();

#ifdef CKGR_MOR_MOSCRCEN
	/* Disable internal oscillator if it wasn't enabled initially */
	if (!(mor & CKGR_MOR_MOSCRCEN))
		pmc_disable_internal_osc();
#endif

	/* Switch back to the former MCK source. */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~mckr_mask) | (mckr & mckr_mask);
	pmc_switch_mck_to_new_source(mckr & PMC_MCKR_CSS_Msk);

	/* Guess the external crystal frequency, if available. */
	if (mainf_rc && mainf_xt) {
		uint32_t ratio = (mainf_xt * 1000) / mainf_rc;

		// Use 10% low and high margins
		if (1800 <= ratio && ratio <= 2200) {
			// 24/12 => ratio = 2000
			_pmc_main_oscillators.crystal_freq = 24000000u;
		} else if (1200 <= ratio && ratio <= 1467) {
			// 16/12 => ratio = 1333
			_pmc_main_oscillators.crystal_freq = 16000000u;
		} else if (900 <= ratio && ratio <= 1100) {
			// 12/12 => ratio = 1000
			_pmc_main_oscillators.crystal_freq = 12000000u;
		} else if (600 <= ratio && ratio <= 733) {
			// 8/12 => ratio = 667
			_pmc_main_oscillators.crystal_freq = 8000000u;
		}
	}
#endif
	return _pmc_main_oscillators.crystal_freq;
}

bool pmc_is_peripheral_enabled(uint32_t id)
{
//	assert(id < ID_PERIPH_COUNT);

#ifdef PMC_CSR_PID0
	return (PMC->PMC_CSR[(id >> 5) & 3] & (1 << (id & 31))) != 0;
#else
	PMC->PMC_PCR = PMC_PCR_PID(id);
	volatile uint32_t pcr = PMC->PMC_PCR;

	return (pcr & PMC_PCR_EN) != 0;
#endif
}

void pmc_enable_peripheral(uint32_t id)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);

	// select peripheral
	PMC->PMC_PCR = PMC_PCR_PID(id);

	volatile uint32_t pcr = PMC->PMC_PCR;
	PMC->PMC_PCR = pcr | PMC_PCR_CMD | PMC_PCR_EN;
}

void pmc_disable_peripheral(uint32_t id)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);

	// select peripheral
	PMC->PMC_PCR = PMC_PCR_PID(id);

	// disable it but keep previous configuration
	PMC->PMC_PCR = (PMC->PMC_PCR & ~PMC_PCR_EN) | PMC_PCR_CMD;
}

Matrix* get_peripheral_matrix(uint32_t id)
{
	//int i;
	switch(id)
	{
	case ID_ARM_PMU: /*  2: Performance Monitor Unit (PMU) (ARM_PMU) */
	case ID_XDMAC0:  /*  6: DMA Controller 0 (XDMAC0) */
	case ID_XDMAC1:  /*  7: DMA Controller 1 (XDMAC1) */
	case ID_AES:     /*  9: Advanced Enion Standard (AES) */
	case ID_AESB:    /* 10: AES bridge (AESB) */
	case ID_SHA:     /* 12: SHA Signature (SHA) */
	case ID_MPDDRC:  /* 13: MPDDR controller (MPDDRC) */
	case ID_MATRIX0: /* 15: H64MX: 64-bit AHB Matrix (MATRIX0) */
	case ID_SDMMC0:  /* 31: Secure Digital Multimedia Card Controller 0 (SDMMC0) */
	case ID_SDMMC1:  /* 32: Secure Digital Multimedia Card Controller 1 (SDMMC1) */
	case ID_LCDC:    /* 45: LCD Controller (LCDC) */
	case ID_ISC:     /* 46: Camera Interface (ISC) */
	case ID_QSPI0:   /* 52: QSPI 0 (QSPI0) */
	case ID_QSPI1:   /* 53: QSPI 1 (QSPI1) */
	case ID_L2CC:    /* 63: L2 Cache Controller (L2CC) */
		return MATRIX0; // AHB 64-bit matrix
	default:
		return MATRIX1; // AHB 32-bit matrix
	};

}

uint32_t get_peripheral_clock_matrix_div(uint32_t id)
{
	Matrix* matrix = get_peripheral_matrix(id);

	if (matrix == MATRIX1) {
		if (PMC->PMC_MCKR & PMC_MCKR_H32MXDIV_H32MXDIV2)
			return 2;
		else
			return 1;
	}

	return 1;
}


uint32_t pmc_get_peripheral_clock(uint32_t id)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);

	uint32_t div = get_peripheral_clock_matrix_div(id);
#ifdef CONFIG_HAVE_PMC_PERIPH_DIV
	PMC->PMC_PCR = PMC_PCR_PID(id);
	volatile uint32_t pcr = PMC->PMC_PCR;
	div *= 1 << ((pcr & PMC_PCR_DIV_Msk) >> PMC_PCR_DIV_Pos);
#endif

	return pmc_get_master_clock() / div;
}

bool slowclock_is_internal(enum _slowclock_domain domain)
{
	(void)domain;
	return (SCKC->SCKC_CR & SCKC_CR_OSCSEL) != SCKC_CR_OSCSEL;
}

uint32_t slowclock_get_clock(enum _slowclock_domain domain)
{
	if (slowclock_is_internal(domain))
		return 32000;
	else
		return 32768;
}

uint32_t pmc_get_slow_clock(void)
{
	return slowclock_get_clock(SLOWCLOCK_DOMAIN_DEFAULT);
}

uint32_t pmc_get_upll_clock(void)
{
	uint32_t upllclk;

#if defined(SFR_UTMICKTRIM_FREQ_Msk)
	uint32_t clktrim = SFR->SFR_UTMICKTRIM & SFR_UTMICKTRIM_FREQ_Msk;
	switch (clktrim) {
#ifdef SFR_UTMICKTRIM_FREQ_48
		case SFR_UTMICKTRIM_FREQ_48:
			upllclk = 10 * _pmc_main_oscillators.crystal_freq;
			break;
#endif
		case SFR_UTMICKTRIM_FREQ_24:
			upllclk = 20 * _pmc_main_oscillators.crystal_freq;
			break;
		case SFR_UTMICKTRIM_FREQ_16:
			upllclk = 30 * _pmc_main_oscillators.crystal_freq;
			break;
		default:
			upllclk = 40 * _pmc_main_oscillators.crystal_freq;
			break;
	}
#elif defined(UTMI_CKTRIM_FREQ_Msk)
	uint32_t clktrim = UTMI->UTMI_CKTRIM & UTMI_CKTRIM_FREQ_Msk;
	switch (clktrim) {
		case UTMI_CKTRIM_FREQ_XTAL16:
			upllclk = 30 * _pmc_main_oscillators.crystal_freq;
			break;
		default:
			upllclk = 40 * _pmc_main_oscillators.crystal_freq;
			break;
	}
#else
	upllclk = 40 * _pmc_main_oscillators.crystal_freq;
#endif

#ifdef CONFIG_HAVE_PMC_UPLLDIV2
	if (PMC->PMC_MCKR & PMC_MCKR_UPLLDIV2)
		upllclk >>= 1;
#endif

	return upllclk;
}

uint32_t pmc_get_main_clock(void)
{
	if (PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)
		return _pmc_main_oscillators.crystal_freq; /* external crystal */
	else
		return _pmc_main_oscillators.rc_freq; /* on-chip main clock RC */
}

uint32_t pmc_get_plla_clock(void)
{
	uint32_t pllaclk, pllar, pllmula, plldiva;

	pllar = PMC->CKGR_PLLAR;
	pllmula = (pllar & CKGR_PLLAR_MULA_Msk) >> CKGR_PLLAR_MULA_Pos;
	plldiva = (pllar & CKGR_PLLAR_DIVA_Msk) >> CKGR_PLLAR_DIVA_Pos;
	if (plldiva == 0 || pllmula == 0)
		return 0;

	pllaclk = pmc_get_main_clock();
	pllaclk = pllaclk * (pllmula + 1) / plldiva;
#ifdef CONFIG_HAVE_PMC_PLLADIV2
	if (PMC->PMC_MCKR & PMC_MCKR_PLLADIV2)
		pllaclk >>= 1;
#endif
	return pllaclk;
}

uint32_t pmc_get_processor_clock(void)
{
	uint32_t procclk, mdiv;

	procclk = pmc_get_master_clock();

	mdiv = PMC->PMC_MCKR & PMC_MCKR_MDIV_Msk;
	switch (mdiv) {
	case PMC_MCKR_MDIV_EQ_PCK:
		break;
	case PMC_MCKR_MDIV_PCK_DIV2:
		procclk <<= 1; // multiply by 2
		break;
	case PMC_MCKR_MDIV_PCK_DIV3:
		procclk *= 3;  // multiply by 3
		break;
	case PMC_MCKR_MDIV_PCK_DIV4:
		procclk <<= 2; // multiply by 4
		break;
	default:
		/* should never get here... */
		break;
	}

	return procclk;
}



static void _pmc_compute_mck(void)
{
	uint32_t clk = 0;
	uint32_t mckr = PMC->PMC_MCKR;

	uint32_t css = mckr & PMC_MCKR_CSS_Msk;
	switch (css) {
	case PMC_MCKR_CSS_SLOW_CLK:
		clk = pmc_get_slow_clock();
		break;
	case PMC_MCKR_CSS_MAIN_CLK:
		clk = pmc_get_main_clock();
		break;
	case PMC_MCKR_CSS_PLLA_CLK:
		clk = pmc_get_plla_clock();
		break;
	case PMC_MCKR_CSS_UPLL_CLK:
		clk = pmc_get_upll_clock();
		break;
	default:
		/* should never get here... */
		break;
	}

	uint32_t pres = mckr & PMC_MCKR_PRES_Msk;
	switch (pres) {
	case PMC_MCKR_PRES_CLOCK:
		break;
	case PMC_MCKR_PRES_CLOCK_DIV2:
		clk >>= 1;
		break;
	case PMC_MCKR_PRES_CLOCK_DIV4:
		clk >>= 2;
		break;
	case PMC_MCKR_PRES_CLOCK_DIV8:
		clk >>= 3;
		break;
	case PMC_MCKR_PRES_CLOCK_DIV16:
		clk >>= 4;
		break;
	case PMC_MCKR_PRES_CLOCK_DIV32:
		clk >>= 5;
		break;
	case PMC_MCKR_PRES_CLOCK_DIV64:
		clk >>= 6;
		break;
#ifdef PMC_MCKR_PRES_CLOCK_DIV3
	case PMC_MCKR_PRES_CLOCK_DIV3:
		clk /= 3;
		break;
#endif
	default:
		/* should never get here... */
		break;
	}

	uint32_t mdiv = mckr & PMC_MCKR_MDIV_Msk;
	switch (mdiv) {
	case PMC_MCKR_MDIV_EQ_PCK:
		break;
	case PMC_MCKR_MDIV_PCK_DIV2:
		clk >>= 1; // divide by 2
		break;
	case PMC_MCKR_MDIV_PCK_DIV4:
		clk >>= 2; // divide by 4
		break;
	case PMC_MCKR_MDIV_PCK_DIV3:
		clk /= 3;  // divide by 3
		break;
	default:
		/* should never get here... */
		break;
	}

	_pmc_mck = clk;
}

uint32_t pmc_get_master_clock(void)
{
	if (!_pmc_mck)
		_pmc_compute_mck();
	return _pmc_mck;
}


void pmc_configure_gck(uint32_t id, uint32_t clock_source, uint32_t div)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);
	osalDbgCheck(!(clock_source & ~PMC_PCR_GCKCSS_Msk));
	osalDbgCheck(div > 0);
	osalDbgCheck(!((div << PMC_PCR_GCKDIV_Pos) & ~PMC_PCR_GCKDIV_Msk));

	pmc_disable_gck(id);
	PMC->PMC_PCR = PMC_PCR_PID(id);
	volatile uint32_t pcr = PMC->PMC_PCR & ~(PMC_PCR_GCKCSS_Msk | PMC_PCR_GCKDIV_Msk);
	PMC->PMC_PCR = pcr | clock_source | PMC_PCR_CMD | PMC_PCR_GCKDIV(div - 1);
}


void pmc_enable_gck(uint32_t id)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);

	PMC->PMC_PCR = PMC_PCR_PID(id);
	volatile uint32_t pcr = PMC->PMC_PCR;
	PMC->PMC_PCR = pcr | PMC_PCR_CMD | PMC_PCR_GCKEN;

#ifdef PMC_GCSR_PID0
	while ((PMC->PMC_GCSR[(id >> 5) & 3] & (1 << (id & 31))) == 0);
#else
	while (!(PMC->PMC_SR & PMC_SR_GCKRDY));
#endif
}

void pmc_disable_gck(uint32_t id)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);

	PMC->PMC_PCR = PMC_PCR_PID(id);
	volatile uint32_t pcr = PMC->PMC_PCR;
	PMC->PMC_PCR = PMC_PCR_CMD | (pcr & ~PMC_PCR_GCKEN);
}

uint32_t pmc_get_gck_clock(uint32_t id)
{
	uint32_t clk = 0;
	osalDbgCheck(id < ID_PERIPH_COUNT);

	PMC->PMC_PCR = PMC_PCR_PID(id);
	volatile uint32_t pcr = PMC->PMC_PCR;

	switch (pcr & PMC_PCR_GCKCSS_Msk) {
	case PMC_PCR_GCKCSS_SLOW_CLK:
		clk = pmc_get_slow_clock();
		break;
	case PMC_PCR_GCKCSS_MAIN_CLK:
		clk = pmc_get_main_clock();
		break;
	case PMC_PCR_GCKCSS_PLLA_CLK:
		clk = pmc_get_plla_clock();
		break;
	case PMC_PCR_GCKCSS_UPLL_CLK:
		clk = pmc_get_upll_clock();
		break;
	case PMC_PCR_GCKCSS_MCK_CLK:
		clk = pmc_get_master_clock();
		break;
#ifdef CONFIG_HAVE_PMC_AUDIO_CLOCK
	case PMC_PCR_GCKCSS_AUDIO_CLK:
		clk = pmc_get_audio_pmc_clock();
		break;
#endif
	}

	uint32_t div = (pcr & PMC_PCR_GCKDIV_Msk) >> PMC_PCR_GCKDIV_Pos;
	return ROUND_INT_DIV(clk, div + 1);
}

void pmc_configure_peripheral(uint32_t id, const struct _pmc_periph_cfg* cfg, bool enable)
{
	osalDbgCheck(id < ID_PERIPH_COUNT);

	pmc_disable_peripheral(id);

	if (cfg != NULL) {

		if (cfg->gck.div > 0)
			pmc_configure_gck(id, cfg->gck.css, cfg->gck.div);

	} else {
		pmc_disable_gck(id);
	}

	/* Enable peripheral, gck or only configure it */
	if (enable) {
		if (cfg && cfg->gck.div > 0)
			pmc_enable_gck(id);

		pmc_enable_peripheral(id);
	}
}


void pmc_enable_upll_clock(void)
{
	uint32_t uckr = CKGR_UCKR_UPLLEN | CKGR_UCKR_UPLLCOUNT(0x3);
	uckr |= CKGR_UCKR_BIASCOUNT(0x1);


#if defined(SFR_UTMICKTRIM_FREQ_Msk)
	switch (_pmc_main_oscillators.crystal_freq) {
#ifdef SFR_UTMICKTRIM_FREQ_48
	case 48000000:
		SFR->SFR_UTMICKTRIM = (SFR->SFR_UTMICKTRIM & ~SFR_UTMICKTRIM_FREQ_Msk) | SFR_UTMICKTRIM_FREQ_48;
		break;
#endif
	case 24000000:
		SFR->SFR_UTMICKTRIM = (SFR->SFR_UTMICKTRIM & ~SFR_UTMICKTRIM_FREQ_Msk) | SFR_UTMICKTRIM_FREQ_24;
		break;
	case 16000000:
		SFR->SFR_UTMICKTRIM = (SFR->SFR_UTMICKTRIM & ~SFR_UTMICKTRIM_FREQ_Msk) | SFR_UTMICKTRIM_FREQ_16;
		break;
	default:
		SFR->SFR_UTMICKTRIM = (SFR->SFR_UTMICKTRIM & ~SFR_UTMICKTRIM_FREQ_Msk) | SFR_UTMICKTRIM_FREQ_12;
	}
#elif defined(UTMI_CKTRIM_FREQ_Msk)
	switch (_pmc_main_oscillators.crystal_freq) {
	case 16000000:
		UTMI->UTMI_CKTRIM = (UTMI->UTMI_CKTRIM & ~UTMI_CKTRIM_FREQ_Msk) | UTMI_CKTRIM_FREQ_XTAL16;
		break;
	default:
		UTMI->UTMI_CKTRIM = (UTMI->UTMI_CKTRIM & ~UTMI_CKTRIM_FREQ_Msk) | UTMI_CKTRIM_FREQ_XTAL12;
	}
#endif

	/* enable the 480MHz UTMI PLL  */
	PMC->CKGR_UCKR = uckr;

	/* wait until UPLL is locked */
	while (!(PMC->PMC_SR & PMC_SR_LOCKU));
}


void pmc_enable_upll_bias(void)
{
	PMC->CKGR_UCKR |= CKGR_UCKR_BIASEN;
}

void pmc_disable_upll_bias(void)
{
	PMC->CKGR_UCKR &= ~CKGR_UCKR_BIASEN;
}

void pmc_switch_mck_to_slck(void)
{
	/* Select Slow Clock as input clock for PCK and MCK */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_SLOW_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	_pmc_mck = 0;
}

void pmc_select_internal_osc(void)
{
	pmc_enable_internal_osc();

	/* switch MAIN clock to internal 12MHz RC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~(CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_Msk)) | CKGR_MOR_KEY_PASSWD;

	/* in case where MCK is running on MAIN CLK */
	if ((PMC->PMC_MCKR & PMC_MCKR_CSS_PLLA_CLK) || (PMC->PMC_MCKR & PMC_MCKR_CSS_MAIN_CLK))
		while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	/* disable external OSC 12 MHz to save power*/
	pmc_disable_external_osc();
}

int pmc_enable_external_osc(bool bypass)
{
	uint32_t cgmor = PMC->CKGR_MOR;
	uint32_t mask = CKGR_MOR_MOSCXTEN;
	volatile uint32_t timeout;

	if (bypass)
		mask = CKGR_MOR_MOSCXTBY;

	/* Enable Crystal Oscillator if needed */
	if ((cgmor & mask) != mask) {
		cgmor &= ~CKGR_MOR_KEY_Msk;
		cgmor |= CKGR_MOR_KEY_PASSWD;

		if (bypass) {
			/* Disable Crystal Oscillator */
			cgmor &= ~CKGR_MOR_MOSCXTEN;
			PMC->CKGR_MOR = cgmor;

			/* Wait Main Oscillator not ready */
			while (PMC->PMC_SR & PMC_SR_MOSCXTS);

			/* Enable Crystal Oscillator Bypass */
			cgmor |= CKGR_MOR_MOSCXTBY;
			PMC->CKGR_MOR = cgmor;
		} else {
			/* Disable Crystal Oscillator Bypass */
			cgmor &= ~CKGR_MOR_MOSCXTBY;
			PMC->CKGR_MOR = cgmor;

			/* Wait Main Oscillator not ready */
			while (PMC->PMC_SR & PMC_SR_MOSCXTS);

			/* Set Oscillator Startup Time */
			cgmor &= ~CKGR_MOR_MOSCXTST_Msk;
			cgmor |= CKGR_MOR_MOSCXTST(18);
			PMC->CKGR_MOR = cgmor;

			/* Enable Crystal Oscillator */
			cgmor |= CKGR_MOR_MOSCXTEN;
			PMC->CKGR_MOR = cgmor;
		}

		/* Wait Main Oscillator ready */
		timeout = MOSCXTS_TIMEOUT;
		while (!(PMC->PMC_SR & PMC_SR_MOSCXTS) && --timeout > 0);

		/* Return true if oscillator ready before timeout */
		return timeout == 0 ? -1 : 0;
	} else {
		/* Crystal Oscillator already selected, just check if ready */
		if (PMC->PMC_SR & PMC_SR_MOSCXTS)
			return 0;
		else
			return -2;
	}
}

void pmc_disable_external_osc(void)
{
	/* disable external OSC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~(CKGR_MOR_MOSCSEL | CKGR_MOR_KEY_Msk)) | CKGR_MOR_KEY_PASSWD;
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~(CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCXTBY | CKGR_MOR_KEY_Msk)) | CKGR_MOR_KEY_PASSWD;
}
int pmc_select_external_osc(bool bypass)
{
	int err;
	volatile uint32_t timeout;

	/* Return if external oscillator had been selected */
	if ((PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) == CKGR_MOR_MOSCSEL) {
		uint32_t mask = bypass ? CKGR_MOR_MOSCXTBY : CKGR_MOR_MOSCXTEN;
		if ((PMC->CKGR_MOR & mask) == mask)
			return 0;
	}

	/*
	 * When switching the source of the main clock between the RC oscillator and the crystal
	 * oscillator, both oscillators must be enabled. After completion of the switch, the
	 * unused oscillator can be disabled.
	 */
	pmc_enable_internal_osc();
	err = pmc_enable_external_osc(bypass);
	if (err < 0)
		return err;

	/* switch MAIN clock to external oscillator */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_KEY_Msk) | CKGR_MOR_MOSCSEL
	    | CKGR_MOR_KEY_PASSWD;

	/* wait for the command to be taken into account */
	while ((PMC->CKGR_MOR & CKGR_MOR_MOSCSEL) != CKGR_MOR_MOSCSEL);

	/* wait MAIN clock status change for external oscillator selection */
	timeout = MOSCSELS_TIMEOUT;
	while (!(PMC->PMC_SR & PMC_SR_MOSCSELS) && --timeout > 0);
	if (!timeout) {
		PMC->CKGR_MOR &= ~CKGR_MOR_MOSCSEL;
		return -1;
	}

	/* in case where MCK is running on MAIN CLK */
	if ((PMC->PMC_MCKR & PMC_MCKR_CSS_PLLA_CLK) || (PMC->PMC_MCKR & PMC_MCKR_CSS_MAIN_CLK))
		while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	/* disable internal 12MHz RC to save power */
	pmc_disable_internal_osc();

	return 0;
}


void pmc_enable_internal_osc(void)
{
#ifdef CKGR_MOR_MOSCRCEN
	/* Enable internal 12MHz RC when needed */
	if ((PMC->CKGR_MOR & CKGR_MOR_MOSCRCEN) != CKGR_MOR_MOSCRCEN) {
		PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_KEY_Msk) | CKGR_MOR_MOSCRCEN | CKGR_MOR_KEY_PASSWD;
		/* Wait internal 12MHz RC Startup Time for clock stabilization */
		while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));
	}
#endif
}

void pmc_switch_mck_to_pll(void)
{
	/* Select PLL as input clock for PCK and MCK */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));

	_pmc_mck = 0;
}

void pmc_set_mck_prescaler(uint32_t prescaler)
{
	//assert(!(prescaler & ~PMC_MCKR_PRES_Msk));

	/* Change MCK Prescaler divider in PMC_MCKR register */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_PRES_Msk) | prescaler;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}


#ifdef CONFIG_HAVE_PMC_H32MXDIV
void pmc_set_mck_h32mxdiv(bool div2)
{
	uint32_t mckr = PMC->PMC_MCKR;
	if (div2) {
		if ((mckr & PMC_MCKR_H32MXDIV) != PMC_MCKR_H32MXDIV_H32MXDIV2)
			PMC->PMC_MCKR = (mckr & ~PMC_MCKR_H32MXDIV) | PMC_MCKR_H32MXDIV_H32MXDIV2;
	} else {
		if ((mckr & PMC_MCKR_H32MXDIV) != PMC_MCKR_H32MXDIV_H32MXDIV1)
			PMC->PMC_MCKR = (mckr & ~PMC_MCKR_H32MXDIV) | PMC_MCKR_H32MXDIV_H32MXDIV1;
	}
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}
#endif /* CONFIG_HAVE_PMC_H32MXDIV */

#ifdef CONFIG_HAVE_PMC_PLLADIV2
void pmc_set_mck_plladiv2(bool div2)
{
	uint32_t mckr = PMC->PMC_MCKR;
	if (div2) {
		if ((mckr & PMC_MCKR_PLLADIV2) != PMC_MCKR_PLLADIV2)
			PMC->PMC_MCKR = mckr | PMC_MCKR_PLLADIV2;
	} else {
		if ((mckr & PMC_MCKR_PLLADIV2) == PMC_MCKR_PLLADIV2)
			PMC->PMC_MCKR = mckr & ~PMC_MCKR_PLLADIV2;
	}
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}
#endif

void pmc_set_mck_divider(uint32_t divider)
{
	//assert(!(divider & ~PMC_MCKR_MDIV_Msk));

	/* change MCK Prescaler divider in PMC_MCKR register */
	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | divider;
	while (!(PMC->PMC_SR & PMC_SR_MCKRDY));
}

void pmc_configure_plla(const struct _pmc_plla_cfg* plla)
{
	uint32_t pllar = 0;

#ifdef CKGR_PLLAR_ONE
	pllar |= CKGR_PLLAR_ONE;
#endif
	pllar |= CKGR_PLLAR_MULA(plla->mul);
	pllar |= CKGR_PLLAR_DIVA(plla->div);
	pllar |= CKGR_PLLAR_PLLACOUNT(plla->count);
	PMC->CKGR_PLLAR = pllar;

#ifdef CONFIG_HAVE_PMC_PLLA_CHARGEPUMP
	PMC->PMC_PLLICPR = plla->icp & PMC_PLLICPR_ICP_PLLA_Msk;
#endif /* CONFIG_HAVE_PMC_PLLA_CHARGEPUMP */

	if (plla->mul > 0)
		while (!(PMC->PMC_SR & PMC_SR_LOCKA));
}

void pmc_disable_plla(void)
{
	PMC->CKGR_PLLAR = (PMC->CKGR_PLLAR & ~CKGR_PLLAR_MULA_Msk) | CKGR_PLLAR_MULA(0);
}






void pmc_disable_internal_osc(void)
{
#ifdef CKGR_MOR_MOSCRCEN
	/* disable internal 12MHz RC */
	PMC->CKGR_MOR = (PMC->CKGR_MOR & ~CKGR_MOR_MOSCRCEN & ~CKGR_MOR_KEY_Msk) | CKGR_MOR_KEY_PASSWD;
#endif
}

