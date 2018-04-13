/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    SAMA5D2x/sama_trng.c
 * @brief   SAMA TRNG support code.
 *
 * @addtogroup SAMA5D2x_TRNG
 * @{
 */

#include "hal.h"

#if (HAL_USE_TRNG) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
TRNGDriver TRNGD0;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level TRNG driver initialization.
 *
 * @notapi
 */
void trng_lld_init(void) {
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_TRNG, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  TRNGD0.state    = TRNG_STOP;
  TRNGD0.trng     = TRNG;
}

/**
 * @brief   Configures and activates the TRNG peripheral.
 *
 * @param[in] trngp     pointer to the @p TRNGDriver object
 *
 * @notapi
 */
void trng_lld_start(TRNGDriver *trngp) {

  /* Configures the peripheral. */
  if (trngp->state == TRNG_STOP) {

    /* Enable the TRNG peripheral clock. */
    pmcEnableTRNG0();

    /* Enable the TRNG. */
    trngp->trng->TRNG_CR = TRNG_CR_ENABLE | TRNG_CR_KEY_PASSWD;
  }
}

/**
 * @brief   Deactivates the TRNG peripheral.
 *
 * @param[in] trngp     pointer to the @p TRNGDriver object
 *
 * @notapi
 */
void trng_lld_stop(TRNGDriver *trngp) {

  if (trngp->state == TRNG_READY) {
    /* Disable the TRNG. */
    trngp->trng->TRNG_CR = TRNG_CR_KEY_PASSWD;
    /* Disable the TRNG clock. */
    pmcDisableTRNG0();
  }
}

/**
 * @brief   Get random number from TRNG.
 *
 * @param[in] trngp     pointer to the @p TRNGDriver object
 * @return    TRNG_ODATA  content of the TRNG_ODATA register
 *
 * @notapi
 */
uint32_t trng_lld_get_random_number(TRNGDriver *trngp) {

  while (!(trngp->trng->TRNG_ISR & TRNG_ISR_DATRDY));
  return trngp->trng->TRNG_ODATA;
}

/**
 * @brief   TRNG driver initialization.
 *
 * @notapi
 */
void trngInit(void) {

  trng_lld_init();
}

/**
 * @brief   Configures and activates the TRNG peripheral.
 *
 * @param[in] trngp     pointer to the @p TRNGDriver object
 *
 * @api
 */
void trngStart(TRNGDriver *trngp) {

  osalDbgCheck(trngp != NULL);

  osalSysLock();
  osalDbgAssert((trngp->state == TRNG_STOP) ||
                (trngp->state == TRNG_READY), "invalid state");
  trng_lld_start(trngp);
  trngp->state = TRNG_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the TRNG peripheral.
 *
 * @param[in] trngp    pointer to the @p TRNGDriver object
 *
 * @api
 */
void trngStop(TRNGDriver *trngp) {

  osalDbgCheck(trngp != NULL);

  osalSysLock();
  osalDbgAssert((trngp->state == TRNG_STOP) ||
                (trngp->state == TRNG_READY), "invalid state");

  trng_lld_stop(trngp);
  trngp->state = TRNG_STOP;
  osalSysUnlock();
}

/**
 * @brief   Get random number from TRNG.
 *
 * @param[in] trngp     pointer to the @p CLASSDDriver object
 * @return    num       random number generated
 *
 * @api
 */
uint32_t trngGetRandomNumber(TRNGDriver *trngp) {

  osalDbgCheck(trngp != NULL);

  uint32_t num = trng_lld_get_random_number(trngp);
  return num;
}

#endif /* HAL_USE_TRNG == TRUE */

/** @} */
