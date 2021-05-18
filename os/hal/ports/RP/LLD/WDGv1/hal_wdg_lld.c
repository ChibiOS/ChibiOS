/*
    ChibiOS - Copyright (C) 2006..2021 Giovanni Di Sirio

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
 * @file    hal_wdg_lld.c
 * @brief   RP2040 watchdog low level driver source.
 *
 * @addtogroup WDG
 * @{
 */

#include "hal.h"

#if (HAL_USE_WDG == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

WDGDriver WDGD1;

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
 * @brief   Low level WDG driver initialization.
 * @note    The watchdog tick provides clocking to the TIMER block.
 *          The tick is initialised and started in system clock setup.
 *
 * @notapi
 */
void wdg_lld_init(void) {

  WDGD1.state = WDG_STOP;
  WDGD1.wdg   = WATCHDOG;
#if WDG_HAS_STORAGE
  WDGD1.scratch = (uint8_t *)WDGD1.wdg->SCRATCH;
#endif
  WDGD1.wdg->CTRL &= ~WATCHDOG_CTRL_ENABLE;
}

/**
 * @brief   Configures and activates the WDG peripheral.
 *
 * @param[in] wdgp      pointer to the @p WDGDriver object
 *
 * @notapi
 */
void wdg_lld_start(WDGDriver *wdgp) {

  /* Set the time. */
  uint32_t time = wdgp->config->rlr;

  /* Due to a silicon bug (see errata RP2040-E1) WDG decrements at each edge. */
  time = ((time == 0U) ? 50 : time) * 2 * 1000;

  /* Set ceiling if greater than count capability. */
  time = (time > WATCHDOG_CTRL_TIME) ? WATCHDOG_CTRL_TIME : time;

  /* Set the initial interval, resets, control bits and enable WDG. */
  wdgp->wdg->LOAD = time;
  //PSM_SET->WDSEL = PSM_ANY_PROC1 | PSM_ANY_PROC0;
  wdgp->wdg->CTRL = WATCHDOG_CTRL_PAUSE_DBG0 |
                    WATCHDOG_CTRL_PAUSE_DBG1 |
                    WATCHDOG_CTRL_PAUSE_JTAG |
                    WATCHDOG_CTRL_ENABLE;
}

/**
 * @brief   Deactivates the WDG peripheral.
 *
 * @param[in] wdgp      pointer to the @p WDGDriver object
 *
 * @notapi
 */
void wdg_lld_stop(WDGDriver *wdgp) {

  wdgp->wdg->CTRL &= ~WATCHDOG_CTRL_ENABLE;
}

/**
 * @brief   Reloads WDG's counter.
 *
 * @param[in] wdgp      pointer to the @p WDGDriver object
 *
 * @notapi
 */
void wdg_lld_reset(WDGDriver * wdgp) {

  wdgp->wdg->LOAD = wdgp->config->rlr;
}

#endif /* HAL_USE_WDG */

/** @} */
