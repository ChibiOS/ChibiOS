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
 * @name    Clock source enable configurations
 * @{
 */
/**
 * @brief   Enables the HSI16 clock source.
 */
#if !defined(STM32_HSI16_ENABLE) || defined(__DOXYGEN__)
#define STM32_HSI16_ENABLE                  FALSE
#endif

/**
 * @brief   Enables the HSI48 clock source.
 */
#if !defined(STM32_HSI48_ENABLE) || defined(__DOXYGEN__)
#define STM32_HSI48_ENABLE                  FALSE
#endif

/**
 * @brief   Enables the HSE clock source.
 */
#if !defined(STM32_HSE_ENABLE) || defined(__DOXYGEN__)
#define STM32_HSE_ENABLE                    FALSE
#endif

/**
 * @brief   Enables the LSE clock source.
 */
#if !defined(STM32_LSE_ENABLE) || defined(__DOXYGEN__)
#define STM32_LSE_ENABLE                    FALSE
#endif

/**
 * @brief   Enables the LSI clock source.
 */
#if !defined(STM32_LSI_ENABLE) || defined(__DOXYGEN__)
#define STM32_LSI_ENABLE                    FALSE
#endif

/**
 * @brief   Enables the PLLP clock source.
 */
#if !defined(STM32_PLLP_ENABLE) || defined(__DOXYGEN__)
#define STM32_PLLP_ENABLE                   FALSE
#endif

/**
 * @brief   Enables the PLLQ clock source.
 */
#if !defined(STM32_PLLQ_ENABLE) || defined(__DOXYGEN__)
#define STM32_PLLQ_ENABLE                   FALSE
#endif

/**
 * @brief   Enables the PLLR clock source.
 */
#if !defined(STM32_PLLR_ENABLE) || defined(__DOXYGEN__)
#define STM32_PLLR_ENABLE                   FALSE
#endif
/** @} */

/**
 * @name    Mux configurations
 * @{
 */
#if !defined(STM32_CFG_PLLIN_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_PLLIN_SEL                 RCC_PLLSRC_NONE
#endif

#if !defined(STM32_CFG_SYSCLK_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_SYSCLK_SEL                (1U << 0)
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Calculated or fixed clocks
 * @{
 */
/**
 * @brief   Pseudo-clock for disabled sources clock point.
 */
#define CLK_NONE_FREQ                       0U

/**
 * @brief   16MHz high speed internal oscillator clock point.
 */
#define CLK_HSI16_FREQ                      16000000U

/**
 * @brief   48MHz high speed internal oscillator clock point.
 */
#define CLK_HSI48_FREQ                      48000000U

/**
 * @brief   High frequency external oscillator clock point.
 */
#define CLK_HSE_FREQ                        STM32_HSECLK

/**
 * @brief   Low frequency external oscillator clock point.
 */
#define CLK_LSE_FREQ                        STM32_LSECLK

/**
 * @brief   Low frequency internal oscillator clock point.
 */
#define CLK_LSI_FREQ                        32768U
/** @} */

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
