/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM8L/hal_lld.h
 * @brief   STM8L HAL subsystem low level driver source.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - HSECLK (@p 0 if disabled or frequency in Hertz).
 *          - HSEBYPASS (@p TRUE if external oscillator rather than a crystal).
 *          - LSECLK (@p 0 if disabled or frequency in Hertz).
 *          - LSEBYPASS (@p TRUE if external oscillator rather than a crystal).
 *          .
 *          One of the following macros must also be defined:
 *          - STM8L15X_MD for Medium Density devices.
 *          - STM8L15X_MDP for Medium Density Plus devices.
 *          - STM8L15X_HD for High Density devices.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#undef FALSE
#undef TRUE
#include "stm8l15x.h"
#define FALSE 0
#define TRUE (!FALSE)

#if defined (STM8L15X_MD)
#include "hal_lld_stm8l_md.h"
#elif defined (STM8L15X_MDP)
#include "hal_lld_stm8l_mdp.h"
#elif defined (STM8L15X_HD)
#include "hal_lld_stm8l_hd.h"
#else
#error "unspecified, unsupported or invalid STM8L platform"
#endif

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME       "STM8L"

#define LSICLK              38000       /**< Low speed internal clock.      */
#define HSICLK              16000000    /**< High speed internal clock.     */

#define CLK_SYSSEL_HSI      1           /**< HSI system clock selector.     */
#define CLK_SYSSEL_LSI      2           /**< LSI system clock selector.     */
#define CLK_SYSSEL_HSE      4           /**< HSE system clock selector.     */
#define CLK_SYSSEL_LSE      8           /**< LSE system clock selector.     */

#define CLK_SYSCLK_DIV1     0           /**< Source clock divided by 1.     */
#define CLK_SYSCLK_DIV2     1           /**< Source clock divided by 2.     */
#define CLK_SYSCLK_DIV4     2           /**< Source clock divided by 4.     */
#define CLK_SYSCLK_DIV8     3           /**< Source clock divided by 8.     */
#define CLK_SYSCLK_DIV16    4           /**< Source clock divided by 16.    */
#define CLK_SYSCLK_DIV32    5           /**< Source clock divided by 32.    */
#define CLK_SYSCLK_DIV64    6           /**< Source clock divided by 64.    */
#define CLK_SYSCLK_DIV128   7           /**< Source clock divided by 128.   */

#define CLK_RTCSEL_HSI      1           /**< HSI RTC clock selector.        */
#define CLK_RTCSEL_LSI      2           /**< LSI RTC clock selector.        */
#define CLK_RTCSEL_HSE      4           /**< HSE RTC clock selector.        */
#define CLK_RTCSEL_LSE      8           /**< LSE RTC clock selector.        */

#define CLK_RTCCLK_DIV1     0           /**< Source clock divided by 1.     */
#define CLK_RTCCLK_DIV2     1           /**< Source clock divided by 2.     */
#define CLK_RTCCLK_DIV4     2           /**< Source clock divided by 4.     */
#define CLK_RTCCLK_DIV8     3           /**< Source clock divided by 8.     */
#define CLK_RTCCLK_DIV16    4           /**< Source clock divided by 16.    */
#define CLK_RTCCLK_DIV32    5           /**< Source clock divided by 32.    */
#define CLK_RTCCLK_DIV64    6           /**< Source clock divided by 64.    */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Disables the clock initialization in the HAL.
 */
#if !defined(STM8L_NO_CLOCK_INIT) || defined(__DOXYGEN__)
#define STM8L_NO_CLOCK_INIT         FALSE
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM8L_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM8L_HSI_ENABLED           TRUE
#endif

/**
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM8L_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM8L_LSI_ENABLED           TRUE
#endif

/**
 * @brief   Enables or disables the HSE clock source.
 */
#if !defined(STM8L_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM8L_HSE_ENABLED           FALSE
#endif

/**
 * @brief   Enables or disables the LSE clock source.
 */
#if !defined(STM8L_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM8L_LSE_ENABLED           FALSE
#endif

/**
 * @brief   System clock source selection.
 */
#if !defined(STM8L_SYSCLK_SOURCE) || defined(__DOXYGEN__)
#define STM8L_SYSCLK_SOURCE         CLK_SYSSEL_HSI
#endif

/**
 * @brief   System clock divider.
 */
#if !defined(STM8L_SYSCLK_DIVIDER) || defined(__DOXYGEN__)
#define STM8L_SYSCLK_DIVIDER        CLK_SYSCLK_DIV1
#endif

/**
 * @brief   RTC clock source selection.
 */
#if !defined(STM8L_RTCCLK_SOURCE) || defined(__DOXYGEN__)
#define STM8L_RTCCLK_SOURCE         CLK_RTCSEL_HSI
#endif

/**
 * @brief   RTC clock divider.
 */
#if !defined(STM8L_RTCCLK_DIVIDER) || defined(__DOXYGEN__)
#define STM8L_RTCCLK_DIVIDER        CLK_RTCCLK_DIV1
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV1)  &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV2)  &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV4)  &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV8)  &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV16) &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV32) &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV64) &&                           \
    (STM8L_SYSCLK_DIVIDER != CLK_SYSCLK_DIV128)
#error "specified invalid SYSCLK divider"
#endif

#if (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV1)  &&                           \
    (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV2)  &&                           \
    (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV4)  &&                           \
    (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV8)  &&                           \
    (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV16) &&                           \
    (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV32) &&                           \
    (STM8L_RTCCLK_DIVIDER != CLK_RTCCLK_DIV64)
#error "specified invalid RTCCLK divider"
#endif

#if STM8L_HSE_ENABLED && (HSECLK == 0)
#error "impossible to activate HSE"
#endif

#if STM8L_LSE_ENABLED && (LSECLK == 0)
#error "impossible to activate LSE"
#endif

#if !STM8L_HSI_ENABLED && ((STM8L_SYSCLK_SOURCE == CLK_SYSSEL_HSI) ||       \
                           (STM8L_RTCCLK_SOURCE == CLK_RTCSEL_HSI))
#error "requested HSI clock is not enabled"
#endif

#if !STM8L_LSI_ENABLED && ((STM8L_SYSCLK_SOURCE == CLK_SYSSEL_LSI) ||       \
                           (STM8L_RTCCLK_SOURCE == CLK_RTCSEL_LSI))
#error "requested LSI clock is not enabled"
#endif

#if !STM8L_HSE_ENABLED && ((STM8L_SYSCLK_SOURCE == CLK_SYSSEL_HSE) ||       \
                           (STM8L_RTCCLK_SOURCE == CLK_RTCSEL_HSE))
#error "requested HSE clock is not enabled"
#endif

#if !STM8L_LSE_ENABLED && ((STM8L_SYSCLK_SOURCE == CLK_SYSSEL_LSE) ||       \
                           (STM8L_RTCCLK_SOURCE == CLK_RTCSEL_LSE))
#error "requested LSE clock is not enabled"
#endif

/**
 * @brief   System clock.
 */
#if STM8L_NO_CLOCK_INIT || defined(__DOXYGEN__)
#define SYSCLK                      (HSICLK / 8)
#elif STM8L_SYSCLK_SOURCE == CLK_SYSSEL_HSI
#define SYSCLK                      (HSICLK / (1 << STM8L_SYSCLK_DIVIDER))
#elif STM8L_SYSCLK_SOURCE == CLK_SYSSEL_LSI
#define SYSCLK                      (LSICLK / (1 << STM8L_SYSCLK_DIVIDER))
#elif STM8L_SYSCLK_SOURCE == CLK_SYSSEL_HSE
#define SYSCLK                      (HSECLK / (1 << STM8L_SYSCLK_DIVIDER))
#elif STM8L_SYSCLK_SOURCE == CLK_SYSSEL_LSE
#define SYSCLK                      (LSECLK / (1 << STM8L_SYSCLK_DIVIDER))
#else
#error "specified invalid SYSCLK source"
#endif

/**
 * @brief   RTC clock.
 */
#if STM8L_NO_CLOCK_INIT || defined(__DOXYGEN__)
#define RTCCLK                      0
#elif STM8L_RTCCLK_SOURCE == CLK_RTCSEL_HSI
#define RTCCLK                      (HSICLK / (1 << STM8L_RTCCLK_DIVIDER))
#elif STM8L_RTCCLK_SOURCE == CLK_RTCSEL_LSI
#define RTCCLK                      (LSICLK / (1 << STM8L_RTCCLK_DIVIDER))
#elif STM8L_RTCCLK_SOURCE == CLK_RTCSEL_HSE
#define RTCCLK                      (HSECLK / (1 << STM8L_RTCCLK_DIVIDER))
#elif STM8L_RTCCLK_SOURCE == CLK_RTCSEL_LSE
#define RTCCLK                      (LSECLK / (1 << STM8L_RTCCLK_DIVIDER))
#else
#error "specified invalid RTCCLK source"
#endif

/**
 * @brief   CPU clock.
 * @details On the STM8L the CPU clock is always equal to the system clock.
 */
#define CPUCLK                      SYSCLK

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
