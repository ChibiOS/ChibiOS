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

/**
 * @name    Dynamic clock point indexes and names
 * @{
 */
#define CLK_HSI16               0U
#define CLK_HSI48               1U
#define CLK_HSE                 2U
#define CLK_PLLP                3U
#define CLK_PLLQ                4U
#define CLK_PLLR                5U
#define CLK_SYSCLK              6U
#define CLK_HCLK                7U
#define CLK_PCLK1               8U
#define CLK_PCLK2               9U
#define CLK_PCLK1TIM            10U
#define CLK_PCLK2TIM            11U
#define CLK_MCO                 12U
#define CLK_ARRAY_SIZE          13U

#define CLK_POINT_NAMES                                                     \
  {                                                                         \
    "HSI16",                                                                \
    "HSI48",                                                                \
    "HSE",                                                                  \
    "PLLP",                                                                 \
    "PLLQ",                                                                 \
    "PLLR",                                                                 \
    "SYSCLK",                                                               \
    "HCLK",                                                                 \
    "PCLK1",                                                                \
    "PCLK2",                                                                \
    "PCLK1TIM",                                                             \
    "PCLK2TIM",                                                             \
    "MCO"                                                                   \
  }
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Clock tree configurations
 * @{
 */
/**
 * @brief   Enables dynamic clock handling.
 */
#if !defined(STM32_CFG_CLOCK_DYNAMIC) || defined(__DOXYGEN__)
#define STM32_CFG_CLOCK_DYNAMIC             FALSE
#endif

/**
 * @brief   Selects the core voltage scaling range.
 */
#if !defined(STM32_CFG_PWR_VOS) || defined(__DOXYGEN__)
#define STM32_CFG_PWR_VOS                   STM32_VOS_RANGE1
#endif

/**
 * @brief   Enables the power boost mode.
 */
#if !defined(STM32_CFG_PWR_BOOST) || defined(__DOXYGEN__)
#define STM32_CFG_PWR_BOOST                 TRUE
#endif

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
#define STM32_CFG_PLLIN_SEL                 STM32_PLLSRC_HSI16
#endif

/**
 * @brief   Configures the PLLREF clock divider value.
 */
#if !defined(STM32_CFG_PLLREF_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLREF_VALUE              4
#endif

/**
 * @brief   Configures the PLLVCO clock multiplier value.
 */
#if !defined(STM32_CFG_PLLVCO_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLVCO_VALUE              84
#endif

/**
 * @brief   Configures the PLLP clock divider value.
 */
#if !defined(STM32_CFG_PLLP_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLP_VALUE                7
#endif

/**
 * @brief   Configures the PLLQ clock divider value.
 */
#if !defined(STM32_CFG_PLLQ_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PLLQ_VALUE                8
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
#define STM32_CFG_SYSCLK_SEL                STM32_SW_PLLRCLK
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
#define STM32_CFG_PCLK1_VALUE               2
#endif

/**
 * @brief   Configures the PCLK2 clock divider value.
 */
#if !defined(STM32_CFG_PCLK2_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_PCLK2_VALUE               1
#endif

/**
 * @brief   Selects the MCODIV clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - SYSCLK.
 *          - HSI16.
 *          - HSE.
 *          - PLLR.
 *          - LSI.
 *          - LSE.
 *          - HSI48.
 */
#if !defined(STM32_CFG_MCODIV_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_MCODIV_SEL                STM32_MCOSEL_NOCLOCK
#endif

/**
 * @brief   Configures the MCO clock divider value.
 */
#if !defined(STM32_CFG_MCO_VALUE) || defined(__DOXYGEN__)
#define STM32_CFG_MCO_VALUE                 1
#endif

/**
 * @brief   Selects the LSCO clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - LSI.
 *          - LSE.
 */
#if !defined(STM32_CFG_LSCO_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_LSCO_SEL                  STM32_LSCOSEL_NOCLOCK
#endif

/**
 * @brief   Selects the RTC clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - LSE.
 *          - LSI.
 *          - HSEDIV.
 */
#if !defined(STM32_CFG_RTC_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_RTC_SEL                   STM32_RTCSEL_NOCLOCK
#endif

/**
 * @brief   Selects the USART1 clock source.
 * @note    Allowed sources:
 *          - PCLK2.
 *          - SYSCLK.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_USART1_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_USART1_SEL                STM32_USART1SEL_SYSCLK
#endif

/**
 * @brief   Selects the USART2 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_USART2_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_USART2_SEL                STM32_USART2SEL_SYSCLK
#endif

/**
 * @brief   Selects the USART3 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_USART3_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_USART3_SEL                STM32_USART3SEL_SYSCLK
#endif

/**
 * @brief   Selects the UART4 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_UART4_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_UART4_SEL                 STM32_UART4SEL_SYSCLK
#endif

/**
 * @brief   Selects the UART5 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_UART5_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_UART5_SEL                 STM32_UART5SEL_SYSCLK
#endif

/**
 * @brief   Selects the LPUART1 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_LPUART1_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_LPUART1_SEL               STM32_LPUART1SEL_SYSCLK
#endif

/**
 * @brief   Selects the I2C1 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 */
#if !defined(STM32_CFG_I2C1_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_I2C1_SEL                  STM32_I2C1SEL_PCLK1
#endif

/**
 * @brief   Selects the I2C2 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 */
#if !defined(STM32_CFG_I2C2_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_I2C2_SEL                  STM32_I2C2SEL_PCLK1
#endif

/**
 * @brief   Selects the I2C3 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 */
#if !defined(STM32_CFG_I2C3_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_I2C3_SEL                  STM32_I2C3SEL_PCLK1
#endif

/**
 * @brief   Selects the I2C4 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - SYSCLK.
 *          - HSI16.
 */
#if !defined(STM32_CFG_I2C4_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_I2C4_SEL                  STM32_I2C4SEL_PCLK1
#endif

/**
 * @brief   Selects the LPTIM1 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - LSI.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_LPTIM1_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_LPTIM1_SEL                STM32_LPTIM1SEL_PCLK1
#endif

/**
 * @brief   Selects the SAI1 clock source.
 * @note    Allowed sources:
 *          - SYSCLK.
 *          - PLLQ.
 *          - CKIN.
 *          - HSI16.
 */
#if !defined(STM32_CFG_SAI1_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_SAI1_SEL                  STM32_SAI1SEL_SYSCLK
#endif

/**
 * @brief   Selects the I2S23 clock source.
 * @note    Allowed sources:
 *          - SYSCLK.
 *          - PLLQ.
 *          - CKIN.
 *          - HSI16.
 */
#if !defined(STM32_CFG_I2S23_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_I2S23_SEL                 STM32_I2S23SEL_SYSCLK
#endif

/**
 * @brief   Selects the FDCAN clock source.
 * @note    Allowed sources:
 *          - HSE.
 *          - PLLQ.
 *          - PCLK1.
 */
#if !defined(STM32_CFG_FDCAN_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_FDCAN_SEL                 STM32_FDCANSEL_HSE
#endif

/**
 * @brief   Selects the CLK48 clock source.
 * @note    Allowed sources:
 *          - HSI48.
 *          - PLLQ.
 */
#if !defined(STM32_CFG_CLK48_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_CLK48_SEL                 STM32_CLK48SEL_HSI48
#endif

/**
 * @brief   Selects the ADC12 clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - PLLP.
 *          - SYSCLK.
 */
#if !defined(STM32_CFG_ADC12_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_ADC12_SEL                 STM32_ADC12SEL_PLLPCLK
#endif

/**
 * @brief   Selects the ADC345 clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - PLLP.
 *          - SYSCLK.
 */
#if !defined(STM32_CFG_ADC345_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_ADC345_SEL                STM32_ADC345SEL_PLLPCLK
#endif

/**
 * @brief   Selects the QSPI clock source.
 * @note    Allowed sources:
 *          - SYSCLK.
 *          - HSI16.
 *          - PLLQ.
 */
#if !defined(STM32_CFG_QSPI_SEL) || defined(__DOXYGEN__)
#define STM32_CFG_QSPI_SEL                  STM32_QSPISEL_SYSCLK
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @name    Clock point derived constants and checks
 * @{
 */
/*
 * Extra configuration checks.
 */
#if (STM32_CFG_CLOCK_DYNAMIC != TRUE) && \
    (STM32_CFG_CLOCK_DYNAMIC != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_CLOCK_DYNAMIC value specified"
#endif

#if (STM32_CFG_PWR_VOS != STM32_VOS_RANGE1) && \
    (STM32_CFG_PWR_VOS != STM32_VOS_RANGE2) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PWR_VOS value specified"
#endif

#if (STM32_CFG_PWR_BOOST != TRUE) && \
    (STM32_CFG_PWR_BOOST != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PWR_BOOST value specified"
#endif

/**
 * @name    Frequency limits for boost state
 * @{
 */
#if defined(STM32_HSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_BOOST_HSECLK_MIN              8000000
#define STM32_BOOST_HSECLK_MAX              48000000
#else
#define STM32_BOOST_HSECLK_MIN              8000000
#define STM32_BOOST_HSECLK_MAX              48000000
#endif
#if defined(STM32_LSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_BOOST_LSECLK_MIN              32768
#define STM32_BOOST_LSECLK_MAX              1000000
#else
#define STM32_BOOST_LSECLK_MIN              32768
#define STM32_BOOST_LSECLK_MAX              32768
#endif
#define STM32_BOOST_PLLIN_MIN               2660000
#define STM32_BOOST_PLLIN_MAX               16000000
#define STM32_BOOST_PLLVCO_MIN              96000000
#define STM32_BOOST_PLLVCO_MAX              344000000
#define STM32_BOOST_PLLP_MIN                2064500
#define STM32_BOOST_PLLP_MAX                170000000
#define STM32_BOOST_PLLQ_MIN                8000000
#define STM32_BOOST_PLLQ_MAX                170000000
#define STM32_BOOST_PLLR_MIN                8000000
#define STM32_BOOST_PLLR_MAX                170000000
#define STM32_BOOST_SYSCLK_MAX              170000000
#define STM32_BOOST_PCLK1_MAX               170000000
#define STM32_BOOST_PCLK2_MAX               170000000
#define STM32_BOOST_ADCCLK_MAX              60000000
#define STM32_BOOST_FLASH_0WS_MAX           34000000
#define STM32_BOOST_FLASH_1WS_MAX           68000000
#define STM32_BOOST_FLASH_2WS_MAX           102000000
#define STM32_BOOST_FLASH_3WS_MAX           136000000
#define STM32_BOOST_FLASH_4WS_MAX           170000000
/** @} */

/**
 * @name    Frequency limits for vos1 state
 * @{
 */
#if defined(STM32_HSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_HSECLK_MIN               8000000
#define STM32_VOS1_HSECLK_MAX               48000000
#else
#define STM32_VOS1_HSECLK_MIN               8000000
#define STM32_VOS1_HSECLK_MAX               48000000
#endif
#if defined(STM32_LSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_LSECLK_MIN               32768
#define STM32_VOS1_LSECLK_MAX               1000000
#else
#define STM32_VOS1_LSECLK_MIN               32768
#define STM32_VOS1_LSECLK_MAX               32768
#endif
#define STM32_VOS1_PLLIN_MIN                2660000
#define STM32_VOS1_PLLIN_MAX                16000000
#define STM32_VOS1_PLLVCO_MIN               96000000
#define STM32_VOS1_PLLVCO_MAX               344000000
#define STM32_VOS1_PLLP_MIN                 2064500
#define STM32_VOS1_PLLP_MAX                 150000000
#define STM32_VOS1_PLLQ_MIN                 8000000
#define STM32_VOS1_PLLQ_MAX                 150000000
#define STM32_VOS1_PLLR_MIN                 8000000
#define STM32_VOS1_PLLR_MAX                 150000000
#define STM32_VOS1_SYSCLK_MAX               150000000
#define STM32_VOS1_PCLK1_MAX                150000000
#define STM32_VOS1_PCLK2_MAX                150000000
#define STM32_VOS1_ADCCLK_MAX               60000000
#define STM32_VOS1_FLASH_0WS_MAX            30000000
#define STM32_VOS1_FLASH_1WS_MAX            60000000
#define STM32_VOS1_FLASH_2WS_MAX            90000000
#define STM32_VOS1_FLASH_3WS_MAX            120000000
#define STM32_VOS1_FLASH_4WS_MAX            150000000
/** @} */

/**
 * @name    Frequency limits for vos2 state
 * @{
 */
#if defined(STM32_HSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS2_HSECLK_MIN               8000000
#define STM32_VOS2_HSECLK_MAX               26000000
#else
#define STM32_VOS2_HSECLK_MIN               8000000
#define STM32_VOS2_HSECLK_MAX               26000000
#endif
#if defined(STM32_LSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS2_LSECLK_MIN               32768
#define STM32_VOS2_LSECLK_MAX               1000000
#else
#define STM32_VOS2_LSECLK_MIN               32768
#define STM32_VOS2_LSECLK_MAX               32768
#endif
#define STM32_VOS2_PLLIN_MIN                2660000
#define STM32_VOS2_PLLIN_MAX                16000000
#define STM32_VOS2_PLLVCO_MIN               96000000
#define STM32_VOS2_PLLVCO_MAX               128000000
#define STM32_VOS2_PLLP_MIN                 2064500
#define STM32_VOS2_PLLP_MAX                 26000000
#define STM32_VOS2_PLLQ_MIN                 8000000
#define STM32_VOS2_PLLQ_MAX                 26000000
#define STM32_VOS2_PLLR_MIN                 8000000
#define STM32_VOS2_PLLR_MAX                 26000000
#define STM32_VOS2_SYSCLK_MAX               26000000
#define STM32_VOS2_PCLK1_MAX                26000000
#define STM32_VOS2_PCLK2_MAX                26000000
#define STM32_VOS2_ADCCLK_MAX               26000000
#define STM32_VOS2_FLASH_0WS_MAX            12000000
#define STM32_VOS2_FLASH_1WS_MAX            24000000
#define STM32_VOS2_FLASH_2WS_MAX            26000000
#define STM32_VOS2_FLASH_3WS_MAX            0
#define STM32_VOS2_FLASH_4WS_MAX            0
/** @} */

/*
 * Selected frequency limits.
 */
#if ((STM32_CFG_PWR_VOS == STM32_VOS_RANGE1) && \
     (STM32_CFG_PWR_BOOST == TRUE)) || \
    defined(__DOXYGEN__)
#define STM32_HSECLK_MIN                    STM32_BOOST_HSECLK_MIN
#define STM32_HSECLK_MAX                    STM32_BOOST_HSECLK_MAX
#define STM32_LSECLK_MIN                    STM32_BOOST_LSECLK_MIN
#define STM32_LSECLK_MAX                    STM32_BOOST_LSECLK_MAX
#define STM32_PLLIN_MIN                     STM32_BOOST_PLLIN_MIN
#define STM32_PLLIN_MAX                     STM32_BOOST_PLLIN_MAX
#define STM32_PLLVCO_MIN                    STM32_BOOST_PLLVCO_MIN
#define STM32_PLLVCO_MAX                    STM32_BOOST_PLLVCO_MAX
#define STM32_PLLP_MIN                      STM32_BOOST_PLLP_MIN
#define STM32_PLLP_MAX                      STM32_BOOST_PLLP_MAX
#define STM32_PLLQ_MIN                      STM32_BOOST_PLLQ_MIN
#define STM32_PLLQ_MAX                      STM32_BOOST_PLLQ_MAX
#define STM32_PLLR_MIN                      STM32_BOOST_PLLR_MIN
#define STM32_PLLR_MAX                      STM32_BOOST_PLLR_MAX
#define STM32_SYSCLK_MAX                    STM32_BOOST_SYSCLK_MAX
#define STM32_PCLK1_MAX                     STM32_BOOST_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_BOOST_PCLK2_MAX
#define STM32_ADCCLK_MAX                    STM32_BOOST_ADCCLK_MAX
#define STM32_FLASH_0WS_MAX                 STM32_BOOST_FLASH_0WS_MAX
#define STM32_FLASH_1WS_MAX                 STM32_BOOST_FLASH_1WS_MAX
#define STM32_FLASH_2WS_MAX                 STM32_BOOST_FLASH_2WS_MAX
#define STM32_FLASH_3WS_MAX                 STM32_BOOST_FLASH_3WS_MAX
#define STM32_FLASH_4WS_MAX                 STM32_BOOST_FLASH_4WS_MAX
#elif (STM32_CFG_PWR_VOS == STM32_VOS_RANGE1)
#define STM32_HSECLK_MIN                    STM32_VOS1_HSECLK_MIN
#define STM32_HSECLK_MAX                    STM32_VOS1_HSECLK_MAX
#define STM32_LSECLK_MIN                    STM32_VOS1_LSECLK_MIN
#define STM32_LSECLK_MAX                    STM32_VOS1_LSECLK_MAX
#define STM32_PLLIN_MIN                     STM32_VOS1_PLLIN_MIN
#define STM32_PLLIN_MAX                     STM32_VOS1_PLLIN_MAX
#define STM32_PLLVCO_MIN                    STM32_VOS1_PLLVCO_MIN
#define STM32_PLLVCO_MAX                    STM32_VOS1_PLLVCO_MAX
#define STM32_PLLP_MIN                      STM32_VOS1_PLLP_MIN
#define STM32_PLLP_MAX                      STM32_VOS1_PLLP_MAX
#define STM32_PLLQ_MIN                      STM32_VOS1_PLLQ_MIN
#define STM32_PLLQ_MAX                      STM32_VOS1_PLLQ_MAX
#define STM32_PLLR_MIN                      STM32_VOS1_PLLR_MIN
#define STM32_PLLR_MAX                      STM32_VOS1_PLLR_MAX
#define STM32_SYSCLK_MAX                    STM32_VOS1_SYSCLK_MAX
#define STM32_PCLK1_MAX                     STM32_VOS1_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_VOS1_PCLK2_MAX
#define STM32_ADCCLK_MAX                    STM32_VOS1_ADCCLK_MAX
#define STM32_FLASH_0WS_MAX                 STM32_VOS1_FLASH_0WS_MAX
#define STM32_FLASH_1WS_MAX                 STM32_VOS1_FLASH_1WS_MAX
#define STM32_FLASH_2WS_MAX                 STM32_VOS1_FLASH_2WS_MAX
#define STM32_FLASH_3WS_MAX                 STM32_VOS1_FLASH_3WS_MAX
#define STM32_FLASH_4WS_MAX                 STM32_VOS1_FLASH_4WS_MAX
#elif (STM32_CFG_PWR_VOS == STM32_VOS_RANGE2)
#define STM32_HSECLK_MIN                    STM32_VOS2_HSECLK_MIN
#define STM32_HSECLK_MAX                    STM32_VOS2_HSECLK_MAX
#define STM32_LSECLK_MIN                    STM32_VOS2_LSECLK_MIN
#define STM32_LSECLK_MAX                    STM32_VOS2_LSECLK_MAX
#define STM32_PLLIN_MIN                     STM32_VOS2_PLLIN_MIN
#define STM32_PLLIN_MAX                     STM32_VOS2_PLLIN_MAX
#define STM32_PLLVCO_MIN                    STM32_VOS2_PLLVCO_MIN
#define STM32_PLLVCO_MAX                    STM32_VOS2_PLLVCO_MAX
#define STM32_PLLP_MIN                      STM32_VOS2_PLLP_MIN
#define STM32_PLLP_MAX                      STM32_VOS2_PLLP_MAX
#define STM32_PLLQ_MIN                      STM32_VOS2_PLLQ_MIN
#define STM32_PLLQ_MAX                      STM32_VOS2_PLLQ_MAX
#define STM32_PLLR_MIN                      STM32_VOS2_PLLR_MIN
#define STM32_PLLR_MAX                      STM32_VOS2_PLLR_MAX
#define STM32_SYSCLK_MAX                    STM32_VOS2_SYSCLK_MAX
#define STM32_PCLK1_MAX                     STM32_VOS2_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_VOS2_PCLK2_MAX
#define STM32_ADCCLK_MAX                    STM32_VOS2_ADCCLK_MAX
#define STM32_FLASH_0WS_MAX                 STM32_VOS2_FLASH_0WS_MAX
#define STM32_FLASH_1WS_MAX                 STM32_VOS2_FLASH_1WS_MAX
#define STM32_FLASH_2WS_MAX                 STM32_VOS2_FLASH_2WS_MAX
#define STM32_FLASH_3WS_MAX                 STM32_VOS2_FLASH_3WS_MAX
#define STM32_FLASH_4WS_MAX                 STM32_VOS2_FLASH_4WS_MAX
#else
#error "unable to select clock frequency limits"
#endif

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
#define STM32_PLLVCO_ENABLED                ((STM32_PLLP_ENABLED == TRUE) || \
                                             (STM32_PLLQ_ENABLED == TRUE) || \
                                             (STM32_PLLR_ENABLED == TRUE))

/**
 * @brief   PLLP clock derived enable state.
 */
#define STM32_PLLP_ENABLED                  (((STM32_ADC12_ENABLED == TRUE) && \
                                             (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_PLLPCLK)) || \
                                             ((STM32_ADC345_ENABLED == TRUE) && \
                                             (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_PLLPCLK)))

/**
 * @brief   PLLQ clock derived enable state.
 */
#define STM32_PLLQ_ENABLED                  (((STM32_SAI1_ENABLED == TRUE) && \
                                             (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_PLLQCLK)) || \
                                             ((STM32_I2S23_ENABLED == TRUE) && \
                                             (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_PLLQCLK)) || \
                                             ((STM32_FDCAN_ENABLED == TRUE) && \
                                             (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_PLLQCLK)) || \
                                             ((STM32_CLK48_ENABLED == TRUE) && \
                                             (STM32_CFG_CLK48_SEL == STM32_CLK48SEL_PLLQCLK)) || \
                                             ((STM32_QSPI_ENABLED == TRUE) && \
                                             (STM32_CFG_QSPI_SEL == STM32_QSPISEL_PLLQCLK)))

/**
 * @brief   PLLR clock derived enable state.
 */
#define STM32_PLLR_ENABLED                  (((STM32_SYSCLK_ENABLED == TRUE) && \
                                             (STM32_CFG_SYSCLK_SEL == STM32_SW_PLLRCLK)) || \
                                             ((STM32_MCODIV_ENABLED == TRUE) && \
                                             (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_PLLRCLK)))

/**
 * @brief   SYSCLK clock derived enable state.
 */
#define STM32_SYSCLK_ENABLED                TRUE

/**
 * @brief   HCLK clock derived enable state.
 */
#define STM32_HCLK_ENABLED                  TRUE

/**
 * @brief   PCLK1 clock derived enable state.
 */
#define STM32_PCLK1_ENABLED                 TRUE

/**
 * @brief   PCLK2 clock derived enable state.
 */
#define STM32_PCLK2_ENABLED                 TRUE

/**
 * @brief   PCLK1TIM clock derived enable state.
 */
#define STM32_PCLK1TIM_ENABLED              TRUE

/**
 * @brief   PCLK2TIM clock derived enable state.
 */
#define STM32_PCLK2TIM_ENABLED              TRUE

/**
 * @brief   CKIN clock derived enable state.
 */
#define STM32_CKIN_ENABLED                  TRUE

/**
 * @brief   HSEDIV clock derived enable state.
 */
#define STM32_HSEDIV_ENABLED                (((STM32_RTC_ENABLED == TRUE) && \
                                             (STM32_CFG_RTC_SEL == STM32_RTCSEL_HSEDIV)))

/**
 * @brief   MCODIV clock derived enable state.
 */
#define STM32_MCODIV_ENABLED                TRUE

/**
 * @brief   MCO clock derived enable state.
 */
#define STM32_MCO_ENABLED                   TRUE

/**
 * @brief   LSCO clock derived enable state.
 */
#define STM32_LSCO_ENABLED                  TRUE

/**
 * @brief   RTC clock derived enable state.
 */
#define STM32_RTC_ENABLED                   ((HAL_USE_RTC == TRUE))

/**
 * @brief   USART1 clock derived enable state.
 */
#define STM32_USART1_ENABLED                TRUE

/**
 * @brief   USART2 clock derived enable state.
 */
#define STM32_USART2_ENABLED                TRUE

/**
 * @brief   USART3 clock derived enable state.
 */
#define STM32_USART3_ENABLED                TRUE

/**
 * @brief   UART4 clock derived enable state.
 */
#define STM32_UART4_ENABLED                 TRUE

/**
 * @brief   UART5 clock derived enable state.
 */
#define STM32_UART5_ENABLED                 TRUE

/**
 * @brief   LPUART1 clock derived enable state.
 */
#define STM32_LPUART1_ENABLED               TRUE

/**
 * @brief   I2C1 clock derived enable state.
 */
#define STM32_I2C1_ENABLED                  TRUE

/**
 * @brief   I2C2 clock derived enable state.
 */
#define STM32_I2C2_ENABLED                  TRUE

/**
 * @brief   I2C3 clock derived enable state.
 */
#define STM32_I2C3_ENABLED                  TRUE

/**
 * @brief   I2C4 clock derived enable state.
 */
#define STM32_I2C4_ENABLED                  TRUE

/**
 * @brief   LPTIM1 clock derived enable state.
 */
#define STM32_LPTIM1_ENABLED                TRUE

/**
 * @brief   SAI1 clock derived enable state.
 */
#define STM32_SAI1_ENABLED                  TRUE

/**
 * @brief   I2S23 clock derived enable state.
 */
#define STM32_I2S23_ENABLED                 TRUE

/**
 * @brief   FDCAN clock derived enable state.
 */
#define STM32_FDCAN_ENABLED                 TRUE

/**
 * @brief   CLK48 clock derived enable state.
 */
#define STM32_CLK48_ENABLED                 TRUE

/**
 * @brief   ADC12 clock derived enable state.
 */
#define STM32_ADC12_ENABLED                 TRUE

/**
 * @brief   ADC345 clock derived enable state.
 */
#define STM32_ADC345_ENABLED                TRUE

/**
 * @brief   QSPI clock derived enable state.
 */
#define STM32_QSPI_ENABLED                  TRUE

/**
 * @brief   RNG clock derived enable state.
 */
#define STM32_RNG_ENABLED                   TRUE

/**
 * @brief   USB clock derived enable state.
 */
#define STM32_USB_ENABLED                   TRUE

/*--- Macros and checks for the NONE clock point. --------------------------*/

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

/*--- Macros and checks for the HSI16 clock point. -------------------------*/

#if (STM32_CFG_HSI16_ENABLE != TRUE) && \
    (STM32_CFG_HSI16_ENABLE != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_HSI16_ENABLE value specified"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_PLLIN_ENABLED == TRUE) && \
    (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by PLLIN"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_SYSCLK_ENABLED == TRUE) && \
    (STM32_CFG_SYSCLK_SEL == STM32_SW_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by SYSCLK"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_MCODIV_ENABLED == TRUE) && \
    (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by MCODIV"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_USART1_ENABLED == TRUE) && \
    (STM32_CFG_USART1_SEL == STM32_USART1SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by USART1"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_USART2_ENABLED == TRUE) && \
    (STM32_CFG_USART2_SEL == STM32_USART2SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by USART2"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_USART3_ENABLED == TRUE) && \
    (STM32_CFG_USART3_SEL == STM32_USART3SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by USART3"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_UART4_ENABLED == TRUE) && \
    (STM32_CFG_UART4_SEL == STM32_UART4SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by UART4"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_UART5_ENABLED == TRUE) && \
    (STM32_CFG_UART5_SEL == STM32_UART5SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by UART5"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_LPUART1_ENABLED == TRUE) && \
    (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by LPUART1"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_I2C1_ENABLED == TRUE) && \
    (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by I2C1"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_I2C2_ENABLED == TRUE) && \
    (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by I2C2"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_I2C3_ENABLED == TRUE) && \
    (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by I2C3"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_I2C4_ENABLED == TRUE) && \
    (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by I2C4"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_LPTIM1_ENABLED == TRUE) && \
    (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by LPTIM1"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_SAI1_ENABLED == TRUE) && \
    (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by SAI1"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_I2S23_ENABLED == TRUE) && \
    (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by I2S23"
#endif

#if (STM32_HSI16_ENABLED == FALSE) && \
    (STM32_QSPI_ENABLED == TRUE) && \
    (STM32_CFG_QSPI_SEL == STM32_QSPISEL_HSI16) && \
    !defined(__DOXYGEN__)
#error "HSI16 not enabled, required by QSPI"
#endif

/**
 * @brief   HSI16 clock register bits.
 */
#if (STM32_HSI16_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSI16_BITS                    (RCC_CR_HSIKERON | RCC_CR_HSION)
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

/*--- Macros and checks for the HSI48 clock point. -------------------------*/

#if (STM32_CFG_HSI48_ENABLE != TRUE) && \
    (STM32_CFG_HSI48_ENABLE != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_HSI48_ENABLE value specified"
#endif

#if (STM32_HSI48_ENABLED == FALSE) && \
    (STM32_MCODIV_ENABLED == TRUE) && \
    (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSI48) && \
    !defined(__DOXYGEN__)
#error "HSI48 not enabled, required by MCODIV"
#endif

#if (STM32_HSI48_ENABLED == FALSE) && \
    (STM32_CLK48_ENABLED == TRUE) && \
    (STM32_CFG_CLK48_SEL == STM32_CLK48SEL_HSI48) && \
    !defined(__DOXYGEN__)
#error "HSI48 not enabled, required by CLK48"
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

/*--- Macros and checks for the HSE clock point. ---------------------------*/

#if (STM32_CFG_HSE_ENABLE != TRUE) && \
    (STM32_CFG_HSE_ENABLE != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_HSE_ENABLE value specified"
#endif

#if (STM32_HSE_ENABLED == FALSE) && \
    (STM32_PLLIN_ENABLED == TRUE) && \
    (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_HSE) && \
    !defined(__DOXYGEN__)
#error "HSE not enabled, required by PLLIN"
#endif

#if (STM32_HSE_ENABLED == FALSE) && \
    (STM32_SYSCLK_ENABLED == TRUE) && \
    (STM32_CFG_SYSCLK_SEL == STM32_SW_HSE) && \
    !defined(__DOXYGEN__)
#error "HSE not enabled, required by SYSCLK"
#endif

#if (STM32_HSE_ENABLED == FALSE) && \
    (STM32_HSEDIV_ENABLED == TRUE) && \
    !defined(__DOXYGEN__)
#error "HSE not enabled, required by HSEDIV"
#endif

#if (STM32_HSE_ENABLED == FALSE) && \
    (STM32_MCODIV_ENABLED == TRUE) && \
    (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSE) && \
    !defined(__DOXYGEN__)
#error "HSE not enabled, required by MCODIV"
#endif

#if (STM32_HSE_ENABLED == FALSE) && \
    (STM32_FDCAN_ENABLED == TRUE) && \
    (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_HSE) && \
    !defined(__DOXYGEN__)
#error "HSE not enabled, required by FDCAN"
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

#if (STM32_HSE_ENABLED == TRUE) && \
    (STM32_HSE_FREQ < STM32_HSECLK_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_HSE_FREQ below minimum frequency"
#endif

#if (STM32_HSE_ENABLED == TRUE) && \
    (STM32_HSE_FREQ > STM32_HSECLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_HSE_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the LSE clock point. ---------------------------*/

#if (STM32_CFG_LSE_ENABLE != TRUE) && \
    (STM32_CFG_LSE_ENABLE != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_LSE_ENABLE value specified"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_MCODIV_ENABLED == TRUE) && \
    (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by MCODIV"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_LSCO_ENABLED == TRUE) && \
    (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by LSCO"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_RTC_ENABLED == TRUE) && \
    (STM32_CFG_RTC_SEL == STM32_RTCSEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by RTC"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_USART1_ENABLED == TRUE) && \
    (STM32_CFG_USART1_SEL == STM32_USART1SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by USART1"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_USART2_ENABLED == TRUE) && \
    (STM32_CFG_USART2_SEL == STM32_USART2SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by USART2"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_USART3_ENABLED == TRUE) && \
    (STM32_CFG_USART3_SEL == STM32_USART3SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by USART3"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_UART4_ENABLED == TRUE) && \
    (STM32_CFG_UART4_SEL == STM32_UART4SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by UART4"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_UART5_ENABLED == TRUE) && \
    (STM32_CFG_UART5_SEL == STM32_UART5SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by UART5"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_LPUART1_ENABLED == TRUE) && \
    (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by LPUART1"
#endif

#if (STM32_LSE_ENABLED == FALSE) && \
    (STM32_LPTIM1_ENABLED == TRUE) && \
    (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_LSE) && \
    !defined(__DOXYGEN__)
#error "LSE not enabled, required by LPTIM1"
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

#if (STM32_LSE_ENABLED == TRUE) && \
    (STM32_LSE_FREQ < STM32_LSECLK_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_LSE_FREQ below minimum frequency"
#endif

#if (STM32_LSE_ENABLED == TRUE) && \
    (STM32_LSE_FREQ > STM32_LSECLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_LSE_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the LSI clock point. ---------------------------*/

#if (STM32_CFG_LSI_ENABLE != TRUE) && \
    (STM32_CFG_LSI_ENABLE != FALSE) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_LSI_ENABLE value specified"
#endif

#if (STM32_LSI_ENABLED == FALSE) && \
    (STM32_MCODIV_ENABLED == TRUE) && \
    (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_LSI) && \
    !defined(__DOXYGEN__)
#error "LSI not enabled, required by MCODIV"
#endif

#if (STM32_LSI_ENABLED == FALSE) && \
    (STM32_LSCO_ENABLED == TRUE) && \
    (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_LSI) && \
    !defined(__DOXYGEN__)
#error "LSI not enabled, required by LSCO"
#endif

#if (STM32_LSI_ENABLED == FALSE) && \
    (STM32_RTC_ENABLED == TRUE) && \
    (STM32_CFG_RTC_SEL == STM32_RTCSEL_LSI) && \
    !defined(__DOXYGEN__)
#error "LSI not enabled, required by RTC"
#endif

#if (STM32_LSI_ENABLED == FALSE) && \
    (STM32_LPTIM1_ENABLED == TRUE) && \
    (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_LSI) && \
    !defined(__DOXYGEN__)
#error "LSI not enabled, required by LPTIM1"
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
#define STM32_LSI_FREQ                      STM32_LSICLK
#else
#define STM32_LSI_FREQ                      0U
#endif

/*--- Macros and checks for the PLLIN clock point. -------------------------*/

/**
 * @brief   PLLIN clock register bits.
 */
#if (STM32_PLLIN_ENABLED == FALSE)
#define STM32_PLLIN_BITS                    STM32_PLLSRC_NOCLOCK
#elif (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_PLLIN_BITS                    STM32_PLLSRC_NOCLOCK
#elif (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_HSI16)
#define STM32_PLLIN_BITS                    STM32_PLLSRC_HSI16
#elif (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_HSE)
#define STM32_PLLIN_BITS                    STM32_PLLSRC_HSE
#else
#error "invalid STM32_CFG_PLLIN_SEL value specified"
#endif

/**
 * @brief   PLL input clock point.
 */
#if ((STM32_PLLIN_ENABLED == TRUE) && \
     (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_NOCLOCK)) || \
    defined(__DOXYGEN__)
#define STM32_PLLIN_FREQ                    STM32_NONE_FREQ
#elif (STM32_PLLIN_ENABLED == TRUE) && \
      (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_HSI16)
#define STM32_PLLIN_FREQ                    STM32_HSI16_FREQ
#elif (STM32_PLLIN_ENABLED == TRUE) && \
      (STM32_CFG_PLLIN_SEL == STM32_PLLSRC_HSE)
#define STM32_PLLIN_FREQ                    STM32_HSE_FREQ
#else
#define STM32_PLLIN_FREQ                    0U
#endif

/*--- Macros and checks for the PLLREF clock point. ------------------------*/

#if ((STM32_CFG_PLLREF_VALUE < 1) || \
     (STM32_CFG_PLLREF_VALUE > 16)) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PLLREF_VALUE value specified"
#endif

/**
 * @brief   PLLREF clock register bits.
 */
#define STM32_PLLREF_BITS                   ((STM32_CFG_PLLREF_VALUE - 1U) << RCC_PLLCFGR_PLLM_Pos)

/**
 * @brief   PLL reference clock point.
 */
#if (STM32_PLLREF_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLREF_FREQ                   (STM32_PLLIN_FREQ / STM32_CFG_PLLREF_VALUE)
#else
#define STM32_PLLREF_FREQ                   0U
#endif

#if (STM32_PLLREF_ENABLED == TRUE) && \
    (STM32_PLLREF_FREQ < STM32_PLLIN_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLREF_FREQ below minimum frequency"
#endif

#if (STM32_PLLREF_ENABLED == TRUE) && \
    (STM32_PLLREF_FREQ > STM32_PLLIN_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLREF_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PLLVCO clock point. ------------------------*/

#if ((STM32_CFG_PLLVCO_VALUE < 8) || \
     (STM32_CFG_PLLVCO_VALUE > 127)) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PLLVCO_VALUE value specified"
#endif

/**
 * @brief   PLLVCO clock register bits.
 */
#define STM32_PLLVCO_BITS                   (STM32_CFG_PLLVCO_VALUE << RCC_PLLCFGR_PLLN_Pos)

/**
 * @brief   PLL VCO clock point.
 */
#if (STM32_PLLVCO_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLVCO_FREQ                   (STM32_PLLREF_FREQ * STM32_CFG_PLLVCO_VALUE)
#else
#define STM32_PLLVCO_FREQ                   0U
#endif

#if (STM32_PLLVCO_ENABLED == TRUE) && \
    (STM32_PLLVCO_FREQ < STM32_PLLVCO_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLVCO_FREQ below minimum frequency"
#endif

#if (STM32_PLLVCO_ENABLED == TRUE) && \
    (STM32_PLLVCO_FREQ > STM32_PLLVCO_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLVCO_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PLLP clock point. --------------------------*/

#if ((STM32_CFG_PLLP_VALUE < 2) || \
     (STM32_CFG_PLLP_VALUE > 31)) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PLLP_VALUE value specified"
#endif

/**
 * @brief   PLLP clock register bits.
 */
#if (STM32_PLLP_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLP_BITS                     (RCC_PLLCFGR_PLLPEN | (STM32_CFG_PLLP_VALUE << RCC_PLLCFGR_PLLPDIV_Pos))
#else
#define STM32_PLLP_BITS                     (0U | (STM32_CFG_PLLP_VALUE << RCC_PLLCFGR_PLLPDIV_Pos))
#endif

/**
 * @brief   PLL P output clock point.
 */
#if (STM32_PLLP_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLP_FREQ                     (STM32_PLLVCO_FREQ / STM32_CFG_PLLP_VALUE)
#else
#define STM32_PLLP_FREQ                     0U
#endif

#if (STM32_PLLP_ENABLED == TRUE) && \
    (STM32_PLLP_FREQ < STM32_PLLP_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLP_FREQ below minimum frequency"
#endif

#if (STM32_PLLP_ENABLED == TRUE) && \
    (STM32_PLLP_FREQ > STM32_PLLP_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLP_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PLLQ clock point. --------------------------*/

#if (STM32_CFG_PLLQ_VALUE != 2) && \
    (STM32_CFG_PLLQ_VALUE != 4) && \
    (STM32_CFG_PLLQ_VALUE != 6) && \
    (STM32_CFG_PLLQ_VALUE != 8) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PLLQ_VALUE value specified"
#endif

/**
 * @brief   PLLQ clock register bits.
 */
#if (STM32_PLLQ_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLQ_BITS                     (RCC_PLLCFGR_PLLQEN | (((STM32_CFG_PLLQ_VALUE / 2U) - 1U) << RCC_PLLCFGR_PLLQ_Pos))
#else
#define STM32_PLLQ_BITS                     (0U | (((STM32_CFG_PLLQ_VALUE / 2U) - 1U) << RCC_PLLCFGR_PLLQ_Pos))
#endif

/**
 * @brief   PLL Q output clock point.
 */
#if (STM32_PLLQ_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLQ_FREQ                     (STM32_PLLVCO_FREQ / STM32_CFG_PLLQ_VALUE)
#else
#define STM32_PLLQ_FREQ                     0U
#endif

#if (STM32_PLLQ_ENABLED == TRUE) && \
    (STM32_PLLQ_FREQ < STM32_PLLQ_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLQ_FREQ below minimum frequency"
#endif

#if (STM32_PLLQ_ENABLED == TRUE) && \
    (STM32_PLLQ_FREQ > STM32_PLLQ_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLQ_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PLLR clock point. --------------------------*/

#if (STM32_CFG_PLLR_VALUE != 2) && \
    (STM32_CFG_PLLR_VALUE != 4) && \
    (STM32_CFG_PLLR_VALUE != 6) && \
    (STM32_CFG_PLLR_VALUE != 8) && \
    !defined(__DOXYGEN__)
#error "invalid STM32_CFG_PLLR_VALUE value specified"
#endif

/**
 * @brief   PLLR clock register bits.
 */
#if (STM32_PLLR_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLR_BITS                     (RCC_PLLCFGR_PLLREN | (((STM32_CFG_PLLR_VALUE / 2U) - 1U) << RCC_PLLCFGR_PLLR_Pos))
#else
#define STM32_PLLR_BITS                     (0U | (((STM32_CFG_PLLR_VALUE / 2U) - 1U) << RCC_PLLCFGR_PLLR_Pos))
#endif

/**
 * @brief   PLL R output clock point.
 */
#if (STM32_PLLR_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PLLR_FREQ                     (STM32_PLLVCO_FREQ / STM32_CFG_PLLR_VALUE)
#else
#define STM32_PLLR_FREQ                     0U
#endif

#if (STM32_PLLR_ENABLED == TRUE) && \
    (STM32_PLLR_FREQ < STM32_PLLR_MIN) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLR_FREQ below minimum frequency"
#endif

#if (STM32_PLLR_ENABLED == TRUE) && \
    (STM32_PLLR_FREQ > STM32_PLLR_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PLLR_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the SYSCLK clock point. ------------------------*/

/**
 * @brief   SYSCLK clock register bits.
 */
#if (STM32_CFG_SYSCLK_SEL == STM32_SW_HSI16) || defined(__DOXYGEN__)
#define STM32_SYSCLK_BITS                   STM32_SW_HSI16
#elif (STM32_CFG_SYSCLK_SEL == STM32_SW_HSE)
#define STM32_SYSCLK_BITS                   STM32_SW_HSE
#elif (STM32_CFG_SYSCLK_SEL == STM32_SW_PLLRCLK)
#define STM32_SYSCLK_BITS                   STM32_SW_PLLRCLK
#else
#error "invalid STM32_CFG_SYSCLK_SEL value specified"
#endif

/**
 * @brief   System clock clock point.
 */
#if ((STM32_SYSCLK_ENABLED == TRUE) && \
     (STM32_CFG_SYSCLK_SEL == STM32_SW_HSI16)) || \
    defined(__DOXYGEN__)
#define STM32_SYSCLK_FREQ                   STM32_HSI16_FREQ
#elif (STM32_SYSCLK_ENABLED == TRUE) && \
      (STM32_CFG_SYSCLK_SEL == STM32_SW_HSE)
#define STM32_SYSCLK_FREQ                   STM32_HSE_FREQ
#elif (STM32_SYSCLK_ENABLED == TRUE) && \
      (STM32_CFG_SYSCLK_SEL == STM32_SW_PLLRCLK)
#define STM32_SYSCLK_FREQ                   STM32_PLLR_FREQ
#else
#define STM32_SYSCLK_FREQ                   0U
#endif

#if (STM32_SYSCLK_ENABLED == TRUE) && \
    (STM32_CFG_SYSCLK_SEL == STM32_SW_HSI16) && \
    (STM32_HSI16_FREQ > STM32_SYSCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_SYSCLK_FREQ above maximum frequency"
#endif

#if (STM32_SYSCLK_ENABLED == TRUE) && \
    (STM32_CFG_SYSCLK_SEL == STM32_SW_HSE) && \
    (STM32_HSE_FREQ > STM32_SYSCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_SYSCLK_FREQ above maximum frequency"
#endif

#if (STM32_SYSCLK_ENABLED == TRUE) && \
    (STM32_CFG_SYSCLK_SEL == STM32_SW_PLLRCLK) && \
    (STM32_PLLR_FREQ > STM32_SYSCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_SYSCLK_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the HCLK clock point. --------------------------*/

/**
 * @brief   HCLK clock register bits.
 */
#if (STM32_CFG_HCLK_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV1
#elif (STM32_CFG_HCLK_VALUE == 2)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV2
#elif (STM32_CFG_HCLK_VALUE == 4)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV4
#elif (STM32_CFG_HCLK_VALUE == 8)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV8
#elif (STM32_CFG_HCLK_VALUE == 16)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV16
#elif (STM32_CFG_HCLK_VALUE == 64)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV64
#elif (STM32_CFG_HCLK_VALUE == 128)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV128
#elif (STM32_CFG_HCLK_VALUE == 256)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV256
#elif (STM32_CFG_HCLK_VALUE == 512)
#define STM32_HCLK_BITS                     STM32_HPRE_DIV512
#else
#error "invalid STM32_CFG_HCLK_VALUE value specified"
#endif

/**
 * @brief   AHB clock point.
 */
#if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HCLK_FREQ                     (STM32_SYSCLK_FREQ / STM32_CFG_HCLK_VALUE)
#else
#define STM32_HCLK_FREQ                     0U
#endif

/*--- Macros and checks for the PCLK1 clock point. -------------------------*/

/**
 * @brief   PCLK1 clock register bits.
 */
#if (STM32_CFG_PCLK1_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_PCLK1_BITS                    STM32_PPRE1_DIV1
#elif (STM32_CFG_PCLK1_VALUE == 2)
#define STM32_PCLK1_BITS                    STM32_PPRE1_DIV2
#elif (STM32_CFG_PCLK1_VALUE == 4)
#define STM32_PCLK1_BITS                    STM32_PPRE1_DIV4
#elif (STM32_CFG_PCLK1_VALUE == 8)
#define STM32_PCLK1_BITS                    STM32_PPRE1_DIV8
#elif (STM32_CFG_PCLK1_VALUE == 16)
#define STM32_PCLK1_BITS                    STM32_PPRE1_DIV16
#else
#error "invalid STM32_CFG_PCLK1_VALUE value specified"
#endif

/**
 * @brief   APB1 clock point.
 */
#if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PCLK1_FREQ                    (STM32_HCLK_FREQ / STM32_CFG_PCLK1_VALUE)
#else
#define STM32_PCLK1_FREQ                    0U
#endif

#if (STM32_PCLK1_ENABLED == TRUE) && \
    (STM32_PCLK1_FREQ > STM32_PCLK1_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PCLK1_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PCLK2 clock point. -------------------------*/

/**
 * @brief   PCLK2 clock register bits.
 */
#if (STM32_CFG_PCLK2_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_PCLK2_BITS                    STM32_PPRE2_DIV1
#elif (STM32_CFG_PCLK2_VALUE == 2)
#define STM32_PCLK2_BITS                    STM32_PPRE2_DIV2
#elif (STM32_CFG_PCLK2_VALUE == 4)
#define STM32_PCLK2_BITS                    STM32_PPRE2_DIV4
#elif (STM32_CFG_PCLK2_VALUE == 8)
#define STM32_PCLK2_BITS                    STM32_PPRE2_DIV8
#elif (STM32_CFG_PCLK2_VALUE == 16)
#define STM32_PCLK2_BITS                    STM32_PPRE2_DIV16
#else
#error "invalid STM32_CFG_PCLK2_VALUE value specified"
#endif

/**
 * @brief   APB2 clock point.
 */
#if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PCLK2_FREQ                    (STM32_HCLK_FREQ / STM32_CFG_PCLK2_VALUE)
#else
#define STM32_PCLK2_FREQ                    0U
#endif

#if (STM32_PCLK2_ENABLED == TRUE) && \
    (STM32_PCLK2_FREQ > STM32_PCLK2_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_PCLK2_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PCLK1TIM clock point. ----------------------*/

/**
 * @brief   PCLK1TIM clock register bits.
 */
#define STM32_PCLK1TIM_BITS                 0U

/**
 * @brief   APB1 timer clock clock point.
 */
#if (STM32_PCLK1TIM_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PCLK1TIM_FREQ                 ((STM32_CFG_PCLK1_VALUE == 1) ? STM32_PCLK1_FREQ : (STM32_PCLK1_FREQ * 2U))
#else
#define STM32_PCLK1TIM_FREQ                 0U
#endif

/*--- Macros and checks for the PCLK2TIM clock point. ----------------------*/

/**
 * @brief   PCLK2TIM clock register bits.
 */
#define STM32_PCLK2TIM_BITS                 0U

/**
 * @brief   APB2 timer clock clock point.
 */
#if (STM32_PCLK2TIM_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_PCLK2TIM_FREQ                 ((STM32_CFG_PCLK2_VALUE == 1) ? STM32_PCLK2_FREQ : (STM32_PCLK2_FREQ * 2U))
#else
#define STM32_PCLK2TIM_FREQ                 0U
#endif

/*--- Macros and checks for the CKIN clock point. --------------------------*/

/**
 * @brief   CKIN clock register bits.
 */
#define STM32_CKIN_BITS                     0U

/**
 * @brief   External audio clock input clock point.
 */
#if (STM32_CKIN_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_CKIN_FREQ                     0U
#else
#define STM32_CKIN_FREQ                     0U
#endif

/*--- Macros and checks for the HSEDIV clock point. ------------------------*/

/**
 * @brief   HSEDIV clock register bits.
 */
#define STM32_HSEDIV_BITS                   0U

/**
 * @brief   HSE divided clock clock point.
 */
#if (STM32_HSEDIV_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_HSEDIV_FREQ                   (STM32_HSE_FREQ / 32U)
#else
#define STM32_HSEDIV_FREQ                   0U
#endif

/*--- Macros and checks for the MCODIV clock point. ------------------------*/

/**
 * @brief   MCODIV clock register bits.
 */
#if (STM32_MCODIV_ENABLED == FALSE)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_NOCLOCK
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_NOCLOCK
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_SYSCLK)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_SYSCLK
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSI16)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_HSI16
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSE)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_HSE
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_PLLRCLK)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_PLLRCLK
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_LSI)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_LSI
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_LSE)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_LSE
#elif (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSI48)
#define STM32_MCODIV_BITS                   STM32_MCOSEL_HSI48
#else
#error "invalid STM32_CFG_MCODIV_SEL value specified"
#endif

/**
 * @brief   MCO source before prescaler clock point.
 */
#if ((STM32_MCODIV_ENABLED == TRUE) && \
     (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_NOCLOCK)) || \
    defined(__DOXYGEN__)
#define STM32_MCODIV_FREQ                   STM32_NONE_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_SYSCLK)
#define STM32_MCODIV_FREQ                   STM32_SYSCLK_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSI16)
#define STM32_MCODIV_FREQ                   STM32_HSI16_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSE)
#define STM32_MCODIV_FREQ                   STM32_HSE_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_PLLRCLK)
#define STM32_MCODIV_FREQ                   STM32_PLLR_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_LSI)
#define STM32_MCODIV_FREQ                   STM32_LSI_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_LSE)
#define STM32_MCODIV_FREQ                   STM32_LSE_FREQ
#elif (STM32_MCODIV_ENABLED == TRUE) && \
      (STM32_CFG_MCODIV_SEL == STM32_MCOSEL_HSI48)
#define STM32_MCODIV_FREQ                   STM32_HSI48_FREQ
#else
#define STM32_MCODIV_FREQ                   0U
#endif

/*--- Macros and checks for the MCO clock point. ---------------------------*/

/**
 * @brief   MCO clock register bits.
 */
#if (STM32_CFG_MCO_VALUE == 1) || defined(__DOXYGEN__)
#define STM32_MCO_BITS                      STM32_MCOPRE_DIV1
#elif (STM32_CFG_MCO_VALUE == 2)
#define STM32_MCO_BITS                      STM32_MCOPRE_DIV2
#elif (STM32_CFG_MCO_VALUE == 4)
#define STM32_MCO_BITS                      STM32_MCOPRE_DIV4
#elif (STM32_CFG_MCO_VALUE == 8)
#define STM32_MCO_BITS                      STM32_MCOPRE_DIV8
#elif (STM32_CFG_MCO_VALUE == 16)
#define STM32_MCO_BITS                      STM32_MCOPRE_DIV16
#else
#error "invalid STM32_CFG_MCO_VALUE value specified"
#endif

/**
 * @brief   MCO output pin clock point.
 */
#if (STM32_MCO_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_MCO_FREQ                      (STM32_MCODIV_FREQ / STM32_CFG_MCO_VALUE)
#else
#define STM32_MCO_FREQ                      0U
#endif

/*--- Macros and checks for the LSCO clock point. --------------------------*/

/**
 * @brief   LSCO clock register bits.
 */
#if (STM32_LSCO_ENABLED == FALSE)
#define STM32_LSCO_BITS                     STM32_LSCOSEL_NOCLOCK
#elif (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_LSCO_BITS                     STM32_LSCOSEL_NOCLOCK
#elif (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_LSI)
#define STM32_LSCO_BITS                     STM32_LSCOSEL_LSI
#elif (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_LSE)
#define STM32_LSCO_BITS                     STM32_LSCOSEL_LSE
#else
#error "invalid STM32_CFG_LSCO_SEL value specified"
#endif

/**
 * @brief   LSCO output pin clock point.
 */
#if ((STM32_LSCO_ENABLED == TRUE) && \
     (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_NOCLOCK)) || \
    defined(__DOXYGEN__)
#define STM32_LSCO_FREQ                     STM32_NONE_FREQ
#elif (STM32_LSCO_ENABLED == TRUE) && \
      (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_LSI)
#define STM32_LSCO_FREQ                     STM32_LSI_FREQ
#elif (STM32_LSCO_ENABLED == TRUE) && \
      (STM32_CFG_LSCO_SEL == STM32_LSCOSEL_LSE)
#define STM32_LSCO_FREQ                     STM32_LSE_FREQ
#else
#define STM32_LSCO_FREQ                     0U
#endif

/*--- Macros and checks for the RTC clock point. ---------------------------*/

/**
 * @brief   RTC clock register bits.
 */
#if (STM32_RTC_ENABLED == FALSE)
#define STM32_RTC_BITS                      STM32_RTCSEL_NOCLOCK
#elif (STM32_CFG_RTC_SEL == STM32_RTCSEL_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_RTC_BITS                      STM32_RTCSEL_NOCLOCK
#elif (STM32_CFG_RTC_SEL == STM32_RTCSEL_LSE)
#define STM32_RTC_BITS                      STM32_RTCSEL_LSE
#elif (STM32_CFG_RTC_SEL == STM32_RTCSEL_LSI)
#define STM32_RTC_BITS                      STM32_RTCSEL_LSI
#elif (STM32_CFG_RTC_SEL == STM32_RTCSEL_HSEDIV)
#define STM32_RTC_BITS                      STM32_RTCSEL_HSEDIV
#else
#error "invalid STM32_CFG_RTC_SEL value specified"
#endif

/**
 * @brief   RTC clock point.
 */
#if ((STM32_RTC_ENABLED == TRUE) && \
     (STM32_CFG_RTC_SEL == STM32_RTCSEL_NOCLOCK)) || \
    defined(__DOXYGEN__)
#define STM32_RTC_FREQ                      STM32_NONE_FREQ
#elif (STM32_RTC_ENABLED == TRUE) && \
      (STM32_CFG_RTC_SEL == STM32_RTCSEL_LSE)
#define STM32_RTC_FREQ                      STM32_LSE_FREQ
#elif (STM32_RTC_ENABLED == TRUE) && \
      (STM32_CFG_RTC_SEL == STM32_RTCSEL_LSI)
#define STM32_RTC_FREQ                      STM32_LSI_FREQ
#elif (STM32_RTC_ENABLED == TRUE) && \
      (STM32_CFG_RTC_SEL == STM32_RTCSEL_HSEDIV)
#define STM32_RTC_FREQ                      STM32_HSEDIV_FREQ
#else
#define STM32_RTC_FREQ                      0U
#endif

/*--- Macros and checks for the USART1 clock point. ------------------------*/

/**
 * @brief   USART1 clock register bits.
 */
#if (STM32_CFG_USART1_SEL == STM32_USART1SEL_PCLK2) || defined(__DOXYGEN__)
#define STM32_USART1_BITS                   STM32_USART1SEL_PCLK2
#elif (STM32_CFG_USART1_SEL == STM32_USART1SEL_SYSCLK)
#define STM32_USART1_BITS                   STM32_USART1SEL_SYSCLK
#elif (STM32_CFG_USART1_SEL == STM32_USART1SEL_HSI16)
#define STM32_USART1_BITS                   STM32_USART1SEL_HSI16
#elif (STM32_CFG_USART1_SEL == STM32_USART1SEL_LSE)
#define STM32_USART1_BITS                   STM32_USART1SEL_LSE
#else
#error "invalid STM32_CFG_USART1_SEL value specified"
#endif

/**
 * @brief   USART1 clock point.
 */
#if ((STM32_USART1_ENABLED == TRUE) && \
     (STM32_CFG_USART1_SEL == STM32_USART1SEL_PCLK2)) || \
    defined(__DOXYGEN__)
#define STM32_USART1_FREQ                   hal_lld_get_clock_point(CLK_PCLK2)
#elif (STM32_USART1_ENABLED == TRUE) && \
      (STM32_CFG_USART1_SEL == STM32_USART1SEL_SYSCLK)
#define STM32_USART1_FREQ                   hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_USART1_ENABLED == TRUE) && \
      (STM32_CFG_USART1_SEL == STM32_USART1SEL_HSI16)
#define STM32_USART1_FREQ                   hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_USART1_ENABLED == TRUE) && \
      (STM32_CFG_USART1_SEL == STM32_USART1SEL_LSE)
#define STM32_USART1_FREQ                   STM32_LSE_FREQ
#else
#define STM32_USART1_FREQ                   0U
#endif

/*--- Macros and checks for the USART2 clock point. ------------------------*/

/**
 * @brief   USART2 clock register bits.
 */
#if (STM32_CFG_USART2_SEL == STM32_USART2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART2_BITS                   STM32_USART2SEL_PCLK1
#elif (STM32_CFG_USART2_SEL == STM32_USART2SEL_SYSCLK)
#define STM32_USART2_BITS                   STM32_USART2SEL_SYSCLK
#elif (STM32_CFG_USART2_SEL == STM32_USART2SEL_HSI16)
#define STM32_USART2_BITS                   STM32_USART2SEL_HSI16
#elif (STM32_CFG_USART2_SEL == STM32_USART2SEL_LSE)
#define STM32_USART2_BITS                   STM32_USART2SEL_LSE
#else
#error "invalid STM32_CFG_USART2_SEL value specified"
#endif

/**
 * @brief   USART2 clock point.
 */
#if ((STM32_USART2_ENABLED == TRUE) && \
     (STM32_CFG_USART2_SEL == STM32_USART2SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_USART2_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_USART2_ENABLED == TRUE) && \
      (STM32_CFG_USART2_SEL == STM32_USART2SEL_SYSCLK)
#define STM32_USART2_FREQ                   hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_USART2_ENABLED == TRUE) && \
      (STM32_CFG_USART2_SEL == STM32_USART2SEL_HSI16)
#define STM32_USART2_FREQ                   hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_USART2_ENABLED == TRUE) && \
      (STM32_CFG_USART2_SEL == STM32_USART2SEL_LSE)
#define STM32_USART2_FREQ                   STM32_LSE_FREQ
#else
#define STM32_USART2_FREQ                   0U
#endif

/*--- Macros and checks for the USART3 clock point. ------------------------*/

/**
 * @brief   USART3 clock register bits.
 */
#if (STM32_CFG_USART3_SEL == STM32_USART3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART3_BITS                   STM32_USART3SEL_PCLK1
#elif (STM32_CFG_USART3_SEL == STM32_USART3SEL_SYSCLK)
#define STM32_USART3_BITS                   STM32_USART3SEL_SYSCLK
#elif (STM32_CFG_USART3_SEL == STM32_USART3SEL_HSI16)
#define STM32_USART3_BITS                   STM32_USART3SEL_HSI16
#elif (STM32_CFG_USART3_SEL == STM32_USART3SEL_LSE)
#define STM32_USART3_BITS                   STM32_USART3SEL_LSE
#else
#error "invalid STM32_CFG_USART3_SEL value specified"
#endif

/**
 * @brief   USART3 clock point.
 */
#if ((STM32_USART3_ENABLED == TRUE) && \
     (STM32_CFG_USART3_SEL == STM32_USART3SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_USART3_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_USART3_ENABLED == TRUE) && \
      (STM32_CFG_USART3_SEL == STM32_USART3SEL_SYSCLK)
#define STM32_USART3_FREQ                   hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_USART3_ENABLED == TRUE) && \
      (STM32_CFG_USART3_SEL == STM32_USART3SEL_HSI16)
#define STM32_USART3_FREQ                   hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_USART3_ENABLED == TRUE) && \
      (STM32_CFG_USART3_SEL == STM32_USART3SEL_LSE)
#define STM32_USART3_FREQ                   STM32_LSE_FREQ
#else
#define STM32_USART3_FREQ                   0U
#endif

/*--- Macros and checks for the UART4 clock point. -------------------------*/

/**
 * @brief   UART4 clock register bits.
 */
#if (STM32_CFG_UART4_SEL == STM32_UART4SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART4_BITS                    STM32_UART4SEL_PCLK1
#elif (STM32_CFG_UART4_SEL == STM32_UART4SEL_SYSCLK)
#define STM32_UART4_BITS                    STM32_UART4SEL_SYSCLK
#elif (STM32_CFG_UART4_SEL == STM32_UART4SEL_HSI16)
#define STM32_UART4_BITS                    STM32_UART4SEL_HSI16
#elif (STM32_CFG_UART4_SEL == STM32_UART4SEL_LSE)
#define STM32_UART4_BITS                    STM32_UART4SEL_LSE
#else
#error "invalid STM32_CFG_UART4_SEL value specified"
#endif

/**
 * @brief   UART4 clock point.
 */
#if ((STM32_UART4_ENABLED == TRUE) && \
     (STM32_CFG_UART4_SEL == STM32_UART4SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_UART4_FREQ                    hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_UART4_ENABLED == TRUE) && \
      (STM32_CFG_UART4_SEL == STM32_UART4SEL_SYSCLK)
#define STM32_UART4_FREQ                    hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_UART4_ENABLED == TRUE) && \
      (STM32_CFG_UART4_SEL == STM32_UART4SEL_HSI16)
#define STM32_UART4_FREQ                    hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_UART4_ENABLED == TRUE) && \
      (STM32_CFG_UART4_SEL == STM32_UART4SEL_LSE)
#define STM32_UART4_FREQ                    STM32_LSE_FREQ
#else
#define STM32_UART4_FREQ                    0U
#endif

/*--- Macros and checks for the UART5 clock point. -------------------------*/

/**
 * @brief   UART5 clock register bits.
 */
#if (STM32_CFG_UART5_SEL == STM32_UART5SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART5_BITS                    STM32_UART5SEL_PCLK1
#elif (STM32_CFG_UART5_SEL == STM32_UART5SEL_SYSCLK)
#define STM32_UART5_BITS                    STM32_UART5SEL_SYSCLK
#elif (STM32_CFG_UART5_SEL == STM32_UART5SEL_HSI16)
#define STM32_UART5_BITS                    STM32_UART5SEL_HSI16
#elif (STM32_CFG_UART5_SEL == STM32_UART5SEL_LSE)
#define STM32_UART5_BITS                    STM32_UART5SEL_LSE
#else
#error "invalid STM32_CFG_UART5_SEL value specified"
#endif

/**
 * @brief   UART5 clock point.
 */
#if ((STM32_UART5_ENABLED == TRUE) && \
     (STM32_CFG_UART5_SEL == STM32_UART5SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_UART5_FREQ                    hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_UART5_ENABLED == TRUE) && \
      (STM32_CFG_UART5_SEL == STM32_UART5SEL_SYSCLK)
#define STM32_UART5_FREQ                    hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_UART5_ENABLED == TRUE) && \
      (STM32_CFG_UART5_SEL == STM32_UART5SEL_HSI16)
#define STM32_UART5_FREQ                    hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_UART5_ENABLED == TRUE) && \
      (STM32_CFG_UART5_SEL == STM32_UART5SEL_LSE)
#define STM32_UART5_FREQ                    STM32_LSE_FREQ
#else
#define STM32_UART5_FREQ                    0U
#endif

/*--- Macros and checks for the LPUART1 clock point. -----------------------*/

/**
 * @brief   LPUART1 clock register bits.
 */
#if (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_LPUART1_BITS                  STM32_LPUART1SEL_PCLK1
#elif (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_SYSCLK)
#define STM32_LPUART1_BITS                  STM32_LPUART1SEL_SYSCLK
#elif (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_HSI16)
#define STM32_LPUART1_BITS                  STM32_LPUART1SEL_HSI16
#elif (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_LSE)
#define STM32_LPUART1_BITS                  STM32_LPUART1SEL_LSE
#else
#error "invalid STM32_CFG_LPUART1_SEL value specified"
#endif

/**
 * @brief   LPUART1 clock point.
 */
#if ((STM32_LPUART1_ENABLED == TRUE) && \
     (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_LPUART1_FREQ                  hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_SYSCLK)
#define STM32_LPUART1_FREQ                  hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_HSI16)
#define STM32_LPUART1_FREQ                  hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == STM32_LPUART1SEL_LSE)
#define STM32_LPUART1_FREQ                  STM32_LSE_FREQ
#else
#define STM32_LPUART1_FREQ                  0U
#endif

/*--- Macros and checks for the I2C1 clock point. --------------------------*/

/**
 * @brief   I2C1 clock register bits.
 */
#if (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C1_BITS                     STM32_I2C1SEL_PCLK1
#elif (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_SYSCLK)
#define STM32_I2C1_BITS                     STM32_I2C1SEL_SYSCLK
#elif (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_HSI16)
#define STM32_I2C1_BITS                     STM32_I2C1SEL_HSI16
#else
#error "invalid STM32_CFG_I2C1_SEL value specified"
#endif

/**
 * @brief   I2C1 clock point.
 */
#if ((STM32_I2C1_ENABLED == TRUE) && \
     (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_I2C1_FREQ                     hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I2C1_ENABLED == TRUE) && \
      (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_SYSCLK)
#define STM32_I2C1_FREQ                     hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_I2C1_ENABLED == TRUE) && \
      (STM32_CFG_I2C1_SEL == STM32_I2C1SEL_HSI16)
#define STM32_I2C1_FREQ                     hal_lld_get_clock_point(CLK_HSI16)
#else
#define STM32_I2C1_FREQ                     0U
#endif

/*--- Macros and checks for the I2C2 clock point. --------------------------*/

/**
 * @brief   I2C2 clock register bits.
 */
#if (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C2_BITS                     STM32_I2C2SEL_PCLK1
#elif (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_SYSCLK)
#define STM32_I2C2_BITS                     STM32_I2C2SEL_SYSCLK
#elif (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_HSI16)
#define STM32_I2C2_BITS                     STM32_I2C2SEL_HSI16
#else
#error "invalid STM32_CFG_I2C2_SEL value specified"
#endif

/**
 * @brief   I2C2 clock point.
 */
#if ((STM32_I2C2_ENABLED == TRUE) && \
     (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_I2C2_FREQ                     hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I2C2_ENABLED == TRUE) && \
      (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_SYSCLK)
#define STM32_I2C2_FREQ                     hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_I2C2_ENABLED == TRUE) && \
      (STM32_CFG_I2C2_SEL == STM32_I2C2SEL_HSI16)
#define STM32_I2C2_FREQ                     hal_lld_get_clock_point(CLK_HSI16)
#else
#define STM32_I2C2_FREQ                     0U
#endif

/*--- Macros and checks for the I2C3 clock point. --------------------------*/

/**
 * @brief   I2C3 clock register bits.
 */
#if (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C3_BITS                     STM32_I2C3SEL_PCLK1
#elif (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_SYSCLK)
#define STM32_I2C3_BITS                     STM32_I2C3SEL_SYSCLK
#elif (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_HSI16)
#define STM32_I2C3_BITS                     STM32_I2C3SEL_HSI16
#else
#error "invalid STM32_CFG_I2C3_SEL value specified"
#endif

/**
 * @brief   I2C3 clock point.
 */
#if ((STM32_I2C3_ENABLED == TRUE) && \
     (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_I2C3_FREQ                     hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I2C3_ENABLED == TRUE) && \
      (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_SYSCLK)
#define STM32_I2C3_FREQ                     hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_I2C3_ENABLED == TRUE) && \
      (STM32_CFG_I2C3_SEL == STM32_I2C3SEL_HSI16)
#define STM32_I2C3_FREQ                     hal_lld_get_clock_point(CLK_HSI16)
#else
#define STM32_I2C3_FREQ                     0U
#endif

/*--- Macros and checks for the I2C4 clock point. --------------------------*/

/**
 * @brief   I2C4 clock register bits.
 */
#if (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C4_BITS                     STM32_I2C4SEL_PCLK1
#elif (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_SYSCLK)
#define STM32_I2C4_BITS                     STM32_I2C4SEL_SYSCLK
#elif (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_HSI16)
#define STM32_I2C4_BITS                     STM32_I2C4SEL_HSI16
#else
#error "invalid STM32_CFG_I2C4_SEL value specified"
#endif

/**
 * @brief   I2C4 clock point.
 */
#if ((STM32_I2C4_ENABLED == TRUE) && \
     (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_I2C4_FREQ                     hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I2C4_ENABLED == TRUE) && \
      (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_SYSCLK)
#define STM32_I2C4_FREQ                     hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_I2C4_ENABLED == TRUE) && \
      (STM32_CFG_I2C4_SEL == STM32_I2C4SEL_HSI16)
#define STM32_I2C4_FREQ                     hal_lld_get_clock_point(CLK_HSI16)
#else
#define STM32_I2C4_FREQ                     0U
#endif

/*--- Macros and checks for the LPTIM1 clock point. ------------------------*/

/**
 * @brief   LPTIM1 clock register bits.
 */
#if (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_LPTIM1_BITS                   STM32_LPTIM1SEL_PCLK1
#elif (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_LSI)
#define STM32_LPTIM1_BITS                   STM32_LPTIM1SEL_LSI
#elif (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_HSI16)
#define STM32_LPTIM1_BITS                   STM32_LPTIM1SEL_HSI16
#elif (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_LSE)
#define STM32_LPTIM1_BITS                   STM32_LPTIM1SEL_LSE
#else
#error "invalid STM32_CFG_LPTIM1_SEL value specified"
#endif

/**
 * @brief   LPTIM1 clock point.
 */
#if ((STM32_LPTIM1_ENABLED == TRUE) && \
     (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_PCLK1)) || \
    defined(__DOXYGEN__)
#define STM32_LPTIM1_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_LPTIM1_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_LSI)
#define STM32_LPTIM1_FREQ                   STM32_LSI_FREQ
#elif (STM32_LPTIM1_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_HSI16)
#define STM32_LPTIM1_FREQ                   hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_LPTIM1_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM1_SEL == STM32_LPTIM1SEL_LSE)
#define STM32_LPTIM1_FREQ                   STM32_LSE_FREQ
#else
#define STM32_LPTIM1_FREQ                   0U
#endif

/*--- Macros and checks for the SAI1 clock point. --------------------------*/

/**
 * @brief   SAI1 clock register bits.
 */
#if (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_SYSCLK) || defined(__DOXYGEN__)
#define STM32_SAI1_BITS                     STM32_SAI1SEL_SYSCLK
#elif (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_PLLQCLK)
#define STM32_SAI1_BITS                     STM32_SAI1SEL_PLLQCLK
#elif (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_CKIN)
#define STM32_SAI1_BITS                     STM32_SAI1SEL_CKIN
#elif (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_HSI16)
#define STM32_SAI1_BITS                     STM32_SAI1SEL_HSI16
#else
#error "invalid STM32_CFG_SAI1_SEL value specified"
#endif

/**
 * @brief   SAI1 clock point.
 */
#if ((STM32_SAI1_ENABLED == TRUE) && \
     (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_SYSCLK)) || \
    defined(__DOXYGEN__)
#define STM32_SAI1_FREQ                     hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_SAI1_ENABLED == TRUE) && \
      (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_PLLQCLK)
#define STM32_SAI1_FREQ                     hal_lld_get_clock_point(CLK_PLLQ)
#elif (STM32_SAI1_ENABLED == TRUE) && \
      (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_CKIN)
#define STM32_SAI1_FREQ                     STM32_CKIN_FREQ
#elif (STM32_SAI1_ENABLED == TRUE) && \
      (STM32_CFG_SAI1_SEL == STM32_SAI1SEL_HSI16)
#define STM32_SAI1_FREQ                     hal_lld_get_clock_point(CLK_HSI16)
#else
#define STM32_SAI1_FREQ                     0U
#endif

/*--- Macros and checks for the I2S23 clock point. -------------------------*/

/**
 * @brief   I2S23 clock register bits.
 */
#if (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_SYSCLK) || defined(__DOXYGEN__)
#define STM32_I2S23_BITS                    STM32_I2S23SEL_SYSCLK
#elif (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_PLLQCLK)
#define STM32_I2S23_BITS                    STM32_I2S23SEL_PLLQCLK
#elif (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_CKIN)
#define STM32_I2S23_BITS                    STM32_I2S23SEL_CKIN
#elif (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_HSI16)
#define STM32_I2S23_BITS                    STM32_I2S23SEL_HSI16
#else
#error "invalid STM32_CFG_I2S23_SEL value specified"
#endif

/**
 * @brief   I2S23 clock point.
 */
#if ((STM32_I2S23_ENABLED == TRUE) && \
     (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_SYSCLK)) || \
    defined(__DOXYGEN__)
#define STM32_I2S23_FREQ                    hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_I2S23_ENABLED == TRUE) && \
      (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_PLLQCLK)
#define STM32_I2S23_FREQ                    hal_lld_get_clock_point(CLK_PLLQ)
#elif (STM32_I2S23_ENABLED == TRUE) && \
      (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_CKIN)
#define STM32_I2S23_FREQ                    STM32_CKIN_FREQ
#elif (STM32_I2S23_ENABLED == TRUE) && \
      (STM32_CFG_I2S23_SEL == STM32_I2S23SEL_HSI16)
#define STM32_I2S23_FREQ                    hal_lld_get_clock_point(CLK_HSI16)
#else
#define STM32_I2S23_FREQ                    0U
#endif

/*--- Macros and checks for the FDCAN clock point. -------------------------*/

/**
 * @brief   FDCAN clock register bits.
 */
#if (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_HSE) || defined(__DOXYGEN__)
#define STM32_FDCAN_BITS                    STM32_FDCANSEL_HSE
#elif (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_PLLQCLK)
#define STM32_FDCAN_BITS                    STM32_FDCANSEL_PLLQCLK
#elif (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_PCLK1)
#define STM32_FDCAN_BITS                    STM32_FDCANSEL_PCLK1
#else
#error "invalid STM32_CFG_FDCAN_SEL value specified"
#endif

/**
 * @brief   FDCAN clock point.
 */
#if ((STM32_FDCAN_ENABLED == TRUE) && \
     (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_HSE)) || \
    defined(__DOXYGEN__)
#define STM32_FDCAN_FREQ                    hal_lld_get_clock_point(CLK_HSE)
#elif (STM32_FDCAN_ENABLED == TRUE) && \
      (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_PLLQCLK)
#define STM32_FDCAN_FREQ                    hal_lld_get_clock_point(CLK_PLLQ)
#elif (STM32_FDCAN_ENABLED == TRUE) && \
      (STM32_CFG_FDCAN_SEL == STM32_FDCANSEL_PCLK1)
#define STM32_FDCAN_FREQ                    hal_lld_get_clock_point(CLK_PCLK1)
#else
#define STM32_FDCAN_FREQ                    0U
#endif

/*--- Macros and checks for the CLK48 clock point. -------------------------*/

/**
 * @brief   CLK48 clock register bits.
 */
#if (STM32_CFG_CLK48_SEL == STM32_CLK48SEL_HSI48) || defined(__DOXYGEN__)
#define STM32_CLK48_BITS                    STM32_CLK48SEL_HSI48
#elif (STM32_CFG_CLK48_SEL == STM32_CLK48SEL_PLLQCLK)
#define STM32_CLK48_BITS                    STM32_CLK48SEL_PLLQCLK
#else
#error "invalid STM32_CFG_CLK48_SEL value specified"
#endif

/**
 * @brief   48MHz clock clock point.
 */
#if ((STM32_CLK48_ENABLED == TRUE) && \
     (STM32_CFG_CLK48_SEL == STM32_CLK48SEL_HSI48)) || \
    defined(__DOXYGEN__)
#define STM32_CLK48_FREQ                    hal_lld_get_clock_point(CLK_HSI48)
#elif (STM32_CLK48_ENABLED == TRUE) && \
      (STM32_CFG_CLK48_SEL == STM32_CLK48SEL_PLLQCLK)
#define STM32_CLK48_FREQ                    hal_lld_get_clock_point(CLK_PLLQ)
#else
#define STM32_CLK48_FREQ                    0U
#endif

/*--- Macros and checks for the ADC12 clock point. -------------------------*/

/**
 * @brief   ADC12 clock register bits.
 */
#if (STM32_ADC12_ENABLED == FALSE)
#define STM32_ADC12_BITS                    STM32_ADC12SEL_NOCLK
#elif (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_NOCLK) || defined(__DOXYGEN__)
#define STM32_ADC12_BITS                    STM32_ADC12SEL_NOCLK
#elif (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_PLLPCLK)
#define STM32_ADC12_BITS                    STM32_ADC12SEL_PLLPCLK
#elif (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_SYSCLK)
#define STM32_ADC12_BITS                    STM32_ADC12SEL_SYSCLK
#else
#error "invalid STM32_CFG_ADC12_SEL value specified"
#endif

/**
 * @brief   ADC12 clock point.
 */
#if ((STM32_ADC12_ENABLED == TRUE) && \
     (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_NOCLK)) || \
    defined(__DOXYGEN__)
#define STM32_ADC12_FREQ                    STM32_NONE_FREQ
#elif (STM32_ADC12_ENABLED == TRUE) && \
      (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_PLLPCLK)
#define STM32_ADC12_FREQ                    hal_lld_get_clock_point(CLK_PLLP)
#elif (STM32_ADC12_ENABLED == TRUE) && \
      (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_SYSCLK)
#define STM32_ADC12_FREQ                    hal_lld_get_clock_point(CLK_SYSCLK)
#else
#define STM32_ADC12_FREQ                    0U
#endif

#if (STM32_ADC12_ENABLED == TRUE) && \
    (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_NOCLK) && \
    (STM32_NONE_FREQ > STM32_ADCCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_ADC12_FREQ above maximum frequency"
#endif

#if (STM32_ADC12_ENABLED == TRUE) && \
    (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_PLLPCLK) && \
    (STM32_PLLP_FREQ > STM32_ADCCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_ADC12_FREQ above maximum frequency"
#endif

#if (STM32_ADC12_ENABLED == TRUE) && \
    (STM32_CFG_ADC12_SEL == STM32_ADC12SEL_SYSCLK) && \
    (STM32_SYSCLK_FREQ > STM32_ADCCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_ADC12_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the ADC345 clock point. ------------------------*/

/**
 * @brief   ADC345 clock register bits.
 */
#if (STM32_ADC345_ENABLED == FALSE)
#define STM32_ADC345_BITS                   STM32_ADC345SEL_NOCLK
#elif (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_NOCLK) || defined(__DOXYGEN__)
#define STM32_ADC345_BITS                   STM32_ADC345SEL_NOCLK
#elif (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_PLLPCLK)
#define STM32_ADC345_BITS                   STM32_ADC345SEL_PLLPCLK
#elif (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_SYSCLK)
#define STM32_ADC345_BITS                   STM32_ADC345SEL_SYSCLK
#else
#error "invalid STM32_CFG_ADC345_SEL value specified"
#endif

/**
 * @brief   ADC345 clock point.
 */
#if ((STM32_ADC345_ENABLED == TRUE) && \
     (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_NOCLK)) || \
    defined(__DOXYGEN__)
#define STM32_ADC345_FREQ                   STM32_NONE_FREQ
#elif (STM32_ADC345_ENABLED == TRUE) && \
      (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_PLLPCLK)
#define STM32_ADC345_FREQ                   hal_lld_get_clock_point(CLK_PLLP)
#elif (STM32_ADC345_ENABLED == TRUE) && \
      (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_SYSCLK)
#define STM32_ADC345_FREQ                   hal_lld_get_clock_point(CLK_SYSCLK)
#else
#define STM32_ADC345_FREQ                   0U
#endif

#if (STM32_ADC345_ENABLED == TRUE) && \
    (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_NOCLK) && \
    (STM32_NONE_FREQ > STM32_ADCCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_ADC345_FREQ above maximum frequency"
#endif

#if (STM32_ADC345_ENABLED == TRUE) && \
    (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_PLLPCLK) && \
    (STM32_PLLP_FREQ > STM32_ADCCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_ADC345_FREQ above maximum frequency"
#endif

#if (STM32_ADC345_ENABLED == TRUE) && \
    (STM32_CFG_ADC345_SEL == STM32_ADC345SEL_SYSCLK) && \
    (STM32_SYSCLK_FREQ > STM32_ADCCLK_MAX) && \
    !defined(__DOXYGEN__)
#error "STM32_ADC345_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the QSPI clock point. --------------------------*/

/**
 * @brief   QSPI clock register bits.
 */
#if (STM32_CFG_QSPI_SEL == STM32_QSPISEL_SYSCLK) || defined(__DOXYGEN__)
#define STM32_QSPI_BITS                     STM32_QSPISEL_SYSCLK
#elif (STM32_CFG_QSPI_SEL == STM32_QSPISEL_HSI16)
#define STM32_QSPI_BITS                     STM32_QSPISEL_HSI16
#elif (STM32_CFG_QSPI_SEL == STM32_QSPISEL_PLLQCLK)
#define STM32_QSPI_BITS                     STM32_QSPISEL_PLLQCLK
#else
#error "invalid STM32_CFG_QSPI_SEL value specified"
#endif

/**
 * @brief   QSPI clock point.
 */
#if ((STM32_QSPI_ENABLED == TRUE) && \
     (STM32_CFG_QSPI_SEL == STM32_QSPISEL_SYSCLK)) || \
    defined(__DOXYGEN__)
#define STM32_QSPI_FREQ                     hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_QSPI_ENABLED == TRUE) && \
      (STM32_CFG_QSPI_SEL == STM32_QSPISEL_HSI16)
#define STM32_QSPI_FREQ                     hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_QSPI_ENABLED == TRUE) && \
      (STM32_CFG_QSPI_SEL == STM32_QSPISEL_PLLQCLK)
#define STM32_QSPI_FREQ                     hal_lld_get_clock_point(CLK_PLLQ)
#else
#define STM32_QSPI_FREQ                     0U
#endif

/*--- Macros and checks for the RNG clock point. ---------------------------*/

/**
 * @brief   RNG clock register bits.
 */
#define STM32_RNG_BITS                      0U

/**
 * @brief   RNG clock point.
 */
#if (STM32_RNG_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_RNG_FREQ                      STM32_CLK48_FREQ
#else
#define STM32_RNG_FREQ                      0U
#endif

/*--- Macros and checks for the USB clock point. ---------------------------*/

/**
 * @brief   USB clock register bits.
 */
#define STM32_USB_BITS                      0U

/**
 * @brief   USB clock point.
 */
#if (STM32_USB_ENABLED == TRUE) || defined(__DOXYGEN__)
#define STM32_USB_FREQ                      STM32_CLK48_FREQ
#else
#define STM32_USB_FREQ                      0U
#endif
/** @} */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the frequency of a clock point in Hz.
 * @note    Static implementation.
 *
 * @param[in] clkpt     clock point to be returned
 * @return              The clock point frequency in Hz or zero if the
 *                      frequency is unknown.
 *
 * @notapi
 */
#if (STM32_CFG_CLOCK_DYNAMIC == FALSE) || defined(__DOXYGEN__)
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_HSI16        ? STM32_HSI16_FREQ         :                       \
   (clkpt) == CLK_HSI48        ? STM32_HSI48_FREQ         :                       \
   (clkpt) == CLK_HSE          ? STM32_HSE_FREQ           :                       \
   (clkpt) == CLK_PLLP         ? STM32_PLLP_FREQ          :                       \
   (clkpt) == CLK_PLLQ         ? STM32_PLLQ_FREQ          :                       \
   (clkpt) == CLK_PLLR         ? STM32_PLLR_FREQ          :                       \
   (clkpt) == CLK_SYSCLK       ? STM32_SYSCLK_FREQ        :                       \
   (clkpt) == CLK_HCLK         ? STM32_HCLK_FREQ          :                       \
   (clkpt) == CLK_PCLK1        ? STM32_PCLK1_FREQ         :                       \
   (clkpt) == CLK_PCLK2        ? STM32_PCLK2_FREQ         :                       \
   (clkpt) == CLK_PCLK1TIM     ? STM32_PCLK1TIM_FREQ      :                       \
   (clkpt) == CLK_PCLK2TIM     ? STM32_PCLK2TIM_FREQ      :                       \
   (clkpt) == CLK_MCO          ? STM32_MCO_FREQ           :                       \
   0U)
#endif

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
