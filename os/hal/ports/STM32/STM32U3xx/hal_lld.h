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
 * @file    STM32H5xx/hal_lld.h
 * @brief   STM32H5xx HAL subsystem low level driver header.
 * @pre     This module requires the following macros to be defined in the
 *          @p board.h file:
 *          - STM32_LSECLK.
 *          - STM32_LSEDRV.
 *          - STM32_LSE_BYPASS (optionally).
 *          - STM32_HSECLK.
 *          - STM32_HSE_BYPASS (optionally).
 *          - STM32_HSE_DIGITAL (optionally).
 *          .
 *          One of the following macros must also be defined:
 *          - STM32U375xx, STM32U385xx.
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
#if defined(STM32U375xx) || defined(__DOXYGEN__)
  #define PLATFORM_NAME         "STM32U3 Low-power"

#elif defined(STM32U385xx)
  #define PLATFORM_NAME         "STM32U3 Low-power with Crypto"

#else
  #error "STM32U3 device not specified"
#endif

/**
 * @brief   Sub-family identifier.
 */
#if !defined(STM32U3XX) || defined(__DOXYGEN__)
  #define STM32U3XX
#endif
/** @} */

/**
 * @name   Clock points names
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
/** @} */

/**
 * @name    MSIRC0 96MHz oscillator settings
 * @{
 */
#define STM32_MSIRC0_FREE       0
#define STM32_MSIRC0_PLL_LSE    1
#define STM32_MSIRC0_PLL_HSE    2
#define STM32_MSIRC0_PLL_LSE_FAST 3
#define STM32_MSIRC0_PLL_HSE_FAST 4
/** @} */

/**
 * @name    MSIRC1 24MHz oscillator settings
 * @{
 */
#define STM32_MSIRC1_FREE       0
#define STM32_MSIRC1_PLL_LSE    1
#define STM32_MSIRC1_PLL_HSE    2
#define STM32_MSIRC1_PLL_LSE_FAST 3
#define STM32_MSIRC1_PLL_HSE_FAST 4
/** @} */

/**
 * @name    Source and divider for MSIS
 * @{
 */
#define STM32_MSIS_IRC0_DIV1    0       /* 96MHz */
#define STM32_MSIS_IRC0_DIV2    1       /* 48MHz */
#define STM32_MSIS_IRC0_DIV4    2       /* 24MHz */
#define STM32_MSIS_IRC0_DIV8    3       /* 12MHz */
#define STM32_MSIS_IRC1_DIV1    4       /* ~24MHz */
#define STM32_MSIS_IRC1_DIV3    5       /* ~12MHz */
#define STM32_MSIS_IRC1_DIV4    6       /* ~6MHz */
#define STM32_MSIS_IRC1_DIV8    7       /* ~4MHz */
/** @} */

/**
 * @name    Source and divider for MSIK
 * @{
 */
#define STM32_MSIK_IRC0_DIV1    0       /* 96MHz */
#define STM32_MSIK_IRC0_DIV2    1       /* 48MHz */
#define STM32_MSIK_IRC0_DIV4    2       /* 24MHz */
#define STM32_MSIK_IRC0_DIV8    3       /* 12MHz */
#define STM32_MSIK_IRC1_DIV1    4       /* ~24MHz */
#define STM32_MSIK_IRC1_DIV3    5       /* ~12MHz */
#define STM32_MSIK_IRC1_DIV4    6       /* ~6MHz */
#define STM32_MSIK_IRC1_DIV8    7       /* ~4MHz */
/** @} */

/**
 * @name    PWR_VOSR register definitions
 * @{
 */
#define STM32_VOS_POS           0
#define STM32_VOS_MASK          (3U << 0)
#define STM32_VOS_RANGE1        (1U << PWR_VOSR_R1EN_Pos)
#define STM32_VOS_RANGE2        (1U << PWR_VOSR_R2EN_Pos)
/** @} */

/**
 * @name    PWR_CR3 register definitions
 * @{
 */
#define STM32_REGSEL            (1U << PWR_CR3_REGSEL_Pos)

#define STM32_FSTEN             (1U << PWR_CR3_FSTEN_Pos)
/** @} */

/**
 * @name    PWR_SVMCR register definitions
 * @{
 */
#define STM32_ASV               PWR_SVMCR_ASV
#define STM32_IO2SV             PWR_SVMCR_IO2SV
#define STM32_USV               PWR_SVMCR_USV
#define STM32_AVM2EN            PWR_SVMCR_AVM2EN
#define STM32_AVM1EN            PWR_SVMCR_AVM1EN
#define STM32_IO2VMEN           PWR_SVMCR_IO2VMEN
#define STM32_UVMEN             PWR_SVMCR_UVMEN
#define STM32_PVDLS_POS         PWR_SVMCR_PVDLS_Pos
#define STM32_PVDLS_MASK        PWR_SVMCR_PVDLS_Msk
#define STM32_PVDLS_FIELD(n)    ((n) << STM32_PVDLS_POS)
#define STM32_PVDLS_2P0V        STM32_PVDLS_FIELD(0U)
#define STM32_PVDLS_2P2V        STM32_PVDLS_FIELD(1U)
#define STM32_PVDLS_2P4V        STM32_PVDLS_FIELD(2U)
#define STM32_PVDLS_2P5V        STM32_PVDLS_FIELD(3U)
#define STM32_PVDLS_2P6V        STM32_PVDLS_FIELD(4U)
#define STM32_PVDLS_2P8V        STM32_PVDLS_FIELD(5U)
#define STM32_PVDLS_2P9V        STM32_PVDLS_FIELD(6U)
#define STM32_PVDLS_PVD_IN      STM32_PVDLS_FIELD(7U)
#define STM32_PVDE              PWR_SVMCR_PVDE
/** @} */

/**
 * @name    FLASH_ACR register bits definitions
 * @{
 */
#define STM32_LATENCY_POS       FLASH_ACR_LATENCY_Pos
#define STM32_LATENCY_MASK      FLASH_ACR_LATENCY_Msk
#define STM32_LATENCY_FIELD(n)  ((n) << STM32_LATENCY_POS)
#define STM32_LATENCY_0WS       STM32_LATENCY_FIELD(0U)
#define STM32_LATENCY_1WS       STM32_LATENCY_FIELD(1U)
#define STM32_LATENCY_2WS       STM32_LATENCY_FIELD(2U)
#define STM32_LATENCY_3WS       STM32_LATENCY_FIELD(3U)
#define STM32_LATENCY_4WS       STM32_LATENCY_FIELD(4U)

#define STM32_PRFTEN            FLASH_ACR_PRFTEN
#define STM32_LPM               FLASH_ACR_LPM
#define STM32_PDREQ1            FLASH_ACR_PDREQ1
#define STM32_PDREQ2            FLASH_ACR_PDREQ2
#define STM32_SLEEP_PD          FLASH_ACR_SLEEP_PD
/** @} */

/**
 * @name    RCC_ICSCR1 register bits definitions
 * @{
 */
#define STM32_MSIHSINDIV_POS    RCC_ICSCR1_MSIHSINDIV_Pos
#define STM32_MSIHSINDIV_MASK   RCC_ICSCR1_MSIHSINDIV_Msk
#define STM32_MSIHSINDIV_FIELD(n) ((n) << STM32_MSIHSINDIV_POS)
#define STM32_MSIHSINDIV_HSE16  STM32_MSIHSINDIV_FIELD(0U)
#define STM32_MSIHSINDIV_HSE32  STM32_MSIHSINDIV_FIELD(1U)

#define STM32_MSIPLL1SEL_POS    RCC_ICSCR1_MSIPLL1SEL_Pos
#define STM32_MSIPLL1SEL_MASK   RCC_ICSCR1_MSIPLL1SEL_Msk
#define STM32_MSIPLL1SEL_FIELD(n) ((n) << STM32_MSIPLL1SEL_POS)
#define STM32_MSIPLL1SEL_LSE    STM32_MSIPLL1SEL_FIELD(0U)
#define STM32_MSIPLL1SEL_HSE    STM32_MSIPLL1SEL_FIELD(1U)

#define STM32_MSIPLL0SEL_POS    RCC_ICSCR1_MSIPLL0SEL_Pos
#define STM32_MSIPLL0SEL_MASK   RCC_ICSCR1_MSIPLL0SEL_Msk
#define STM32_MSIPLL0SEL_FIELD(n) ((n) << STM32_MSIPLL0SEL_POS)
#define STM32_MSIPLL0SEL_LSE    STM32_MSIPLL0SEL_FIELD(0U)
#define STM32_MSIPLL0SEL_HSE    STM32_MSIPLL0SEL_FIELD(1U)

#define STM32_MSIBIAS_POS       RCC_ICSCR1_MSIBIAS_Pos
#define STM32_MSIBIAS_MASK      RCC_ICSCR1_MSIBIAS_Msk
#define STM32_MSIBIAS_FIELD(n)  ((n) << STM32_MSIBIAS_POS)
#define STM32_MSIBIAS_CONTINUOUS STM32_MSIBIAS_FIELD(0U)
#define STM32_MSIBIAS_SAMPLIG   STM32_MSIBIAS_FIELD(1U)

#define STM32_MSIRGSEL_POS      RCC_ICSCR1_MSIRGSEL_Pos
#define STM32_MSIRGSEL_MASK     RCC_ICSCR1_MSIRGSEL_Msk
#define STM32_MSIRGSEL_FIELD(n) ((n) << STM32_MSIRGSEL_POS)
#define STM32_MSIRGSEL_CSR      STM32_MSIRGSEL_FIELD(0U)
#define STM32_MSIRGSEL_ICSCR1   STM32_MSIRGSEL_FIELD(1U)

#define STM32_MSIPLL1N_POS      RCC_ICSCR1_MSIPLL1N_Pos
#define STM32_MSIPLL1N_MASK     RCC_ICSCR1_MSIPLL1N_Msk
#define STM32_MSIPLL1N_FIELD(n) ((n) << STM32_MSIPLL1N_POS)

#define STM32_MSIKDIV_POS       RCC_ICSCR1_MSIKDIV_Pos
#define STM32_MSIKDIV_MASK      RCC_ICSCR1_MSIKDIV_Msk
#define STM32_MSIKDIV_FIELD(n)  ((n) << STM32_MSIKDIV_POS)

#define STM32_MSIKSEL_POS       RCC_ICSCR1_MSIKSEL_Pos
#define STM32_MSIKSEL_MASK      RCC_ICSCR1_MSIKSEL_Msk
#define STM32_MSIKSEL_FIELD(n)  ((n) << STM32_MSIKSEL_POS)
#define STM32_MSIKSEL_MSIRC0    STM32_MSIKSEL_FIELD(0U)
#define STM32_MSIKSEL_MSIRC1    STM32_MSIKSEL_FIELD(1U)

#define STM32_MSISDIV_POS       RCC_ICSCR1_MSISDIV_Pos
#define STM32_MSISDIV_MASK      RCC_ICSCR1_MSISDIV_Msk
#define STM32_MSISDIV_FIELD(n)  ((n) << STM32_MSISDIV_POS)

#define STM32_MSISSEL_POS       RCC_ICSCR1_MSISSEL_Pos
#define STM32_MSISSEL_MASK      RCC_ICSCR1_MSISSEL_Msk
#define STM32_MSISSEL_FIELD(n)  ((n) << STM32_MSISSEL_POS)
#define STM32_MSISSEL_MSIRC0    STM32_MSISSEL_FIELD(0U)
#define STM32_MSISSEL_MSIRC1    STM32_MSISSEL_FIELD(1U)
/** @} */

/**
 * @name    RCC_CFGR1 register bits definitions
 * @{
 */
#define STM32_SW_POS            RCC_CFGR1_SW_Pos
#define STM32_SW_MASK           RCC_CFGR1_SW_Msk
#define STM32_SW_FIELD(n)       ((n) << STM32_SW_POS)
#define STM32_SW_MSIS           STM32_SW_FIELD(0U)
#define STM32_SW_HSI16          STM32_SW_FIELD(1U)
#define STM32_SW_HSE            STM32_SW_FIELD(2U)

#define STM32_SWS_POS           RCC_CFGR1_SWS_Pos
#define STM32_SWS_MASK          RCC_CFGR1_SWS_Msk
#define STM32_SWS_FIELD(n)      ((n) << STM32_SWS_POS)
#define STM32_SWS_MSIS          STM32_SWS_FIELD(0U)
#define STM32_SWS_HSI16         STM32_SWS_FIELD(1U)
#define STM32_SWS_HSE           STM32_SWS_FIELD(2U)

#define STM32_STOPWUCK_POS      RCC_CFGR1_STOPWUCK_Pos
#define STM32_STOPWUCK_MASK     RCC_CFGR1_STOPWUCK_Msk
#define STM32_STOPWUCK_FIELD(n) ((n) << STM32_STOPWUCK_POS)
#define STM32_STOPWUCK_MSIS     STM32_STOPWUCK_FIELD(0U)
#define STM32_STOPWUCK_HSI16    STM32_STOPWUCK_FIELD(1U)

#define STM32_STOPKERWUCK_POS   RCC_CFGR1_STOPKERWUCK_Pos
#define STM32_STOPKERWUCK_MASK  RCC_CFGR1_STOPKERWUCK_Msk
#define STM32_STOPKERWUCK_FIELD(n) ((n) << STM32_STOPKERWUCK_POS)
#define STM32_STOPKERWUCK_MSIK  STM32_STOPKERWUCK_FIELD(0U)
#define STM32_STOPKERWUCK_HSI16 STM32_STOPKERWUCK_FIELD(1U)

#define STM32_MCO1SEL_POS       RCC_CFGR1_MCOSEL_Pos
#define STM32_MCO1SEL_MASK      RCC_CFGR1_MCOSEL_Msk
#define STM32_MCO1SEL_FIELD(n)  ((n) << STM32_MCO1SEL_POS)
#define STM32_MCO1SEL_OFF       STM32_MCO1PRE_FIELD(0U)
#define STM32_MCO1SEL_SYSCLK    STM32_MCO1PRE_FIELD(1U)
#define STM32_MCO1SEL_MSIS      STM32_MCO1PRE_FIELD(2U)
#define STM32_MCO1SEL_HSI16     STM32_MCO1PRE_FIELD(3U)
#define STM32_MCO1SEL_HSE       STM32_MCO1PRE_FIELD(4U)
#define STM32_MCO1SEL_LSI       STM32_MCO1PRE_FIELD(5U)
#define STM32_MCO1SEL_LSE       STM32_MCO1PRE_FIELD(6U)
#define STM32_MCO1SEL_HSI48     STM32_MCO1PRE_FIELD(7U)
#define STM32_MCO1SEL_MSIK      STM32_MCO1PRE_FIELD(8U)

#define STM32_MCO1PRE_POS       RCC_CFGR1_MCOPRE_Pos
#define STM32_MCO1PRE_MASK      RCC_CFGR1_MCOPRE_Msk
#define STM32_MCO1PRE_FIELD(n)  ((n) << STM32_MCO1PRE_POS)
#define STM32_MCO1PRE_DIV1      STM32_MCO1PRE_FIELD(0U)
#define STM32_MCO1PRE_DIV2      STM32_MCO1PRE_FIELD(1U)
#define STM32_MCO1PRE_DIV4      STM32_MCO1PRE_FIELD(2U)
#define STM32_MCO1PRE_DIV8      STM32_MCO1PRE_FIELD(3U)
#define STM32_MCO1PRE_DIV16     STM32_MCO1PRE_FIELD(4U)
#define STM32_MCO1PRE_DIV32     STM32_MCO1PRE_FIELD(5U)
#define STM32_MCO1PRE_DIV64     STM32_MCO1PRE_FIELD(6U)
#define STM32_MCO1PRE_DIV128    STM32_MCO1PRE_FIELD(7U)

#define STM32_MCO2SEL_POS       RCC_CFGR1_MCO2SEL_Pos
#define STM32_MCO2SEL_MASK      RCC_CFGR1_MCO2SEL_Msk
#define STM32_MCO2SEL_FIELD(n)  ((n) << STM32_MCO2SEL_POS)
#define STM32_MCO2SEL_OFF       STM32_MCO2SEL_FIELD(0U)
#define STM32_MCO2SEL_SYSCLK    STM32_MCO2SEL_FIELD(1U)
#define STM32_MCO2SEL_MSIS      STM32_MCO2SEL_FIELD(2U)
#define STM32_MCO2SEL_HSI16     STM32_MCO2SEL_FIELD(3U)
#define STM32_MCO2SEL_HSE       STM32_MCO2SEL_FIELD(4U)
#define STM32_MCO2SEL_LSI       STM32_MCO2SEL_FIELD(5U)
#define STM32_MCO2SEL_LSE       STM32_MCO2SEL_FIELD(6U)
#define STM32_MCO2SEL_HSI48     STM32_MCO2SEL_FIELD(7U)
#define STM32_MCO2SEL_MSIK      STM32_MCO2SEL_FIELD(8U)

#define STM32_MCO2PRE_POS       RCC_CFGR1_MCO2PRE_Pos
#define STM32_MCO2PRE_MASK      RCC_CFGR1_MCO2PRE_Msk
#define STM32_MCO2PRE_FIELD(n)  ((n) << STM32_MCO2PRE_POS)
#define STM32_MCO2PRE_DIV1      STM32_MCO2PRE_FIELD(0U)
#define STM32_MCO2PRE_DIV2      STM32_MCO2PRE_FIELD(1U)
#define STM32_MCO2PRE_DIV4      STM32_MCO2PRE_FIELD(2U)
#define STM32_MCO2PRE_DIV8      STM32_MCO2PRE_FIELD(3U)
#define STM32_MCO2PRE_DIV16     STM32_MCO2PRE_FIELD(4U)
#define STM32_MCO2PRE_DIV32     STM32_MCO2PRE_FIELD(5U)
#define STM32_MCO2PRE_DIV64     STM32_MCO2PRE_FIELD(6U)
#define STM32_MCO2PRE_DIV128    STM32_MCO2PRE_FIELD(7U)
/** @} */

/**
 * @name    RCC_CFGR4 register bits definitions
 * @{
 */
#define STM32_BOOSTSEL_POS      RCC_CFGR4_BOOSTSEL_Pos
#define STM32_BOOSTSEL_MASK     RCC_CFGR4_BOOSTSEL_Msk
#define STM32_BOOSTSEL_FIELD(n) ((n) << STM32_BOOSTSEL_POS)
#define STM32_BOOSTSEL_NOCLOCK  STM32_BOOSTSEL_FIELD(0U)
#define STM32_BOOSTSEL_MSIS     STM32_BOOSTSEL_FIELD(1U)
#define STM32_BOOSTSEL_HSI16    STM32_BOOSTSEL_FIELD(2U)
#define STM32_BOOSTSEL_HSE      STM32_BOOSTSEL_FIELD(3U)

#define STM32_BOOSTDIV_POS      RCC_CFGR4_BOOSTDIV_Pos
#define STM32_BOOSTDIV_MASK     RCC_CFGR4_BOOSTDIV_Msk
#define STM32_BOOSTDIV_FIELD(n) ((n) << STM32_BOOSTDIV_POS)
#define STM32_BOOSTDIV_DIV1     STM32_BOOSTDIV_FIELD(0U)
#define STM32_BOOSTDIV_DIV2     STM32_BOOSTDIV_FIELD(1U)
#define STM32_BOOSTDIV_DIV4     STM32_BOOSTDIV_FIELD(2U)
#define STM32_BOOSTDIV_DIV6     STM32_BOOSTDIV_FIELD(3U)
#define STM32_BOOSTDIV_DIV8     STM32_BOOSTDIV_FIELD(4U)
#define STM32_BOOSTDIV_DIV10    STM32_BOOSTDIV_FIELD(5U)
#define STM32_BOOSTDIV_DIV12    STM32_BOOSTDIV_FIELD(6U)
#define STM32_BOOSTDIV_DIV14    STM32_BOOSTDIV_FIELD(7U)
#define STM32_BOOSTDIV_DIV16    STM32_BOOSTDIV_FIELD(8U)
/** @} */

/**
 * @name    RCC_CCIPR1 register bits definitions
 * @{
 */
#define STM32_USART1SEL_POS     0
#define STM32_USART1SEL_MASK    (1U << STM32_USART1SEL_POS)
#define STM32_USART1SEL_FIELD(n) ((n) << STM32_USART1SEL_POS)
#define STM32_USART1SEL_PCLK2   STM32_USART1SEL_FIELD(0U)
#define STM32_USART1SEL_HSI16   STM32_USART1SEL_FIELD(1U)

#define STM32_USART3SEL_POS     2
#define STM32_USART3SEL_MASK    (1U << STM32_USART3SEL_POS)
#define STM32_USART3SEL_FIELD(n) ((n) << STM32_USART3SEL_POS)
#define STM32_USART3SEL_PCLK1   STM32_USART3SEL_FIELD(0U)
#define STM32_USART3SEL_HSI16   STM32_USART3SEL_FIELD(1U)

#define STM32_UART4SEL_POS      4
#define STM32_UART4SEL_MASK     (1U << STM32_UART4SEL_POS)
#define STM32_UART4SEL_FIELD(n) ((n) << STM32_UART4SEL_POS)
#define STM32_UART4SEL_PCLK1    STM32_UART4SEL_FIELD(0U)
#define STM32_UART4SEL_HSI16    STM32_UART4SEL_FIELD(1U)

#define STM32_UART5SEL_POS      6
#define STM32_UART5SEL_MASK     (1U << STM32_UART5SEL_POS)
#define STM32_UART5SEL_FIELD(n) ((n) << STM32_UART5SEL_POS)
#define STM32_UART5SEL_PCLK1    STM32_UART5SEL_FIELD(0U)
#define STM32_UART5SEL_HSI16    STM32_UART5SEL_FIELD(1U)

#define STM32_I3C1SEL_POS       8
#define STM32_I3C1SEL_MASK      (1U << STM32_I3C1SEL_POS)
#define STM32_I3C1SEL_FIELD(n)  ((n) << STM32_I3C1SEL_POS)
#define STM32_I3C1SEL_PCLK1     STM32_I3C1SEL_FIELD(0U)
#define STM32_I3C1SEL_MSIK      STM32_I3C1SEL_FIELD(1U)

#define STM32_I2C1SEL_POS       10
#define STM32_I2C1SEL_MASK      (1U << STM32_I2C1SEL_POS)
#define STM32_I2C1SEL_FIELD(n)  ((n) << STM32_I2C1SEL_POS)
#define STM32_I2C1SEL_PCLK1     STM32_I2C1SEL_FIELD(0U)
#define STM32_I2C1SEL_MSIK      STM32_I2C1SEL_FIELD(1U)

#define STM32_I2C2SEL_POS       12
#define STM32_I2C2SEL_MASK      (1U << STM32_I2C2SEL_POS)
#define STM32_I2C2SEL_FIELD(n)  ((n) << STM32_I2C2SEL_POS)
#define STM32_I2C2SEL_PCLK1     STM32_I2C2SEL_FIELD(0U)
#define STM32_I2C2SEL_MSIK      STM32_I2C2SEL_FIELD(1U)

#define STM32_I3C2SEL_POS       14
#define STM32_I3C2SEL_MASK      (1U << STM32_I3C2SEL_POS)
#define STM32_I3C2SEL_FIELD(n)  ((n) << STM32_I3C2SEL_POS)
#define STM32_I3C2SEL_PCLK3     STM32_I3C2SEL_FIELD(0U)
#define STM32_I3C2SEL_MSIK      STM32_I3C2SEL_FIELD(1U)

#define STM32_SPI2SEL_POS       16
#define STM32_SPI2SEL_MASK      (1U << STM32_SPI2SEL_POS)
#define STM32_SPI2SEL_FIELD(n)  ((n) << STM32_SPI2SEL_POS)
#define STM32_SPI2SEL_PCLK1     STM32_SPI2SEL_FIELD(0U)
#define STM32_SPI2SEL_MSIK      STM32_SPI2SEL_FIELD(1U)

#define STM32_LPTIM2SEL_POS     18
#define STM32_LPTIM2SEL_MASK    (3U << STM32_LPTIM2SEL_POS)
#define STM32_LPTIM2SEL_FIELD(n) ((n) << STM32_LPTIM2SEL_POS)
#define STM32_LPTIM2SEL_PCLK1   STM32_LPTIM2SEL_FIELD(0U)
#define STM32_LPTIM2SEL_LSI     STM32_LPTIM2SEL_FIELD(1U)
#define STM32_LPTIM2SEL_HSI16   STM32_LPTIM2SEL_FIELD(2U)
#define STM32_LPTIM2SEL_LSE     STM32_LPTIM2SEL_FIELD(3U)

#define STM32_SPI1SEL_POS       20
#define STM32_SPI1SEL_MASK      (1U << STM32_SPI1SEL_POS)
#define STM32_SPI1SEL_FIELD(n)  ((n) << STM32_SPI1SEL_POS)
#define STM32_SPI1SEL_PCLK2     STM32_SPI1SEL_FIELD(0U)
#define STM32_SPI1SEL_MSIK      STM32_SPI1SEL_FIELD(1U)

#define STM32_SYSTICKSEL_POS    22
#define STM32_SYSTICKSEL_MASK   (3U << STM32_SYSTICKSEL_POS)
#define STM32_SYSTICKSEL_FIELD(n) ((n) << STM32_SYSTICKSEL_POS)
#define STM32_SYSTICKSEL_HCLKDIV8 STM32_SYSTICKSEL_FIELD(0U)
#define STM32_SYSTICKSEL_LSI    STM32_SYSTICKSEL_FIELD(1U)
#define STM32_SYSTICKSEL_LSE    STM32_SYSTICKSEL_FIELD(2U)

#define STM32_FDCAN1SEL_POS     24
#define STM32_FDCAN1SEL_MASK    (1U << STM32_FDCAN1SEL_POS)
#define STM32_FDCAN1SEL_FIELD(n) ((n) << STM32_FDCAN1SEL_POS)
#define STM32_FDCAN1SEL_IGNORE  0xFFFFFFFFU
#define STM32_FDCAN1SEL_SYSCLK  STM32_FDCAN1SEL_FIELD(0U)
#define STM32_FDCAN1SEL_MSIK    STM32_FDCAN1SEL_FIELD(1U)

#define STM32_ICLKSEL_POS       26
#define STM32_ICLKSEL_MASK      (3U << STM32_ICLKSEL_POS)
#define STM32_ICLKSEL_FIELD(n)  ((n) << STM32_ICLKSEL_POS)
#define STM32_ICLKSEL_HSI48     STM32_ICLKSEL_FIELD(0U)
#define STM32_ICLKSEL_MSIK      STM32_ICLKSEL_FIELD(1U)
#define STM32_ICLKSEL_HSE       STM32_ICLKSEL_FIELD(2U)
#define STM32_ICLKSEL_SYSCLK    STM32_ICLKSEL_FIELD(3U)

#define STM32_USB1SEL_POS       28
#define STM32_USB1SEL_MASK      (1U << STM32_USB1SEL_POS)
#define STM32_USB1SEL_FIELD(n)  ((n) << STM32_USB1SEL_POS)
#define STM32_USB1SEL_ICLK      STM32_USB1SEL_FIELD(0U)
#define STM32_USB1SEL_ICLKDIV2  STM32_USB1SEL_FIELD(1U)

#define STM32_TIMICSEL_POS      29
#define STM32_TIMICSEL_MASK     (7U << STM32_TIMICSEL_POS)
#define STM32_TIMICSEL_FIELD(n) ((n) << STM32_TIMICSEL_POS)
#define STM32_TIMICSEL_NOCLOCK                  STM32_TIMICSEL_FIELD(0U)
#define STM32_TIMICSEL_HSI256_MSIS1024_MSIS4    STM32_TIMICSEL_FIELD(4U)
#define STM32_TIMICSEL_HSI256_MSIS1024_MSIK4    STM32_TIMICSEL_FIELD(5U)
#define STM32_TIMICSEL_HSI256_MSIK1024_MSIS4    STM32_TIMICSEL_FIELD(6U)
#define STM32_TIMICSEL_HSI256_MSIK1024_MSIK4    STM32_TIMICSEL_FIELD(7U)
/** @} */

/**
 * @name    RCC_CCIPR2 register bits definitions
 * @{
 */

#define STM32_ADF1SEL_POS       0
#define STM32_ADF1SEL_MASK      (3U << STM32_ADF1SEL_POS)
#define STM32_ADF1SEL_FIELD(n)  ((n) << STM32_ADF1SEL_POS)
#define STM32_ADF1SEL_HCLK      STM32_ADF1SEL_FIELD(0U)
#define STM32_ADF1SEL_AUDIOCLK  STM32_ADF1SEL_FIELD(1U)
#define STM32_ADF1SEL_HSE       STM32_ADF1SEL_FIELD(2U)

#define STM32_SPI3SEL_POS       3
#define STM32_SPI3SEL_MASK      (1U << STM32_SPI3SEL_POS)
#define STM32_SPI3SEL_FIELD(n)  ((n) << STM32_SPI3SEL_POS)
#define STM32_SPI3SEL_PCLK1     STM32_SPI3SEL_FIELD(0U)
#define STM32_SPI3SEL_MSIK      STM32_SPI3SEL_FIELD(1U)

#define STM32_SAI1SEL_POS       5
#define STM32_SAI1SEL_MASK      (3U << STM32_SAI1SEL_POS)
#define STM32_SAI1SEL_FIELD(n)  ((n) << STM32_SAI1SEL_POS)
#define STM32_SAI1SEL_MSIK      STM32_SAI1SEL_FIELD(0U)
#define STM32_SAI1SEL_AUDIOCLK  STM32_SAI1SEL_FIELD(1U)
#define STM32_SAI1SEL_HSE       STM32_SAI1SEL_FIELD(2U)

#define STM32_RNGSEL_POS        11
#define STM32_RNGSEL_MASK       (1U << STM32_RNGSEL_POS)
#define STM32_RNGSEL_FIELD(n)   ((n) << STM32_RNGSEL_POS)
#define STM32_RNGSEL_IGNORE     0xFFFFFFFFU
#define STM32_RNGSEL_HSI48      STM32_RNGSEL_FIELD(0U)
#define STM32_RNGSEL_MSIK       STM32_RNGSEL_FIELD(1U)

#define STM32_ADCDACPRE_POS     12
#define STM32_ADCDACPRE_MASK    (15U << STM32_ADCDACPRE_POS)
#define STM32_ADCDACPRE_FIELD(n) ((n) << STM32_ADCDACPRE_POS)
#define STM32_ADCDACPRE_ICLK        STM32_ADCDACPRE_FIELD(0U)
#define STM32_ADCDACPRE_ICLKDIV2    STM32_ADCDACPRE_FIELD(1U)
#define STM32_ADCDACPRE_ICLKDIV4    STM32_ADCDACPRE_FIELD(8U)
#define STM32_ADCDACPRE_ICLKDIV8    STM32_ADCDACPRE_FIELD(9U)
#define STM32_ADCDACPRE_ICLKDIV16   STM32_ADCDACPRE_FIELD(10U)
#define STM32_ADCDACPRE_ICLKDIV32   STM32_ADCDACPRE_FIELD(11U)
#define STM32_ADCDACPRE_ICLKDIV64   STM32_ADCDACPRE_FIELD(12U)
#define STM32_ADCDACPRE_ICLKDIV128  STM32_ADCDACPRE_FIELD(13U)
#define STM32_ADCDACPRE_ICLKDIV256  STM32_ADCDACPRE_FIELD(14U)
#define STM32_ADCDACPRE_ICLKDIV512  STM32_ADCDACPRE_FIELD(15U)

#define STM32_ADCDACSEL_POS     16
#define STM32_ADCDACSEL_MASK    (3U << STM32_ADCDACSEL_POS)
#define STM32_ADCDACSEL_FIELD(n) ((n) << STM32_ADCDACSEL_POS)
#define STM32_ADCDACSEL_HCLK    STM32_ADCDACSEL_FIELD(0U)
#define STM32_ADCDACSEL_HSE     STM32_ADCDACSEL_FIELD(1U)
#define STM32_ADCDACSEL_MSIK    STM32_ADCDACSEL_FIELD(2U)

#define STM32_DAC1SHSEL_POS     19
#define STM32_DAC1SHSEL_MASK    (1U << STM32_DAC1SHSEL_POS)
#define STM32_DAC1SHSEL_FIELD(n) ((n) << STM32_DAC1SHSEL_POS)
#define STM32_DAC1SHSEL_IGNORE  0xFFFFFFFFU
#define STM32_DAC1SHSEL_LSE     STM32_DAC1SHSEL_FIELD(0U)
#define STM32_DAC1SHSEL_LSI     STM32_DAC1SHSEL_FIELD(1U)

#define STM32_OCTOSPISEL_POS    20
#define STM32_OCTOSPISEL_MASK   (3U << STM32_OCTOSPISEL_POS)
#define STM32_OCTOSPISEL_FIELD(n) ((n) << STM32_OCTOSPISEL_POS)
#define STM32_OCTOSPISEL_SYSCLK STM32_OCTOSPISEL_FIELD(0U)
#define STM32_OCTOSPISEL_MSIK   STM32_OCTOSPISEL_FIELD(1U)
/** @} */

/**
 * @name    RCC_CCIPR3 register bits definitions
 * @{
 */
#define STM32_LPUART1SEL_POS    0
#define STM32_LPUART1SEL_MASK   (3U << STM32_LPUART1SEL_POS)
#define STM32_LPUART1SEL_FIELD(n) ((n) << STM32_LPUART1SEL_POS)
#define STM32_LPUART1SEL_PCLK3  STM32_LPUART1SEL_FIELD(0U)
#define STM32_LPUART1SEL_HSI16  STM32_LPUART1SEL_FIELD(1U)
#define STM32_LPUART1SEL_LSE    STM32_LPUART1SEL_FIELD(2U)
#define STM32_LPUART1SEL_MSIK   STM32_LPUART1SEL_FIELD(3U)

#define STM32_I2C3SEL_POS       6
#define STM32_I2C3SEL_MASK      (1U << STM32_I2C3SEL_POS)
#define STM32_I2C3SEL_FIELD(n)  ((n) << STM32_I2C3SEL_POS)
#define STM32_I2C3SEL_PCLK3     STM32_I2C3SEL_FIELD(0U)
#define STM32_I2C3SEL_MSIK      STM32_I2C3SEL_FIELD(1U)

#define STM32_LPTIM34SEL_POS    8
#define STM32_LPTIM34SEL_MASK   (3U << STM32_LPTIM34SEL_POS)
#define STM32_LPTIM34SEL_FIELD(n) ((n) << STM32_LPTIM34SEL_POS)
#define STM32_LPTIM34SEL_MSIK   STM32_LPTIM34SEL_FIELD(0U)
#define STM32_LPTIM34SEL_LSI    STM32_LPTIM34SEL_FIELD(1U)
#define STM32_LPTIM34SEL_HSI16  STM32_LPTIM34SEL_FIELD(2U)
#define STM32_LPTIM34SEL_LSE    STM32_LPTIM34SEL_FIELD(3U)

#define STM32_LPTIM1SEL_POS     10
#define STM32_LPTIM1SEL_MASK    (3U << STM32_LPTIM1SEL_POS)
#define STM32_LPTIM1SEL_FIELD(n) ((n) << STM32_LPTIM1SEL_POS)
#define STM32_LPTIM1SEL_MSIK    STM32_LPTIM1SEL_FIELD(0U)
#define STM32_LPTIM1SEL_LSI     STM32_LPTIM1SEL_FIELD(1U)
#define STM32_LPTIM1SEL_HSI16   STM32_LPTIM1SEL_FIELD(2U)
#define STM32_LPTIM1SEL_LSE     STM32_LPTIM1SEL_FIELD(3U)
/** @} */

/**
 * @name    RCC_BDCR register bits definitions
 * @{
 */
#define STM32_RTCSEL_POS        8
#define STM32_RTCSEL_MASK       (3U << STM32_RTCSEL_POS)
#define STM32_RTCSEL_FIELD(n)   ((n) << STM32_RTCSEL_POS)
#define STM32_RTCSEL_NOCLOCK    STM32_RTCSEL_FIELD(0U)
#define STM32_RTCSEL_LSE        STM32_RTCSEL_FIELD(1U)
#define STM32_RTCSEL_LSI        STM32_RTCSEL_FIELD(2U)
#define STM32_RTCSEL_HSEDIV     STM32_RTCSEL_FIELD(3U)

#define STM32_LSCOSEL_POS       24
#define STM32_LSCOSEL_MASK      (3U << STM32_LSCOSEL_POS)
#define STM32_LSCOSEL_FIELD(n)  ((n) << STM32_LSCOSEL_POS)
#define STM32_LSCOSEL_NOCLOCK   STM32_LSCOSEL_FIELD(0U)
#define STM32_LSCOSEL_LSE       STM32_LSCOSEL_FIELD(1U)
#define STM32_LSCOSEL_LSI       STM32_LSCOSEL_FIELD(3U)
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
 * @brief   Enables the dynamic clock handling.
 */
#if !defined(STM32_CLOCK_DYNAMIC) || defined(__DOXYGEN__)
#define STM32_CLOCK_DYNAMIC                 FALSE
#endif

/**
 * @brief   PWR VOSR register initialization value.
 * @note    BOOSTEN is calculated internally, do not specify it here.
 */
#if !defined(STM32_PWR_VOSR) || defined(__DOXYGEN__)
#define STM32_PWR_VOSR                      (STM32_VOS_RANGE1)
#endif

/**
 * @brief   PWR CR3 register initialization value.
 */
#if !defined(STM32_PWR_CR3) || defined(__DOXYGEN__)
#define STM32_PWR_CR3                       (STM32_FSTEN | STM32_REGSEL)
#endif

/**
 * @brief   PWR CR3 register initialization value.
 */
#if !defined(STM32_PWR_SVMCR) || defined(__DOXYGEN__)
#define STM32_PWR_SVMCR                     (PWR_SVMCR_ASV | PWR_SVMCR_USV | \
                                             PWR_SVMCR_AVM1EN | PWR_SVMCR_AVM2EN | \
                                             PWR_SVMCR_UVMEN)
#endif

/**
 * @brief   PWR WUCR1 register initialization value.
 */
#if !defined(STM32_PWR_WUCR1) || defined(__DOXYGEN__)
#define STM32_PWR_WUCR1                     (0U)
#endif

/**
 * @brief   PWR WUCR2 register initialization value.
 */
#if !defined(STM32_PWR_WUCR2) || defined(__DOXYGEN__)
#define STM32_PWR_WUCR2                     (0U)
#endif

/**
 * @brief   PWR WUCR3 register initialization value.
 */
#if !defined(STM32_PWR_WUCR3) || defined(__DOXYGEN__)
#define STM32_PWR_WUCR3                     (0U)
#endif

/**
 * @brief   PWR BDCR register initialization value.
 */
#if !defined(STM32_PWR_BDCR) || defined(__DOXYGEN__)
#define STM32_PWR_BDCR                      (0U)
#endif

/**
 * @brief   PWR SECCFGR register initialization value.
 */
#if !defined(STM32_PWR_SECCFGR) || defined(__DOXYGEN__)
#define STM32_PWR_SECCFGR                   (0U)
#endif

/**
 * @brief   PWR PRIVCFGR register initialization value.
 */
#if !defined(STM32_PWR_PRIVCFGR) || defined(__DOXYGEN__)
#define STM32_PWR_PRIVCFGR                  (0U)
#endif

/**
 * @brief   PWR APCR register initialization value.
 */
#if !defined(STM32_PWR_APCR) || defined(__DOXYGEN__)
#define STM32_PWR_APCR                      (0U)
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
 * @brief   PWR PUCRE register initialization value.
 */
#if !defined(STM32_PWR_PUCRE) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRE                     (0U)
#endif

/**
 * @brief   PWR PDCRE register initialization value.
 */
#if !defined(STM32_PWR_PDCRE) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRE                     (0U)
#endif

/**
 * @brief   PWR PUCRG register initialization value.
 */
#if !defined(STM32_PWR_PUCRG) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRG                     (0U)
#endif

/**
 * @brief   PWR PDCRG register initialization value.
 */
#if !defined(STM32_PWR_PDCRG) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRG                     (0U)
#endif

/**
 * @brief   PWR PUCRH register initialization value.
 */
#if !defined(STM32_PWR_PUCRH) || defined(__DOXYGEN__)
#define STM32_PWR_PUCRH                     (0U)
#endif

/**
 * @brief   PWR PDCRH register initialization value.
 */
#if !defined(STM32_PWR_PDCRH) || defined(__DOXYGEN__)
#define STM32_PWR_PDCRH                     (0U)
#endif

/**
 * @brief   PWR I3CPUCR1 register initialization value.
 */
#if !defined(STM32_PWR_I3CPUCR1) || defined(__DOXYGEN__)
#define STM32_PWR_I3CPUCR1                  (0U)
#endif

/**
 * @brief   PWR I3CPUCR2 register initialization value.
 */
#if !defined(STM32_PWR_I3CPUCR2) || defined(__DOXYGEN__)
#define STM32_PWR_I3CPUCR2                  (0U)
#endif

/**
 * @brief   FLASH ACR register initialization value.
 * @note    Do not specify the LATENCY bits because those are calculated
 *          depending on other settings and ORed to this value.
 */
#if !defined(STM32_FLASH_ACR) || defined(__DOXYGEN__)
#define STM32_FLASH_ACR                     (STM32_PRFTEN)
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM32_HSI16_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI16_ENABLED                 FALSE
#endif

/**
 * @brief   Enables or disables the HSI clock source in STOP mode.
 */
#if !defined(STM32_HSIKERON_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSIKERON_ENABLED              FALSE
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
#define STM32_LSE_ENABLED                   TRUE
#endif

/**
 * @brief   Mode of the MSIRC0 96MHz clock source.
 * @note    It affects bits in various registers.
 */
#if !defined(STM32_MSIRC0_MODE) || defined(__DOXYGEN__)
#define STM32_MSIRC0_MODE                   STM32_MSIRC0_PLL_LSE
#endif

/**
 * @brief   Enables or disables the MSIRC1 24MHz clock source.
 * @note    It affects bits in various registers.
 */
#if !defined(STM32_MSIRC1_MODE) || defined(__DOXYGEN__)
#define STM32_MSIRC1_MODE                   STM32_MSIRC1_FREE
#endif

/**
 * @brief   MSIRC1 PLL correction factors.
 * @details Valid values are 0, 2 and 3 which correspond to slightly
 *          different frequencies when MSIRC1 is used as source, frequencies
 *          are in MHz:
 *          - 0 LSE: 23.986, 11.993, 5.997, 2.998.
 *          - 2 LSE: 22.577, 11.289, 5.644, 2.822.
 *          - 3 LSE: 24.576, 12.288, 6.144, 3.072.
 *          - 0 HSE: 24.016, 12.008, 6.004, 3.002.
 *          - 2 HSE: 22.581, 11.290, 5.645, 2.823.
 *          - 3 HSE: 24.577, 12.289, 6.144, 3.072.
 *          .
 * @note    MSIRC1 does not allow for round numbers, source from MSIRC0 to
 *          obtain precise 96MHz, 48MHz, 24MHz, 12MHz frequencies.
 */
#if !defined(STM32_MSIPLL1N_VALUE) || defined(__DOXYGEN__)
#define STM32_MSIPLL1N_VALUE                0
#endif

/**
 * @brief   Source and divide factor for MSIS.
 */
#if !defined(STM32_MSIS_SRCDIV) || defined(__DOXYGEN__)
#define STM32_MSIS_SRCDIV                   STM32_MSIS_IRC0_DIV1
#endif

/**
 * @brief   Source and divide factor for MSIK.
 */
#if !defined(STM32_MSIK_SRCDIV) || defined(__DOXYGEN__)
#define STM32_MSIK_SRCDIV                   STM32_MSIK_IRC0_DIV1
#endif

/**
 * @brief   PLL bias setting.
 */
#if !defined(STM32_MSIBIAS) || defined(__DOXYGEN__)
#define STM32_MSIBIAS                       STM32_MSIBIAS_CONTINUOUS
#endif

/**
 * @brief   Main clock source selection.
 */
#if !defined(STM32_SW) || defined(__DOXYGEN__)
#define STM32_SW                            STM32_SW_MSIS
#endif

/**
 * @brief   AHB prescaler value.
 */
#if !defined(STM32_HPRE) || defined(__DOXYGEN__)
#define STM32_HPRE                          STM32_HPRE_DIV1
#endif

/**
 * @brief   APB1 prescaler value.
 */
#if !defined(STM32_PPRE1) || defined(__DOXYGEN__)
#define STM32_PPRE1                         STM32_PPRE1_DIV1
#endif

/**
 * @brief   APB2 prescaler value.
 */
#if !defined(STM32_PPRE2) || defined(__DOXYGEN__)
#define STM32_PPRE2                         STM32_PPRE2_DIV1
#endif

/**
 * @brief   APB3 prescaler value.
 */
#if !defined(STM32_PPRE3) || defined(__DOXYGEN__)
#define STM32_PPRE3                         STM32_PPRE3_DIV1
#endif

/**
 * @brief   System clock source after STOP.
 */
#if !defined(STM32_STOPWUCK) || defined(__DOXYGEN__)
#define STM32_STOPWUCK                      STM32_STOPWUCK_MSIS
#endif

/**
 * @brief   Kernel clock source after STOP.
 */
#if !defined(STM32_STOPKERWUCK) || defined(__DOXYGEN__)
#define STM32_STOPKERWUCK                   STM32_STOPKERWUCK_MSIK
#endif

/**
 * @brief   MCO1 clock source.
 */
#if !defined(STM32_MCO1SEL) || defined(__DOXYGEN__)
#define STM32_MCO1SEL                       STM32_MCO1SEL_OFF
#endif

/**
 * @brief   MCO1 divider setting.
 * @brief   Valid values are 1, 2, 4, 8, 16, 32, 64, 128.
 */
#if !defined(STM32_MCO1PRE_VALUE) || defined(__DOXYGEN__)
#define STM32_MCO1PRE_VALUE                 1
#endif

/**
 * @brief   MCO2 clock source.
 */
#if !defined(STM32_MCO2SEL) || defined(__DOXYGEN__)
#define STM32_MCO2SEL                       STM32_MCO2SEL_OFF
#endif

/**
 * @brief   MCO1 divider setting.
 * @brief   Valid values are 1, 2, 4, 8, 16, 32, 64, 128.
 */
#if !defined(STM32_MCO2PRE_VALUE) || defined(__DOXYGEN__)
#define STM32_MCO2PRE_VALUE                 1
#endif

/**
 * @brief   LSCO clock source.
 */
#if !defined(STM32_LSCOSEL) || defined(__DOXYGEN__)
#define STM32_LSCOSEL                       STM32_LSCOSEL_NOCLOCK
#endif

/**
 * @brief   TIMICSEL clock source.
 */
#if !defined(STM32_TIMICSEL) || defined(__DOXYGEN__)
#define STM32_TIMICSEL                      STM32_TIMICSEL_NOCLOCK
#endif

/**
 * @brief   USART1 clock source.
 */
#if !defined(STM32_USART1SEL) || defined(__DOXYGEN__)
#define STM32_USART1SEL                     STM32_USART1SEL_PCLK2
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
 * @brief   LPUART1 clock source.
 */
#if !defined(STM32_LPUART1SEL) || defined(__DOXYGEN__)
#define STM32_LPUART1SEL                    STM32_LPUART1SEL_PCLK3
#endif

/**
 * @brief   LPTIM1 clock source.
 */
#if !defined(STM32_LPTIM1SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_MSIK
#endif

/**
 * @brief   LPTIM2 clock source.
 */
#if !defined(STM32_LPTIM2SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM2SEL                     STM32_LPTIM2SEL_PCLK1
#endif

/**
 * @brief   LPTIM34 clock source.
 */
#if !defined(STM32_LPTIM34SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM34SEL                    STM32_LPTIM34SEL_MSIK
#endif

/**
 * @brief   SPI1 clock source.
 */
#if !defined(STM32_SPI1SEL) || defined(__DOXYGEN__)
#define STM32_SPI1SEL                       STM32_SPI1SEL_PCLK2
#endif

/**
 * @brief   SPI2 clock source.
 */
#if !defined(STM32_SPI2SEL) || defined(__DOXYGEN__)
#define STM32_SPI2SEL                       STM32_SPI2SEL_PCLK1
#endif

/**
 * @brief   SPI3 clock source.
 */
#if !defined(STM32_SPI3SEL) || defined(__DOXYGEN__)
#define STM32_SPI3SEL                       STM32_SPI3SEL_PCLK1
#endif

/**
 * @brief   OCTOSPI clock source.
 */
#if !defined(STM32_OCTOSPISEL) || defined(__DOXYGEN__)
#define STM32_OCTOSPISEL                    STM32_OCTOSPISEL_SYSCLK
#endif

/**
 * @brief   SYSTICK clock source.
 */
#if !defined(STM32_SYSTICKSEL) || defined(__DOXYGEN__)
#define STM32_SYSTICKSEL                    STM32_SYSTICKSEL_HCLKDIV8
#endif

/**
 * @brief   SDMMC1 clock source.
 */
#if !defined(STM32_ICLKSEL) || defined(__DOXYGEN__)
#define STM32_ICLKSEL                       STM32_ICLKSEL_SYSCLK
#endif

/**
 * @brief   USB1 clock source.
 */
#if !defined(STM32_USB1SEL) || defined(__DOXYGEN__)
#define STM32_USB1SEL                       STM32_USB1SEL_ICLKDIV2
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
#define STM32_I2C3SEL                       STM32_I2C3SEL_PCLK3
#endif

/**
 * @brief   I3C1 clock source.
 */
#if !defined(STM32_I3C1SEL) || defined(__DOXYGEN__)
#define STM32_I3C1SEL                       STM32_I3C1SEL_PCLK1
#endif

/**
 * @brief   I3C2 clock source.
 */
#if !defined(STM32_I3C2SEL) || defined(__DOXYGEN__)
#define STM32_I3C2SEL                       STM32_I3C2SEL_PCLK3
#endif

/**
 * @brief   ADCDACSEL clock source.
 */
#if !defined(STM32_ADCDACSEL) || defined(__DOXYGEN__)
#define STM32_ADCDACSEL                     STM32_ADCDACSEL_HCLK
#endif

/**
 * @brief   DAC1SHSEL clock source.
 */
#if !defined(STM32_DAC1SHSEL) || defined(__DOXYGEN__)
#define STM32_DAC1SHSEL                     STM32_DAC1SHSEL_IGNORE
#endif

/**
 * @brief   ADCDACPRE clock source.
 */
#if !defined(STM32_ADCDACPRE) || defined(__DOXYGEN__)
#define STM32_ADCDACPRE                     STM32_ADCDACPRE_ICLK
#endif

/**
 * @brief   RNG clock source.
 */
#if !defined(STM32_RNGSEL) || defined(__DOXYGEN__)
#define STM32_RNGSEL                        STM32_RNGSEL_IGNORE
#endif

/**
 * @brief   FDCAN1 clock source.
 */
#if !defined(STM32_FDCAN1SEL) || defined(__DOXYGEN__)
#define STM32_FDCAN1SEL                     STM32_FDCAN1SEL_SYSCLK
#endif

/**
 * @brief   SAI1 clock source.
 */
#if !defined(STM32_SAI1SEL) || defined(__DOXYGEN__)
#define STM32_SAI1SEL                       STM32_SAI1SEL_MSIK
#endif

/**
 * @brief   ADF1 clock source.
 */
#if !defined(STM32_ADF1SEL) || defined(__DOXYGEN__)
#define STM32_ADF1SEL                       STM32_ADF1SEL_HCLK
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
#if 0 //STM32_CLOCK_DYNAMIC == TRUE
#define HAL_LLD_USE_CLOCK_MANAGEMENT
#endif

/*
 * Configuration-related checks.
 */
#if !defined(STM32U3xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32U3xx_MCUCONF not defined"
#endif

#if defined(STM32U375xx) && !defined(STM32U375_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32U375_MCUCONF not defined"

#elif defined(STM32U385xx) && !defined(STM32U385_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32U385_MCUCONF not defined"

#endif

/* Device limits.*/
#include "stm32_limits.h"

/* ICache handler.*/
#include "stm32_icache.inc"

/* Clock handlers.*/
#include "stm32_lsi.inc"
#include "stm32_hsi48.inc"
#include "stm32_hsi16.inc"
#include "stm32_lse.inc"
#include "stm32_hse.inc"

/*
 * MSIS enable check.
 */
#if (STM32_SW           == STM32_SW_MSIS)           ||                      \
    (STM32_MCO1SEL      == STM32_MCO1SEL_MSIS)      ||                      \
    (STM32_MCO2SEL      == STM32_MCO2SEL_MSIS)      ||                      \
    defined(__DOXYGEN__)
  /**
   * @brief   MSIS activation flag.
   */
  #define STM32_ACTIVATE_MSIS       TRUE
#else

  #define STM32_ACTIVATE_MSIS       FALSE
#endif

/*
 * MSIK enable check.
 */
#if (STM32_MCO1SEL      == STM32_MCO1SEL_MSIK)      ||                      \
    (STM32_MCO2SEL      == STM32_MCO2SEL_MSIK)      ||                      \
    (STM32_LPUART1SEL   == STM32_LPUART1SEL_MSIK)   ||                      \
    (STM32_LPTIM1SEL    == STM32_LPTIM1SEL_MSIK)    ||                      \
    (STM32_LPTIM2SEL    == STM32_LPTIM2SEL_MSIK)    ||                      \
    (STM32_LPTIM34SEL   == STM32_LPTIM34SEL_MSIK)   ||                      \
    (STM32_SPI1SEL      == STM32_SPI1SEL_MSIK)      ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_MSIK)      ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_MSIK)      ||                      \
    (STM32_I2C1SEL      == STM32_I2C1SEL_MSIK)      ||                      \
    (STM32_I2C2SEL      == STM32_I2C2SEL_MSIK)      ||                      \
    (STM32_I2C3SEL      == STM32_I2C3SEL_MSIK)      ||                      \
    (STM32_I3C1SEL      == STM32_I3C1SEL_MSIK)      ||                      \
    (STM32_I3C2SEL      == STM32_I3C2SEL_MSIK)      ||                      \
    (STM32_ADCDACSEL    == STM32_ADCDACSEL_MSIK)    ||                      \
    (STM32_ADF1SEL      == STM32_ADF1SEL_MSIK)      ||                      \
    (STM32_FDCAN1SEL    == STM32_FDCAN1SEL_MSIK)    ||                      \
    (STM32_ICLKSEL      == STM32_ICLKSEL_MSIK)      ||                      \
    (STM32_OCTOSPISEL   == STM32_OCTOSPISEL_MSIK)   ||                      \
    (STM32_RNGSEL       == STM32_RNGSEL_MSIK)       ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_MSIK)      ||                      \
    defined(__DOXYGEN__)
  /**
   * @brief   MSIK activation flag.
   */
  #define STM32_ACTIVATE_MSIK       TRUE
#else

  #define STM32_ACTIVATE_MSIK       FALSE
#endif

/*
 * LSI related checks.
 */
#if STM32_LSI_ENABLED
#else /* !STM32_LSI_ENABLED */

  #if STM32_MCO2SEL == STM32_MCO2SEL_LSI
    #error "LSI not enabled, required by STM32_MCO2SEL"
  #endif

  #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI)
    #error "LSI not enabled, required by STM32_LPTIM1SEL"
  #endif
  #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSI)
    #error "LSI not enabled, required by STM32_LPTIM2SEL"
  #endif
  #if (STM32_LPTIM34SEL == STM32_LPTIM34SEL_LSI)
    #error "LSI not enabled, required by STM32_LPTIM34SEL"
  #endif

  #if STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSI
    #error "LSI not enabled, required by STM32_SYSTICKSEL"
  #endif

  #if STM32_DAC1SHSEL == STM32_DAC1SHSEL_LSI
    #error "LSI not enabled, required by STM32_DAC1SHSEL"
  #endif

  #if HAL_USE_RTC && (STM32_RTCSEL == STM32_RTCSEL_LSI)
    #error "LSI not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSI
    #error "LSI not enabled, required by STM32_LSCOSEL"
  #endif

#endif /* !STM32_LSI_ENABLED */

/*
 * HSI48 related checks.
 */
#if STM32_HSI48_ENABLED
#else /* !STM32_HSI48_ENABLED */

  #if STM32_MCO1SEL == STM32_MCO1SEL_HSI48
    #error "HSI48 not enabled, required by STM32_MCO1SEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_HSI48
    #error "HSI48 not enabled, required by STM32_MCO2SEL"
  #endif

  #if STM32_ICLKSEL == STM32_ICLKSEL_HSI48
    #error "HSI48 not enabled, required by STM32_ICLKSEL"
  #endif

  #if STM32_RNGSEL == STM32_RNGSEL_HSI48
    #error "HSI48 not enabled, required by STM32_RNGSEL"
  #endif

  /* TODO: Check on CRS IP.*/

#endif /* !STM32_HSI48_ENABLED */

/*
 * HSI related checks.
 */
#if STM32_HSI16_ENABLED
#else /* !STM32_HSI16_ENABLED */

  #if STM32_SW == STM32_SW_HSI16
    #error "HSI not enabled, required by STM32_SW"
  #endif

  #if STM32_MCO1SEL == STM32_MCO1SEL_HSI16
    #error "HSI not enabled, required by STM32_MCO1SEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_HSI16
    #error "HSI not enabled, required by STM32_MCO2SEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_HSI16)
    #error "HSI not enabled, required by STM32_USART1SEL"
  #endif
  #if (STM32_USART3SEL == STM32_USART3SEL_HSI16)
    #error "HSI not enabled, required by STM32_USART3SEL"
  #endif
  #if (STM32_UART4SEL == STM32_UART4SEL_HSI16)
    #error "HSI not enabled, required by STM32_UART4SEL"
  #endif
  #if (STM32_UART5SEL == STM32_UART5SEL_HSI16)
    #error "HSI not enabled, required by STM32_UART5SEL"
  #endif
  #if (STM32_LPUART1SEL == STM32_LPUART1SEL_HSI16)
    #error "HSI not enabled, required by STM32_LPUART1SEL"
  #endif

  #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI16)
    #error "HSI not enabled, required by STM32_LPTIM1SEL"
  #endif
  #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_HSI16)
    #error "HSI not enabled, required by STM32_LPTIM2SEL"
  #endif
  #if (STM32_LPTIM34SEL == STM32_LPTIM34SEL_HSI16)
    #error "HSI not enabled, required by STM32_LPTIM34SEL"
  #endif

#endif /* !STM32_HSI16_ENABLED */

/*
 * LSE related checks.
 */
#if STM32_LSE_ENABLED
#else /* !STM32_LSE_ENABLED */

  #if STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_LSE
    #error "LSE not enabled, required by STM32_MSIRC0_MODE"
  #endif

  #if STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_LSE_FAST
    #error "LSE not enabled, required by STM32_MSIRC1_MODE"
  #endif

  #if STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_LSE
    #error "LSE not enabled, required by STM32_MSIRC1_MODE"
  #endif

  #if STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_LSE_FAST
    #error "LSE not enabled, required by STM32_MSIRC1_MODE"
  #endif

  #if STM32_MCO1SEL == STM32_MCO1SEL_LSE
    #error "LSE not enabled, required by STM32_MCO1SEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_LSE
    #error "LSE not enabled, required by STM32_MCO1SEL"
  #endif

  #if (STM32_LPUART1SEL == STM32_LPUART1SEL_LSE)
    #error "LSE not enabled, required by STM32_LPUART1SEL"
  #endif

  #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM1SEL"
  #endif
  #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM2SEL"
  #endif
  #if (STM32_LPTIM34SEL == STM32_LPTIM34SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM34SEL"
  #endif

  #if STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSE
    #error "LSE not enabled, required by STM32_SYSTICKSEL"
  #endif

  #if STM32_DAC1SHSEL == STM32_DAC1SHSEL_LSE
    #error "LSE not enabled, required by STM32_DACSELSH"
  #endif

  #if STM32_RTCSEL == STM32_RTCSEL_LSE
    #error "LSE not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSE
    #error "LSE not enabled, required by STM32_LSCOSEL"
  #endif

#endif /* !STM32_LSE_ENABLED */

/*
 * HSE related checks.
 */
#if STM32_HSE_ENABLED
#else /* !STM32_HSE_ENABLED */

  #if STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_HSE
    #error "HSE not enabled, required by STM32_MSIRC0_MODE"
  #endif

  #if STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_HSE
    #error "HSE not enabled, required by STM32_MSIRC1_MODE"
  #endif

  #if STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_HSE_FAST
    #error "HSE not enabled, required by STM32_MSIRC0_MODE"
  #endif

  #if STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_HSE_FAST
    #error "HSE not enabled, required by STM32_MSIRC1_MODE"
  #endif

  #if STM32_SW == STM32_SW_HSE
    #error "HSE not enabled, required by STM32_SW"
  #endif

  #if STM32_MCO1SEL == STM32_MCO1SEL_HSE
    #error "HSE not enabled, required by STM32_MCO1SEL"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_HSE
    #error "HSE not enabled, required by STM32_MCO2SEL"
  #endif

  #if STM32_ADCDACSEL == STM32_ADCDACSEL_HSE
    #error "HSE not enabled, required by STM32_ADCDACSEL"
  #endif

  #if STM32_SAI1SEL == STM32_SAI1SEL_HSE
    #error "HSE not enabled, required by STM32_SAI1SEL"
  #endif

  #if STM32_RTCSEL == STM32_RTCSEL_HSEDIV
    #error "HSE not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_ICLKSEL == STM32_ICLKSEL_HSE
    #error "HSE not enabled, required by STM32_ICLKSEL"
  #endif

#endif /* !STM32_HSE_ENABLED */

/**
 * @brief   PLL input divider selection based on HSE frequency.
 */
#if (STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_HSE) ||                          \
    (STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_HSE) || defined(__DOXYGEN__)
  #if STM32_HSECLK == 32000000 || defined(__DOXYGEN__)
    #define STM32_MSIHSINDIV        STM32_MSIHSINDIV_HSE32
  #elif STM32_HSECLK == 16000000
    #define STM32_MSIHSINDIV        STM32_MSIHSINDIV_HSE16
  #else
    #error "MSIPLL feature requires a 16 or 32 MHz HSE clock"
  #endif

#else
  #define STM32_MSIHSINDIV          0U
#endif

/**
 * @brief   MSIRC0 mode selection.
 */
#if (STM32_MSIRC0_MODE == STM32_MSIRC0_FREE) || defined(__DOXYGEN__)
  #define STM32_MSIPLL0EN           0U
  #define STM32_MSIPLL0SEL          0U
  #define STM32_MSIPLL0FAST         0U

#elif STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_LSE
  #define STM32_MSIPLL0EN           RCC_CR_MSIPLL0EN
  #define STM32_MSIPLL0SEL          STM32_MSIPLL0SEL_LSE
  #define STM32_MSIPLL0FAST         0U

#elif STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_HSE
  #define STM32_MSIPLL0EN           RCC_CR_MSIPLL0EN
  #define STM32_MSIPLL0SEL          STM32_MSIPLL0SEL_HSE
  #define STM32_MSIPLL0FAST         0U

#elif STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_LSE_FAST
  #define STM32_MSIPLL0EN           RCC_CR_MSIPLL0EN
  #define STM32_MSIPLL0SEL          STM32_MSIPLL0SEL_LSE
  #define STM32_MSIPLL0FAST         RCC_CR_MSIPLL0FAST

#elif STM32_MSIRC0_MODE == STM32_MSIRC0_PLL_HSE_FAST
  #define STM32_MSIPLL0EN           RCC_CR_MSIPLL0EN
  #define STM32_MSIPLL0SEL          STM32_MSIPLL0SEL_HSE
  #define STM32_MSIPLL0FAST         RCC_CR_MSIPLL0FAST

#else
  #error "invalid STM32_MSIRC0_MODE value specified"
#endif

/**
 * @brief   MSIRC1 mode selection.
 */
#if (STM32_MSIRC1_MODE == STM32_MSIRC1_FREE) || defined(__DOXYGEN__)
  #define STM32_MSIPLL1EN           0U
  #define STM32_MSIPLL1SEL          0U
  #define STM32_MSIPLL1FAST         0U

#elif STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_LSE
  #define STM32_MSIPLL1EN           RCC_CR_MSIPLL1EN
  #define STM32_MSIPLL1SEL          STM32_MSIPLL1SEL_LSE
  #define STM32_MSIPLL1FAST         0U

#elif STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_HSE
  #define STM32_MSIPLL1EN           RCC_CR_MSIPLL1EN
  #define STM32_MSIPLL1SEL          STM32_MSIPLL1SEL_HSE
  #define STM32_MSIPLL1FAST         0U

#elif STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_LSE
  #define STM32_MSIPLL1EN           RCC_CR_MSIPLL1EN
  #define STM32_MSIPLL1SEL          STM32_MSIPLL1SEL_LSE
  #define STM32_MSIPLL1FAST         RCC_CR_MSIPLL1FAST

#elif STM32_MSIRC1_MODE == STM32_MSIRC1_PLL_HSE
  #define STM32_MSIPLL1EN           RCC_CR_MSIPLL1EN
  #define STM32_MSIPLL1SEL          STM32_MSIPLL1SEL_HSE
  #define STM32_MSIPLL1FAST         RCC_CR_MSIPLL1FAST

#else
  #error "invalid STM32_MSIRC1_MODE value specified"
#endif

#if (STM32_MSIPLL0EN == 0) || defined(__DOXYGEN__)
  /**
   * @brief   MSIPLL1N field initializer.
   */
  #define STM32_MSIPLL1N            0U

  /**
   * @brief   MSIRC0 base frequency.
   */
  #define STM32_MSIRC0CLK           96000000U

  /**
   * @brief   MSIRC1 base frequency.
   */
  #define STM32_MSIRC1CLK           24000000U

#else /* PLL mode enabled.*/
  #if STM32_MSIPLL1N_VALUE == 0
    #define STM32_MSIPLL1N          0U
    #if STM32_MSIPLL1SEL == 0
      #define STM32_MSIRC0CLK       96010000U
      #define STM32_MSIRC1CLK       23986000U
    #else
      #define STM32_MSIRC0CLK       96000000U
      #define STM32_MSIRC1CLK       24016000U
    #endif

  #elif STM32_MSIPLL1N_VALUE == 2
    #define STM32_MSIPLL1N          (RCC_ICSCR1_MSIPLL1N_1)
    #if STM32_MSIPLL1SEL == 0
      #define STM32_MSIRC0CLK       96010000U
      #define STM32_MSIRC1CLK       22577000U
    #else
      #define STM32_MSIRC0CLK       96000000U
      #define STM32_MSIRC1CLK       22581000U
    #endif

  #elif STM32_MSIPLL1N_VALUE == 3
    #define STM32_MSIPLL1N          (RCC_ICSCR1_MSIPLL1N_1 | RCC_ICSCR1_MSIPLL1N_0)
    #if STM32_MSIPLL1SEL == 0
      #define STM32_MSIRC0CLK       96010000U
      #define STM32_MSIRC1CLK       24576000U
    #else
      #define STM32_MSIRC0CLK       96000000U
      #define STM32_MSIRC1CLK       24577000U
    #endif

  #else
    #error "invalid STM32_MSIPLL1N_VALUE value specified"
  #endif
#endif

/**
 * @brief   MSIS clock frequency.
 */
#if (STM32_MSIS_SRCDIV == STM32_MSIS_IRC0_DIV1) || defined(__DOXYGEN__)
  #define STM32_MSISIRC0_ENABLED    TRUE
  #define STM32_MSISIRC1_ENABLED    FALSE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(0U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC0
  #define STM32_MSISCLK             STM32_MSIRC0CLK

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC0_DIV2
  #define STM32_MSISIRC0_ENABLED    TRUE
  #define STM32_MSISIRC1_ENABLED    FALSE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(1U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC0
  #define STM32_MSISCLK             (STM32_MSIRC0CLK / 2U)

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC0_DIV4
  #define STM32_MSISIRC0_ENABLED    TRUE
  #define STM32_MSISIRC1_ENABLED    FALSE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(2U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC0
  #define STM32_MSISCLK             (STM32_MSIRC0CLK / 4U)

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC0_DIV8
  #define STM32_MSISIRC0_ENABLED    TRUE
  #define STM32_MSISIRC1_ENABLED    FALSE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(3U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC0
  #define STM32_MSISCLK             (STM32_MSIRC0CLK / 8U)

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC1_DIV1
  #define STM32_MSISIRC0_ENABLED    FALSE
  #define STM32_MSISIRC1_ENABLED    TRUE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(0U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC1
  #define STM32_MSISCLK             STM32_MSIRC1CLK

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC1_DIV2
  #define STM32_MSISIRC0_ENABLED    FALSE
  #define STM32_MSISIRC1_ENABLED    TRUE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(1U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC1
  #define STM32_MSISCLK             (STM32_MSIRC1CLK / 2U)

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC1_DIV4
  #define STM32_MSISIRC0_ENABLED    FALSE
  #define STM32_MSISIRC1_ENABLED    TRUE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(2U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC1
  #define STM32_MSISCLK             (STM32_MSIRC1CLK / 4U)

#elif STM32_MSIS_SRCDIV == STM32_MSIS_IRC1_DIV8
  #define STM32_MSISIRC0_ENABLED    FALSE
  #define STM32_MSISIRC1_ENABLED    TRUE
  #define STM32_MSISDIV             STM32_MSISDIV_FIELD(3U)
  #define STM32_MSISSEL             STM32_MSISSEL_MSIRC1
  #define STM32_MSISCLK             (STM32_MSIRC1CLK / 8U)

#else
  #error "invalid STM32_MSIS_SRCDIV value specified"
#endif

/**
 * @brief   MSIK clock frequency.
 */
#if (STM32_MSIK_SRCDIV == STM32_MSIK_IRC0_DIV1) || defined(__DOXYGEN__)
  #define STM32_MSIKIRC0_ENABLED    TRUE
  #define STM32_MSIKIRC1_ENABLED    FALSE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(0U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC0
  #define STM32_MSIKCLK             STM32_MSIRC0CLK

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC0_DIV2
  #define STM32_MSIKIRC0_ENABLED    TRUE
  #define STM32_MSIKIRC1_ENABLED    FALSE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(1U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC0
  #define STM32_MSIKCLK             (STM32_MSIRC0CLK / 2U)

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC0_DIV4
  #define STM32_MSIKIRC0_ENABLED    TRUE
  #define STM32_MSIKIRC1_ENABLED    FALSE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(2U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC0
  #define STM32_MSIKCLK             (STM32_MSIRC0CLK / 4U)

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC0_DIV8
  #define STM32_MSIKIRC0_ENABLED    TRUE
  #define STM32_MSIKIRC1_ENABLED    FALSE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(3U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC0
  #define STM32_MSIKCLK             (STM32_MSIRC0CLK / 8U)

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC1_DIV1
  #define STM32_MSIKIRC0_ENABLED    FALSE
  #define STM32_MSIKIRC1_ENABLED    TRUE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(0U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC1
  #define STM32_MSIKCLK             STM32_MSIRC1CLK

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC1_DIV2
  #define STM32_MSIKIRC0_ENABLED    FALSE
  #define STM32_MSIKIRC1_ENABLED    TRUE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(1U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC1
  #define STM32_MSIKCLK             (STM32_MSIRC1CLK / 2U)

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC1_DIV4
  #define STM32_MSIKIRC0_ENABLED    FALSE
  #define STM32_MSIKIRC1_ENABLED    TRUE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(2U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC1
  #define STM32_MSIKCLK             (STM32_MSIRC1CLK / 4U)

#elif STM32_MSIK_SRCDIV == STM32_MSIK_IRC1_DIV8
  #define STM32_MSIKIRC0_ENABLED    FALSE
  #define STM32_MSIKIRC1_ENABLED    TRUE
  #define STM32_MSIKDIV             STM32_MSIKDIV_FIELD(3U)
  #define STM32_MSIKSEL             STM32_MSIKSEL_MSIRC1
  #define STM32_MSIKCLK             (STM32_MSIRC1CLK / 8U)

#else
  #error "invalid STM32_MSIK_SRCDIV value specified"
#endif

/* PLL enabling checks, PLL cannot be enabled if the associated oscillator
   is not active.*/
#if (STM32_MSIPLL0EN > 0) && !STM32_MSISIRC0_ENABLED && !STM32_MSIKIRC0_ENABLED
#error "STM32_MSIRC0_MODE requires PLL but MSIIRC0 is not used"
#endif
#if (STM32_MSIPLL1EN > 0) && !STM32_MSISIRC1_ENABLED && !STM32_MSIKIRC1_ENABLED
#error "STM32_MSIRC1_MODE requires PLL but MSIIRC1 is not used"
#endif

/**
 * @brief   System clock source.
 */
#if (STM32_SW == STM32_SW_MSIS) || defined(__DOXYGEN__)
  #define STM32_SYSCLK              STM32_MSISCLK

#elif STM32_SW == STM32_SW_HSI16
  #define STM32_SYSCLK              STM32_HSI16CLK

#elif STM32_SW == STM32_SW_HSE
  #define STM32_SYSCLK              STM32_HSECLK

#else
  #error "invalid STM32_SW value specified"
#endif

/* EPOD boost-related settings.*/
#if STM32_SYSCLK > STM32_BOOSTEN_THRESHOLD
  #if STM32_SW == STM32_SW_MSIS
    #define STM32_BOOSTER_ENABLED   TRUE
    #define STM32_BOOSTSEL          STM32_BOOSTSEL_MSIS
    #define STM32_BOOSTDIV          0U

  #elif STM32_SW == STM32_SW_HSI16
    #define STM32_BOOSTER_ENABLED   TRUE
    #define STM32_BOOSTSEL          STM32_BOOSTSEL_HSI16
    #define STM32_BOOSTDIV          STM32_BOOSTDIV_DIV2

  #else /* STM32_SW == STM32_SW_HSE */
    #define STM32_BOOSTER_ENABLED   TRUE
    #define STM32_BOOSTSEL          STM32_BOOSTSEL_HSE
    /* The divider is a function of the HSE frequency.*/
    #if STM32_HSECLK <= 12000000U
      #define STM32_BOOSTDIV        STM32_BOOSTDIV_DIV1
    #elif STM32_HSECLK <= 24000000U
      #define STM32_BOOSTDIV        STM32_BOOSTDIV_DIV2
    #else
      #define STM32_BOOSTDIV        STM32_BOOSTDIV_DIV4
    #endif
  #endif
#else /* EPOD boost not required.*/
  #define STM32_BOOSTER_ENABLED     FALSE
  #define STM32_BOOSTSEL            0U
  #define STM32_BOOSTDIV            0U
#endif

/* Bus handlers.*/
#include "stm32_ahb.inc"
#include "stm32_apb1.inc"
#include "stm32_apb2.inc"
#include "stm32_apb3.inc"

/* STOPWUCK setting check.*/
#if (STM32_STOPWUCK == STM32_STOPWUCK_MSIS) || defined(__DOXYGEN__)

#elif STM32_STOPWUCK == STM32_STOPWUCK_HSI16

#else
  #error "invalid STM32_STOPWUCK value specified"
#endif

/* STOPKERWUCK setting check.*/
#if (STM32_STOPKERWUCK == STM32_STOPKERWUCK_MSIK) || defined(__DOXYGEN__)

#elif STM32_STOPKERWUCK == STM32_STOPKERWUCK_HSI16

#else
  #error "invalid STM32_STOPKERWUCK value specified"
#endif

/* STM32_MSIBIASL setting check.*/
#if (STM32_MSIBIAS == STM32_MSIBIAS_CONTINUOUS) || defined(__DOXYGEN__)

#elif STM32_MSIBIAS == STM32_MSIBIAS_SAMPLING

#else
  #error "invalid STM32_MSIBIAS value specified"
#endif

/**
 * @brief   MCO1 source clock.
 */
#if (STM32_MCO1SEL == STM32_MCO1SEL_OFF) || defined(__DOXYGEN__)
  #define STM32_MCO1DIVCLK          0U

#elif STM32_MCO1SEL == STM32_MCO1SEL_SYSCLK
  #define STM32_MCO1DIVCLK          STM32_SYSCLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_MSIS
  #define STM32_MCO1DIVCLK          STM32_MSISCLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSI16
  #define STM32_MCO1DIVCLK          STM32_HSI16CLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSE
  #define STM32_MCO1DIVCLK          STM32_HSE

#elif STM32_MCO1SEL == STM32_MCO1SEL_LSI
  #define STM32_MCO1DIVCLK          STM32_LSICLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_LSE
  #define STM32_MCO1DIVCLK          STM32_LSECLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSI48
  #define STM32_MCO1DIVCLK          STM32_HSI48CLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_MSIK
  #define STM32_MCO1DIVCLK          STM32_MSIKCLK

#else
  #error "invalid STM32_MCO1SEL value specified"
#endif

/**
 * @brief   MCO1PRE field.
 */
#if (STM32_MCO1PRE_VALUE == 1) || defined(__DOXYGEN__)
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV1
#elif STM32_MCO1PRE_VALUE == 2
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV2
#elif STM32_MCO1PRE_VALUE == 4
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV4
#elif STM32_MCO1PRE_VALUE == 8
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV8
#elif STM32_MCO1PRE_VALUE == 16
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV16
#elif STM32_MCO1PRE_VALUE == 32
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV32
#elif STM32_MCO1PRE_VALUE == 64
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV64
#elif STM32_MCO1PRE_VALUE == 128
  #define STM32_MCO1PRE             STM32_MCO1PRE_DIV128

#else
#error "invalid STM32_MCO1PRE_VALUE value specified"
#endif

/**
 * @brief   MCO2 source clock.
 */
#if (STM32_MCO2SEL == STM32_MCO2SEL_OFF) || defined(__DOXYGEN__)
  #define STM32_MCO2DIVCLK          0U

#elif STM32_MCO2SEL == STM32_MCO2SEL_SYSCLK
  #define STM32_MCO2DIVCLK          STM32_SYSCLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_MSIS
  #define STM32_MCO2DIVCLK          STM32_MSISCLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_HSI16
  #define STM32_MCO2DIVCLK          STM32_HSI16CLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_HSE
  #define STM32_MCO2DIVCLK          STM32_HSE

#elif STM32_MCO2SEL == STM32_MCO2SEL_LSI
  #define STM32_MCO2DIVCLK          STM32_LSICLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_LSE
  #define STM32_MCO2DIVCLK          STM32_LSECLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_HSI48
  #define STM32_MCO2DIVCLK          STM32_HSI48CLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_MSIK
  #define STM32_MCO2DIVCLK          STM32_MSIKCLK

#else
  #error "invalid STM32_MCO2SEL value specified"
#endif

/**
 * @brief   MCO2PRE field.
 */
#if (STM32_MCO2PRE_VALUE == 1) || defined(__DOXYGEN__)
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV1
#elif STM32_MCO2PRE_VALUE == 2
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV2
#elif STM32_MCO2PRE_VALUE == 4
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV4
#elif STM32_MCO2PRE_VALUE == 8
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV8
#elif STM32_MCO2PRE_VALUE == 16
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV16
#elif STM32_MCO2PRE_VALUE == 32
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV32
#elif STM32_MCO2PRE_VALUE == 64
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV64
#elif STM32_MCO2PRE_VALUE == 128
  #define STM32_MCO2PRE             STM32_MCO2PRE_DIV128

#else
#error "invalid STM32_MCO2PRE_VALUE value specified"
#endif

/**
 * @brief   MCO1 output pin clock frequency.
 */
#define STM32_MCO1CLK               (STM32_MCO1DIVCLK / STM32_MCO1PRE_VALUE)

/**
 * @brief   MCO2 output pin clock frequency.
 */
#define STM32_MCO2CLK               (STM32_MCO2DIVCLK / STM32_MCO2PRE_VALUE)

/**
 * @brief   RTC clock frequency.
 */
#if (STM32_RTCSEL == STM32_RTCSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_RTCCLK              0U

#elif STM32_RTCSEL == STM32_RTCSEL_LSE
  #define STM32_RTCCLK              STM32_LSECLK

#elif STM32_RTCSEL == STM32_RTCSEL_LSI
  #define STM32_RTCCLK              STM32_LSICLK

#elif STM32_RTCSEL == STM32_RTCSEL_HSEDIV
  #define STM32_RTCCLK              (STM32_HSECLK / 32U)

#else
  #error "invalid STM32_RTCSEL value specified"
#endif

/**
 * @brief   LSCO clock frequency.
 */
#if (STM32_LSCOSEL == STM32_LSCOSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_LSCOCLK             0U

#elif STM32_LSCOSEL == STM32_LSCOSEL_LSI
  #define STM32_LSCOCLK             STM32_LSICLK

#elif STM32_LSCOSEL == STM32_LSCOSEL_LSE
  #define STM32_LSCOCLK             STM32_LSECLK

#else
  #error "invalid STM32_LSCOSEL value specified"
#endif

/**
 * @brief   USART1 clock frequency.
 */
#if (STM32_USART1SEL == STM32_USART1SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_PCLK2)

#elif STM32_USART1SEL == STM32_USART1SEL_MSIK
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for USART1 clock"
#endif

/**
 * @brief   USART3 clock frequency.
 */
#if (STM32_USART3SEL == STM32_USART3SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_USART3SEL == STM32_USART3SEL_MSIK
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for USART3 clock"
#endif

/**
 * @brief   UART4 clock frequency.
 */
#if (STM32_UART4SEL == STM32_UART4SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART4CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART4SEL == STM32_UART4SEL_MSIK
  #define STM32_UART4CLK            hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for UART4 clock"
#endif

/**
 * @brief   UART5 clock frequency.
 */
#if (STM32_UART5SEL == STM32_UART5SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART5CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART5SEL == STM32_UART5SEL_MSIK
  #define STM32_UART5CLK            hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for UART5 clock"
#endif

/**
 * @brief   LPUART1 clock frequency.
 */
#if (STM32_LPUART1SEL == STM32_LPUART1SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_MSIK
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_MSIK)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_HSI16
  #define STM32_LPUART1CLK          STM32_HSI16CLK

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_LSE
  #define STM32_LPUART1CLK          STM32_LSECLK

#else
  #error "invalid source selected for LPUART1 clock"
#endif

#if (STM32_TIMICSEL == STM32_TIMICSEL_NOCLOCK) || defined(__DOXYGEN__)
  /**
   * @brief   TIM16 internal capture clock frequency.
   */
  #define STM32_TIM16CCLK           0U
  /**
   * @brief   TIM17 internal capture clock frequency.
   */
  #define STM32_TIM17CCLK           0U
  /**
   * @brief   LPTIM2 internal capture clock frequency.
   */
  #define STM32_LPTIM2CCLK          0U

#elif STM32_TIMICSEL == STM32_TIMICSEL_HSI256_MSIS1024_MSIS4
  #define STM32_TIM16CCLK           (STM32_HSI16CLK / 256U)
  #define STM32_TIM15CCLK           (STM32_MSISCLK / 1024U)
  #define STM32_LPTIM2CCLK          (STM32_MSISCLK / 4U)

#elif STM32_TIMICSEL == STM32_TIMICSEL_HSI256_MSIS1024_MSIK4
  #define STM32_TIM16CCLK           (STM32_HSI16CLK / 256U)
  #define STM32_TIM15CCLK           (STM32_MSISCLK / 1024U)
  #define STM32_LPTIM2CCLK          (STM32_MSIKCLK / 4U)

#elif STM32_TIMICSEL == STM32_TIMICSEL_HSI256_MSIK1024_MSIS4
  #define STM32_TIM16CCLK           (STM32_HSI16CLK / 256U)
  #define STM32_TIM15CCLK           (STM32_MSIKCLK / 1024U)
  #define STM32_LPTIM2CCLK          (STM32_MSISCLK / 4U)

#elif STM32_TIMICSEL == STM32_TIMICSEL_HSI256_MSIK1024_MSIK4
  #define STM32_TIM16CCLK           (STM32_HSI16CLK / 256U)
  #define STM32_TIM15CCLK           (STM32_MSIKCLK / 1024U)
  #define STM32_LPTIM2CCLK          (STM32_MSIKCLK / 4U)

#else
  #error "invalid source selected for TIMICSEL clock"
#endif

/**
 * @brief   LPTIM1 clock frequency.
 */
#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_MSIK) || defined(__DOXYGEN__)
  #define STM32_LPTIM1CLK           hal_lld_get_clock_point(CLK_MSIK)

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI
  #define STM32_LPUART1CLK          STM32_LSICLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_PLL3R
  #define STM32_LPUART1CLK          STM32_HSI16CLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE
  #define STM32_LPTIM1CLK           STM32_LSECLK

#else
  #error "invalid source selected for LPTIM1 clock"
#endif

/**
 * @brief   LPTIM2 clock frequency.
 */
#if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_LPTIM2CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSI
  #define STM32_LPUART1CLK          STM32_LSICLK

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_PLL3R
  #define STM32_LPUART1CLK          STM32_HSI16CLK

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSE
  #define STM32_LPTIM2CLK           STM32_LSECLK

#else
  #error "invalid source selected for LPTIM2 clock"
#endif

/**
 * @brief   LPTIM3/4 clock frequency.
 */
#if (STM32_LPTIM34SEL == STM32_LPTIM34SEL_MSIK) || defined(__DOXYGEN__)
  #define STM32_LPTIM3CLK           hal_lld_get_clock_point(CLK_MSIK)
  #define STM32_LPTIM4CLK           hal_lld_get_clock_point(CLK_MSIK)

#elif STM32_LPTIM34SEL == STM32_LPTIM34SEL_LSI
  #define STM32_LPUART1CLK          STM32_LSICLK

#elif STM32_LPTIM34SEL == STM32_LPTIM34SEL_PLL3R
  #define STM32_LPUART1CLK          STM32_HSI16CLK

#elif STM32_LPTIM34SEL == STM32_LPTIM34SEL_LSE
  #define STM32_LPTIM2CLK           STM32_LSECLK

#else
  #error "invalid source selected for LPTIM2 clock"
#endif

/**
 * @brief   SPI1 clock frequency.
 */
#if (STM32_SPI1SEL == STM32_SPI1SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_SPI1CLK             hal_lld_get_clock_point(CLK_PCLK2)

#elif STM32_SPI1SEL == STM32_SPI1SEL_MSIK
  #define STM32_SPI1CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for SPI1 clock"
#endif

/**
 * @brief   SPI2 clock frequency.
 */
#if (STM32_SPI2SEL == STM32_SPI2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_SPI2CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_SPI2SEL == STM32_SPI2SEL_MSIK
  #define STM32_SPI2CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for SPI2 clock"
#endif

/**
 * @brief   SPI3 clock frequency.
 */
#if (STM32_SPI3SEL == STM32_SPI3SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_SPI3CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_SPI3SEL == STM32_SPI3SEL_MSIK
  #define STM32_SPI3CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for SPI3 clock"
#endif

/**
 * @brief   OCTOSPI clock frequency.
 */
#if (STM32_OCTOSPISEL == STM32_OCTOSPISEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_OSPICLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_OCTOSPISEL == STM32_OCTOSPISEL_MSIK
  #define STM32_OSPICLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for OCTOSPI clock"
#endif

/**
 * @brief   SYSTICK clock frequency.
 */
#if (STM32_SYSTICKSEL == STM32_SYSTICKSEL_HCLKDIV8) || defined(__DOXYGEN__)
  #define STM32_SYSTICKCLK          (hal_lld_get_clock_point(CLK_HCLK) / 8U)

#elif STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSI
  #define STM32_SYSTICKCLK          STM32_LSICLK

#elif STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSE
  #define STM32_SYSTICKCLK          STM32_LSECLK

#else
  #error "invalid source selected for STM32_SYSTICKSEL clock"
#endif

/**
 * @brief   ICLK clock frequency.
 */
#if (STM32_ICLKSEL == STM32_ICLKSEL_HSI48) || defined(__DOXYGEN__)
  #define STM32_ICLK                STM32_HSI48CLK

#elif STM32_ICLKSEL == STM32_ICLKSEL_MSIK
  #define STM32_ICLK                hal_lld_get_clock_point(CLK_MSIK)

#elif STM32_ICLKSEL == STM32_ICLKSEL_HSE
  #define STM32_ICLK                STM32_HSECLK

#elif STM32_ICLKSEL == STM32_ICLKSEL_SYSCLK
  #define STM32_ICLK                hal_lld_get_clock_point(CLK_SYSCLK)

#else
  #error "invalid source selected for ICLK clock"
#endif

/**
 * @brief   USB clock frequency.
 */
#if (STM32_USB1SEL == STM32_USB1SEL_ICLK) || defined(__DOXYGEN__)
  #define STM32_USBCLK              STM32_ICLK

#elif STM32_USB1SEL == STM32_USB1SEL_ICLKDIV2
  #define STM32_USBCLK              (STM32_ICLK / 2U)

#else
  #error "invalid source selected for USB clock"
#endif

/**
 * @brief   SDMMC1 clock frequency.
 */
#define STM32_SDMMC1CLK             STM32_ICLK

/**
 * @brief   I2C1 clock frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C1SEL == STM32_I2C1SEL_MSIK
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C2 clock frequency.
 */
#if (STM32_I2C2SEL == STM32_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C2CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C2SEL == STM32_I2C2SEL_MSIK
  #define STM32_I2C2CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for I2C2 clock"
#endif

/**
 * @brief   I2C3 clock frequency.
 */
#if (STM32_I2C3SEL == STM32_I2C3SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_I2C3CLK             hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_I2C3SEL == STM32_I2C3SEL_MSIK
  #define STM32_I2C3CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for I2C3 clock"
#endif

/**
 * @brief   I3C1 clock frequency.
 */
#if (STM32_I3C1SEL == STM32_I3C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I3C1CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I3C1SEL == STM32_I3C1SEL_MSIK
  #define STM32_I3C1CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for I3C1 clock"
#endif

/**
 * @brief   I3C2 clock frequency.
 */
#if (STM32_I3C2SEL == STM32_I3C2SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_I3C2CLK             hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_I3C2SEL == STM32_I3C2SEL_MSIK
  #define STM32_I3C2CLK             hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for I3C2 clock"
#endif

/**
 * @brief   ADCDACSEL clock frequency.
 */
#if (STM32_ADCDACSEL == STM32_ADCDACSEL_HCLK) || defined(__DOXYGEN__)
  #define STM32_ADCDACCLK           hal_lld_get_clock_point(CLK_HCLK)

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_HSE
  #define STM32_ADCDACCLK           STM32_HSECLK

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_MSIK
  #define STM32_ADCDACCLK           hal_lld_get_clock_point(CLK_HCLK)

#else
  #error "invalid source selected for ADCDACSEL clock"
#endif

/**
 * @brief   DAC1SELSH clock frequency.
 */
#if (STM32_DAC1SHSEL == STM32_DAC1SHSEL_LSI) || defined(__DOXYGEN__)
  #define STM32_DAC1SHSELCLK        STM32_LSICLK

#elif STM32_DAC1SHSEL == STM32_DAC1SHSEL_LSE
  #define STM32_DAC1SHSELCLK        STM32_LSECLK

#elif STM32_DAC1SHSEL == STM32_DAC1SHSEL_IGNORE
  #define STM32_DAC1SHSELCLK        0U

#else
  #error "invalid source selected for DAC1SELSH clock"
#endif

/**
 * @brief   RNG clock frequency.
 */
#if (STM32_RNGSEL == STM32_RNGSEL_HSI48) || defined(__DOXYGEN__)
  #define STM32_RNGCLK              STM32_HSI48CLK

#elif STM32_RNGSEL == STM32_RNGSEL_MSIK
  #define STM32_RNGCLK              hal_lld_get_clock_point(CLK_MSIK)

#elif STM32_RNGSEL == STM32_RNGSEL_IGNORE
  #define STM32_RNGCLK              0U

#else
  #error "invalid source selected for RNG clock"
#endif

/**
 * @brief   FDCAN1 clock frequency.
 */
#if (STM32_FDCAN1SEL == STM32_FDCAN1SEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_FDCAN1CLK           STM32_SYSCLK

#elif STM32_FDCAN1SEL == STM32_FDCANSEL_MSIK
  #define STM32_FDCAN1CLK           hal_lld_get_clock_point(CLK_MSIK)

#else
  #error "invalid source selected for FDCAN1 clock"
#endif

/**
 * @brief   SAI1 clock frequency.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_MSIK) || defined(__DOXYGEN__)
  #define STM32_SAI1CLK             hal_lld_get_clock_point(CLKMSIK)

#elif STM32_SAI1SEL == STM32_SAI1SEL_AUDIOCLK
  #define STM32_SAI1CLK             0 /* TODO board.h */

#elif STM32_SAI1SEL == STM32_SAI1SEL_HSE
  #define STM32_SAI1CLK             STM32_HSECLK

#else
  #error "invalid source selected for SAI1 clock"
#endif

/**
 * @brief   TIMP1CLK clock frequency.
 */
#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
  #define STM32_TIMP1CLK                STM32_PCLK1
#else
  #define STM32_TIMP1CLK                (STM32_PCLK1 * 2)
#endif

/**
 * @brief   TIMP2CLK clock frequency.
 */
#if (STM32_PPRE2 == STM32_PPRE2_DIV1) || defined(__DOXYGEN__)
  #define STM32_TIMP2CLK            STM32_PCLK2
#else
  #define STM32_TIMP2CLK            (STM32_PCLK2 * 2)
#endif

/**
 * @brief   Clock of timers connected to APB1.
 */
#define STM32_TIMCLK1               hal_lld_get_clock_point(CLK_PCLK1TIM)

/**
 * @brief   Clock of timers connected to APB2.
 */
#define STM32_TIMCLK2               hal_lld_get_clock_point(CLK_PCLK2TIM)

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
  #define STM32_FLASHBITS           STM32_LATENCY_0WS

#elif STM32_HCLK <= STM32_1WS_THRESHOLD
  #define STM32_FLASHBITS           STM32_LATENCY_1WS

#elif STM32_HCLK <= STM32_2WS_THRESHOLD
  #define STM32_FLASHBITS           STM32_LATENCY_2WS

#elif STM32_HCLK <= STM32_3WS_THRESHOLD
  #define STM32_FLASHBITS           STM32_LATENCY_3WS

#elif STM32_HCLK <= STM32_4WS_THRESHOLD
  #define STM32_FLASHBITS           STM32_LATENCY_4WS

#else
  #define STM32_FLASHBITS           STM32_LATENCY_5WS
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Type of PLL configuration registers.
 */
typedef struct {
  uint32_t          cfgr;
  uint32_t          divr;
  uint32_t          frac;
} stm32_pll_regs_t;

/**
 * @brief   Type of a clock configuration structure.
 */
typedef struct {
  uint32_t          pwr_voscr;
  uint32_t          pwr_vmcr;
  uint32_t          rcc_cr;
  uint32_t          rcc_cfgr1;
  uint32_t          rcc_cfgr2;
  uint32_t          flash_acr;
#if STM32_RCC_HAS_PLL3 || defined(__DOXYGEN__)
  stm32_pll_regs_t  plls[3];
#else
  stm32_pll_regs_t  plls[2];
#endif
} halclkcfg_t;
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
  ((clkpt) == CLK_HSI16     ? STM32_HSI16CLK        :                       \
   (clkpt) == CLK_HSI48     ? STM32_HSI48CLK        :                       \
   (clkpt) == CLK_HSE       ? STM32_HSECLK          :                       \
   (clkpt) == CLK_MSIS      ? STM32_MSISCLK         :                       \
   (clkpt) == CLK_MSIK      ? STM32_MSIKCLK         :                       \
   (clkpt) == CLK_SYSCLK    ? STM32_SYSCLK          :                       \
   (clkpt) == CLK_HCLK      ? STM32_HCLK            :                       \
   (clkpt) == CLK_PCLK1     ? STM32_PCLK1           :                       \
   (clkpt) == CLK_PCLK1TIM  ? STM32_TIMP1CLK        :                       \
   (clkpt) == CLK_PCLK2     ? STM32_PCLK2           :                       \
   (clkpt) == CLK_PCLK2TIM  ? STM32_TIMP2CLK        :                       \
   (clkpt) == CLK_PCLK3     ? STM32_PCLK3           :                       \
   (clkpt) == CLK_MCO1      ? STM32_MCO1CLK         :                       \
   (clkpt) == CLK_MCO2      ? STM32_MCO2CLK         :                       \
   0U)
#endif /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Various helpers.*/
#include "nvic.h"
#include "cache.h"
//#include "mpu_v8m.h"
#include "stm32_isr.h"
#include "stm32_dma3.h"
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
