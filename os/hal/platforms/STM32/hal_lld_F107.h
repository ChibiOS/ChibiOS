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
 * @file    STM32/hal_lld_F107.h
 * @brief   STM32F107 HAL subsystem low level driver header.
 *
 * @addtogroup STM32F107_HAL
 * @{
 */

#ifndef _HAL_LLD_F107_H_
#define _HAL_LLD_F107_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define STM32_HSICLK            8000000     /**< High speed internal clock. */
#define STM32_LSICLK            40000       /**< Low speed internal clock.  */

/* RCC_CFGR register bits definitions.*/
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

#define STM32_PLL1SRC_HSI       (0 << 16)   /**< PLL1 clock source is HSI.  */
#define STM32_PLL1SRC_PREDIV1   (1 << 16)   /**< PLL1 clock source is
                                                 PREDIV1.                   */

#define STM32_PLL1MUL_MUL4      (2 << 18)   /**< PLL2CLK multiplied by 4.   */
#define STM32_PLL1MUL_MUL5      (3 << 18)   /**< PLL2CLK multiplied by 5.   */
#define STM32_PLL1MUL_MUL6      (4 << 18)   /**< PLL2CLK multiplied by 6.   */
#define STM32_PLL1MUL_MUL7      (5 << 18)   /**< PLL2CLK multiplied by 7.   */
#define STM32_PLL1MUL_MUL8      (6 << 18)   /**< PLL2CLK multiplied by 8.   */
#define STM32_PLL1MUL_MUL9      (7 << 18)   /**< PLL2CLK multiplied by 9.   */
#define STM32_PLL1MUL_MUL6P5    (13 << 18)  /**< PLL2CLK multiplied by 6.5. */

#define STM32_OTGFSPRE_DIV2     (1 << 22)   /**< HCLK*2 divided by 2.       */
#define STM32_OTGFSPRE_DIV3     (0 << 22)   /**< HCLK*2 divided by 3.       */

/* RCC_CFGR2 register bits definitions.*/
#define STM32_PREDIV1_DIV(n)    ((n) << 0)  /**< PREDIV1 divided by n.      */
#define STM32_PREDIV2_DIV(n)    ((n) << 4)  /**< PREDIV2 divided by n.      */

#define STM32_PREDIV1SRC_HSE    (0 << 16)   /**< PREDIV1 source is HSE.     */
#define STM32_PREDIV1SRC_PLL2   (1 << 16)   /**< PREDIV1 source is PLL2.    */

#define STM32_PLL2MUL_MUL8      (6 << 8)    /**< PLL2CLK multiplied by 8.   */
#define STM32_PLL2MUL_MUL9      (7 << 8)    /**< PLL2CLK multiplied by 9.   */
#define STM32_PLL2MUL_MUL10     (8 << 8)    /**< PLL2CLK multiplied by 10.  */
#define STM32_PLL2MUL_MUL11     (9 << 8)    /**< PLL2CLK multiplied by 11.  */
#define STM32_PLL2MUL_MUL12     (10 << 8)   /**< PLL2CLK multiplied by 12.  */
#define STM32_PLL2MUL_MUL13     (11 << 8)   /**< PLL2CLK multiplied by 13.  */
#define STM32_PLL2MUL_MUL14     (12 << 8)   /**< PLL2CLK multiplied by 14.  */
#define STM32_PLL2MUL_MUL16     (14 << 8)   /**< PLL2CLK multiplied by 16.  */
#define STM32_PLL2MUL_MUL20     (15 << 8)   /**< PLL2CLK multiplied by 20.  */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Main clock source selection.
 * @note    If the selected clock source is not the PLL1 then the PLL1 is not
 *          initialized and started.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_SW) || defined(__DOXYGEN__)
#define STM32_SW                    STM32_SW_PLL
#endif

/**
 * @brief   Clock source for the PLL1.
 * @note    This setting has only effect if the PLL1 is selected as the
 *          system clock source.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_PLL1SRC) || defined(__DOXYGEN__)
#define STM32_PLL1SRC                STM32_PLL1SRC_PREDIV1
#endif

/**
 * @brief   PREDIV1 clock source.
 * @note    This setting has only effect if the PLL1 is selected as the
 *          system clock source.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_PREDIV1SCR) || defined(__DOXYGEN__)
#define STM32_PREDIV1SRC            STM32_PREDIV1SRC_PLL2
#endif

/**
 * @brief   PREDIV1 division factor.
 * @note    This setting has only effect if the PLL1 is selected as the
 *          system clock source.
 * @note    The allowed range is 1...16.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_PREDIV1) || defined(__DOXYGEN__)
#define STM32_PREDIV1               5
#endif

/**
 * @brief   PLL1 multiplier value.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_PLL1MUL) || defined(__DOXYGEN__)
#define STM32_PLL1MUL               STM32_PLL1MUL_MUL9
#endif

/**
 * @brief   PREDIV2 division factor.
 * @note    This setting has only effect if the PLL2 is selected as the
 *          clock source for the PLL1.
 * @note    The allowed range is 1...16.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_PREDIV2) || defined(__DOXYGEN__)
#define STM32_PREDIV2               5
#endif

/**
 * @brief   PLL2 multiplier value.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 25MHz crystal using both PLL1 and PLL2.
 */
#if !defined(STM32_PLL2MUL) || defined(__DOXYGEN__)
#define STM32_PLL2MUL               STM32_PLL2MUL_MUL8
#endif

/**
 * @brief   AHB prescaler value.
 * @note    The default value is calculated for a 72MHz system clock from
 *          a 8MHz crystal using the PLL.
 */
#if !defined(STM32_HPRE) || defined(__DOXYGEN__)
#define STM32_HPRE                  STM32_HPRE_DIV1
#endif

/**
 * @brief   APB1 prescaler value.
 */
#if !defined(STM32_PPRE1) || defined(__DOXYGEN__)
#define STM32_PPRE1                 STM32_PPRE1_DIV2
#endif

/**
 * @brief   APB2 prescaler value.
 */
#if !defined(STM32_PPRE2) || defined(__DOXYGEN__)
#define STM32_PPRE2                 STM32_PPRE2_DIV2
#endif

/**
 * @brief   ADC prescaler value.
 */
#if !defined(STM32_ADCPRE) || defined(__DOXYGEN__)
#define STM32_ADCPRE                 STM32_ADCPRE_DIV4
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* PREDIV1 prescaler setting check.*/
#if (STM32_PREDIV1 < 1) || (STM32_PREDIV1 > 16)
#error "invalid STM32_PREDIV1 value specified"
#endif

/* PREDIV2 prescaler setting check.*/
#if (STM32_PREDIV2 < 1) || (STM32_PREDIV2 > 16)
#error "invalid STM32_PREDIV2 value specified"
#endif

/**
 * @brief   PLL2 input frequency.
 */
#define STM32_PLL2CLKIN             (STM32_HSECLK / STM32_PREDIV2)

/* PLL2 input frequency range check.*/
#if (STM32_PLL2CLKIN < 3000000) || (STM32_PLL2CLKIN > 5000000)
#error "STM32_PLL2CLKIN outside acceptable range (3...5MHz)"
#endif

/**
 * @brief   PLL2 output clock frequency.
 */
#define STM32_PLL2CLKOUT            (STM32_PLL2CLKIN * STM32_PLL2MUL)

/* PLL2 output frequency range check.*/
#if (STM32_PLL2CLKOUT < 40000000) || (STM32_PLL2CLKOUT > 74000000)
#error "STM32_PLL2CLKOUT outside acceptable range (40...74MHz)"
#endif

/**
 * @brief   PREDIV1 input frequency.
 */
#if (STM32_PREDIV1SRC == STM32_PREDIV1SRC_PLL2) || defined(__DOXYGEN__)
#define STM32_PREDIV1CLK            STM32_HSECLK
#elif STM32_PREDIV1SRC == STM32_PREDIV1SRC_HSE
#define STM32_PREDIV1CLK            STM32_PLL2CLKOUT
#else
#endif

/**
 * @brief   PLL1 input clock frequency.
 */
#if (STM32_PLL1SRC == STM32_PLL1SRC_PREDIV1) || defined(__DOXYGEN__)
#define STM32_PLL1CLKIN             (STM32_PREDIV1CLK / STM32_PREDIV1)
#elif STM32_PLLSRC == STM32_PLLSRC_HSI
#define STM32_PLL1CLKIN             (STM32_HSICLK / 2)
#else
#error "invalid STM32_PLL1SRC value specified"
#endif

/* PLL1 input frequency range check.*/
#if (STM32_PLL1CLKIN < 3000000) || (STM32_PLL1CLKIN > 12000000)
#error "STM32_PLL1CLKIN outside acceptable range (3...12MHz)"
#endif

/**
 * @brief   PLL1 output clock frequency.
 */
#define STM32_PLL1CLKOUT            (STM32_PLL1CLKIN * STM32_PLL1MUL)

/* PLL1 output frequency range check.*/
#if (STM32_PLL1CLKOUT < 18000000) || (STM32_PLL1CLKOUT > 72000000)
#error "STM32_PLL1CLKOUT outside acceptable range (18...72MHz)"
#endif

/**
 * @brief   System clock source.
 */
#if (STM32_SW == STM32_SW_PLL) || defined(__DOXYGEN__)
#define STM32_SYSCLK                STM32_PLL1CLKOUT
#elif (STM32_SW == STM32_SW_HSI)
#define STM32_SYSCLK                STM32_HSICLK
#elif (STM32_SW == STM32_SW_HSE)
#define STM32_SYSCLK                STM32_HSECLK
#else
#error "invalid STM32_SYSCLK_SW value specified"
#endif

/* Check on the system clock.*/
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

/* AHB frequency check.*/
#if STM32_HPRE > 72000000
#error "STM32_HPRE exceeding maximum frequency (72MHz)"
#endif

/**
 * @brief   APB1 frequency.
 */
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

/* APB1 frequency check.*/
#if STM32_PCLK2 > 36000000
#error "STM32_PCLK1 exceeding maximum frequency (36MHz)"
#endif

/**
 * @brief   APB2 frequency.
 */
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

/* APB2 frequency check.*/
#if STM32_PCLK2 > 72000000
#error "STM32_PCLK2 exceeding maximum frequency (72MHz)"
#endif

/**
 * @brief   ADC frequency.
 */
#if (STM32_ADCPRE == STM32_ADCPRE_DIV2) || defined(__DOXYGEN__)
#define STM32_ADCCLK                (STM32_PCLK2 / 2)
#elif STM32_ADCPRE == STM32_ADCPRE_DIV4
#define STM32_ADCCLK                (STM32_PCLK2 / 4)
#elif STM32_ADCPRE == STM32_ADCPRE_DIV6
#define STM32_ADCCLK                (STM32_PCLK2 / 6)
#elif STM32_ADCPRE == STM32_ADCPRE_DIV8
#define STM32_ADCCLK                (STM32_PCLK2 / 8)
#else
#error "invalid STM32_ADCPRE value specified"
#endif

/* ADC frequency check.*/
#if STM32_ADCCLK > 14000000
#error "STM32_ADCCLK exceeding maximum frequency (14MHz)"
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

#endif /* _HAL_LLD_F107_H_ */

/** @} */
