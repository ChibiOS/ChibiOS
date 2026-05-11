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
 * @name    Calculated or fixed clocks
 * @{
 */
/**
 * @brief   Pseudo-clock for disabled sources clock point.
 */
#define STM32_NONE_FREQ                     0U

/**
 * @brief   16MHz high speed internal oscillator clock point.
 */
#if (STM32_CFG_HSI16_ENABLE == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI16_FREQ                    16000000U
#else
#define STM32_HSI16_FREQ                    0U
#endif

/**
 * @brief   48MHz high speed internal oscillator clock point.
 */
#if (STM32_CFG_HSI48_ENABLE == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI48_FREQ                    48000000U
#else
#define STM32_HSI48_FREQ                    0U
#endif

/**
 * @brief   High frequency external oscillator clock point.
 */
#if (STM32_CFG_HSE_ENABLE == TRUE) || defined(__DOXYGEN__)
#define STM32_HSE_FREQ                      STM32_HSECLK
#else
#define STM32_HSE_FREQ                      0U
#endif

/**
 * @brief   Low frequency external oscillator clock point.
 */
#if (STM32_CFG_LSE_ENABLE == TRUE) || defined(__DOXYGEN__)
#define STM32_LSE_FREQ                      STM32_LSECLK
#else
#define STM32_LSE_FREQ                      0U
#endif

/**
 * @brief   Low frequency internal oscillator clock point.
 */
#if (STM32_CFG_LSI_ENABLE == TRUE) || defined(__DOXYGEN__)
#define STM32_LSI_FREQ                      32768U
#else
#define STM32_LSI_FREQ                      0U
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
