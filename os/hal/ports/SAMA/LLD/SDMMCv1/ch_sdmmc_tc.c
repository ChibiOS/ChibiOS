#include "hal.h"
#include "sama_sdmmc_lld.h"
#include "ch_sdmmc_pmc.h"
#include "ch_sdmmc_tc.h"

/*------------------------------------------------------------------------------
 *         Global functions
 *------------------------------------------------------------------------------*/
uint32_t get_tc_id_from_addr(const Tc* addr, uint8_t channel)
{
	(void)channel;
#ifdef TC0
	if (addr == TC0)
#ifdef ID_TC0_CH0
		return ID_TC0 + channel;
#else
		return ID_TC0;
#endif
#endif

#ifdef TC1
	if (addr == TC1)
#ifdef ID_TC1_CH0
		return ID_TC1 + channel;
#else
		return ID_TC1;
#endif
#endif

#ifdef TC2
	if (addr == TC2)
#ifdef ID_TC2_CH0
		return ID_TC2 + channel;
#else
		return ID_TC2;
#endif
#endif

#ifdef TC3
	if (addr == TC3)
#ifdef ID_TC3_CH0
		return ID_TC3 + channel;
#else
		return ID_TC3;
#endif
#endif
	return ID_PERIPH_COUNT;
}
void tc_configure(Tc *tc, uint32_t channel, uint32_t mode)
{
	TcChannel *ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	/*  Disable TC clock */
	ch->TC_CCR = TC_CCR_CLKDIS;

	/*  Disable interrupts */
	ch->TC_IDR = ch->TC_IMR;

	/*  Clear status register */
	ch->TC_SR;

	/*  Set mode */
	ch->TC_CMR = mode;
}

void tc_start(Tc *tc, uint32_t channel)
{
	TcChannel *ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	/*  Clear status register */
	ch->TC_SR;

	ch->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void tc_stop(Tc *tc, uint32_t channel)
{
	TcChannel *ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	ch->TC_CCR = TC_CCR_CLKDIS;
}

void tc_enable_it(Tc *tc, uint32_t channel, uint32_t mask)
{
	TcChannel *ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	ch->TC_IER = mask;
}

void tc_disable_it(Tc *tc, uint32_t channel, uint32_t mask)
{
	TcChannel *ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	ch->TC_IDR = mask;
}

uint32_t tc_find_best_clock_source(Tc *tc, uint8_t channel, uint32_t freq)
{
	const int tcclks[] = {
		TC_CMR_TCCLKS_TIMER_CLOCK1,
		TC_CMR_TCCLKS_TIMER_CLOCK2,
		TC_CMR_TCCLKS_TIMER_CLOCK3,
		TC_CMR_TCCLKS_TIMER_CLOCK4,
		TC_CMR_TCCLKS_TIMER_CLOCK5,
	};
	int i, best, higher;
	int best_freq, higher_freq;

	best = higher = -1;
	best_freq = higher_freq = 0;
	for (i = 0 ; i <(int) ARRAY_SIZE(tcclks) ; i++) {
		uint32_t f = tc_get_available_freq(tc, channel, tcclks[i]);
		if ( higher < 0 || (f > ((uint32_t)higher_freq) ) ) {
			higher_freq = f;
			higher = tcclks[i];
		}
		if (f > freq) {
			if (best < 0 || (f - freq) < (f - best_freq)) {
				best_freq = f;
				best = tcclks[i];
			}
		}
	}

	if (best < 0)
		best = higher;

	return best;
}

uint32_t tc_get_status(Tc *tc, uint32_t channel)
{
//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	return tc->TC_CHANNEL[channel].TC_SR;
}

uint32_t tc_get_available_freq(Tc *tc, uint8_t channel, uint8_t tc_clks)
{
	uint32_t tc_id = get_tc_id_from_addr(tc, channel);

	switch (tc_clks) {
	case TC_CMR_TCCLKS_TIMER_CLOCK1:
#ifdef CONFIG_HAVE_PMC_GENERATED_CLOCKS
		if (pmc_is_gck_enabled(tc_id))
			return pmc_get_gck_clock(tc_id);
		else
			return 0;
#else
		return pmc_get_peripheral_clock(tc_id) >> 1;
#endif
	case TC_CMR_TCCLKS_TIMER_CLOCK2:
		return pmc_get_peripheral_clock(tc_id) >> 3;
	case TC_CMR_TCCLKS_TIMER_CLOCK3:
		return pmc_get_peripheral_clock(tc_id) >> 5;
	case TC_CMR_TCCLKS_TIMER_CLOCK4:
		return pmc_get_peripheral_clock(tc_id) >> 7;
	case TC_CMR_TCCLKS_TIMER_CLOCK5:
		return pmc_get_slow_clock();
	default:
		return 0;
	}
}

uint32_t tc_get_channel_freq(Tc *tc, uint32_t channel)
{
	TcChannel* ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	return tc_get_available_freq(tc, channel, ch->TC_CMR & TC_CMR_TCCLKS_Msk);
}

void tc_set_ra_rb_rc(Tc *tc, uint32_t channel,
	uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
	TcChannel* ch;

	//assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

//	assert(!(ra && rb) || (ch->TC_CMR & TC_CMR_WAVE));

	if (ra)
		ch->TC_RA = *ra;
	if (rb)
		ch->TC_RB = *rb;
	if (rc)
		ch->TC_RC = *rc;
}

void tc_get_ra_rb_rc(Tc *tc, uint32_t channel,
	uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
	TcChannel* ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	if (ra)
		*ra = ch->TC_RA;
	if (rb)
		*rb = ch->TC_RB;
	if (rc)
		*rc = ch->TC_RC;
}

#ifdef CONFIG_HAVE_TC_FAULT_MODE

void tc_set_fault_mode(Tc *tc, uint32_t mode)
{
	tc->TC_FMR = mode;
}

#endif /* CONFIG_HAVE_TC_FAULT_MODE */

uint32_t tc_get_cv(Tc* tc, uint32_t channel)
{
	TcChannel* ch;

//	assert(channel < ARRAY_SIZE(tc->TC_CHANNEL));

	ch = &tc->TC_CHANNEL[channel];

	return ch->TC_CV;
}

