/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio
              Copyright (C) 2017 Fabien Poussin (fabien.poussin (at) google's mail)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in sdramliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_sdram.c
 * @brief   SDRAM Driver code.
 *
 * @addtogroup SDRAM
 * @{
 */

#include "hal.h"

#if HAL_USE_SDRAM || defined(__DOXYGEN__)


/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

#include "hal_sdram_lld.h"

/**
 * @brief   SDRAM Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sdramInit(void) {
  fsmcInit();
  sdramObjectInit(&SDRAMD1);
}

/**
 * @brief   Initializes the standard part of a @p SDRAMDriver structure.
 *
 * @param[out] sdramp     pointer to the @p SDRAMDriver object
 *
 * @init
 */
void sdramObjectInit(SDRAMDriver *sdramp) {
  
  sdramp->sdram = FSMCD1.sdram;
  sdramp->state  = SDRAM_STOP;
}

/**
 * @brief   Configures and activates the SDRAM peripheral.
 *
 * @param[in] sdramp      pointer to the @p SDRAMDriver object
 * @param[in] config    pointer to the @p SDRAMConfig object
 *
 * @api
 */
void sdramStart(SDRAMDriver *sdramp, const SDRAMConfig *config) {

  osalDbgCheck((sdramp != NULL) && (config != NULL));

  if (FSMCD1.state == FSMC_STOP)
    fsmcStart(&FSMCD1);

  osalSysLock();
  osalDbgAssert((sdramp->state == SDRAM_STOP) || (sdramp->state == SDRAM_READY),
              "invalid state");
  sdram_lld_start(sdramp, config);
  sdramp->state = SDRAM_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the SDRAM peripheral.
 *
 * @param[in] sdramp      pointer to the @p SDRAMDriver object
 *
 * @api
 */
void sdramStop(SDRAMDriver *sdramp) {

  osalDbgCheck(sdramp != NULL);

  osalSysLock();
  osalDbgAssert((sdramp->state == SDRAM_STOP) || (sdramp->state == SDRAM_READY),
              "invalid state");
  sdram_lld_stop(sdramp);
  sdramp->state = SDRAM_STOP;
  osalSysUnlock();
}

#endif /* HAL_USE_SDRAM */

/** @} */
