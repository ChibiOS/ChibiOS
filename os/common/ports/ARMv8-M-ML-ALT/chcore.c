/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio.

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
 * @file    ARMv8-M-ML-ALT/chcore.c
 * @brief   ARMv8-M MainLine (alt) port code.
 *
 * @addtogroup ARMV8M_ML_ALT_CORE
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

CC_NO_INLINE uint32_t port_get_x_psp(void) {

  return (uint32_t)__port_syscall_get_x_psp(__sch_get_currthread());
}

CC_WEAK void __port_do_fastcall_entry(struct port_extctx *ectxp,
                                      uint32_t n) {

  (void)ectxp;
  (void)n;

  chSysHalt("unimplemented fastcall");
}

CC_WEAK void __port_do_syscall_entry(struct port_extctx *ectxp,
                                     uint32_t n) {

  (void)ectxp;
  (void)n;

  chSysHalt("unimplemented syscall");
}

CC_WEAK void __port_do_syscall_return(void) {

  __set_PSP(__port_syscall_get_x_psp(__sch_get_currthread()));
}
#endif /* PORT_USE_SYSCALL == TRUE */

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

    __trace_switch(ntp, otp);
    __stats_ctxswc(ntp, otp);

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
#if CORTEX_USE_FPU_FAST_SWITCHING == 0
    /* No lazy context saving, always long exception context.*/
    control = CONTROL_FPCA_Msk | CONTROL_SPSEL_Msk;
    FPU->FPCCR = 0U;
#elif CORTEX_USE_FPU_FAST_SWITCHING == 1
    /* Lazy context saving enabled, always long exception context.*/
    control = CONTROL_FPCA_Msk | CONTROL_SPSEL_Msk;
    FPU->FPCCR = FPU_FPCCR_LSPEN_Msk;
#else /*CORTEX_USE_FPU_FAST_SWITCHING >= 2 */
    /* Lazy context saving enabled, automatic FPCA control.*/
    control = CONTROL_SPSEL_Msk;
    FPU->FPCCR = FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
#endif
    __set_CONTROL(control);
    __ISB();
  }
#endif /* CORTEX_USE_FPU == TRUE */

  /* Initializing priority grouping.*/
  NVIC_SetPriorityGrouping(CORTEX_PRIGROUP_INIT);

  /* DWT cycle counter enable.*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /* Initialization of the system vectors used by the port.*/
  NVIC_SetPriority(SVCall_IRQn, CORTEX_PRIORITY_SVCALL);
  NVIC_SetPriority(PendSV_IRQn, CORTEX_PRIORITY_PENDSV);

#if PORT_USE_SYSCALL == TRUE
  /* MPU is enabled.*/
  mpuEnable(MPU_CTRL_PRIVDEFENA);
#endif
}

/** @} */
