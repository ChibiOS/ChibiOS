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
 * @file    STM32U5xx/hal_lld.h
 * @brief   STM32U5xx HAL subsystem low level driver header.
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
 * @brief   Requires use of SPIv2 driver model.
 */
#define HAL_LLD_SELECT_SPI_V2               TRUE

/**
 * @name    Platform identification
 * @{
 */
#if defined(STM32U535xx) || defined(__DOXYGEN__)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power"

#elif defined(STM32U545xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power with Crypto"

#elif defined(STM32U575xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power"

#elif defined(STM32U585xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power with Crypto"

#elif defined(STM32U595xx) || defined(STM32U599xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power with Graphics"

#elif defined(STM32U5A5xx) || defined(STM32U5A9xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power with Graphics"

#elif defined(STM32U5F7xx) || defined(STM32U5G7xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power with Neo-Chrom"

#elif defined(STM32U5F9xx) || defined(STM32U5G9xx)
  #define PLATFORM_NAME                     "STM32U5 Ultra Low Power with Graphics and Neo-Chrom"

#else
  #error "STM32U5 device not specified"
#endif

/**
 * @brief   Sub-family identifier.
 */
#if !defined(STM32U5XX) || defined(__DOXYGEN__)
  #define STM32U5XX
#endif
/** @} */

/**
 * @name   Clock points names
 * @{
 */
#define CLK_HSI16                           0U
#define CLK_HSI48                           1U
#define CLK_HSE                             2U
#define CLK_MSIS                            3U
#define CLK_MSIK                            4U
#define CLK_SYSCLK                          5U
#define CLK_PLL1PCLK                        6U
#define CLK_PLL1QCLK                        7U
#define CLK_PLL1RCLK                        8U
#define CLK_PLL2PCLK                        9U
#define CLK_PLL2QCLK                        10U
#define CLK_PLL2RCLK                        11U
#define CLK_PLL3PCLK                        12U
#define CLK_PLL3QCLK                        13U
#define CLK_PLL3RCLK                        14U
#define CLK_HCLK                            15U
#define CLK_PCLK1                           16U
#define CLK_PCLK1TIM                        17U
#define CLK_PCLK2                           18U
#define CLK_PCLK2TIM                        19U
#define CLK_PCLK3                           20U
#define CLK_MCO                             21U
#define CLK_ARRAY_SIZE                      22U

#define CLK_POINT_NAMES                                                     \
  {                                                                         \
    "HSI16", "HSI48", "HSE", "MSIS", "MSIK", "SYSCLK", "PLL1P", "PLL1Q",    \
    "PLL1R", "PLL2P", "PLL2Q", "PLL2R", "PLL3P", "PLL3Q", "PLL3R",          \
    "HCLK", "PCLK1", "PCLK1TIM", "PCLK2", "PCLK2TIM", "PCLK3", "MCO"        \
  }
/** @} */

/*===========================================================================*/
/* RCC/PWR/FLASH configuration aliases.                                      */
/*===========================================================================*/

/**
 * @name    FLASH_ACR register helpers
 * @{
 */
#define FLASH_ACR_LATENCY_FIELD(n)          ((n) << FLASH_ACR_LATENCY_Pos)
/** @} */

/**
 * @name    PWR_VOSR register helpers
 * @{
 */
#define PWR_VOSR_VOS_FIELD(n)               ((n) << PWR_VOSR_VOS_Pos)
#define PWR_VOSR_VOS_RANGE4                 PWR_VOSR_VOS_FIELD(0U)
#define PWR_VOSR_VOS_RANGE3                 PWR_VOSR_VOS_FIELD(1U)
#define PWR_VOSR_VOS_RANGE2                 PWR_VOSR_VOS_FIELD(2U)
#define PWR_VOSR_VOS_RANGE1                 PWR_VOSR_VOS_FIELD(3U)
/** @} */

/**
 * @name    RCC_ICSCR1 register helpers
 * @{
 */
#define RCC_ICSCR1_MSIBIAS_FIELD(n)         ((n) << RCC_ICSCR1_MSIBIAS_Pos)
#define RCC_ICSCR1_MSIBIAS_CONTINUOUS       RCC_ICSCR1_MSIBIAS_FIELD(0U)
#define RCC_ICSCR1_MSIBIAS_SAMPLING         RCC_ICSCR1_MSIBIAS_FIELD(1U)

#define RCC_ICSCR1_MSIRGSEL_FIELD(n)        ((n) << RCC_ICSCR1_MSIRGSEL_Pos)
#define RCC_ICSCR1_MSIRGSEL_CSR             RCC_ICSCR1_MSIRGSEL_FIELD(0U)
#define RCC_ICSCR1_MSIRGSEL_ICSCR1          RCC_ICSCR1_MSIRGSEL_FIELD(1U)

#define RCC_ICSCR1_MSIKRANGE_FIELD(n)       ((n) << RCC_ICSCR1_MSIKRANGE_Pos)
#define RCC_ICSCR1_MSISRANGE_FIELD(n)       ((n) << RCC_ICSCR1_MSISRANGE_Pos)
/** @} */

/**
 * @name    RCC_CFGR1 register helpers
 * @{
 */
#define RCC_CFGR1_SW_FIELD(n)               ((n) << RCC_CFGR1_SW_Pos)
#define RCC_CFGR1_SW_MSIS                   RCC_CFGR1_SW_FIELD(0U)
#define RCC_CFGR1_SW_HSI16                  RCC_CFGR1_SW_FIELD(1U)
#define RCC_CFGR1_SW_HSE                    RCC_CFGR1_SW_FIELD(2U)
#define RCC_CFGR1_SW_PLL1P                  RCC_CFGR1_SW_FIELD(3U)

#define RCC_CFGR1_SWS_FIELD(n)              ((n) << RCC_CFGR1_SWS_Pos)
#define RCC_CFGR1_SWS_MSIS                  RCC_CFGR1_SWS_FIELD(0U)
#define RCC_CFGR1_SWS_HSI16                 RCC_CFGR1_SWS_FIELD(1U)
#define RCC_CFGR1_SWS_HSE                   RCC_CFGR1_SWS_FIELD(2U)
#define RCC_CFGR1_SWS_PLL1P                 RCC_CFGR1_SWS_FIELD(3U)

#define RCC_CFGR1_STOPWUCK_FIELD(n)         ((n) << RCC_CFGR1_STOPWUCK_Pos)
#define RCC_CFGR1_STOPWUCK_MSIS             RCC_CFGR1_STOPWUCK_FIELD(0U)
#define RCC_CFGR1_STOPWUCK_HSI16            RCC_CFGR1_STOPWUCK_FIELD(1U)

#define RCC_CFGR1_STOPKERWUCK_FIELD(n)      ((n) << RCC_CFGR1_STOPKERWUCK_Pos)
#define RCC_CFGR1_STOPKERWUCK_MSIK          RCC_CFGR1_STOPKERWUCK_FIELD(0U)
#define RCC_CFGR1_STOPKERWUCK_HSI16         RCC_CFGR1_STOPKERWUCK_FIELD(1U)

#define RCC_CFGR1_MCOSEL_FIELD(n)           ((n) << RCC_CFGR1_MCOSEL_Pos)
#define RCC_CFGR1_MCOPRE_FIELD(n)           ((n) << RCC_CFGR1_MCOPRE_Pos)
#define RCC_CFGR1_MCOPRE_DIV1               RCC_CFGR1_MCOPRE_FIELD(0U)
#define RCC_CFGR1_MCOPRE_DIV2               RCC_CFGR1_MCOPRE_FIELD(1U)
#define RCC_CFGR1_MCOPRE_DIV4               RCC_CFGR1_MCOPRE_FIELD(2U)
#define RCC_CFGR1_MCOPRE_DIV8               RCC_CFGR1_MCOPRE_FIELD(3U)
#define RCC_CFGR1_MCOPRE_DIV16              RCC_CFGR1_MCOPRE_FIELD(4U)
#define RCC_CFGR1_MCOPRE_DIV32              RCC_CFGR1_MCOPRE_FIELD(5U)
#define RCC_CFGR1_MCOPRE_DIV64              RCC_CFGR1_MCOPRE_FIELD(6U)
#define RCC_CFGR1_MCOPRE_DIV128             RCC_CFGR1_MCOPRE_FIELD(7U)
/** @} */

/**
 * @name    RCC_CFGR2 register helpers
 * @{
 */
#if !defined(RCC_CFGR2_HPRE_FIELD)
#define RCC_CFGR2_HPRE_FIELD(n)             ((n) << RCC_CFGR2_HPRE_Pos)
#endif
#if !defined(RCC_CFGR2_HPRE_DIV1)
#define RCC_CFGR2_HPRE_DIV1                 RCC_CFGR2_HPRE_FIELD(0U)
#define RCC_CFGR2_HPRE_DIV2                 RCC_CFGR2_HPRE_FIELD(8U)
#define RCC_CFGR2_HPRE_DIV4                 RCC_CFGR2_HPRE_FIELD(9U)
#define RCC_CFGR2_HPRE_DIV8                 RCC_CFGR2_HPRE_FIELD(10U)
#define RCC_CFGR2_HPRE_DIV16                RCC_CFGR2_HPRE_FIELD(11U)
#define RCC_CFGR2_HPRE_DIV64                RCC_CFGR2_HPRE_FIELD(12U)
#define RCC_CFGR2_HPRE_DIV128               RCC_CFGR2_HPRE_FIELD(13U)
#define RCC_CFGR2_HPRE_DIV256               RCC_CFGR2_HPRE_FIELD(14U)
#define RCC_CFGR2_HPRE_DIV512               RCC_CFGR2_HPRE_FIELD(15U)
#endif

#if !defined(RCC_CFGR2_PPRE1_FIELD)
#define RCC_CFGR2_PPRE1_FIELD(n)            ((n) << RCC_CFGR2_PPRE1_Pos)
#endif
#if !defined(RCC_CFGR2_PPRE1_DIV1)
#define RCC_CFGR2_PPRE1_DIV1                RCC_CFGR2_PPRE1_FIELD(0U)
#define RCC_CFGR2_PPRE1_DIV2                RCC_CFGR2_PPRE1_FIELD(4U)
#define RCC_CFGR2_PPRE1_DIV4                RCC_CFGR2_PPRE1_FIELD(5U)
#define RCC_CFGR2_PPRE1_DIV8                RCC_CFGR2_PPRE1_FIELD(6U)
#define RCC_CFGR2_PPRE1_DIV16               RCC_CFGR2_PPRE1_FIELD(7U)
#endif

#if !defined(RCC_CFGR2_PPRE2_FIELD)
#define RCC_CFGR2_PPRE2_FIELD(n)            ((n) << RCC_CFGR2_PPRE2_Pos)
#endif
#if !defined(RCC_CFGR2_PPRE2_DIV1)
#define RCC_CFGR2_PPRE2_DIV1                RCC_CFGR2_PPRE2_FIELD(0U)
#define RCC_CFGR2_PPRE2_DIV2                RCC_CFGR2_PPRE2_FIELD(4U)
#define RCC_CFGR2_PPRE2_DIV4                RCC_CFGR2_PPRE2_FIELD(5U)
#define RCC_CFGR2_PPRE2_DIV8                RCC_CFGR2_PPRE2_FIELD(6U)
#define RCC_CFGR2_PPRE2_DIV16               RCC_CFGR2_PPRE2_FIELD(7U)
#endif
/** @} */

/**
 * @name    RCC_CFGR3 register helpers
 * @{
 */
#if !defined(RCC_CFGR3_PPRE3_FIELD)
#define RCC_CFGR3_PPRE3_FIELD(n)            ((n) << RCC_CFGR3_PPRE3_Pos)
#endif
#if !defined(RCC_CFGR3_PPRE3_DIV1)
#define RCC_CFGR3_PPRE3_DIV1                RCC_CFGR3_PPRE3_FIELD(0U)
#define RCC_CFGR3_PPRE3_DIV2                RCC_CFGR3_PPRE3_FIELD(4U)
#define RCC_CFGR3_PPRE3_DIV4                RCC_CFGR3_PPRE3_FIELD(5U)
#define RCC_CFGR3_PPRE3_DIV8                RCC_CFGR3_PPRE3_FIELD(6U)
#define RCC_CFGR3_PPRE3_DIV16               RCC_CFGR3_PPRE3_FIELD(7U)
#endif
/** @} */

/**
 * @name    RCC_PLL1CFGR register helpers
 * @{
 */
#define RCC_PLL1CFGR_PLL1SRC_FIELD(n)       ((n) << RCC_PLL1CFGR_PLL1SRC_Pos)
#define RCC_PLL1CFGR_PLL1SRC_NOCLOCK        RCC_PLL1CFGR_PLL1SRC_FIELD(0U)
#define RCC_PLL1CFGR_PLL1SRC_MSIS           RCC_PLL1CFGR_PLL1SRC_FIELD(1U)
#define RCC_PLL1CFGR_PLL1SRC_HSI16          RCC_PLL1CFGR_PLL1SRC_FIELD(2U)
#define RCC_PLL1CFGR_PLL1SRC_HSE            RCC_PLL1CFGR_PLL1SRC_FIELD(3U)

#define RCC_PLL1CFGR_PLL1RGE_FIELD(n)       ((n) << RCC_PLL1CFGR_PLL1RGE_Pos)
#define RCC_PLL1CFGR_PLL1RGE_1TO2           RCC_PLL1CFGR_PLL1RGE_FIELD(0U)
#define RCC_PLL1CFGR_PLL1RGE_2TO4           RCC_PLL1CFGR_PLL1RGE_FIELD(1U)
#define RCC_PLL1CFGR_PLL1RGE_4TO8           RCC_PLL1CFGR_PLL1RGE_FIELD(2U)
#define RCC_PLL1CFGR_PLL1RGE_8TO16          RCC_PLL1CFGR_PLL1RGE_FIELD(3U)
/** @} */

/**
 * @name    RCC_PLL2CFGR register helpers
 * @{
 */
#define RCC_PLL2CFGR_PLL2SRC_FIELD(n)       ((n) << RCC_PLL2CFGR_PLL2SRC_Pos)
#define RCC_PLL2CFGR_PLL2SRC_NOCLOCK        RCC_PLL2CFGR_PLL2SRC_FIELD(0U)
#define RCC_PLL2CFGR_PLL2SRC_MSIS           RCC_PLL2CFGR_PLL2SRC_FIELD(1U)
#define RCC_PLL2CFGR_PLL2SRC_HSI16          RCC_PLL2CFGR_PLL2SRC_FIELD(2U)
#define RCC_PLL2CFGR_PLL2SRC_HSE            RCC_PLL2CFGR_PLL2SRC_FIELD(3U)

#define RCC_PLL2CFGR_PLL2RGE_FIELD(n)       ((n) << RCC_PLL2CFGR_PLL2RGE_Pos)
#define RCC_PLL2CFGR_PLL2RGE_1TO2           RCC_PLL2CFGR_PLL2RGE_FIELD(0U)
#define RCC_PLL2CFGR_PLL2RGE_2TO4           RCC_PLL2CFGR_PLL2RGE_FIELD(1U)
#define RCC_PLL2CFGR_PLL2RGE_4TO8           RCC_PLL2CFGR_PLL2RGE_FIELD(2U)
#define RCC_PLL2CFGR_PLL2RGE_8TO16          RCC_PLL2CFGR_PLL2RGE_FIELD(3U)
/** @} */

/**
 * @name    RCC_PLL3CFGR register helpers
 * @{
 */
#define RCC_PLL3CFGR_PLL3SRC_FIELD(n)       ((n) << RCC_PLL3CFGR_PLL3SRC_Pos)
#define RCC_PLL3CFGR_PLL3SRC_NOCLOCK        RCC_PLL3CFGR_PLL3SRC_FIELD(0U)
#define RCC_PLL3CFGR_PLL3SRC_MSIS           RCC_PLL3CFGR_PLL3SRC_FIELD(1U)
#define RCC_PLL3CFGR_PLL3SRC_HSI16          RCC_PLL3CFGR_PLL3SRC_FIELD(2U)
#define RCC_PLL3CFGR_PLL3SRC_HSE            RCC_PLL3CFGR_PLL3SRC_FIELD(3U)

#define RCC_PLL3CFGR_PLL3RGE_FIELD(n)       ((n) << RCC_PLL3CFGR_PLL3RGE_Pos)
#define RCC_PLL3CFGR_PLL3RGE_1TO2           RCC_PLL3CFGR_PLL3RGE_FIELD(0U)
#define RCC_PLL3CFGR_PLL3RGE_2TO4           RCC_PLL3CFGR_PLL3RGE_FIELD(1U)
#define RCC_PLL3CFGR_PLL3RGE_4TO8           RCC_PLL3CFGR_PLL3RGE_FIELD(2U)
#define RCC_PLL3CFGR_PLL3RGE_8TO16          RCC_PLL3CFGR_PLL3RGE_FIELD(3U)
/** @} */

/**
 * @name    RCC_BDCR register helpers
 * @{
 */
#define RCC_BDCR_RTCSEL_FIELD(n)            ((n) << RCC_BDCR_RTCSEL_Pos)
#define RCC_BDCR_RTCSEL_NOCLOCK             RCC_BDCR_RTCSEL_FIELD(0U)
#define RCC_BDCR_RTCSEL_LSE                 RCC_BDCR_RTCSEL_FIELD(1U)
#define RCC_BDCR_RTCSEL_LSI                 RCC_BDCR_RTCSEL_FIELD(2U)
#define RCC_BDCR_RTCSEL_HSEDIV              RCC_BDCR_RTCSEL_FIELD(3U)

#define RCC_BDCR_LSCOSEL_FIELD(n)           ((n) << RCC_BDCR_LSCOSEL_Pos)
#define RCC_BDCR_LSCOSEL_NOCLOCK            0U
#define RCC_BDCR_LSCOSEL_LSI                (RCC_BDCR_LSCOEN |      \
                                             RCC_BDCR_LSCOSEL_FIELD(0U))
#define RCC_BDCR_LSCOSEL_LSE                (RCC_BDCR_LSCOEN |      \
                                             RCC_BDCR_LSCOSEL_FIELD(1U))
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT                       FALSE
#endif

#if !defined(STM32_PWR_VOSR) || defined(__DOXYGEN__)
#define STM32_PWR_VOSR                      PWR_VOSR_VOS_RANGE1
#endif

#if !defined(STM32_HSI16_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI16_ENABLED                 FALSE
#endif

#if !defined(STM32_HSI48_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI48_ENABLED                 FALSE
#endif

#if !defined(STM32_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSE_ENABLED                   FALSE
#endif

#if !defined(STM32_LSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSE_ENABLED                   FALSE
#endif

#if !defined(STM32_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSI_ENABLED                   FALSE
#endif

#if !defined(RCC_CCIPR1_USART1SEL_FIELD)
#define RCC_CCIPR1_USART1SEL_FIELD(n)       ((n) << RCC_CCIPR1_USART1SEL_Pos)
#define RCC_CCIPR1_USART1SEL_PCLK2          RCC_CCIPR1_USART1SEL_FIELD(0U)
#define RCC_CCIPR1_USART1SEL_HSI16          RCC_CCIPR1_USART1SEL_FIELD(1U)
#endif

#if !defined(RCC_CCIPR1_USART3SEL_FIELD)
#define RCC_CCIPR1_USART3SEL_FIELD(n)       ((n) << RCC_CCIPR1_USART3SEL_Pos)
#define RCC_CCIPR1_USART3SEL_PCLK1          RCC_CCIPR1_USART3SEL_FIELD(0U)
#define RCC_CCIPR1_USART3SEL_HSI16          RCC_CCIPR1_USART3SEL_FIELD(1U)
#endif

#if !defined(RCC_CCIPR1_UART4SEL_FIELD)
#define RCC_CCIPR1_UART4SEL_FIELD(n)        ((n) << RCC_CCIPR1_UART4SEL_Pos)
#define RCC_CCIPR1_UART4SEL_PCLK1           RCC_CCIPR1_UART4SEL_FIELD(0U)
#define RCC_CCIPR1_UART4SEL_HSI16           RCC_CCIPR1_UART4SEL_FIELD(1U)
#endif

#if !defined(RCC_CCIPR1_UART5SEL_FIELD)
#define RCC_CCIPR1_UART5SEL_FIELD(n)        ((n) << RCC_CCIPR1_UART5SEL_Pos)
#define RCC_CCIPR1_UART5SEL_PCLK1           RCC_CCIPR1_UART5SEL_FIELD(0U)
#define RCC_CCIPR1_UART5SEL_HSI16           RCC_CCIPR1_UART5SEL_FIELD(1U)
#endif

#if !defined(RCC_CCIPR3_LPUART1SEL_FIELD)
#define RCC_CCIPR3_LPUART1SEL_FIELD(n)      ((n) << RCC_CCIPR3_LPUART1SEL_Pos)
#define RCC_CCIPR3_LPUART1SEL_PCLK3         RCC_CCIPR3_LPUART1SEL_FIELD(0U)
#define RCC_CCIPR3_LPUART1SEL_HSI16         RCC_CCIPR3_LPUART1SEL_FIELD(1U)
#define RCC_CCIPR3_LPUART1SEL_LSE           RCC_CCIPR3_LPUART1SEL_FIELD(2U)
#define RCC_CCIPR3_LPUART1SEL_MSIK          RCC_CCIPR3_LPUART1SEL_FIELD(3U)
#endif

#if !defined(STM32_USART1SEL) || defined(__DOXYGEN__)
#define STM32_USART1SEL                     RCC_CCIPR1_USART1SEL_PCLK2
#endif

#if !defined(STM32_USART3SEL) || defined(__DOXYGEN__)
#define STM32_USART3SEL                     RCC_CCIPR1_USART3SEL_PCLK1
#endif

#if !defined(STM32_UART4SEL) || defined(__DOXYGEN__)
#define STM32_UART4SEL                      RCC_CCIPR1_UART4SEL_PCLK1
#endif

#if !defined(STM32_UART5SEL) || defined(__DOXYGEN__)
#define STM32_UART5SEL                      RCC_CCIPR1_UART5SEL_PCLK1
#endif

#if !defined(STM32_LPUART1SEL) || defined(__DOXYGEN__)
#define STM32_LPUART1SEL                    RCC_CCIPR3_LPUART1SEL_PCLK3
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * First clock model for the initial U5 demo enablement. The target currently
 * stays on the reset MSIS configuration, assumed here as 4 MHz.
 */
#define STM32_HSI16CLK                      16000000U
#define STM32_HSI48CLK                      48000000U
#define STM32_MSISCLK                       4000000U
#define STM32_MSIKCLK                       STM32_MSISCLK
#define STM32_SYSCLK                        STM32_MSISCLK
#define STM32_HCLK                          STM32_SYSCLK
#define STM32_PCLK1                         STM32_HCLK
#define STM32_PCLK1TIM                      STM32_PCLK1
#define STM32_PCLK2                         STM32_HCLK
#define STM32_PCLK2TIM                      STM32_PCLK2
#define STM32_PCLK3                         STM32_HCLK

#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_HSI16    ? STM32_HSI16CLK :                               \
   (clkpt) == CLK_HSI48    ? STM32_HSI48CLK :                               \
   (clkpt) == CLK_HSE      ? STM32_HSECLK   :                               \
   (clkpt) == CLK_MSIS     ? STM32_MSISCLK  :                               \
   (clkpt) == CLK_MSIK     ? STM32_MSIKCLK  :                               \
   (clkpt) == CLK_SYSCLK   ? STM32_SYSCLK   :                               \
   (clkpt) == CLK_HCLK     ? STM32_HCLK     :                               \
   (clkpt) == CLK_PCLK1    ? STM32_PCLK1    :                               \
   (clkpt) == CLK_PCLK1TIM ? STM32_PCLK1TIM :                               \
   (clkpt) == CLK_PCLK2    ? STM32_PCLK2    :                               \
   (clkpt) == CLK_PCLK2TIM ? STM32_PCLK2TIM :                               \
   (clkpt) == CLK_PCLK3    ? STM32_PCLK3    :                               \
   0U)

#if (STM32_USART1SEL == RCC_CCIPR1_USART1SEL_PCLK2) || defined(__DOXYGEN__)
#define STM32_USART1CLK                     hal_lld_get_clock_point(CLK_PCLK2)
#elif STM32_USART1SEL == RCC_CCIPR1_USART1SEL_HSI16
#define STM32_USART1CLK                     hal_lld_get_clock_point(CLK_HSI16)
#else
#error "unsupported STM32_USART1SEL value"
#endif

#if (STM32_USART3SEL == RCC_CCIPR1_USART3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART3CLK                     hal_lld_get_clock_point(CLK_PCLK1)
#elif STM32_USART3SEL == RCC_CCIPR1_USART3SEL_HSI16
#define STM32_USART3CLK                     hal_lld_get_clock_point(CLK_HSI16)
#else
#error "unsupported STM32_USART3SEL value"
#endif

#if (STM32_UART4SEL == RCC_CCIPR1_UART4SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART4CLK                      hal_lld_get_clock_point(CLK_PCLK1)
#elif STM32_UART4SEL == RCC_CCIPR1_UART4SEL_HSI16
#define STM32_UART4CLK                      hal_lld_get_clock_point(CLK_HSI16)
#else
#error "unsupported STM32_UART4SEL value"
#endif

#if (STM32_UART5SEL == RCC_CCIPR1_UART5SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART5CLK                      hal_lld_get_clock_point(CLK_PCLK1)
#elif STM32_UART5SEL == RCC_CCIPR1_UART5SEL_HSI16
#define STM32_UART5CLK                      hal_lld_get_clock_point(CLK_HSI16)
#else
#error "unsupported STM32_UART5SEL value"
#endif

#if (STM32_LPUART1SEL == RCC_CCIPR3_LPUART1SEL_PCLK3) || defined(__DOXYGEN__)
#define STM32_LPUART1CLK                    hal_lld_get_clock_point(CLK_PCLK3)
#elif STM32_LPUART1SEL == RCC_CCIPR3_LPUART1SEL_HSI16
#define STM32_LPUART1CLK                    hal_lld_get_clock_point(CLK_HSI16)
#elif STM32_LPUART1SEL == RCC_CCIPR3_LPUART1SEL_LSE
#define STM32_LPUART1CLK                    STM32_LSECLK
#elif STM32_LPUART1SEL == RCC_CCIPR3_LPUART1SEL_MSIK
#define STM32_LPUART1CLK                    hal_lld_get_clock_point(CLK_MSIK)
#else
#error "unsupported STM32_LPUART1SEL value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a timeout counter.
 */
typedef uint32_t halcnt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Real time counter frequency exported to the safety module.
 * @note    The counter is the internal DWT cycles counter so it runs at the
 *          same frequency as the CPU.
 */
#define HAL_LLD_GET_CNT_FREQUENCY()         hal_lld_get_clock_point(CLK_HCLK)

/**
 * @brief   Real time counter value exported to the safety module.
 */
#define HAL_LLD_GET_CNT_VALUE()             (DWT->CYCCNT)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "stm32_isr.h"
#include "stm32_exti.h"
#include "stm32_rcc.h"
#include "stm32_limits.h"
#include "stm32_tim.h"

#if HAL_USE_ADC || HAL_USE_DAC || HAL_USE_I2C || HAL_USE_SPI ||             \
    defined(__DOXYGEN__)
#include "stm32_dma3.h"
#endif

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
