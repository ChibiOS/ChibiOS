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
#define SAMA_PIT                            (SAMA_MCK / 16 / SAMA_H64MX_H32MX_RATIO)

#if (SAMA_ST_USE_TC0 == TRUE) || (SAMA_ST_USE_TC1 == TRUE)
/**
 * @brief   Enable write protection on TC registers block.
 *
 * @param[in] tc    pointer to a TC
 *
 * @notapi
 */
#define tcEnableWP(tc) {                                                     \
  tc->TC_WPMR = TC_WPMR_WPKEY_PASSWD | TC_WPMR_WPEN;                         \
}

/**
 * @brief   Disable write protection on TC registers block.
 *
 * @param[in] tc    pointer to a TC
 *
 * @notapi
 */
#define tcDisableWP(tc) {                                                     \
  tc->TC_WPMR = TC_WPMR_WPKEY_PASSWD;                                         \
}
#endif

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

#if (SAMA_ST_USE_TC0)
OSAL_IRQ_HANDLER(SAMA_ST_TC0_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  if (((TC0->TC_CHANNEL[0].TC_SR & TC_SR_CPCS) != 0) &&
      ((TC0->TC_CHANNEL[0].TC_IMR & TC_IMR_CPCS) != 0)) {
    osalSysLockFromISR();
    osalOsTimerHandlerI();
    osalSysUnlockFromISR();
  }
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if (SAMA_ST_USE_TC1)
OSAL_IRQ_HANDLER(SAMA_ST_TC1_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  if (((TC1->TC_CHANNEL[0].TC_SR & TC_SR_CPCS) != 0) &&
      ((TC1->TC_CHANNEL[0].TC_IMR & TC_IMR_CPCS) != 0)) {
    osalSysLockFromISR();
    osalOsTimerHandlerI();
    osalSysUnlockFromISR();
  }
  aicAckInt();
  OSAL_IRQ_EPILOGUE();
}
#endif

#if (SAMA_ST_USE_PIT == TRUE)
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
#endif /* SAMA_ST_USE_PIT == TRUE */

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

#if (SAMA_ST_USE_TC0 == TRUE)
  pmcEnableTC0();
  aicSetSourcePriority(ID_TC0, SAMA_TC0_IRQ_PRIORITY);
  aicSetSourceHandler(ID_TC0, SAMA_ST_TC0_HANDLER);
  aicEnableInt(ID_TC0);

  tcDisableWP(TC0);
  uint32_t rc = (SAMA_TC0CLK) / (OSAL_ST_FREQUENCY);
  TC0->TC_CHANNEL[0].TC_EMR = TC_EMR_NODIVCLK;
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_ACPA_SET |
                              TC_CMR_ACPC_CLEAR | TC_CMR_WAVSEL_UP_RC;
  TC0->TC_CHANNEL[0].TC_RC = TC_RC_RC(rc);
  TC0->TC_CHANNEL[0].TC_RA = TC_RA_RA(rc);
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
  TC0->TC_CHANNEL[0].TC_SR;                       /* Clear pending IRQs.          */
  TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
  tcEnableWP(TC0);
#endif /* SAMA_ST_USE_TC0 == TRUE */

#if (SAMA_ST_USE_TC1 == TRUE)
  pmcEnableTC1();
  aicSetSourcePriority(ID_TC1, SAMA_TC1_IRQ_PRIORITY);
  aicSetSourceHandler(ID_TC1, SAMA_ST_TC1_HANDLER);
  aicEnableInt(ID_TC1);

  tcDisableWP(TC1);
  uint32_t rc = (SAMA_TC1CLK) / (OSAL_ST_FREQUENCY);
  TC1->TC_CHANNEL[0].TC_EMR = TC_EMR_NODIVCLK;
  TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_ACPA_SET |
                              TC_CMR_ACPC_CLEAR | TC_CMR_WAVSEL_UP_RC;
  TC1->TC_CHANNEL[0].TC_RC = TC_RC_RC(rc);
  TC1->TC_CHANNEL[0].TC_RA = TC_RA_RA(rc);
  TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN;
  TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
  TC1->TC_CHANNEL[0].TC_SR;                       /* Clear pending IRQs.          */
  TC1->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
  tcEnableWP(TC1);
#endif /* SAMA_ST_USE_TC1 == TRUE */

#if (SAMA_ST_USE_PIT == TRUE)
  /* Enabling PIT.*/
  pmcEnablePIT();

  PIT->PIT_MR = PIT_MR_PIV((SAMA_PIT / OSAL_ST_FREQUENCY) - 1);
  PIT->PIT_MR |= PIT_MR_PITEN | PIT_MR_PITIEN;
  (void) PIT->PIT_PIVR;    /* reset PIT PICNT counter */

  /* IRQ enabled.*/
  aicSetSourcePriority(ID_PIT, SAMA_ST_IRQ_PRIORITY);
  aicSetSourceHandler(ID_PIT, PIT_Handler);
  aicEnableInt(ID_PIT);
#endif /* SAMA_ST_USE_PIT == TRUE */

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

/** @} */
