/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    STM32/hal_lld_F10x.h
 * @brief   STM32F10x HAL subsystem low level driver header.
 *
 * @addtogroup STM32F10x_HAL
 * @{
 */

#ifndef _HAL_LLD_F10X_H_
#define _HAL_LLD_F10X_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define STM32_HSICLK            8000000     /**< High speed internal clock. */
#define STM32_LSICLK            40000       /**< Low speed internal clock.  */

#define STM32_SW_HSI            (0 << 0)    /**< SYSCLK source is HSI.      */
#define STM32_SW_HSE            (1 << 0)    /**< SYSCLK source is HSE.      */
#define STM32_SW_PLL            (2 << 0)    /**< SYSCLK source is PLL.      */

#define STM32_HPRE_DIV1         (0 << 4)    /**< SYSCLK divided by 1.       */
#define STM32_HPRE_DIV2         (8 << 4)    /**< SYSCLK divided by 2.       */
#define STM32_HPRE_DIV4         (9 << 4)    /**< SYSCLK divided by 4.       */
#define STM32_HPRE_DIV8         (10 << 4)   /**< SYSCLK divided by 8.       */
#define STM32_HPRE_DIV16        (11 << 4)   /**< SYSCLK divided by 16.      */
#define STM32_HPRE_DIV64        (12 << 4)   /**< SYSCLK divided by 64.      */
#define STM32_HPRE_DIV128       (13 << 4)   /**< SYSCLK divided by 128.     */
#define STM32_HPRE_DIV256       (14 << 4)   /**< SYSCLK divided by 256.     */
#define STM32_HPRE_DIV512       (15 << 4)   /**< SYSCLK divided by 512.     */

#define STM32_PPRE1_DIV1        (0 << 8)    /**< HCLK divided by 1.         */
#define STM32_PPRE1_DIV2        (4 << 8)    /**< HCLK divided by 2.         */
#define STM32_PPRE1_DIV4        (5 << 8)    /**< HCLK divided by 4.         */
#define STM32_PPRE1_DIV8        (6 << 8)    /**< HCLK divided by 8.         */
#define STM32_PPRE1_DIV16       (7 << 8)    /**< HCLK divided by 16.        */

#define STM32_PPRE2_DIV1        (0 << 11)   /**< HCLK divided by 1.         */
#define STM32_PPRE2_DIV2        (4 << 11)   /**< HCLK divided by 2.         */
#define STM32_PPRE2_DIV4        (5 << 11)   /**< HCLK divided by 4.         */
#define STM32_PPRE2_DIV8        (6 << 11)   /**< HCLK divided by 8.         */
#define STM32_PPRE2_DIV16       (7 << 11)   /**< HCLK divided by 16.        */

#define STM32_ADCPRE_DIV2       (0 << 14)   /**< HCLK divided by 2.         */
#define STM32_ADCPRE_DIV4       (1 << 14)   /**< HCLK divided by 4.         */
#define STM32_ADCPRE_DIV6       (2 << 14)   /**< HCLK divided by 6.         */
#define STM32_ADCPRE_DIV8       (3 << 14)   /**< HCLK divided by 8.         */

#define STM32_PLLSRC_HSI        (0 << 16)   /**< PLL clock source is HSI.   */
#define STM32_PLLSRC_HSE        (1 << 16)   /**< PLL clock source is HSE.   */

#define STM32_PLLXTPRE_DIV1     (0 << 17)   /**< HSE divided by 1.          */
#define STM32_PLLXTPRE_DIV2     (1 << 17)   /**< HSE divided by 2.          */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Main clock source selection.
 * @note    If the selected clock source is not the PLL the the PLL is not
 *          initialized and started.
 */
#define STM32_SW                    STM32_SW_PLL

/**
 * @brief   Clock source for the PLL.
 * @note    This setting has only effect if the PLL is selected as the
 *          system clock source.
 */
#if !defined(STM32_PLLSRC) || defined(__DOXYGEN__)
#define STM32_PLLSRC                STM32_PLLSRC_HSE
#endif

/**
 * @brief   Crystal PLL pre-divider.
 * @note    This setting has only effect if the PLL is selected as the
 *          system clock source.
 */
#if !defined(STM32_PLLXTPRE) || defined(__DOXYGEN__)
#define STM32_PLLXTPRE              STM32_PLLXTPRE_DIV1
#endif

/**
 * @brief   Desired PLL output frequency.
 * @note    The PLL multiplier is calculated from the input clock and this
 *          value.
 * @note    This setting has only effect if the PLL is selected as the
 *          system clock source.
 */
#if !defined(STM32_PLLCLKOUT) || defined(__DOXYGEN__)
#define STM32_PLLCLKOUT             72000000
#endif

/**
 * @brief   AHB prescaler value.
 * @note    It is recommended to not change this default.
 */
#if !defined(STM32_HPRE) || defined(__DOXYGEN__)
#define STM32_HPRE                  STM32_HPRE_DIV1
#endif

/**
 * @brief   APB1 prescaler value.
 * @note    It is recommended to not change this default.
 */
#if !defined(STM32_PPRE1) || defined(__DOXYGEN__)
#define STM32_PPRE1                 STM32_PPRE1_DIV2
#endif

/**
 * @brief   APB2 prescaler value.
 * @note    It is recommended to not change this default.
 */
#if !defined(STM32_PPRE2) || defined(__DOXYGEN__)
#define STM32_PPRE2                 STM32_PPRE2_DIV2
#endif

/**
 * @brief   ADC prescaler value.
 * @note    It is recommended to not change this default.
 */
#if !defined(STM32_ADCPRE) || defined(__DOXYGEN__)
#define STM32_ADCPRE                 STM32_ADCPRE_DIV2
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* HSE prescaler setting check.*/
#if (STM32_PLLXTPRE != STM32_PLLXTPRE_DIV1) &&                              \
    (STM32_PLLXTPRE != STM32_PLLXTPRE_DIV2)
#error "invalid STM32_PLLXTPRE value specified"
#endif

/* ADC prescaler setting check.*/
#if (STM32_ADCPRE != STM32_PLLXTPRE_DIV2) &&                                \
    (STM32_ADCPRE != STM32_PLLXTPRE_DIV4) &&                                \
    (STM32_ADCPRE != STM32_PLLXTPRE_DIV6) &&                                \
    (STM32_ADCPRE != STM32_PLLXTPRE_DIV8)
#error "invalid STM32_ADCPRE value specified"
#endif

/**
 * @brief   PLL input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
#if STM32_PLLXTPRE == STM32_PLLXTPRE_DIV1
#define STM32_PLLCLKIN              (STM32_HSECLK / 1)
#else
#define STM32_PLLCLKIN              (STM32_HSECLK / 2)
#endif
#elif STM32_PLLSRC == STM32_PLLSRC_HSI
#define STM32_PLLCLKIN              (STM32_HSICLK / 2)
#else
#error "invalid STM32_PLLSRC value specified"
#endif

/**
 * @brief   PLL multiplier.
 */
#define STM32_PLLMUL                (STM32_PLLCLKOUT / STM32_PLLCLKIN)
#if (STM32_PLLMUL % 1) != 0
#error "the requested PLL output frequency is not a multiple of the input frequency"
#endif
#if (STM32_PLLMUL < 2) || (STM32_PLLMUL > 16)
#error "the calculated PLL multiplier is out of the allowed range (2...16)"
#endif

/**
 * @brief   System clock source.
 */
#if (STM32_SW == STM32_SW_PLL) || defined(__DOXYGEN__)
#define STM32_SYSCLK                STM32_PLLCLKOUT
#elif (STM32_SW == STM32_SW_HSI)
#define STM32_SYSCLK                STM32_PLLCLKOUT
#elif (STM32_SW == STM32_SW_HSE)
#define STM32_SYSCLK                STM32_PLLCLKOUT
#else
#error "invalid STM32_SYSCLK_SW value specified"
#endif

#if STM32_SYSCLK > 72000000
#error "STM32_SYSCLK above maximum rated frequency (72MHz)"
#endif

/**
 * @brief   AHB frequency.
 */
#if (STM32_HPRE == STM32_HPRE_DIV1) || defined(__DOXYGEN__)
#define STM32_HCLK                  (STM32_SYSCLK / 1)
#elif STM32_HPRE == STM32_HPRE_DIV2
#define STM32_HCLK                  (STM32_SYSCLK / 2)
#elif STM32_HPRE == STM32_HPRE_DIV4
#define STM32_HCLK                  (STM32_SYSCLK / 4)
#elif STM32_HPRE == STM32_HPRE_DIV8
#define STM32_HCLK                  (STM32_SYSCLK / 8)
#elif STM32_HPRE == STM32_HPRE_DIV16
#define STM32_HCLK                  (STM32_SYSCLK / 16)
#elif STM32_HPRE == STM32_HPRE_DIV64
#define STM32_HCLK                  (STM32_SYSCLK / 64)
#elif STM32_HPRE == STM32_HPRE_DIV128
#define STM32_HCLK                  (STM32_SYSCLK / 128)
#elif STM32_HPRE == STM32_HPRE_DIV256
#define STM32_HCLK                  (STM32_SYSCLK / 256)
#elif STM32_HPRE == STM32_HPRE_DIV512
#define STM32_HCLK                  (STM32_SYSCLK / 512)
#else
#error "invalid STM32_HPRE value specified"
#endif

#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
#define STM32_PCLK1                 (STM32_HCLK / 1)
#elif STM32_PPRE1 == STM32_PPRE1_DIV2
#define STM32_PCLK1                 (STM32_HCLK / 2)
#elif STM32_PPRE1 == STM32_PPRE1_DIV4
#define STM32_PCLK1                 (STM32_HCLK / 4)
#elif STM32_PPRE1 == STM32_PPRE1_DIV8
#define STM32_PCLK1                 (STM32_HCLK / 8)
#elif STM32_PPRE1 == STM32_PPRE1_DIV16
#define STM32_PCLK1                 (STM32_HCLK / 16)
#else
#error "invalid STM32_PPRE1 value specified"
#endif

#if (STM32_PPRE2 == STM32_PPRE2_DIV1) || defined(__DOXYGEN__)
#define STM32_PCLK2                 (STM32_HCLK / 1)
#elif STM32_PPRE2 == STM32_PPRE2_DIV2
#define STM32_PCLK2                 (STM32_HCLK / 2)
#elif STM32_PPRE2 == STM32_PPRE2_DIV4
#define STM32_PCLK2                 (STM32_HCLK / 4)
#elif STM32_PPRE2 == STM32_PPRE2_DIV8
#define STM32_PCLK2                 (STM32_HCLK / 8)
#elif STM32_PPRE2 == STM32_PPRE2_DIV16
#define STM32_PCLK2                 (STM32_HCLK / 16)
#else
#error "invalid STM32_PPRE2 value specified"
#endif

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= 24000000) || defined(__DOXYGEN__)
#define STM32_FLASHBITS             0x00000010
#elif STM32_HCLK <= 48000000
#define STM32_FLASHBITS             0x00000011
#else
#define STM32_FLASHBITS             0x00000012
#endif

#endif /* _HAL_LLD_F10X_H_ */

/** @} */
