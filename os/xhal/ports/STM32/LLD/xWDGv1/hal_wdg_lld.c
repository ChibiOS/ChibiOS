/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    xWDGv1/hal_wdg_lld.c
 * @brief   STM32 WDG subsystem low level driver source.
 *
 * @addtogroup WDG
 * @{
 */

#include "hal.h"

#if HAL_USE_WDG || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define KR_KEY_RELOAD                       0xAAAAU
#define KR_KEY_ENABLE                       0xCCCCU
#define KR_KEY_WRITE                        0x5555U

#if !defined(IWDG) && defined(IWDG1)
#define IWDG                                IWDG1
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_WDG_USE_IWDG || defined(__DOXYGEN__)
WDGDriver WDGD1;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static const WDGConfig *wdg_validate_config(const WDGConfig *config) {

  if (config == NULL) {
    return NULL;
  }
  if ((config->pr & ~STM32_IWDG_PR_MASK) != 0U) {
    return NULL;
  }
  if ((config->rlr & ~STM32_IWDG_RL_MASK) != 0U) {
    return NULL;
  }
#if STM32_IWDG_IS_WINDOWED
  if ((config->winr & ~STM32_IWDG_WIN_MASK) != 0U) {
    return NULL;
  }
#endif

  return config;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level WDG driver initialization.
 *
 * @notapi
 */
void wdg_lld_init(void) {

#if STM32_WDG_USE_IWDG
  wdgObjectInit(&WDGD1);
  WDGD1.wdg = IWDG;
#endif
}

/**
 * @brief   Configures and activates the WDG peripheral.
 *
 * @param[in,out] wdgp          Pointer to the WDG driver instance.
 * @return                      The operation status.
 *
 * @notapi
 */
msg_t wdg_lld_start(WDGDriver *wdgp) {
  const WDGConfig *config;

  osalDbgCheck(wdgp != NULL);

  config = (const WDGConfig *)wdgp->config;
  osalDbgAssert(config != NULL, "config missing");

  /* Enable IWDG and unlock for write.*/
  wdgp->wdg->KR  = KR_KEY_ENABLE;
  wdgp->wdg->KR  = KR_KEY_WRITE;

  /* Write configuration.*/
  wdgp->wdg->PR  = config->pr;
  wdgp->wdg->RLR = config->rlr;

  /* Wait the registers to be updated.*/
  while (wdgp->wdg->SR != 0U) {
  }

#if STM32_IWDG_IS_WINDOWED
  /* This also triggers a refresh.*/
  wdgp->wdg->WINR = config->winr;
#else
  wdgp->wdg->KR   = KR_KEY_RELOAD;
#endif

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the WDG peripheral.
 *
 * @param[in,out] wdgp          Pointer to the WDG driver instance.
 *
 * @notapi
 */
void wdg_lld_stop(WDGDriver *wdgp) {

  osalDbgCheck(wdgp != NULL);
  osalDbgAssert(wdgp->state == HAL_DRV_STATE_STOP,
                "IWDG cannot be stopped once activated");
}

/**
 * @brief   Applies a configuration.
 *
 * @param[in,out] wdgp          Pointer to the WDG driver instance.
 * @param[in]     config        Pointer to the configuration structure.
 * @return                      The accepted configuration or @p NULL.
 *
 * @notapi
 */
const WDGConfig *wdg_lld_setcfg(WDGDriver *wdgp, const WDGConfig *config) {

  osalDbgCheck(wdgp != NULL);

  if (wdgp->state != HAL_DRV_STATE_STOP) {
    return NULL;
  }

  return wdg_validate_config(config);
}

/**
 * @brief   Selects one of the predefined configurations.
 *
 * @param[in,out] wdgp          Pointer to the WDG driver instance.
 * @param[in]     cfgnum        Configuration selector.
 * @return                      The selected configuration or @p NULL.
 *
 * @notapi
 */
const WDGConfig *wdg_lld_selcfg(WDGDriver *wdgp, unsigned cfgnum) {

  osalDbgCheck(wdgp != NULL);
  (void)cfgnum;

  return NULL;
}

/**
 * @brief   Reloads WDG's counter.
 *
 * @param[in,out] wdgp          Pointer to the WDG driver instance.
 *
 * @notapi
 */
void wdg_lld_reset(WDGDriver *wdgp) {

  osalDbgCheck(wdgp != NULL);

  wdgp->wdg->KR = KR_KEY_RELOAD;
}

#endif /* HAL_USE_WDG */

/** @} */
