/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    RP2350/rp_xosc.c
 * @brief   RP2350 XOSC source.
 *
 * @addtogroup RP_XOSC
 * @{
 */

#include "hal.h"
#include "hal_safety.h"
#include "rp_xosc.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Timeout for XOSC stabilization in microseconds.
 * @note    Crystal startup typically takes 1-2 ms. We allow up to 100 ms
 *          to account for slow crystals and ROSC timing variance.
 */
#define RP_XOSC_STABLE_TIMEOUT_US       100000U

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

/**
 * @brief   Initializes the XOSC
 * @note    Assumes 1-15 MHz crystal is attached.
 * @note    See RP2350 Datasheet 8.2 XOSC
 */
void rp_xosc_init(void) {

  /* Set frequency range */
  XOSC->CTRL = XOSC_CTRL_FREQ_RANGE_1_15MHZ;

  /* Set startup delay */
  XOSC->STARTUP = RP_XOSC_STARTUP_DELAY;

  /* Enable the XOSC */
  XOSC->SET.CTRL = XOSC_CTRL_ENABLE_ENABLE;

  /* Wait for XOSC to be stable with timeout protection. */
  if (halRegWaitAnySet32X((volatile uint32_t *)&XOSC->STATUS, XOSC_STATUS_STABLE,
                          RP_XOSC_STABLE_TIMEOUT_US, NULL)) {
    halSftFail("XOSC stable timeout");
  }
}

/**
 * @brief   Disable the XOSC
 * @pre     The XOSC must not be the current source for CLK_REF.
 */
void rp_xosc_disable(void) {

  /* Disable XOSC */
  XOSC->XOR.CTRL = (XOSC->CTRL ^ XOSC_CTRL_ENABLE_DISABLE) & XOSC_CTRL_ENABLE_Msk;
}

/** @} */
