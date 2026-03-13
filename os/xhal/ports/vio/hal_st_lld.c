/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    sandbox/hal_st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"

#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if (PORT_USE_LOCAL_SYSTICK == FALSE) && (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC)
static sysinterval_t st_alarm_interval;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   Interrupt handler.
 *
 * @isr
 */
#if (PORT_USE_LOCAL_SYSTICK == FALSE) && (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC)
OSAL_IRQ_HANDLER(MK_VECTOR(SB_VRQ_ALARM)) {

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if (PORT_USE_LOCAL_SYSTICK == FALSE) && (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC)
  st_alarm_interval = sbGetFrequency() / OSAL_ST_FREQUENCY;

  /* The ST layer owns the alarm VRQ when the port-local systick is disabled. */
  __sb_vrq_seten(1U << SB_VRQ_ALARM);
  sbSetAlarm(st_alarm_interval, true);
#endif
}

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

/** @} */
