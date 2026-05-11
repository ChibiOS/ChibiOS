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
 * @name    Mux configurations
 * @{
 */
#if !defined(name) || defined(__DOXYGEN__)
#define CLK_PLLIN_SEL                       CLK_PLLIN_NONE
#endif

#if !defined(name) || defined(__DOXYGEN__)
#define CLK_SYSCLK_SEL                      CLK_SYSCLK_HSI16
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
 * @brief   No clock clock point.
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
