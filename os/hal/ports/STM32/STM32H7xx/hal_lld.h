/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    STM32H7xx/hal_lld.h
 * @brief   STM32H7xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_LSEDRV.
 *          - STM32_LSE_BYPASS (optionally).
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          - STM32_VDD (as hundredths of Volt).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32H743xx, STM32H753xx very high-performance MCUs.
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
 * @name    Platform identification macros
 * @{
 */
#if defined(STM32H743xx) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32H743 Very High Performance with DSP and FPU"

#elif defined(STM32H753xx)
#define PLATFORM_NAME           "STM32H753 Very High Performance with DSP and FPU"

#else
#error "STM32H7xx device not specified"
#endif
/** @} */

/**
 * @name    Sub-family identifier
 */
#if !defined(STM32H7XX) || defined(__DOXYGEN__)
#define STM32H7XX
#endif
/** @} */

/**
 * @name    Absolute Maximum Ratings
 * @{
 */
/**
 * @brief   Absolute maximum system clock.
 */
#define STM32_SYSCLK_MAX        400000000

/**
 * @brief   Absolute maximum HCLK clock.
 */
#define STM32_HCLK_MAX          200000000

/**
 * @brief   Maximum HSE clock frequency.
 */
#define STM32_HSECLK_MAX        48000000

/**
 * @brief   Maximum HSE clock frequency using an external source.
 */
#define STM32_HSECLK_BYP_MAX    50000000

/**
 * @brief   Minimum HSE clock frequency.
 */
#define STM32_HSECLK_MIN        4000000

/**
 * @brief   Minimum HSE clock frequency.
 */
#define STM32_HSECLK_BYP_MIN    4000000

/**
 * @brief   Maximum LSE clock frequency.
 */
#define STM32_LSECLK_MAX        32768

/**
 * @brief   Maximum LSE clock frequency.
 */
#define STM32_LSECLK_BYP_MAX    1000000

/**
 * @brief   Minimum LSE clock frequency.
 */
#define STM32_LSECLK_MIN        32768

/**
 * @brief   Maximum PLLs input clock frequency using VCOL.
 */
#define STM32_PLLINL_MAX        2000000

/**
 * @brief   Minimum PLLs input clock frequency using VCOL.
 */
#define STM32_PLLINL_MIN        1000000

/**
 * @brief   Maximum PLLs input clock frequency using VCOH.
 */
#define STM32_PLLINH_MAX        16000000

/**
 * @brief   Minimum PLLs input clock frequency using VCOH.
 */
#define STM32_PLLINH_MIN        2000000

/**
 * @brief   Maximum PLLs VCOL clock frequency.
 */
#define STM32_PLLVCOL_MAX       420000000

/**
 * @brief   Minimum PLLs VCOL clock frequency.
 */
#define STM32_PLLVCOL_MIN       150000000

/**
 * @brief   Maximum PLLs VCOH clock frequency.
 */
#define STM32_PLLVCOH_MAX       836000000

/**
 * @brief   Minimum PLLs VCOH clock frequency.
 */
#define STM32_PLLVCOH_MIN       192000000

/**
 * @brief   Maximum PLL output clock frequency.
 */
#define STM32_PLLOUT_MAX        400000000

/**
 * @brief   Minimum PLL output clock frequency.
 */
#define STM32_PLLOUT_MIN        1500000

/**
 * @brief   Maximum APB1 clock frequency.
 */
#define STM32_PCLK1_MAX         (STM32_HCLK_MAX / 2)

/**
 * @brief   Maximum APB2 clock frequency.
 */
#define STM32_PCLK2_MAX         (STM32_HCLK_MAX / 2)

/**
 * @brief   Maximum APB3 clock frequency.
 */
#define STM32_PCLK3_MAX         (STM32_HCLK_MAX / 2)

/**
 * @brief   Maximum APB4 clock frequency.
 */
#define STM32_PCLK4_MAX         (STM32_HCLK_MAX / 2)

/**
 * @brief   Maximum SPI1, SPI2 and SPI3 max clock frequency.
 */
#define STM32_SPI123_MAX        133000000

/**
 * @brief   Maximum SPI4, SPI5 and SPI6 max clock frequency.
 */
#define STM32_SPI456_MAX        100000000
/** @} */

/**
 * @name    Internal clock sources
 * @{
 */
#define STM32_HSICLK            64000000    /**< High speed internal clock. */
#define STM32_LSICLK            32000       /**< Low speed internal clock.  */
/** @} */

/**
 * @name    Register helpers not found in ST headers
 * @{
 */
#define RCC_CR_HSIDIV_VALUE(n)          ((n) << 3U)

#define RCC_CFGR_SW_VALUE(n)            ((n) << 0U)
#define RCC_CFGR_RTCPRE_VALUE(n)        ((n) << 8U)
#define RCC_CFGR_MCO1PRE_VALUE(n)       ((n) << 18U)
#define RCC_CFGR_MCO1_VALUE(n)          ((n) << 22U)
#define RCC_CFGR_MCO2PRE_VALUE(n)       ((n) << 25U)
#define RCC_CFGR_MCO2_VALUE(n)          ((n) << 29U)

#define RCC_D1CFGR_D1CPRE_VALUE(n)      ((n) << RCC_D1CFGR_D1CPRE_Pos)
#define RCC_D1CFGR_D1HPRE_VALUE(n)      ((n) << RCC_D1CFGR_D1HPRE_Pos)
#define RCC_D1CFGR_D1PPRE3_VALUE(n)     ((n) << RCC_D1CFGR_D1PPRE_Pos)

#define RCC_D2CFGR_D2PPRE1_VALUE(n)     ((n) << RCC_D2CFGR_D2PPRE1_Pos)
#define RCC_D2CFGR_D2PPRE2_VALUE(n)     ((n) << RCC_D2CFGR_D2PPRE2_Pos)

#define RCC_D3CFGR_D3PPRE4_VALUE(n)     ((n) << RCC_D3CFGR_D3PPRE_Pos)

#define RCC_PLLCKSELR_DIVM1_VALUE(n)    ((n) << RCC_PLLCKSELR_DIVM1_Pos)
#define RCC_PLLCKSELR_DIVM2_VALUE(n)    ((n) << RCC_PLLCKSELR_DIVM2_Pos)
#define RCC_PLLCKSELR_DIVM3_VALUE(n)    ((n) << RCC_PLLCKSELR_DIVM3_Pos)

#define RCC_PLL1DIVR_DIVN1_VALUE(n)     ((n) << RCC_PLL1DIVR_N1)
#define RCC_PLL1DIVR_DIVP1_VALUE(n)     ((n) << RCC_PLL1DIVR_P1)
#define RCC_PLL1DIVR_DIVQ1_VALUE(n)     ((n) << RCC_PLL1DIVR_Q1)
#define RCC_PLL1DIVR_DIVR1_VALUE(n)     ((n) << RCC_PLL1DIVR_R1)

#define RCC_PLL1FRACR_FRACN1_VALUE(n)   ((n) << RCC_PLL1FRACR_FRACN1_Pos)

#define RCC_PLL2DIVR_DIVN2_VALUE(n)     ((n) << RCC_PLL2DIVR_N2)
#define RCC_PLL2DIVR_DIVP2_VALUE(n)     ((n) << RCC_PLL2DIVR_P2)
#define RCC_PLL2DIVR_DIVQ2_VALUE(n)     ((n) << RCC_PLL2DIVR_Q2)
#define RCC_PLL2DIVR_DIVR2_VALUE(n)     ((n) << RCC_PLL2DIVR_R2)

#define RCC_PLL2FRACR_FRACN2_VALUE(n)   ((n) << RCC_PLL2FRACR_FRACN2_Pos)

#define RCC_PLL3DIVR_DIVN3_VALUE(n)     ((n) << RCC_PLL3DIVR_N3)
#define RCC_PLL3DIVR_DIVP3_VALUE(n)     ((n) << RCC_PLL3DIVR_P3)
#define RCC_PLL3DIVR_DIVQ3_VALUE(n)     ((n) << RCC_PLL3DIVR_Q3)
#define RCC_PLL3DIVR_DIVR3_VALUE(n)     ((n) << RCC_PLL3DIVR_R3)

#define RCC_PLL3FRACR_FRACN3_VALUE(n)   ((n) << RCC_PLL3FRACR_FRACN3_Pos)

#define RCC_D1CCIPR_CKPERSEL_VALUE(n)   ((n) << RCC_D1CCIPR_CKPERSEL_Pos)

#define RCC_BDCR_RTCSEL_VALUE(n)        ((n) << RCC_BDCR_RTCSEL_Pos)
/** @} */

/**
 * @name    Configuration switches to be used in @p mcuconf.h
 * @{
 */
#define STM32_SW_HSI_CK                 RCC_CFGR_SW_VALUE(0U)
#define STM32_SW_CSI_CK                 RCC_CFGR_SW_VALUE(1U)
#define STM32_SW_HSE_CK                 RCC_CFGR_SW_VALUE(2U)
#define STM32_SW_PLL1_P_CK              RCC_CFGR_SW_VALUE(3U)

#define STM32_D1CPRE_DIV1               RCC_D1CFGR_D1CPRE_VALUE(0U)
#define STM32_D1CPRE_DIV2               RCC_D1CFGR_D1CPRE_VALUE(8U)
#define STM32_D1CPRE_DIV4               RCC_D1CFGR_D1CPRE_VALUE(9U)
#define STM32_D1CPRE_DIV8               RCC_D1CFGR_D1CPRE_VALUE(10U)
#define STM32_D1CPRE_DIV16              RCC_D1CFGR_D1CPRE_VALUE(11U)
#define STM32_D1CPRE_DIV64              RCC_D1CFGR_D1CPRE_VALUE(12U)
#define STM32_D1CPRE_DIV128             RCC_D1CFGR_D1CPRE_VALUE(13U)
#define STM32_D1CPRE_DIV256             RCC_D1CFGR_D1CPRE_VALUE(14U)
#define STM32_D1CPRE_DIV512             RCC_D1CFGR_D1CPRE_VALUE(15U)

#define STM32_D1HPRE_DIV1               RCC_D1CFGR_D1HPRE_VALUE(0U)
#define STM32_D1HPRE_DIV2               RCC_D1CFGR_D1HPRE_VALUE(8U)
#define STM32_D1HPRE_DIV4               RCC_D1CFGR_D1HPRE_VALUE(9U)
#define STM32_D1HPRE_DIV8               RCC_D1CFGR_D1HPRE_VALUE(10U)
#define STM32_D1HPRE_DIV16              RCC_D1CFGR_D1HPRE_VALUE(11U)
#define STM32_D1HPRE_DIV64              RCC_D1CFGR_D1HPRE_VALUE(12U)
#define STM32_D1HPRE_DIV128             RCC_D1CFGR_D1HPRE_VALUE(13U)
#define STM32_D1HPRE_DIV256             RCC_D1CFGR_D1HPRE_VALUE(14U)
#define STM32_D1HPRE_DIV512             RCC_D1CFGR_D1HPRE_VALUE(15U)

#define STM32_D1PPRE3_DIV1              RCC_D1CFGR_D1PPRE3_VALUE(0U)
#define STM32_D1PPRE3_DIV2              RCC_D1CFGR_D1PPRE3_VALUE(4U)
#define STM32_D1PPRE3_DIV4              RCC_D1CFGR_D1PPRE3_VALUE(5U)
#define STM32_D1PPRE3_DIV8              RCC_D1CFGR_D1PPRE3_VALUE(6U)
#define STM32_D1PPRE3_DIV16             RCC_D1CFGR_D1PPRE3_VALUE(7U)

#define STM32_D2PPRE1_DIV1              RCC_D2CFGR_D2PPRE1_VALUE(0U)
#define STM32_D2PPRE1_DIV2              RCC_D2CFGR_D2PPRE1_VALUE(4U)
#define STM32_D2PPRE1_DIV4              RCC_D2CFGR_D2PPRE1_VALUE(5U)
#define STM32_D2PPRE1_DIV8              RCC_D2CFGR_D2PPRE1_VALUE(6U)
#define STM32_D2PPRE1_DIV16             RCC_D2CFGR_D2PPRE1_VALUE(7U)

#define STM32_D2PPRE2_DIV1              RCC_D2CFGR_D2PPRE2_VALUE(0U)
#define STM32_D2PPRE2_DIV2              RCC_D2CFGR_D2PPRE2_VALUE(4U)
#define STM32_D2PPRE2_DIV4              RCC_D2CFGR_D2PPRE2_VALUE(5U)
#define STM32_D2PPRE2_DIV8              RCC_D2CFGR_D2PPRE2_VALUE(6U)
#define STM32_D2PPRE2_DIV16             RCC_D2CFGR_D2PPRE2_VALUE(7U)

#define STM32_D3PPRE4_DIV1              RCC_D1CFGR_D3PPRE4_VALUE(0U)
#define STM32_D3PPRE4_DIV2              RCC_D1CFGR_D3PPRE4_VALUE(4U)
#define STM32_D3PPRE4_DIV4              RCC_D1CFGR_D3PPRE4_VALUE(5U)
#define STM32_D3PPRE4_DIV8              RCC_D1CFGR_D3PPRE4_VALUE(6U)
#define STM32_D3PPRE4_DIV16             RCC_D1CFGR_D3PPRE4_VALUE(7U)

#define STM32_HSIDIV_DIV1               RCC_CR_HSIDIV_VALUE(0U)
#define STM32_HSIDIV_DIV2               RCC_CR_HSIDIV_VALUE(1U)
#define STM32_HSIDIV_DIV4               RCC_CR_HSIDIV_VALUE(2U)
#define STM32_HSIDIV_DIV8               RCC_CR_HSIDIV_VALUE(3U)

#define STM32_MCO1SEL_HSI_CK            RCC_CFGR_MCO1_VALUE(0U)
#define STM32_MCO1SEL_LSE_CK            RCC_CFGR_MCO1_VALUE(1U)
#define STM32_MCO1SEL_HSE_CK            RCC_CFGR_MCO1_VALUE(2U)
#define STM32_MCO1SEL_PLL1_Q_CK         RCC_CFGR_MCO1_VALUE(3U)
#define STM32_MCO1SEL_HSI48_CK          RCC_CFGR_MCO1_VALUE(4U)

#define STM32_MCO2SEL_SYS_CK            RCC_CFGR_MCO2_VALUE(0U)
#define STM32_MCO2SEL_PLL2_Q_CK         RCC_CFGR_MCO2_VALUE(1U)
#define STM32_MCO2SEL_HSE_CK            RCC_CFGR_MCO2_VALUE(2U)
#define STM32_MCO2SEL_PLL1_P_CK         RCC_CFGR_MCO2_VALUE(3U)
#define STM32_MCO2SEL_CSI_CK            RCC_CFGR_MCO2_VALUE(4U)
#define STM32_MCO2SEL_LSI_CK            RCC_CFGR_MCO2_VALUE(5U)

#define STM32_RTCSEL_NOCLK              RCC_BDCR_RTCSEL_VALUE(0U)
#define STM32_RTCSEL_LSE_CK             RCC_BDCR_RTCSEL_VALUE(1U)
#define STM32_RTCSEL_LSI_CK             RCC_BDCR_RTCSEL_VALUE(2U)
#define STM32_RTCSEL_HSE_1M_CK          RCC_BDCR_RTCSEL_VALUE(3U)

#define STM32_CKPERSEL_HSI_KER_CLK      RCC_D1CCIPR_CKPERSEL_VALUE(0U)
#define STM32_CKPERSEL_CSI_KER_CLK      RCC_D1CCIPR_CKPERSEL_VALUE(1U)
#define STM32_CKPERSEL_HSE_CLK          RCC_D1CCIPR_CKPERSEL_VALUE(2U)
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
 * @brief   HCLK value assumed if @p STM32_NO_INIT is enabled.
 */
#if !defined(STM32_HCLK_ENFORCED_VALUE) || defined(__DOXYGEN__)
#define STM32_HCLK_ENFORCED_VALUE           STM32_HSICLK
#endif

/**
 * @brief   PWR CR1 initializer.
 */
#if !defined(STM32_PWR_CR1) || defined(__DOXYGEN__)
#define STM32_PWR_CR1                       (PWR_CR1_SVOS_1 | PWR_CR1_SVOS_0)
#endif

/**
 * @brief   PWR CR2 initializer.
 */
#if !defined(STM32_PWR_CR2) || defined(__DOXYGEN__)
#define STM32_PWR_CR2                       (PWR_CR2_BREN)
#endif

/**
 * @brief   PWR CR3 initializer.
 */
#if !defined(STM32_PWR_CR3) || defined(__DOXYGEN__)
#define STM32_PWR_CR3                       (PWR_CR3_SCUEN | PWR_CR3_LDOEN)
#endif

/**
 * @brief   PWR CPUCR initializer.
 */
#if !defined(STM32_PWR_CPUCR) || defined(__DOXYGEN__)
#define STM32_PWR_CPUCR                     0
#endif

/**
 * @brief   PWR D3CR initializer.
 */
#if !defined(STM32_PWR_D3CR) || defined(__DOXYGEN__)
#define STM32_PWR_D3CR                      (PWR_D3CR_VOS_0)
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM32_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI_ENABLED                   TRUE
#endif

/**
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM32_LSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSI_ENABLED                   FALSE
#endif

/**
 * @brief   Enables or disables the LSI clock source.
 */
#if !defined(STM32_CSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_CSI_ENABLED                   FALSE
#endif

/**
 * @brief   Enables or disables the HSI48 clock source.
 */
#if !defined(STM32_HSI48_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI48_ENABLED                 TRUE
#endif

/**
 * @brief   Enables or disables the HSE clock source.
 */
#if !defined(STM32_HSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSE_ENABLED                   TRUE
#endif

/**
 * @brief   Enables or disables the LSE clock source.
 */
#if !defined(STM32_LSE_ENABLED) || defined(__DOXYGEN__)
#define STM32_LSE_ENABLED                   TRUE
#endif

/**
 * @brief   HSI divider.
 */
#if !defined(STM32_HSIDIV) || defined(__DOXYGEN__)
#define STM32_HSIDIV                        STM32_HSIDIV_DIV1
#endif

/**
 * @brief   RTC HSE prescaler value.
 * @note    The allowed values are 2..63.
 */
#if !defined(STM32_RTCPRE_VALUE) || defined(__DOXYGEN__)
#define STM32_RTCPRE_VALUE                  8
#endif

/**
 * @brief   Clock source for all PLLs.
 */
#if !defined(STM32_PLLSRC) || defined(__DOXYGEN__)
#define STM32_PLLSRC                        STM32_PLLSRC_HSE
#endif

/**
 * @brief   Enables or disables the PLL1.
 */
#if !defined(STM32_PLL1_ENABLED) || defined(__DOXYGEN__)
#define STM32_PLL1_ENABLED                  TRUE
#endif

/**
 * @brief   PLL1 DIVM divider.
 * @note    The allowed values are 1..63.
 */
#if !defined(STM32_PLL1_DIVM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1_DIVM_VALUE               4
#endif

/**
 * @brief   PLL1 DIVN multiplier.
 * @note    The allowed values are 4..512.
 */
#if !defined(STM32_PLL1_DIVN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1_DIVN_VALUE               400
#endif

/**
 * @brief   PLL1 FRACN multiplier, zero if no fractional part.
 * @note    The allowed values are 0..8191.
 */
#if !defined(STM32_PLL1_FRACN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1_FRACN_VALUE              0
#endif

/**
 * @brief   PLL1 DIVP divider.
 * @note    The allowed values are 2..128, odd values not allowed.
 */
#if !defined(STM32_PLL1_DIVP_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1_DIVP_VALUE               2
#endif

/**
 * @brief   PLL1 DIVQ divider.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL1_DIVQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1_DIVQ_VALUE               8
#endif

/**
 * @brief   PLL1 DIVR divider.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL1_DIVR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1_DIVR_VALUE               8
#endif

/**
 * @brief   Enables or disables the PLL2.
 */
#if !defined(STM32_PLL2_ENABLED) || defined(__DOXYGEN__)
#define STM32_PLL2_ENABLED                  TRUE
#endif

/**
 * @brief   PLL2 DIVM divider.
 * @note    The allowed values are 1..63.
 */
#if !defined(STM32_PLL2_DIVM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2_DIVM_VALUE               4
#endif

/**
 * @brief   PLL2 DIVN multiplier.
 * @note    The allowed values are 4..512.
 */
#if !defined(STM32_PLL2_DIVN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2_DIVN_VALUE               400
#endif

/**
 * @brief   PLL2 FRACN multiplier, zero if no fractional part.
 * @note    The allowed values are 0..8191.
 */
#if !defined(STM32_PLL2_FRACN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2_FRACN_VALUE              0
#endif

/**
 * @brief   PLL2 DIVP divider.
 * @note    The allowed values are 2..128, odd values not allowed.
 */
#if !defined(STM32_PLL2_DIVP_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2_DIVP_VALUE               2
#endif

/**
 * @brief   PLL2 DIVQ divider.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL2_DIVQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2_DIVQ_VALUE               8
#endif

/**
 * @brief   PLL2 DIVR divider.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL2_DIVR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2_DIVR_VALUE               8
#endif

/**
 * @brief   Enables or disables the PLL3.
 */
#if !defined(STM32_PLL3_ENABLED) || defined(__DOXYGEN__)
#define STM32_PLL3_ENABLED                  TRUE
#endif

/**
 * @brief   PLL3 DIVM divider.
 * @note    The allowed values are 1..63.
 */
#if !defined(STM32_PLL3_DIVM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3_DIVM_VALUE               4
#endif

/**
 * @brief   PLL3 DIVN multiplier.
 * @note    The allowed values are 4..512.
 */
#if !defined(STM32_PLL3_DIVN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3_DIVN_VALUE               400
#endif

/**
 * @brief   PLL3 FRACN multiplier, zero if no fractional part.
 * @note    The allowed values are 0..8191.
 */
#if !defined(STM32_PLL3_FRACN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3_FRACN_VALUE              0
#endif

/**
 * @brief   PLL3 DIVP divider.
 * @note    The allowed values are 2..128, odd values not allowed.
 */
#if !defined(STM32_PLL3_DIVP_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3_DIVP_VALUE               2
#endif

/**
 * @brief   PLL3 DIVQ divider.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL3_DIVQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3_DIVQ_VALUE               8
#endif

/**
 * @brief   PLL3 DIVR divider.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL3_DIVR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3_DIVR_VALUE               8
#endif

/**
 * @brief   Peripherals clock selector.
 */
#if !defined(STM32_CKPERSEL) || defined(__DOXYGEN__)
#define STM32_CKPERSEL                      STM32_CKPERSEL_HSE_CLK
#endif

/**
 * @brief   MCO1 clock selector.
 */
#if !defined(STM32_MCO1SEL) || defined(__DOXYGEN__)
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI_CK
#endif

/**
 * @brief   MCO1 clock prescaler.
 */
#if !defined(STM32_MCO1PRE) || defined(__DOXYGEN__)
#define STM32_MCO1PRE                       4
#endif

/**
 * @brief   MCO2 clock selector.
 */
#if !defined(STM32_MCO2SEL) || defined(__DOXYGEN__)
#define STM32_MCO2SEL                       STM32_MCO2SEL_SYS_CK
#endif

/**
 * @brief   MCO2 clock prescaler.
 */
#if !defined(STM32_MCO2PRE) || defined(__DOXYGEN__)
#define STM32_MCO2PRE                       4
#endif

/**
 * @brief   Main clock source selection.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_SW) || defined(__DOXYGEN__)
#define STM32_SW                            STM32_SW_PLL1_P_CK
#endif

/**
 * @brief   RTC clock selector.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_RTCSEL) || defined(__DOXYGEN__)
#define STM32_RTCSEL                        STM32_RTCSEL_LSE
#endif

/**
 * @brief   Clock domain 1 core bus prescaler.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_D1CPRE) || defined(__DOXYGEN__)
#define STM32_D1CPRE                        STM32_D1CPRE_DIV1
#endif

/**
 * @brief   Clock domain 1 HPRE prescaler.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_D1HPRE) || defined(__DOXYGEN__)
#define STM32_D1HPRE                        STM32_D1HPRE_DIV2
#endif

/**
 * @brief   Clock domain 1 peripherals bus prescaler.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_D1PPRE) || defined(__DOXYGEN__)
#define STM32_D1PPRE3                       STM32_D1PPRE3_DIV2
#endif

/**
 * @brief   Clock domain 2 peripherals bus 1 prescaler.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_D2PPRE1) || defined(__DOXYGEN__)
#define STM32_D2PPRE1                       STM32_D2PPRE1_DIV2
#endif

/**
 * @brief   Clock domain 2 peripherals bus 2 prescaler.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_D2PPRE2) || defined(__DOXYGEN__)
#define STM32_D2PPRE2                       STM32_D2PPRE2_DIV2
#endif

/**
 * @brief   Clock domain 3 peripherals bus prescaler.
 * @note    This setting can be modified at runtime.
 */
#if !defined(STM32_D3PPRE4) || defined(__DOXYGEN__)
#define STM32_D3PPRE4                       STM32_D3PPRE4_DIV2
#endif
















/**
 * @brief   I2S clock source.
 */
#if !defined(STM32_I2SSRC) || defined(__DOXYGEN__)
#define STM32_I2SSRC                        STM32_I2SSRC_PLLI2S
#endif

/**
 * @brief   PLLI2SN multiplier value.
 * @note    The allowed values are 49..432.
 */
#if !defined(STM32_PLLI2SN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLI2SN_VALUE                 192
#endif

/**
 * @brief   PLLI2SP divider value.
 * @note    The allowed values are 2, 4, 6 and 8.
 */
#if !defined(STM32_PLLI2SP_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLI2SP_VALUE                 4
#endif

/**
 * @brief   PLLI2SQ divider value.
 * @note    The allowed values are 2..15.
 */
#if !defined(STM32_PLLI2SQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLI2SQ_VALUE                 4
#endif

/**
 * @brief   PLLI2SDIVQ divider value (SAI clock divider).
 */
#if !defined(STM32_PLLI2SDIVQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLI2SDIVQ_VALUE              2
#endif

/**
 * @brief   PLLI2SR divider value.
 * @note    The allowed values are 2..7.
 */
#if !defined(STM32_PLLI2SR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLI2SR_VALUE                 4
#endif

/**
 * @brief   PLLSAIN multiplier value.
 * @note    The allowed values are 49..432.
 */
#if !defined(STM32_PLLSAIN_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLSAIN_VALUE                 192
#endif

/**
 * @brief   PLLSAIP divider value.
 * @note    The allowed values are 2, 4, 6 and 8.
 */
#if !defined(STM32_PLLSAIP_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLSAIP_VALUE                 4
#endif

/**
 * @brief   PLLSAIQ divider value.
 * @note    The allowed values are 2..15.
 */
#if !defined(STM32_PLLSAIQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLSAIQ_VALUE                 4
#endif

/**
 * @brief   PLLSAIR divider value.
 * @note    The allowed values are 2..7.
 */
#if !defined(STM32_PLLSAIR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLSAIR_VALUE                 4
#endif

/**
 * @brief   PLLSAIDIVQ divider value (SAI clock divider).
 */
#if !defined(STM32_PLLSAIDIVQ_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLSAIDIVQ_VALUE              2
#endif

/**
 * @brief   PLLSAIDIVR divider value (LCD clock divider).
 */
#if !defined(STM32_PLLSAIDIVR_VALUE) || defined(__DOXYGEN__)
#define STM32_PLLSAIDIVR_VALUE              2
#endif

/**
 * @brief   SAI1SEL value (SAI1 clock source).
 */
#if !defined(STM32_SAI1SEL) || defined(__DOXYGEN__)
#define STM32_SAI1SEL                       STM32_SAI1SEL_OFF
#endif

/**
 * @brief   SAI2SEL value (SAI2 clock source).
 */
#if !defined(STM32_SAI2SEL) || defined(__DOXYGEN__)
#define STM32_SAI2SEL                       STM32_SAI2SEL_OFF
#endif

/**
 * @brief   LCD-TFT clock enable switch.
 */
#if !defined(STM32_LCDTFT_REQUIRED) || defined(__DOXYGEN__)
#define STM32_LCDTFT_REQUIRED               FALSE
#endif

/**
 * @brief   USART1 clock source.
 */
#if !defined(STM32_USART1SEL) || defined(__DOXYGEN__)
#define STM32_USART1SEL                     STM32_USART1SEL_PCLK2
#endif

/**
 * @brief   USART2 clock source.
 */
#if !defined(STM32_USART2SEL) || defined(__DOXYGEN__)
#define STM32_USART2SEL                     STM32_USART2SEL_PCLK1
#endif

/**
 * @brief   USART3 clock source.
 */
#if !defined(STM32_USART3SEL) || defined(__DOXYGEN__)
#define STM32_USART3SEL                     STM32_USART3SEL_PCLK1
#endif

/**
 * @brief   UART4 clock source.
 */
#if !defined(STM32_UART4SEL) || defined(__DOXYGEN__)
#define STM32_UART4SEL                      STM32_UART4SEL_PCLK1
#endif

/**
 * @brief   UART5 clock source.
 */
#if !defined(STM32_UART5SEL) || defined(__DOXYGEN__)
#define STM32_UART5SEL                      STM32_UART5SEL_PCLK1
#endif

/**
 * @brief   USART6 clock source.
 */
#if !defined(STM32_USART6SEL) || defined(__DOXYGEN__)
#define STM32_USART6SEL                     STM32_USART6SEL_PCLK2
#endif

/**
 * @brief   UART7 clock source.
 */
#if !defined(STM32_UART7SEL) || defined(__DOXYGEN__)
#define STM32_UART7SEL                      STM32_UART7SEL_PCLK1
#endif

/**
 * @brief   UART8 clock source.
 */
#if !defined(STM32_UART8SEL) || defined(__DOXYGEN__)
#define STM32_UART8SEL                      STM32_UART8SEL_PCLK1
#endif

/**
 * @brief   I2C1 clock source.
 */
#if !defined(STM32_I2C1SEL) || defined(__DOXYGEN__)
#define STM32_I2C1SEL                       STM32_I2C1SEL_PCLK1
#endif

/**
 * @brief   I2C2 clock source.
 */
#if !defined(STM32_I2C2SEL) || defined(__DOXYGEN__)
#define STM32_I2C2SEL                       STM32_I2C2SEL_PCLK1
#endif

/**
 * @brief   I2C3 clock source.
 */
#if !defined(STM32_I2C3SEL) || defined(__DOXYGEN__)
#define STM32_I2C3SEL                       STM32_I2C3SEL_PCLK1
#endif

/**
 * @brief   I2C4 clock source.
 */
#if !defined(STM32_I2C4SEL) || defined(__DOXYGEN__)
#define STM32_I2C4SEL                       STM32_I2C4SEL_PCLK1
#endif

/**
 * @brief   LPTIM1 clock source.
 */
#if !defined(STM32_LPTIM1SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK1
#endif

/**
 * @brief   CEC clock source.
 */
#if !defined(STM32_CECSEL) || defined(__DOXYGEN__)
#define STM32_CECSEL                        STM32_CECSEL_LSE
#endif

/**
 * @brief   PLL48CLK clock source.
 */
#if !defined(STM32_CK48MSEL) || defined(__DOXYGEN__)
#define STM32_CK48MSEL                      STM32_CK48MSEL_PLL
#endif

/**
 * @brief   SDMMC clock source.
 */
#if !defined(STM32_SDMMCSEL) || defined(__DOXYGEN__)
#define STM32_SDMMCSEL                      STM32_SDMMCSEL_PLL48CLK
#endif

/**
 * @brief   SRAM2 cache-ability.
 * @note    This setting uses the MPU region 7 if at @p TRUE.
 */
#if !defined(STM32_SRAM2_NOCACHE) || defined(__DOXYGEN__)
#define STM32_SRAM2_NOCACHE                 FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(STM32H7xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32H7xx_MCUCONF not defined"
#endif

/*
 * Board file checks.
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
#if !defined(STM32_VDD)
#error "STM32_VDD not defined in board.h"
#endif

/**
 * @brief   Maximum frequency thresholds and wait states for flash access.
 * @note    The values are valid for 2.7V to 3.6V supply range.
 */
#if ((STM32_VDD >= 270) && (STM32_VDD <= 360)) || defined(__DOXYGEN__)
#define STM32_0WS_THRESHOLD         30000000
#define STM32_1WS_THRESHOLD         60000000
#define STM32_2WS_THRESHOLD         90000000
#define STM32_3WS_THRESHOLD         120000000
#define STM32_4WS_THRESHOLD         150000000
#define STM32_5WS_THRESHOLD         180000000
#define STM32_6WS_THRESHOLD         210000000
#define STM32_7WS_THRESHOLD         0
#define STM32_8WS_THRESHOLD         0

#elif (STM32_VDD >= 240) && (STM32_VDD < 270)
#define STM32_0WS_THRESHOLD         24000000
#define STM32_1WS_THRESHOLD         48000000
#define STM32_2WS_THRESHOLD         72000000
#define STM32_3WS_THRESHOLD         96000000
#define STM32_4WS_THRESHOLD         120000000
#define STM32_5WS_THRESHOLD         144000000
#define STM32_6WS_THRESHOLD         168000000
#define STM32_7WS_THRESHOLD         192000000
#define STM32_8WS_THRESHOLD         0

#elif (STM32_VDD >= 210) && (STM32_VDD < 240)
#define STM32_0WS_THRESHOLD         22000000
#define STM32_1WS_THRESHOLD         44000000
#define STM32_2WS_THRESHOLD         66000000
#define STM32_3WS_THRESHOLD         88000000
#define STM32_4WS_THRESHOLD         110000000
#define STM32_5WS_THRESHOLD         132000000
#define STM32_6WS_THRESHOLD         154000000
#define STM32_7WS_THRESHOLD         176000000
#define STM32_8WS_THRESHOLD         198000000

#elif (STM32_VDD >= 180) && (STM32_VDD < 210)
#define STM32_0WS_THRESHOLD         20000000
#define STM32_1WS_THRESHOLD         40000000
#define STM32_2WS_THRESHOLD         60000000
#define STM32_3WS_THRESHOLD         80000000
#define STM32_4WS_THRESHOLD         100000000
#define STM32_5WS_THRESHOLD         120000000
#define STM32_6WS_THRESHOLD         140000000
#define STM32_7WS_THRESHOLD         160000000
#define STM32_8WS_THRESHOLD         0

#else
#error "invalid VDD voltage specified"
#endif

/*
 * HSI related checks.
 */
#if STM32_HSI_ENABLED
#else /* !STM32_HSI_ENABLED */

#if STM32_SW == STM32_SW_HSI
#error "HSI not enabled, required by STM32_SW"
#endif

#if (STM32_SW == STM32_SW_PLL) && (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_SW and STM32_PLLSRC"
#endif

#if (STM32_MCO1SEL == STM32_MCO1SEL_HSI) ||                                 \
    ((STM32_MCO1SEL == STM32_MCO1SEL_PLL) &&                                \
     (STM32_PLLSRC == STM32_PLLSRC_HSI))
#error "HSI not enabled, required by STM32_MCO1SEL"
#endif

#if (STM32_MCO2SEL == STM32_MCO2SEL_PLL) &&                                 \
    (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_MCO2SEL"
#endif

#if (STM32_I2SSRC == STM32_I2SSRC_PLLI2S) &&                                \
    (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_I2SSRC"
#endif

#if ((STM32_SAI1SEL == STM32_SAI1SEL_SAIPLL) ||                             \
     (STM32_SAI1SEL == STM32_SAI1SEL_I2SPLL)) &&                            \
    (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_SAI1SEL"
#endif

#if ((STM32_SAI2SEL == STM32_SAI2SEL_SAIPLL) ||                             \
     (STM32_SAI2SEL == STM32_SAI2SEL_I2SPLL)) &&                            \
    (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_SAI2SEL"
#endif

#if STM32_LCDTFT_REQUIRED &&                                                \
    (STM32_PLLSRC == STM32_PLLSRC_HSI)
#error "HSI not enabled, required by STM32_LCDTFT_REQUIRED"
#endif

#endif /* !STM32_HSI_ENABLED */

/*
 * HSE related checks.
 */
#if STM32_HSE_ENABLED

#if STM32_HSECLK == 0
#error "HSE frequency not defined"
#else /* STM32_HSECLK != 0 */
#if defined(STM32_HSE_BYPASS)
#if (STM32_HSECLK < STM32_HSECLK_MIN) || (STM32_HSECLK > STM32_HSECLK_BYP_MAX)
#error "STM32_HSECLK outside acceptable range (STM32_HSECLK_MIN...STM32_HSECLK_BYP_MAX)"
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

#if (STM32_MCO1SEL == STM32_MCO1SEL_HSE) ||                                 \
    ((STM32_MCO1SEL == STM32_MCO1SEL_PLL) &&                                \
     (STM32_PLLSRC == STM32_PLLSRC_HSE))
#error "HSE not enabled, required by STM32_MCO1SEL"
#endif

#if (STM32_MCO2SEL == STM32_MCO2SEL_HSE) ||                                 \
    ((STM32_MCO2SEL == STM32_MCO2SEL_PLL) &&                                \
     (STM32_PLLSRC == STM32_PLLSRC_HSE))
#error "HSE not enabled, required by STM32_MCO2SEL"
#endif

#if (STM32_I2SSRC == STM32_I2SSRC_PLLI2S) &&                                \
    (STM32_PLLSRC == STM32_PLLSRC_HSE)
#error "HSE not enabled, required by STM32_I2SSRC"
#endif

#if ((STM32_SAI1SEL == STM32_SAI1SEL_SAIPLL) |                              \
     (STM32_SAI1SEL == STM32_SAI1SEL_I2SPLL)) &&                            \
    (STM32_PLLSRC == STM32_PLLSRC_HSE)
#error "HSE not enabled, required by STM32_SAI1SEL"
#endif

#if ((STM32_SAI2SEL == STM32_SAI2SEL_SAIPLL) |                              \
     (STM32_SAI2SEL == STM32_SAI2SEL_I2SPLL)) &&                            \
    (STM32_PLLSRC == STM32_PLLSRC_HSE)
#error "HSE not enabled, required by STM32_SAI2SEL"
#endif

#if STM32_LCDTFT_REQUIRED &&                                                \
    (STM32_PLLSRC == STM32_PLLSRC_HSE)
#error "HSE not enabled, required by STM32_LCDTFT_REQUIRED"
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

#if !defined(STM32_LSEDRV)
#error "STM32_LSEDRV not defined"
#endif

#if (STM32_LSEDRV >> 3) > 3
#error "STM32_LSEDRV outside acceptable range ((0<<3)...(3<<3))"
#endif

#else /* !STM32_LSE_ENABLED */

#if STM32_RTCSEL == STM32_RTCSEL_LSE
#error "LSE not enabled, required by STM32_RTCSEL"
#endif

#if STM32_MCO1SEL == STM32_MCO1SEL_LSE
#error "LSE not enabled, required by STM32_MCO1SEL"
#endif

#endif /* !STM32_LSE_ENABLED */

/**
 * @brief   STM32_PLLM field.
 */
#if ((STM32_PLLM_VALUE >= 2) && (STM32_PLLM_VALUE <= 63)) ||                \
    defined(__DOXYGEN__)
#define STM32_PLLM                  (STM32_PLLM_VALUE << 0)
#else
#error "invalid STM32_PLLM_VALUE value specified"
#endif

/**
 * @brief   PLLs input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
#define STM32_PLLCLKIN              (STM32_HSECLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_HSI
#define STM32_PLLCLKIN              (STM32_HSICLK / STM32_PLLM_VALUE)

#else
#error "invalid STM32_PLLSRC value specified"
#endif

/*
 * PLLs input frequency range check.
 */
#if (STM32_PLLCLKIN < STM32_PLLIN_MIN) || (STM32_PLLCLKIN > STM32_PLLIN_MAX)
#error "STM32_PLLCLKIN outside acceptable range (STM32_PLLIN_MIN...STM32_PLLIN_MAX)"
#endif

/*
 * PLL enable check.
 */
#if (STM32_CLOCK48_REQUIRED && (STM32_CK48MSEL == STM32_CK48MSEL_PLL)) ||   \
    (STM32_SW == STM32_SW_PLL) ||                                           \
    (STM32_MCO1SEL == STM32_MCO1SEL_PLL) ||                                 \
    (STM32_MCO2SEL == STM32_MCO2SEL_PLL) ||                                 \
    defined(__DOXYGEN__)
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
#if ((STM32_PLLN_VALUE >= 64) && (STM32_PLLN_VALUE <= 432)) ||              \
    defined(__DOXYGEN__)
#define STM32_PLLN                  (STM32_PLLN_VALUE << 6)
#else
#error "invalid STM32_PLLN_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLP field.
 */
#if (STM32_PLLP_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLP                  (0 << 16)

#elif STM32_PLLP_VALUE == 4
#define STM32_PLLP                  (1 << 16)

#elif STM32_PLLP_VALUE == 6
#define STM32_PLLP                  (2 << 16)

#elif STM32_PLLP_VALUE == 8
#define STM32_PLLP                  (3 << 16)

#else
#error "invalid STM32_PLLP_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLQ field.
 */
#if ((STM32_PLLQ_VALUE >= 2) && (STM32_PLLQ_VALUE <= 15)) ||                \
    defined(__DOXYGEN__)
#define STM32_PLLQ                  (STM32_PLLQ_VALUE << 24)
#else
#error "invalid STM32_PLLQ_VALUE value specified"
#endif

/**
 * @brief   PLL VCO frequency.
 */
#define STM32_PLLVCO                (STM32_PLLCLKIN * STM32_PLLN_VALUE)

/*
 * PLL VCO frequency range check.
 */
#if (STM32_PLLVCO < STM32_PLLVCO_MIN) || (STM32_PLLVCO > STM32_PLLVCO_MAX)
#error "STM32_PLLVCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/**
 * @brief   PLL P output clock frequency.
 */
#define STM32_PLL_P_CLKOUT          (STM32_PLLVCO / STM32_PLLP_VALUE)

/**
 * @brief   PLL Q output clock frequency.
 */
#define STM32_PLL_Q_CLKOUT          (STM32_PLLVCO / STM32_PLLQ_VALUE)

/*
 * PLL output frequency range check.
 */
#if (STM32_PLL_P_CLKOUT < STM32_PLLOUT_MIN) || (STM32_PLL_P_CLKOUT > STM32_PLLOUT_MAX)
#error "STM32_PLL_P_CLKOUT outside acceptable range (STM32_PLLOUT_MIN...STM32_PLLOUT_MAX)"
#endif

/**
 * @brief   System clock source.
 */
#if STM32_NO_INIT || defined(__DOXYGEN__)
#define STM32_SYSCLK                STM32_HSICLK

#elif (STM32_SW == STM32_SW_HSI)
#define STM32_SYSCLK                STM32_HSICLK

#elif (STM32_SW == STM32_SW_HSE)
#define STM32_SYSCLK                STM32_HSECLK

#elif (STM32_SW == STM32_SW_PLL)
#define STM32_SYSCLK                STM32_PLL_P_CLKOUT

#else
#error "invalid STM32_SW value specified"
#endif

/* Check on the system clock.*/
#if STM32_SYSCLK > STM32_SYSCLK_MAX
#error "STM32_SYSCLK above maximum rated frequency (STM32_SYSCLK_MAX)"
#endif

/* Calculating VOS settings.*/
#if STM32_SYSCLK <= 144000000
#define STM32_VOS                   STM32_VOS_SCALE3
#define STM32_OVERDRIVE_REQUIRED    FALSE

#elif STM32_SYSCLK <= 168000000
#define STM32_VOS                   STM32_VOS_SCALE2
#define STM32_OVERDRIVE_REQUIRED    FALSE

#elif STM32_SYSCLK <= 180000000
#define STM32_VOS                   STM32_VOS_SCALE1
#define STM32_OVERDRIVE_REQUIRED    FALSE

#else
#define STM32_VOS                   STM32_VOS_SCALE1
#define STM32_OVERDRIVE_REQUIRED    TRUE
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

/*
 * PLLI2S enable check.
 */
#if (STM32_I2SSRC == STM32_I2SSRC_PLLI2S) ||                                \
    (STM32_SAI1SEL == STM32_SAI1SEL_I2SPLL) ||                              \
    (STM32_SAI2SEL == STM32_SAI2SEL_I2SPLL) ||                              \
    defined(__DOXYGEN__)
/**
 * @brief   PLLI2S activation flag.
 */
#define STM32_ACTIVATE_PLLI2S       TRUE
#else
#define STM32_ACTIVATE_PLLI2S       FALSE
#endif

/**
 * @brief   STM32_PLLI2SN field.
 */
#if ((STM32_PLLI2SN_VALUE >= 49) && (STM32_PLLI2SN_VALUE <= 432)) ||       \
    defined(__DOXYGEN__)
#define STM32_PLLI2SN               (STM32_PLLI2SN_VALUE << 6)
#else
#error "invalid STM32_PLLI2SN_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLI2SQ field.
 */
#if ((STM32_PLLI2SQ_VALUE >= 2) && (STM32_PLLI2SQ_VALUE <= 15)) ||          \
    defined(__DOXYGEN__)
#define STM32_PLLI2SQ               (STM32_PLLI2SQ_VALUE << 24)
#else
#error "invalid STM32_PLLI2SQ_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLI2SR field.
 */
#if ((STM32_PLLI2SR_VALUE >= 2) && (STM32_PLLI2SR_VALUE <= 7)) ||           \
    defined(__DOXYGEN__)
#define STM32_PLLI2SR               (STM32_PLLI2SR_VALUE << 28)
#else
#error "invalid STM32_PLLI2SR_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLI2SP field.
 */
#if (STM32_PLLI2SP_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLI2SP               (0 << 16)

#elif STM32_PLLI2SP_VALUE == 4
#define STM32_PLLI2SP               (1 << 16)

#elif STM32_PLLI2SP_VALUE == 6
#define STM32_PLLI2SP               (2 << 16)

#elif STM32_PLLI2SP_VALUE == 8
#define STM32_PLLI2SP               (3 << 16)

#else
#error "invalid STM32_PLLI2SP_VALUE value specified"
#endif

/**
 * @brief   PLLI2S VCO frequency.
 */
#define STM32_PLLI2SVCO             (STM32_PLLCLKIN * STM32_PLLI2SN_VALUE)

/*
 * PLLI2S VCO frequency range check.
 */
#if (STM32_PLLI2SVCO < STM32_PLLVCO_MIN) ||                                 \
    (STM32_PLLI2SVCO > STM32_PLLVCO_MAX)
#error "STM32_PLLI2SVCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/**
 * @brief   PLLI2S P output clock frequency.
 */
#define STM32_PLLI2S_P_CLKOUT       (STM32_PLLI2SVCO / STM32_PLLI2SP_VALUE)

/**
 * @brief   PLLI2S Q output clock frequency.
 */
#define STM32_PLLI2S_Q_CLKOUT       (STM32_PLLI2SVCO / STM32_PLLI2SQ_VALUE)

/**
 * @brief   PLLI2S R output clock frequency.
 */
#define STM32_PLLI2S_R_CLKOUT       (STM32_PLLI2SVCO / STM32_PLLI2SR_VALUE)

/**
 * @brief   STM32_PLLI2SDIVQ field.
 */
#if (STM32_PLLI2SDIVQ_VALUE < 1) || (STM32_PLLI2SDIVQ_VALUE > 32)
#error "STM32_PLLI2SDIVQ_VALUE out of acceptable range"
#endif
#define STM32_PLLI2SDIVQ            ((STM32_PLLI2SDIVQ_VALUE - 1) << 0)

/**
 * @brief   PLLI2S Q output clock frequency after divisor.
 */
#define STM32_PLLI2SDIVQ_CLKOUT     (STM32_PLLI2S_Q_CLKOUT / STM32_PLLI2SDIVQ_VALUE)

/*
 * PLLSAI enable check.
 */
#if (STM32_CLOCK48_REQUIRED && (STM32_CK48MSEL == STM32_CK48MSEL_PLLSAI)) | \
    STM32_LCDTFT_REQUIRED ||                                                \
    (STM32_SAI1SEL == STM32_SAI1SEL_SAIPLL) ||                              \
    (STM32_SAI2SEL == STM32_SAI2SEL_SAIPLL) ||                              \
    defined(__DOXYGEN__)
/**
 * @brief   PLLSAI activation flag.
 */
#define STM32_ACTIVATE_PLLSAI       TRUE
#else
#define STM32_ACTIVATE_PLLSAI       FALSE
#endif

/**
 * @brief   STM32_PLLSAIN field.
 */
#if ((STM32_PLLSAIN_VALUE >= 49) && (STM32_PLLSAIN_VALUE <= 432)) ||        \
    defined(__DOXYGEN__)
#define STM32_PLLSAIN               (STM32_PLLSAIN_VALUE << 6)
#else
#error "invalid STM32_PLLSAIN_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAIQ field.
 */
#if ((STM32_PLLSAIQ_VALUE >= 2) && (STM32_PLLSAIQ_VALUE <= 15)) ||          \
    defined(__DOXYGEN__)
#define STM32_PLLSAIQ               (STM32_PLLSAIQ_VALUE << 24)
#else
#error "invalid STM32_PLLSAIR_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAIR field.
 */
#if ((STM32_PLLSAIR_VALUE >= 2) && (STM32_PLLSAIR_VALUE <= 7)) ||           \
    defined(__DOXYGEN__)
#define STM32_PLLSAIR               (STM32_PLLSAIR_VALUE << 28)
#else
#error "invalid STM32_PLLSAIR_VALUE value specified"
#endif

/**
 * @brief   STM32_PLLSAIP field.
 */
#if (STM32_PLLSAIP_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAIP               (0 << 16)

#elif STM32_PLLSAIP_VALUE == 4
#define STM32_PLLSAIP               (1 << 16)

#elif STM32_PLLSAIP_VALUE == 6
#define STM32_PLLSAIP               (2 << 16)

#elif STM32_PLLSAIP_VALUE == 8
#define STM32_PLLSAIP               (3 << 16)

#else
#error "invalid STM32_PLLSAIP_VALUE value specified"
#endif

/**
 * @brief   PLLSAI VCO frequency.
 */
#define STM32_PLLSAIVCO             (STM32_PLLCLKIN * STM32_PLLSAIN_VALUE)

/*
 * PLLSAI VCO frequency range check.
 */
#if (STM32_PLLSAIVCO < STM32_PLLVCO_MIN) ||                                 \
    (STM32_PLLSAIVCO > STM32_PLLVCO_MAX)
#error "STM32_PLLSAIVCO outside acceptable range (STM32_PLLVCO_MIN...STM32_PLLVCO_MAX)"
#endif

/**
 * @brief   PLLSAI P output clock frequency.
 */
#define STM32_PLLSAI_P_CLKOUT       (STM32_PLLSAIVCO / STM32_PLLSAIP_VALUE)

/**
 * @brief   PLLSAI Q output clock frequency.
 */
#define STM32_PLLSAI_Q_CLKOUT       (STM32_PLLSAIVCO / STM32_PLLSAIQ_VALUE)

/**
 * @brief   PLLSAI R output clock frequency.
 */
#define STM32_PLLSAI_R_CLKOUT       (STM32_PLLSAIVCO / STM32_PLLSAIR_VALUE)

/**
 * @brief   STM32_PLLSAIDIVQ field.
 */
#if (STM32_PLLSAIDIVQ_VALUE < 1) || (STM32_PLLSAIDIVQ_VALUE > 32)
#error "STM32_PLLSAIDIVQ_VALUE out of acceptable range"
#endif
#define STM32_PLLSAIDIVQ            ((STM32_PLLSAIDIVQ_VALUE - 1) << 8)

/**
 * @brief   PLLSAI Q output clock frequency after divisor.
 */
#define STM32_PLLSAIDIVQ_CLKOUT     (STM32_PLLSAI_Q_CLKOUT / STM32_PLLSAIDIVQ_VALUE)

/*
 * STM32_PLLSAIDIVR field.
 */
#if (STM32_PLLSAIDIVR_VALUE == 2) || defined(__DOXYGEN__)
#define STM32_PLLSAIDIVR            (0 << 16)

#elif STM32_PLLSAIDIVR_VALUE == 4
#define STM32_PLLSAIDIVR            (1 << 16)

#elif STM32_PLLSAIDIVR_VALUE == 8
#define STM32_PLLSAIDIVR            (2 << 16)

#elif STM32_PLLSAIDIVR_VALUE == 16
#define STM32_PLLSAIDIVR            (3 << 16)

#else
#error "invalid STM32_PLLSAIDIVR_VALUE value specified"
#endif

/**
 * @brief   PLLSAI R output clock frequency after divisor.
 */
#define STM32_PLLSAIDIVR_CLKOUT       (STM32_PLLSAI_R_CLKOUT / STM32_PLLSAIDIVR_VALUE)

/**
 * @brief   MCO1 divider clock.
 */
#if (STM32_MCO1SEL == STM32_MCO1SEL_HSI) || defined(__DOXYGEN__)
#define STM32_MCO1DIVCLK            STM32_HSICLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_LSE
#define STM32_MCO1DIVCLK            STM32_LSECLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSE
#define STM32_MCO1DIVCLK            STM32_HSECLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_PLL
#define STM32_MCO1DIVCLK            STM32_PLL_P_CLKOUT

#else
#error "invalid STM32_MCO1SEL value specified"
#endif

/**
 * @brief   MCO1 output pin clock.
 */
#if (STM32_MCO1PRE == STM32_MCO1PRE_DIV1) || defined(__DOXYGEN__)
#define STM32_MCO1CLK               STM32_MCO1DIVCLK

#elif STM32_MCO1PRE == STM32_MCO1PRE_DIV2
#define STM32_MCO1CLK               (STM32_MCO1DIVCLK / 2)

#elif STM32_MCO1PRE == STM32_MCO1PRE_DIV3
#define STM32_MCO1CLK               (STM32_MCO1DIVCLK / 3)

#elif STM32_MCO1PRE == STM32_MCO1PRE_DIV4
#define STM32_MCO1CLK               (STM32_MCO1DIVCLK / 4)

#elif STM32_MCO1PRE == STM32_MCO1PRE_DIV5
#define STM32_MCO1CLK               (STM32_MCO1DIVCLK / 5)

#else
#error "invalid STM32_MCO1PRE value specified"
#endif

/**
 * @brief   MCO2 divider clock.
 */
#if (STM32_MCO2SEL == STM32_MCO2SEL_HSE) || defined(__DOXYGEN__)
#define STM32_MCO2DIVCLK            STM32_HSECLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_PLL
#define STM32_MCO2DIVCLK            STM32_PLL_P_CLKOUT

#elif STM32_MCO2SEL == STM32_MCO2SEL_SYSCLK
#define STM32_MCO2DIVCLK            STM32_SYSCLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_PLLI2S
#define STM32_MCO2DIVCLK            STM32_PLLI2S

#else
#error "invalid STM32_MCO2SEL value specified"
#endif

/**
 * @brief   MCO2 output pin clock.
 */
#if (STM32_MCO2PRE == STM32_MCO2PRE_DIV1) || defined(__DOXYGEN__)
#define STM32_MCO2CLK               STM32_MCO2DIVCLK

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV2
#define STM32_MCO2CLK               (STM32_MCO2DIVCLK / 2)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV3
#define STM32_MCO2CLK               (STM32_MCO2DIVCLK / 3)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV4
#define STM32_MCO2CLK               (STM32_MCO2DIVCLK / 4)

#elif STM32_MCO2PRE == STM32_MCO2PRE_DIV5
#define STM32_MCO2CLK               (STM32_MCO2DIVCLK / 5)

#else
#error "invalid STM32_MCO2PRE value specified"
#endif

/**
 * @brief   RTC HSE divider setting.
 */
#if ((STM32_RTCPRE_VALUE >= 2) && (STM32_RTCPRE_VALUE <= 31)) ||            \
    defined(__DOXYGEN__)
#define STM32_RTCPRE                (STM32_RTCPRE_VALUE << 16)
#else
#error "invalid STM32_RTCPRE value specified"
#endif

/**
 * @brief   HSE divider toward RTC clock.
 */
#if ((STM32_RTCPRE_VALUE >= 2) && (STM32_RTCPRE_VALUE <= 31))  ||           \
    defined(__DOXYGEN__)
#define STM32_HSEDIVCLK             (STM32_HSECLK / STM32_RTCPRE_VALUE)
#else
#error "invalid STM32_RTCPRE value specified"
#endif

/**
 * @brief   RTC clock.
 */
#if (STM32_RTCSEL == STM32_RTCSEL_NOCLOCK) || defined(__DOXYGEN__)
#define STM32_RTCCLK                0

#elif STM32_RTCSEL == STM32_RTCSEL_LSE
#define STM32_RTCCLK                STM32_LSECLK

#elif STM32_RTCSEL == STM32_RTCSEL_LSI
#define STM32_RTCCLK                STM32_LSICLK

#elif STM32_RTCSEL == STM32_RTCSEL_HSEDIV
#define STM32_RTCCLK                STM32_HSEDIVCLK

#else
#error "invalid STM32_RTCSEL value specified"
#endif

/**
 * @brief   USART1 frequency.
 */
#if (STM32_USART1SEL == STM32_USART1SEL_PCLK2) || defined(__DOXYGEN__)
#define STM32_USART1CLK             STM32_PCLK2
#elif STM32_USART1SEL == STM32_USART1SEL_SYSCLK
#define STM32_USART1CLK             STM32_SYSCLK
#elif STM32_USART1SEL == STM32_USART1SEL_HSI
#define STM32_USART1CLK             STM32_HSICLK
#elif STM32_USART1SEL == STM32_USART1SEL_LSE
#define STM32_USART1CLK             STM32_LSECLK
#else
#error "invalid source selected for USART1 clock"
#endif

/**
 * @brief   USART2 frequency.
 */
#if (STM32_USART2SEL == STM32_USART2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART2CLK             STM32_PCLK1
#elif STM32_USART2SEL == STM32_USART2SEL_SYSCLK
#define STM32_USART2CLK             STM32_SYSCLK
#elif STM32_USART2SEL == STM32_USART2SEL_HSI
#define STM32_USART2CLK             STM32_HSICLK
#elif STM32_USART2SEL == STM32_USART2SEL_LSE
#define STM32_USART2CLK             STM32_LSECLK
#else
#error "invalid source selected for USART2 clock"
#endif

/**
 * @brief   USART3 frequency.
 */
#if (STM32_USART3SEL == STM32_USART3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_USART3CLK             STM32_PCLK1
#elif STM32_USART3SEL == STM32_USART3SEL_SYSCLK
#define STM32_USART3CLK             STM32_SYSCLK
#elif STM32_USART3SEL == STM32_USART3SEL_HSI
#define STM32_USART3CLK             STM32_HSICLK
#elif STM32_USART3SEL == STM32_USART3SEL_LSE
#define STM32_USART3CLK             STM32_LSECLK
#else
#error "invalid source selected for USART3 clock"
#endif

/**
 * @brief   UART4 frequency.
 */
#if (STM32_UART4SEL == STM32_UART4SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART4CLK              STM32_PCLK1
#elif STM32_UART4SEL == STM32_UART4SEL_SYSCLK
#define STM32_UART4CLK              STM32_SYSCLK
#elif STM32_UART4SEL == STM32_UART4SEL_HSI
#define STM32_UART4CLK              STM32_HSICLK
#elif STM32_UART4SEL == STM32_UART4SEL_LSE
#define STM32_UART4CLK              STM32_LSECLK
#else
#error "invalid source selected for UART4 clock"
#endif

/**
 * @brief   UART5 frequency.
 */
#if (STM32_UART5SEL == STM32_UART5SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART5CLK              STM32_PCLK1
#elif STM32_UART5SEL == STM32_UART5SEL_SYSCLK
#define STM32_UART5CLK              STM32_SYSCLK
#elif STM32_UART5SEL == STM32_UART5SEL_HSI
#define STM32_UART5CLK              STM32_HSICLK
#elif STM32_UART5SEL == STM32_UART5SEL_LSE
#define STM32_UART5CLK              STM32_LSECLK
#else
#error "invalid source selected for UART5 clock"
#endif

/**
 * @brief   USART6 frequency.
 */
#if (STM32_USART6SEL == STM32_USART6SEL_PCLK2) || defined(__DOXYGEN__)
#define STM32_USART6CLK             STM32_PCLK2
#elif STM32_USART6SEL == STM32_USART6SEL_SYSCLK
#define STM32_USART6CLK             STM32_SYSCLK
#elif STM32_USART6SEL == STM32_USART6SEL_HSI
#define STM32_USART6CLK             STM32_HSICLK
#elif STM32_USART6SEL == STM32_USART6SEL_LSE
#define STM32_USART6CLK             STM32_LSECLK
#else
#error "invalid source selected for USART6 clock"
#endif

/**
 * @brief   UART7 frequency.
 */
#if (STM32_UART7SEL == STM32_UART7SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART7CLK              STM32_PCLK1
#elif STM32_UART7SEL == STM32_UART7SEL_SYSCLK
#define STM32_UART7CLK              STM32_SYSCLK
#elif STM32_UART7SEL == STM32_UART7SEL_HSI
#define STM32_UART7CLK              STM32_HSICLK
#elif STM32_UART7SEL == STM32_UART7SEL_LSE
#define STM32_UART7CLK              STM32_LSECLK
#else
#error "invalid source selected for UART7 clock"
#endif

/**
 * @brief   UART8 frequency.
 */
#if (STM32_UART8SEL == STM32_UART8SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_UART8CLK              STM32_PCLK1
#elif STM32_UART8SEL == STM32_UART8SEL_SYSCLK
#define STM32_UART8CLK              STM32_SYSCLK
#elif STM32_UART8SEL == STM32_UART8SEL_HSI
#define STM32_UART8CLK              STM32_HSICLK
#elif STM32_UART8SEL == STM32_UART8SEL_LSE
#define STM32_UART8CLK              STM32_LSECLK
#else
#error "invalid source selected for UART8 clock"
#endif

/**
 * @brief   I2C1 frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C1CLK               STM32_PCLK1
#elif STM32_I2C1SEL == STM32_I2C1SEL_SYSCLK
#define STM32_I2C1CLK               STM32_SYSCLK
#elif STM32_I2C1SEL == STM32_I2C1SEL_HSI
#define STM32_I2C1CLK               STM32_HSICLK
#else
#error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C2 frequency.
 */
#if (STM32_I2C2SEL == STM32_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C2CLK               STM32_PCLK1
#elif STM32_I2C2SEL == STM32_I2C2SEL_SYSCLK
#define STM32_I2C2CLK               STM32_SYSCLK
#elif STM32_I2C2SEL == STM32_I2C2SEL_HSI
#define STM32_I2C2CLK               STM32_HSICLK
#else
#error "invalid source selected for I2C2 clock"
#endif

/**
 * @brief   I2C3 frequency.
 */
#if (STM32_I2C3SEL == STM32_I2C3SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C3CLK               STM32_PCLK1
#elif STM32_I2C3SEL == STM32_I2C3SEL_SYSCLK
#define STM32_I2C3CLK               STM32_SYSCLK
#elif STM32_I2C3SEL == STM32_I2C3SEL_HSI
#define STM32_I2C3CLK               STM32_HSICLK
#else
#error "invalid source selected for I2C3 clock"
#endif

/**
 * @brief   I2C4 frequency.
 */
#if (STM32_I2C4SEL == STM32_I2C4SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_I2C4CLK               STM32_PCLK1
#elif STM32_I2C4SEL == STM32_I2C4SEL_SYSCLK
#define STM32_I2C4CLK               STM32_SYSCLK
#elif STM32_I2C4SEL == STM32_I2C4SEL_HSI
#define STM32_I2C4CLK               STM32_HSICLK
#else
#error "invalid source selected for I2C4 clock"
#endif

/**
 * @brief   LPTIM1 frequency.
 */
#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PCLK1) || defined(__DOXYGEN__)
#define STM32_LPTIM1CLK             STM32_PCLK1
#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI
#define STM32_LPTIM1CLK             STM32_LSICLK
#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI
#define STM32_LPTIM1CLK             STM32_HSICLK
#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE
#define STM32_LPTIM1CLK             STM32_LSECLK
#else
#error "invalid source selected for LPTIM1 clock"
#endif

/**
 * @brief   48MHz frequency.
 */
#if STM32_CLOCK48_REQUIRED || defined(__DOXYGEN__)
#if (STM32_CK48MSEL == STM32_CK48MSEL_PLL) || defined(__DOXYGEN__)
#define STM32_PLL48CLK              (STM32_PLLVCO / STM32_PLLQ_VALUE)
#elif STM32_CK48MSEL == STM32_CK48MSEL_PLLSAI
#define STM32_PLL48CLK              (STM32_PLLSAIVCO / STM32_PLLSAIQ_VALUE)
#else
#error "invalid source selected for PLL48CLK clock"
#endif
#else /* !STM32_CLOCK48_REQUIRED */
#define STM32_PLL48CLK              0
#endif /* !STM32_CLOCK48_REQUIRED */

/**
 * @brief   I2S frequency.
 */
#if (STM32_I2SSRC == STM32_I2SSRC_OFF) || defined(__DOXYGEN__)
#define STM32_I2SCLK                0
#elif STM32_I2SSRC == STM32_I2SSRC_CKIN
#define STM32_I2SCLK                0 /* Unknown, would require a board value */
#elif STM32_I2SSRC == STM32_I2SSRC_PLLI2S
#define STM32_I2SCLK                STM32_PLLI2S_R_CLKOUT
#else
#error "invalid source selected for I2S clock"
#endif

/**
 * @brief   SAI1 frequency.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_OFF) || defined(__DOXYGEN__)
#define STM32_SAI1CLK               0
#elif STM32_SAI1SEL == STM32_SAI1SEL_SAIPLL
#define STM32_SAI1CLK               STM32_PLLSAIDIVQ_CLKOUT
#elif STM32_SAI1SEL == STM32_SAI1SEL_I2SPLL
#define STM32_SAI1CLK               STM32_PLLI2SDIVQ_CLKOUT
#elif STM32_SAI1SEL == STM32_SAI1SEL_CKIN
#define STM32_SAI1CLK               0 /* Unknown, would require a board value */
#else
#error "invalid source selected for SAI1 clock"
#endif

/**
 * @brief   SAI2 frequency.
 */
#if (STM32_SAI2SEL == STM32_SAI2SEL_OFF) || defined(__DOXYGEN__)
#define STM32_SAI2CLK               0
#elif STM32_SAI2SEL == STM32_SAI2SEL_SAIPLL
#define STM32_SAI2CLK               STM32_PLLSAIDIVQ_CLKOUT
#elif STM32_SAI2SEL == STM32_SAI2SEL_I2SPLL
#define STM32_SAI2CLK               STM32_PLLI2SDIVQ_CLKOUT
#elif STM32_SAI2SEL == STM32_SAI2SEL_CKIN
#define STM32_SAI2CLK               0 /* Unknown, would require a board value */
#else
#error "invalid source selected for SAI2 clock"
#endif

/**
 * @brief   SDMMC frequency.
 */
#if (STM32_SDMMCSEL == STM32_SDMMCSEL_PLL48CLK) || defined(__DOXYGEN__)
#define STM32_SDMMCCLK               STM32_PLL48CLK
#elif STM32_SDMMCSEL == STM32_SDMMCSEL_SYSCLK
#define STM32_SDMMCCLK               STM32_SYSCLK
#else
#error "invalid source selected for SDMMC clock"
#endif

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
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
#define STM32_FLASHBITS             0x00000000

#elif STM32_HCLK <= STM32_1WS_THRESHOLD
#define STM32_FLASHBITS             0x00000001

#elif STM32_HCLK <= STM32_2WS_THRESHOLD
#define STM32_FLASHBITS             0x00000002

#elif STM32_HCLK <= STM32_3WS_THRESHOLD
#define STM32_FLASHBITS             0x00000003

#elif STM32_HCLK <= STM32_4WS_THRESHOLD
#define STM32_FLASHBITS             0x00000004

#elif STM32_HCLK <= STM32_5WS_THRESHOLD
#define STM32_FLASHBITS             0x00000005

#elif STM32_HCLK <= STM32_6WS_THRESHOLD
#define STM32_FLASHBITS             0x00000006

#elif STM32_HCLK <= STM32_7WS_THRESHOLD
#define STM32_FLASHBITS             0x00000007

#elif STM32_HCLK <= STM32_8WS_THRESHOLD
#define STM32_FLASHBITS             0x00000008

#else
#define STM32_FLASHBITS             0x00000009
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
#include "mpu.h"
#include "stm32_isr.h"
#include "stm32_dma.h"
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
