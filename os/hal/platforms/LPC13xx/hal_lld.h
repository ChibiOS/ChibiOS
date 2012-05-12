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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    LPC13xx/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "LPC13xx.h"
#include "nvic.h"

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
#define PLATFORM_NAME           "LPC13xx"

#define IRCOSCCLK               12000000    /**< High speed internal clock. */
#define WDGOSCCLK               1600000     /**< Watchdog internal clock.   */

#define SYSPLLCLKSEL_IRCOSC     0           /**< Internal RC oscillator
                                                 clock source.              */
#define SYSPLLCLKSEL_SYSOSC     1           /**< System oscillator clock
                                                 source.                    */

#define SYSMAINCLKSEL_IRCOSC    0           /**< Clock source is IRC.       */
#define SYSMAINCLKSEL_PLLIN     1           /**< Clock source is PLLIN.     */
#define SYSMAINCLKSEL_WDGOSC    2           /**< Clock source is WDGOSC.    */
#define SYSMAINCLKSEL_PLLOUT    3           /**< Clock source is PLLOUT.    */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   System PLL clock source.
 */
#if !defined(LPC13xx_PLLCLK_SOURCE) || defined(__DOXYGEN__)
#define LPC13xx_PLLCLK_SOURCE               SYSPLLCLKSEL_SYSOSC
#endif

/**
 * @brief   System PLL multiplier.
 * @note    The value must be in the 1..32 range and the final frequency
 *          must not exceed the CCO ratings.
 */
#if !defined(LPC13xx_SYSPLL_MUL) || defined(__DOXYGEN__)
#define LPC13xx_SYSPLL_MUL                  6
#endif

/**
 * @brief   System PLL divider.
 * @note    The value must be chosen between (2, 4, 8, 16).
 */
#if !defined(LPC13xx_SYSPLL_DIV) || defined(__DOXYGEN__)
#define LPC13xx_SYSPLL_DIV                  4
#endif

/**
 * @brief   System main clock source.
 */
#if !defined(LPC13xx_MAINCLK_SOURCE) || defined(__DOXYGEN__)
#define LPC13xx_MAINCLK_SOURCE              SYSMAINCLKSEL_PLLOUT
#endif

/**
 * @brief   AHB clock divider.
 * @note    The value must be chosen between (1...255).
 */
#if !defined(LPC13xx_SYSCLK_DIV) || defined(__DOXYGEN__)
#define LPC13xx_SYSABHCLK_DIV               1
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   Calculated SYSOSCCTRL setting.
 */
#if (SYSOSCCLK < 18000000) || defined(__DOXYGEN__)
#define LPC13xx_SYSOSCCTRL      0
#else
#define LPC13xx_SYSOSCCTRL      1
#endif

/**
 * @brief   PLL input clock frequency.
 */
#if (LPC13xx_PLLCLK_SOURCE == SYSPLLCLKSEL_SYSOSC) || defined(__DOXYGEN__)
#define LPC13xx_SYSPLLCLKIN     SYSOSCCLK
#elif LPC13xx_PLLCLK_SOURCE == SYSPLLCLKSEL_IRCOSC
#define LPC13xx_SYSPLLCLKIN     IRCOSCCLK
#else
#error "invalid LPC13xx_PLLCLK_SOURCE clock source specified"
#endif

/**
 * @brief   MSEL mask in SYSPLLCTRL register.
 */
#if (LPC13xx_SYSPLL_MUL >= 1) && (LPC13xx_SYSPLL_MUL <= 32) ||              \
    defined(__DOXYGEN__)
#define LPC13xx_SYSPLLCTRL_MSEL (LPC13xx_SYSPLL_MUL - 1)
#else
#error "LPC13xx_SYSPLL_MUL out of range (1...32)"
#endif

/**
 * @brief   PSEL mask in SYSPLLCTRL register.
 */
#if (LPC13xx_SYSPLL_DIV == 2) || defined(__DOXYGEN__)
#define LPC13xx_SYSPLLCTRL_PSEL (0 << 5)
#elif LPC13xx_SYSPLL_DIV == 4
#define LPC13xx_SYSPLLCTRL_PSEL (1 << 5)
#elif LPC13xx_SYSPLL_DIV == 8
#define LPC13xx_SYSPLLCTRL_PSEL (2 << 5)
#elif LPC13xx_SYSPLL_DIV == 16
#define LPC13xx_SYSPLLCTRL_PSEL (3 << 5)
#else
#error "invalid LPC13xx_SYSPLL_DIV value (2,4,8,16)"
#endif

/**
 * @brief   CCP frequency.
 */
#define  LPC13xx_SYSPLLCCO   (LPC13xx_SYSPLLCLKIN * LPC13xx_SYSPLL_MUL *    \
                              LPC13xx_SYSPLL_DIV)

#if (LPC13xx_SYSPLLCCO < 156000000) || (LPC13xx_SYSPLLCCO > 320000000)
#error "CCO frequency out of the acceptable range (156...320)"
#endif

/**
 * @brief   PLL output clock frequency.
 */
#define  LPC13xx_SYSPLLCLKOUT   (LPC13xx_SYSPLLCCO / LPC13xx_SYSPLL_DIV)

#if (LPC13xx_MAINCLK_SOURCE == SYSMAINCLKSEL_IRCOSC) || defined(__DOXYGEN__)
#define LPC13xx_MAINCLK     IRCOSCCLK
#elif LPC13xx_MAINCLK_SOURCE == SYSMAINCLKSEL_PLLIN
#define LPC13xx_MAINCLK     LPC13xx_SYSPLLCLKIN
#elif LPC13xx_MAINCLK_SOURCE == SYSMAINCLKSEL_WDGOSC
#define LPC13xx_MAINCLK     WDGOSCCLK
#elif LPC13xx_MAINCLK_SOURCE == SYSMAINCLKSEL_PLLOUT
#define LPC13xx_MAINCLK     LPC13xx_SYSPLLCLKOUT
#else
#error "invalid LPC13xx_MAINCLK_SOURCE clock source specified"
#endif

/**
 * @brief   AHB clock.
 */
#define  LPC13xx_SYSCLK     (LPC13xx_MAINCLK / LPC13xx_SYSABHCLK_DIV)
#if LPC13xx_SYSCLK > 72000000
#error "AHB clock frequency out of the acceptable range (72MHz max)"
#endif

/**
 * @brief   Flash wait states.
 */
#if (LPC13xx_SYSCLK <= 20000000) || defined(__DOXYGEN__)
#define LPC13xx_FLASHCFG_FLASHTIM   0
#elif LPC13xx_SYSCLK <= 40000000
#define LPC13xx_FLASHCFG_FLASHTIM   1
#else
#define LPC13xx_FLASHCFG_FLASHTIM   2
#endif

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
  void LPC13xx_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
