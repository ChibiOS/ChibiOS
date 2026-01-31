/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ARMv7-M-ALT/chcore.c
 * @brief   ARMv7-M (alternate) port code.
 *
 * @addtogroup ARMV7M_ALT_CORE
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if (PORT_MPU_INITIALIZE == TRUE) || defined(__DOXYGEN__)
static void port_init_regions(const uint32_t *src, volatile uint32_t *dst) {

  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
  *dst++ = *src++;
}
#endif

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if (PORT_USE_SYSCALL == TRUE) || defined(__DOXYGEN__)
CC_NO_INLINE void port_syslock_noinline(void) {

  port_lock();
  __stats_start_measure_crit_thd();
  __dbg_check_lock();
}

CC_NO_INLINE uint32_t port_get_s_psp(void) {

  return (uint32_t)__sch_get_currthread()->waend;
}

CC_WEAK void __port_do_fastcall_entry(uint32_t n, struct port_extctx *ectxp) {

  (void)ectxp;
  (void)n;

  chSysHalt("unimplemented fastcall");
}

CC_WEAK void __port_do_syscall_entry(uint32_t n, struct port_extctx *ectxp) {

  (void)ectxp;
  (void)n;

  chSysHalt("unimplemented syscall");
}

CC_WEAK void __port_do_syscall_return(void) {

  chSysHalt("unimplemented syscall return");
}
#endif /* PORT_USE_SYSCALL == TRUE */

#if (PORT_ENABLE_GUARD_PAGES == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Setting up MPU region for the current thread.
 */
void __port_set_region(void) {

  mpuSetRegionAddress(PORT_USE_GUARD_MPU_REGION,
                      chThdGetSelfX()->wabase);
}
#endif

/**
 * @brief   Tail ISR context switch code.
 *
 * @return              The threads pointers encoded in a single 64 bits value.
 */
uint64_t __port_schedule_next(void) {

  /* Note, not an error, we are outside the ISR already.*/
  chSysLock();

  if (likely(chSchIsPreemptionRequired())) {
    thread_t *otp, *ntp;

    otp = chThdGetSelfX();
    ntp = chSchSelectFirst();

#if PORT_ENABLE_GUARD_PAGES == TRUE
    mpuSetRegionAddress(PORT_USE_GUARD_MPU_REGION, ntp->wabase);
#endif

    __trace_switch(ntp, otp);
    __stats_ctxswc(ntp, otp);
    CH_CFG_CONTEXT_SWITCH_HOOK(ntp, otp);

    return ((uint64_t)(uint32_t)otp << 32) | ((uint64_t)(uint32_t)ntp << 0);
  }

  chSysUnlock();

  return (uint64_t)0;
}

/**
 * @brief   Port-related initialization code.
 *
 * @param[in, out] oip  pointer to the @p os_instance_t structure
 *
 * @notapi
 */
void port_init(os_instance_t *oip) {

  (void)oip;

  /* Starting in a known IRQ configuration.*/
  port_suspend();

#if CORTEX_USE_FPU == TRUE
  {
    uint32_t control;

    /* Making sure to use the correct settings for FPU-related exception
       handling, better do not rely on startup settings.*/
    FPU->FPDSCR = 0U;
    __set_FPSCR(0U);
#if PORT_USE_FPU_FAST_SWITCHING == 0
    /* No lazy context saving, always long exception context.*/
    control = CONTROL_FPCA_Msk | CONTROL_SPSEL_Msk;
    FPU->FPCCR = FPU_FPCCR_ASPEN_Msk;
#elif PORT_USE_FPU_FAST_SWITCHING == 1
    /* Lazy context saving enabled, always long exception context.*/
    control = CONTROL_FPCA_Msk | CONTROL_SPSEL_Msk;
    FPU->FPCCR = FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
#else /*PORT_USE_FPU_FAST_SWITCHING >= 2 */
    /* Lazy context saving enabled, automatic FPCA control.*/
    control = CONTROL_SPSEL_Msk;
    FPU->FPCCR = FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
#endif
    __set_CONTROL(control);
    __ISB();
  }
#endif /* CORTEX_USE_FPU == TRUE */

  /* Initializing priority grouping.*/
  NVIC_SetPriorityGrouping(PORT_PRIGROUP_INIT);

  /* DWT cycle counter enable.*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#if CORTEX_MODEL == 7
  DWT->LAR = 0xC5ACCE55U;
#endif
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /* Initialization of the system vectors used by the port.*/
  NVIC_SetPriority(SVCall_IRQn, CORTEX_PRIORITY_SVCALL);
  NVIC_SetPriority(PendSV_IRQn, CORTEX_PRIORITY_PENDSV);

#if PORT_MPU_INITIALIZE == TRUE
  /* MPU initialization as specified in port options.*/
  {
    static const uint32_t regs0[]  = {PORT_MPU_RBAR0_INIT | MPU_REGION_0, PORT_MPU_RASR0_INIT,
                                      PORT_MPU_RBAR1_INIT | MPU_REGION_1, PORT_MPU_RASR1_INIT,
                                      PORT_MPU_RBAR2_INIT | MPU_REGION_2, PORT_MPU_RASR2_INIT,
                                      PORT_MPU_RBAR3_INIT | MPU_REGION_3, PORT_MPU_RASR3_INIT};
    port_init_regions(regs0, &MPU->RBAR);

#if CORTEX_MPU_REGIONS > 4
    static const uint32_t regs4[]  = {PORT_MPU_RBAR4_INIT | MPU_REGION_4, PORT_MPU_RASR4_INIT,
                                      PORT_MPU_RBAR5_INIT | MPU_REGION_5, PORT_MPU_RASR5_INIT,
                                      PORT_MPU_RBAR6_INIT | MPU_REGION_6, PORT_MPU_RASR6_INIT,
                                      PORT_MPU_RBAR7_INIT | MPU_REGION_7, PORT_MPU_RASR7_INIT};
    port_init_regions(regs4, &MPU->RBAR);
#endif
  }
#endif

#if PORT_ENABLE_GUARD_PAGES == TRUE
  {
    extern stkline_t __main_thread_stack_base__;

    /* Setting up the guard page on the main() function stack base
       initially.*/
    mpuConfigureRegion(PORT_USE_GUARD_MPU_REGION,
                       &__main_thread_stack_base__,
                       MPU_RASR_ATTR_AP_NA_NA |
                       MPU_RASR_ATTR_NON_CACHEABLE |
                       MPU_RASR_SIZE_32 |
                       MPU_RASR_ENABLE);
  }
#endif

#if (PORT_MPU_INITIALIZE == TRUE) || (PORT_SWITCHED_REGIONS_NUMBER > 0) ||  \
    (PORT_ENABLE_GUARD_PAGES == TRUE)
  /* MPU is enabled.*/
  mpuEnable(MPU_CTRL_PRIVDEFENA);
#endif
}

/** @} */
