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
 * @brief   RP watchdog low level driver source.
 *
 * @addtogroup WDG
 * @{
 */

#include "hal.h"

#if (HAL_USE_WDG == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Mask of all valid bits for watchdog reset
 */
#if defined(RP2040)
#define PSM_WDSEL_ALL_BITS              0x1FFFFU
#else /* RP2350 */
#define PSM_WDSEL_ALL_BITS              0x1FFFFFFU
#endif

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

/**
 * @brief Calculates and sets the RP's watchdog LOAD register.
 *
 * @notapi
 */
static void set_wdg_counter(WDGDriver *wdgp) {

  /* Set the time in milliseconds, default to 50ms */
  uint32_t time = wdgp->config->rlr;
  time = ((time == 0U) ? 50U : time) * 1000U;

#if defined(RP2040)
  /* RP2040-E1 Errata: Watchdog counter decrements on both clock edges */
  time = time * 2U;
#endif

  /* Set ceiling if greater than count capability.*/
  time = (time > WATCHDOG_CTRL_TIME) ? WATCHDOG_CTRL_TIME : time;

  /* Set the interval.*/
  wdgp->wdg->LOAD = time;
}

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

  /* Set the watchdog counter.*/
  set_wdg_counter(wdgp);

  /* When watchdog fires, reset everything except ROSC and XOSC. */
  PSM->WDSEL = PSM_WDSEL_ALL_BITS & ~(PSM_ANY_ROSC | PSM_ANY_XOSC);

  /* Set control bits and enable WDG.*/
  wdgp->wdg->CTRL = WATCHDOG_CTRL_PAUSE_DBG0  |
                    WATCHDOG_CTRL_PAUSE_DBG1  |
                    WATCHDOG_CTRL_PAUSE_JTAG  |
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

  set_wdg_counter(wdgp);
}

#endif /* HAL_USE_WDG */

/** @} */
