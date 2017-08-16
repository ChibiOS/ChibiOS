/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    SAMA5D2x/hal_st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Periodic Interrupt Timer frequency.
 */
#define SAMA_PIT                            (SAMA_MCK / 16)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)
/**
 * @brief   System Timer vector.
 * @details This interrupt is used for system tick in periodic mode.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(PIT_Handler) {
  uint32_t ivr;
  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  ivr = PIT->PIT_PIVR;    /* acknowledge PIT interrupt */
  osalDbgAssert((ivr & PIT_PIVR_PICNT_Msk) == (1 << PIT_PIVR_PICNT_Pos),
      "check for lost tick");
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC)
  /* Enabling PIT.*/
  pmcEnablePIT();

  PIT->PIT_MR = PIT_MR_PIV((SAMA_PIT / OSAL_ST_FREQUENCY) - 1);
  PIT->PIT_MR |= PIT_MR_PITEN | PIT_MR_PITIEN;

  /* IRQ enabled.*/
  aicSetSourcePriority(ID_PIT, SAMA_ST_IRQ_PRIORITY);
  aicSetSourceHandler(ID_PIT, PIT_Handler);
  aicEnableInt(ID_PIT);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

/** @} */
