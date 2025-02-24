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
    limitations under the License.f
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

#elif defined(STM32C051xx)
#define PLATFORM_NAME           "STM32C0 Entry-level Mainstream MCU"

#elif defined(STM32C071xx)
#define PLATFORM_NAME           "STM32C0 Entry-level Mainstream MCU with USB"

#elif defined(STM32C091xx)
#define PLATFORM_NAME           "STM32C0 Entry-level Mainstream MCU with FDCAN"

#elif defined(STM32C092xx)
#define PLATFORM_NAME           "STM32C0 Entry-level Mainstream MCU with FDCAN"

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
#define CLK_HSIKERCLK           2U
#define CLK_HSIUSB48CLK         3U
#define CLK_HCLK                4U
#define CLK_PCLK                5U
#define CLK_PCLKTIM             6U
#define CLK_MCO                 7U
#define CLK_MCO2                8U
#define CLK_ARRAY_SIZE          9U
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
#define STM32_SW_HSIUSB48       (2U << 0)
#define STM32_SW_LSI            (3U << 0)
#define STM32_SW_LSE            (4U << 0)

#define STM32_MCO2SEL_MASK      (7U << 16)
#define STM32_MCO2SEL_FIELD(n)  ((n) << 16)
#define STM32_MCO2SEL_NOCLOCK   STM32_MCO2SEL_FIELD(0U)
#define STM32_MCO2SEL_SYSCLK    STM32_MCO2SEL_FIELD(1U)
#define STM32_MCO2SEL_HSI48     STM32_MCO2SEL_FIELD(3U)
#define STM32_MCO2SEL_HSE       STM32_MCO2SEL_FIELD(4U)
#define STM32_MCO2SEL_LSI       STM32_MCO2SEL_FIELD(6U)
#define STM32_MCO2SEL_LSE       STM32_MCO2SEL_FIELD(7U)
#define STM32_MCO2SEL_HSIUSB48  STM32_MCO2SEL_FIELD(8U)

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
#define STM32_MCOSEL_HSIUSB48   STM32_MCOSEL_FIELD(8U)

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

#define STM32_FDCAN1SEL_MASK    (3U << 8)
#define STM32_FDCAN1SEL_PCLK    (0U << 8)
#define STM32_FDCAN1SEL_HSIKER  (0U << 8)
#define STM32_FDCAN1SEL_HSE     (0U << 8)

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
 * @name    RCC_CCIPR2 register bits definitions
 * @{
 */
#define STM32_USBSEL_MASK       (1U << 12)
#define STM32_USBSEL_HSIUSB48   (0U << 12)
#define STM32_USBSEL_HSE        (1U << 12)
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
 * @brief   Enables or disables the HSIUSB48 clock source.
 */
#if !defined(STM32_HSIUSB48_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSIUSB48_ENABLED              FALSE
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
 * @brief   FDCAN1 clock source.
 */
#if !defined(STM32_FDCAN1SEL) || defined(__DOXYGEN__)
#define STM32_FDCAN1SEL                     STM32_FDCAN1SEL_PCLK
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
 * @brief   USB clock source.
 */
#if !defined(STM32_USBSEL) || defined(__DOXYGEN__)
#define STM32_USBSEL                        STM32_USBSEL_HSIUSB48
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

/*
 * Configuration-related checks.
 */
#if !defined(STM32C0xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C0xx_MCUCONF not defined"
#endif

#if defined(STM32C011xx) && !defined(STM32C011_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C011_MCUCONF not defined"

#elif defined(STM32C031xx) && !defined(STM32C031_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C031_MCUCONF not defined"

#elif defined(STM32C051xx) && !defined(STM32C051_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C051_MCUCONF not defined"

#elif defined(STM32C071xx) && !defined(STM32C071_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C071_MCUCONF not defined"

#elif defined(STM32C091xx) && !defined(STM32C091_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C091_MCUCONF not defined"

#elif defined(STM32C092xx) && !defined(STM32C092_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32C092_MCUCONF not defined"

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
/** @} */

/* Clock handlers.*/
#include "stm32_lse_v3.inc"
#include "stm32_lsi_v3.inc"
#include "stm32_hsi48.inc"
#include "stm32_hsiusb48.inc"
#include "stm32_hse.inc"

/*
 * HSI48 related checks.
 */
#if STM32_RCC_HAS_HSI48
#if STM32_HSI48_ENABLED
#else /* !STM32_HSI48_ENABLED */

  #if STM32_SW == STM32_SW_HSISYS
    #error "HSI48 not enabled, required by STM32_SW"
  #endif

  #if STM32_MCOSEL == STM32_MCOSEL_HSI48
    #error "HSI48 not enabled, required by STM32_MCOSEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_HSI48
    #error "HSI48 not enabled, required by STM32_MCO2SEL"
  #endif

  #if (STM32_ADCSEL == STM32_ADCSEL_HSIKER)
    #error "HSI48 not enabled, required by STM32_ADCSEL"
  #endif

  #if (STM32_I2C1SEL == STM32_I2C1SEL_HSIKER)
    #error "HSI48 not enabled, required by STM32_I2C1SEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_HSIKER)
    #error "HSI48 not enabled, required by STM32_USART1SEL"
  #endif

#endif /* !STM32_HSI48_ENABLED */
#endif /* STM32_RCC_HAS_HSI48 */

/*
 * HSIUSB48 related checks.
 */
#if STM32_RCC_HAS_HSIUSB48
#if STM32_HSIUSB48_ENABLED
#else /* !STM32_HSIUSB48_ENABLED */

  #if STM32_SW == STM32_SW_HSIUSB48
    #error "HSIUSB48 not enabled, required by STM32_SW"
  #endif

  #if STM32_MCOSEL == STM32_MCOSEL_HSIUSB48
    #error "HSIUSB48 not enabled, required by STM32_MCOSEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_HSIUSB48
    #error "HSIUSB48 not enabled, required by STM32_MCO2SEL"
  #endif

  #if STM32_USBSEL == STM32_USBSEL_HSIUSB48
    #error "HSIUSB48 not enabled, required by STM32_USBSEL"
  #endif

#endif /* !STM32_HSIUSB48_ENABLED */
#endif /* STM32_RCC_HAS_HSI48 */

/*
 * HSE related checks.
 */
#if STM32_HSE_ENABLED

  #if STM32_HSECLK == 0
   #error "HSE oscillator not available on the board"
  #endif

#else /* !STM32_HSE_ENABLED */

  #if STM32_SW == STM32_SW_HSE
    #error "HSE not enabled, required by STM32_SW"
  #endif

  #if STM32_MCOSEL == STM32_MCOSEL_HSE
    #error "HSE not enabled, required by STM32_MCOSEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_HSE
    #error "HSE not enabled, required by STM32_MCO2SEL"
  #endif

  #if STM32_RTCSEL == STM32_RTCSEL_HSEDIV
    #error "HSE not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_HAS_USB
    #if STM32_USBSEL == STM32_USBSEL_HSE
      #error "HSE not enabled, required by STM32_USBSEL"
    #endif
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

  #if STM32_MCO2SEL == STM32_MCO2SEL_LSI
    #error "LSI not enabled, required by STM32_MCO2SEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSI
    #error "LSI not enabled, required by STM32_LSCOSEL"
  #endif

#endif /* !STM32_LSI_ENABLED */

/*
 * LSE related checks.
 */
#if STM32_LSE_ENABLED

  #if STM32_LSECLK == 0
   #error "LSE oscillator not available on the board"
  #endif

#else /* !STM32_LSE_ENABLED */

  #if STM32_RTCSEL == STM32_RTCSEL_LSE
    #error "LSE not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_MCOSEL == STM32_MCOSEL_LSE
    #error "LSE not enabled, required by STM32_MCOSEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_LSE
    #error "LSE not enabled, required by STM32_MCO2SEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSE
    #error "LSE not enabled, required by STM32_LSCOSEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_LSE)
    #error "LSE not enabled, required by STM32_USART1SEL"
  #endif

#endif /* !STM32_LSE_ENABLED */

/**
 * @brief   STM32_HSIDIV field.
 */
#if (STM32_HSIDIV_VALUE == 1) || defined(__DOXYGEN__)
  #define STM32_HSIDIV              STM32_HSIDIV_DIV1

#elif STM32_HSIDIV_VALUE == 2
  #define STM32_HSIDIV              STM32_HSIDIV_DIV2

#elif STM32_HSIDIV_VALUE == 4
  #define STM32_HSIDIV              STM32_HSIDIV_DIV4

#elif STM32_HSIDIV_VALUE == 8
  #define STM32_HSIDIV              STM32_HSIDIV_DIV8

#elif STM32_HSIDIV_VALUE == 16
  #define STM32_HSIDIV              STM32_HSIDIV_DIV16

#elif STM32_HSIDIV_VALUE == 32
  #define STM32_HSIDIV              STM32_HSIDIV_DIV32

#elif STM32_HSIDIV_VALUE == 64
  #define STM32_HSIDIV              STM32_HSIDIV_DIV64

#elif STM32_HSIDIV_VALUE == 128
  #define STM32_HSIDIV              STM32_HSIDIV_DIV128

#else
  #error "invalid STM32_HSIDIV_VALUE value specified"
#endif

/**
 * @brief   STM32_HSIKER field.
 */
#if (STM32_HSIKER_VALUE == 1) || defined(__DOXYGEN__)
  #define STM32_HSIKER              STM32_HSIKER_DIV1

#elif STM32_HSIKER_VALUE == 2
  #define STM32_HSIKER              STM32_HSIKER_DIV2

#elif STM32_HSIKER_VALUE == 3
  #define STM32_HSIKER              STM32_HSIKER_DIV3

#elif STM32_HSIKER_VALUE == 4
  #define STM32_HSIKER              STM32_HSIKER_DIV4

#elif STM32_HSIKER_VALUE == 5
  #define STM32_HSIKER              STM32_HSIKER_DIV5

#elif STM32_HSIKER_VALUE == 6
  #define STM32_HSIKER              STM32_HSIKER_DIV6

#elif STM32_HSIKER_VALUE == 7
  #define STM32_HSIKER              STM32_HSIKER_DIV7

#elif STM32_HSIKER_VALUE == 8
  #define STM32_HSIKER              STM32_HSIKER_DIV8

#else
  #error "invalid STM32_HSIKER_VALUE value specified"
#endif

/**
 * @brief   HSISYS clock frequency.
 */
#define STM32_HSISYSCLK             (STM32_HSI48CLK / STM32_HSIDIV_VALUE)

/**
 * @brief   HSIKER clock frequency.
 */
#define STM32_HSIKERCLK             (STM32_HSI48CLK / STM32_HSIKER_VALUE)

/**
 * @brief   System clock source.
 */
#if (STM32_SW == STM32_SW_HSISYS)
  #define STM32_SYSCLK              STM32_HSISYSCLK

#elif (STM32_SW == STM32_SW_HSE)
  #define STM32_SYSCLK              STM32_HSECLK

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

#elif STM32_MCOSEL == STM32_MCOSEL_HSI48
  #define STM32_MCODIVCLK           STM32_HSI48CLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSE
  #define STM32_MCODIVCLK           STM32_HSECLK

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
 * @brief   MCO2 divider clock frequency.
 */
#if (STM32_MCO2SEL == STM32_MCO2SEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_MCO2DIVCLK          0

#elif STM32_MCO2SEL == STM32_MCO2SEL_SYSCLK
  #define STM32_MCO2DIVCLK          STM32_SYSCLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_HSI48
  #define STM32_MCO2DIVCLK          STM32_HSI48CLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_HSE
  #define STM32_MCO2DIVCLK          STM32_HSECLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_LSI
  #define STM32_MCO2DIVCLK          STM32_LSICLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_LSE
  #define STM32_MCO2DIVCLK          STM32_LSECLK

#else
  #error "invalid STM32_MCO2SEL value specified"
#endif

/**
 * @brief   MCO2 output pin clock frequency.
 */
#if (STM32_MCO2PRE == STM32_MCO2PRE_DIV1) || defined(__DOXYGEN__)
  #define STM32_MCO2CLK             STM32_MCO2DIVCLK

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV2
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 2)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV4
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 4)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV8
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 8)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV16
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 16)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV32
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 32)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV64
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 64)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV128
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / 128)

#else
  #error "invalid STM32_MCO2PRE value specified"
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

#elif STM32_USART1SEL == STM32_USART1SEL_HSIKER
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_HSIKERCLK)

#elif STM32_USART1SEL == STM32_USART1SEL_LSE
  #define STM32_USART1CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART1 clock"
#endif

/**
 * @brief   USART2 clock frequency.
 */
#define STM32_USART2CLK             hal_lld_get_clock_point(CLK_PCLK)

/**
 * @brief   FDCAN1 clock frequency.
 */
#if (STM32_FDCAN1SEL == STM32_FDCAN1SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_FDCAN1CLK           hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_FDCAN1SEL == STM32_FDCAN1SELL_HSIKER
  #define STM32_FDCAN1CLK           hal_lld_get_clock_point(CLK_HSIKERCLK)

#elif STM32_FDCAN1SEL == STM32_FDCAN1SELL_HSE
  #define STM32_FDCAN1CLK           STM32_HSECLK

#else
  #error "invalid source selected for FDCAN1 clock"
#endif

/**
 * @brief   I2C1 clock frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK) || defined(__DOXYGEN__)
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_PCLK)

#elif STM32_I2C1SEL == STM32_I2C1SEL_SYSCLK
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2C1SEL == STM32_I2C1SEL_HSIKER
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_HSIKERCLK)

#else
  #error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C2 clock frequency.
 */
#define STM32_I2C2CLK               hal_lld_get_clock_point(CLK_PCLK)

/**
 * @brief   I2S1 clock frequency.
 */
#if (STM32_I2S1SEL == STM32_I2S1SEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_I2S1CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2S1SEL == STM32_I2S1SEL_PLLPCLK
  #define STM32_I2S1CLK             hal_lld_get_clock_point(CLK_PLLPCLK)

#elif STM32_I2S1SEL == STM32_I2S1SEL_HSIKER
  #define STM32_I2S1CLK             hal_lld_get_clock_point(CLK_HSIKERCLK)

#elif STM32_I2S1SEL == STM32_I2S1SEL_CKIN
  #define STM32_I2S1CLK             0 /* Unknown, would require a board value */

#else
#error "invalid source selected for I2S1 clock"
#endif

/**
 * @brief   ADC clock frequency.
 */
#if (STM32_ADCSEL == STM32_ADCSEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_ADCCLK              hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_ADCSEL == STM32_ADCSEL_HSIKER
  #define STM32_ADCCLK              hal_lld_get_clock_point(CLK_HSIKERCLK)

#else
  #error "invalid source selected for ADC clock"
#endif

/**
 * @brief   USB clock frequency.
 */
#if (STM32_USBSEL == STM32_USBSEL_HSIUSB48) || defined(__DOXYGEN__)
  #define STM32_USBCLK              hal_lld_get_clock_point(CLK_HSIUSB48CLK)

#elif STM32_USBSEL == STM32_USBSEL_HSE
  #define STM32_USBCLK              STM32_HSECLK

#else
  #error "invalid source selected for USB clock"
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
 * @brief   Clock of timers connected to APB1.
 */
#define STM32_TIMCLK1               hal_lld_get_clock_point(CLK_PCLKTIM)

/**
 * @brief   Clock of timers connected to APB2.
 */
#define STM32_TIMCLK2               hal_lld_get_clock_point(CLK_PCLKTIM)

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
  #define STM32_FLASHBITS           0

#else
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_0
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a clock point identifier.
 */
typedef unsigned halclkpt_t;

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
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_SYSCLK        ? STM32_SYSCLK      :                       \
   (clkpt) == CLK_HSISYSCLK     ? STM32_HSISYSCLK   :                       \
   (clkpt) == CLK_HSIKERCLK     ? STM32_HSIKERCLK   :                       \
   (clkpt) == CLK_HSIUSB48CLK   ? STM32_HSIUSB48CLK :                       \
   (clkpt) == CLK_HCLK          ? STM32_HCLK        :                       \
   (clkpt) == CLK_PCLK          ? STM32_PCLK        :                       \
   (clkpt) == CLK_PCLKTIM       ? STM32_TIMPCLK     :                       \
   (clkpt) == CLK_MCO           ? STM32_MCOCLK      :                       \
   (clkpt) == CLK_MCO2          ? STM32_MCO2CLK     :                       \
   0U)

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
