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
#define CLK_MSIS                3U
#define CLK_MSIK                4U
#define CLK_SYSCLK              5U
#define CLK_HCLK                6U
#define CLK_PCLK1               7U
#define CLK_PCLK1TIM            8U
#define CLK_PCLK2               9U
#define CLK_PCLK2TIM            10U
#define CLK_PCLK3               11U
#define CLK_MCO1                12U
#define CLK_MCO2                13U
#define CLK_ARRAY_SIZE          14U

#define CLK_POINT_NAMES                                                     \
  {                                                                         \
    "HSI16",                                                                \
    "HSI48",                                                                \
    "HSE",                                                                  \
    "MSIS",                                                                 \
    "MSIK",                                                                 \
    "SYSCLK",                                                               \
    "HCLK",                                                                 \
    "PCLK1",                                                                \
    "PCLK1TIM",                                                             \
    "PCLK2",                                                                \
    "PCLK2TIM",                                                             \
    "PCLK3",                                                                \
    "MCO1",                                                                 \
    "MCO2"                                                                  \
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
  #define STM32_CFG_CLOCK_DYNAMIC           FALSE
#endif

/**
 * @brief   Selects the core voltage scaling range.
 */
#if !defined(STM32_CFG_PWR_VOSR) || defined(__DOXYGEN__)
  #define STM32_CFG_PWR_VOSR                PWR_VOSR_RANGE1
#endif

/**
 * @brief   Selects the MSIRC0 operating mode.
 */
#if !defined(STM32_CFG_MSIRC0_MODE) || defined(__DOXYGEN__)
  #define STM32_CFG_MSIRC0_MODE             RCC_MSIRC0_PLL_LSE
#endif

/**
 * @brief   Selects the MSIRC1 operating mode.
 */
#if !defined(STM32_CFG_MSIRC1_MODE) || defined(__DOXYGEN__)
  #define STM32_CFG_MSIRC1_MODE             RCC_MSIRC1_FREE
#endif

/**
 * @brief   Selects the MSIRC1 PLL correction factor.
 */
#if !defined(STM32_CFG_MSIPLL1N_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_MSIPLL1N_VALUE          0
#endif

/**
 * @brief   Selects the source and divide factor for MSIS.
 */
#if !defined(STM32_CFG_MSIS_SRCDIV) || defined(__DOXYGEN__)
  #define STM32_CFG_MSIS_SRCDIV             RCC_ICSCR1_MSIS_IRC0_DIV1
#endif

/**
 * @brief   Selects the source and divide factor for MSIK.
 */
#if !defined(STM32_CFG_MSIK_SRCDIV) || defined(__DOXYGEN__)
  #define STM32_CFG_MSIK_SRCDIV             RCC_ICSCR1_MSIK_IRC0_DIV1
#endif

/**
 * @brief   Selects the system clock source after STOP mode.
 */
#if !defined(STM32_CFG_STOPWUCK) || defined(__DOXYGEN__)
  #define STM32_CFG_STOPWUCK                RCC_CFGR1_STOPWUCK_MSIS
#endif

/**
 * @brief   Selects the kernel clock source after STOP mode.
 */
#if !defined(STM32_CFG_STOPKERWUCK) || defined(__DOXYGEN__)
  #define STM32_CFG_STOPKERWUCK             RCC_CFGR1_STOPKERWUCK_MSIK
#endif

/**
 * @brief   Selects the MSI bias mode.
 */
#if !defined(STM32_CFG_MSIBIAS) || defined(__DOXYGEN__)
  #define STM32_CFG_MSIBIAS                 RCC_ICSCR1_MSIBIAS_CONTINUOUS
#endif

/**
 * @brief   Enables demand for the DAC1 sample-and-hold clock.
 */
#if !defined(STM32_CFG_DAC1SH_REQUIRED) || defined(__DOXYGEN__)
  #define STM32_CFG_DAC1SH_REQUIRED         FALSE
#endif

/**
 * @brief   Enables the HSI16 clock source.
 */
#if !defined(STM32_CFG_HSI16_ENABLE) || defined(__DOXYGEN__)
  #define STM32_CFG_HSI16_ENABLE            FALSE
#endif

/**
 * @brief   Enables the HSI48 clock source.
 */
#if !defined(STM32_CFG_HSI48_ENABLE) || defined(__DOXYGEN__)
  #define STM32_CFG_HSI48_ENABLE            FALSE
#endif

/**
 * @brief   Enables the HSE clock source.
 */
#if !defined(STM32_CFG_HSE_ENABLE) || defined(__DOXYGEN__)
  #define STM32_CFG_HSE_ENABLE              FALSE
#endif

/**
 * @brief   Enables the LSE clock source.
 */
#if !defined(STM32_CFG_LSE_ENABLE) || defined(__DOXYGEN__)
  #define STM32_CFG_LSE_ENABLE              FALSE
#endif

/**
 * @brief   Enables the LSI clock source.
 */
#if !defined(STM32_CFG_LSI_ENABLE) || defined(__DOXYGEN__)
  #define STM32_CFG_LSI_ENABLE              FALSE
#endif

/**
 * @brief   Selects the SYSCLK clock source.
 * @note    Allowed sources:
 *          - MSIS.
 *          - HSI16.
 *          - HSE.
 */
#if !defined(STM32_CFG_SYSCLK_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_SYSCLK_SEL              RCC_CFGR1_SW_MSIS
#endif

/**
 * @brief   Configures the HCLK clock divider value.
 */
#if !defined(STM32_CFG_HCLK_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_HCLK_VALUE              1
#endif

/**
 * @brief   Configures the PCLK1 clock divider value.
 */
#if !defined(STM32_CFG_PCLK1_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_PCLK1_VALUE             1
#endif

/**
 * @brief   Configures the PCLK2 clock divider value.
 */
#if !defined(STM32_CFG_PCLK2_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_PCLK2_VALUE             1
#endif

/**
 * @brief   Configures the PCLK3 clock divider value.
 */
#if !defined(STM32_CFG_PCLK3_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_PCLK3_VALUE             1
#endif

/**
 * @brief   Selects the MCO1DIV clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - SYSCLK.
 *          - MSIS.
 *          - HSI16.
 *          - HSE.
 *          - LSI.
 *          - LSE.
 *          - HSI48.
 *          - MSIK.
 */
#if !defined(STM32_CFG_MCO1DIV_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_MCO1DIV_SEL             RCC_CFGR1_MCO1SEL_OFF
#endif

/**
 * @brief   Configures the MCO1 clock divider value.
 */
#if !defined(STM32_CFG_MCO1_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_MCO1_VALUE              1
#endif

/**
 * @brief   Selects the MCO2DIV clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - SYSCLK.
 *          - MSIS.
 *          - HSI16.
 *          - HSE.
 *          - LSI.
 *          - LSE.
 *          - HSI48.
 *          - MSIK.
 */
#if !defined(STM32_CFG_MCO2DIV_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_MCO2DIV_SEL             RCC_CFGR1_MCO2SEL_OFF
#endif

/**
 * @brief   Configures the MCO2 clock divider value.
 */
#if !defined(STM32_CFG_MCO2_VALUE) || defined(__DOXYGEN__)
  #define STM32_CFG_MCO2_VALUE              1
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
  #define STM32_CFG_RTC_SEL                 RCC_BDCR_RTCSEL_NOCLOCK
#endif

/**
 * @brief   Selects the LSCO clock source.
 * @note    Allowed sources:
 *          - NONE.
 *          - LSI.
 *          - LSE.
 */
#if !defined(STM32_CFG_LSCO_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_LSCO_SEL                RCC_BDCR_LSCOSEL_NOCLOCK
#endif

/**
 * @brief   Selects the USART1 clock source.
 * @note    Allowed sources:
 *          - PCLK2.
 *          - HSI16.
 */
#if !defined(STM32_CFG_USART1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_USART1_SEL              RCC_CCIPR1_USART1SEL_PCLK2
#endif

/**
 * @brief   Selects the USART3 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - HSI16.
 */
#if !defined(STM32_CFG_USART3_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_USART3_SEL              RCC_CCIPR1_USART3SEL_PCLK1
#endif

/**
 * @brief   Selects the UART4 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - HSI16.
 */
#if !defined(STM32_CFG_UART4_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_UART4_SEL               RCC_CCIPR1_UART4SEL_PCLK1
#endif

/**
 * @brief   Selects the UART5 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - HSI16.
 */
#if !defined(STM32_CFG_UART5_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_UART5_SEL               RCC_CCIPR1_UART5SEL_PCLK1
#endif

/**
 * @brief   Selects the LPUART1 clock source.
 * @note    Allowed sources:
 *          - PCLK3.
 *          - HSI16.
 *          - LSE.
 *          - MSIK.
 */
#if !defined(STM32_CFG_LPUART1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_LPUART1_SEL             RCC_CCIPR3_LPUART1SEL_PCLK3
#endif

/**
 * @brief   Selects the LPTIM1 clock source.
 * @note    Allowed sources:
 *          - MSIK.
 *          - LSI.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_LPTIM1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_LPTIM1_SEL              RCC_CCIPR3_LPTIM1SEL_MSIK
#endif

/**
 * @brief   Selects the LPTIM2 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - LSI.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_LPTIM2_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_LPTIM2_SEL              RCC_CCIPR1_LPTIM2SEL_PCLK1
#endif

/**
 * @brief   Selects the LPTIM34 clock source.
 * @note    Allowed sources:
 *          - MSIK.
 *          - LSI.
 *          - HSI16.
 *          - LSE.
 */
#if !defined(STM32_CFG_LPTIM34_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_LPTIM34_SEL             RCC_CCIPR3_LPTIM34SEL_MSIK
#endif

/**
 * @brief   Selects the SPI1 clock source.
 * @note    Allowed sources:
 *          - PCLK2.
 *          - MSIK.
 */
#if !defined(STM32_CFG_SPI1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_SPI1_SEL                RCC_CCIPR1_SPI1SEL_PCLK2
#endif

/**
 * @brief   Selects the SPI2 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - MSIK.
 */
#if !defined(STM32_CFG_SPI2_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_SPI2_SEL                RCC_CCIPR1_SPI2SEL_PCLK1
#endif

/**
 * @brief   Selects the SPI3 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - MSIK.
 */
#if !defined(STM32_CFG_SPI3_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_SPI3_SEL                RCC_CCIPR2_SPI3SEL_PCLK1
#endif

/**
 * @brief   Selects the OCTOSPI clock source.
 * @note    Allowed sources:
 *          - SYSCLK.
 *          - MSIK.
 */
#if !defined(STM32_CFG_OCTOSPI_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_OCTOSPI_SEL             RCC_CCIPR2_OCTOSPISEL_SYSCLK
#endif

/**
 * @brief   Selects the SYSTICK clock source.
 * @note    Allowed sources:
 *          - HCLKDIV8.
 *          - LSI.
 *          - LSE.
 */
#if !defined(STM32_CFG_SYSTICK_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_SYSTICK_SEL             RCC_CCIPR1_SYSTICKSEL_HCLKDIV8
#endif

/**
 * @brief   Selects the ICLK clock source.
 * @note    Allowed sources:
 *          - HSI48.
 *          - MSIK.
 *          - HSE.
 *          - SYSCLK.
 */
#if !defined(STM32_CFG_ICLK_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_ICLK_SEL                RCC_CCIPR1_ICLKSEL_SYSCLK
#endif

/**
 * @brief   Selects the USB clock source.
 * @note    Allowed sources:
 *          - ICLK.
 *          - ICLKDIV2.
 */
#if !defined(STM32_CFG_USB_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_USB_SEL                 RCC_CCIPR1_USB1SEL_ICLKDIV2
#endif

/**
 * @brief   Selects the I2C1 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - MSIK.
 */
#if !defined(STM32_CFG_I2C1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_I2C1_SEL                RCC_CCIPR1_I2C1SEL_PCLK1
#endif

/**
 * @brief   Selects the I2C2 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - MSIK.
 */
#if !defined(STM32_CFG_I2C2_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_I2C2_SEL                RCC_CCIPR1_I2C2SEL_PCLK1
#endif

/**
 * @brief   Selects the I2C3 clock source.
 * @note    Allowed sources:
 *          - PCLK3.
 *          - MSIK.
 */
#if !defined(STM32_CFG_I2C3_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_I2C3_SEL                RCC_CCIPR3_I2C3SEL_PCLK3
#endif

/**
 * @brief   Selects the I3C1 clock source.
 * @note    Allowed sources:
 *          - PCLK1.
 *          - MSIK.
 */
#if !defined(STM32_CFG_I3C1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_I3C1_SEL                RCC_CCIPR1_I3C1SEL_PCLK1
#endif

/**
 * @brief   Selects the I3C2 clock source.
 * @note    Allowed sources:
 *          - PCLK3.
 *          - MSIK.
 */
#if !defined(STM32_CFG_I3C2_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_I3C2_SEL                RCC_CCIPR1_I3C2SEL_PCLK3
#endif

/**
 * @brief   Selects the ADCDAC clock source.
 * @note    Allowed sources:
 *          - HCLK.
 *          - HSE.
 *          - MSIK.
 */
#if !defined(STM32_CFG_ADCDAC_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_ADCDAC_SEL              RCC_CCIPR2_ADCDACSEL_HCLK
#endif

/**
 * @brief   Selects the DAC1SH clock source.
 * @note    Allowed sources:
 *          - LSE.
 *          - LSI.
 */
#if !defined(STM32_CFG_DAC1SH_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_DAC1SH_SEL              RCC_CCIPR2_DAC1SHSEL_LSE
#endif

/**
 * @brief   Selects the RNG clock source.
 * @note    Allowed sources:
 *          - HSI48.
 *          - MSIK.
 */
#if !defined(STM32_CFG_RNG_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_RNG_SEL                 RCC_CCIPR2_RNGSEL_MSIK
#endif

/**
 * @brief   Selects the FDCAN1 clock source.
 * @note    Allowed sources:
 *          - SYSCLK.
 *          - MSIK.
 */
#if !defined(STM32_CFG_FDCAN1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_FDCAN1_SEL              RCC_CCIPR1_FDCAN1SEL_SYSCLK
#endif

/**
 * @brief   Selects the SAI1 clock source.
 * @note    Allowed sources:
 *          - MSIK.
 *          - AUDIOCLK.
 *          - HSE.
 */
#if !defined(STM32_CFG_SAI1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_SAI1_SEL                RCC_CCIPR2_SAI1SEL_MSIK
#endif

/**
 * @brief   Selects the ADF1 clock source.
 * @note    Allowed sources:
 *          - HCLK.
 *          - AUDIOCLK.
 *          - HSE.
 *          - SAI1.
 */
#if !defined(STM32_CFG_ADF1_SEL) || defined(__DOXYGEN__)
  #define STM32_CFG_ADF1_SEL                RCC_CCIPR2_ADF1SEL_HCLK
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
#if !defined(TRUE) && !defined(__DOXYGEN__)
  #error "TRUE not defined"
#endif
#if !defined(FALSE) && !defined(__DOXYGEN__)
  #error "FALSE not defined"
#endif
#if !((STM32_CFG_CLOCK_DYNAMIC == TRUE) || (STM32_CFG_CLOCK_DYNAMIC == FALSE)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_CLOCK_DYNAMIC value specified"
#endif

#if !defined(PWR_VOSR_RANGE1) && !defined(__DOXYGEN__)
  #error "PWR_VOSR_RANGE1 not defined"
#endif
#if !defined(PWR_VOSR_RANGE2) && !defined(__DOXYGEN__)
  #error "PWR_VOSR_RANGE2 not defined"
#endif
#if !((STM32_CFG_PWR_VOSR == PWR_VOSR_RANGE1) ||                            \
     (STM32_CFG_PWR_VOSR == PWR_VOSR_RANGE2)) && !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_PWR_VOSR value specified"
#endif

#if !defined(RCC_MSIRC0_FREE) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC0_FREE not defined"
#endif
#if !defined(RCC_MSIRC0_PLL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC0_PLL_LSE not defined"
#endif
#if !defined(RCC_MSIRC0_PLL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC0_PLL_HSE not defined"
#endif
#if !defined(RCC_MSIRC0_PLL_LSE_FAST) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC0_PLL_LSE_FAST not defined"
#endif
#if !defined(RCC_MSIRC0_PLL_HSE_FAST) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC0_PLL_HSE_FAST not defined"
#endif
#if !((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_FREE) ||                         \
     (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) ||                       \
     (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE) ||                       \
     (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST) ||                  \
     (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE_FAST)) && !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_MSIRC0_MODE value specified"
#endif

#if !defined(RCC_MSIRC1_FREE) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC1_FREE not defined"
#endif
#if !defined(RCC_MSIRC1_PLL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC1_PLL_LSE not defined"
#endif
#if !defined(RCC_MSIRC1_PLL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC1_PLL_HSE not defined"
#endif
#if !defined(RCC_MSIRC1_PLL_LSE_FAST) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC1_PLL_LSE_FAST not defined"
#endif
#if !defined(RCC_MSIRC1_PLL_HSE_FAST) && !defined(__DOXYGEN__)
  #error "RCC_MSIRC1_PLL_HSE_FAST not defined"
#endif
#if !((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_FREE) ||                         \
     (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) ||                       \
     (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) ||                       \
     (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) ||                  \
     (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST)) && !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_MSIRC1_MODE value specified"
#endif

#if !((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1) || \
     (STM32_CFG_MSIPLL1N_VALUE == 2) || (STM32_CFG_MSIPLL1N_VALUE == 3)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_MSIPLL1N_VALUE value specified"
#endif

#if !defined(RCC_ICSCR1_MSIS_IRC0_DIV1) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC0_DIV1 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC0_DIV2) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC0_DIV2 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC0_DIV4) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC0_DIV4 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC0_DIV8) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC0_DIV8 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC1_DIV1) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC1_DIV1 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC1_DIV2) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC1_DIV2 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC1_DIV4) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC1_DIV4 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIS_IRC1_DIV8) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIS_IRC1_DIV8 not defined"
#endif
#if !((STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV1) ||               \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV2) ||                \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV4) ||                \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV8) ||                \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) ||                \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) ||                \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) ||                \
     (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8)) &&               \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_MSIS_SRCDIV value specified"
#endif

#if !defined(RCC_ICSCR1_MSIK_IRC0_DIV1) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC0_DIV1 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC0_DIV2) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC0_DIV2 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC0_DIV4) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC0_DIV4 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC0_DIV8) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC0_DIV8 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC1_DIV1) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC1_DIV1 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC1_DIV2) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC1_DIV2 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC1_DIV4) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC1_DIV4 not defined"
#endif
#if !defined(RCC_ICSCR1_MSIK_IRC1_DIV8) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIK_IRC1_DIV8 not defined"
#endif
#if !((STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV1) ||               \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV2) ||                \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV4) ||                \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV8) ||                \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) ||                \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) ||                \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) ||                \
     (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8)) &&               \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_MSIK_SRCDIV value specified"
#endif

#if !defined(RCC_CFGR1_STOPWUCK_MSIS) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_STOPWUCK_MSIS not defined"
#endif
#if !defined(RCC_CFGR1_STOPWUCK_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_STOPWUCK_HSI16 not defined"
#endif
#if !((STM32_CFG_STOPWUCK == RCC_CFGR1_STOPWUCK_MSIS) ||                    \
     (STM32_CFG_STOPWUCK == RCC_CFGR1_STOPWUCK_HSI16)) && !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_STOPWUCK value specified"
#endif

#if !defined(RCC_CFGR1_STOPKERWUCK_MSIK) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_STOPKERWUCK_MSIK not defined"
#endif
#if !defined(RCC_CFGR1_STOPKERWUCK_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_STOPKERWUCK_HSI16 not defined"
#endif
#if !((STM32_CFG_STOPKERWUCK == RCC_CFGR1_STOPKERWUCK_MSIK) ||              \
     (STM32_CFG_STOPKERWUCK == RCC_CFGR1_STOPKERWUCK_HSI16)) &&             \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_STOPKERWUCK value specified"
#endif

#if !defined(RCC_ICSCR1_MSIBIAS_CONTINUOUS) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIBIAS_CONTINUOUS not defined"
#endif
#if !defined(RCC_ICSCR1_MSIBIAS_SAMPLING) && !defined(__DOXYGEN__)
  #error "RCC_ICSCR1_MSIBIAS_SAMPLING not defined"
#endif
#if !((STM32_CFG_MSIBIAS == RCC_ICSCR1_MSIBIAS_CONTINUOUS) ||               \
     (STM32_CFG_MSIBIAS == RCC_ICSCR1_MSIBIAS_SAMPLING)) && !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_MSIBIAS value specified"
#endif

#if !((STM32_CFG_DAC1SH_REQUIRED == TRUE) ||                                \
     (STM32_CFG_DAC1SH_REQUIRED == FALSE)) && !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_DAC1SH_REQUIRED value specified"
#endif

/**
 * @name    Frequency limits for vos1 state
 * @{
 */
#if defined(STM32_HSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_HSECLK_MIN               4000000
#define STM32_VOS1_HSECLK_MAX               50000000
#else
#define STM32_VOS1_HSECLK_MIN               4000000
#define STM32_VOS1_HSECLK_MAX               50000000
#endif
#if defined(STM32_LSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_LSECLK_MIN               5000
#define STM32_VOS1_LSECLK_MAX               40000
#else
#define STM32_VOS1_LSECLK_MIN               32768
#define STM32_VOS1_LSECLK_MAX               32768
#endif
#define STM32_VOS1_SYSCLK_MAX               96010000
#define STM32_VOS1_HCLK_MAX                 96010000
#define STM32_VOS1_PCLK1_MAX                96010000
#define STM32_VOS1_PCLK2_MAX                96010000
#define STM32_VOS1_PCLK3_MAX                96010000
#define STM32_VOS1_ADCCLK_MAX               55000000
#if ((STM32_FLASH_ACR & FLASH_ACR_LPM) == 0) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_FLASH_0WS_MAX            32005000
#else
#define STM32_VOS1_FLASH_0WS_MAX            20000000
#endif
#if ((STM32_FLASH_ACR & FLASH_ACR_LPM) == 0) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_FLASH_1WS_MAX            64007500
#else
#define STM32_VOS1_FLASH_1WS_MAX            40000000
#endif
#if ((STM32_FLASH_ACR & FLASH_ACR_LPM) == 0) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_FLASH_2WS_MAX            96010000
#else
#define STM32_VOS1_FLASH_2WS_MAX            60000000
#endif
#if ((STM32_FLASH_ACR & FLASH_ACR_LPM) == 0) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_FLASH_3WS_MAX            0
#else
#define STM32_VOS1_FLASH_3WS_MAX            80000000
#endif
#if ((STM32_FLASH_ACR & FLASH_ACR_LPM) == 0) || \
    defined(__DOXYGEN__)
#define STM32_VOS1_FLASH_4WS_MAX            0
#else
#define STM32_VOS1_FLASH_4WS_MAX            96010000
#endif
/** @} */

/**
 * @name    Frequency limits for vos2 state
 * @{
 */
#if defined(STM32_HSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS2_HSECLK_MIN               4000000
#define STM32_VOS2_HSECLK_MAX               48000000
#else
#define STM32_VOS2_HSECLK_MIN               4000000
#define STM32_VOS2_HSECLK_MAX               48000000
#endif
#if defined(STM32_LSE_BYPASS) || \
    defined(__DOXYGEN__)
#define STM32_VOS2_LSECLK_MIN               5000
#define STM32_VOS2_LSECLK_MAX               40000
#else
#define STM32_VOS2_LSECLK_MIN               32768
#define STM32_VOS2_LSECLK_MAX               32768
#endif
#define STM32_VOS2_SYSCLK_MAX               48005000
#define STM32_VOS2_HCLK_MAX                 48005000
#define STM32_VOS2_PCLK1_MAX                48005000
#define STM32_VOS2_PCLK2_MAX                48005000
#define STM32_VOS2_PCLK3_MAX                48005000
#define STM32_VOS2_ADCCLK_MAX               48005000
#define STM32_VOS2_FLASH_0WS_MAX            16001250
#define STM32_VOS2_FLASH_1WS_MAX            32002500
#define STM32_VOS2_FLASH_2WS_MAX            48005000
#define STM32_VOS2_FLASH_3WS_MAX            0
#define STM32_VOS2_FLASH_4WS_MAX            0
/** @} */

/*
 * Selected frequency limits.
 */
#if (STM32_CFG_PWR_VOSR == PWR_VOSR_RANGE1) || \
    defined(__DOXYGEN__)
#define STM32_HSECLK_MIN                    STM32_VOS1_HSECLK_MIN
#define STM32_HSECLK_MAX                    STM32_VOS1_HSECLK_MAX
#define STM32_LSECLK_MIN                    STM32_VOS1_LSECLK_MIN
#define STM32_LSECLK_MAX                    STM32_VOS1_LSECLK_MAX
#define STM32_SYSCLK_MAX                    STM32_VOS1_SYSCLK_MAX
#define STM32_HCLK_MAX                      STM32_VOS1_HCLK_MAX
#define STM32_PCLK1_MAX                     STM32_VOS1_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_VOS1_PCLK2_MAX
#define STM32_PCLK3_MAX                     STM32_VOS1_PCLK3_MAX
#define STM32_ADCCLK_MAX                    STM32_VOS1_ADCCLK_MAX
#define STM32_FLASH_0WS_MAX                 STM32_VOS1_FLASH_0WS_MAX
#define STM32_FLASH_1WS_MAX                 STM32_VOS1_FLASH_1WS_MAX
#define STM32_FLASH_2WS_MAX                 STM32_VOS1_FLASH_2WS_MAX
#define STM32_FLASH_3WS_MAX                 STM32_VOS1_FLASH_3WS_MAX
#define STM32_FLASH_4WS_MAX                 STM32_VOS1_FLASH_4WS_MAX
#elif (STM32_CFG_PWR_VOSR == PWR_VOSR_RANGE2)
#define STM32_HSECLK_MIN                    STM32_VOS2_HSECLK_MIN
#define STM32_HSECLK_MAX                    STM32_VOS2_HSECLK_MAX
#define STM32_LSECLK_MIN                    STM32_VOS2_LSECLK_MIN
#define STM32_LSECLK_MAX                    STM32_VOS2_LSECLK_MAX
#define STM32_SYSCLK_MAX                    STM32_VOS2_SYSCLK_MAX
#define STM32_HCLK_MAX                      STM32_VOS2_HCLK_MAX
#define STM32_PCLK1_MAX                     STM32_VOS2_PCLK1_MAX
#define STM32_PCLK2_MAX                     STM32_VOS2_PCLK2_MAX
#define STM32_PCLK3_MAX                     STM32_VOS2_PCLK3_MAX
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
 * @brief   AUDIOCLK clock derived enable state.
 */
#define STM32_AUDIOCLK_ENABLED              TRUE

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
 * @brief   MSIRC0 clock derived enable state.
 */
#define STM32_MSIRC0_ENABLED                (((STM32_MSIS_ENABLED == TRUE) && \
                                              ((STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV1) || \
                                               (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV2) || \
                                               (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV4) || \
                                               (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV8))) || \
                                             ((STM32_MSIK_ENABLED == TRUE) && \
                                              ((STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV1) || \
                                               (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV2) || \
                                               (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV4) || \
                                               (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV8))))

/**
 * @brief   MSIRC1 clock derived enable state.
 */
#define STM32_MSIRC1_ENABLED                (((STM32_MSIS_ENABLED == TRUE) && \
                                              ((STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) || \
                                               (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) || \
                                               (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) || \
                                               (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8))) || \
                                             ((STM32_MSIK_ENABLED == TRUE) && \
                                              ((STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) || \
                                               (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) || \
                                               (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) || \
                                               (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8))))

/**
 * @brief   MSIS clock derived enable state.
 */
#define STM32_MSIS_ENABLED                  (((STM32_SYSCLK_ENABLED == TRUE) && \
                                              (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_MSIS)) || \
                                             ((STM32_MCO1DIV_ENABLED == TRUE) && \
                                              (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_MSIS)) || \
                                             ((STM32_MCO2DIV_ENABLED == TRUE) && \
                                              (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_MSIS)))

/**
 * @brief   MSIK clock derived enable state.
 */
#define STM32_MSIK_ENABLED                  (((STM32_MCO1DIV_ENABLED == TRUE) && \
                                              (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_MSIK)) || \
                                             ((STM32_MCO2DIV_ENABLED == TRUE) && \
                                              (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_MSIK)) || \
                                             ((STM32_LPUART1_ENABLED == TRUE) && \
                                              (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_MSIK)) || \
                                             ((STM32_LPTIM1_ENABLED == TRUE) && \
                                              (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_MSIK)) || \
                                             ((STM32_LPTIM34_ENABLED == TRUE) && \
                                              (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_MSIK)) || \
                                             ((STM32_SPI1_ENABLED == TRUE) && \
                                              (STM32_CFG_SPI1_SEL == RCC_CCIPR1_SPI1SEL_MSIK)) || \
                                             ((STM32_SPI2_ENABLED == TRUE) && \
                                              (STM32_CFG_SPI2_SEL == RCC_CCIPR1_SPI2SEL_MSIK)) || \
                                             ((STM32_SPI3_ENABLED == TRUE) && \
                                              (STM32_CFG_SPI3_SEL == RCC_CCIPR2_SPI3SEL_MSIK)) || \
                                             ((STM32_OCTOSPI_ENABLED == TRUE) && \
                                              (STM32_CFG_OCTOSPI_SEL == RCC_CCIPR2_OCTOSPISEL_MSIK)) || \
                                             ((STM32_ICLK_ENABLED == TRUE) && \
                                              (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_MSIK)) || \
                                             ((STM32_I2C1_ENABLED == TRUE) && \
                                              (STM32_CFG_I2C1_SEL == RCC_CCIPR1_I2C1SEL_MSIK)) || \
                                             ((STM32_I2C2_ENABLED == TRUE) && \
                                              (STM32_CFG_I2C2_SEL == RCC_CCIPR1_I2C2SEL_MSIK)) || \
                                             ((STM32_I2C3_ENABLED == TRUE) && \
                                              (STM32_CFG_I2C3_SEL == RCC_CCIPR3_I2C3SEL_MSIK)) || \
                                             ((STM32_I3C1_ENABLED == TRUE) && \
                                              (STM32_CFG_I3C1_SEL == RCC_CCIPR1_I3C1SEL_MSIK)) || \
                                             ((STM32_I3C2_ENABLED == TRUE) && \
                                              (STM32_CFG_I3C2_SEL == RCC_CCIPR1_I3C2SEL_MSIK)) || \
                                             ((STM32_ADCDAC_ENABLED == TRUE) && \
                                              (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_MSIK)) || \
                                             ((STM32_RNG_ENABLED == TRUE) && \
                                              (STM32_CFG_RNG_SEL == RCC_CCIPR2_RNGSEL_MSIK)) || \
                                             ((STM32_FDCAN1_ENABLED == TRUE) && \
                                              (STM32_CFG_FDCAN1_SEL == RCC_CCIPR1_FDCAN1SEL_MSIK)) || \
                                             ((STM32_SAI1_ENABLED == TRUE) && \
                                              (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_MSIK)))

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
 * @brief   PCLK1TIM clock derived enable state.
 */
#define STM32_PCLK1TIM_ENABLED              TRUE

/**
 * @brief   PCLK2 clock derived enable state.
 */
#define STM32_PCLK2_ENABLED                 TRUE

/**
 * @brief   PCLK2TIM clock derived enable state.
 */
#define STM32_PCLK2TIM_ENABLED              TRUE

/**
 * @brief   PCLK3 clock derived enable state.
 */
#define STM32_PCLK3_ENABLED                 TRUE

/**
 * @brief   HSEDIV clock derived enable state.
 */
#define STM32_HSEDIV_ENABLED                (((STM32_RTC_ENABLED == TRUE) && \
                                              (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_HSEDIV)))

/**
 * @brief   HCLKDIV8 clock derived enable state.
 */
#define STM32_HCLKDIV8_ENABLED              TRUE

/**
 * @brief   MCO1DIV clock derived enable state.
 */
#define STM32_MCO1DIV_ENABLED               TRUE

/**
 * @brief   MCO1 clock derived enable state.
 */
#define STM32_MCO1_ENABLED                  TRUE

/**
 * @brief   MCO2DIV clock derived enable state.
 */
#define STM32_MCO2DIV_ENABLED               TRUE

/**
 * @brief   MCO2 clock derived enable state.
 */
#define STM32_MCO2_ENABLED                  TRUE

/**
 * @brief   RTC clock derived enable state.
 */
#define STM32_RTC_ENABLED                   TRUE

/**
 * @brief   LSCO clock derived enable state.
 */
#define STM32_LSCO_ENABLED                  TRUE

/**
 * @brief   USART1 clock derived enable state.
 */
#define STM32_USART1_ENABLED                TRUE

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
 * @brief   LPTIM1 clock derived enable state.
 */
#define STM32_LPTIM1_ENABLED                TRUE

/**
 * @brief   LPTIM2 clock derived enable state.
 */
#define STM32_LPTIM2_ENABLED                TRUE

/**
 * @brief   LPTIM34 clock derived enable state.
 */
#define STM32_LPTIM34_ENABLED               TRUE

/**
 * @brief   SPI1 clock derived enable state.
 */
#define STM32_SPI1_ENABLED                  TRUE

/**
 * @brief   SPI2 clock derived enable state.
 */
#define STM32_SPI2_ENABLED                  TRUE

/**
 * @brief   SPI3 clock derived enable state.
 */
#define STM32_SPI3_ENABLED                  TRUE

/**
 * @brief   OCTOSPI clock derived enable state.
 */
#define STM32_OCTOSPI_ENABLED               TRUE

/**
 * @brief   SYSTICK clock derived enable state.
 */
#define STM32_SYSTICK_ENABLED               TRUE

/**
 * @brief   ICLK clock derived enable state.
 */
#define STM32_ICLK_ENABLED                  TRUE

/**
 * @brief   ICLKDIV2 clock derived enable state.
 */
#define STM32_ICLKDIV2_ENABLED              TRUE

/**
 * @brief   USB clock derived enable state.
 */
#define STM32_USB_ENABLED                   TRUE

/**
 * @brief   SDMMC1 clock derived enable state.
 */
#define STM32_SDMMC1_ENABLED                TRUE

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
 * @brief   I3C1 clock derived enable state.
 */
#define STM32_I3C1_ENABLED                  TRUE

/**
 * @brief   I3C2 clock derived enable state.
 */
#define STM32_I3C2_ENABLED                  TRUE

/**
 * @brief   ADCDAC clock derived enable state.
 */
#define STM32_ADCDAC_ENABLED                TRUE

/**
 * @brief   DAC1SH clock derived enable state.
 */
#define STM32_DAC1SH_ENABLED                ((STM32_CFG_DAC1SH_REQUIRED == TRUE))

/**
 * @brief   RNG clock derived enable state.
 */
#define STM32_RNG_ENABLED                   (((HAL_USE_TRNG == TRUE) &&     \
                                              (STM32_TRNG_USE_RNG1 == TRUE)))

/**
 * @brief   FDCAN1 clock derived enable state.
 */
#define STM32_FDCAN1_ENABLED                (((HAL_USE_CAN == TRUE) &&      \
                                              (STM32_CAN_USE_FDCAN1 == TRUE)))

/**
 * @brief   SAI1 clock derived enable state.
 */
#define STM32_SAI1_ENABLED                  TRUE

/**
 * @brief   ADF1 clock derived enable state.
 */
#define STM32_ADF1_ENABLED                  TRUE

/*--- Macros and checks for the NONE clock point. --------------------------*/

/**
 * @brief   NONE clock register bits.
 */
#if (STM32_NONE_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_NONE_BITS                   0U
#else
  #define STM32_NONE_BITS                   0U
#endif

/**
 * @brief   Pseudo-clock for disabled sources clock point.
 */
#if (STM32_NONE_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_NONE_FREQ                   0U
#else
  #define STM32_NONE_FREQ                   0U
#endif

/*--- Macros and checks for the AUDIOCLK clock point. ----------------------*/

/**
 * @brief   AUDIOCLK clock register bits.
 */
#if (STM32_AUDIOCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_AUDIOCLK_BITS               0U
#else
  #define STM32_AUDIOCLK_BITS               0U
#endif

/**
 * @brief   External audio clock input clock point.
 */
#if (STM32_AUDIOCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_AUDIOCLK_FREQ               STM32_AUDIOCLK
#else
  #define STM32_AUDIOCLK_FREQ               0U
#endif

/*--- Macros and checks for the HSI16 clock point. -------------------------*/

#if !((STM32_CFG_HSI16_ENABLE == TRUE) || (STM32_CFG_HSI16_ENABLE == FALSE)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_HSI16_ENABLE value specified"
#endif

#if !defined(RCC_CFGR1_SW_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_SW_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_SYSCLK_ENABLED == TRUE) &&  \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSI16))) && !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by SYSCLK"
#endif

#if !defined(RCC_CFGR1_MCO1SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO1SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSI16))) &&               \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by MCO1DIV"
#endif

#if !defined(RCC_CFGR1_MCO2SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO2SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSI16))) &&               \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by MCO2DIV"
#endif

#if !defined(RCC_CCIPR1_USART1SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_USART1SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_USART1_ENABLED == TRUE) &&  \
      (STM32_CFG_USART1_SEL == RCC_CCIPR1_USART1SEL_HSI16))) &&             \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by USART1"
#endif

#if !defined(RCC_CCIPR1_USART3SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_USART3SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_USART3_ENABLED == TRUE) &&  \
      (STM32_CFG_USART3_SEL == RCC_CCIPR1_USART3SEL_HSI16))) &&             \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by USART3"
#endif

#if !defined(RCC_CCIPR1_UART4SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_UART4SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_UART4_ENABLED == TRUE) &&   \
      (STM32_CFG_UART4_SEL == RCC_CCIPR1_UART4SEL_HSI16))) &&               \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by UART4"
#endif

#if !defined(RCC_CCIPR1_UART5SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_UART5SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_UART5_ENABLED == TRUE) &&   \
      (STM32_CFG_UART5_SEL == RCC_CCIPR1_UART5SEL_HSI16))) &&               \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by UART5"
#endif

#if !defined(RCC_CCIPR3_LPUART1SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPUART1SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_HSI16))) &&           \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by LPUART1"
#endif

#if !defined(RCC_CCIPR3_LPTIM1SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPTIM1SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_LPTIM1_ENABLED == TRUE) &&  \
      (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_HSI16))) &&             \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by LPTIM1"
#endif

#if !defined(RCC_CCIPR1_LPTIM2SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_LPTIM2SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_LPTIM2_ENABLED == TRUE) &&  \
      (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_HSI16))) &&             \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by LPTIM2"
#endif

#if !defined(RCC_CCIPR3_LPTIM34SEL_HSI16) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPTIM34SEL_HSI16 not defined"
#endif
#if !((STM32_HSI16_ENABLED == TRUE) || !((STM32_LPTIM34_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_HSI16))) &&           \
    !defined(__DOXYGEN__)
  #error "HSI16 not enabled, required by LPTIM34"
#endif

/**
 * @brief   HSI16 clock register bits.
 */
#if (STM32_HSI16_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSI16_BITS                  (RCC_CR_HSION | RCC_CR_HSIKERON)
#else
  #define STM32_HSI16_BITS                  0U
#endif

/**
 * @brief   16MHz high speed internal oscillator clock point.
 */
#if (STM32_HSI16_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSI16_FREQ                  16000000U
#else
  #define STM32_HSI16_FREQ                  0U
#endif

/*--- Macros and checks for the HSI48 clock point. -------------------------*/

#if !((STM32_CFG_HSI48_ENABLE == TRUE) || (STM32_CFG_HSI48_ENABLE == FALSE)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_HSI48_ENABLE value specified"
#endif

#if !defined(RCC_CFGR1_MCO1SEL_HSI48) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO1SEL_HSI48 not defined"
#endif
#if !((STM32_HSI48_ENABLED == TRUE) || !((STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSI48))) &&               \
    !defined(__DOXYGEN__)
  #error "HSI48 not enabled, required by MCO1DIV"
#endif

#if !defined(RCC_CFGR1_MCO2SEL_HSI48) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO2SEL_HSI48 not defined"
#endif
#if !((STM32_HSI48_ENABLED == TRUE) || !((STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSI48))) &&               \
    !defined(__DOXYGEN__)
  #error "HSI48 not enabled, required by MCO2DIV"
#endif

#if !defined(RCC_CCIPR1_ICLKSEL_HSI48) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_ICLKSEL_HSI48 not defined"
#endif
#if !((STM32_HSI48_ENABLED == TRUE) || !((STM32_ICLK_ENABLED == TRUE) &&    \
      (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_HSI48))) && !defined(__DOXYGEN__)
  #error "HSI48 not enabled, required by ICLK"
#endif

#if !defined(RCC_CCIPR2_RNGSEL_HSI48) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_RNGSEL_HSI48 not defined"
#endif
#if !((STM32_HSI48_ENABLED == TRUE) || !((STM32_RNG_ENABLED == TRUE) &&     \
      (STM32_CFG_RNG_SEL == RCC_CCIPR2_RNGSEL_HSI48))) && !defined(__DOXYGEN__)
  #error "HSI48 not enabled, required by RNG"
#endif

/**
 * @brief   HSI48 clock register bits.
 */
#if (STM32_HSI48_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSI48_BITS                  RCC_CR_HSI48ON
#else
  #define STM32_HSI48_BITS                  0U
#endif

/**
 * @brief   48MHz high speed internal oscillator clock point.
 */
#if (STM32_HSI48_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSI48_FREQ                  48000000U
#else
  #define STM32_HSI48_FREQ                  0U
#endif

/*--- Macros and checks for the HSE clock point. ---------------------------*/

#if !((STM32_CFG_HSE_ENABLE == TRUE) || (STM32_CFG_HSE_ENABLE == FALSE)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_HSE_ENABLE value specified"
#endif

#if !defined(RCC_CFGR1_SW_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_SW_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_SYSCLK_ENABLED == TRUE) &&    \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSE))) && !defined(__DOXYGEN__)
  #error "HSE not enabled, required by SYSCLK"
#endif

#if !defined(STM32_HSEDIV_ENABLED) && !defined(__DOXYGEN__)
  #error "STM32_HSEDIV_ENABLED not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_HSEDIV_ENABLED == TRUE))) &&  \
    !defined(__DOXYGEN__)
  #error "HSE not enabled, required by HSEDIV"
#endif

#if !defined(RCC_CFGR1_MCO1SEL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO1SEL_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_MCO1DIV_ENABLED == TRUE) &&   \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSE))) && !defined(__DOXYGEN__)
  #error "HSE not enabled, required by MCO1DIV"
#endif

#if !defined(RCC_CFGR1_MCO2SEL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO2SEL_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_MCO2DIV_ENABLED == TRUE) &&   \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSE))) && !defined(__DOXYGEN__)
  #error "HSE not enabled, required by MCO2DIV"
#endif

#if !defined(RCC_CCIPR1_ICLKSEL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_ICLKSEL_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_ICLK_ENABLED == TRUE) &&      \
      (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_HSE))) && !defined(__DOXYGEN__)
  #error "HSE not enabled, required by ICLK"
#endif

#if !defined(RCC_CCIPR2_ADCDACSEL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_ADCDACSEL_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_ADCDAC_ENABLED == TRUE) &&    \
      (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HSE))) &&               \
    !defined(__DOXYGEN__)
  #error "HSE not enabled, required by ADCDAC"
#endif

#if !defined(RCC_CCIPR2_SAI1SEL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_SAI1SEL_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_SAI1_ENABLED == TRUE) &&      \
      (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_HSE))) && !defined(__DOXYGEN__)
  #error "HSE not enabled, required by SAI1"
#endif

#if !defined(RCC_CCIPR2_ADF1SEL_HSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_ADF1SEL_HSE not defined"
#endif
#if !((STM32_HSE_ENABLED == TRUE) || !((STM32_ADF1_ENABLED == TRUE) &&      \
      (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_HSE))) && !defined(__DOXYGEN__)
  #error "HSE not enabled, required by ADF1"
#endif

/**
 * @brief   HSE clock register bits.
 */
#if (STM32_HSE_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSE_BITS                    RCC_CR_HSEON
#else
  #define STM32_HSE_BITS                    0U
#endif

/**
 * @brief   High frequency external oscillator clock point.
 */
#if (STM32_HSE_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSE_FREQ                    STM32_HSECLK
#else
  #define STM32_HSE_FREQ                    0U
#endif

#if !((STM32_HSE_ENABLED != TRUE) || (STM32_HSE_FREQ >= STM32_HSECLK_MIN)) && \
    !defined(__DOXYGEN__)
  #error "STM32_HSE_FREQ below minimum frequency"
#endif

#if !((STM32_HSE_ENABLED != TRUE) || (STM32_HSE_FREQ <= STM32_HSECLK_MAX)) && \
    !defined(__DOXYGEN__)
  #error "STM32_HSE_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the LSE clock point. ---------------------------*/

#if !((STM32_CFG_LSE_ENABLE == TRUE) || (STM32_CFG_LSE_ENABLE == FALSE)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_LSE_ENABLE value specified"
#endif

#if !defined(RCC_CFGR1_MCO1SEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO1SEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_MCO1DIV_ENABLED == TRUE) &&   \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_LSE))) && !defined(__DOXYGEN__)
  #error "LSE not enabled, required by MCO1DIV"
#endif

#if !defined(RCC_CFGR1_MCO2SEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO2SEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_MCO2DIV_ENABLED == TRUE) &&   \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_LSE))) && !defined(__DOXYGEN__)
  #error "LSE not enabled, required by MCO2DIV"
#endif

#if !defined(RCC_BDCR_RTCSEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_BDCR_RTCSEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_RTC_ENABLED == TRUE) &&       \
      (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_LSE))) && !defined(__DOXYGEN__)
  #error "LSE not enabled, required by RTC"
#endif

#if !defined(RCC_BDCR_LSCOSEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_BDCR_LSCOSEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_LSCO_ENABLED == TRUE) &&      \
      (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_LSE))) && !defined(__DOXYGEN__)
  #error "LSE not enabled, required by LSCO"
#endif

#if !defined(RCC_CCIPR3_LPUART1SEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPUART1SEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_LPUART1_ENABLED == TRUE) &&   \
      (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_LSE))) &&             \
    !defined(__DOXYGEN__)
  #error "LSE not enabled, required by LPUART1"
#endif

#if !defined(RCC_CCIPR3_LPTIM1SEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPTIM1SEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_LPTIM1_ENABLED == TRUE) &&    \
      (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_LSE))) &&               \
    !defined(__DOXYGEN__)
  #error "LSE not enabled, required by LPTIM1"
#endif

#if !defined(RCC_CCIPR1_LPTIM2SEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_LPTIM2SEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_LPTIM2_ENABLED == TRUE) &&    \
      (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_LSE))) &&               \
    !defined(__DOXYGEN__)
  #error "LSE not enabled, required by LPTIM2"
#endif

#if !defined(RCC_CCIPR3_LPTIM34SEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPTIM34SEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_LPTIM34_ENABLED == TRUE) &&   \
      (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_LSE))) &&             \
    !defined(__DOXYGEN__)
  #error "LSE not enabled, required by LPTIM34"
#endif

#if !defined(RCC_CCIPR1_SYSTICKSEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_SYSTICKSEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_SYSTICK_ENABLED == TRUE) &&   \
      (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_LSE))) &&             \
    !defined(__DOXYGEN__)
  #error "LSE not enabled, required by SYSTICK"
#endif

#if !defined(RCC_CCIPR2_DAC1SHSEL_LSE) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_DAC1SHSEL_LSE not defined"
#endif
#if !((STM32_LSE_ENABLED == TRUE) || !((STM32_DAC1SH_ENABLED == TRUE) &&    \
      (STM32_CFG_DAC1SH_SEL == RCC_CCIPR2_DAC1SHSEL_LSE))) &&               \
    !defined(__DOXYGEN__)
  #error "LSE not enabled, required by DAC1SH"
#endif

/**
 * @brief   LSE clock register bits.
 */
#if (STM32_LSE_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_LSE_BITS                    RCC_BDCR_LSEON
#else
  #define STM32_LSE_BITS                    0U
#endif

/**
 * @brief   Low frequency external oscillator clock point.
 */
#if (STM32_LSE_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_LSE_FREQ                    STM32_LSECLK
#else
  #define STM32_LSE_FREQ                    0U
#endif

#if !((STM32_LSE_ENABLED != TRUE) || (STM32_LSE_FREQ >= STM32_LSECLK_MIN)) && \
    !defined(__DOXYGEN__)
  #error "STM32_LSE_FREQ below minimum frequency"
#endif

#if !((STM32_LSE_ENABLED != TRUE) || (STM32_LSE_FREQ <= STM32_LSECLK_MAX)) && \
    !defined(__DOXYGEN__)
  #error "STM32_LSE_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the LSI clock point. ---------------------------*/

#if !((STM32_CFG_LSI_ENABLE == TRUE) || (STM32_CFG_LSI_ENABLE == FALSE)) && \
    !defined(__DOXYGEN__)
  #error "invalid STM32_CFG_LSI_ENABLE value specified"
#endif

#if !defined(RCC_CFGR1_MCO1SEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO1SEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_MCO1DIV_ENABLED == TRUE) &&   \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_LSI))) && !defined(__DOXYGEN__)
  #error "LSI not enabled, required by MCO1DIV"
#endif

#if !defined(RCC_CFGR1_MCO2SEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_MCO2SEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_MCO2DIV_ENABLED == TRUE) &&   \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_LSI))) && !defined(__DOXYGEN__)
  #error "LSI not enabled, required by MCO2DIV"
#endif

#if !defined(RCC_BDCR_RTCSEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_BDCR_RTCSEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_RTC_ENABLED == TRUE) &&       \
      (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_LSI))) && !defined(__DOXYGEN__)
  #error "LSI not enabled, required by RTC"
#endif

#if !defined(RCC_BDCR_LSCOSEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_BDCR_LSCOSEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_LSCO_ENABLED == TRUE) &&      \
      (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_LSI))) && !defined(__DOXYGEN__)
  #error "LSI not enabled, required by LSCO"
#endif

#if !defined(RCC_CCIPR3_LPTIM1SEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPTIM1SEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_LPTIM1_ENABLED == TRUE) &&    \
      (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_LSI))) &&               \
    !defined(__DOXYGEN__)
  #error "LSI not enabled, required by LPTIM1"
#endif

#if !defined(RCC_CCIPR1_LPTIM2SEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_LPTIM2SEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_LPTIM2_ENABLED == TRUE) &&    \
      (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_LSI))) &&               \
    !defined(__DOXYGEN__)
  #error "LSI not enabled, required by LPTIM2"
#endif

#if !defined(RCC_CCIPR3_LPTIM34SEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR3_LPTIM34SEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_LPTIM34_ENABLED == TRUE) &&   \
      (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_LSI))) &&             \
    !defined(__DOXYGEN__)
  #error "LSI not enabled, required by LPTIM34"
#endif

#if !defined(RCC_CCIPR1_SYSTICKSEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR1_SYSTICKSEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_SYSTICK_ENABLED == TRUE) &&   \
      (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_LSI))) &&             \
    !defined(__DOXYGEN__)
  #error "LSI not enabled, required by SYSTICK"
#endif

#if !defined(RCC_CCIPR2_DAC1SHSEL_LSI) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_DAC1SHSEL_LSI not defined"
#endif
#if !((STM32_LSI_ENABLED == TRUE) || !((STM32_DAC1SH_ENABLED == TRUE) &&    \
      (STM32_CFG_DAC1SH_SEL == RCC_CCIPR2_DAC1SHSEL_LSI))) &&               \
    !defined(__DOXYGEN__)
  #error "LSI not enabled, required by DAC1SH"
#endif

/**
 * @brief   LSI clock register bits.
 */
#if (STM32_LSI_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_LSI_BITS                    RCC_CSR_LSION
#else
  #define STM32_LSI_BITS                    0U
#endif

/**
 * @brief   Low frequency internal oscillator clock point.
 */
#if (STM32_LSI_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_LSI_FREQ                    STM32_LSICLK
#else
  #define STM32_LSI_FREQ                    0U
#endif

/*--- Macros and checks for the MSIRC0 clock point. ------------------------*/

/**
 * @brief   MSIRC0 clock register bits.
 */
#if (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || \
    defined(__DOXYGEN__)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_CR_BITS            RCC_CR_MSIPLL0EN
  #else
    #define STM32_MSIRC0_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_CR_BITS            RCC_CR_MSIPLL0EN
  #else
    #define STM32_MSIRC0_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_CR_BITS            (RCC_CR_MSIPLL0EN |             \
                                             RCC_CR_MSIPLL0FAST)
  #else
    #define STM32_MSIRC0_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE_FAST)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_CR_BITS            (RCC_CR_MSIPLL0EN |             \
                                             RCC_CR_MSIPLL0FAST)
  #else
    #define STM32_MSIRC0_CR_BITS            0U
  #endif
#else
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_CR_BITS            0U
  #else
    #define STM32_MSIRC0_CR_BITS            0U
  #endif
#endif

#if (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || \
    defined(__DOXYGEN__)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_ICSCR1_BITS        RCC_ICSCR1_MSIPLL0SEL_LSE
  #else
    #define STM32_MSIRC0_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL0SEL_HSE |    \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC0_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_ICSCR1_BITS        RCC_ICSCR1_MSIPLL0SEL_LSE
  #else
    #define STM32_MSIRC0_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE_FAST)
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL0SEL_HSE |    \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC0_ICSCR1_BITS        0U
  #endif
#else
  #if (STM32_MSIRC0_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC0_ICSCR1_BITS        0U
  #else
    #define STM32_MSIRC0_ICSCR1_BITS        0U
  #endif
#endif

/**
 * @brief   MSI 96MHz internal oscillator clock point.
 */
#if (STM32_MSIRC0_ENABLED == FALSE) && !defined(__DOXYGEN__)
  #define STM32_MSIRC0_FREQ                 0U
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || \
    defined(__DOXYGEN__)
  #define STM32_MSIRC0_FREQ                 96010000U
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE)
  #define STM32_MSIRC0_FREQ                 96000000U
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST)
  #define STM32_MSIRC0_FREQ                 96010000U
#elif (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_HSE_FAST)
  #define STM32_MSIRC0_FREQ                 96000000U
#else
  #define STM32_MSIRC0_FREQ                 96000000U
#endif

/*--- Macros and checks for the MSIRC1 clock point. ------------------------*/

/**
 * @brief   MSIRC1 clock register bits.
 */
#if ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
     ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))) || \
    defined(__DOXYGEN__)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            RCC_CR_MSIPLL1EN
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            (RCC_CR_MSIPLL1EN |             \
                                             RCC_CR_MSIPLL1FAST)
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#else
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_CR_BITS            0U
  #else
    #define STM32_MSIRC1_CR_BITS            0U
  #endif
#endif

#if ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
     ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))) || \
    defined(__DOXYGEN__)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_LSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE))
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST)
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        (RCC_ICSCR1_MSIPLL1SEL_HSE |    \
                                             RCC_ICSCR1_MSIPLL1N_FIELD(STM32_CFG_MSIPLL1N_VALUE) | \
                                             STM32_MSIHSINDIV)
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#else
  #if (STM32_MSIRC1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #else
    #define STM32_MSIRC1_ICSCR1_BITS        0U
  #endif
#endif

/**
 * @brief   MSI 24MHz internal oscillator clock point.
 */
#if (STM32_MSIRC1_ENABLED == FALSE) && !defined(__DOXYGEN__)
  #define STM32_MSIRC1_FREQ                 0U
#elif ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))) || \
    defined(__DOXYGEN__)
  #define STM32_MSIRC1_FREQ                 23986000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #define STM32_MSIRC1_FREQ                 22577000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #define STM32_MSIRC1_FREQ                 24576000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE)
  #define STM32_MSIRC1_FREQ                 24000000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #define STM32_MSIRC1_FREQ                 24016000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #define STM32_MSIRC1_FREQ                 22581000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #define STM32_MSIRC1_FREQ                 24577000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE)
  #define STM32_MSIRC1_FREQ                 24000000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #define STM32_MSIRC1_FREQ                 23986000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #define STM32_MSIRC1_FREQ                 22577000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #define STM32_MSIRC1_FREQ                 24576000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST)
  #define STM32_MSIRC1_FREQ                 24000000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1))
  #define STM32_MSIRC1_FREQ                 24016000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2)
  #define STM32_MSIRC1_FREQ                 22581000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3)
  #define STM32_MSIRC1_FREQ                 24577000U
#elif (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST)
  #define STM32_MSIRC1_FREQ                 24000000U
#else
  #define STM32_MSIRC1_FREQ                 24000000U
#endif

/*--- Macros and checks for the MSIS clock point. --------------------------*/

/**
 * @brief   MSIS clock register bits.
 */
#if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_MSIS_CR_BITS                RCC_CR_MSISON
#else
  #define STM32_MSIS_CR_BITS                0U
#endif

#if ((STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV1) && \
     ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))) || \
    defined(__DOXYGEN__)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV1)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV2) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV2)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV4) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV4)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV8) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV8)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(0U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(1U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4)
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(2U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#else
  #if (STM32_MSIS_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIS_ICSCR1_BITS          (RCC_ICSCR1_MSISSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSISDIV_FIELD(3U))
  #else
    #define STM32_MSIS_ICSCR1_BITS          0U
  #endif
#endif

/**
 * @brief   MSI system oscillator clock point.
 */
#if (STM32_MSIS_ENABLED == FALSE) && !defined(__DOXYGEN__)
  #define STM32_MSIS_FREQ                   0U
#elif ((STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV1) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))) || \
    defined(__DOXYGEN__)
  #define STM32_MSIS_FREQ                   96010000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV1)
  #define STM32_MSIS_FREQ                   96000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV2) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   48005000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV2)
  #define STM32_MSIS_FREQ                   48000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV4) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   24002500U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV4)
  #define STM32_MSIS_FREQ                   24000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV8) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   12001250U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC0_DIV8)
  #define STM32_MSIS_FREQ                   12000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   23986000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   24016000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   22577000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   22581000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   24576000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   24577000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV1)
  #define STM32_MSIS_FREQ                   24000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   11993000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   12008000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   11288500U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   11290500U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   12288000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   12288500U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV2)
  #define STM32_MSIS_FREQ                   12000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   5996500U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   6004000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   5644250U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   5645250U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   6144000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   6144250U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV4)
  #define STM32_MSIS_FREQ                   6000000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   2998250U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   3002000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   2822125U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   2822625U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIS_FREQ                   3072000U
#elif (STM32_CFG_MSIS_SRCDIV == RCC_ICSCR1_MSIS_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIS_FREQ                   3072125U
#else
  #define STM32_MSIS_FREQ                   3000000U
#endif

/*--- Macros and checks for the MSIK clock point. --------------------------*/

/**
 * @brief   MSIK clock register bits.
 */
#if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_MSIK_CR_BITS                (RCC_CR_MSIKON | RCC_CR_MSIKERON)
#else
  #define STM32_MSIK_CR_BITS                0U
#endif

#if ((STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV1) && \
     ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))) || \
    defined(__DOXYGEN__)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV1)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV2) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV2)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV4) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV4)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV8) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV8)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC0 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(0U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(1U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4)
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(2U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#else
  #if (STM32_MSIK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MSIK_ICSCR1_BITS          (RCC_ICSCR1_MSIKSEL_MSIRC1 |    \
                                             RCC_ICSCR1_MSIKDIV_FIELD(3U))
  #else
    #define STM32_MSIK_ICSCR1_BITS          0U
  #endif
#endif

/**
 * @brief   MSI kernel oscillator clock point.
 */
#if (STM32_MSIK_ENABLED == FALSE) && !defined(__DOXYGEN__)
  #define STM32_MSIK_FREQ                   0U
#elif ((STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV1) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))) || \
    defined(__DOXYGEN__)
  #define STM32_MSIK_FREQ                   96010000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV1)
  #define STM32_MSIK_FREQ                   96000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV2) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   48005000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV2)
  #define STM32_MSIK_FREQ                   48000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV4) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   24002500U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV4)
  #define STM32_MSIK_FREQ                   24000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV8) && \
      ((STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE) || (STM32_CFG_MSIRC0_MODE == RCC_MSIRC0_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   12001250U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC0_DIV8)
  #define STM32_MSIK_FREQ                   12000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   23986000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   24016000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   22577000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   22581000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   24576000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   24577000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV1)
  #define STM32_MSIK_FREQ                   24000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   11993000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   12008000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   11288500U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   11290500U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   12288000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   12288500U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV2)
  #define STM32_MSIK_FREQ                   12000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   5996500U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   6004000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   5644250U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   5645250U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   6144000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   6144250U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV4)
  #define STM32_MSIK_FREQ                   6000000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   2998250U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      ((STM32_CFG_MSIPLL1N_VALUE == 0) || (STM32_CFG_MSIPLL1N_VALUE == 1)) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   3002000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   2822125U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 2) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   2822625U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_LSE_FAST))
  #define STM32_MSIK_FREQ                   3072000U
#elif (STM32_CFG_MSIK_SRCDIV == RCC_ICSCR1_MSIK_IRC1_DIV8) && \
      (STM32_CFG_MSIPLL1N_VALUE == 3) && \
      ((STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE) || (STM32_CFG_MSIRC1_MODE == RCC_MSIRC1_PLL_HSE_FAST))
  #define STM32_MSIK_FREQ                   3072125U
#else
  #define STM32_MSIK_FREQ                   3000000U
#endif

/*--- Macros and checks for the SYSCLK clock point. ------------------------*/

/**
 * @brief   SYSCLK clock register bits.
 */
#if (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_MSIS) || defined(__DOXYGEN__)
  #define STM32_SYSCLK_BITS                 RCC_CFGR1_SW_MSIS
#elif (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSI16)
  #define STM32_SYSCLK_BITS                 RCC_CFGR1_SW_HSI16
#elif (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSE)
  #define STM32_SYSCLK_BITS                 RCC_CFGR1_SW_HSE
#else
  #error "invalid STM32_CFG_SYSCLK_SEL value specified"
#endif

/**
 * @brief   System clock clock point.
 */
#if ((STM32_SYSCLK_ENABLED == TRUE) && \
     (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_MSIS)) || \
    defined(__DOXYGEN__)
  #define STM32_SYSCLK_FREQ                 STM32_MSIS_FREQ
#elif (STM32_SYSCLK_ENABLED == TRUE) && \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSI16)
  #define STM32_SYSCLK_FREQ                 STM32_HSI16_FREQ
#elif (STM32_SYSCLK_ENABLED == TRUE) && \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSE)
  #define STM32_SYSCLK_FREQ                 STM32_HSE_FREQ
#else
  #define STM32_SYSCLK_FREQ                 0U
#endif

#if !defined(RCC_CFGR1_SW_MSIS) && !defined(__DOXYGEN__)
  #error "RCC_CFGR1_SW_MSIS not defined"
#endif
#if !(!((STM32_SYSCLK_ENABLED == TRUE) &&                                   \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_MSIS)) ||                       \
     (STM32_MSIS_FREQ <= STM32_SYSCLK_MAX)) && !defined(__DOXYGEN__)
  #error "STM32_SYSCLK_FREQ above maximum frequency"
#endif

#if !(!((STM32_SYSCLK_ENABLED == TRUE) &&                                   \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSI16)) ||                      \
     (STM32_HSI16_FREQ <= STM32_SYSCLK_MAX)) && !defined(__DOXYGEN__)
  #error "STM32_SYSCLK_FREQ above maximum frequency"
#endif

#if !(!((STM32_SYSCLK_ENABLED == TRUE) &&                                   \
      (STM32_CFG_SYSCLK_SEL == RCC_CFGR1_SW_HSE)) ||                        \
     (STM32_HSE_FREQ <= STM32_SYSCLK_MAX)) && !defined(__DOXYGEN__)
  #error "STM32_SYSCLK_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the HCLK clock point. --------------------------*/

/**
 * @brief   HCLK clock register bits.
 */
#if (STM32_CFG_HCLK_VALUE == 1) || defined(__DOXYGEN__)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV1
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 2)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV2
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 4)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV4
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 8)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV8
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 16)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV16
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 64)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV64
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 128)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV128
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 256)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV256
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#elif (STM32_CFG_HCLK_VALUE == 512)
  #if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_HCLK_BITS                 RCC_CFGR2_HPRE_DIV512
  #else
    #define STM32_HCLK_BITS                 0U
  #endif
#else
  #error "invalid STM32_CFG_HCLK_VALUE value specified"
#endif

/**
 * @brief   AHB clock clock point.
 */
#if (STM32_HCLK_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HCLK_FREQ                   (STM32_SYSCLK_FREQ /            \
                                             STM32_CFG_HCLK_VALUE)
#else
  #define STM32_HCLK_FREQ                   0U
#endif

#if !((STM32_HCLK_ENABLED != TRUE) || (STM32_HCLK_FREQ <= STM32_HCLK_MAX)) && \
    !defined(__DOXYGEN__)
  #error "STM32_HCLK_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PCLK1 clock point. -------------------------*/

/**
 * @brief   PCLK1 clock register bits.
 */
#if (STM32_CFG_PCLK1_VALUE == 1) || defined(__DOXYGEN__)
  #if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK1_BITS                RCC_CFGR2_PPRE1_DIV1
  #else
    #define STM32_PCLK1_BITS                0U
  #endif
#elif (STM32_CFG_PCLK1_VALUE == 2)
  #if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK1_BITS                RCC_CFGR2_PPRE1_DIV2
  #else
    #define STM32_PCLK1_BITS                0U
  #endif
#elif (STM32_CFG_PCLK1_VALUE == 4)
  #if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK1_BITS                RCC_CFGR2_PPRE1_DIV4
  #else
    #define STM32_PCLK1_BITS                0U
  #endif
#elif (STM32_CFG_PCLK1_VALUE == 8)
  #if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK1_BITS                RCC_CFGR2_PPRE1_DIV8
  #else
    #define STM32_PCLK1_BITS                0U
  #endif
#elif (STM32_CFG_PCLK1_VALUE == 16)
  #if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK1_BITS                RCC_CFGR2_PPRE1_DIV16
  #else
    #define STM32_PCLK1_BITS                0U
  #endif
#else
  #error "invalid STM32_CFG_PCLK1_VALUE value specified"
#endif

/**
 * @brief   APB1 clock clock point.
 */
#if (STM32_PCLK1_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK1_FREQ                  (STM32_HCLK_FREQ /              \
                                             STM32_CFG_PCLK1_VALUE)
#else
  #define STM32_PCLK1_FREQ                  0U
#endif

#if !((STM32_PCLK1_ENABLED != TRUE) || (STM32_PCLK1_FREQ <= STM32_PCLK1_MAX)) && \
    !defined(__DOXYGEN__)
  #error "STM32_PCLK1_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PCLK1TIM clock point. ----------------------*/

/**
 * @brief   PCLK1TIM clock register bits.
 */
#if (STM32_PCLK1TIM_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK1TIM_BITS               0U
#else
  #define STM32_PCLK1TIM_BITS               0U
#endif

/**
 * @brief   APB1 timer clock clock point.
 */
#if (STM32_PCLK1TIM_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK1TIM_FREQ               ((STM32_CFG_PCLK1_VALUE == 1) ? STM32_PCLK1_FREQ : \
                                             (STM32_PCLK1_FREQ * 2U))
#else
  #define STM32_PCLK1TIM_FREQ               0U
#endif

/*--- Macros and checks for the PCLK2 clock point. -------------------------*/

/**
 * @brief   PCLK2 clock register bits.
 */
#if (STM32_CFG_PCLK2_VALUE == 1) || defined(__DOXYGEN__)
  #if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK2_BITS                RCC_CFGR2_PPRE2_DIV1
  #else
    #define STM32_PCLK2_BITS                0U
  #endif
#elif (STM32_CFG_PCLK2_VALUE == 2)
  #if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK2_BITS                RCC_CFGR2_PPRE2_DIV2
  #else
    #define STM32_PCLK2_BITS                0U
  #endif
#elif (STM32_CFG_PCLK2_VALUE == 4)
  #if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK2_BITS                RCC_CFGR2_PPRE2_DIV4
  #else
    #define STM32_PCLK2_BITS                0U
  #endif
#elif (STM32_CFG_PCLK2_VALUE == 8)
  #if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK2_BITS                RCC_CFGR2_PPRE2_DIV8
  #else
    #define STM32_PCLK2_BITS                0U
  #endif
#elif (STM32_CFG_PCLK2_VALUE == 16)
  #if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK2_BITS                RCC_CFGR2_PPRE2_DIV16
  #else
    #define STM32_PCLK2_BITS                0U
  #endif
#else
  #error "invalid STM32_CFG_PCLK2_VALUE value specified"
#endif

/**
 * @brief   APB2 clock clock point.
 */
#if (STM32_PCLK2_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK2_FREQ                  (STM32_HCLK_FREQ /              \
                                             STM32_CFG_PCLK2_VALUE)
#else
  #define STM32_PCLK2_FREQ                  0U
#endif

#if !((STM32_PCLK2_ENABLED != TRUE) || (STM32_PCLK2_FREQ <= STM32_PCLK2_MAX)) && \
    !defined(__DOXYGEN__)
  #error "STM32_PCLK2_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the PCLK2TIM clock point. ----------------------*/

/**
 * @brief   PCLK2TIM clock register bits.
 */
#if (STM32_PCLK2TIM_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK2TIM_BITS               0U
#else
  #define STM32_PCLK2TIM_BITS               0U
#endif

/**
 * @brief   APB2 timer clock clock point.
 */
#if (STM32_PCLK2TIM_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK2TIM_FREQ               ((STM32_CFG_PCLK2_VALUE == 1) ? STM32_PCLK2_FREQ : \
                                             (STM32_PCLK2_FREQ * 2U))
#else
  #define STM32_PCLK2TIM_FREQ               0U
#endif

/*--- Macros and checks for the PCLK3 clock point. -------------------------*/

/**
 * @brief   PCLK3 clock register bits.
 */
#if (STM32_CFG_PCLK3_VALUE == 1) || defined(__DOXYGEN__)
  #if (STM32_PCLK3_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK3_BITS                RCC_CFGR3_PPRE3_DIV1
  #else
    #define STM32_PCLK3_BITS                0U
  #endif
#elif (STM32_CFG_PCLK3_VALUE == 2)
  #if (STM32_PCLK3_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK3_BITS                RCC_CFGR3_PPRE3_DIV2
  #else
    #define STM32_PCLK3_BITS                0U
  #endif
#elif (STM32_CFG_PCLK3_VALUE == 4)
  #if (STM32_PCLK3_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK3_BITS                RCC_CFGR3_PPRE3_DIV4
  #else
    #define STM32_PCLK3_BITS                0U
  #endif
#elif (STM32_CFG_PCLK3_VALUE == 8)
  #if (STM32_PCLK3_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK3_BITS                RCC_CFGR3_PPRE3_DIV8
  #else
    #define STM32_PCLK3_BITS                0U
  #endif
#elif (STM32_CFG_PCLK3_VALUE == 16)
  #if (STM32_PCLK3_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_PCLK3_BITS                RCC_CFGR3_PPRE3_DIV16
  #else
    #define STM32_PCLK3_BITS                0U
  #endif
#else
  #error "invalid STM32_CFG_PCLK3_VALUE value specified"
#endif

/**
 * @brief   APB3 clock clock point.
 */
#if (STM32_PCLK3_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_PCLK3_FREQ                  (STM32_HCLK_FREQ /              \
                                             STM32_CFG_PCLK3_VALUE)
#else
  #define STM32_PCLK3_FREQ                  0U
#endif

#if !((STM32_PCLK3_ENABLED != TRUE) || (STM32_PCLK3_FREQ <= STM32_PCLK3_MAX)) && \
    !defined(__DOXYGEN__)
  #error "STM32_PCLK3_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the HSEDIV clock point. ------------------------*/

/**
 * @brief   HSEDIV clock register bits.
 */
#if (STM32_HSEDIV_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSEDIV_BITS                 0U
#else
  #define STM32_HSEDIV_BITS                 0U
#endif

/**
 * @brief   HSE divided clock clock point.
 */
#if (STM32_HSEDIV_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HSEDIV_FREQ                 (STM32_HSE_FREQ / 32U)
#else
  #define STM32_HSEDIV_FREQ                 0U
#endif

/*--- Macros and checks for the HCLKDIV8 clock point. ----------------------*/

/**
 * @brief   HCLKDIV8 clock register bits.
 */
#if (STM32_HCLKDIV8_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HCLKDIV8_BITS               0U
#else
  #define STM32_HCLKDIV8_BITS               0U
#endif

/**
 * @brief   HCLK divided by 8 clock point.
 */
#if (STM32_HCLKDIV8_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_HCLKDIV8_FREQ               (hal_lld_get_clock_point(CLK_HCLK) / \
                                             8U)
#else
  #define STM32_HCLKDIV8_FREQ               0U
#endif

/*--- Macros and checks for the MCO1DIV clock point. -----------------------*/

/**
 * @brief   MCO1DIV clock register bits.
 */
#if (STM32_MCO1DIV_ENABLED == FALSE)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_OFF
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_OFF) || defined(__DOXYGEN__)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_OFF
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_SYSCLK)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_SYSCLK
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_MSIS)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_MSIS
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSI16)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_HSI16
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSE)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_HSE
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_LSI)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_LSI
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_LSE)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_LSE
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSI48)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_HSI48
#elif (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_MSIK)
  #define STM32_MCO1DIV_BITS                RCC_CFGR1_MCO1SEL_MSIK
#else
  #error "invalid STM32_CFG_MCO1DIV_SEL value specified"
#endif

/**
 * @brief   MCO1 selected source before prescaler clock point.
 */
#if ((STM32_MCO1DIV_ENABLED == TRUE) && \
     (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_OFF)) || \
    defined(__DOXYGEN__)
  #define STM32_MCO1DIV_FREQ                STM32_NONE_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_SYSCLK)
  #define STM32_MCO1DIV_FREQ                STM32_SYSCLK_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_MSIS)
  #define STM32_MCO1DIV_FREQ                STM32_MSIS_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSI16)
  #define STM32_MCO1DIV_FREQ                STM32_HSI16_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSE)
  #define STM32_MCO1DIV_FREQ                STM32_HSE_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_LSI)
  #define STM32_MCO1DIV_FREQ                STM32_LSI_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_LSE)
  #define STM32_MCO1DIV_FREQ                STM32_LSE_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_HSI48)
  #define STM32_MCO1DIV_FREQ                STM32_HSI48_FREQ
#elif (STM32_MCO1DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO1DIV_SEL == RCC_CFGR1_MCO1SEL_MSIK)
  #define STM32_MCO1DIV_FREQ                STM32_MSIK_FREQ
#else
  #define STM32_MCO1DIV_FREQ                0U
#endif

/*--- Macros and checks for the MCO1 clock point. --------------------------*/

/**
 * @brief   MCO1 clock register bits.
 */
#if (STM32_CFG_MCO1_VALUE == 1) || defined(__DOXYGEN__)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV1
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 2)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV2
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 4)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV4
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 8)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV8
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 16)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV16
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 32)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV32
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 64)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV64
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#elif (STM32_CFG_MCO1_VALUE == 128)
  #if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO1_BITS                 RCC_CFGR1_MCO1PRE_DIV128
  #else
    #define STM32_MCO1_BITS                 0U
  #endif
#else
  #error "invalid STM32_CFG_MCO1_VALUE value specified"
#endif

/**
 * @brief   MCO1 output pin clock point.
 */
#if (STM32_MCO1_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_MCO1_FREQ                   (STM32_MCO1DIV_FREQ /           \
                                             STM32_CFG_MCO1_VALUE)
#else
  #define STM32_MCO1_FREQ                   0U
#endif

/*--- Macros and checks for the MCO2DIV clock point. -----------------------*/

/**
 * @brief   MCO2DIV clock register bits.
 */
#if (STM32_MCO2DIV_ENABLED == FALSE)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_OFF
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_OFF) || defined(__DOXYGEN__)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_OFF
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_SYSCLK)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_SYSCLK
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_MSIS)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_MSIS
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSI16)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_HSI16
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSE)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_HSE
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_LSI)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_LSI
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_LSE)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_LSE
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSI48)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_HSI48
#elif (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_MSIK)
  #define STM32_MCO2DIV_BITS                RCC_CFGR1_MCO2SEL_MSIK
#else
  #error "invalid STM32_CFG_MCO2DIV_SEL value specified"
#endif

/**
 * @brief   MCO2 selected source before prescaler clock point.
 */
#if ((STM32_MCO2DIV_ENABLED == TRUE) && \
     (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_OFF)) || \
    defined(__DOXYGEN__)
  #define STM32_MCO2DIV_FREQ                STM32_NONE_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_SYSCLK)
  #define STM32_MCO2DIV_FREQ                STM32_SYSCLK_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_MSIS)
  #define STM32_MCO2DIV_FREQ                STM32_MSIS_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSI16)
  #define STM32_MCO2DIV_FREQ                STM32_HSI16_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSE)
  #define STM32_MCO2DIV_FREQ                STM32_HSE_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_LSI)
  #define STM32_MCO2DIV_FREQ                STM32_LSI_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_LSE)
  #define STM32_MCO2DIV_FREQ                STM32_LSE_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_HSI48)
  #define STM32_MCO2DIV_FREQ                STM32_HSI48_FREQ
#elif (STM32_MCO2DIV_ENABLED == TRUE) && \
      (STM32_CFG_MCO2DIV_SEL == RCC_CFGR1_MCO2SEL_MSIK)
  #define STM32_MCO2DIV_FREQ                STM32_MSIK_FREQ
#else
  #define STM32_MCO2DIV_FREQ                0U
#endif

/*--- Macros and checks for the MCO2 clock point. --------------------------*/

/**
 * @brief   MCO2 clock register bits.
 */
#if (STM32_CFG_MCO2_VALUE == 1) || defined(__DOXYGEN__)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV1
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 2)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV2
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 4)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV4
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 8)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV8
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 16)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV16
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 32)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV32
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 64)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV64
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#elif (STM32_CFG_MCO2_VALUE == 128)
  #if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_MCO2_BITS                 RCC_CFGR1_MCO2PRE_DIV128
  #else
    #define STM32_MCO2_BITS                 0U
  #endif
#else
  #error "invalid STM32_CFG_MCO2_VALUE value specified"
#endif

/**
 * @brief   MCO2 output pin clock point.
 */
#if (STM32_MCO2_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_MCO2_FREQ                   (STM32_MCO2DIV_FREQ /           \
                                             STM32_CFG_MCO2_VALUE)
#else
  #define STM32_MCO2_FREQ                   0U
#endif

/*--- Macros and checks for the RTC clock point. ---------------------------*/

/**
 * @brief   RTC clock register bits.
 */
#if (STM32_RTC_ENABLED == FALSE)
  #define STM32_RTC_BITS                    RCC_BDCR_RTCSEL_NOCLOCK
#elif (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_RTC_BITS                    RCC_BDCR_RTCSEL_NOCLOCK
#elif (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_LSE)
  #define STM32_RTC_BITS                    RCC_BDCR_RTCSEL_LSE
#elif (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_LSI)
  #define STM32_RTC_BITS                    RCC_BDCR_RTCSEL_LSI
#elif (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_HSEDIV)
  #define STM32_RTC_BITS                    RCC_BDCR_RTCSEL_HSEDIV
#else
  #error "invalid STM32_CFG_RTC_SEL value specified"
#endif

/**
 * @brief   RTC clock clock point.
 */
#if ((STM32_RTC_ENABLED == TRUE) && \
     (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_NOCLOCK)) || \
    defined(__DOXYGEN__)
  #define STM32_RTC_FREQ                    STM32_NONE_FREQ
#elif (STM32_RTC_ENABLED == TRUE) && \
      (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_LSE)
  #define STM32_RTC_FREQ                    STM32_LSE_FREQ
#elif (STM32_RTC_ENABLED == TRUE) && \
      (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_LSI)
  #define STM32_RTC_FREQ                    STM32_LSI_FREQ
#elif (STM32_RTC_ENABLED == TRUE) && \
      (STM32_CFG_RTC_SEL == RCC_BDCR_RTCSEL_HSEDIV)
  #define STM32_RTC_FREQ                    STM32_HSEDIV_FREQ
#else
  #define STM32_RTC_FREQ                    0U
#endif

/*--- Macros and checks for the LSCO clock point. --------------------------*/

/**
 * @brief   LSCO clock register bits.
 */
#if (STM32_LSCO_ENABLED == FALSE)
  #define STM32_LSCO_BITS                   RCC_BDCR_LSCOSEL_NOCLOCK
#elif (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_LSCO_BITS                   RCC_BDCR_LSCOSEL_NOCLOCK
#elif (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_LSI)
  #define STM32_LSCO_BITS                   RCC_BDCR_LSCOSEL_LSI
#elif (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_LSE)
  #define STM32_LSCO_BITS                   RCC_BDCR_LSCOSEL_LSE
#else
  #error "invalid STM32_CFG_LSCO_SEL value specified"
#endif

/**
 * @brief   LSCO output pin clock point.
 */
#if ((STM32_LSCO_ENABLED == TRUE) && \
     (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_NOCLOCK)) || \
    defined(__DOXYGEN__)
  #define STM32_LSCO_FREQ                   STM32_NONE_FREQ
#elif (STM32_LSCO_ENABLED == TRUE) && \
      (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_LSI)
  #define STM32_LSCO_FREQ                   STM32_LSI_FREQ
#elif (STM32_LSCO_ENABLED == TRUE) && \
      (STM32_CFG_LSCO_SEL == RCC_BDCR_LSCOSEL_LSE)
  #define STM32_LSCO_FREQ                   STM32_LSE_FREQ
#else
  #define STM32_LSCO_FREQ                   0U
#endif

/*--- Macros and checks for the USART1 clock point. ------------------------*/

/**
 * @brief   USART1 clock register bits.
 */
#if (STM32_CFG_USART1_SEL == RCC_CCIPR1_USART1SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_USART1_BITS                 RCC_CCIPR1_USART1SEL_PCLK2
#elif (STM32_CFG_USART1_SEL == RCC_CCIPR1_USART1SEL_HSI16)
  #define STM32_USART1_BITS                 RCC_CCIPR1_USART1SEL_HSI16
#else
  #error "invalid STM32_CFG_USART1_SEL value specified"
#endif

/**
 * @brief   USART1 clock clock point.
 */
#if ((STM32_USART1_ENABLED == TRUE) && \
     (STM32_CFG_USART1_SEL == RCC_CCIPR1_USART1SEL_PCLK2)) || \
    defined(__DOXYGEN__)
  #define STM32_USART1_FREQ                 hal_lld_get_clock_point(CLK_PCLK2)
#elif (STM32_USART1_ENABLED == TRUE) && \
      (STM32_CFG_USART1_SEL == RCC_CCIPR1_USART1SEL_HSI16)
  #define STM32_USART1_FREQ                 hal_lld_get_clock_point(CLK_HSI16)
#else
  #define STM32_USART1_FREQ                 0U
#endif

/*--- Macros and checks for the USART3 clock point. ------------------------*/

/**
 * @brief   USART3 clock register bits.
 */
#if (STM32_CFG_USART3_SEL == RCC_CCIPR1_USART3SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART3_BITS                 RCC_CCIPR1_USART3SEL_PCLK1
#elif (STM32_CFG_USART3_SEL == RCC_CCIPR1_USART3SEL_HSI16)
  #define STM32_USART3_BITS                 RCC_CCIPR1_USART3SEL_HSI16
#else
  #error "invalid STM32_CFG_USART3_SEL value specified"
#endif

/**
 * @brief   USART3 clock clock point.
 */
#if ((STM32_USART3_ENABLED == TRUE) && \
     (STM32_CFG_USART3_SEL == RCC_CCIPR1_USART3SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_USART3_FREQ                 hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_USART3_ENABLED == TRUE) && \
      (STM32_CFG_USART3_SEL == RCC_CCIPR1_USART3SEL_HSI16)
  #define STM32_USART3_FREQ                 hal_lld_get_clock_point(CLK_HSI16)
#else
  #define STM32_USART3_FREQ                 0U
#endif

/*--- Macros and checks for the UART4 clock point. -------------------------*/

/**
 * @brief   UART4 clock register bits.
 */
#if (STM32_CFG_UART4_SEL == RCC_CCIPR1_UART4SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART4_BITS                  RCC_CCIPR1_UART4SEL_PCLK1
#elif (STM32_CFG_UART4_SEL == RCC_CCIPR1_UART4SEL_HSI16)
  #define STM32_UART4_BITS                  RCC_CCIPR1_UART4SEL_HSI16
#else
  #error "invalid STM32_CFG_UART4_SEL value specified"
#endif

/**
 * @brief   UART4 clock clock point.
 */
#if ((STM32_UART4_ENABLED == TRUE) && \
     (STM32_CFG_UART4_SEL == RCC_CCIPR1_UART4SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_UART4_FREQ                  hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_UART4_ENABLED == TRUE) && \
      (STM32_CFG_UART4_SEL == RCC_CCIPR1_UART4SEL_HSI16)
  #define STM32_UART4_FREQ                  hal_lld_get_clock_point(CLK_HSI16)
#else
  #define STM32_UART4_FREQ                  0U
#endif

/*--- Macros and checks for the UART5 clock point. -------------------------*/

/**
 * @brief   UART5 clock register bits.
 */
#if (STM32_CFG_UART5_SEL == RCC_CCIPR1_UART5SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART5_BITS                  RCC_CCIPR1_UART5SEL_PCLK1
#elif (STM32_CFG_UART5_SEL == RCC_CCIPR1_UART5SEL_HSI16)
  #define STM32_UART5_BITS                  RCC_CCIPR1_UART5SEL_HSI16
#else
  #error "invalid STM32_CFG_UART5_SEL value specified"
#endif

/**
 * @brief   UART5 clock clock point.
 */
#if ((STM32_UART5_ENABLED == TRUE) && \
     (STM32_CFG_UART5_SEL == RCC_CCIPR1_UART5SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_UART5_FREQ                  hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_UART5_ENABLED == TRUE) && \
      (STM32_CFG_UART5_SEL == RCC_CCIPR1_UART5SEL_HSI16)
  #define STM32_UART5_FREQ                  hal_lld_get_clock_point(CLK_HSI16)
#else
  #define STM32_UART5_FREQ                  0U
#endif

/*--- Macros and checks for the LPUART1 clock point. -----------------------*/

/**
 * @brief   LPUART1 clock register bits.
 */
#if (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_PCLK3) ||               \
    defined(__DOXYGEN__)
  #define STM32_LPUART1_BITS                RCC_CCIPR3_LPUART1SEL_PCLK3
#elif (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_HSI16)
  #define STM32_LPUART1_BITS                RCC_CCIPR3_LPUART1SEL_HSI16
#elif (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_LSE)
  #define STM32_LPUART1_BITS                RCC_CCIPR3_LPUART1SEL_LSE
#elif (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_MSIK)
  #define STM32_LPUART1_BITS                RCC_CCIPR3_LPUART1SEL_MSIK
#else
  #error "invalid STM32_CFG_LPUART1_SEL value specified"
#endif

/**
 * @brief   LPUART1 clock clock point.
 */
#if ((STM32_LPUART1_ENABLED == TRUE) && \
     (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_PCLK3)) || \
    defined(__DOXYGEN__)
  #define STM32_LPUART1_FREQ                hal_lld_get_clock_point(CLK_PCLK3)
#elif (STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_HSI16)
  #define STM32_LPUART1_FREQ                hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_LSE)
  #define STM32_LPUART1_FREQ                STM32_LSE_FREQ
#elif (STM32_LPUART1_ENABLED == TRUE) && \
      (STM32_CFG_LPUART1_SEL == RCC_CCIPR3_LPUART1SEL_MSIK)
  #define STM32_LPUART1_FREQ                hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_LPUART1_FREQ                0U
#endif

/*--- Macros and checks for the LPTIM1 clock point. ------------------------*/

/**
 * @brief   LPTIM1 clock register bits.
 */
#if (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_MSIK) || defined(__DOXYGEN__)
  #define STM32_LPTIM1_BITS                 RCC_CCIPR3_LPTIM1SEL_MSIK
#elif (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_LSI)
  #define STM32_LPTIM1_BITS                 RCC_CCIPR3_LPTIM1SEL_LSI
#elif (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_HSI16)
  #define STM32_LPTIM1_BITS                 RCC_CCIPR3_LPTIM1SEL_HSI16
#elif (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_LSE)
  #define STM32_LPTIM1_BITS                 RCC_CCIPR3_LPTIM1SEL_LSE
#else
  #error "invalid STM32_CFG_LPTIM1_SEL value specified"
#endif

/**
 * @brief   LPTIM1 clock clock point.
 */
#if ((STM32_LPTIM1_ENABLED == TRUE) && \
     (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_MSIK)) || \
    defined(__DOXYGEN__)
  #define STM32_LPTIM1_FREQ                 hal_lld_get_clock_point(CLK_MSIK)
#elif (STM32_LPTIM1_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_LSI)
  #define STM32_LPTIM1_FREQ                 STM32_LSI_FREQ
#elif (STM32_LPTIM1_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_HSI16)
  #define STM32_LPTIM1_FREQ                 hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_LPTIM1_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM1_SEL == RCC_CCIPR3_LPTIM1SEL_LSE)
  #define STM32_LPTIM1_FREQ                 STM32_LSE_FREQ
#else
  #define STM32_LPTIM1_FREQ                 0U
#endif

/*--- Macros and checks for the LPTIM2 clock point. ------------------------*/

/**
 * @brief   LPTIM2 clock register bits.
 */
#if (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_LPTIM2_BITS                 RCC_CCIPR1_LPTIM2SEL_PCLK1
#elif (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_LSI)
  #define STM32_LPTIM2_BITS                 RCC_CCIPR1_LPTIM2SEL_LSI
#elif (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_HSI16)
  #define STM32_LPTIM2_BITS                 RCC_CCIPR1_LPTIM2SEL_HSI16
#elif (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_LSE)
  #define STM32_LPTIM2_BITS                 RCC_CCIPR1_LPTIM2SEL_LSE
#else
  #error "invalid STM32_CFG_LPTIM2_SEL value specified"
#endif

/**
 * @brief   LPTIM2 clock clock point.
 */
#if ((STM32_LPTIM2_ENABLED == TRUE) && \
     (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_LPTIM2_FREQ                 hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_LPTIM2_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_LSI)
  #define STM32_LPTIM2_FREQ                 STM32_LSI_FREQ
#elif (STM32_LPTIM2_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_HSI16)
  #define STM32_LPTIM2_FREQ                 hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_LPTIM2_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM2_SEL == RCC_CCIPR1_LPTIM2SEL_LSE)
  #define STM32_LPTIM2_FREQ                 STM32_LSE_FREQ
#else
  #define STM32_LPTIM2_FREQ                 0U
#endif

/*--- Macros and checks for the LPTIM34 clock point. -----------------------*/

/**
 * @brief   LPTIM34 clock register bits.
 */
#if (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_MSIK) ||                \
    defined(__DOXYGEN__)
  #define STM32_LPTIM34_BITS                RCC_CCIPR3_LPTIM34SEL_MSIK
#elif (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_LSI)
  #define STM32_LPTIM34_BITS                RCC_CCIPR3_LPTIM34SEL_LSI
#elif (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_HSI16)
  #define STM32_LPTIM34_BITS                RCC_CCIPR3_LPTIM34SEL_HSI16
#elif (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_LSE)
  #define STM32_LPTIM34_BITS                RCC_CCIPR3_LPTIM34SEL_LSE
#else
  #error "invalid STM32_CFG_LPTIM34_SEL value specified"
#endif

/**
 * @brief   LPTIM3 and LPTIM4 clock clock point.
 */
#if ((STM32_LPTIM34_ENABLED == TRUE) && \
     (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_MSIK)) || \
    defined(__DOXYGEN__)
  #define STM32_LPTIM34_FREQ                hal_lld_get_clock_point(CLK_MSIK)
#elif (STM32_LPTIM34_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_LSI)
  #define STM32_LPTIM34_FREQ                STM32_LSI_FREQ
#elif (STM32_LPTIM34_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_HSI16)
  #define STM32_LPTIM34_FREQ                hal_lld_get_clock_point(CLK_HSI16)
#elif (STM32_LPTIM34_ENABLED == TRUE) && \
      (STM32_CFG_LPTIM34_SEL == RCC_CCIPR3_LPTIM34SEL_LSE)
  #define STM32_LPTIM34_FREQ                STM32_LSE_FREQ
#else
  #define STM32_LPTIM34_FREQ                0U
#endif

/*--- Macros and checks for the SPI1 clock point. --------------------------*/

/**
 * @brief   SPI1 clock register bits.
 */
#if (STM32_CFG_SPI1_SEL == RCC_CCIPR1_SPI1SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_SPI1_BITS                   RCC_CCIPR1_SPI1SEL_PCLK2
#elif (STM32_CFG_SPI1_SEL == RCC_CCIPR1_SPI1SEL_MSIK)
  #define STM32_SPI1_BITS                   RCC_CCIPR1_SPI1SEL_MSIK
#else
  #error "invalid STM32_CFG_SPI1_SEL value specified"
#endif

/**
 * @brief   SPI1 clock clock point.
 */
#if ((STM32_SPI1_ENABLED == TRUE) && \
     (STM32_CFG_SPI1_SEL == RCC_CCIPR1_SPI1SEL_PCLK2)) || \
    defined(__DOXYGEN__)
  #define STM32_SPI1_FREQ                   hal_lld_get_clock_point(CLK_PCLK2)
#elif (STM32_SPI1_ENABLED == TRUE) && \
      (STM32_CFG_SPI1_SEL == RCC_CCIPR1_SPI1SEL_MSIK)
  #define STM32_SPI1_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_SPI1_FREQ                   0U
#endif

/*--- Macros and checks for the SPI2 clock point. --------------------------*/

/**
 * @brief   SPI2 clock register bits.
 */
#if (STM32_CFG_SPI2_SEL == RCC_CCIPR1_SPI2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_SPI2_BITS                   RCC_CCIPR1_SPI2SEL_PCLK1
#elif (STM32_CFG_SPI2_SEL == RCC_CCIPR1_SPI2SEL_MSIK)
  #define STM32_SPI2_BITS                   RCC_CCIPR1_SPI2SEL_MSIK
#else
  #error "invalid STM32_CFG_SPI2_SEL value specified"
#endif

/**
 * @brief   SPI2 clock clock point.
 */
#if ((STM32_SPI2_ENABLED == TRUE) && \
     (STM32_CFG_SPI2_SEL == RCC_CCIPR1_SPI2SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_SPI2_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_SPI2_ENABLED == TRUE) && \
      (STM32_CFG_SPI2_SEL == RCC_CCIPR1_SPI2SEL_MSIK)
  #define STM32_SPI2_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_SPI2_FREQ                   0U
#endif

/*--- Macros and checks for the SPI3 clock point. --------------------------*/

/**
 * @brief   SPI3 clock register bits.
 */
#if (STM32_CFG_SPI3_SEL == RCC_CCIPR2_SPI3SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_SPI3_BITS                   RCC_CCIPR2_SPI3SEL_PCLK1
#elif (STM32_CFG_SPI3_SEL == RCC_CCIPR2_SPI3SEL_MSIK)
  #define STM32_SPI3_BITS                   RCC_CCIPR2_SPI3SEL_MSIK
#else
  #error "invalid STM32_CFG_SPI3_SEL value specified"
#endif

/**
 * @brief   SPI3 clock clock point.
 */
#if ((STM32_SPI3_ENABLED == TRUE) && \
     (STM32_CFG_SPI3_SEL == RCC_CCIPR2_SPI3SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_SPI3_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_SPI3_ENABLED == TRUE) && \
      (STM32_CFG_SPI3_SEL == RCC_CCIPR2_SPI3SEL_MSIK)
  #define STM32_SPI3_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_SPI3_FREQ                   0U
#endif

/*--- Macros and checks for the OCTOSPI clock point. -----------------------*/

/**
 * @brief   OCTOSPI clock register bits.
 */
#if (STM32_CFG_OCTOSPI_SEL == RCC_CCIPR2_OCTOSPISEL_SYSCLK) ||              \
    defined(__DOXYGEN__)
  #define STM32_OCTOSPI_BITS                RCC_CCIPR2_OCTOSPISEL_SYSCLK
#elif (STM32_CFG_OCTOSPI_SEL == RCC_CCIPR2_OCTOSPISEL_MSIK)
  #define STM32_OCTOSPI_BITS                RCC_CCIPR2_OCTOSPISEL_MSIK
#else
  #error "invalid STM32_CFG_OCTOSPI_SEL value specified"
#endif

/**
 * @brief   OCTOSPI clock clock point.
 */
#if ((STM32_OCTOSPI_ENABLED == TRUE) && \
     (STM32_CFG_OCTOSPI_SEL == RCC_CCIPR2_OCTOSPISEL_SYSCLK)) || \
    defined(__DOXYGEN__)
  #define STM32_OCTOSPI_FREQ                hal_lld_get_clock_point(CLK_SYSCLK)
#elif (STM32_OCTOSPI_ENABLED == TRUE) && \
      (STM32_CFG_OCTOSPI_SEL == RCC_CCIPR2_OCTOSPISEL_MSIK)
  #define STM32_OCTOSPI_FREQ                hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_OCTOSPI_FREQ                0U
#endif

/*--- Macros and checks for the SYSTICK clock point. -----------------------*/

/**
 * @brief   SYSTICK clock register bits.
 */
#if (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_HCLKDIV8) ||            \
    defined(__DOXYGEN__)
  #define STM32_SYSTICK_BITS                RCC_CCIPR1_SYSTICKSEL_HCLKDIV8
#elif (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_LSI)
  #define STM32_SYSTICK_BITS                RCC_CCIPR1_SYSTICKSEL_LSI
#elif (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_LSE)
  #define STM32_SYSTICK_BITS                RCC_CCIPR1_SYSTICKSEL_LSE
#else
  #error "invalid STM32_CFG_SYSTICK_SEL value specified"
#endif

/**
 * @brief   SYSTICK clock clock point.
 */
#if ((STM32_SYSTICK_ENABLED == TRUE) && \
     (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_HCLKDIV8)) || \
    defined(__DOXYGEN__)
  #define STM32_SYSTICK_FREQ                STM32_HCLKDIV8_FREQ
#elif (STM32_SYSTICK_ENABLED == TRUE) && \
      (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_LSI)
  #define STM32_SYSTICK_FREQ                STM32_LSI_FREQ
#elif (STM32_SYSTICK_ENABLED == TRUE) && \
      (STM32_CFG_SYSTICK_SEL == RCC_CCIPR1_SYSTICKSEL_LSE)
  #define STM32_SYSTICK_FREQ                STM32_LSE_FREQ
#else
  #define STM32_SYSTICK_FREQ                0U
#endif

/*--- Macros and checks for the ICLK clock point. --------------------------*/

/**
 * @brief   ICLK clock register bits.
 */
#if (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_HSI48) || defined(__DOXYGEN__)
  #define STM32_ICLK_BITS                   RCC_CCIPR1_ICLKSEL_HSI48
#elif (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_MSIK)
  #define STM32_ICLK_BITS                   RCC_CCIPR1_ICLKSEL_MSIK
#elif (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_HSE)
  #define STM32_ICLK_BITS                   RCC_CCIPR1_ICLKSEL_HSE
#elif (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_SYSCLK)
  #define STM32_ICLK_BITS                   RCC_CCIPR1_ICLKSEL_SYSCLK
#else
  #error "invalid STM32_CFG_ICLK_SEL value specified"
#endif

/**
 * @brief   ICLK clock clock point.
 */
#if ((STM32_ICLK_ENABLED == TRUE) && \
     (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_HSI48)) || \
    defined(__DOXYGEN__)
  #define STM32_ICLK_FREQ                   hal_lld_get_clock_point(CLK_HSI48)
#elif (STM32_ICLK_ENABLED == TRUE) && \
      (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_MSIK)
  #define STM32_ICLK_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#elif (STM32_ICLK_ENABLED == TRUE) && \
      (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_HSE)
  #define STM32_ICLK_FREQ                   hal_lld_get_clock_point(CLK_HSE)
#elif (STM32_ICLK_ENABLED == TRUE) && \
      (STM32_CFG_ICLK_SEL == RCC_CCIPR1_ICLKSEL_SYSCLK)
  #define STM32_ICLK_FREQ                   hal_lld_get_clock_point(CLK_SYSCLK)
#else
  #define STM32_ICLK_FREQ                   0U
#endif

/*--- Macros and checks for the ICLKDIV2 clock point. ----------------------*/

/**
 * @brief   ICLKDIV2 clock register bits.
 */
#if (STM32_ICLKDIV2_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_ICLKDIV2_BITS               0U
#else
  #define STM32_ICLKDIV2_BITS               0U
#endif

/**
 * @brief   ICLK divided by 2 clock point.
 */
#if (STM32_ICLKDIV2_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_ICLKDIV2_FREQ               (STM32_ICLK_FREQ / 2U)
#else
  #define STM32_ICLKDIV2_FREQ               0U
#endif

/*--- Macros and checks for the USB clock point. ---------------------------*/

/**
 * @brief   USB clock register bits.
 */
#if (STM32_CFG_USB_SEL == RCC_CCIPR1_USB1SEL_ICLK) || defined(__DOXYGEN__)
  #define STM32_USB_BITS                    RCC_CCIPR1_USB1SEL_ICLK
#elif (STM32_CFG_USB_SEL == RCC_CCIPR1_USB1SEL_ICLKDIV2)
  #define STM32_USB_BITS                    RCC_CCIPR1_USB1SEL_ICLKDIV2
#else
  #error "invalid STM32_CFG_USB_SEL value specified"
#endif

/**
 * @brief   USB clock clock point.
 */
#if ((STM32_USB_ENABLED == TRUE) && \
     (STM32_CFG_USB_SEL == RCC_CCIPR1_USB1SEL_ICLK)) || \
    defined(__DOXYGEN__)
  #define STM32_USB_FREQ                    STM32_ICLK_FREQ
#elif (STM32_USB_ENABLED == TRUE) && \
      (STM32_CFG_USB_SEL == RCC_CCIPR1_USB1SEL_ICLKDIV2)
  #define STM32_USB_FREQ                    STM32_ICLKDIV2_FREQ
#else
  #define STM32_USB_FREQ                    0U
#endif

/*--- Macros and checks for the SDMMC1 clock point. ------------------------*/

/**
 * @brief   SDMMC1 clock register bits.
 */
#if (STM32_SDMMC1_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_SDMMC1_BITS                 0U
#else
  #define STM32_SDMMC1_BITS                 0U
#endif

/**
 * @brief   SDMMC1 clock clock point.
 */
#if (STM32_SDMMC1_ENABLED == TRUE) || defined(__DOXYGEN__)
  #define STM32_SDMMC1_FREQ                 STM32_ICLK_FREQ
#else
  #define STM32_SDMMC1_FREQ                 0U
#endif

/*--- Macros and checks for the I2C1 clock point. --------------------------*/

/**
 * @brief   I2C1 clock register bits.
 */
#if (STM32_CFG_I2C1_SEL == RCC_CCIPR1_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C1_BITS                   RCC_CCIPR1_I2C1SEL_PCLK1
#elif (STM32_CFG_I2C1_SEL == RCC_CCIPR1_I2C1SEL_MSIK)
  #define STM32_I2C1_BITS                   RCC_CCIPR1_I2C1SEL_MSIK
#else
  #error "invalid STM32_CFG_I2C1_SEL value specified"
#endif

/**
 * @brief   I2C1 clock clock point.
 */
#if ((STM32_I2C1_ENABLED == TRUE) && \
     (STM32_CFG_I2C1_SEL == RCC_CCIPR1_I2C1SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_I2C1_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I2C1_ENABLED == TRUE) && \
      (STM32_CFG_I2C1_SEL == RCC_CCIPR1_I2C1SEL_MSIK)
  #define STM32_I2C1_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_I2C1_FREQ                   0U
#endif

/*--- Macros and checks for the I2C2 clock point. --------------------------*/

/**
 * @brief   I2C2 clock register bits.
 */
#if (STM32_CFG_I2C2_SEL == RCC_CCIPR1_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C2_BITS                   RCC_CCIPR1_I2C2SEL_PCLK1
#elif (STM32_CFG_I2C2_SEL == RCC_CCIPR1_I2C2SEL_MSIK)
  #define STM32_I2C2_BITS                   RCC_CCIPR1_I2C2SEL_MSIK
#else
  #error "invalid STM32_CFG_I2C2_SEL value specified"
#endif

/**
 * @brief   I2C2 clock clock point.
 */
#if ((STM32_I2C2_ENABLED == TRUE) && \
     (STM32_CFG_I2C2_SEL == RCC_CCIPR1_I2C2SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_I2C2_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I2C2_ENABLED == TRUE) && \
      (STM32_CFG_I2C2_SEL == RCC_CCIPR1_I2C2SEL_MSIK)
  #define STM32_I2C2_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_I2C2_FREQ                   0U
#endif

/*--- Macros and checks for the I2C3 clock point. --------------------------*/

/**
 * @brief   I2C3 clock register bits.
 */
#if (STM32_CFG_I2C3_SEL == RCC_CCIPR3_I2C3SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_I2C3_BITS                   RCC_CCIPR3_I2C3SEL_PCLK3
#elif (STM32_CFG_I2C3_SEL == RCC_CCIPR3_I2C3SEL_MSIK)
  #define STM32_I2C3_BITS                   RCC_CCIPR3_I2C3SEL_MSIK
#else
  #error "invalid STM32_CFG_I2C3_SEL value specified"
#endif

/**
 * @brief   I2C3 clock clock point.
 */
#if ((STM32_I2C3_ENABLED == TRUE) && \
     (STM32_CFG_I2C3_SEL == RCC_CCIPR3_I2C3SEL_PCLK3)) || \
    defined(__DOXYGEN__)
  #define STM32_I2C3_FREQ                   hal_lld_get_clock_point(CLK_PCLK3)
#elif (STM32_I2C3_ENABLED == TRUE) && \
      (STM32_CFG_I2C3_SEL == RCC_CCIPR3_I2C3SEL_MSIK)
  #define STM32_I2C3_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_I2C3_FREQ                   0U
#endif

/*--- Macros and checks for the I3C1 clock point. --------------------------*/

/**
 * @brief   I3C1 clock register bits.
 */
#if (STM32_CFG_I3C1_SEL == RCC_CCIPR1_I3C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I3C1_BITS                   RCC_CCIPR1_I3C1SEL_PCLK1
#elif (STM32_CFG_I3C1_SEL == RCC_CCIPR1_I3C1SEL_MSIK)
  #define STM32_I3C1_BITS                   RCC_CCIPR1_I3C1SEL_MSIK
#else
  #error "invalid STM32_CFG_I3C1_SEL value specified"
#endif

/**
 * @brief   I3C1 clock clock point.
 */
#if ((STM32_I3C1_ENABLED == TRUE) && \
     (STM32_CFG_I3C1_SEL == RCC_CCIPR1_I3C1SEL_PCLK1)) || \
    defined(__DOXYGEN__)
  #define STM32_I3C1_FREQ                   hal_lld_get_clock_point(CLK_PCLK1)
#elif (STM32_I3C1_ENABLED == TRUE) && \
      (STM32_CFG_I3C1_SEL == RCC_CCIPR1_I3C1SEL_MSIK)
  #define STM32_I3C1_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_I3C1_FREQ                   0U
#endif

/*--- Macros and checks for the I3C2 clock point. --------------------------*/

/**
 * @brief   I3C2 clock register bits.
 */
#if (STM32_CFG_I3C2_SEL == RCC_CCIPR1_I3C2SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_I3C2_BITS                   RCC_CCIPR1_I3C2SEL_PCLK3
#elif (STM32_CFG_I3C2_SEL == RCC_CCIPR1_I3C2SEL_MSIK)
  #define STM32_I3C2_BITS                   RCC_CCIPR1_I3C2SEL_MSIK
#else
  #error "invalid STM32_CFG_I3C2_SEL value specified"
#endif

/**
 * @brief   I3C2 clock clock point.
 */
#if ((STM32_I3C2_ENABLED == TRUE) && \
     (STM32_CFG_I3C2_SEL == RCC_CCIPR1_I3C2SEL_PCLK3)) || \
    defined(__DOXYGEN__)
  #define STM32_I3C2_FREQ                   hal_lld_get_clock_point(CLK_PCLK3)
#elif (STM32_I3C2_ENABLED == TRUE) && \
      (STM32_CFG_I3C2_SEL == RCC_CCIPR1_I3C2SEL_MSIK)
  #define STM32_I3C2_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_I3C2_FREQ                   0U
#endif

/*--- Macros and checks for the ADCDAC clock point. ------------------------*/

/**
 * @brief   ADCDAC clock register bits.
 */
#if (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HCLK) || defined(__DOXYGEN__)
  #define STM32_ADCDAC_BITS                 RCC_CCIPR2_ADCDACSEL_HCLK
#elif (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HSE)
  #define STM32_ADCDAC_BITS                 RCC_CCIPR2_ADCDACSEL_HSE
#elif (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_MSIK)
  #define STM32_ADCDAC_BITS                 RCC_CCIPR2_ADCDACSEL_MSIK
#else
  #error "invalid STM32_CFG_ADCDAC_SEL value specified"
#endif

/**
 * @brief   ADC and DAC clock clock point.
 */
#if ((STM32_ADCDAC_ENABLED == TRUE) && \
     (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HCLK)) || \
    defined(__DOXYGEN__)
  #define STM32_ADCDAC_FREQ                 hal_lld_get_clock_point(CLK_HCLK)
#elif (STM32_ADCDAC_ENABLED == TRUE) && \
      (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HSE)
  #define STM32_ADCDAC_FREQ                 hal_lld_get_clock_point(CLK_HSE)
#elif (STM32_ADCDAC_ENABLED == TRUE) && \
      (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_MSIK)
  #define STM32_ADCDAC_FREQ                 hal_lld_get_clock_point(CLK_MSIK)
#else
  #define STM32_ADCDAC_FREQ                 0U
#endif

#if !defined(RCC_CCIPR2_ADCDACSEL_HCLK) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_ADCDACSEL_HCLK not defined"
#endif
#if !(!((STM32_ADCDAC_ENABLED == TRUE) &&                                   \
      (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HCLK)) ||               \
     (STM32_HCLK_FREQ <= STM32_ADCCLK_MAX)) && !defined(__DOXYGEN__)
  #error "STM32_ADCDAC_FREQ above maximum frequency"
#endif

#if !(!((STM32_ADCDAC_ENABLED == TRUE) &&                                   \
      (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_HSE)) ||                \
     (STM32_HSE_FREQ <= STM32_ADCCLK_MAX)) && !defined(__DOXYGEN__)
  #error "STM32_ADCDAC_FREQ above maximum frequency"
#endif

#if !defined(RCC_CCIPR2_ADCDACSEL_MSIK) && !defined(__DOXYGEN__)
  #error "RCC_CCIPR2_ADCDACSEL_MSIK not defined"
#endif
#if !(!((STM32_ADCDAC_ENABLED == TRUE) &&                                   \
      (STM32_CFG_ADCDAC_SEL == RCC_CCIPR2_ADCDACSEL_MSIK)) ||               \
     (STM32_MSIK_FREQ <= STM32_ADCCLK_MAX)) && !defined(__DOXYGEN__)
  #error "STM32_ADCDAC_FREQ above maximum frequency"
#endif

/*--- Macros and checks for the DAC1SH clock point. ------------------------*/

/**
 * @brief   DAC1SH clock register bits.
 */
#if (STM32_DAC1SH_ENABLED == FALSE)
  #define STM32_DAC1SH_BITS                 RCC_CCIPR2_DAC1SHSEL_IGNORE
#elif (STM32_CFG_DAC1SH_SEL == RCC_CCIPR2_DAC1SHSEL_LSE) || defined(__DOXYGEN__)
  #if (STM32_DAC1SH_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_DAC1SH_BITS               (0U | RCC_CCIPR2_DAC1SHSEL_LSE)
  #else
    #define STM32_DAC1SH_BITS               RCC_CCIPR2_DAC1SHSEL_IGNORE
  #endif
#elif (STM32_CFG_DAC1SH_SEL == RCC_CCIPR2_DAC1SHSEL_LSI)
  #if (STM32_DAC1SH_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_DAC1SH_BITS               (0U | RCC_CCIPR2_DAC1SHSEL_LSI)
  #else
    #define STM32_DAC1SH_BITS               RCC_CCIPR2_DAC1SHSEL_IGNORE
  #endif
#else
  #error "invalid STM32_CFG_DAC1SH_SEL value specified"
#endif

/**
 * @brief   DAC1 sample and hold clock clock point.
 */
#if ((STM32_DAC1SH_ENABLED == TRUE) && \
     (STM32_CFG_DAC1SH_SEL == RCC_CCIPR2_DAC1SHSEL_LSE)) || \
    defined(__DOXYGEN__)
  #define STM32_DAC1SH_FREQ                 STM32_LSE_FREQ
#elif (STM32_DAC1SH_ENABLED == TRUE) && \
      (STM32_CFG_DAC1SH_SEL == RCC_CCIPR2_DAC1SHSEL_LSI)
  #define STM32_DAC1SH_FREQ                 STM32_LSI_FREQ
#else
  #define STM32_DAC1SH_FREQ                 0U
#endif

/*--- Macros and checks for the RNG clock point. ---------------------------*/

/**
 * @brief   RNG clock register bits.
 */
#if (STM32_RNG_ENABLED == FALSE)
  #define STM32_RNG_BITS                    RCC_CCIPR2_RNGSEL_IGNORE
#elif (STM32_CFG_RNG_SEL == RCC_CCIPR2_RNGSEL_HSI48) || defined(__DOXYGEN__)
  #if (STM32_RNG_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_RNG_BITS                  (0U | RCC_CCIPR2_RNGSEL_HSI48)
  #else
    #define STM32_RNG_BITS                  RCC_CCIPR2_RNGSEL_IGNORE
  #endif
#elif (STM32_CFG_RNG_SEL == RCC_CCIPR2_RNGSEL_MSIK)
  #if (STM32_RNG_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_RNG_BITS                  (0U | RCC_CCIPR2_RNGSEL_MSIK)
  #else
    #define STM32_RNG_BITS                  RCC_CCIPR2_RNGSEL_IGNORE
  #endif
#else
  #error "invalid STM32_CFG_RNG_SEL value specified"
#endif

/**
 * @brief   RNG clock clock point.
 */
#if ((STM32_RNG_ENABLED == TRUE) && \
     (STM32_CFG_RNG_SEL == RCC_CCIPR2_RNGSEL_HSI48)) || \
    defined(__DOXYGEN__)
  #define STM32_RNG_FREQ                    STM32_HSI48_FREQ
#elif (STM32_RNG_ENABLED == TRUE) && \
      (STM32_CFG_RNG_SEL == RCC_CCIPR2_RNGSEL_MSIK)
  #define STM32_RNG_FREQ                    STM32_MSIK_FREQ
#else
  #define STM32_RNG_FREQ                    0U
#endif

/*--- Macros and checks for the FDCAN1 clock point. ------------------------*/

/**
 * @brief   FDCAN1 clock register bits.
 */
#if (STM32_FDCAN1_ENABLED == FALSE)
  #define STM32_FDCAN1_BITS                 RCC_CCIPR1_FDCAN1SEL_IGNORE
#elif (STM32_CFG_FDCAN1_SEL == RCC_CCIPR1_FDCAN1SEL_SYSCLK) ||              \
      defined(__DOXYGEN__)
  #if (STM32_FDCAN1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_FDCAN1_BITS               (0U | RCC_CCIPR1_FDCAN1SEL_SYSCLK)
  #else
    #define STM32_FDCAN1_BITS               RCC_CCIPR1_FDCAN1SEL_IGNORE
  #endif
#elif (STM32_CFG_FDCAN1_SEL == RCC_CCIPR1_FDCAN1SEL_MSIK)
  #if (STM32_FDCAN1_ENABLED == TRUE) || defined(__DOXYGEN__)
    #define STM32_FDCAN1_BITS               (0U | RCC_CCIPR1_FDCAN1SEL_MSIK)
  #else
    #define STM32_FDCAN1_BITS               RCC_CCIPR1_FDCAN1SEL_IGNORE
  #endif
#else
  #error "invalid STM32_CFG_FDCAN1_SEL value specified"
#endif

/**
 * @brief   FDCAN1 clock clock point.
 */
#if ((STM32_FDCAN1_ENABLED == TRUE) && \
     (STM32_CFG_FDCAN1_SEL == RCC_CCIPR1_FDCAN1SEL_SYSCLK)) || \
    defined(__DOXYGEN__)
  #define STM32_FDCAN1_FREQ                 STM32_SYSCLK_FREQ
#elif (STM32_FDCAN1_ENABLED == TRUE) && \
      (STM32_CFG_FDCAN1_SEL == RCC_CCIPR1_FDCAN1SEL_MSIK)
  #define STM32_FDCAN1_FREQ                 STM32_MSIK_FREQ
#else
  #define STM32_FDCAN1_FREQ                 0U
#endif

/*--- Macros and checks for the SAI1 clock point. --------------------------*/

/**
 * @brief   SAI1 clock register bits.
 */
#if (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_MSIK) || defined(__DOXYGEN__)
  #define STM32_SAI1_BITS                   RCC_CCIPR2_SAI1SEL_MSIK
#elif (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_AUDIOCLK)
  #define STM32_SAI1_BITS                   RCC_CCIPR2_SAI1SEL_AUDIOCLK
#elif (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_HSE)
  #define STM32_SAI1_BITS                   RCC_CCIPR2_SAI1SEL_HSE
#else
  #error "invalid STM32_CFG_SAI1_SEL value specified"
#endif

/**
 * @brief   SAI1 clock clock point.
 */
#if ((STM32_SAI1_ENABLED == TRUE) && \
     (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_MSIK)) || \
    defined(__DOXYGEN__)
  #define STM32_SAI1_FREQ                   hal_lld_get_clock_point(CLK_MSIK)
#elif (STM32_SAI1_ENABLED == TRUE) && \
      (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_AUDIOCLK)
  #define STM32_SAI1_FREQ                   STM32_AUDIOCLK_FREQ
#elif (STM32_SAI1_ENABLED == TRUE) && \
      (STM32_CFG_SAI1_SEL == RCC_CCIPR2_SAI1SEL_HSE)
  #define STM32_SAI1_FREQ                   hal_lld_get_clock_point(CLK_HSE)
#else
  #define STM32_SAI1_FREQ                   0U
#endif

/*--- Macros and checks for the ADF1 clock point. --------------------------*/

/**
 * @brief   ADF1 clock register bits.
 */
#if (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_HCLK) || defined(__DOXYGEN__)
  #define STM32_ADF1_BITS                   RCC_CCIPR2_ADF1SEL_HCLK
#elif (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_AUDIOCLK)
  #define STM32_ADF1_BITS                   RCC_CCIPR2_ADF1SEL_AUDIOCLK
#elif (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_HSE)
  #define STM32_ADF1_BITS                   RCC_CCIPR2_ADF1SEL_HSE
#elif (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_SAI1)
  #define STM32_ADF1_BITS                   RCC_CCIPR2_ADF1SEL_SAI1
#else
  #error "invalid STM32_CFG_ADF1_SEL value specified"
#endif

/**
 * @brief   ADF1 clock clock point.
 */
#if ((STM32_ADF1_ENABLED == TRUE) && \
     (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_HCLK)) || \
    defined(__DOXYGEN__)
  #define STM32_ADF1_FREQ                   hal_lld_get_clock_point(CLK_HCLK)
#elif (STM32_ADF1_ENABLED == TRUE) && \
      (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_AUDIOCLK)
  #define STM32_ADF1_FREQ                   STM32_AUDIOCLK_FREQ
#elif (STM32_ADF1_ENABLED == TRUE) && \
      (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_HSE)
  #define STM32_ADF1_FREQ                   hal_lld_get_clock_point(CLK_HSE)
#elif (STM32_ADF1_ENABLED == TRUE) && \
      (STM32_CFG_ADF1_SEL == RCC_CCIPR2_ADF1SEL_SAI1)
  #define STM32_ADF1_FREQ                   STM32_SAI1_FREQ
#else
  #define STM32_ADF1_FREQ                   0U
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
   (clkpt) == CLK_MSIS         ? STM32_MSIS_FREQ          :                       \
   (clkpt) == CLK_MSIK         ? STM32_MSIK_FREQ          :                       \
   (clkpt) == CLK_SYSCLK       ? STM32_SYSCLK_FREQ        :                       \
   (clkpt) == CLK_HCLK         ? STM32_HCLK_FREQ          :                       \
   (clkpt) == CLK_PCLK1        ? STM32_PCLK1_FREQ         :                       \
   (clkpt) == CLK_PCLK1TIM     ? STM32_PCLK1TIM_FREQ      :                       \
   (clkpt) == CLK_PCLK2        ? STM32_PCLK2_FREQ         :                       \
   (clkpt) == CLK_PCLK2TIM     ? STM32_PCLK2TIM_FREQ      :                       \
   (clkpt) == CLK_PCLK3        ? STM32_PCLK3_FREQ         :                       \
   (clkpt) == CLK_MCO1         ? STM32_MCO1_FREQ          :                       \
   (clkpt) == CLK_MCO2         ? STM32_MCO2_FREQ          :                       \
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
