/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio
              Copyright (C) 2017 Fabien Poussin (fabien.poussin (at) google's mail)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in sramliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_sram.c
 * @brief   SRAM Driver code.
 *
 * @addtogroup SRAM
 * @{
 */

#include "hal.h"

#if HAL_USE_SRAM || defined(__DOXYGEN__)


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

#include "hal_sram_lld.h"

/**
 * @brief   SRAM Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sramInit(void) {

  fsmcInit();

#if STM32_SRAM_USE_SRAM1
  SRAMD1.sram = FSMCD1.sram1;
  SRAMD1.state = SRAM_STOP;
  sramObjectInit(&SRAMD1);
#endif /* STM32_SRAM_USE_SRAM1 */

#if STM32_SRAM_USE_SRAM2
  SRAMD2.sram = FSMCD1.sram2;
  SRAMD2.state = SRAM_STOP;
  sramObjectInit(&SRAMD2);
#endif /* STM32_SRAM_USE_SRAM2 */

#if STM32_SRAM_USE_SRAM3
  SRAMD3.sram = FSMCD1.sram3;
  SRAMD3.state = SRAM_STOP;
  sramObjectInit(&SRAMD3);
#endif /* STM32_SRAM_USE_SRAM3 */

#if STM32_SRAM_USE_SRAM4
  SRAMD4.sram = FSMCD1.sram4;
  SRAMD4.state = SRAM_STOP;
  sramObjectInit(&SRAMD4);
#endif /* STM32_SRAM_USE_SRAM4 */

}

/**
 * @brief   Initializes the standard part of a @p SRAMDriver structure.
 *
 * @param[out] sramp     pointer to the @p SRAMDriver object
 *
 * @init
 */
void sramObjectInit(SRAMDriver *sramp) {

  sramp->state  = SRAM_STOP;
}

/**
 * @brief   Configures and activates the SRAM peripheral.
 *
 * @param[in] sramp      pointer to the @p SRAMDriver object
 * @param[in] config    pointer to the @p SRAMConfig object
 *
 * @api
 */
void sramStart(SRAMDriver *sramp, const SRAMConfig *config) {

  osalDbgCheck((sramp != NULL) && (config != NULL));

  if (FSMCD1.state == FSMC_STOP)
    fsmcStart(&FSMCD1);

  osalSysLock();
  osalDbgAssert((sramp->state == SRAM_STOP) || (sramp->state == SRAM_READY),
              "invalid state");
  sram_lld_start(sramp, config);
  sramp->state = SRAM_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the SRAM peripheral.
 *
 * @param[in] sramp      pointer to the @p SRAMDriver object
 *
 * @api
 */
void sramStop(SRAMDriver *sramp) {

  osalDbgCheck(sramp != NULL);

  osalSysLock();
  osalDbgAssert((sramp->state == SRAM_STOP) || (sramp->state == SRAM_READY),
              "invalid state");
  sram_lld_stop(sramp);
  sramp->state = SRAM_STOP;
  osalSysUnlock();
}

#endif /* HAL_USE_SRAM */

/** @} */
