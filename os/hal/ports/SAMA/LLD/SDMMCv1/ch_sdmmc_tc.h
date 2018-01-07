
#ifndef OS_HAL_PORTS_SAMA_LLD_SDMMCV0__SAMA_SDMMC_TC_H_
#define OS_HAL_PORTS_SAMA_LLD_SDMMCV0__SAMA_SDMMC_TC_H_


extern void tc_configure(Tc *tc, uint32_t channel, uint32_t mode);
extern void tc_start(Tc *tc, uint32_t channel);
extern void tc_stop(Tc *tc, uint32_t channel);
extern void tc_enable_it(Tc *tc, uint32_t channel, uint32_t mask);
extern void tc_disable_it(Tc *tc, uint32_t channel, uint32_t mask);
extern uint32_t tc_find_best_clock_source(Tc *tc, uint8_t channel, uint32_t freq);
extern uint32_t tc_get_status(Tc *tc, uint32_t channel);
extern uint32_t tc_get_available_freq(Tc *tc, uint8_t channel, uint8_t tc_clks);
extern uint32_t tc_get_channel_freq(Tc *tc, uint32_t channel);
extern void tc_set_ra_rb_rc(Tc *tc, uint32_t channel,uint32_t *ra, uint32_t *rb, uint32_t *rc);
extern void tc_get_ra_rb_rc(Tc *tc, uint32_t channel,uint32_t *ra, uint32_t *rb, uint32_t *rc);
extern uint32_t tc_get_cv(Tc* tc, uint32_t channel);

extern uint32_t get_tc_id_from_addr(const Tc* addr, uint8_t channel);

#endif /* OS_HAL_PORTS_SAMA_LLD_SDMMCV0__SAMA_SDMMC_TC_H_ */
