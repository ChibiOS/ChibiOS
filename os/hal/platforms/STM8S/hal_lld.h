/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM8S/hal_lld.h
 * @brief   STM8S HAL subsystem low level driver source.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - HSECLK (@p 0 if disabled or frequency in Hertz).
 *          .
 *          One of the following macros must also be defined:
 *          - STM8S103.
 *          - STM8S105.
 *          - STM8S207.
 *          - STM8S208.
 *          - STM8S903.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#undef FALSE
#undef TRUE

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S105) ||          \
    defined(STM8S103) || defined(STM8S903)
#include "stm8s.h"
#else
#error "unsupported or invalid STM8 platform"
#endif

#define FALSE 0
#define TRUE (!FALSE)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME       "STM8S"

#define LSICLK              128000      /**< Low speed internal clock.      */
#define HSICLK              16000000    /**< High speed internal clock.     */

#define CLK_SYSSEL_HSI      0xE1        /**< HSI clock selector.            */
#define CLK_SYSSEL_LSI      0xD2        /**< LSI clock selector.            */
#define CLK_SYSSEL_HSE      0xB4        /**< HSE clock selector.            */

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
 * @brief   Disables the clock initialization in the HAL.
 */
#if !defined(STM8S_NO_CLOCK_INIT) || defined(__DOXYGEN__)
#define STM8S_NO_CLOCK_INIT         FALSE
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM8S_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM8S_HSI_ENABLED           FALSE
#endif

/**
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM8S_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM8S_LSI_ENABLED           TRUE
#endif

/**
 * @brief   Enables or disables the HSE clock source.
 */
#if !defined(STM8S_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM8S_HSE_ENABLED           TRUE
#endif

/**
 * @brief   Clock source setting.
 */
#if !defined(STM8S_SYSCLK_SOURCE) || defined(__DOXYGEN__)
#define STM8S_SYSCLK_SOURCE         CLK_SYSSEL_HSE
#endif

/**
 * @brief   HSI clock divider.
 */
#if !defined(STM8S_HSI_DIVIDER) || defined(__DOXYGEN__)
#define STM8S_HSI_DIVIDER           CLK_HSI_DIV8
#endif

/**
 * @brief   CPU clock divider.
 */
#if !defined(STM8S_CPU_DIVIDER) || defined(__DOXYGEN__)
#define STM8S_CPU_DIVIDER           CLK_CPU_DIV1
#endif

/**
 * @brief   bxCAN divider value.
 */
#if !defined(STM8S_CAN_DIVIDER_VALUE) || defined(__DOXYGEN__)
#define STM8S_CAN_DIVIDER_VALUE     1
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (STM8S_HSI_DIVIDER != CLK_HSI_DIV1)  &&                                 \
    (STM8S_HSI_DIVIDER != CLK_HSI_DIV2)  &&                                 \
    (STM8S_HSI_DIVIDER != CLK_HSI_DIV4)  &&                                 \
    (STM8S_HSI_DIVIDER != CLK_HSI_DIV8)
#error "specified invalid HSI divider"
#endif

#if (STM8S_CPU_DIVIDER != CLK_CPU_DIV1)  &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV2)  &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV4)  &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV8)  &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV16) &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV32) &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV64) &&                                 \
    (STM8S_CPU_DIVIDER != CLK_CPU_DIV128)
#error "specified invalid CPU divider"
#endif

#if (STM8S_CAN_DIVIDER_VALUE < 1) || (STM8S_CAN_DIVIDER_VALUE > 8)
#error "specified invalid CAN divider value"
#endif

#if STM8S_HSE_ENABLED && (HSECLK == 0)
#error "impossible to activate HSE"
#endif

#if !STM8S_HSI_ENABLED && (STM8S_SYSCLK_SOURCE == CLK_SYSSEL_HSI)
#error "requested HSI clock is not enabled"
#endif

#if !STM8S_LSI_ENABLED && (STM8S_SYSCLK_SOURCE == CLK_SYSSEL_LSI)
#error "requested LSI clock is not enabled"
#endif

#if !STM8S_HSE_ENABLED && (STM8S_SYSCLK_SOURCE == CLK_SYSSEL_HSE)
#error "requested HSE clock is not enabled"
#endif

/**
 * @brief   System clock.
 */
#if STM8SL_NO_CLOCK_INIT || defined(__DOXYGEN__)
#define SYSCLK                      (HSICLK / 8)
#elif STM8S_SYSCLK_SOURCE == CLK_SYSSEL_HSI
#define SYSCLK                      (HSICLK / (1 << STM8S_HSI_DIVIDER))
#elif STM8S_SYSCLK_SOURCE == CLK_SYSSEL_LSI
#define SYSCLK                      LSICLK
#elif STM8S_SYSCLK_SOURCE == CLK_SYSSEL_HSE
#define SYSCLK                      HSECLK
#else
#error "specified invalid clock source"
#endif

/**
 * @brief   CPU clock.
 * @details On the STM8SS the CPU clock can be programmed to be a fraction of
 *          the system clock.
 */
#define CPUCLK                      (SYSCLK / (1 << STM8S_CPU_DIVIDER))

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
