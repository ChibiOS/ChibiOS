/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    STM32G0xx/hal_lld.h
 * @brief   STM32G0xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_LSEDRV.
 *          - STM32_LSE_BYPASS (optionally).
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32G070xx.
 *          - STM32G071xx, STM32G081xx.
 *          .
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

#include "stm32_registry.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification
 * @{
 */
#if defined(STM32G070xx) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32G0 Entry-level Value Line"

#elif defined(STM32G071xx)
#define PLATFORM_NAME           "STM32G0 Entry-level"

#elif defined(STM32G081xx)
#define PLATFORM_NAME           "STM32G0 Entry-level with Crypto"

#else
#error "STM32G0 device not specified"
#endif

/**
 * @brief   Sub-family identifier.
 */
#if !defined(STM32G0xx) || defined(__DOXYGEN__)
#define STM32G0xx
#endif
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define STM32_HSI16CLK          16000000U   /**< 16MHz internal clock.      */
#define STM32_LSICLK            32000U      /**< Low speed internal clock.  */
/** @} */

/**
 * @name    PWR_CR1 register bits definitions
 * @{
 */
#define STM32_VOS_MASK          (3U << 9U)  /**< Core voltage mask.         */
#define STM32_VOS_RANGE1        (1U << 9U)  /**< Core voltage 1.2 Volts.    */
#define STM32_VOS_RANGE2        (2U << 9U)  /**< Core voltage 1.0 Volts.    */
/** @} */

/**
 * @name    PWR_CR2 register bits definitions
 * @{
 */
#define STM32_PVDE_DISABLED     (0U << 1U)      /**< PVD enable bit off.    */
#define STM32_PVDE_ENABLED      (1U << 1U)      /**< PVD enable bit on.     */

#define STM32_PVDFT_MASK        (7U << 1U)      /**< PVDFT bits mask.       */
#define STM32_PVDFT(n)          ((n) << 1U)     /**< PVDFT level.           */
#define STM32_PVDFT_LEV0        STM32_PVDFT(0U) /**< PVDFT level 0.         */
#define STM32_PVDFT_LEV1        STM32_PVDFT(1U) /**< PVDFT level 1.         */
#define STM32_PVDFT_LEV2        STM32_PVDFT(2U) /**< PVDFT level 2.         */
#define STM32_PVDFT_LEV3        STM32_PVDFT(3U) /**< PVDFT level 3.         */
#define STM32_PVDFT_LEV4        STM32_PVDFT(4U) /**< PVDFT level 4.         */
#define STM32_PVDFT_LEV5        STM32_PVDFT(5U) /**< PVDFT level 5.         */
#define STM32_PVDFT_LEV6        STM32_PVDFT(6U) /**< PVDFT level 6.         */
#define STM32_PVDFT_LEV7        STM32_PVDFT(7U) /**< PVDFT level 7.         */

#define STM32_PVDRT_MASK        (7U << 4U)      /**< PVDRT bits mask.       */
#define STM32_PVDRT(n)          ((n) << 4U)     /**< PVDRT level.           */
#define STM32_PVDRT_LEV0        STM32_PVDRT(0U) /**< PVDRT level 0.         */
#define STM32_PVDRT_LEV1        STM32_PVDRT(1U) /**< PVDRT level 1.         */
#define STM32_PVDRT_LEV2        STM32_PVDRT(2U) /**< PVDRT level 2.         */
#define STM32_PVDRT_LEV3        STM32_PVDRT(3U) /**< PVDRT level 3.         */
#define STM32_PVDRT_LEV4        STM32_PVDRT(4U) /**< PVDRT level 4.         */
#define STM32_PVDRT_LEV5        STM32_PVDRT(5U) /**< PVDRT level 5.         */
#define STM32_PVDRT_LEV6        STM32_PVDRT(6U) /**< PVDRT level 6.         */
#define STM32_PVDRT_LEV7        STM32_PVDRT(7U) /**< PVDRT level 7.         */
/** @} */

/**
 * @name    RCC_CR register bits definitions
 * @{
 */
#define STM32_HSIDIV_MASK       (7U << 11U)     /**< HSIDIV field mask.     */
#define STM32_HSIDIV(n)         ((n) << 11U)    /**< HSIDIV field value.    */
#define STM32_HSIDIV_1          STM32_HSIDIV(0U)
#define STM32_HSIDIV_2          STM32_HSIDIV(1U)
#define STM32_HSIDIV_4          STM32_HSIDIV(2U)
#define STM32_HSIDIV_8          STM32_HSIDIV(3U)
#define STM32_HSIDIV_16         STM32_HSIDIV(4U)
#define STM32_HSIDIV_32         STM32_HSIDIV(5U)
#define STM32_HSIDIV_64         STM32_HSIDIV(6U)
#define STM32_HSIDIV_128        STM32_HSIDIV(7U)
/** @} */

/**
 * @name    RCC_CFGR register bits definitions
 * @{
 */
#define STM32_SW_MASK           (7U << 0U)  /**< SW field mask.             */
#define STM32_SW_HSISYS         (0U << 0U)  /**< SYSCLK source is HSISYS.   */
#define STM32_SW_HSE            (1U << 0U)  /**< SYSCLK source is HSE.      */
#define STM32_SW_PLLRCLK        (2U << 0U)  /**< SYSCLK source is PLL.      */
#define STM32_SW_LSI            (3U << 0U)  /**< SYSCLK source is LSI.      */
#define STM32_SW_LSE            (4U << 0U)  /**< SYSCLK source is LSE.      */

#define STM32_HPRE_MASK         (15U << 8U) /**< HPRE field mask.           */
#define STM32_HPRE(n)           ((n) << 8U) /**< HPRE field value.          */
#define STM32_HPRE_DIV1         STM32_HPRE(0U)
#define STM32_HPRE_DIV2         STM32_HPRE(8U)
#define STM32_HPRE_DIV4         STM32_HPRE(9U)
#define STM32_HPRE_DIV8         STM32_HPRE(10U)
#define STM32_HPRE_DIV16        STM32_HPRE(11U)
#define STM32_HPRE_DIV64        STM32_HPRE(12U)
#define STM32_HPRE_DIV128       STM32_HPRE(13U)
#define STM32_HPRE_DIV256       STM32_HPRE(14U)
#define STM32_HPRE_DIV512       STM32_HPRE(15U)

#define STM32_PPRE_MASK         (7U << 12U) /**< PPRE field mask.           */
#define STM32_PPRE(n)           (7U << 12U) /**< PPRE field value.          */
#define STM32_PPRE_DIV1         STM32_PPRE(0U)
#define STM32_PPRE_DIV2         STM32_PPRE(4U)
#define STM32_PPRE_DIV4         STM32_PPRE(5U)
#define STM32_PPRE_DIV8         STM32_PPRE(6U)
#define STM32_PPRE_DIV16        STM32_PPRE(7U)

#define STM32_MCOSEL_MASK       (7U << 24U) /**< MCOSEL field mask.         */
#define STM32_MCOSEL_NOCLOCK    (0U << 24U) /**< No clock on MCO pin.       */
#define STM32_MCOSEL_SYSCLK     (1U << 24U) /**< SYSCLK on MCO pin.         */
#define STM32_MCOSEL_HSI16      (3U << 24U) /**< HSI16 clock on MCO pin.    */
#define STM32_MCOSEL_HSE        (4U << 24U) /**< HSE clock on MCO pin.      */
#define STM32_MCOSEL_PLLRCLK    (5U << 24U) /**< PLLR clock on MCO pin.      */
#define STM32_MCOSEL_LSI        (6U << 24U) /**< LSI clock on MCO pin.      */
#define STM32_MCOSEL_LSE        (7U << 24U) /**< LSE clock on MCO pin.      */

#define STM32_MCOPRE_MASK       (7U << 28U) /**< MCOPRE field mask.         */
#define STM32_MCOPRE(n)         ((n) << 28U)/**< MCOPRE field value         */
#define STM32_MCOPRE_DIV1       STM32_MCOPRE(0U)
#define STM32_MCOPRE_DIV2       STM32_MCOPRE(1U)
#define STM32_MCOPRE_DIV4       STM32_MCOPRE(2U)
#define STM32_MCOPRE_DIV8       STM32_MCOPRE(3U)
#define STM32_MCOPRE_DIV16      STM32_MCOPRE(4U)
#define STM32_MCOPRE_DIV32      STM32_MCOPRE(5U)
#define STM32_MCOPRE_DIV64      STM32_MCOPRE(6U)
#define STM32_MCOPRE_DIV128     STM32_MCOPRE(7U)
/** @} */

/**
 * @name    RCC_PLLCFGR register bits definitions
 * @{
 */
#define STM32_PLLSRC_MASK       (3 << 0)    /**< PLL clock source mask.     */
#define STM32_PLLSRC_NOCLOCK    (0 << 0)    /**< PLL clock source disabled. */
#define STM32_PLLSRC_HSI16      (2 << 0)    /**< PLL clock source is HSI16. */
#define STM32_PLLSRC_HSE        (3 << 0)    /**< PLL clock source is HSE.   */
/** @} */

/**
 * @name    RCC_CCIPR register bits definitions
 * @{
 */
#define STM32_USART1SEL_MASK    (3U << 0U)  /**< USART1SEL mask.            */
#define STM32_USART1SEL_PCLK    (0U << 0U)  /**< USART1 source is PCLK.     */
#define STM32_USART1SEL_SYSCLK  (1U << 0U)  /**< USART1 source is SYSCLK.   */
#define STM32_USART1SEL_HSI16   (2U << 0U)  /**< USART1 source is HSI16.    */
#define STM32_USART1SEL_LSE     (3U << 0U)  /**< USART1 source is LSE.      */

#define STM32_USART2SEL_MASK    (3U << 2U)  /**< USART2 mask.               */
#define STM32_USART2SEL_PCLK    (0U << 2U)  /**< USART2 source is PCLK.     */
#define STM32_USART2SEL_SYSCLK  (1U << 2U)  /**< USART2 source is SYSCLK.   */
#define STM32_USART2SEL_HSI16   (2U << 2U)  /**< USART2 source is HSI16.    */
#define STM32_USART2SEL_LSE     (3U << 2U)  /**< USART2 source is LSE.      */

#define STM32_CECSEL_MASK       (1U << 6U)  /**< CEC mask.                  */
#define STM32_CECSEL_HSI16      (0U << 6U)  /**< CEC source is HSI16.       */
#define STM32_CECSEL_LSE        (1U << 6U)  /**< CEC source is LSE.         */

#define STM32_LPUART1SEL_MASK   (3U << 10U) /**< LPUART1 mask.              */
#define STM32_LPUART1SEL_PCLK   (0U << 10U) /**< LPUART1 source is PCLK.    */
#define STM32_LPUART1SEL_SYSCLK (1U << 10U) /**< LPUART1 source is SYSCLK.  */
#define STM32_LPUART1SEL_HSI16  (2U << 10U) /**< LPUART1 source is HSI16.   */
#define STM32_LPUART1SEL_LSE    (3U << 10U) /**< LPUART1 source is LSE.     */

#define STM32_I2C1SEL_MASK      (3U << 12U) /**< I2C1SEL mask.              */
#define STM32_I2C1SEL_PCLK      (0U << 12U) /**< I2C1 source is PCLK.       */
#define STM32_I2C1SEL_SYSCLK    (1U << 12U) /**< I2C1 source is SYSCLK.     */
#define STM32_I2C1SEL_HSI16     (2U << 12U) /**< I2C1 source is HSI16.      */

#define STM32_I2S1SEL_MASK      (3U << 14U) /**< I2S1SEL mask.              */
#define STM32_I2S1SEL_PCLK      (0U << 14U) /**< I2S1 source is PCLK.       */
#define STM32_I2S1SEL_SYSCLK    (1U << 14U) /**< I2S1 source is SYSCLK.     */
#define STM32_I2S1SEL_HSI16     (2U << 14U) /**< I2S1 source is HSI16.      */
#define STM32_I2S1SEL_CKIN      (3U << 14U) /**< I2S1 source is CKIN.       */

#define STM32_LPTIM1SEL_MASK    (3U << 18U) /**< LPTIM1SEL mask.            */
#define STM32_LPTIM1SEL_PCLK    (0U << 18U) /**< LPTIM1 source is PCLK.     */
#define STM32_LPTIM1SEL_LSI     (1U << 18U) /**< LPTIM1 source is LSI.      */
#define STM32_LPTIM1SEL_HSI16   (2U << 18U) /**< LPTIM1 source is HSI16.    */
#define STM32_LPTIM1SEL_LSE     (3U << 18U) /**< LPTIM1 source is LSE.      */

#define STM32_LPTIM2SEL_MASK    (3U << 20U) /**< LPTIM2SEL mask.            */
#define STM32_LPTIM2SEL_PCLK    (0U << 20U) /**< LPTIM2 source is PCLK.     */
#define STM32_LPTIM2SEL_LSI     (1U << 20U) /**< LPTIM2 source is LSI.      */
#define STM32_LPTIM2SEL_HSI16   (2U << 20U) /**< LPTIM2 source is HSI16.    */
#define STM32_LPTIM2SEL_LSE     (3U << 20U) /**< LPTIM2 source is LSE.      */

#define STM32_TIM1SEL_MASK      (1U << 22U) /**< TIM1SEL mask.              */
#define STM32_TIM1SEL_TIMPCLK   (0U << 22U) /**< TIM1SEL source is TIMPCLK. */
#define STM32_TIM1SEL_PLLQCLK   (1U << 22U) /**< TIM1SEL source is PLLQCLK. */

#define STM32_TIM15SEL_MASK     (1U << 24U) /**< TIM15SEL mask.             */
#define STM32_TIM15SEL_TIMPCLK  (0U << 24U) /**< TIM15SEL source is TIMPCLK.*/
#define STM32_TIM15SEL_PLLQCLK  (1U << 24U) /**< TIM15SEL source is PLLQCLK.*/

#define STM32_RNGSEL_MASK       (3U << 26U) /**< RNGSEL mask.               */
#define STM32_RNGSEL_NOCLOCK    (0U << 26U) /**< RNG source is disabled.    */
#define STM32_RNGSEL_HSI16      (1U << 26U) /**< RNG source is HSI16.       */
#define STM32_RNGSEL_SYSCLK     (2U << 26U) /**< RNG source is SYSCLK.      */
#define STM32_RNGSEL_PLLQCLK    (3U << 26U) /**< RNG source is PLLQCLK.     */

#define STM32_RNGDIV_MASK       (3U << 28U) /**< RNGDIV field mask.         */
#define STM32_RNGDIV(n)         ((n) << 28U)/**< RNGDIV field value         */
#define STM32_RNGDIV_1          STM32_RNGDIV(0U)
#define STM32_RNGDIV_2          STM32_RNGDIV(1U)
#define STM32_RNGDIV_4          STM32_RNGDIV(2U)
#define STM32_RNGDIV_8          STM32_RNGDIV(3U)

#define STM32_ADCSEL_MASK       (3U << 30U) /**< ADCSEL mask.               */
#define STM32_ADCSEL_NOCLK      (0U << 30U) /**< ADC source is SYSCLK.      */
#define STM32_ADCSEL_PLLPCLK    (1U << 30U) /**< ADC source is PLLPCLK.     */
#define STM32_ADCSEL_HSI16      (2U << 30U) /**< ADC source is HSI16.       */
/** @} */

/**
 * @name    RCC_BDCR register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK       (3U << 8U)  /**< RTC source mask.           */
#define STM32_RTCSEL_NOCLOCK    (0U << 8U)  /**< No RTC source.             */
#define STM32_RTCSEL_LSE        (1U << 8U)  /**< RTC source is LSE.         */
#define STM32_RTCSEL_LSI        (2U << 8U)  /**< RTC source is LSI.         */
#define STM32_RTCSEL_HSEDIV     (3U << 8U)  /**< RTC source is HSE divided. */

#define STM32_LSCOSEL_MASK      (3U << 24U) /**< LSCO pin clock source.     */
#define STM32_LSCOSEL_NOCLOCK   (0U << 24U) /**< No clock on LSCO pin.      */
#define STM32_LSCOSEL_LSI       (1U << 24U) /**< LSI on LSCO pin.           */
#define STM32_LSCOSEL_LSE       (3U << 24U) /**< LSE on LSCO pin.           */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Disables the PWR/RCC initialization in the HAL.
 */
#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT                       FALSE
#endif

/**
 * @brief   Core voltage selection.
 * @note    This setting affects all the performance and clock related
 *          settings, the maximum performance is only obtainable selecting
 *          the maximum voltage.
 */
#if !defined(STM32_VOS) || defined(__DOXYGEN__)
#define STM32_VOS                           STM32_VOS_RANGE1
#endif

/**
 * @brief   PWR CR2 register initialization value.
 */
#if !defined(STM32_PWR_CR2) || defined(__DOXYGEN__)
#define STM32_PWR_CR2                       (STM32_PVDRT_LEV0 |              \
                                             STM32_PVDFT_LEV0 |              \
                                             STM32_PVDE_DISABLED)
#endif

/**
 * @brief   Enables or disables the HSI16 clock source.
 */
#if !defined(STM32_HSI16_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI16_ENABLED                 FALSE
#endif

/**
 * @brief   Enables or disables the HSI48 clock source.
 */
#if !defined(STM32_HSI48_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI48_ENABLED                 FALSE
#endif

/**
 * @brief   Enables or disables the HSE clock source.
 */
#if !defined(STM32_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSE_ENABLED                   FALSE
#endif

/**
 * @brief   Enables or disables the LSE clock source.
 */
#if !defined(STM32_LSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSE_ENABLED                   FALSE
#endif

/**
 * @brief   Main clock source selection.
 * @note    If the selected clock source is not the PLL then the PLL is not
 *          initialized and started.
 * @note    The default value is calculated for a 64MHz system clock from
 *          the internal 16MHz HSI clock.
 */
#if !defined(STM32_SW) || defined(__DOXYGEN__)
#define STM32_SW                            STM32_SW_PLLRCLK
#endif

/**
 * @brief   Clock source for the PLL.
 * @note    This setting has only effect if the PLL is selected as the
 *          system clock source.
 * @note    The default value is calculated for a 64MHz system clock from
 *          the internal 16MHz HSI clock.
 */
#if !defined(STM32_PLLSRC) || defined(__DOXYGEN__)
#define STM32_PLLSRC                        STM32_PLLSRC_HSI16
#endif

/**
 * @brief   PLLM divider value.
 * @note    The allowed values are 1..16.
 * @note    The default value is calculated for a 64MHz system clock from
 *          the internal 16MHz HSI clock.
 */
#if !defined(STM32_PLLM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLM_VALUE                    2
#endif

/**
 * @brief   PLLN multiplier value.
 * @note    The allowed values are 8..86.
 * @note    The default value is calculated for a 64MHz system clock from
 *          the internal 16MHz HSI clock.
 */
#if !defined(STM32_PLLN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLN_VALUE                    16
#endif

/**
 * @brief   PLLP divider value or zero if disabled.
 * @note    The allowed values are 0, 2..32.
 */
#if !defined(STM32_PLLP_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLP_VALUE                    4
#endif

/**
 * @brief   PLLQ divider value or zero if disabled.
 * @note    The allowed values are 0, 2..8.
 */
#if !defined(STM32_PLLQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLQ_VALUE                    4
#endif

/**
 * @brief   PLLR divider value or zero if disabled.
 * @note    The allowed values are 0, 2..8.
 * @note    The default value is calculated for a 64MHz system clock from
 *          the internal 16MHz HSI clock.
 */
#if !defined(STM32_PLLR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLR_VALUE                    2
#endif

/**
 * @brief   AHB prescaler value.
 * @note    The default value is calculated for a 64MHz system clock from
 *          the internal 16MHz HSI clock.
 */
#if !defined(STM32_HPRE) || defined(__DOXYGEN__)
#define STM32_HPRE                          STM32_HPRE_DIV1
#endif

/**
 * @brief   APB prescaler value.
 */
#if !defined(STM32_PPRE) || defined(__DOXYGEN__)
#define STM32_PPRE                          STM32_PPRE_DIV1
#endif

/**
 * @brief   MCO clock source.
 */
#if !defined(STM32_MCOSEL) || defined(__DOXYGEN__)
#define STM32_MCOSEL                        STM32_MCOSEL_NOCLOCK
#endif

/**
 * @brief   MCO divider setting.
 */
#if !defined(STM32_MCOPRE) || defined(__DOXYGEN__)
#define STM32_MCOPRE                        STM32_MCOPRE_DIV1
#endif

/**
 * @brief   LSCO clock source.
 */
#if !defined(STM32_LSCOSEL) || defined(__DOXYGEN__)
#define STM32_LSCOSEL                       STM32_LSCOSEL_NOCLOCK
#endif

/**
 * @brief   USART1 clock source.
 */
#if !defined(STM32_USART1SEL) || defined(__DOXYGEN__)
#define STM32_USART1SEL                     STM32_USART1SEL_SYSCLK
#endif

/**
 * @brief   USART2 clock source.
 */
#if !defined(STM32_USART2SEL) || defined(__DOXYGEN__)
#define STM32_USART2SEL                     STM32_USART2SEL_SYSCLK
#endif

/**
 * @brief   LPUART1 clock source.
 */
#if !defined(STM32_LPUART1SEL) || defined(__DOXYGEN__)
#define STM32_LPUART1SEL                    STM32_LPUART1SEL_SYSCLK
#endif

/**
 * @brief   CEC clock source.
 */
#if !defined(STM32_CECSEL) || defined(__DOXYGEN__)
#define STM32_CECSEL                        STM32_CECSEL_HSI16
#endif

/**
 * @brief   I2C1 clock source.
 */
#if !defined(STM32_I2C1SEL) || defined(__DOXYGEN__)
#define STM32_I2C1SEL                       STM32_I2C1SEL_PCLK
#endif

/**
 * @brief   I2S1 clock source.
 */
#if !defined(STM32_I2S1SEL) || defined(__DOXYGEN__)
#define STM32_I2S1SEL                       STM32_I2S1SEL_PCLK
#endif

/**
 * @brief   LPTIM1 clock source.
 */
#if !defined(STM32_LPTIM1SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK
#endif

/**
 * @brief   LPTIM2 clock source.
 */
#if !defined(STM32_LPTIM2SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM2SEL                     STM32_LPTIM2SEL_PCLK
#endif

/**
 * @brief   TIM1 clock source.
 */
#if !defined(STM32_TIM1SEL) || defined(__DOXYGEN__)
#define STM32_TIM1SEL                       STM32_TIM1SEL_TIMPCLK
#endif

/**
 * @brief   TIM15 clock source.
 */
#if !defined(STM32_TIM15SEL) || defined(__DOXYGEN__)
#define STM32_TIM15SEL                      STM32_TIM15SEL_TIMPCLK
#endif

/**
 * @brief   ADCSEL value (ADCs clock source).
 */
#if !defined(STM32_ADCSEL) || defined(__DOXYGEN__)
#define STM32_ADCSEL                        STM32_ADCSEL_PLLPCLK
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(STM32G0xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G0xx_MCUCONF not defined"
#endif

#if defined(STM32G070xx) && !defined(STM32G070_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G070_MCUCONF not defined"

#elif defined(STM32G071xx) && !defined(STM32G071_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G071_MCUCONF not defined"

#elif defined(STM32G081xx) && !defined(STM32G081_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G071_MCUCONF not defined"

#endif

/*
 * Board files sanity checks.
 */
#if !defined(STM32_LSECLK)
#error "STM32_LSECLK not defined in board.h"
#endif

#if !defined(STM32_LSEDRV)
#error "STM32_LSEDRV not defined in board.h"
#endif

#if !defined(STM32_HSECLK)
#error "STM32_HSECLK not defined in board.h"
#endif

/* Voltage related limits.*/
#if (STM32_VOS == STM32_VOS_RANGE1) || defined(__DOXYGEN__)
/**
 * @name    System Limits
 * @{
 */
/**
 * @brief   Maximum SYSCLK clock frequency in boost mode.
 */
#define STM32_SYSCLK_MAX            120000000

/**
 * @brief   Maximum SYSCLK clock frequency in normal mode.
 */
#define STM32_SYSCLK_NOBOOST_MAX    80000000

/**
 * @brief   Maximum HSE clock frequency at current voltage setting.
 */
#define STM32_HSECLK_MAX            48000000

/**
 * @brief   Maximum HSE clock frequency using an external source.
 */
#define STM32_HSECLK_BYP_MAX        48000000

/**
 * @brief   Minimum HSE clock frequency.
 */
#define STM32_HSECLK_MIN            8000000

/**
 * @brief   Minimum HSE clock frequency using an external source.
 */
#define STM32_HSECLK_BYP_MIN        8000000

/**
 * @brief   Maximum LSE clock frequency.
 */
#define STM32_LSECLK_MAX            32768

/**
 * @brief   Maximum LSE clock frequency.
 */
#define STM32_LSECLK_BYP_MAX        1000000

/**
 * @brief   Minimum LSE clock frequency.
 */
#define STM32_LSECLK_MIN            32768

/**
 * @brief   Minimum LSE clock frequency.
 */
#define STM32_LSECLK_BYP_MIN        32768

/**
 * @brief   Maximum PLLs input clock frequency.
 */
#define STM32_PLLIN_MAX             16000000

/**
 * @brief   Minimum PLLs input clock frequency.
 */
#define STM32_PLLIN_MIN             2660000

/**
 * @brief   Maximum VCO clock frequency at current voltage setting.
 */
#define STM32_PLLVCO_MAX            344000000

/**
 * @brief   Minimum VCO clock frequency at current voltage setting.
 */
#define STM32_PLLVCO_MIN            64000000

/**
 * @brief   Maximum PLL-P output clock frequency.
 */
#define STM32_PLLP_MAX              120000000

/**
 * @brief   Minimum PLL-P output clock frequency.
 */
#define STM32_PLLP_MIN              2064500

/**
 * @brief   Maximum PLL-Q output clock frequency.
 */
#define STM32_PLLQ_MAX              120000000

/**
 * @brief   Minimum PLL-Q output clock frequency.
 */
#define STM32_PLLQ_MIN              8000000

/**
 * @brief   Maximum PLL-R output clock frequency.
 */
#define STM32_PLLR_MAX              120000000

/**
 * @brief   Minimum PLL-R output clock frequency.
 */
#define STM32_PLLR_MIN              8000000

/**
 * @brief   Maximum APB1 clock frequency.
 */
#define STM32_PCLK1_MAX             120000000

/**
 * @brief   Maximum APB2 clock frequency.
 */
#define STM32_PCLK2_MAX             120000000

/**
 * @brief   Maximum ADC clock frequency.
 */
#define STM32_ADCCLK_MAX            80000000
/** @} */

/**
 * @name    Flash Wait states
 * @{
 */
#define STM32_0WS_THRESHOLD         20000000
#define STM32_1WS_THRESHOLD         40000000
#define STM32_2WS_THRESHOLD         60000000
#define STM32_3WS_THRESHOLD         80000000
#define STM32_4WS_THRESHOLD         100000000
#define STM32_5WS_THRESHOLD         120000000
/** @} */

#elif STM32_VOS == STM32_VOS_RANGE2
#define STM32_SYSCLK_MAX            26000000
#define STM32_SYSCLK_NOBOOST_MAX    26000000
#define STM32_HSECLK_MAX            26000000
#define STM32_HSECLK_BYP_MAX        26000000
#define STM32_HSECLK_MIN            8000000
#define STM32_HSECLK_BYP_MIN        8000000
#define STM32_LSECLK_MAX            32768
#define STM32_LSECLK_BYP_MAX        1000000
#define STM32_LSECLK_MIN            32768
#define STM32_LSECLK_BYP_MIN        32768
#define STM32_PLLIN_MAX             16000000
#define STM32_PLLIN_MIN             2660000
#define STM32_PLLVCO_MAX            128000000
#define STM32_PLLVCO_MIN            64000000
#define STM32_PLLP_MAX              26000000
#define STM32_PLLP_MIN              2064500
#define STM32_PLLQ_MAX              26000000
#define STM32_PLLQ_MIN              8000000
#define STM32_PLLR_MAX              26000000
#define STM32_PLLR_MIN              8000000
#define STM32_PCLK1_MAX             26000000
#define STM32_PCLK2_MAX             26000000
#define STM32_ADCCLK_MAX            26000000

#define STM32_0WS_THRESHOLD         8000000
#define STM32_1WS_THRESHOLD         16000000
#define STM32_2WS_THRESHOLD         26000000
#define STM32_3WS_THRESHOLD         0
#define STM32_4WS_THRESHOLD         0
#define STM32_5WS_THRESHOLD         0

#else
#error "invalid STM32_VOS value specified"
#endif

/**
 * @brief   MSI frequency.
 */
#if STM32_MSIRANGE == STM32_MSIRANGE_100K
#define STM32_MSICLK                100000
#elif STM32_MSIRANGE == STM32_MSIRANGE_200K
#define STM32_MSICLK                200000
#elif STM32_MSIRANGE == STM32_MSIRANGE_400K
#define STM32_MSICLK                400000
#elif STM32_MSIRANGE == STM32_MSIRANGE_800K
#define STM32_MSICLK                800000
#elif STM32_MSIRANGE == STM32_MSIRANGE_1M
#define STM32_MSICLK                1000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_2M
#define STM32_MSICLK                2000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_4M
#define STM32_MSICLK                4000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_8M
#define STM32_MSICLK                8000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_16M
#define STM32_MSICLK                16000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_24M
#define STM32_MSICLK                24000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_32M
#define STM32_MSICLK                32000000
#elif STM32_MSIRANGE == STM32_MSIRANGE_48M
#define STM32_MSICLK                48000000
#else
#error "invalid STM32_MSIRANGE value specified"
#endif

/**
 * @brief   MSIS frequency.
 */
#if STM32_MSISRANGE == STM32_MSISRANGE_1M
#define STM32_MSISCLK               1000000
#elif STM32_MSISRANGE == STM32_MSISRANGE_2M
#define STM32_MSISCLK               2000000
#elif STM32_MSISRANGE == STM32_MSISRANGE_4M
#define STM32_MSISCLK               4000000
#elif STM32_MSISRANGE == STM32_MSISRANGE_8M
#define STM32_MSISCLK               8000000
#else
#error "invalid STM32_MSISRANGE value specified"
#endif

/*
 * HSI16 related checks.
 */
#if STM32_HSI16_ENABLED
#else /* !STM32_HSI16_ENABLED */

#if STM32_SW == STM32_SW_HSI16
#error "HSI16 not enabled, required by STM32_SW"
#endif

#if (STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_HSI16)
#error "HSI16 not enabled, required by STM32_SW and STM32_PLLSRC"
#endif

/* NOTE: Missing checks on the HSI16 pre-muxes, it is also required for newer
   L4 devices.*/

#if (STM32_MCOSEL == STM32_MCOSEL_HSI16) ||                                 \
    ((STM32_MCOSEL == STM32_MCOSEL_PLL) &&                                  \
     (STM32_PLLSRC == STM32_PLLSRC_HSI16))
#error "HSI16 not enabled, required by STM32_MCOSEL"
#endif

#if ((STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI1) ||                            \
     (STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI2)) &&                           \
    (STM32_PLLSRC == STM32_PLLSRC_HSI16)
#error "HSI16 not enabled, required by STM32_SAI1SEL"
#endif

#if ((STM32_SAI2SEL == STM32_SAI1SEL_PLLSAI1) ||                            \
     (STM32_SAI2SEL == STM32_SAI1SEL_PLLSAI2)) &&                           \
    (STM32_PLLSRC == STM32_PLLSRC_HSI16)
#error "HSI16 not enabled, required by STM32_SAI2SEL"
#endif

#if (STM32_USART1SEL == STM32_USART1SEL_HSI16)
#error "HSI16 not enabled, required by STM32_USART1SEL"
#endif
#if (STM32_USART2SEL == STM32_USART2SEL_HSI16)
#error "HSI16 not enabled, required by STM32_USART2SEL"
#endif
#if (STM32_USART3SEL == STM32_USART3SEL_HSI16)
#error "HSI16 not enabled, required by STM32_USART3SEL"
#endif
#if (STM32_UART4SEL == STM32_UART4SEL_HSI16)
#error "HSI16 not enabled, required by STM32_UART4SEL"
#endif
#if (STM32_UART5SEL == STM32_UART5SEL_HSI16)
#error "HSI16 not enabled, required by STM32_UART5SEL"
#endif
#if (STM32_LPUART1SEL == STM32_LPUART1SEL_HSI16)
#error "HSI16 not enabled, required by STM32_LPUART1SEL"
#endif

#if (STM32_I2C1SEL == STM32_I2C1SEL_HSI16)
#error "HSI16 not enabled, required by I2C1SEL"
#endif
#if (STM32_I2C2SEL == STM32_I2C2SEL_HSI16)
#error "HSI16 not enabled, required by I2C2SEL"
#endif
#if (STM32_I2C3SEL == STM32_I2C3SEL_HSI16)
#error "HSI16 not enabled, required by I2C3SEL"
#endif

#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI16)
#error "HSI16 not enabled, required by LPTIM1SEL"
#endif
#if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_HSI16)
#error "HSI16 not enabled, required by LPTIM2SEL"
#endif

#if (STM32_STOPWUCK == STM32_STOPWUCK_HSI16)
#error "HSI16 not enabled, required by STM32_STOPWUCK"
#endif

#endif /* !STM32_HSI16_ENABLED */

#if STM32_HSI48_ENABLED
#else /* !STM32_HSI48_ENABLED */

#if STM32_MCOSEL == STM32_MCOSEL_HSI48
#error "HSI48 not enabled, required by STM32_MCOSEL"
#endif

#if STM32_CLK48SEL == STM32_CLK48SEL_HSI48
#error "HSI48 not enabled, required by STM32_CLK48SEL"
#endif
#endif /* !STM32_HSI48_ENABLED */

/*
 * HSE related checks.
 */
#if STM32_HSE_ENABLED

  #if STM32_HSECLK == 0
    #error "HSE frequency not defined"
  #else /* STM32_HSECLK != 0 */
    #if defined(STM32_HSE_BYPASS)
      #if (STM32_HSECLK < STM32_HSECLK_BYP_MIN) || (STM32_HSECLK > STM32_HSECLK_BYP_MAX)
        #error "STM32_HSECLK outside acceptable range (STM32_HSECLK_BYP_MIN...STM32_HSECLK_BYP_MAX)"
      #endif
    #else /* !defined(STM32_HSE_BYPASS) */
      #if (STM32_HSECLK < STM32_HSECLK_MIN) || (STM32_HSECLK > STM32_HSECLK_MAX)
        #error "STM32_HSECLK outside acceptable range (STM32_HSECLK_MIN...STM32_HSECLK_MAX)"
      #endif
    #endif /* !defined(STM32_HSE_BYPASS) */
  #endif /* STM32_HSECLK != 0 */

  #else /* !STM32_HSE_ENABLED */

    #if STM32_SW == STM32_SW_HSE
      #error "HSE not enabled, required by STM32_SW"
    #endif

    #if (STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_HSE)
      #error "HSE not enabled, required by STM32_SW and STM32_PLLSRC"
    #endif

    #if (STM32_MCOSEL == STM32_MCOSEL_HSE) ||                               \
        ((STM32_MCOSEL == STM32_MCOSEL_PLL) &&                              \
         (STM32_PLLSRC == STM32_PLLSRC_HSE))
      #error "HSE not enabled, required by STM32_MCOSEL"
    #endif

    #if ((STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI1) |                         \
         (STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI2)) &&                       \
        (STM32_PLLSRC == STM32_PLLSRC_HSE)
      #error "HSE not enabled, required by STM32_SAI1SEL"
    #endif

    #if ((STM32_SAI2SEL == STM32_SAI1SEL_PLLSAI1) |                         \
         (STM32_SAI2SEL == STM32_SAI1SEL_PLLSAI2)) &&                       \
        (STM32_PLLSRC == STM32_PLLSRC_HSE)
      #error "HSE not enabled, required by STM32_SAI2SEL"
    #endif

    #if STM32_RTCSEL == STM32_RTCSEL_HSEDIV
      #error "HSE not enabled, required by STM32_RTCSEL"
    #endif

#endif /* !STM32_HSE_ENABLED */

/*
 * LSI related checks.
 */
#if STM32_LSI_ENABLED
#else /* !STM32_LSI_ENABLED */

  #if STM32_RTCSEL == STM32_RTCSEL_LSI
    #error "LSI not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_MCOSEL == STM32_MCOSEL_LSI
    #error "LSI not enabled, required by STM32_MCOSEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSI
    #error "LSI not enabled, required by STM32_LSCOSEL"
  #endif

#endif /* !STM32_LSI_ENABLED */

/*
 * LSE related checks.
 */
#if STM32_LSE_ENABLED

  #if (STM32_LSECLK == 0)
    #error "LSE frequency not defined"
  #endif

  #if (STM32_LSECLK < STM32_LSECLK_MIN) || (STM32_LSECLK > STM32_LSECLK_MAX)
    #error "STM32_LSECLK outside acceptable range (STM32_LSECLK_MIN...STM32_LSECLK_MAX)"
  #endif

#else /* !STM32_LSE_ENABLED */

  #if STM32_RTCSEL == STM32_RTCSEL_LSE
    #error "LSE not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_MCOSEL == STM32_MCOSEL_LSE
    #error "LSE not enabled, required by STM32_MCOSEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSE
    #error "LSE not enabled, required by STM32_LSCOSEL"
  #endif

  #if STM32_MSIPLL_ENABLED == TRUE
    #error "LSE not enabled, required by STM32_MSIPLL_ENABLED"
  #endif

#endif /* !STM32_LSE_ENABLED */

/*
 * MSI related checks.
 */
#if (STM32_MSIRANGE == STM32_MSIRANGE_48M) && !STM32_MSIPLL_ENABLED
#warning "STM32_MSIRANGE_48M should be used with STM32_MSIPLL_ENABLED"
#endif

/**
 * @brief   STM32_PLLM field.
 */
#if ((STM32_PLLM_VALUE >= 1) && (STM32_PLLM_VALUE <= 16)) ||                 \
    defined(__DOXYGEN__)
#define STM32_PLLM                  ((STM32_PLLM_VALUE - 1) << 4)
#else
#error "invalid STM32_PLLM_VALUE value specified"
#endif

/**
 * @brief   PLL input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
#define STM32_PLLCLKIN              (STM32_HSECLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_MSI
#define STM32_PLLCLKIN              (STM32_MSICLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_HSI16
#define STM32_PLLCLKIN              (STM32_HSI16CLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_NOCLOCK
#define STM32_PLLCLKIN              0
      
#else
#error "invalid STM32_PLLSRC value specified"
#endif

/*
 * PLL input frequency range check.
 */
#if (STM32_PLLCLKIN != 0) &&                                                \
    ((STM32_PLLCLKIN < STM32_PLLIN_MIN) || (STM32_PLLCLKIN > STM32_PLLIN_MAX))
#error "STM32_PLLCLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/*
 * PLL enable check.
 */
#if (STM32_HSI48_ENABLED && (STM32_CLK48SEL == STM32_CLK48SEL_PLL)) ||      \
    (STM32_SW == STM32_SW_PLL) ||                                           \
    (STM32_MCOSEL == STM32_MCOSEL_PLL) ||                                   \
    (STM32_SAI1SEL == STM32_SAI1SEL_PLL) ||                                 \
    (STM32_SAI2SEL == STM32_SAI2SEL_PLL) ||                                 \
    defined(__DOXYGEN__)

#if STM32_PLLCLKIN == 0
#error "PLL activation required but no PLL clock selected"
#endif

/**
 * @brief   PLL activation flag.
 */
#define STM32_ACTIVATE_PLL          TRUE
#else
#define STM32_ACTIVATE_PLL          FALSE
#endif

/**
 * @brief   STM32_PLLN field.
 */
#if ((STM32_PLLN_VALUE >= 8) && (STM32_PLLN_VALUE <= 127)) ||                \
    defined(__DOXYGEN__)
#define STM32_PLLN                  (STM32_PLLN_VALUE << 8)
#else
#error "invalid STM32_PLLN_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLP field.
 */
#if (STM32_PLLP_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLLP                  (0 << 17)

#elif STM32_PLLP_VALUE == 17
#define STM32_PLLP                  (1 << 17)

#else
#error "invalid STM32_PLLP_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLQ field.
 */
#if (STM32_PLLQ_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLQ                  (0 << 21)

#elif STM32_PLLQ_VALUE == 4
#define STM32_PLLQ                  (1 << 21)

#elif STM32_PLLQ_VALUE == 6
#define STM32_PLLQ                  (2 << 21)

#elif STM32_PLLQ_VALUE == 8
#define STM32_PLLQ                  (3 << 21)

#else
#error "invalid STM32_PLLQ_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLR field.
 */
#if (STM32_PLLR_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLR                  (0 << 25)

#elif STM32_PLLR_VALUE == 4
#define STM32_PLLR                  (1 << 25)

#elif STM32_PLLR_VALUE == 6
#define STM32_PLLR                  (2 << 25)

#elif STM32_PLLR_VALUE == 8
#define STM32_PLLR                  (3 << 25)

#else
#error "invalid STM32_PLLR_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLPDIV field.
 */
#if (STM32_PLLPDIV_VALUE == 0) ||                                           \
    ((STM32_PLLPDIV_VALUE != 1) && (STM32_PLLPDIV_VALUE <= 31)) ||          \
    defined(__DOXYGEN__)
#define STM32_PLLPDIV               (STM32_PLLPDIV_VALUE << 27)
#else
#error "invalid STM32_PLLPDIV_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLPEN field.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_PLL) ||                                 \
    (STM32_SAI2SEL == STM32_SAI2SEL_PLL) ||                                 \
    defined(__DOXYGEN__)
#define STM32_PLLPEN                (1 << 16)
#else
#define STM32_PLLPEN                (0 << 16)
#endif

/**
 * @brief   STM32_PLLQEN field.
 */
#if (STM32_CLK48SEL == STM32_CLK48SEL_PLL) || defined(__DOXYGEN__)
#define STM32_PLLQEN                (1 << 20)
#else
#define STM32_PLLQEN                (0 << 20)
#endif

/**
 * @brief   STM32_PLLREN field.
 */
#if (STM32_SW == STM32_SW_PLL) ||                                           \
    (STM32_MCOSEL == STM32_MCOSEL_PLL) ||                                   \
    defined(__DOXYGEN__)
#define STM32_PLLREN                (1 << 24)
#else
#define STM32_PLLREN                (0 << 24)
#endif

/**
 * @brief   PLL VCO frequency.
 */
#define STM32_PLLVCO                (STM32_PLLCLKIN * STM32_PLLN_VALUE)

/*
 * PLL VCO frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLLVCO < STM32_PLLVCO_MIN) || (STM32_PLLVCO > STM32_PLLVCO_MAX))
#error "STM32_PLLVCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/**
 * @brief   PLL P output clock frequency.
 */
#if (STM32_PLLPDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLL_P_CLKOUT          (STM32_PLLVCO / STM32_PLLP_VALUE)
#else
#define STM32_PLL_P_CLKOUT          (STM32_PLLVCO / STM32_PLLPDIV_VALUE)
#endif

/**
 * @brief   PLL Q output clock frequency.
 */
#define STM32_PLL_Q_CLKOUT          (STM32_PLLVCO / STM32_PLLQ_VALUE)

/**
 * @brief   PLL R output clock frequency.
 */
#define STM32_PLL_R_CLKOUT          (STM32_PLLVCO / STM32_PLLR_VALUE)

/*
 * PLL-P output frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLL_P_CLKOUT < STM32_PLLP_MIN) || (STM32_PLL_P_CLKOUT > STM32_PLLP_MAX))
#error "STM32_PLL_P_CLKOUT outside acceptable range (STM32_PLLP_MIN...STM32_PLLP_MAX)"
#endif

/*
 * PLL-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLL_Q_CLKOUT < STM32_PLLQ_MIN) || (STM32_PLL_Q_CLKOUT > STM32_PLLQ_MAX))
#error "STM32_PLL_Q_CLKOUT outside acceptable range (STM32_PLLQ_MIN...STM32_PLLQ_MAX)"
#endif

/*
 * PLL-R output frequency range check.
 */
#if STM32_ACTIVATE_PLL &&                                                   \
    ((STM32_PLL_R_CLKOUT < STM32_PLLR_MIN) || (STM32_PLL_R_CLKOUT > STM32_PLLR_MAX))
#error "STM32_PLL_R_CLKOUT outside acceptable range (STM32_PLLR_MIN...STM32_PLLR_MAX)"
#endif

/**
 * @brief   System clock source.
 */
#if STM32_NO_INIT || defined(__DOXYGEN__)
#define STM32_SYSCLK                STM32_MSICLK

#elif (STM32_SW == STM32_SW_MSI)
#define STM32_SYSCLK                STM32_MSICLK

#elif (STM32_SW == STM32_SW_HSI16)
#define STM32_SYSCLK                STM32_HSI16CLK

#elif (STM32_SW == STM32_SW_HSE)
#define STM32_SYSCLK                STM32_HSECLK

#elif (STM32_SW == STM32_SW_PLL)
#define STM32_SYSCLK                STM32_PLL_R_CLKOUT

#else
#error "invalid STM32_SW value specified"
#endif

/* Check on the system clock.*/
#if STM32_SYSCLK > STM32_SYSCLK_MAX
#error "STM32_SYSCLK above maximum rated frequency (STM32_SYSCLK_MAX)"
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

/*
 * AHB frequency check.
 */
#if STM32_HCLK > STM32_SYSCLK_MAX
#error "STM32_HCLK exceeding maximum frequency (STM32_SYSCLK_MAX)"
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

/*
 * APB1 frequency check.
 */
#if STM32_PCLK1 > STM32_PCLK1_MAX
#error "STM32_PCLK1 exceeding maximum frequency (STM32_PCLK1_MAX)"
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

/*
 * APB2 frequency check.
 */
#if STM32_PCLK2 > STM32_PCLK2_MAX
#error "STM32_PCLK2 exceeding maximum frequency (STM32_PCLK2_MAX)"
#endif

/**
 * @brief   STM32_PLLSAI1M field.
 */
#if ((STM32_PLLSAI1M_VALUE >= 1) && (STM32_PLLSAI1M_VALUE <= 16)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAI1M              ((STM32_PLLSAI1M_VALUE - 1) << 4)
#else
#error "invalid STM32_PLLSAI1M_VALUE value specified"
#endif

/**
 * @brief   PLLSAI1 input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
#define STM32_PLLSAI1CLKIN          (STM32_HSECLK / STM32_PLLSAI1M_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_MSI
#define STM32_PLLSAI1CLKIN          (STM32_MSICLK / STM32_PLLSAI1M_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_HSI16
#define STM32_PLLSAI1CLKIN          (STM32_HSI16CLK / STM32_PLLSAI1M_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_NOCLOCK
#define STM32_PLLSAI1CLKIN          0

#else
#error "invalid STM32_PLLSRC value specified"
#endif

/*
 * PLLSAI1 input frequency range check.
 */
#if (STM32_PLLSAI1CLKIN != 0) &&                                            \
    ((STM32_PLLSAI1CLKIN < STM32_PLLIN_MIN) ||                              \
     (STM32_PLLSAI1CLKIN > STM32_PLLIN_MAX))
#error "STM32_PLLSAI1CLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/*
 * PLLSAI1 enable check.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI1) ||                             \
    (STM32_SAI2SEL == STM32_SAI2SEL_PLLSAI1) ||                             \
    (STM32_CLK48SEL == STM32_CLK48SEL_PLLSAI1) ||                           \
    (STM32_ADCSEL == STM32_ADCSEL_PLLSAI1) ||                               \
    defined(__DOXYGEN__)

#if STM32_PLLSAI1CLKIN == 0
#error "PLLSAI1 activation required but no PLL clock selected"
#endif

/**
 * @brief   PLLSAI1 activation flag.
 */
#define STM32_ACTIVATE_PLLSAI1      TRUE
#else
#define STM32_ACTIVATE_PLLSAI1      FALSE
#endif

/**
 * @brief   STM32_PLLSAI1N field.
 */
#if ((STM32_PLLSAI1N_VALUE >= 8) && (STM32_PLLSAI1N_VALUE <= 127)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAI1N               (STM32_PLLSAI1N_VALUE << 8)
#else
#error "invalid STM32_PLLSAI1N_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI1P field.
 */
#if (STM32_PLLSAI1P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLLSAI1P              (0 << 17)

#elif STM32_PLLSAI1P_VALUE == 17
#define STM32_PLLSAI1P              (1 << 17)

#else
#error "invalid STM32_PLLSAI1P_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI1Q field.
 */
#if (STM32_PLLSAI1Q_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI1Q              (0 << 21)

#elif STM32_PLLSAI1Q_VALUE == 4
#define STM32_PLLSAI1Q              (1 << 21)

#elif STM32_PLLSAI1Q_VALUE == 6
#define STM32_PLLSAI1Q              (2 << 21)

#elif STM32_PLLSAI1Q_VALUE == 8
#define STM32_PLLSAI1Q              (3 << 21)

#else
#error "invalid STM32_PLLSAI1Q_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI1R field.
 */
#if (STM32_PLLSAI1R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI1R              (0 << 25)

#elif STM32_PLLSAI1R_VALUE == 4
#define STM32_PLLSAI1R              (1 << 25)

#elif STM32_PLLSAI1R_VALUE == 6
#define STM32_PLLSAI1R              (2 << 25)

#elif STM32_PLLSAI1R_VALUE == 8
#define STM32_PLLSAI1R              (3 << 25)

#else
#error "invalid STM32_PLLSAI1R_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI1PDIV field.
 */
#if ((STM32_PLLSAI1PDIV_VALUE != 1) && (STM32_PLLSAI1PDIV_VALUE <= 31)) ||  \
    defined(__DOXYGEN__)
#define STM32_PLLSAI1PDIV           (STM32_PLLSAI1PDIV_VALUE << 27)
#else
#error "invalid STM32_PLLSAI1PDIV_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI1PEN field.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI1) ||                             \
    (STM32_SAI2SEL == STM32_SAI2SEL_PLLSAI1) ||                             \
    defined(__DOXYGEN__)
#define STM32_PLLSAI1PEN            (1 << 16)
#else
#define STM32_PLLSAI1PEN            (0 << 16)
#endif

/**
 * @brief   STM32_PLLSAI1QEN field.
 */
#if (STM32_CLK48SEL == STM32_CLK48SEL_PLLSAI1) || defined(__DOXYGEN__)
#define STM32_PLLSAI1QEN            (1 << 20)
#else
#define STM32_PLLSAI1QEN            (0 << 20)
#endif

/**
 * @brief   STM32_PLLSAI1REN field.
 */
#if (STM32_ADCSEL == STM32_ADCSEL_PLLSAI1) || defined(__DOXYGEN__)
#define STM32_PLLSAI1REN            (1 << 24)
#else
#define STM32_PLLSAI1REN            (0 << 24)
#endif

/**
 * @brief   PLLSAI1 VCO frequency.
 */
#define STM32_PLLSAI1VCO             (STM32_PLLSAI1CLKIN * STM32_PLLSAI1N_VALUE)

/*
 * PLLSAI1 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                               \
    ((STM32_PLLSAI1VCO < STM32_PLLVCO_MIN) || (STM32_PLLSAI1VCO > STM32_PLLVCO_MAX))
#error "STM32_PLLSAI1VCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/**
 * @brief   PLLSAI1-P output clock frequency.
 */
#if (STM32_PLLSAI1PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLLSAI1_P_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1P_VALUE)
#else
#define STM32_PLLSAI1_P_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1PDIV_VALUE)
#endif

/**
 * @brief   PLLSAI1-Q output clock frequency.
 */
#define STM32_PLLSAI1_Q_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1Q_VALUE)

/**
 * @brief   PLLSAI1-R output clock frequency.
 */
#define STM32_PLLSAI1_R_CLKOUT      (STM32_PLLSAI1VCO / STM32_PLLSAI1R_VALUE)

/*
 * PLLSAI1-P output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                               \
    ((STM32_PLLSAI1_P_CLKOUT < STM32_PLLP_MIN) || (STM32_PLLSAI1_P_CLKOUT > STM32_PLLP_MAX))
#error "STM32_PLLSAI1_P_CLKOUT outside acceptable range (STM32_PLLP_MIN...STM32_PLLP_MAX)"
#endif

/*
 * PLLSAI1-Q output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                               \
    ((STM32_PLLSAI1_Q_CLKOUT < STM32_PLLQ_MIN) || (STM32_PLLSAI1_Q_CLKOUT > STM32_PLLQ_MAX))
#error "STM32_PLLSAI1_Q_CLKOUT outside acceptable range (STM32_PLLQ_MIN...STM32_PLLQ_MAX)"
#endif

/*
 * PLLSAI1-R output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI1 &&                                               \
    ((STM32_PLLSAI1_R_CLKOUT < STM32_PLLR_MIN) || (STM32_PLLSAI1_R_CLKOUT > STM32_PLLR_MAX))
#error "STM32_PLLSAI1_R_CLKOUT outside acceptable range (STM32_PLLR_MIN...STM32_PLLR_MAX)"
#endif

/**
 * @brief   STM32_PLLSAI2M field.
 */
#if ((STM32_PLLSAI2M_VALUE >= 1) && (STM32_PLLSAI2M_VALUE <= 16)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAI2M              ((STM32_PLLSAI2M_VALUE - 1) << 4)
#else
#error "invalid STM32_PLLSAI2M_VALUE value specified"
#endif

/**
 * @brief   PLLSAI2 input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
#define STM32_PLLSAI2CLKIN          (STM32_HSECLK / STM32_PLLSAI2M_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_MSI
#define STM32_PLLSAI2CLKIN          (STM32_MSICLK / STM32_PLLSAI2M_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_HSI16
#define STM32_PLLSAI2CLKIN          (STM32_HSI16CLK / STM32_PLLSAI2M_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_NOCLOCK
#define STM32_PLLSAI2CLKIN          0

#else
#error "invalid STM32_PLLSRC value specified"
#endif

/*
 * PLLSAI2 input frequency range check.
 */
#if (STM32_PLLSAI2CLKIN != 0) &&                                            \
    ((STM32_PLLSAI2CLKIN < STM32_PLLIN_MIN) ||                              \
     (STM32_PLLSAI2CLKIN > STM32_PLLIN_MAX))
#error "STM32_PLLSAI2CLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/*
 * PLLSAI2 enable check.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI2) ||                             \
    (STM32_SAI2SEL == STM32_SAI2SEL_PLLSAI2) ||                             \
    (STM32_ADCSEL == STM32_ADCSEL_PLLSAI1) ||                               \
    defined(__DOXYGEN__)

#if STM32_PLLSAI2CLKIN == 0
#error "PLLSAI2 activation required but no PLL clock selected"
#endif

/**
 * @brief   PLLSAI2 activation flag.
 */
#define STM32_ACTIVATE_PLLSAI2      TRUE
#else
#define STM32_ACTIVATE_PLLSAI2      FALSE
#endif

/**
 * @brief   STM32_PLLSAI2N field.
 */
#if ((STM32_PLLSAI2N_VALUE >= 8) && (STM32_PLLSAI2N_VALUE <= 127)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAI2N               (STM32_PLLSAI2N_VALUE << 8)
#else
#error "invalid STM32_PLLSAI2N_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI2P field.
 */
#if (STM32_PLLSAI2P_VALUE == 7) || defined(__DOXYGEN__)
#define STM32_PLLSAI2P              (0 << 17)

#elif STM32_PLLSAI2P_VALUE == 17
#define STM32_PLLSAI2P              (1 << 17)

#else
#error "invalid STM32_PLLSAI2P_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI2R field.
 */
#if (STM32_PLLSAI2R_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAI2R              (0 << 25)

#elif STM32_PLLSAI2R_VALUE == 4
#define STM32_PLLSAI2R              (1 << 25)

#elif STM32_PLLSAI2R_VALUE == 6
#define STM32_PLLSAI2R              (2 << 25)

#elif STM32_PLLSAI2R_VALUE == 8
#define STM32_PLLSAI2R              (3 << 25)

#else
#error "invalid STM32_PLLSAI2R_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI2PDIV field.
 */
#if ((STM32_PLLSAI2PDIV_VALUE != 1) && (STM32_PLLSAI2PDIV_VALUE <= 31)) ||  \
    defined(__DOXYGEN__)
#define STM32_PLLSAI2PDIV           (STM32_PLLSAI2PDIV_VALUE << 27)
#else
#error "invalid STM32_PLLSAI2PDIV_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAI2PEN field.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_PLLSAI2) ||                             \
    (STM32_SAI2SEL == STM32_SAI2SEL_PLLSAI2) ||                             \
    defined(__DOXYGEN__)
#define STM32_PLLSAI2PEN            (1 << 16)
#else
#define STM32_PLLSAI2PEN            (0 << 16)
#endif

/**
 * @brief   STM32_PLLSAI2REN field.
 * @note    Always enabled.
 * @todo    It should depend on some condition.
 */
#define STM32_PLLSAI2REN            (1 << 24)

/**
 * @brief   PLLSAI2 VCO frequency.
 */
#define STM32_PLLSAI2VCO             (STM32_PLLSAI2CLKIN * STM32_PLLSAI2N_VALUE)

/*
 * PLLSAI2 VCO frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                               \
    ((STM32_PLLSAI2VCO < STM32_PLLVCO_MIN) || (STM32_PLLSAI2VCO > STM32_PLLVCO_MAX))
#error "STM32_PLLSAI2VCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/**
 * @brief   PLLSAI2-P output clock frequency.
 */
#if (STM32_PLLSAI2PDIV_VALUE == 0) || defined(__DOXYGEN__)
#define STM32_PLLSAI2_P_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2P_VALUE)
#else
#define STM32_PLLSAI2_P_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2PDIV_VALUE)
#endif

/**
 * @brief   PLLSAI2-R output clock frequency.
 */
#define STM32_PLLSAI2_R_CLKOUT      (STM32_PLLSAI2VCO / STM32_PLLSAI2R_VALUE)

/*
 * PLLSAI2-P output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                               \
    ((STM32_PLLSAI2_P_CLKOUT < STM32_PLLP_MIN) || (STM32_PLLSAI2_P_CLKOUT > STM32_PLLP_MAX))
#error "STM32_PLLSAI2_P_CLKOUT outside acceptable range (STM32_PLLP_MIN...STM32_PLLP_MAX)"
#endif

/*
 * PLLSAI2-R output frequency range check.
 */
#if STM32_ACTIVATE_PLLSAI2 &&                                               \
    ((STM32_PLLSAI2_R_CLKOUT < STM32_PLLR_MIN) || (STM32_PLLSAI2_R_CLKOUT > STM32_PLLR_MAX))
#error "STM32_PLLSAI2_R_CLKOUT outside acceptable range (STM32_PLLR_MIN...STM32_PLLR_MAX)"
#endif

/**
 * @brief   MCO divider clock frequency.
 */
#if (STM32_MCOSEL == STM32_MCOSEL_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_MCODIVCLK             0

#elif STM32_MCOSEL == STM32_MCOSEL_SYSCLK
#define STM32_MCODIVCLK             STM32_SYSCLK

#elif STM32_MCOSEL == STM32_MCOSEL_MSI
#define STM32_MCODIVCLK             STM32_MSICLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSI16
#define STM32_MCODIVCLK             STM32_HSI16CLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSE
#define STM32_MCODIVCLK             STM32_HSECLK

#elif STM32_MCOSEL == STM32_MCOSEL_PLL
#define STM32_MCODIVCLK             STM32_PLL_P_CLKOUT

#elif STM32_MCOSEL == STM32_MCOSEL_LSI
#define STM32_MCODIVCLK             STM32_LSICLK

#elif STM32_MCOSEL == STM32_MCOSEL_LSE
#define STM32_MCODIVCLK             STM32_LSECLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSI48
#define STM32_MCODIVCLK             STM32_HSI48CLK

#else
#error "invalid STM32_MCOSEL value specified"
#endif

/**
 * @brief   MCO output pin clock frequency.
 */
#if (STM32_MCOPRE == STM32_MCOPRE_DIV1) || defined(__DOXYGEN__)
#define STM32_MCOCLK                STM32_MCODIVCLK

#elif STM32_MCOPRE == STM32_MCOPRE_DIV2
#define STM32_MCOCLK                (STM32_MCODIVCLK / 2)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV4
#define STM32_MCOCLK                (STM32_MCODIVCLK / 4)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV8
#define STM32_MCOCLK                (STM32_MCODIVCLK / 8)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV16
#define STM32_MCOCLK                (STM32_MCODIVCLK / 16)

#else
#error "invalid STM32_MCOPRE value specified"
#endif

/**
 * @brief   RTC clock frequency.
 */
#if (STM32_RTCSEL == STM32_RTCSEL_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_RTCCLK                0

#elif STM32_RTCSEL == STM32_RTCSEL_LSE
#define STM32_RTCCLK                STM32_LSECLK

#elif STM32_RTCSEL == STM32_RTCSEL_LSI
#define STM32_RTCCLK                STM32_LSICLK

#elif STM32_RTCSEL == STM32_RTCSEL_HSEDIV
#define STM32_RTCCLK                (STM32_HSECLK / 32)

#else
#error "invalid STM32_RTCSEL value specified"
#endif

/**
 * @brief   USART1 clock frequency.
 */
#if (STM32_USART1SEL == STM32_USART1SEL_PCLK2) || defined(__DOXYGEN__)
#define STM32_USART1CLK             STM32_PCLK2
#elif STM32_USART1SEL == STM32_USART1SEL_SYSCLK
#define STM32_USART1CLK             STM32_SYSCLK
#elif STM32_USART1SEL == STM32_USART1SEL_HSI16
#define STM32_USART1CLK             STM32_HSI16CLK
#elif STM32_USART1SEL == STM32_USART1SEL_LSE
#define STM32_USART1CLK             STM32_LSECLK
#else
#error "invalid source selected for USART1 clock"
#endif

/**
 * @brief   USART2 clock frequency.
 */
#if (STM32_USART2SEL == STM32_USART2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART2CLK             STM32_PCLK1
#elif STM32_USART2SEL == STM32_USART2SEL_SYSCLK
#define STM32_USART2CLK             STM32_SYSCLK
#elif STM32_USART2SEL == STM32_USART2SEL_HSI16
#define STM32_USART2CLK             STM32_HSI16CLK
#elif STM32_USART2SEL == STM32_USART2SEL_LSE
#define STM32_USART2CLK             STM32_LSECLK
#else
#error "invalid source selected for USART2 clock"
#endif

/**
 * @brief   USART3 clock frequency.
 */
#if (STM32_USART3SEL == STM32_USART3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART3CLK             STM32_PCLK1
#elif STM32_USART3SEL == STM32_USART3SEL_SYSCLK
#define STM32_USART3CLK             STM32_SYSCLK
#elif STM32_USART3SEL == STM32_USART3SEL_HSI16
#define STM32_USART3CLK             STM32_HSI16CLK
#elif STM32_USART3SEL == STM32_USART3SEL_LSE
#define STM32_USART3CLK             STM32_LSECLK
#else
#error "invalid source selected for USART3 clock"
#endif

/**
 * @brief   UART4 clock frequency.
 */
#if (STM32_UART4SEL == STM32_UART4SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART4CLK              STM32_PCLK1
#elif STM32_UART4SEL == STM32_UART4SEL_SYSCLK
#define STM32_UART4CLK              STM32_SYSCLK
#elif STM32_UART4SEL == STM32_UART4SEL_HSI16
#define STM32_UART4CLK              STM32_HSI16CLK
#elif STM32_UART4SEL == STM32_UART4SEL_LSE
#define STM32_UART4CLK              STM32_LSECLK
#else
#error "invalid source selected for UART4 clock"
#endif

/**
 * @brief   UART5 clock frequency.
 */
#if (STM32_UART5SEL == STM32_UART5SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART5CLK              STM32_PCLK1
#elif STM32_UART5SEL == STM32_UART5SEL_SYSCLK
#define STM32_UART5CLK              STM32_SYSCLK
#elif STM32_UART5SEL == STM32_UART5SEL_HSI16
#define STM32_UART5CLK              STM32_HSI16CLK
#elif STM32_UART5SEL == STM32_UART5SEL_LSE
#define STM32_UART5CLK              STM32_LSECLK
#else
#error "invalid source selected for UART5 clock"
#endif

/**
 * @brief   LPUART1 clock frequency.
 */
#if (STM32_LPUART1SEL == STM32_LPUART1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_LPUART1CLK            STM32_PCLK1
#elif STM32_LPUART1SEL == STM32_LPUART1SEL_SYSCLK
#define STM32_LPUART1CLK            STM32_SYSCLK
#elif STM32_LPUART1SEL == STM32_LPUART1SEL_HSI16
#define STM32_LPUART1CLK            STM32_HSI16CLK
#elif STM32_LPUART1SEL == STM32_LPUART1SEL_LSE
#define STM32_LPUART1CLK            STM32_LSECLK
#else
#error "invalid source selected for LPUART1 clock"
#endif

/**
 * @brief   I2C1 clock frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C1CLK               STM32_PCLK1
#elif STM32_I2C1SEL == STM32_I2C1SEL_SYSCLK
#define STM32_I2C1CLK               STM32_SYSCLK
#elif STM32_I2C1SEL == STM32_I2C1SEL_HSI16
#define STM32_I2C1CLK               STM32_HSI16CLK
#else
#error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C2 clock frequency.
 */
#if (STM32_I2C2SEL == STM32_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C2CLK               STM32_PCLK1
#elif STM32_I2C2SEL == STM32_I2C2SEL_SYSCLK
#define STM32_I2C2CLK               STM32_SYSCLK
#elif STM32_I2C2SEL == STM32_I2C2SEL_HSI16
#define STM32_I2C2CLK               STM32_HSI16CLK
#else
#error "invalid source selected for I2C2 clock"
#endif

/**
 * @brief   I2C3 clock frequency.
 */
#if (STM32_I2C3SEL == STM32_I2C3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C3CLK               STM32_PCLK1
#elif STM32_I2C3SEL == STM32_I2C3SEL_SYSCLK
#define STM32_I2C3CLK               STM32_SYSCLK
#elif STM32_I2C3SEL == STM32_I2C3SEL_HSI16
#define STM32_I2C3CLK               STM32_HSI16CLK
#else
#error "invalid source selected for I2C3 clock"
#endif

/**
 * @brief   LPTIM1 clock frequency.
 */
#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_LPTIM1CLK             STM32_PCLK1
#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI
#define STM32_LPTIM1CLK             STM32_LSICLK
#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI16
#define STM32_LPTIM1CLK             STM32_HSI16CLK
#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE
#define STM32_LPTIM1CLK             STM32_LSECLK
#else
#error "invalid source selected for LPTIM1 clock"
#endif

/**
 * @brief   LPTIM2 clock frequency.
 */
#if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_LPTIM2CLK             STM32_PCLK1
#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSI
#define STM32_LPTIM2CLK             STM32_LSICLK
#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_HSI16
#define STM32_LPTIM2CLK             STM32_HSI16CLK
#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSE
#define STM32_LPTIM2CLK             STM32_LSECLK
#else
#error "invalid source selected for LPTIM2 clock"
#endif

/**
 * @brief   48MHz clock frequency.
 */
#if (STM32_CLK48SEL == STM32_CLK48SEL_HSI48) || defined(__DOXYGEN__)
#define STM32_48CLK                 STM32_HSI48CLK
#elif STM32_CLK48SEL == STM32_CLK48SEL_PLLSAI1
#define STM32_48CLK                 (STM32_PLLSAI1VCO / STM32_PLLSAI1Q_VALUE)
#elif STM32_CLK48SEL == STM32_CLK48SEL_PLL
#define STM32_48CLK                 (STM32_PLLVCO / STM32_PLLQ_VALUE)
#elif STM32_CLK48SEL == STM32_CLK48SEL_MSI
#define STM32_48CLK                 STM32_MSICLK
#else
#error "invalid source selected for 48CLK clock"
#endif

/**
 * @brief   USB clock point.
 */
#define STM32_USBCLK                STM32_48CLK

/**
 * @brief   RNG clock point.
 */
#define STM32_RNGCLK                STM32_48CLK

/**
 * @brief   ADC clock frequency.
 */
#if (STM32_ADCSEL == STM32_ADCSEL_NOCLK) || defined(__DOXYGEN__)
#define STM32_ADCCLK                0
#elif STM32_ADCSEL == STM32_ADCSEL_PLLSAI1
#define STM32_ADCCLK                STM32_PLLSAI1_R_CLKOUT
#elif STM32_ADCSEL == STM32_ADCSEL_SYSCLK
#define STM32_ADCCLK                STM32_SYSCLK
#else
#error "invalid source selected for ADC clock"
#endif

/**
 * @brief   DFSDM clock frequency.
 */
#if (STM32_DFSDMSEL == STM32_DFSDMSEL_PCLK2) || defined(__DOXYGEN__)
#define STM32_DFSDMCLK              STM32_PCLK2
#elif STM32_DFSDMSEL == STM32_DFSDMSEL_SYSCLK
#define STM32_DFSDMCLK              STM32_SYSCLK
#else
#error "invalid source selected for DFSDM clock"
#endif

/**
 * @brief   SDMMC frequency.
 */
#define STM32_SDMMC1CLK             STM32_48CLK

/**
 * @brief   Clock of timers connected to APB1
 */
#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
#define STM32_TIMCLK1               (STM32_PCLK1 * 1)
#else
#define STM32_TIMCLK1               (STM32_PCLK1 * 2)
#endif

/**
 * @brief   Clock of timers connected to APB2.
 */
#if (STM32_PPRE2 == STM32_PPRE2_DIV1) || defined(__DOXYGEN__)
#define STM32_TIMCLK2               (STM32_PCLK2 * 1)
#else
#define STM32_TIMCLK2               (STM32_PCLK2 * 2)
#endif

/**
 * @brief   Voltage boost settings.
 */
#if (STM32_SYSCLK <= STM32_SYSCLK_NOBOOST_MAX) || defined(__DOXYGEN__)
#define STM32_R1MODE                PWR_CR5_R1MODE
#else
#define STM32_R1MODE                0
#endif

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
#define STM32_FLASHBITS             FLASH_ACR_LATENCY_0WS

#elif STM32_HCLK <= STM32_1WS_THRESHOLD
#define STM32_FLASHBITS             FLASH_ACR_LATENCY_1WS

#elif STM32_HCLK <= STM32_2WS_THRESHOLD
#define STM32_FLASHBITS             FLASH_ACR_LATENCY_2WS

#elif STM32_HCLK <= STM32_3WS_THRESHOLD
#define STM32_FLASHBITS             FLASH_ACR_LATENCY_3WS

#else
#define STM32_FLASHBITS             FLASH_ACR_LATENCY_4WS
#endif

/**
 * @brief   Flash settings for MSI.
 */
#if (STM32_MSICLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
#define STM32_MSI_FLASHBITS         FLASH_ACR_LATENCY_0WS

#elif STM32_MSICLK <= STM32_1WS_THRESHOLD
#define STM32_MSI_FLASHBITS         FLASH_ACR_LATENCY_1WS

#elif STM32_MSICLK <= STM32_2WS_THRESHOLD
#define STM32_MSI_FLASHBITS         FLASH_ACR_LATENCY_2WS

#elif STM32_MSICLK <= STM32_3WS_THRESHOLD
#define STM32_MSI_FLASHBITS         FLASH_ACR_LATENCY_3WS

#else
#define STM32_MSI_FLASHBITS         FLASH_ACR_LATENCY_4WS
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

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "mpu_v7m.h"
#include "stm32_isr.h"
#include "stm32_dma.h"
#include "stm32_exti.h"
#include "stm32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
