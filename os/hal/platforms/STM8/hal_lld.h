/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM8/hal_lld.h
 * @brief   STM8 HAL subsystem low level driver source.
 *
 * @addtogroup STM8_HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "stm8.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define LSICLK              128000      /**< Low speed internal clock.      */
#define HSICLK              16000000    /**< High speed internal clock.     */

#define CLK_SOURCE_DEFAULT  0           /**< No clock initialization.       */
#define CLK_SOURCE_HSI      0xE1        /**< HSI clock selector.            */
#define CLK_SOURCE_LSI      0xD2        /**< LSI clock selector.            */
#define CLK_SOURCE_HSE      0xB4        /**< HSE clock selector.            */

#define CLK_HSI_DIV1        0           /**< HSI clock divided by 1.        */
#define CLK_HSI_DIV2        1           /**< HSI clock divided by 2.        */
#define CLK_HSI_DIV4        2           /**< HSI clock divided by 4.        */
#define CLK_HSI_DIV8        3           /**< HSI clock divided by 8.        */

#define CLK_CPU_DIV1        0           /**< CPU clock divided by 1.        */
#define CLK_CPU_DIV2        1           /**< CPU clock divided by 2.        */
#define CLK_CPU_DIV4        2           /**< CPU clock divided by 4.        */
#define CLK_CPU_DIV8        3           /**< CPU clock divided by 8.        */
#define CLK_CPU_DIV16       4           /**< CPU clock divided by 16.       */
#define CLK_CPU_DIV32       5           /**< CPU clock divided by 32.       */
#define CLK_CPU_DIV64       6           /**< CPU clock divided by 64.       */
#define CLK_CPU_DIV128      7           /**< CPU clock divided by 128.      */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Clock source setting.
 */
#if !defined(STM8_CLOCK_SOURCE) || defined(__DOXYGEN__)
#define STM8_CLOCK_SOURCE           CLK_SOURCE_DEFAULT
#endif

/**
 * @brief HSI clock divider.
 */
#if !defined(STM8_HSI_DIVIDER) || defined(__DOXYGEN__)
#define STM8_HSI_DIVIDER            CLK_HSI_DIV8
#endif

/**
 * @brief CPU clock divider.
 */
#if !defined(STM8_CPU_DIVIDER) || defined(__DOXYGEN__)
#define STM8_CPU_DIVIDER            CLK_CPU_DIV1
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (STM8_HSI_DIVIDER != CLK_HSI_DIV1)  &&                                  \
    (STM8_HSI_DIVIDER != CLK_HSI_DIV2)  &&                                  \
    (STM8_HSI_DIVIDER != CLK_HSI_DIV4)  &&                                  \
    (STM8_HSI_DIVIDER != CLK_HSI_DIV8)
#error "specified invalid HSI divider"
#endif

#if (STM8_CPU_DIVIDER != CLK_CPU_DIV1)  &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV2)  &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV4)  &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV8)  &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV16) &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV32) &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV64) &&                                  \
    (STM8_CPU_DIVIDER != CLK_CPU_DIV128)
#error "specified invalid CPU divider"
#endif

#if STM8_CLOCK_SOURCE == CLK_SOURCE_DEFAULT
#define SYSCLK                      (HSICLK / 8)
#elif STM8_CLOCK_SOURCE == CLK_SOURCE_HSI
#define SYSCLK                      (HSICLK / (1 << STM8_HSI_DIVIDER))
#elif STM8_CLOCK_SOURCE == CLK_SOURCE_LSI
#define SYSCLK                      LSICLK
#elif STM8_CLOCK_SOURCE == CLK_SOURCE_HSE
#define SYSCLK                      HSECLK
#else
#error "specified invalid clock source"
#endif

#define CPUCLK                      (SYSCLK / (1 << STM8_CPU_DIVIDER))

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
