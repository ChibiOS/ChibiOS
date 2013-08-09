/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    common/ARMCMx/nvic.c
 * @brief   Cortex-Mx NVIC support code.
 *
 * @addtogroup COMMON_ARMCMx_NVIC
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
 * @brief   Sets the priority of an interrupt handler and enables it.
 * @note    The parameters are not tested for correctness.
 *
 * @param[in] n         the interrupt number
 * @param[in] prio      the interrupt priority mask
 */
void nvicEnableVector(IRQn_Type n, uint32_t prio) {

  NVIC->IP[n]        = NVIC_PRIORITY_MASK(prio);
  NVIC->ICPR[n >> 5] = 1 << (n & 0x1F);
  NVIC->ISER[n >> 5] = 1 << (n & 0x1F);
}

/**
 * @brief   Disables an interrupt handler.
 * @note    The parameters are not tested for correctness.
 *
 * @param[in] n         the interrupt number
 */
void nvicDisableVector(IRQn_Type n) {

  NVIC->ICER[n >> 5] = 1 << (n & 0x1F);
  NVIC->IP[n]        = 0;
}

/**
 * @brief   Changes the priority of a system handler.
 * @note    The parameters are not tested for correctness.
 *
 * @param[in] handler   the system handler number
 * @param[in] prio      the system handler priority mask
 */
void nvicSetSystemHandlerPriority(IRQn_Type handler, uint32_t prio) {

  SCB->SHP[((uint32_t)(handler) & 15) - 4] = NVIC_PRIORITY_MASK(prio);
}

/** @} */
