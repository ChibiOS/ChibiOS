/*
    ChibiOS - Copyright (C) 2006..2022 Giovanni Di Sirio

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
 * @file    STM32C0xx/hal_lld.h
 * @brief   STM32C0xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_LSEDRV.
 *          - STM32_LSE_BYPASS (optionally).
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32C011xx, STM32C011xx.
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
 * @brief   Requires use of SPIv2 driver model.
 */
#define HAL_LLD_SELECT_SPI_V2           TRUE

/**
 * @name    Platform identification
 * @{
 */
#if defined(STM32C011xx) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32C0 Entry-level Mainstream MCU"

#elif defined(STM32C031xx)
#define PLATFORM_NAME           "STM32C0 Entry-level Mainstream MCU"

#else
#error "STM32C0 device not specified"
#endif

/**
 * @brief   Sub-family identifier.
 */
#if !defined(STM32C0XX) || defined(__DOXYGEN__)
#define STM32C0XX
#endif
/** @} */

/**
 * @name   Clock points names
 * @{
 */
#define CLK_SYSCLK              0U
#define CLK_HSISYSCLK           1U
#define CLK_HSIKERCLK           1U
#define CLK_HCLK                2U
#define CLK_PCLK                3U
#define CLK_PCLKTIM             4U
#define CLK_MCO1                5U
#define CLK_MCO2                6U
#define CLK_ARRAY_SIZE          7U
/** @} */

/**
 * @name    RCC_CR register bits definitions
 * @{
 */
#define STM32_HSIDIV_MASK       (7U << 11)
#define STM32_HSIDIV_FIELD(n)   ((n) << 11)
#define STM32_HSIDIV_DIV1       STM32_HSIDIV_FIELD(0U)
#define STM32_HSIDIV_DIV2       STM32_HSIDIV_FIELD(1U)
#define STM32_HSIDIV_DIV4       STM32_HSIDIV_FIELD(2U)
#define STM32_HSIDIV_DIV8       STM32_HSIDIV_FIELD(3U)
#define STM32_HSIDIV_DIV16      STM32_HSIDIV_FIELD(4U)
#define STM32_HSIDIV_DIV32      STM32_HSIDIV_FIELD(5U)
#define STM32_HSIDIV_DIV64      STM32_HSIDIV_FIELD(6U)
#define STM32_HSIDIV_DIV128     STM32_HSIDIV_FIELD(7U)

#define STM32_HSIKER_MASK       (7U << 5)
#define STM32_HSIKER_FIELD(n)   ((n) << 5)
#define STM32_HSIKER_DIV1       STM32_HSIKER_FIELD(0U)
#define STM32_HSIKER_DIV2       STM32_HSIKER_FIELD(1U)
#define STM32_HSIKER_DIV3       STM32_HSIKER_FIELD(2U)
#define STM32_HSIKER_DIV4       STM32_HSIKER_FIELD(3U)
#define STM32_HSIKER_DIV5       STM32_HSIKER_FIELD(4U)
#define STM32_HSIKER_DIV6       STM32_HSIKER_FIELD(5U)
#define STM32_HSIKER_DIV7       STM32_HSIKER_FIELD(6U)
#define STM32_HSIKER_DIV8       STM32_HSIKER_FIELD(7U)
/** @} */

/**
 * @name    RCC_CFGR register bits definitions
 * @{
 */
#define STM32_SW_MASK           (7U << 0)
#define STM32_SW_HSISYS         (0U << 0)
#define STM32_SW_HSE            (1U << 0)
#define STM32_SW_LSI            (3U << 0)
#define STM32_SW_LSE            (4U << 0)

#define STM32_HPRE_DIV_MASK     (15U << 8)
#define STM32_HPRE_DIV_FIELD(n) ((n) << 8)
#define STM32_HPRE_DIV1         STM32_HPRE_DIV_FIELD(0U)
#define STM32_HPRE_DIV2         STM32_HPRE_DIV_FIELD(8U)
#define STM32_HPRE_DIV4         STM32_HPRE_DIV_FIELD(9U)
#define STM32_HPRE_DIV8         STM32_HPRE_DIV_FIELD(10U)
#define STM32_HPRE_DIV16        STM32_HPRE_DIV_FIELD(11U)
#define STM32_HPRE_DIV64        STM32_HPRE_DIV_FIELD(12U)
#define STM32_HPRE_DIV128       STM32_HPRE_DIV_FIELD(13U)
#define STM32_HPRE_DIV256       STM32_HPRE_DIV_FIELD(14U)
#define STM32_HPRE_DIV512       STM32_HPRE_DIV_FIELD(15U)

#define STM32_PPRE_DIV_MASK     (15U << 12)
#define STM32_PPRE_DIV_FIELD(n) ((n) << 12)
#define STM32_PPRE_DIV1         STM32_PPRE_DIV_FIELD(0U)
#define STM32_PPRE_DIV2         STM32_PPRE_DIV_FIELD(4U)
#define STM32_PPRE_DIV4         STM32_PPRE_DIV_FIELD(5U)
#define STM32_PPRE_DIV8         STM32_PPRE_DIV_FIELD(6U)
#define STM32_PPRE_DIV16        STM32_PPRE_DIV_FIELD(7U)

#define STM32_MCO2SEL_MASK      (7U << 16)
#define STM32_MCO2SEL_FIELD(n)  ((n) << 16)
#define STM32_MCO2SEL_NOCLOCK   STM32_MCO2SEL_FIELD(0U)
#define STM32_MCO2SEL_SYSCLK    STM32_MCO2SEL_FIELD(1U)
#define STM32_MCO2SEL_HSI48     STM32_MCO2SEL_FIELD(3U)
#define STM32_MCO2SEL_HSE       STM32_MCO2SEL_FIELD(4U)
#define STM32_MCO2SEL_LSI       STM32_MCO2SEL_FIELD(6U)
#define STM32_MCO2SEL_LSE       STM32_MCO2SEL_FIELD(7U)

#define STM32_MCO2PRE_MASK      (7U << 20)
#define STM32_MCO2PRE_FIELD(n)  ((n) << 20)
#define STM32_MCO2PRE_DIV1      STM32_MCO2PRE_FIELD(0U)
#define STM32_MCO2PRE_DIV2      STM32_MCO2PRE_FIELD(1U)
#define STM32_MCO2PRE_DIV4      STM32_MCO2PRE_FIELD(2U)
#define STM32_MCO2PRE_DIV8      STM32_MCO2PRE_FIELD(3U)
#define STM32_MCO2PRE_DIV16     STM32_MCO2PRE_FIELD(4U)
#define STM32_MCO2PRE_DIV32     STM32_MCO2PRE_FIELD(5U)
#define STM32_MCO2PRE_DIV64     STM32_MCO2PRE_FIELD(6U)
#define STM32_MCO2PRE_DIV128    STM32_MCO2PRE_FIELD(7U)

#define STM32_MCOSEL_MASK       (7U << 16)
#define STM32_MCOSEL_FIELD(n)   ((n) << 16)
#define STM32_MCOSEL_NOCLOCK    STM32_MCOSEL_FIELD(0U)
#define STM32_MCOSEL_SYSCLK     STM32_MCOSEL_FIELD(1U)
#define STM32_MCOSEL_HSI48      STM32_MCOSEL_FIELD(3U)
#define STM32_MCOSEL_HSE        STM32_MCOSEL_FIELD(4U)
#define STM32_MCOSEL_LSI        STM32_MCOSEL_FIELD(6U)
#define STM32_MCOSEL_LSE        STM32_MCOSEL_FIELD(7U)

#define STM32_MCOPRE_MASK       (7U << 28)
#define STM32_MCOPRE_FIELD(n)   ((n) << 28)
#define STM32_MCOPRE_DIV1       STM32_MCOPRE_FIELD(0U)
#define STM32_MCOPRE_DIV2       STM32_MCOPRE_FIELD(1U)
#define STM32_MCOPRE_DIV4       STM32_MCOPRE_FIELD(2U)
#define STM32_MCOPRE_DIV8       STM32_MCOPRE_FIELD(3U)
#define STM32_MCOPRE_DIV16      STM32_MCOPRE_FIELD(4U)
#define STM32_MCOPRE_DIV32      STM32_MCOPRE_FIELD(5U)
#define STM32_MCOPRE_DIV64      STM32_MCOPRE_FIELD(6U)
#define STM32_MCOPRE_DIV128     STM32_MCOPRE_FIELD(7U)
/** @} */

/**
 * @name    RCC_CCIPR register bits definitions
 * @{
 */
#define STM32_USART1SEL_MASK    (3U << 0)
#define STM32_USART1SEL_PCLK    (0U << 0)
#define STM32_USART1SEL_SYSCLK  (1U << 0)
#define STM32_USART1SEL_HSIKER  (2U << 0)
#define STM32_USART1SEL_LSE     (3U << 0)

#define STM32_I2C1SEL_MASK      (3U << 12)
#define STM32_I2C1SEL_PCLK      (0U << 12)
#define STM32_I2C1SEL_SYSCLK    (1U << 12)
#define STM32_I2C1SEL_HSIKER    (2U << 12)

#define STM32_I2S1SEL_MASK      (3U << 14)
#define STM32_I2S1SEL_SYSCLK    (0U << 14)
#define STM32_I2S1SEL_HSIKER    (2U << 14)
#define STM32_I2S1SEL_CKIN      (3U << 14)

#define STM32_ADCSEL_MASK       (3U << 30)
#define STM32_ADCSEL_SYSCLK     (0U << 30)
#define STM32_ADCSEL_HSIKER     (2U << 30)
/** @} */

/**
 * @name    RCC_CSR1 register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK       (3U << 8)
#define STM32_RTCSEL_NOCLOCK    (0U << 8)
#define STM32_RTCSEL_LSE        (1U << 8)
#define STM32_RTCSEL_LSI        (2U << 8)
#define STM32_RTCSEL_HSEDIV     (3U << 8)

#define STM32_LSCOSEL_MASK      (3U << 24)
#define STM32_LSCOSEL_NOCLOCK   (0U << 24)
#define STM32_LSCOSEL_LSI       (1U << 24)
#define STM32_LSCOSEL_LSE       (3U << 24)
/** @} */

/* ST headers inconsistencies...*/
#if !defined(FLASH_ACR_LATENCY_0WS)
#define FLASH_ACR_LATENCY_0WS   (0U << FLASH_ACR_LATENCY_Pos)
#endif
#if !defined(FLASH_ACR_LATENCY_1WS)
#define FLASH_ACR_LATENCY_1WS   (0U << FLASH_ACR_LATENCY_Pos)
#endif

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
 * @brief   Enables the dynamic clock handling.
 */
#if !defined(STM32_CLOCK_DYNAMIC) || defined(__DOXYGEN__)
#define STM32_CLOCK_DYNAMIC                 FALSE
#endif

/**
 * @brief   PWR CR3 register initialization value.
 */
#if !defined(STM32_PWR_CR3) || defined(__DOXYGEN__)
#define STM32_PWR_CR3                       (PWR_CR3_EIWUL)
#endif

/**
 * @brief   PWR CR4 register initialization value.
 */
#if !defined(STM32_PWR_CR4) || defined(__DOXYGEN__)
#define STM32_PWR_CR4                       (0U)
#endif

/**
 * @brief   PWR PUCRA register initialization value.
 */
#if !defined(STM32_PWR_PUCRA) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRA                     (0U)
#endif

/**
 * @brief   PWR PDCRA register initialization value.
 */
#if !defined(STM32_PWR_PDCRA) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRA                     (0U)
#endif

/**
 * @brief   PWR PUCRB register initialization value.
 */
#if !defined(STM32_PWR_PUCRB) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRB                     (0U)
#endif

/**
 * @brief   PWR PDCRB register initialization value.
 */
#if !defined(STM32_PWR_PDCRB) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRB                     (0U)
#endif

/**
 * @brief   PWR PUCRC register initialization value.
 */
#if !defined(STM32_PWR_PUCRC) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRC                     (0U)
#endif

/**
 * @brief   PWR PDCRC register initialization value.
 */
#if !defined(STM32_PWR_PDCRC) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRC                     (0U)
#endif

/**
 * @brief   PWR PUCRD register initialization value.
 */
#if !defined(STM32_PWR_PUCRD) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRD                     (0U)
#endif

/**
 * @brief   PWR PDCRD register initialization value.
 */
#if !defined(STM32_PWR_PDCRD) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRD                     (0U)
#endif

/**
 * @brief   PWR PUCRF register initialization value.
 */
#if !defined(STM32_PWR_PUCRF) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRF                     (0U)
#endif

/**
 * @brief   PWR PDCRF register initialization value.
 */
#if !defined(STM32_PWR_PDCRF) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRF                     (0U)
#endif

/**
 * @brief   HSIDIV divider value.
 * @note    The allowed values are 1, 2, 4, 8, 16, 32, 64, 128.
 */
#if !defined(STM32_HSIDIV_VALUE) || defined(__DOXYGEN__)
#define STM32_HSIDIV_VALUE                  4
#endif

/**
 * @brief   HSIKER divider value.
 * @note    The allowed values are 1..8.
 */
#if !defined(STM32_HSIKER_VALUE) || defined(__DOXYGEN__)
#define STM32_HSIKER_VALUE                  3
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
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM32_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSI_ENABLED                   FALSE
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
 */
#if !defined(STM32_SW) || defined(__DOXYGEN__)
#define STM32_SW                            STM32_SW_HSISYS
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
 * @brief   MCO2 clock source.
 */
#if !defined(STM32_MCO2SEL) || defined(__DOXYGEN__)
#define STM32_MCO2SEL                       STM32_MCO2SEL_NOCLOCK
#endif

/**
 * @brief   MCO2 divider setting.
 */
#if !defined(STM32_MCO2PRE) || defined(__DOXYGEN__)
#define STM32_MCO2PRE                       STM32_MCO2PRE_DIV1
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
 * @brief   I2C1 clock source.
 */
#if !defined(STM32_I2C1SEL) || defined(__DOXYGEN__)
#define STM32_I2C1SEL                       STM32_I2C1SEL_PCLK
#endif

/**
 * @brief   I2S1 clock source.
 */
#if !defined(STM32_I2S1SEL) || defined(__DOXYGEN__)
#define STM32_I2S1SEL                       STM32_I2S1SEL_SYSCLK
#endif

/**
 * @brief   ADC clock source.
 */
#if !defined(STM32_ADCSEL) || defined(__DOXYGEN__)
#define STM32_ADCSEL                        STM32_ADCSEL_SYSCLK
#endif

/**
 * @brief   RTC clock source.
 */
#if !defined(STM32_RTCSEL) || defined(__DOXYGEN__)
#define STM32_RTCSEL                        STM32_RTCSEL_NOCLOCK
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Clock handling mode selection.*/
#if STM32_CLOCK_DYNAMIC == TRUE
#define HAL_LLD_USE_CLOCK_MANAGEMENT
#endif

/*
 * Configuration-related checks.
 */
#if !defined(STM32C0xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C0xx_MCUCONF not defined"
#endif

#if defined(STM32C011xx) && !defined(STM32G011_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G011_MCUCONF not defined"

#elif defined(STM32C031xx) && !defined(STM32G031_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G031_MCUCONF not defined"

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

/**
 * @name    System Limits
 * @{
 */
#define STM32_SYSCLK_MAX                48000000
#define STM32_HSECLK_MAX                48000000
#define STM32_HSECLK_BYP_MAX            48000000
#define STM32_HSECLK_MIN                4000000
#define STM32_HSECLK_BYP_MIN            8000000
#define STM32_LSECLK_MAX                32768
#define STM32_LSECLK_BYP_MAX            1000000
#define STM32_LSECLK_MIN                32768
#define STM32_LSECLK_BYP_MIN            32768
#define STM32_PCLK_MAX                  48000000
#define STM32_ADCCLK_MAX                350000000

#define STM32_0WS_THRESHOLD             24000000
#define STM32_1WS_THRESHOLD             48000000
#define STM32_2WS_THRESHOLD             64000000
#define STM32_3WS_THRESHOLD             0
#define STM32_4WS_THRESHOLD             0
#define STM32_5WS_THRESHOLD             0
/** @} */

/* Clock handlers.*/
#include "stm32_lse.inc"
#include "stm32_lsi.inc"
#include "stm32_hsi48.inc"
#include "stm32_hse.inc"

/*
 * HSI16 related checks.
 */
#if STM32_HSI16_ENABLED
#else /* !STM32_HSI16_ENABLED */

  #if STM32_SW == STM32_SW_HSISYS
    #error "HSI16 not enabled, required by STM32_SW"
  #endif

  #if (STM32_SW == STM32_SW_PLLRCLK) && (STM32_PLLSRC == STM32_PLLSRC_HSI16)
    #error "HSI16 not enabled, required by STM32_SW and STM32_PLLSRC"
  #endif

  /* NOTE: Missing checks on the HSI16 pre-muxes, it is also required for newer
     L4 devices.*/

  #if (STM32_MCOSEL == STM32_MCOSEL_HSI16) ||                               \
      (STM32_I2S1SEL == STM32_I2S1SEL_HSI16) ||                             \
      (STM32_I2S2SEL == STM32_I2S2SEL_HSI16) ||                             \
      ((STM32_MCOSEL == STM32_MCOSEL_PLL) &&                                \
       (STM32_PLLSRC == STM32_PLLSRC_HSI16))
    #error "HSI16 not enabled, required by STM32_MCOSEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_USART1SEL"
  #endif
  #if (STM32_USART2SEL == STM32_USART2SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_USART2SEL"
  #endif
  #if (STM32_LPUART1SEL == STM32_LPUART1SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_LPUART1SEL"
  #endif

  #if (STM32_CECSEL == STM32_CECSEL_HSI16DIV)
    #error "HSI16 not enabled, required by STM32_CECSEL"
  #endif

  #if (STM32_I2C1SEL == STM32_I2C1SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_I2C1SEL"
  #endif
  #if (STM32_I2S1SEL == STM32_I2S1SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_I2S1SEL"
  #endif

  #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_LPTIM1SEL"
  #endif
  #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_HSI16)
    #error "HSI16 not enabled, required by STM32_LPTIM2SEL"
  #endif

  #if (STM32_RNGSEL == STM32_RNGSEL_HSI16)
    #error "HSI16 not enabled, required by STM32_RNGSEL"
  #endif

  #if (STM32_ADCSEL == STM32_ADCSEL_HSI16)
    #error "HSI16 not enabled, required by STM32_ADCSEL"
  #endif

#endif /* !STM32_HSI16_ENABLED */

/*
 * HSI48 related checks.
 */
#if STM32_RCC_HAS_HSI48
#if STM32_HSI48_ENABLED
#else /* !STM32_HSI48_ENABLED */

  #if STM32_MCOSEL == STM32_MCOSEL_HSI48
    #error "HSI48 not enabled, required by STM32_MCOSEL"
  #endif

  #if (STM32_USBSEL == STM32_USBSEL_HSI48) && (HAL_USE_USB == TRUE)
    #error "HSI48 not enabled, required by STM32_USBSEL"
  #endif

#endif /* !STM32_HSI48_ENABLED */
#endif /* STM32_RCC_HAS_HSI48 */

/*
 * HSE related checks.
 */
#if STM32_HSE_ENABLED
#else /* !STM32_HSE_ENABLED */

  #if STM32_SW == STM32_SW_HSE
    #error "HSE not enabled, required by STM32_SW"
  #endif

  #if (STM32_SW == STM32_SW_PLLRCLK) && (STM32_PLLSRC == STM32_PLLSRC_HSE)
    #error "HSE not enabled, required by STM32_SW and STM32_PLLSRC"
  #endif

  #if (STM32_MCOSEL == STM32_MCOSEL_HSE) ||                                 \
      (STM32_FDCANSEL == STM32_FDCANSEL_HSE) ||                             \
      (STM32_USBSEL == STM32_USBSEL_HSE) ||                                 \
      ((STM32_MCOSEL == STM32_MCOSEL_PLLRCLK) &&                            \
       (STM32_PLLSRC == STM32_PLLSRC_HSE))
    #error "HSE not enabled, required by STM32_MCOSEL"
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

  #if HAL_USE_RTC && (STM32_RTCSEL == STM32_RTCSEL_LSI)
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

#endif /* !STM32_LSE_ENABLED */

/**
 * @brief   STM32_HSIDIV field.
 */
#if (STM32_HSIDIV_VALUE == 1) || defined(__DOXYGEN__)
  #define STM32_HSIDIV              STM32_HSIDIV_1

#elif STM32_HSIDIV_VALUE == 2
  #define STM32_HSIDIV              STM32_HSIDIV_2

#elif STM32_HSIDIV_VALUE == 4
  #define STM32_HSIDIV              STM32_HSIDIV_4

#elif STM32_HSIDIV_VALUE == 8
  #define STM32_HSIDIV              STM32_HSIDIV_8

#elif STM32_HSIDIV_VALUE == 16
  #define STM32_HSIDIV              STM32_HSIDIV_16

#elif STM32_HSIDIV_VALUE == 32
  #define STM32_HSIDIV              STM32_HSIDIV_32

#elif STM32_HSIDIV_VALUE == 64
  #define STM32_HSIDIV              STM32_HSIDIV_64

#elif STM32_HSIDIV_VALUE == 128
  #define STM32_HSIDIV              STM32_HSIDIV_128

#else
  #error "invalid STM32_HSIDIV_VALUE value specified"
#endif

/**
 * @brief   PLL input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
  #define STM32_PLLCLKIN            (STM32_HSECLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_HSI16
  #define STM32_PLLCLKIN            (STM32_HSI16CLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_NOCLOCK
  #define STM32_PLLCLKIN            0

#else
#error "invalid STM32_PLLSRC value specified"
#endif

/*
 * PLL enable check.
 */
#if (STM32_SW == STM32_SW_PLLRCLK) ||                                       \
    (STM32_MCOSEL == STM32_MCOSEL_PLLRCLK) ||                               \
    (STM32_TIM1SEL == STM32_TIM1SEL_PLLQCLK) ||                             \
    (STM32_TIM15SEL == STM32_TIM15SEL_PLLQCLK) ||                           \
    (STM32_RNGSEL == STM32_RNGSEL_PLLQCLK) ||                               \
    (STM32_ADCSEL == STM32_ADCSEL_PLLPCLK) ||                               \
    (STM32_I2S1SEL == STM32_I2S1SEL_PLLPCLK) ||                             \
    (STM32_I2S2SEL == STM32_I2S2SEL_PLLPCLK) ||                             \
    (STM32_FDCANSEL == STM32_FDCANSEL_PLLQCLK) ||                           \
    (STM32_USBSEL == STM32_USBSEL_PLLQCLK) ||                               \
    defined(__DOXYGEN__)
  /**
   * @brief   PLL activation flag.
   */
  #define STM32_ACTIVATE_PLL          TRUE

#else
  #define STM32_ACTIVATE_PLL          FALSE
#endif

/**
 * @brief   STM32_PLLREN field.
 */
#if (STM32_SW == STM32_SW_PLLRCLK) ||                                       \
    (STM32_MCOSEL == STM32_MCOSEL_PLLRCLK) ||                               \
    defined(__DOXYGEN__)
#define STM32_PLLREN                (1 << 28)
#else
#define STM32_PLLREN                (0 << 28)
#endif

/**
 * @brief   STM32_PLLQEN field.
 */
#if (STM32_TIM1SEL == STM32_TIM1SEL_PLLQCLK) ||                             \
    (STM32_TIM15SEL == STM32_TIM15SEL_PLLQCLK) ||                           \
    (STM32_RNGSEL == STM32_RNGSEL_PLLQCLK) ||                               \
    (STM32_FDCANSEL == STM32_FDCANSEL_PLLQCLK) ||                           \
    (STM32_USBSEL == STM32_USBSEL_PLLQCLK) ||                               \
    defined(__DOXYGEN__)
#define STM32_PLLQEN                (1 << 24)
#else
#define STM32_PLLQEN                (0 << 24)
#endif

/**
 * @brief   STM32_PLLPEN field.
 */
#if (STM32_ADCSEL == STM32_ADCSEL_PLLPCLK) ||                               \
    (STM32_I2S1SEL == STM32_I2S1SEL_PLLPCLK) ||                             \
    (STM32_I2S2SEL == STM32_I2S2SEL_PLLPCLK) ||                             \
    defined(__DOXYGEN__)
#define STM32_PLLPEN                (1 << 16)
#else
#define STM32_PLLPEN                (0 << 16)
#endif

/* Inclusion of PLL-related checks and calculations.*/
#include <stm32_pll_v2.inc>

/**
 * @brief   HSISYS clock frequency.
 */
#define STM32_HSISYSCLK             (STM32_HSI16CLK / STM32_HSIDIV_VALUE)

/**
 * @brief   System clock source.
 */
#if STM32_NO_INIT || defined(__DOXYGEN__)
  #define STM32_SYSCLK              STM32_HSISYSCLK

#elif (STM32_SW == STM32_SW_HSISYS)
  #define STM32_SYSCLK              STM32_HSISYSCLK

#elif (STM32_SW == STM32_SW_HSE)
  #define STM32_SYSCLK              STM32_HSECLK

#elif (STM32_SW == STM32_SW_PLLRCLK)
  #define STM32_SYSCLK              STM32_PLL_R_CLKOUT

#elif (STM32_SW == STM32_SW_LSI)
  #define STM32_SYSCLK              STM32_LSICLK

#elif (STM32_SW == STM32_SW_LSE)
  #define STM32_SYSCLK              STM32_LSECLK

#else
#error "invalid STM32_SW value specified"
#endif

/* Bus handlers.*/
#include "stm32_ahb.inc"
#include "stm32_apb.inc"

/*
 * Compatibility definitions.
 */
#define STM32_PCLK1                 STM32_PCLK
#define STM32_PCLK2                 STM32_PCLK

/**
 * @brief   MCO divider clock frequency.
 */
#if (STM32_MCOSEL == STM32_MCOSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_MCODIVCLK           0

#elif STM32_MCOSEL == STM32_MCOSEL_SYSCLK
  #define STM32_MCODIVCLK           STM32_SYSCLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSI16
  #define STM32_MCODIVCLK           STM32_HSI16CLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSE
  #define STM32_MCODIVCLK           STM32_HSECLK

#elif STM32_MCOSEL == STM32_MCOSEL_PLLRCLK
  #define STM32_MCODIVCLK           STM32_PLL_R_CLKOUT

#elif STM32_MCOSEL == STM32_MCOSEL_LSI
  #define STM32_MCODIVCLK           STM32_LSICLK

#elif STM32_MCOSEL == STM32_MCOSEL_LSE
  #define STM32_MCODIVCLK           STM32_LSECLK

#else
  #error "invalid STM32_MCOSEL value specified"
#endif

/**
 * @brief   MCO output pin clock frequency.
 */
#if (STM32_MCOPRE == STM32_MCOPRE_DIV1) || defined(__DOXYGEN__)
  #define STM32_MCOCLK              STM32_MCODIVCLK

#elif STM32_MCOPRE == STM32_MCOPRE_DIV2
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 2)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV4
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 4)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV8
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 8)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV16
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 16)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV32
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 32)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV64
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 64)

#elif STM32_MCOPRE == STM32_MCOPRE_DIV128
  #define STM32_MCOCLK              (STM32_MCODIVCLK / 128)

#else
  #error "invalid STM32_MCOPRE value specified"
#endif

/**
 * @brief   RTC clock frequency.
 */
#if (STM32_RTCSEL == STM32_RTCSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_RTCCLK              0

#elif STM32_RTCSEL == STM32_RTCSEL_LSE
  #define STM32_RTCCLK              STM32_LSECLK

#elif STM32_RTCSEL == STM32_RTCSEL_LSI
  #define STM32_RTCCLK              STM32_LSICLK

#elif STM32_RTCSEL == STM32_RTCSEL_HSEDIV
  #define STM32_RTCCLK              (STM32_HSECLK / 32)

#else
  #error "invalid STM32_RTCSEL value specified"
#endif

/**
 * @brief   USART1 clock frequency.
 */
#if (STM32_USART1SEL == STM32_USART1SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_USART1SEL == STM32_USART1SEL_SYSCLK
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_USART1SEL == STM32_USART1SEL_HSI16
  #define STM32_USART1CLK           STM32_HSI16CLK

#elif STM32_USART1SEL == STM32_USART1SEL_LSE
  #define STM32_USART1CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART1 clock"
#endif

/**
 * @brief   USART2 clock frequency.
 */
#if (STM32_USART2SEL == STM32_USART2SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_USART2CLK           hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_USART2SEL == STM32_USART2SEL_SYSCLK
  #define STM32_USART2CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_USART2SEL == STM32_USART2SEL_HSI16
  #define STM32_USART2CLK           STM32_HSI16CLK

#elif STM32_USART2SEL == STM32_USART2SEL_LSE
  #define STM32_USART2CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART2 clock"
#endif

/**
 * @brief   USART3 clock frequency.
 */
#if defined(STM32G0B1xx) || defined(STM32G0C1xx)
#if (STM32_USART3SEL == STM32_USART3SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_USART3SEL == STM32_USART3SEL_SYSCLK
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_USART3SEL == STM32_USART3SEL_HSI16
  #define STM32_USART3CLK           STM32_HSI16CLK

#elif STM32_USART3SEL == STM32_USART3SEL_LSE
  #define STM32_USART3CLK           STM32_LSECLK
#else
  #error "invalid source selected for USART3 clock"
#endif

#else
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_PCLK)
#endif

/**
 * @brief   UART4 frequency.
 */
#define STM32_UART4CLK              hal_lld_get_clock_point(CLK_PCLK)

/**
 * @brief   UART5 frequency.
 */
#define STM32_UART5CLK              hal_lld_get_clock_point(CLK_PCLK)

/**
 * @brief   UART6 frequency.
 */
#define STM32_UART6CLK              hal_lld_get_clock_point(CLK_PCLK)

/**
 * @brief   LPUART1 clock frequency.
 */
#if (STM32_LPUART1SEL == STM32_LPUART1SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_SYSCLK
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_HSI16
  #define STM32_LPUART1CLK          STM32_HSI16CLK

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_LSE
  #define STM32_LPUART1CLK          STM32_LSECLK

#else
  #error "invalid source selected for LPUART1 clock"
#endif

/**
 * @brief   LPUART2 clock frequency.
 */
#if defined(STM32G0B1xx) || defined(STM32G0C1xx) || defined(__DOXYGEN__)
#if (STM32_LPUART2SEL == STM32_LPUART2SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_LPUART2CLK          hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_LPUART2SEL == STM32_LPUART2SEL_SYSCLK
  #define STM32_LPUART2CLK          hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_LPUART2SEL == STM32_LPUART2SEL_HSI16
  #define STM32_LPUART2CLK          STM32_HSI16CLK

#elif STM32_LPUART2SEL == STM32_LPUART2SEL_LSE
  #define STM32_LPUART2CLK          STM32_LSECLK

#else
  #error "invalid source selected for LPUART2 clock"
#endif
#endif

/**
 * @brief   CEC clock frequency.
 */
#if (STM32_CECSEL == STM32_CECSEL_HSI16DIV) || defined(__DOXYGEN__)
  #define STM32_CECCLK              (STM32_HSI16CLK / 448)

#elif STM32_CECSEL == STM32_CECSEL_LSE
  #define STM32_CECCLK              STM32_LSECLK

#else
  #error "invalid source selected for CEC clock"
#endif

/**
 * @brief   I2C1 clock frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_I2C1SEL == STM32_I2C1SEL_SYSCLK
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2C1SEL == STM32_I2C1SEL_HSI16
  #define STM32_I2C1CLK             STM32_HSI16CLK

#else
  #error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2S1 clock frequency.
 */
#if (STM32_I2S1SEL == STM32_I2S1SEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_I2S1CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2S1SEL == STM32_I2S1SEL_PLLPCLK
  #define STM32_I2S1CLK             hal_lld_get_clock_point(CLK_PLLPCLK)

#elif STM32_I2S1SEL == STM32_I2S1SEL_HSI16
  #define STM32_I2S1CLK             STM32_HSI16CLK

#elif STM32_I2S1SEL == STM32_I2S1SEL_CKIN
  #define STM32_I2S1CLK             0 /* Unknown, would require a board value */

#else
#error "invalid source selected for I2S1 clock"
#endif

/**
 * @brief   LPTIM1 clock frequency.
 */
#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_LPTIM1CLK           hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI
  #define STM32_LPTIM1CLK           STM32_LSICLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI16
  #define STM32_LPTIM1CLK           STM32_HSI16CLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE
  #define STM32_LPTIM1CLK           STM32_LSECLK

#else
#error "invalid source selected for LPTIM1 clock"
#endif

/**
 * @brief   LPTIM2 clock frequency.
 */
#if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_LPTIM2CLK           hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSI
  #define STM32_LPTIM2CLK           STM32_LSICLK

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_HSI16
  #define STM32_LPTIM2CLK           STM32_HSI16CLK

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSE
  #define STM32_LPTIM2CLK           STM32_LSECLK

#else
#error "invalid source selected for LPTIM2 clock"
#endif

/**
 * @brief   RNGDIV field.
 */
#if (STM32_RNGDIV_VALUE == 1) || defined(__DOXYGEN__)
  #define STM32_RNGDIV              (0U << 28U)

#elif STM32_RNGDIV_VALUE == 2
  #define STM32_RNGDIV              (1U << 28U)

#elif STM32_RNGDIV_VALUE == 4
  #define STM32_RNGDIV              (2U << 28U)

#elif STM32_RNGDIV_VALUE == 8
  #define STM32_RNGDIV              (3U << 28U)

#else
  #error "invalid STM32_RNGDIV_VALUE value specified"
#endif

/**
 * @brief   RNG clock frequency.
 */
#if (STM32_RNGSEL == STM32_RNGSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_RNGCLK              0

#elif STM32_RNGSEL == STM32_RNGSEL_HSI16
  #define STM32_RNGCLK              (STM32_HSI16CLK / STM32_RNGDIV_VALUE)

#elif STM32_RNGSEL == STM32_RNGSEL_SYSCLK
  #define STM32_RNGCLK              (hal_lld_get_clock_point(CLK_SYSCLK) / STM32_RNGDIV_VALUE)

#elif STM32_RNGSEL == STM32_RNGSEL_PLLQCLK
  #define STM32_RNGCLK              (hal_lld_get_clock_point(CLK_PLLQCLK) / STM32_RNGDIV_VALUE)

#else
#error "invalid source selected for RNG clock"
#endif

/**
 * @brief   ADC clock frequency.
 */
#if (STM32_ADCSEL == STM32_ADCSEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_ADCCLK              hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_ADCSEL == STM32_ADCSEL_PLLPCLK
  #define STM32_ADCCLK              hal_lld_get_clock_point(CLK_PLLPCLK)

#elif STM32_ADCSEL == STM32_ADCSEL_HSI16
  #define STM32_ADCCLK              STM32_HSI16CLK

#else
  #error "invalid source selected for ADC clock"
#endif

/**
 * @brief   TIMPCLK clock frequency.
 */
#if (STM32_PPRE == STM32_PPRE_DIV1) || defined(__DOXYGEN__)
  #define STM32_TIMPCLK             (STM32_PCLK * 1)

#else
  #define STM32_TIMPCLK             (STM32_PCLK * 2)
#endif

/**
 * @brief   TIM1 clock frequency.
 */
#if (STM32_TIM1SEL == STM32_TIM1SEL_TIMPCLK) || defined(__DOXYGEN__)
  #define STM32_TIM1CLK             STM32_TIMPCLK

#elif STM32_TIM1SEL == STM32_TIM1SEL_PLLQCLK
  #define STM32_TIM1CLK             hal_lld_get_clock_point(CLK_PLLQCLK)

#else
  #error "invalid source selected for TIM1 clock"
#endif

/**
 * @brief   TIM15 clock frequency.
 */
#if (STM32_TIM15SEL == STM32_TIM15SEL_TIMPCLK) || defined(__DOXYGEN__)
  #define STM32_TIM15CLK            STM32_TIMPCLK

#elif STM32_TIM15SEL == STM32_TIM15SEL_PLLQCLK
  #define STM32_TIM15CLK            hal_lld_get_clock_point(CLK_PLLQCLK)

#else
#error "invalid source selected for TIM15 clock"
#endif

/**
 * @brief   Clock of timers connected to APB1.
 */
#define STM32_TIMCLK1               hal_lld_get_clock_point(CLK_PCLKTIM)

/**
 * @brief   Clock of timers connected to APB2.
 */
#define STM32_TIMCLK2               hal_lld_get_clock_point(CLK_PCLKTIM)

#if STM32_HAS_TIM1617_ERRATA
/* TIM16 and TIM17 require special handling and checks on some devices, see
   the errata: "TIM16 and TIM17 are unduly clocked by SYSCLK".*/
#define STM32_TIM16CLK              hal_lld_get_clock_point(CLK_SYSCLK)
#define STM32_TIM17CLK              hal_lld_get_clock_point(CLK_SYSCLK)
#endif

/**
 * @brief   FDCAN clock point.
 */
#if (STM32_FDCANSEL == STM32_FDCANSEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_FDCANCLK            hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_FDCANSEL == STM32_FDCANSEL_PLLQCLK
  #define STM32_FDCANCLK            hal_lld_get_clock_point(CLK_PLLQCLK)

#elif STM32_FDCANSEL == STM32_FDCANSEL_HSE
  #define STM32_FDCANCLK            STM32_HSECLK

#else
  #error "invalid source selected for FDCAN clock"
#endif

/**
 * @brief   USB clock point.
 */
#if (STM32_USBSEL == STM32_USBSEL_HSI48) || defined(__DOXYGEN__)
  #define STM32_USBCLK              STM32_HSI48CLK

#elif STM32_USBSEL == STM32_USBSEL_PLLQCLK
  #define STM32_USBCLK              hal_lld_get_clock_point(CLK_PLLQCLK)

#elif STM32_USBSEL == STM32_USBSEL_HSE
  #define STM32_USBCLK              STM32_HSECLK

#else
  #error "invalid source selected for USB clock"
#endif

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
  #define STM32_FLASHBITS           0

#elif STM32_HCLK <= STM32_1WS_THRESHOLD
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_0

#elif STM32_HCLK <= STM32_2WS_THRESHOLD
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_1

#else
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_1 | FLASH_ACR_LATENCY_0)
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a clock point identifier.
 */
typedef unsigned halclkpt_t;

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Type of a clock point frequency in Hz.
 */
typedef uint32_t halfreq_t;

/**
 * @brief   Type of a clock configuration and switch structure.
 */
typedef struct {
  uint32_t          pwr_cr1;
  uint32_t          pwr_cr2;
  uint32_t          rcc_cr;
  uint32_t          rcc_cfgr;
  uint32_t          rcc_pllcfgr;
  uint32_t          flash_acr;
} halclkcfg_t;

/**
 * @brief   Type of a clock switch-only structure.
 */
typedef struct {
  uint32_t          pwr_cr1;
  uint32_t          rcc_cfgr;
  uint32_t          flash_acr;
} halclkswc_t;
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#if !defined(HAL_LLD_USE_CLOCK_MANAGEMENT)
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
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_SYSCLK    ? STM32_SYSCLK        :                         \
   (clkpt) == CLK_HSISYSCLK ? STM32_HSISYSCLK     :                         \
   (clkpt) == CLK_PLLPCLK   ? STM32_PLL_P_CLKOUT  :                         \
   (clkpt) == CLK_PLLQCLK   ? STM32_PLL_Q_CLKOUT  :                         \
   (clkpt) == CLK_PLLRCLK   ? STM32_PLL_R_CLKOUT  :                         \
   (clkpt) == CLK_HCLK      ? STM32_HCLK          :                         \
   (clkpt) == CLK_PCLK      ? STM32_PCLK          :                         \
   (clkpt) == CLK_PCLKTIM   ? STM32_TIMPCLK       :                         \
   (clkpt) == CLK_MCO       ? STM32_MCOCLK        :                         \
   0U)
#endif /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
#include "stm32_isr.h"
#include "stm32_dma.h"
#include "stm32_exti.h"
#include "stm32_rcc.h"
#include "stm32_tim.h"

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) && !defined(__DOXYGEN__)
extern const halclkcfg_t hal_clkcfg_reset;
extern const halclkcfg_t hal_clkcfg_default;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void stm32_clock_init(void);
#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
  bool hal_lld_clock_switch_mode(const halclkcfg_t *ccp);
  halfreq_t hal_lld_get_clock_point(halclkpt_t clkpt);
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
