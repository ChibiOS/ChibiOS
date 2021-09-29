/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Tail ISR context switch code.
 *
 * @return              The thread being switched-in.
 */
thread_t *port_schedule_next(void) {
  thread_t *ntp;

  chSysLock();

  /* TODO statistics, tracing etc */
  ntp = chSchSelectFirst();

  chSysUnlock();

  return ntp;
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

  /* Initializing priority grouping.*/
  NVIC_SetPriorityGrouping(CORTEX_PRIGROUP_INIT);

  /* DWT cycle counter enable.*/
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
#if CORTEX_MODEL == 7
  DWT->LAR = 0xC5ACCE55U;
#endif
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /* Initialization of the system vectors used by the port.*/
  NVIC_SetPriority(SVCall_IRQn, CORTEX_PRIORITY_SVCALL);
  NVIC_SetPriority(PendSV_IRQn, CORTEX_PRIORITY_PENDSV);

#if PORT_ENABLE_GUARD_PAGES == TRUE
  {
    extern stkalign_t __main_thread_stack_base__;

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

#if PORT_USE_SYSCALL == TRUE
  /* MPU is enabled.*/
  mpuEnable(MPU_CTRL_PRIVDEFENA);
#endif
}

/** @} */
