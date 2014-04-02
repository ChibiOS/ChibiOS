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
 * @file    RX62N/hal_lld.c
 * @brief   RX62N HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

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
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

}

/**
 * @brief   RX62N clocks initialization.
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
void rx62n_clock_init(void) {

  SYSTEM.SCKCR.LONG = RX62N_SCKCR_ICK | RX62N_SCKCR_PCK | RX62N_SCKCR_BCK
#if RX62N_SDCLK_OUTPUT_ENABLED == FALSE
  | (1 << 22)
#endif
#if RX62N_BCLK_OUTPUT_ENABLED == FALSE
  | (1 << 23)
#endif
  ;
}

/** @} */
