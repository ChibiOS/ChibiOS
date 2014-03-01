/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx HAL driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

#include "LPC17xx.h"
#include "nvic.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS TRUE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME           "LPC17xx"

#define IRCOSCCLK               4000000UL	/**< High speed internal clock. */

#define CLKSRCSEL_IRCOSC        0UL      	/**< Clock source is IRC.       */
#define CLKSRCSEL_MAINOSC       1UL         /**< Clock source is Main oscillator.   */
#define CLKSRCSEL_RTCOSC        2UL         /**< Clock source is RTC oscillator.    */

#define PCLKSEL_CCLK_DIV_4      0UL         /**< Peripheral clock source is CCLK/4  */
#define PCLKSEL_CCLK            1UL         /**< Peripheral clock source is CCLK    */
#define PCLKSEL_CCLK_DIV_2      2UL         /**< Peripheral clock source is CCLK/2  */
#define PCLKSEL_CCLK_DIV_8      3UL         /**< Peripheral clock source is CCLK/8  */
#define PCLKSEL_MASK            3UL

#define CLKOUTSEL_CCLK          0UL         /**< Clock output is CPU clock.         */
#define CLKOUTSEL_MAINOSC       1UL         /**< Clock output is Main oscillator.   */
#define CLKOUTSEL_IRCOSC        2UL         /**< Clock output is IRC oscillator.    */
#define CLKOUTSEL_USBCLK        3UL         /**< Clock output is USB clock.         */
#define CLKOUTSEL_RTCOSC        4UL         /**< Clock output is RTC oscillator.    */

#define PCLKSEL_CCLK_DIV_4      0UL         /**< Peripheral clock output is CCLK/4  */
#define PCLKSEL_CCLK            1UL         /**< Peripheral clock output is CCLK    */
#define PCLKSEL_CCLK_DIV_2      2UL         /**< Peripheral clock output is CCLK/2  */
#define PCLKSEL_CCLK_DIV_8      3UL         /**< Peripheral clock output is CCLK/8  */

#define USBSEL_PLL0_DIV_6       5UL         /**< USB clock source is PLL0/6.        */
#define USBSEL_PLL0_DIV_7       7UL         /**< USB clock source is PLL0/7.        */
#define USBSEL_PLL0_DIV_9       9UL         /**< USB clock source is PLL0/9.        */
/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Main oscillator enable.
 */
#if !defined(LPC17xx_MAINOSC_ENABLE) || defined(__DOXYGEN__)
#define LPC17xx_MAINOSC_ENABLE              TRUE
#endif

/**
 * @brief   System PLL clock source select.
 */
#if !defined(LPC17xx_SYSCLK_SELECT) || defined(__DOXYGEN__)
#define LPC17xx_SYSCLK_SELECT               CLKSRCSEL_MAINOSC
#endif

/**
 * @brief   Main PLL enable.
 */
#if !defined(LPC17xx_MAINPLL_ENABLE) || defined(__DOXYGEN__)
#define LPC17xx_MAINPLL_ENABLE              TRUE
#endif

/**
 * @brief   Main PLL multiplier.
 * @note    Final frequency must not exceed the CCO ratings.
 */
#if !defined(LPC17xx_MAINPLL_MUL) || defined(__DOXYGEN__)
#define LPC17xx_MAINPLL_MUL                 200
#endif

/**
 * @brief   Main PLL pre-divider.
 * @note    The value must be in the 1..32 range and the final frequency
 *          must not exceed the CCO ratings.
 */
#if !defined(LPC17xx_MAINPLL_PREDIV) || defined(__DOXYGEN__)
#define LPC17xx_MAINPLL_PREDIV              6
#endif

/**
 * @brief   USB PLL enable.
 */
#if !defined(LPC17xx_USBPLL_ENABLE) || defined(__DOXYGEN__)
#define LPC17xx_USBPLL_ENABLE               FALSE
#endif

/**
 * @brief   USB PLL multiplier.
 * @note    The value must be in the 1..32 range and the final frequency
 *          must not exceed the CCO ratings and USB clock must be equal 48MHz.
 */
#if !defined(LPC17xx_USBPLL_MUL) || defined(__DOXYGEN__)
#define LPC17xx_USBPLL_MUL                  4
#endif

/**
 * @brief   USB PLL divider.
 * @note    The value must be 2, 4, 8 or 16 and the final frequency
 *          must not exceed the CCO ratings and USB clock must be equal 48MHz.
 */
#if !defined(LPC17xx_USBPLL_DIV) || defined(__DOXYGEN__)
#define LPC17xx_USBPLL_DIV                  4
#endif

/**
 * @brief   CPU clock divider.
 * @note    The value must be chosen between (1...255).
 */
#if !defined(LPC17xx_CCLK_DIV) || defined(__DOXYGEN__)
#define LPC17xx_CCLK_DIV                    4
#endif

/**
 * @brief   PCLK clock select.
 */
#if !defined(LPC17xx_PCLK_SELECT) || defined(__DOXYGEN__)
#define LPC17xx_PCLK_SELECT                 PCLKSEL_CCLK
#endif

/**
 * @brief   Clock output enable.
 * @note
 */
#if !defined(LPC17xx_CLKOUT_ENABLE) || defined(__DOXYGEN__)
#define LPC17xx_CLKOUT_ENABLE               FALSE
#endif

/**
 * @brief   Clock output divider.
 * @note    The value must be chosen between (1...16).
 */
#if !defined(LPC17xx_CLKOUT_DIV) || defined(__DOXYGEN__)
#define LPC17xx_CLKOUT_DIV                  4
#endif

/**
 * @brief   Clock output clock source select.
 */
#if !defined(LPC17xx_CLKOUT_SELECT) || defined(__DOXYGEN__)
#define LPC17xx_CLKOUT_SELECT               CLKOUTSEL_MAINOSC
#endif

/**
 * @brief   USB clock PPL0 clock source select.
 * @note    PLL0 output must be 288MHz (USBSEL_PLL0_DIV_6), 384MHz (USBSEL_PLL0_DIV_8) or
 * 			480MHz(USBSEL_PLL0_DIV_10). Only is used when USB PLL (PLL1) disable.
 */
#if !defined(LPC17xx_USBCLKPLL0_SELECT) || defined(__DOXYGEN__)
#define LPC17xx_USBCLKPLL0_SELECT           USBSEL_PLL0_DIV_6
#endif
/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   Calculated OSCRANGE setting.
 */
#if (MAINOSCCLK < 18000000) || defined(__DOXYGEN__)
#define LPC17xx_OSCRANGE        0
#else
#define LPC17xx_OSCRANGE        1
#endif

/**
 * @brief   PLL input clock frequency.
 */
#if (LPC17xx_SYSCLK_SELECT == CLKSRCSEL_IRCOSC) || defined(__DOXYGEN__)
#define LPC17xx_SYSCLK          IRCOSCCLK
#elif LPC17xx_SYSCLK_SELECT == CLKSRCSEL_MAINOSC
#define LPC17xx_SYSCLK          MAINOSCCLK
#elif LPC17xx_SYSCLK_SELECT == CLKSRCSEL_RTCOSC
#define LPC17xx_SYSCLK          RTCOSCCLK
#else
#error "Invalid LPC17xx_SYSCLK_SELECT clock source specified."
#endif

/**
 * @brief   MSEL mask in SYSPLLCTRL register.
 */
#if ((LPC17xx_MAINPLL_MUL % 2) == 0) || defined(__DOXYGEN__)
#define LPC17xx_PLL0CFG_MSEL0 ((LPC17xx_MAINPLL_MUL/2) - 1)
#else
#error "Invalid LPC17xx_PLL0CFG_MUL value."
#endif

/**
 * @brief   PSEL mask in SYSPLLCTRL register.
 */
#if ((LPC17xx_MAINPLL_PREDIV >= 1) && (LPC17xx_MAINPLL_PREDIV <= 32)) || defined(__DOXYGEN__)
#define LPC17xx_PLL0CFG_NSEL0 (LPC17xx_MAINPLL_PREDIV - 1)
#else
#error "Invalid LPC17xx_MAINPLL_PREDIV value (1 to 32 accepted)."
#endif

/**
 * @brief   CCO frequency.
 */
#define  LPC17xx_MAINPLLCCO   ((LPC17xx_MAINPLL_MUL *    \
                              LPC17xx_SYSCLK)/LPC17xx_MAINPLL_PREDIV)

#if (LPC17xx_MAINPLLCCO < 275000000) || (LPC17xx_SYSPLLCCO > 550000000)
#error "CCO frequency out of the acceptable range (275...550)."
#endif

/**
 * @brief   PLL output clock frequency.
 */
#if LPC17xx_MAINPLL_ENABLE
#define  LPC17xx_MAINPLLCLK   LPC17xx_MAINPLLCCO
#else
#define  LPC17xx_MAINPLLCLK   LPC17xx_SYSCLK
#endif

/**
 * @brief   CPU clock frequency.
 * @note 	Most of LPC17xx have max 120 MHz clock.
 */
#define LPC17xx_CCLK     (LPC17xx_MAINPLLCLK/LPC17xx_CCLK_DIV)
#if (LPC17xx_CCLK > 120000000) || defined(__DOXYGEN__)
#error "CPU Clock out of range."
#endif

#if LPC17xx_USBPLL_ENABLE
/**
 * @brief   Main oscillator out of range.
 */
#if ((MAINOSCCLK < 10000000) && (MAINOSCCLK > 25000000)) || defined(__DOXYGEN__)
#error "Main oscillator clock out of range."
#endif

/**
 * @brief   MSEL1 mask in PLL1CFG register.
 */
#if (LPC17xx_USBPLL_MUL >= 1) && (LPC17xx_USBPLL_MUL <= 32) || defined(__DOXYGEN__)
#define LPC17xx_PLL1CFG_MSEL1 (LPC17xx_USBPLL_MUL - 1)
#else
#error "Invalid LPC17xx_USBPLL_MUL value (1 to 32 accepted)."
#endif

/**
 * @brief   PSEL1 mask in PLL1CFG register.
 */
#if (LPC17xx_USBPLL_DIV == 2) || defined(__DOXYGEN__)
#define LPC17xx_PLL1CFG_PSEL1 		0UL
#elif (LPC17xx_USBPLL_DIV == 4)
#define LPC17xx_PLL1CFG_PSEL1 		1UL
#elif (LPC17xx_USBPLL_DIV == 8)
#define LPC17xx_PLL1CFG_PSEL1 		2UL
#elif (LPC17xx_USBPLL_DIV == 16)
#define LPC17xx_PLL1CFG_PSEL1 		3UL
#else
#error "Invalid LPC17xx_USBPLL_DIV value (2, 4, 8, 16 accepted)."
#endif

/**
 * @brief   USB PLL CCO frequency.
 */
#define  LPC17xx_USBPLLCCO   (MAINOSCCLK * LPC17xx_USBPLL_MUL * \
								LPC17xx_USBPLL_DIV)

#if (LPC17xx_USBPLLCCO < 156000000) || (LPC17xx_SYSPLLCCO > 320000000)
#error "CCO frequency out of the acceptable range (156...320)"
#endif

/**
 * @brief   USB clock frequency.
 * @note 	Must be 48 MHz.
 */
#define LPC17xx_USBCLK     (LPC17xx_USBPLLCCO/LPC17xx_USBPLL_DIV)
#if (LPC17xx_USBCLK != 48000000) || defined(__DOXYGEN__)
#error "USB clock out of range."
#endif
#endif

/**
 * @brief   Peripheral clock frequency.
 */
#if (LPC17xx_PCLK_SELECT == PCLKSEL_CCLK_DIV_4) || defined(__DOXYGEN__)
#define LPC17xx_PCLK            (LPC17xx_CCLK/4)
#define LPC17xx_PCLKSEL0        0x00
#define LPC17xx_PCLKSEL1        0x00
#elif (LPC17xx_PCLK_SELECT == PCLKSEL_CCLK)
#define LPC17xx_PCLK            (LPC17xx_CCLK)
#define LPC17xx_PCLKSEL0        0x55515155
#define LPC17xx_PCLKSEL1        0x54555455
#elif (LPC17xx_PCLK_SELECT == PCLKSEL_CCLK_DIV_2)
#define LPC17xx_PCLK            (LPC17xx_CCLK/2)
#define LPC17xx_PCLKSEL0        0xAAA2A2AA
#define LPC17xx_PCLKSEL1        0xA8AAA8AA
#elif (LPC17xx_PCLK_SELECT == PCLKSEL_CCLK_DIV_8)
#define LPC17xx_PCLK            (LPC17xx_CCLK/8)
#define LPC17xx_PCLKSEL0        0xFFF3F3FF
#define LPC17xx_PCLKSEL1        0xFCFFFCFF
#else
#error "Invalid LPC17xx_PCLK_SELECT value"
#endif

/**
 * @brief   LPC17xx_CLKOUT_DIV out of range.
 */
#if ((LPC17xx_CLKOUT_DIV < 1) && (LPC17xx_CLKOUT_DIV > 16)) || defined(__DOXYGEN__)
#error "Invalid LPC17xx_CLKOUT_DIV value (1 to 16 accepted)."
#endif

/**
 * @brief   CLKOUT frequency.
 */
#if (LPC17xx_CLKOUT_SELECT == CLKOUTSEL_CCLK) || defined(__DOXYGEN__)
#define LPC17xx_CLKOUTCLK (LPC17xx_CCLK/LPC17xx_CLKOUT_DIV)
#elif (LPC17xx_CLKOUT_SELECT == CLKOUTSEL_MAINOSC)
#define LPC17xx_CLKOUTCLK (MAINOSCCLK/LPC17xx_CLKOUT_DIV)
#elif (LPC17xx_CLKOUT_SELECT == CLKOUTSEL_IRCOSC)
#define LPC17xx_CLKOUTCLK (IRCOSCCLK/LPC17xx_CLKOUT_DIV)
#elif (LPC17xx_CLKOUT_SELECT == CLKOUTSEL_USBCLK)
#define LPC17xx_CLKOUTCLK (LPC17xx_USBCLK/LPC17xx_CLKOUT_DIV)
#elif (LPC17xx_CLKOUT_SELECT == CLKOUTSEL_RTCOSC)
#define LPC17xx_CLKOUTCLK (RTCOSCCLK/LPC17xx_CLKOUT_DIV)
#else
#error "Invalid LPC17xx_CLKOUT_SELECT value."
#endif

/**
 * @brief   CLKOUT frequency out of range.
 */
#if (LPC17xx_CLKOUTCLK > 50000000) || defined(__DOXYGEN__)
#error "CLKOUT frequency out of the acceptable range (less than 50 MHz)"
#endif

/**
 * @brief   Flash wait states.
 */
#if (LPC17xx_CCLK <= 20000000) || defined(__DOXYGEN__)
#define LPC17xx_FLASHCFG_FLASHTIM   0UL
#elif LPC17xx_CCLK <= 40000000
#define LPC17xx_FLASHCFG_FLASHTIM   1UL
#elif LPC17xx_CCLK <= 60000000
#define LPC17xx_FLASHCFG_FLASHTIM   2UL
#elif LPC17xx_CCLK <= 80000000
#define LPC17xx_FLASHCFG_FLASHTIM   3UL
#elif LPC17xx_CCLK <= 120000000
#define LPC17xx_FLASHCFG_FLASHTIM   4UL
#else
#define LPC17xx_FLASHCFG_FLASHTIM   5UL
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of the realtime free counter value.
 */
typedef uint32_t halrtcnt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the current value of the system free running counter.
 * @note    This service is implemented by returning the content of the
 *          DWT_CYCCNT register.
 *
 * @return              The value of the system free running counter of
 *                      type halrtcnt_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_value()         DWT_CYCCNT

/**
 * @brief   Realtime counter frequency.
 * @note    The DWT_CYCCNT register is incremented directly by the cpu
 *          clock so this function returns LPC17xx_CCLK.
 *
 * @return              The realtime counter frequency of type halclock_t.
 *
 * @notapi
 */
#define hal_lld_get_counter_frequency()     LPC17xx_CCLK

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#include "lpc17xx_dma.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void LPC17xx_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
