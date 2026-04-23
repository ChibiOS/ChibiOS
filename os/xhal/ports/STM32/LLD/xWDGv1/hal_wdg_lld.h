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
 * @file    xWDGv1/hal_wdg_lld.h
 * @brief   STM32 WDG subsystem low level driver header.
 *
 * @addtogroup WDG
 * @{
 */

#ifndef HAL_WDG_LLD_H
#define HAL_WDG_LLD_H

#if HAL_USE_WDG || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    RLR register definitions
 * @{
 */
#define STM32_IWDG_RL_MASK                  (0x00000FFFU << 0)
#define STM32_IWDG_RL(n)                    ((n) << 0)
/** @} */

/**
 * @name    PR register definitions
 * @{
 */
#define STM32_IWDG_PR_MASK                  (7U << 0)
#define STM32_IWDG_PR_4                     0U
#define STM32_IWDG_PR_8                     1U
#define STM32_IWDG_PR_16                    2U
#define STM32_IWDG_PR_32                    3U
#define STM32_IWDG_PR_64                    4U
#define STM32_IWDG_PR_128                   5U
#define STM32_IWDG_PR_256                   6U
/** @} */

/**
 * @name    WINR register definitions
 * @{
 */
#define STM32_IWDG_WIN_MASK                 (0x00000FFFU << 0)
#define STM32_IWDG_WIN(n)                   ((n) << 0)
#define STM32_IWDG_WIN_DISABLED             STM32_IWDG_WIN(0x00000FFFU)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if !defined(STM32_WDG_USE_IWDG) || defined(__DOXYGEN__)
#define STM32_WDG_USE_IWDG                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_IWDG)
#define STM32_HAS_IWDG                      FALSE
#endif

#if !defined(STM32_IWDG_IS_WINDOWED)
#define STM32_IWDG_IS_WINDOWED              FALSE
#endif

#if STM32_WDG_USE_IWDG && !STM32_HAS_IWDG
#error "IWDG not present in the selected device"
#endif

#if !STM32_WDG_USE_IWDG
#error "WDG driver activated but no xWDG peripheral assigned"
#endif

#if !defined(STM32_LSI_ENABLED)
#error "STM32_LSI_ENABLED not defined"
#endif

#if (STM32_WDG_USE_IWDG == TRUE) && (STM32_LSI_ENABLED == FALSE)
#error "IWDG requires LSI clock"
#endif

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the WDG driver structure.
 */
#define wdg_lld_driver_fields                                               \
  IWDG_TypeDef              *wdg

/**
 * @brief   Low level fields of the WDG configuration structure.
 */
#if STM32_IWDG_IS_WINDOWED || defined(__DOXYGEN__)
#define wdg_lld_config_fields                                               \
  uint32_t                  pr;                                              \
  uint32_t                  rlr;                                             \
  uint32_t                  winr
#else
#define wdg_lld_config_fields                                               \
  uint32_t                  pr;                                              \
  uint32_t                  rlr
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_WDG_USE_IWDG && !defined(__DOXYGEN__)
extern WDGDriver WDGD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void wdg_lld_init(void);
  msg_t wdg_lld_start(WDGDriver *wdgp);
  void wdg_lld_stop(WDGDriver *wdgp);
  const WDGConfig *wdg_lld_setcfg(WDGDriver *wdgp, const WDGConfig *config);
  const WDGConfig *wdg_lld_selcfg(WDGDriver *wdgp, unsigned cfgnum);
  void wdg_lld_reset(WDGDriver *wdgp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_WDG */

#endif /* HAL_WDG_LLD_H */

/** @} */
