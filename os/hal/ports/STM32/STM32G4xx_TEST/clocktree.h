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
 * @file    clocktree.h
 * @brief   Generated clock tree description header.
 *
 * @addtogroup CLOCKTREE
 * @{
 */
#ifndef CLOCKTREE_H
#define CLOCKTREE_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Clock point configurations
 * @{
 */
/**
 * @brief   Enables the HSI16 clock source.
 */
#if !defined(STM32_CFG_HSI16_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_HSI16_ENABLE              FALSE
#endif

/**
 * @brief   Enables the HSI48 clock source.
 */
#if !defined(STM32_CFG_HSI48_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_HSI48_ENABLE              FALSE
#endif

/**
 * @brief   Enables the HSE clock source.
 */
#if !defined(STM32_CFG_HSE_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_HSE_ENABLE                FALSE
#endif

/**
 * @brief   Enables the LSE clock source.
 */
#if !defined(STM32_CFG_LSE_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_LSE_ENABLE                FALSE
#endif

/**
 * @brief   Enables the LSI clock source.
 */
#if !defined(STM32_CFG_LSI_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_LSI_ENABLE                FALSE
#endif

/**
 * @brief   Selects the PLLIN clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - HSI16.
 *          - HSE.
 */
#if !defined(STM32_CFG_PLLIN_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_PLLIN_SEL                 RCC_PLLSRC_NONE
#endif

/**
 * @brief   Configures the PLLREF clock divider value.
 */
#if !defined(STM32_CFG_PLLREF_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLREF_VALUE              1
#endif

/**
 * @brief   Configures the PLLVCO clock multiplier value.
 */
#if !defined(STM32_CFG_PLLVCO_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLVCO_VALUE              8
#endif

/**
 * @brief   Enables the PLLP clock source.
 */
#if !defined(STM32_CFG_PLLP_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLP_ENABLE               FALSE
#endif

/**
 * @brief   Configures the PLLP clock divider value.
 */
#if !defined(STM32_CFG_PLLP_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLP_VALUE                2
#endif

/**
 * @brief   Enables the PLLQ clock source.
 */
#if !defined(STM32_CFG_PLLQ_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLQ_ENABLE               FALSE
#endif

/**
 * @brief   Configures the PLLQ clock divider value.
 */
#if !defined(STM32_CFG_PLLQ_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLQ_VALUE                2
#endif

/**
 * @brief   Enables the PLLR clock source.
 */
#if !defined(STM32_CFG_PLLR_ENABLE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLR_ENABLE               FALSE
#endif

/**
 * @brief   Configures the PLLR clock divider value.
 */
#if !defined(STM32_CFG_PLLR_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLR_VALUE                2
#endif

/**
 * @brief   Selects the SYSCLK clock source.
 * @note    Allowed sources:
 *          - HSI16.
 *          - HSE.
 *          - PLLR.
 */
#if !defined(STM32_CFG_SYSCLK_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_SYSCLK_SEL                (1U << 0)
#endif

/**
 * @brief   Configures the HCLK clock divider value.
 */
#if !defined(STM32_CFG_HCLK_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_HCLK_VALUE                1
#endif

/**
 * @brief   Configures the PCLK1 clock divider value.
 */
#if !defined(STM32_CFG_PCLK1_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PCLK1_VALUE               1
#endif

/**
 * @brief   Configures the PCLK2 clock divider value.
 */
#if !defined(STM32_CFG_PCLK2_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PCLK2_VALUE               1
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @name    Clock point derived constants and checks
 * @{
 */
/**
 * @brief   NONE clock derived enable state.
 */
#define STM32_NONE_ENABLED                  FALSE

/**
 * @brief   HSI16 clock derived enable state.
 */
#define STM32_HSI16_ENABLED                 STM32_CFG_HSI16_ENABLE

/**
 * @brief   HSI48 clock derived enable state.
 */
#define STM32_HSI48_ENABLED                 STM32_CFG_HSI48_ENABLE

/**
 * @brief   HSE clock derived enable state.
 */
#define STM32_HSE_ENABLED                   STM32_CFG_HSE_ENABLE

/**
 * @brief   LSE clock derived enable state.
 */
#define STM32_LSE_ENABLED                   STM32_CFG_LSE_ENABLE

/**
 * @brief   LSI clock derived enable state.
 */
#define STM32_LSI_ENABLED                   STM32_CFG_LSI_ENABLE

/**
 * @brief   PLLIN clock derived enable state.
 */
#define STM32_PLLIN_ENABLED                 ((STM32_PLLREF_ENABLED == TRUE))

/**
 * @brief   PLLREF clock derived enable state.
 */
#define STM32_PLLREF_ENABLED                ((STM32_PLLVCO_ENABLED == TRUE))

/**
 * @brief   PLLVCO clock derived enable state.
 */
#define STM32_PLLVCO_ENABLED                ((STM32_PLLP_ENABLED == TRUE) || (STM32_PLLQ_ENABLED == TRUE) || (STM32_PLLR_ENABLED == TRUE))

/**
 * @brief   PLLP clock derived enable state.
 */
#define STM32_PLLP_ENABLED                  STM32_CFG_PLLP_ENABLE

/**
 * @brief   PLLQ clock derived enable state.
 */
#define STM32_PLLQ_ENABLED                  STM32_CFG_PLLQ_ENABLE

/**
 * @brief   PLLR clock derived enable state.
 */
#define STM32_PLLR_ENABLED                  STM32_CFG_PLLR_ENABLE

/**
 * @brief   SYSCLK clock derived enable state.
 */
#define STM32_SYSCLK_ENABLED                ((STM32_HCLK_ENABLED == TRUE))

/**
 * @brief   HCLK clock derived enable state.
 */
#define STM32_HCLK_ENABLED                  ((STM32_PCLK1_ENABLED == TRUE) || (STM32_PCLK2_ENABLED == TRUE))

/**
 * @brief   PCLK1 clock derived enable state.
 */
#define STM32_PCLK1_ENABLED                 FALSE

/**
 * @brief   PCLK2 clock derived enable state.
 */
#define STM32_PCLK2_ENABLED                 FALSE

/**
 * @brief   NONE clock register bits.
 */
#define STM32_NONE_BITS                     0U
/**
 * @brief   Pseudo-clock for disabled sources clock point.
 */
#if (STM32_NONE_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_NONE_FREQ                     0U
#else
#define STM32_NONE_FREQ                     0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_HSI16_ENABLE != TRUE) && (STM32_CFG_HSI16_ENABLE != FALSE)
#error "invalid STM32_CFG_HSI16_ENABLE value specified"
#endif

/**
 * @brief   HSI16 clock register bits.
 */
#if (STM32_HSI16_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI16_BITS                    RCC_CR_HSION
#else
#define STM32_HSI16_BITS                    0U
#endif

/**
 * @brief   16MHz high speed internal oscillator clock point.
 */
#if (STM32_HSI16_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI16_FREQ                    16000000U
#else
#define STM32_HSI16_FREQ                    0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_HSI48_ENABLE != TRUE) && (STM32_CFG_HSI48_ENABLE != FALSE)
#error "invalid STM32_CFG_HSI48_ENABLE value specified"
#endif

/**
 * @brief   HSI48 clock register bits.
 */
#if (STM32_HSI48_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI48_BITS                    RCC_CRRCR_HSI48ON
#else
#define STM32_HSI48_BITS                    0U
#endif

/**
 * @brief   48MHz high speed internal oscillator clock point.
 */
#if (STM32_HSI48_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI48_FREQ                    48000000U
#else
#define STM32_HSI48_FREQ                    0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_HSE_ENABLE != TRUE) && (STM32_CFG_HSE_ENABLE != FALSE)
#error "invalid STM32_CFG_HSE_ENABLE value specified"
#endif

/**
 * @brief   HSE clock register bits.
 */
#if (STM32_HSE_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSE_BITS                      RCC_CR_HSEON
#else
#define STM32_HSE_BITS                      0U
#endif

/**
 * @brief   High frequency external oscillator clock point.
 */
#if (STM32_HSE_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSE_FREQ                      STM32_HSECLK
#else
#define STM32_HSE_FREQ                      0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_LSE_ENABLE != TRUE) && (STM32_CFG_LSE_ENABLE != FALSE)
#error "invalid STM32_CFG_LSE_ENABLE value specified"
#endif

/**
 * @brief   LSE clock register bits.
 */
#if (STM32_LSE_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_LSE_BITS                      RCC_BDCR_LSEON
#else
#define STM32_LSE_BITS                      0U
#endif

/**
 * @brief   Low frequency external oscillator clock point.
 */
#if (STM32_LSE_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_LSE_FREQ                      STM32_LSECLK
#else
#define STM32_LSE_FREQ                      0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_LSI_ENABLE != TRUE) && (STM32_CFG_LSI_ENABLE != FALSE)
#error "invalid STM32_CFG_LSI_ENABLE value specified"
#endif

/**
 * @brief   LSI clock register bits.
 */
#if (STM32_LSI_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_LSI_BITS                      RCC_CSR_LSION
#else
#define STM32_LSI_BITS                      0U
#endif

/**
 * @brief   Low frequency internal oscillator clock point.
 */
#if (STM32_LSI_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_LSI_FREQ                      32768U
#else
#define STM32_LSI_FREQ                      0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_PLLIN_SEL != RCC_PLLSRC_NONE) && (STM32_CFG_PLLIN_SEL != RCC_PLLSRC_HSI16) && (STM32_CFG_PLLIN_SEL != RCC_PLLSRC_HSE))
#error "invalid STM32_CFG_PLLIN_SEL value specified"
#endif

/**
 * @brief   PLLIN clock register bits.
 */
#if (STM32_CFG_PLLIN_SEL == RCC_PLLSRC_NONE) || defined(__DOXYGEN__)
#define STM32_PLLIN_BITS                    RCC_PLLSRC_NONE
#elif (STM32_CFG_PLLIN_SEL == RCC_PLLSRC_HSI16)
#define STM32_PLLIN_BITS                    RCC_PLLSRC_HSI16
#elif (STM32_CFG_PLLIN_SEL == RCC_PLLSRC_HSE)
#define STM32_PLLIN_BITS                    RCC_PLLSRC_HSE
#endif

/**
 * @brief   PLL input clock point.
 */
#if ((STM32_PLLIN_ENABLED == TRUE) && (STM32_CFG_PLLIN_SEL == RCC_PLLSRC_NONE)) || defined(__DOXYGEN__)
#define STM32_PLLIN_FREQ                    STM32_NONE_FREQ
#elif ((STM32_PLLIN_ENABLED == TRUE) && (STM32_CFG_PLLIN_SEL == RCC_PLLSRC_HSI16))
#define STM32_PLLIN_FREQ                    STM32_HSI16_FREQ
#elif ((STM32_PLLIN_ENABLED == TRUE) && (STM32_CFG_PLLIN_SEL == RCC_PLLSRC_HSE))
#define STM32_PLLIN_FREQ                    STM32_HSE_FREQ
#else
#define STM32_PLLIN_FREQ                    0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_PLLREF_VALUE < 1) || (STM32_CFG_PLLREF_VALUE > 16))
#error "invalid STM32_CFG_PLLREF_VALUE value specified"
#endif

/**
 * @brief   PLLREF clock register bits.
 */
#define STM32_PLLREF_BITS                   ((STM32_CFG_PLLREF_VALUE - 1U) << 4)

/**
 * @brief   PLL reference clock point.
 */
#if (STM32_PLLREF_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLREF_FREQ                   (STM32_PLLIN_FREQ / STM32_CFG_PLLREF_VALUE)
#else
#define STM32_PLLREF_FREQ                   0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_PLLVCO_VALUE < 8) || (STM32_CFG_PLLVCO_VALUE > 127))
#error "invalid STM32_CFG_PLLVCO_VALUE value specified"
#endif

/**
 * @brief   PLLVCO clock register bits.
 */
#define STM32_PLLVCO_BITS                   (STM32_CFG_PLLVCO_VALUE << 8)

/**
 * @brief   PLL VCO clock point.
 */
#if (STM32_PLLVCO_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLVCO_FREQ                   (STM32_PLLREF_FREQ * STM32_CFG_PLLVCO_VALUE)
#else
#define STM32_PLLVCO_FREQ                   0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_PLLP_ENABLE != TRUE) && (STM32_CFG_PLLP_ENABLE != FALSE)
#error "invalid STM32_CFG_PLLP_ENABLE value specified"
#endif
#if !defined(__DOXYGEN__) && ((STM32_CFG_PLLP_VALUE < 2) || (STM32_CFG_PLLP_VALUE > 31))
#error "invalid STM32_CFG_PLLP_VALUE value specified"
#endif

/**
 * @brief   PLLP clock register bits.
 */
#if (STM32_PLLP_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLP_BITS                     (RCC_PLLCFGR_PLLPEN | (STM32_CFG_PLLP_VALUE << 27))
#else
#define STM32_PLLP_BITS                     (0U | (STM32_CFG_PLLP_VALUE << 27))
#endif

/**
 * @brief   PLL P output clock point.
 */
#if (STM32_PLLP_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLP_FREQ                     (STM32_PLLVCO_FREQ / STM32_CFG_PLLP_VALUE)
#else
#define STM32_PLLP_FREQ                     0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_PLLQ_ENABLE != TRUE) && (STM32_CFG_PLLQ_ENABLE != FALSE)
#error "invalid STM32_CFG_PLLQ_ENABLE value specified"
#endif
#if !defined(__DOXYGEN__) && ((STM32_CFG_PLLQ_VALUE != 2) && (STM32_CFG_PLLQ_VALUE != 4) && (STM32_CFG_PLLQ_VALUE != 6) && (STM32_CFG_PLLQ_VALUE != 8))
#error "invalid STM32_CFG_PLLQ_VALUE value specified"
#endif

/**
 * @brief   PLLQ clock register bits.
 */
#if (STM32_PLLQ_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLQ_BITS                     (RCC_PLLCFGR_PLLQEN | (((STM32_CFG_PLLQ_VALUE / 2U) - 1U) << 21))
#else
#define STM32_PLLQ_BITS                     (0U | (((STM32_CFG_PLLQ_VALUE / 2U) - 1U) << 21))
#endif

/**
 * @brief   PLL Q output clock point.
 */
#if (STM32_PLLQ_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLQ_FREQ                     (STM32_PLLVCO_FREQ / STM32_CFG_PLLQ_VALUE)
#else
#define STM32_PLLQ_FREQ                     0U
#endif

#if !defined(__DOXYGEN__) && (STM32_CFG_PLLR_ENABLE != TRUE) && (STM32_CFG_PLLR_ENABLE != FALSE)
#error "invalid STM32_CFG_PLLR_ENABLE value specified"
#endif
#if !defined(__DOXYGEN__) && ((STM32_CFG_PLLR_VALUE != 2) && (STM32_CFG_PLLR_VALUE != 4) && (STM32_CFG_PLLR_VALUE != 6) && (STM32_CFG_PLLR_VALUE != 8))
#error "invalid STM32_CFG_PLLR_VALUE value specified"
#endif

/**
 * @brief   PLLR clock register bits.
 */
#if (STM32_PLLR_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLR_BITS                     (RCC_PLLCFGR_PLLREN | (((STM32_CFG_PLLR_VALUE / 2U) - 1U) << 25))
#else
#define STM32_PLLR_BITS                     (0U | (((STM32_CFG_PLLR_VALUE / 2U) - 1U) << 25))
#endif

/**
 * @brief   PLL R output clock point.
 */
#if (STM32_PLLR_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLR_FREQ                     (STM32_PLLVCO_FREQ / STM32_CFG_PLLR_VALUE)
#else
#define STM32_PLLR_FREQ                     0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_SYSCLK_SEL != (1U << 0)) && (STM32_CFG_SYSCLK_SEL != (2U << 0)) && (STM32_CFG_SYSCLK_SEL != (3U << 0)))
#error "invalid STM32_CFG_SYSCLK_SEL value specified"
#endif

/**
 * @brief   SYSCLK clock register bits.
 */
#if (STM32_CFG_SYSCLK_SEL == (1U << 0)) || defined(__DOXYGEN__)
#define STM32_SYSCLK_BITS                   (1U << 0)
#elif (STM32_CFG_SYSCLK_SEL == (2U << 0))
#define STM32_SYSCLK_BITS                   (2U << 0)
#elif (STM32_CFG_SYSCLK_SEL == (3U << 0))
#define STM32_SYSCLK_BITS                   (3U << 0)
#endif

/**
 * @brief   System clock clock point.
 */
#if ((STM32_SYSCLK_ENABLED == TRUE) && (STM32_CFG_SYSCLK_SEL == (1U << 0))) || defined(__DOXYGEN__)
#define STM32_SYSCLK_FREQ                   STM32_HSI16_FREQ
#elif ((STM32_SYSCLK_ENABLED == TRUE) && (STM32_CFG_SYSCLK_SEL == (2U << 0)))
#define STM32_SYSCLK_FREQ                   STM32_HSE_FREQ
#elif ((STM32_SYSCLK_ENABLED == TRUE) && (STM32_CFG_SYSCLK_SEL == (3U << 0)))
#define STM32_SYSCLK_FREQ                   STM32_PLLR_FREQ
#else
#define STM32_SYSCLK_FREQ                   0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_HCLK_VALUE != 1) && (STM32_CFG_HCLK_VALUE != 2) && (STM32_CFG_HCLK_VALUE != 4) && (STM32_CFG_HCLK_VALUE != 8) && (STM32_CFG_HCLK_VALUE != 16) && (STM32_CFG_HCLK_VALUE != 64) && (STM32_CFG_HCLK_VALUE != 128) && (STM32_CFG_HCLK_VALUE != 256) && (STM32_CFG_HCLK_VALUE != 512))
#error "invalid STM32_CFG_HCLK_VALUE value specified"
#endif

/**
 * @brief   HCLK clock register bits.
 */
#if (STM32_CFG_HCLK_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_HCLK_BITS                     (1U << 4)
#elif (STM32_CFG_HCLK_VALUE == 2)
#define STM32_HCLK_BITS                     (8U << 4)
#elif (STM32_CFG_HCLK_VALUE == 4)
#define STM32_HCLK_BITS                     (9U << 4)
#elif (STM32_CFG_HCLK_VALUE == 8)
#define STM32_HCLK_BITS                     (10U << 4)
#elif (STM32_CFG_HCLK_VALUE == 16)
#define STM32_HCLK_BITS                     (11U << 4)
#elif (STM32_CFG_HCLK_VALUE == 64)
#define STM32_HCLK_BITS                     (12U << 4)
#elif (STM32_CFG_HCLK_VALUE == 128)
#define STM32_HCLK_BITS                     (13U << 4)
#elif (STM32_CFG_HCLK_VALUE == 256)
#define STM32_HCLK_BITS                     (14U << 4)
#elif (STM32_CFG_HCLK_VALUE == 512)
#define STM32_HCLK_BITS                     (15U << 4)
#endif

/**
 * @brief   AHB clock point.
 */
#if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HCLK_FREQ                     (STM32_SYSCLK_FREQ / STM32_CFG_HCLK_VALUE)
#else
#define STM32_HCLK_FREQ                     0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_PCLK1_VALUE != 1) && (STM32_CFG_PCLK1_VALUE != 2) && (STM32_CFG_PCLK1_VALUE != 4) && (STM32_CFG_PCLK1_VALUE != 8) && (STM32_CFG_PCLK1_VALUE != 16))
#error "invalid STM32_CFG_PCLK1_VALUE value specified"
#endif

/**
 * @brief   PCLK1 clock register bits.
 */
#if (STM32_CFG_PCLK1_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_PCLK1_BITS                    (1U << 8)
#elif (STM32_CFG_PCLK1_VALUE == 2)
#define STM32_PCLK1_BITS                    (4U << 8)
#elif (STM32_CFG_PCLK1_VALUE == 4)
#define STM32_PCLK1_BITS                    (5U << 8)
#elif (STM32_CFG_PCLK1_VALUE == 8)
#define STM32_PCLK1_BITS                    (6U << 8)
#elif (STM32_CFG_PCLK1_VALUE == 16)
#define STM32_PCLK1_BITS                    (7U << 8)
#endif

/**
 * @brief   APB1 clock point.
 */
#if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PCLK1_FREQ                    (STM32_HCLK_FREQ / STM32_CFG_PCLK1_VALUE)
#else
#define STM32_PCLK1_FREQ                    0U
#endif

#if !defined(__DOXYGEN__) && ((STM32_CFG_PCLK2_VALUE != 1) && (STM32_CFG_PCLK2_VALUE != 2) && (STM32_CFG_PCLK2_VALUE != 4) && (STM32_CFG_PCLK2_VALUE != 8) && (STM32_CFG_PCLK2_VALUE != 16))
#error "invalid STM32_CFG_PCLK2_VALUE value specified"
#endif

/**
 * @brief   PCLK2 clock register bits.
 */
#if (STM32_CFG_PCLK2_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_PCLK2_BITS                    (1U << 11)
#elif (STM32_CFG_PCLK2_VALUE == 2)
#define STM32_PCLK2_BITS                    (4U << 11)
#elif (STM32_CFG_PCLK2_VALUE == 4)
#define STM32_PCLK2_BITS                    (5U << 11)
#elif (STM32_CFG_PCLK2_VALUE == 8)
#define STM32_PCLK2_BITS                    (6U << 11)
#elif (STM32_CFG_PCLK2_VALUE == 16)
#define STM32_PCLK2_BITS                    (7U << 11)
#endif

/**
 * @brief   APB2 clock point.
 */
#if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PCLK2_FREQ                    (STM32_HCLK_FREQ / STM32_CFG_PCLK2_VALUE)
#else
#define STM32_PCLK2_FREQ                    0U
#endif
/** @} */

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
#ifdef __cplusplus
}
#endif

#endif /* CLOCKTREE_H */

/** @} */
