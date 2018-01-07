#ifndef CH_SDMMC_PMC_H_
#define CH_SDMMC_PMC_H_


struct _pmc_plla_cfg {
	/** PLLA MUL value */
	uint32_t mul;

	/** PLLA DIV value */
	uint32_t div;

	/** PLLA COUNT value (number of slow clock cycles before the PLLA is locked) */
	uint32_t count;

#ifdef CONFIG_HAVE_PMC_PLLA_CHARGE_PUMP
	/** PLLA ICP value */
	uint32_t icp;
#endif
};

struct	_pmc_periph_cfg{

	struct {
		/** gck source selection: SLOW, MAIN, PLLA, UPLL, MCK or AUDIO */
		uint32_t css;
		/** gck division ratio (0 means disable, n >= 1 divide by n) */
		uint32_t div;
	} gck;

};

#define pmcEnableSDMMC0() 	pmcEnablePidLow(ID_SDMMC0_MSK)
#define pmcDisableSDMMC0() pmcDisablePidLow(ID_SDMMC0_MSK)

#define pmcEnableSDMMC1() 	pmcEnablePidHigh(ID_SDMMC1_MSK)
#define pmcDisableSDMMC1() pmcDisablePidHigh(ID_SDMMC1_MSK)


extern void pmc_configure_peripheral(uint32_t id, const struct _pmc_periph_cfg* cfg, bool enable);
extern void pmc_enable_upll_clock(void);
extern void pmc_enable_upll_bias(void);
extern uint32_t pmc_get_peripheral_clock(uint32_t id);
extern uint32_t pmc_get_master_clock(void);
extern uint32_t pmc_get_gck_clock(uint32_t id);
extern bool pmc_is_peripheral_enabled(uint32_t id);
extern uint32_t pmc_set_main_oscillator_freq(uint32_t freq);
extern uint32_t pmc_get_slow_clock(void);
extern uint32_t pmc_get_processor_clock(void);

#endif /* CH_SDMMC_PMC_H_ */
