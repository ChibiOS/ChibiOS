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
 * @file    STM32G4xx_TEST/hal_lld.h
 * @brief   STM32G4xx generated clock tree prototype HAL header.
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
#if defined(STM32G431xx) || defined(STM32G441xx) ||                         \
    defined(STM32G471xx) || defined(STM32G491xx) || defined(__DOXYGEN__)
#define PLATFORM_NAME                   "STM32G4 Access Line"

#elif defined(STM32G473xx)
#define PLATFORM_NAME                   "STM32G4 Performance Line"

#elif defined(STM32G483xx)
#define PLATFORM_NAME                   "STM32G4 Performance Line with Crypto"

#elif defined(STM32G474xx)
#define PLATFORM_NAME                   "STM32G4 Hi-resolution Line"

#elif defined(STM32G484xx)
#define PLATFORM_NAME                   "STM32G4 Hi-resolution Line with Crypto"

#elif defined(STM32GBK1CB)
#define PLATFORM_NAME                   "STM32G4 Mystery Line"

#else
#error "STM32G4 device not specified"
#endif

#if !defined(STM32G4XX) || defined(__DOXYGEN__)
#define STM32G4XX
#endif
/** @} */

/**
 * @name    PWR register bits definitions
 * @{
 */
#define STM32_VOS_MASK                  (3U << 9U)
#define STM32_VOS_RANGE1                (1U << 9U)
#define STM32_VOS_RANGE2                (2U << 9U)
/** @} */

/**
 * @name    RCC_CFGR register bits definitions
 * @{
 */
#define STM32_SW_MASK                   (3U << 0U)
#define STM32_SW_HSI16                  (1U << 0U)
#define STM32_SW_HSE                    (2U << 0U)
#define STM32_SW_PLLRCLK                (3U << 0U)

#define STM32_HPRE_FIELD(n)             ((n) << 4U)
#define STM32_HPRE_DIV1                 STM32_HPRE_FIELD(0U)
#define STM32_HPRE_DIV2                 STM32_HPRE_FIELD(8U)
#define STM32_HPRE_DIV4                 STM32_HPRE_FIELD(9U)
#define STM32_HPRE_DIV8                 STM32_HPRE_FIELD(10U)
#define STM32_HPRE_DIV16                STM32_HPRE_FIELD(11U)
#define STM32_HPRE_DIV64                STM32_HPRE_FIELD(12U)
#define STM32_HPRE_DIV128               STM32_HPRE_FIELD(13U)
#define STM32_HPRE_DIV256               STM32_HPRE_FIELD(14U)
#define STM32_HPRE_DIV512               STM32_HPRE_FIELD(15U)

#define STM32_PPRE1_FIELD(n)            ((n) << 8U)
#define STM32_PPRE1_DIV1                STM32_PPRE1_FIELD(0U)
#define STM32_PPRE1_DIV2                STM32_PPRE1_FIELD(4U)
#define STM32_PPRE1_DIV4                STM32_PPRE1_FIELD(5U)
#define STM32_PPRE1_DIV8                STM32_PPRE1_FIELD(6U)
#define STM32_PPRE1_DIV16               STM32_PPRE1_FIELD(7U)

#define STM32_PPRE2_FIELD(n)            ((n) << 11U)
#define STM32_PPRE2_DIV1                STM32_PPRE2_FIELD(0U)
#define STM32_PPRE2_DIV2                STM32_PPRE2_FIELD(4U)
#define STM32_PPRE2_DIV4                STM32_PPRE2_FIELD(5U)
#define STM32_PPRE2_DIV8                STM32_PPRE2_FIELD(6U)
#define STM32_PPRE2_DIV16               STM32_PPRE2_FIELD(7U)

#define STM32_MCOSEL_MASK               (15U << 24U)
#define STM32_MCOSEL_NOCLOCK            (0U << 24U)
#define STM32_MCOSEL_SYSCLK             (1U << 24U)
#define STM32_MCOSEL_HSI16              (3U << 24U)
#define STM32_MCOSEL_HSE                (4U << 24U)
#define STM32_MCOSEL_PLLRCLK            (5U << 24U)
#define STM32_MCOSEL_LSI                (6U << 24U)
#define STM32_MCOSEL_LSE                (7U << 24U)
#define STM32_MCOSEL_HSI48              (8U << 24U)

#define STM32_MCOPRE_FIELD(n)           ((n) << 28U)
#define STM32_MCOPRE_DIV1               STM32_MCOPRE_FIELD(0U)
#define STM32_MCOPRE_DIV2               STM32_MCOPRE_FIELD(1U)
#define STM32_MCOPRE_DIV4               STM32_MCOPRE_FIELD(2U)
#define STM32_MCOPRE_DIV8               STM32_MCOPRE_FIELD(3U)
#define STM32_MCOPRE_DIV16              STM32_MCOPRE_FIELD(4U)
/** @} */

/**
 * @name    RCC_PLLCFGR register bits definitions
 * @{
 */
#define STM32_PLLSRC_MASK               (3U << 0U)
#define STM32_PLLSRC_NOCLOCK            (0U << 0U)
#define STM32_PLLSRC_HSI16              (2U << 0U)
#define STM32_PLLSRC_HSE                (3U << 0U)
/** @} */

/**
 * @name    RCC_CCIPR register bits definitions
 * @{
 */
#define STM32_USART1SEL_MASK            (3U << 0U)
#define STM32_USART1SEL_PCLK2           (0U << 0U)
#define STM32_USART1SEL_SYSCLK          (1U << 0U)
#define STM32_USART1SEL_HSI16           (2U << 0U)
#define STM32_USART1SEL_LSE             (3U << 0U)

#define STM32_USART2SEL_MASK            (3U << 2U)
#define STM32_USART2SEL_PCLK1           (0U << 2U)
#define STM32_USART2SEL_SYSCLK          (1U << 2U)
#define STM32_USART2SEL_HSI16           (2U << 2U)
#define STM32_USART2SEL_LSE             (3U << 2U)

#define STM32_USART3SEL_MASK            (3U << 4U)
#define STM32_USART3SEL_PCLK1           (0U << 4U)
#define STM32_USART3SEL_SYSCLK          (1U << 4U)
#define STM32_USART3SEL_HSI16           (2U << 4U)
#define STM32_USART3SEL_LSE             (3U << 4U)

#define STM32_UART4SEL_MASK             (3U << 6U)
#define STM32_UART4SEL_PCLK1            (0U << 6U)
#define STM32_UART4SEL_SYSCLK           (1U << 6U)
#define STM32_UART4SEL_HSI16            (2U << 6U)
#define STM32_UART4SEL_LSE              (3U << 6U)

#define STM32_UART5SEL_MASK             (3U << 8U)
#define STM32_UART5SEL_PCLK1            (0U << 8U)
#define STM32_UART5SEL_SYSCLK           (1U << 8U)
#define STM32_UART5SEL_HSI16            (2U << 8U)
#define STM32_UART5SEL_LSE              (3U << 8U)

#define STM32_LPUART1SEL_MASK           (3U << 10U)
#define STM32_LPUART1SEL_PCLK1          (0U << 10U)
#define STM32_LPUART1SEL_SYSCLK         (1U << 10U)
#define STM32_LPUART1SEL_HSI16          (2U << 10U)
#define STM32_LPUART1SEL_LSE            (3U << 10U)

#define STM32_I2C1SEL_MASK              (3U << 12U)
#define STM32_I2C1SEL_PCLK1             (0U << 12U)
#define STM32_I2C1SEL_SYSCLK            (1U << 12U)
#define STM32_I2C1SEL_HSI16             (2U << 12U)

#define STM32_I2C2SEL_MASK              (3U << 14U)
#define STM32_I2C2SEL_PCLK1             (0U << 14U)
#define STM32_I2C2SEL_SYSCLK            (1U << 14U)
#define STM32_I2C2SEL_HSI16             (2U << 14U)

#define STM32_I2C3SEL_MASK              (3U << 16U)
#define STM32_I2C3SEL_PCLK1             (0U << 16U)
#define STM32_I2C3SEL_SYSCLK            (1U << 16U)
#define STM32_I2C3SEL_HSI16             (2U << 16U)

#define STM32_LPTIM1SEL_MASK            (3U << 18U)
#define STM32_LPTIM1SEL_PCLK1           (0U << 18U)
#define STM32_LPTIM1SEL_LSI             (1U << 18U)
#define STM32_LPTIM1SEL_HSI16           (2U << 18U)
#define STM32_LPTIM1SEL_LSE             (3U << 18U)

#define STM32_SAI1SEL_MASK              (3U << 20U)
#define STM32_SAI1SEL_SYSCLK            (0U << 20U)
#define STM32_SAI1SEL_PLLQCLK           (1U << 20U)
#define STM32_SAI1SEL_CKIN              (2U << 20U)
#define STM32_SAI1SEL_HSI16             (3U << 20U)

#define STM32_I2S23SEL_MASK             (3U << 22U)
#define STM32_I2S23SEL_SYSCLK           (0U << 22U)
#define STM32_I2S23SEL_PLLQCLK          (1U << 22U)
#define STM32_I2S23SEL_CKIN             (2U << 22U)
#define STM32_I2S23SEL_HSI16            (3U << 22U)

#define STM32_FDCANSEL_MASK             (3U << 24U)
#define STM32_FDCANSEL_HSE              (0U << 24U)
#define STM32_FDCANSEL_PLLQCLK          (1U << 24U)
#define STM32_FDCANSEL_PCLK1            (2U << 24U)

#define STM32_CLK48SEL_MASK             (3U << 26U)
#define STM32_CLK48SEL_HSI48            (0U << 26U)
#define STM32_CLK48SEL_PLLQCLK          (2U << 26U)

#define STM32_ADC12SEL_MASK             (3U << 28U)
#define STM32_ADC12SEL_NOCLK            (0U << 28U)
#define STM32_ADC12SEL_PLLPCLK          (1U << 28U)
#define STM32_ADC12SEL_SYSCLK           (2U << 28U)

#define STM32_ADC345SEL_MASK            (3U << 30U)
#define STM32_ADC345SEL_NOCLK           (0U << 30U)
#define STM32_ADC345SEL_PLLPCLK         (1U << 30U)
#define STM32_ADC345SEL_SYSCLK          (2U << 30U)
/** @} */

/**
 * @name    RCC_CCIPR2 register bits definitions
 * @{
 */
#define STM32_I2C4SEL_MASK              (3U << 0U)
#define STM32_I2C4SEL_PCLK1             (0U << 0U)
#define STM32_I2C4SEL_SYSCLK            (1U << 0U)
#define STM32_I2C4SEL_HSI16             (2U << 0U)

#define STM32_QSPISEL_MASK              (3U << 20U)
#define STM32_QSPISEL_SYSCLK            (0U << 20U)
#define STM32_QSPISEL_HSI16             (1U << 20U)
#define STM32_QSPISEL_PLLQCLK           (2U << 20U)
/** @} */

/**
 * @name    RCC_BDCR register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK               (3U << 8U)
#define STM32_RTCSEL_NOCLOCK            (0U << 8U)
#define STM32_RTCSEL_LSE                (1U << 8U)
#define STM32_RTCSEL_LSI                (2U << 8U)
#define STM32_RTCSEL_HSEDIV             (3U << 8U)

#define STM32_LSCOSEL_MASK              (3U << 24U)
#define STM32_LSCOSEL_NOCLOCK           (0U << 24U)
#define STM32_LSCOSEL_LSI               (1U << 24U)
#define STM32_LSCOSEL_LSE               (3U << 24U)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Static configuration options
 * @{
 */
#if !defined(STM32_NO_INIT) || defined(__DOXYGEN__)
#define STM32_NO_INIT                       FALSE
#endif

#if !defined(STM32_PWR_CR2) || defined(__DOXYGEN__)
#define STM32_PWR_CR2                       (PWR_CR2_PLS_LEV0)
#endif

#if !defined(STM32_PWR_CR3) || defined(__DOXYGEN__)
#define STM32_PWR_CR3                       (PWR_CR3_EIWF)
#endif

#if !defined(STM32_PWR_CR4) || defined(__DOXYGEN__)
#define STM32_PWR_CR4                       (0U)
#endif

#if !defined(STM32_PWR_PUCRA) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRA                     (0U)
#endif

#if !defined(STM32_PWR_PDCRA) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRA                     (0U)
#endif

#if !defined(STM32_PWR_PUCRB) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRB                     (0U)
#endif

#if !defined(STM32_PWR_PDCRB) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRB                     (0U)
#endif

#if !defined(STM32_PWR_PUCRC) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRC                     (0U)
#endif

#if !defined(STM32_PWR_PDCRC) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRC                     (0U)
#endif

#if !defined(STM32_PWR_PUCRD) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRD                     (0U)
#endif

#if !defined(STM32_PWR_PDCRD) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRD                     (0U)
#endif

#if !defined(STM32_PWR_PUCRE) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRE                     (0U)
#endif

#if !defined(STM32_PWR_PDCRE) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRE                     (0U)
#endif

#if !defined(STM32_PWR_PUCRF) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRF                     (0U)
#endif

#if !defined(STM32_PWR_PDCRF) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRF                     (0U)
#endif

#if !defined(STM32_PWR_PUCRG) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRG                     (0U)
#endif

#if !defined(STM32_PWR_PDCRG) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRG                     (0U)
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32G4xx_TEST_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G4xx_TEST_MCUCONF not defined"
#endif

#if defined(STM32G431xx) && !defined(STM32G431_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G431_MCUCONF not defined"

#elif defined(STM32G441xx) && !defined(STM32G441_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G441_MCUCONF not defined"

#elif defined(STM32G471xx) && !defined(STM32G471_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G471_MCUCONF not defined"

#elif defined(STM32G473xx) && !defined(STM32G473_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G473_MCUCONF not defined"

#elif defined(STM32G483xx) && !defined(STM32G483_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G483_MCUCONF not defined"

#elif defined(STM32G474xx) && !defined(STM32G474_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G474_MCUCONF not defined"

#elif defined(STM32G484xx) && !defined(STM32G484_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G484_MCUCONF not defined"

#elif defined(STM32GBK1CB) && !defined(STM32GBK1CB_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32GBK1CB_MCUCONF not defined"

#elif defined(STM32G491xx) && !defined(STM32G491_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32G491_MCUCONF not defined"
#endif

#include "clocktree.h"

#if STM32_CFG_PWR_BOOST && (STM32_CFG_PWR_VOS != STM32_VOS_RANGE1)
#error "STM32_CFG_PWR_BOOST requires STM32_CFG_PWR_VOS to be STM32_VOS_RANGE1"
#endif

/**
 * @name    Activation times in microseconds
 * @{
 */
#define STM32_RELAXED_TIMEOUT_FACTOR        5U
#define STM32_HSI_STARTUP_TIME              (4U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_OSCILLATORS_STARTUP_TIME      (2000U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_HSI48_STARTUP_TIME            (6U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_PLL_STARTUP_TIME              (40U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_SYSCLK_SWITCH_TIME            (50U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_REGULATORS_TRANSITION_TIME    (40U * STM32_RELAXED_TIMEOUT_FACTOR)
/** @} */

/**
 * @brief   Flash wait-state settings.
 */
#if (STM32_HCLK_FREQ <= STM32_FLASH_0WS_MAX) || defined(__DOXYGEN__)
#define STM32_FLASHBITS                     FLASH_ACR_LATENCY_0WS
#elif (STM32_HCLK_FREQ <= STM32_FLASH_1WS_MAX)
#define STM32_FLASHBITS                     FLASH_ACR_LATENCY_1WS
#elif (STM32_HCLK_FREQ <= STM32_FLASH_2WS_MAX)
#define STM32_FLASHBITS                     FLASH_ACR_LATENCY_2WS
#elif (STM32_HCLK_FREQ <= STM32_FLASH_3WS_MAX)
#define STM32_FLASHBITS                     FLASH_ACR_LATENCY_3WS
#elif (STM32_HCLK_FREQ <= STM32_FLASH_4WS_MAX)
#define STM32_FLASHBITS                     FLASH_ACR_LATENCY_4WS
#else
#define STM32_FLASHBITS                     FLASH_ACR_LATENCY_5WS
#endif

#if STM32_CFG_CLOCK_DYNAMIC == TRUE
#define HAL_LLD_USE_CLOCK_MANAGEMENT
#endif

/* Clock handlers.*/
#include "stm32_lse.inc"
#include "stm32_lsi.inc"

/*===========================================================================*/
/* Compatibility clock aliases.                                              */
/*===========================================================================*/

/*
 * LLD drivers still consume the historical STM32_*CLK symbols.  The clock
 * tree generator owns the actual model; this block is only the compatibility
 * boundary exported by hal_lld.h.
 */
#define STM32_CLOCK_DYNAMIC                 STM32_CFG_CLOCK_DYNAMIC

#define CLK_PLLPCLK                         CLK_PLLP
#define CLK_PLLQCLK                         CLK_PLLQ
#define CLK_PLLRCLK                         CLK_PLLR

#define STM32_HSI16CLK                      STM32_HSI16_FREQ
#define STM32_HSI48CLK                      STM32_HSI48_FREQ

#define STM32_PLLCLKIN                      STM32_PLLIN_FREQ
#define STM32_PLLVCO                        STM32_PLLVCO_FREQ
#define STM32_PLL_P_CLKOUT                  STM32_PLLP_FREQ
#define STM32_PLL_Q_CLKOUT                  STM32_PLLQ_FREQ
#define STM32_PLL_R_CLKOUT                  STM32_PLLR_FREQ

#define STM32_SYSCLK                        STM32_SYSCLK_FREQ
#define STM32_HCLK                          STM32_HCLK_FREQ
#define STM32_PCLK1                         STM32_PCLK1_FREQ
#define STM32_PCLK2                         STM32_PCLK2_FREQ
#define STM32_TIMP1CLK                      STM32_PCLK1TIM_FREQ
#define STM32_TIMP2CLK                      STM32_PCLK2TIM_FREQ
#define STM32_MCODIVCLK                     STM32_MCODIV_FREQ
#define STM32_MCOCLK                        STM32_MCO_FREQ
#define STM32_LSCOSEL                       STM32_LSCO_BITS
#define STM32_RTCCLK                        STM32_RTC_FREQ
#define STM32_RTCSEL                        STM32_RTC_BITS

#define STM32_USART1CLK                     STM32_USART1_FREQ
#define STM32_USART2CLK                     STM32_USART2_FREQ
#define STM32_USART3CLK                     STM32_USART3_FREQ
#define STM32_UART4CLK                      STM32_UART4_FREQ
#define STM32_UART5CLK                      STM32_UART5_FREQ
#define STM32_LPUART1CLK                    STM32_LPUART1_FREQ

#define STM32_I2C1CLK                       STM32_I2C1_FREQ
#define STM32_I2C2CLK                       STM32_I2C2_FREQ
#define STM32_I2C3CLK                       STM32_I2C3_FREQ
#define STM32_I2C4CLK                       STM32_I2C4_FREQ

#define STM32_LPTIM1CLK                     STM32_LPTIM1_FREQ
#define STM32_SAI1CLK                       STM32_SAI1_FREQ
#define STM32_I2S23CLK                      STM32_I2S23_FREQ
#define STM32_FDCANCLK                      STM32_FDCAN_FREQ
#define STM32_48CLK                         STM32_CLK48_FREQ
#define STM32_ADC12CLK                      STM32_ADC12_FREQ
#define STM32_ADC345CLK                     STM32_ADC345_FREQ
#define STM32_TIMCLK1                       STM32_PCLK1TIM_FREQ
#define STM32_TIMCLK2                       STM32_PCLK2TIM_FREQ
#define STM32_RNGCLK                        STM32_RNG_FREQ
#define STM32_USBCLK                        STM32_USB_FREQ

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a clock configuration structure.
 */
typedef struct {
  uint32_t          pwr_cr1;
  uint32_t          pwr_cr2;
  uint32_t          pwr_cr5;
  uint32_t          rcc_cr;
  uint32_t          rcc_cfgr;
  uint32_t          rcc_pllcfgr;
  uint32_t          flash_acr;
  uint32_t          rcc_crrcr;
} halclkcfg_t;

/**
 * @brief   Type of a timeout counter.
 */
typedef uint32_t halcnt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Real time counter frequency exported to the safety module.
 */
#define HAL_LLD_GET_CNT_FREQUENCY()         SystemCoreClock

/**
 * @brief   Real time counter value exported to the safety module.
 */
#define HAL_LLD_GET_CNT_VALUE()             (DWT->CYCCNT)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#include "nvic.h"
#include "cache.h"
#include "mpu_v7m.h"
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
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
