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
 * @file    SAMA5D2x/aic.c
 * @brief   SAMA AIC support code.
 *
 * @addtogroup COMMON_SAMA5D2x_AIC
 * @{
 */

#include "hal.h"

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
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   AIC Initialization.
 * @note    Better reset everything in the AIC.
 *
 * @notapi
 */
void aicInit(void) {

  Aic *aic = SAIC;

  unsigned i;
  /* Disable all interrupts */
  for (i = 1; i < ID_PERIPH_COUNT; i++) {
    aic->AIC_SSR = i;
    aic->AIC_IDCR = AIC_IDCR_INTD;
  }
  /* Clear All pending interrupts flags */
  for (i = 0; i < ID_PERIPH_COUNT; i++) {
    aic->AIC_SSR = i;
    aic->AIC_ICCR = AIC_ICCR_INTCLR;
  }
}

/**
 * @brief   Configures an interrupt in the AIC.
 *
 * @param[in] source    interrupt source to configure
 * @param[in] prior     priority level of the source selected
 *                      by INTSEL except FIQ source (source 0).
 */
void aicConfigureInt(uint32_t source, uint8_t prior) {

  Aic *aic = SAIC;

  /* Disable write protection */
  aicDisableWP(aic);
  /* Set source id */
  aic->AIC_SSR = source;
  /* Disable the interrupt first */
  aic->AIC_IDCR = AIC_IDCR_INTD;
  /* Configure priority */
  aic->AIC_SMR = AIC_SMR_PRIOR(prior);
  /* Clear interrupt */
  aic->AIC_ICCR = AIC_ICCR_INTCLR;
  /* Enable write protection */
  aicEnableWP(aic);
}

/**
 * @brief   Sets the source vector of an interrupt.
 *
 * @param[in] source    interrupt source to configure
 * @param[in] handler   handler for the interrupt source selected
 */
void aicSetSourceVector(uint32_t source, bool (*handler)(void)) {

  Aic *aic = SAIC;

  /* Disable write protection */
  aicDisableWP(aic);
  /* Select source and assign handler */
  aic->AIC_SSR = AIC_SSR_INTSEL(source);
  aic->AIC_SVR = (uint32_t)handler;
  /* Enable write protection */
  aicEnableWP(aic);
}

/**
 * @brief   Sets the spurious vector of an interrupt.
 *
 * @param[in] handler   handler for the interrupt
 */
void aicSetSpuriousVector(bool (*handler)(void)) {

  Aic *aic = SAIC;

  /* Disable write protection */
  aicDisableWP(aic);
  /* Assign handler */
  aic->AIC_SPU = (uint32_t)handler;
  /* Enable write protection */
  aicEnableWP(aic);
}

/**
 * @brief   Enables interrupts coming from the source.
 *
 * @param[in] source    interrupt source to enable
 */
void aicEnableInt(uint32_t source) {

  Aic *aic = SAIC;

  aic->AIC_SSR = AIC_SSR_INTSEL(source);
  aic->AIC_IECR = AIC_IECR_INTEN;
}

/**
 * @brief   Disables interrupts coming from the selected source.
 *
 * @param[in] source    interrupt source to disable
 */
void aicDisableInt(uint32_t source) {

  Aic *aic = SAIC;

  aic->AIC_SSR = AIC_SSR_INTSEL(source);
  aic->AIC_IDCR = AIC_IDCR_INTD;
}

/**
 * @brief   Clears interrupts coming from the selected source.
 *
 * @param[in] source    interrupt source to Clear
 */
void aicClearInt(uint32_t source) {

  Aic *aic = SAIC;

  aic->AIC_SSR = AIC_SSR_INTSEL(source);
  aic->AIC_ICCR = AIC_ICCR_INTCLR;
}

/**
 * @brief   Sets interrupts coming from the selected source.
 *
 * @param[in] source    interrupt source to Set
 */
void aicSetInt(uint32_t source) {

  Aic *aic = SAIC;

  aic->AIC_SSR = AIC_SSR_INTSEL(source);
  aic->AIC_ISCR = AIC_ISCR_INTSET;
}

/** @} */
