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
#if !defined(STM32U5XX) || defined(__DOXYGEN__)
  #define STM32U5XX
#endif
/** @} */

/**
 * @name   Clock points names
 * @{
 */
#define CLK_HSI                 0U
#define CLK_CSI                 1U
#define CLK_HSI48               2U
#define CLK_HSE                 3U
#define CLK_MSIS                4U
#define CLK_MSIK                5U
#define CLK_SYSCLK              6U
#define CLK_HCLK                7U
#define CLK_PCLK1               8U
#define CLK_PCLK1TIM            9U
#define CLK_PCLK2               10U
#define CLK_PCLK2TIM            11U
#define CLK_PCLK3               12U
#define CLK_MCO1                13U
#define CLK_MCO2                14U
#define CLK_ARRAY_SIZE          15U
/** @} */

/**
 * @name    PWR_CR3 register definitions
 * @{
 */
#define STM32_REGSEL            (1U << PWR_CR3_REGSEL_Pos)

#define STM32_FSTEN             (1U << PWR_CR3_FSTEN_Pos)
/** @} */

/**
 * @name    PWR_VOSR register definitions
 * @{
 */
#define STM32_VOS_POS           0
#define STM32_VOS_MASK          (3U << 0)
#define STM32_VOS_RANGE1        (1U << PWR_VOSR_R1EN_Pos)
#define STM32_VOS_RANGE2        (1U << PWR_VOSR_R2EN_Pos)

#define STM32_BOOSTEN           (1U << PWR_VOSR_BOOSTEN_Pos)

#define STM32_R1RDY             (1U << PWR_VOSR_R1RDY_Pos)

#define STM32_R2RDY             (1U << PWR_VOSR_R2RDY_Pos)

#define STM32_BOOSTRDY          (1U << PWR_VOSR_BOOSTRDY_Pos)
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
#define STM32_STOPWUCK_HSI      STM32_STOPWUCK_FIELD(0U)
#define STM32_STOPWUCK_CSI      STM32_STOPWUCK_FIELD(1U)

#define STM32_STOPKERWUCK_POS   RCC_CFGR1_STOPKERWUCK_Pos
#define STM32_STOPKERWUCK_MASK  RCC_CFGR1_STOPKERWUCK_Msk
#define STM32_STOPKERWUCK_FIELD(n) ((n) << STM32_STOPKERWUCK_POS)
#define STM32_STOPKERWUCK_HSI   STM32_STOPKERWUCK_FIELD(0U)
#define STM32_STOPKERWUCK_CSI   STM32_STOPKERWUCK_FIELD(1U)

#define STM32_MCO1SEL_POS       RCC_CFGR1_MCO1SEL_Pos
#define STM32_MCO1SEL_MASK      RCC_CFGR1_MCO1SEL_Msk
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

#define STM32_MCO1PRE_POS       RCC_CFGR1_MCO1PRE_Pos
#define STM32_MCO1PRE_MASK      RCC_CFGR1_MCO1PRE_Msk
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
#define STM32_MCO2SEL_OFF       STM32_MCO2PRE_FIELD(0U)
#define STM32_MCO2SEL_SYSCLK    STM32_MCO2PRE_FIELD(1U)
#define STM32_MCO2SEL_MSIS      STM32_MCO2PRE_FIELD(2U)
#define STM32_MCO2SEL_HSI16     STM32_MCO2PRE_FIELD(3U)
#define STM32_MCO2SEL_HSE       STM32_MCO2PRE_FIELD(4U)
#define STM32_MCO2SEL_LSI       STM32_MCO2PRE_FIELD(5U)
#define STM32_MCO2SEL_LSE       STM32_MCO2PRE_FIELD(6U)
#define STM32_MCO2SEL_HSI48     STM32_MCO2PRE_FIELD(7U)
#define STM32_MCO2SEL_MSIK      STM32_MCO2PRE_FIELD(8U)

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
 * @name    RCC_CFGR2 register bits definitions
 * @{
 */
#define STM32_HPRE_POS          RCC_CFGR2_HPRE_Pos
#define STM32_HPRE_MASK         RCC_CFGR2_HPRE_Msk
#define STM32_HPRE_FIELD(n)     ((n) << STM32_HPRE_POS)
#define STM32_HPRE_DIV1         STM32_HPRE_FIELD(0U)
#define STM32_HPRE_DIV2         STM32_HPRE_FIELD(8U)
#define STM32_HPRE_DIV4         STM32_HPRE_FIELD(9U)
#define STM32_HPRE_DIV8         STM32_HPRE_FIELD(10U)
#define STM32_HPRE_DIV16        STM32_HPRE_FIELD(11U)
#define STM32_HPRE_DIV64        STM32_HPRE_FIELD(12U)
#define STM32_HPRE_DIV128       STM32_HPRE_FIELD(13U)
#define STM32_HPRE_DIV256       STM32_HPRE_FIELD(14U)
#define STM32_HPRE_DIV512       STM32_HPRE_FIELD(15U)

#define STM32_PPRE1_POS         RCC_CFGR2_PPRE1_Pos
#define STM32_PPRE1_MASK        RCC_CFGR2_PPRE1_Msk
#define STM32_PPRE1_FIELD(n)    ((n) << STM32_PPRE1_POS)
#define STM32_PPRE1_DIV1        STM32_PPRE1_FIELD(0U)
#define STM32_PPRE1_DIV2        STM32_PPRE1_FIELD(4U)
#define STM32_PPRE1_DIV4        STM32_PPRE1_FIELD(5U)
#define STM32_PPRE1_DIV8        STM32_PPRE1_FIELD(6U)
#define STM32_PPRE1_DIV16       STM32_PPRE1_FIELD(7U)

#define STM32_PPRE2_POS         RCC_CFGR2_PPRE2_Pos
#define STM32_PPRE2_MASK        RCC_CFGR2_PPRE2_Msk
#define STM32_PPRE2_FIELD(n)    ((n) << STM32_PPRE2_POS)
#define STM32_PPRE2_DIV1        STM32_PPRE2_FIELD(0U)
#define STM32_PPRE2_DIV2        STM32_PPRE2_FIELD(4U)
#define STM32_PPRE2_DIV4        STM32_PPRE2_FIELD(5U)
#define STM32_PPRE2_DIV8        STM32_PPRE2_FIELD(6U)
#define STM32_PPRE2_DIV16       STM32_PPRE2_FIELD(7U)
/** @} */

/**
 * @name    RCC_CFGR3 register bits definitions
 * @{
 */
#define STM32_PPRE3_POS         RCC_CFGR3_PPRE3_Pos
#define STM32_PPRE3_MASK        RCC_CFGR3_PPRE3_Msk
#define STM32_PPRE3_FIELD(n)    ((n) << STM32_PPRE3_POS)
#define STM32_PPRE3_DIV1        STM32_PPRE3_FIELD(0U)
#define STM32_PPRE3_DIV2        STM32_PPRE3_FIELD(4U)
#define STM32_PPRE3_DIV4        STM32_PPRE3_FIELD(5U)
#define STM32_PPRE3_DIV8        STM32_PPRE3_FIELD(6U)
#define STM32_PPRE3_DIV16       STM32_PPRE3_FIELD(7U)
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
#define STM32_USART3SEL_PCLK1   STM32_USART1SEL_FIELD(0U)
#define STM32_USART3SEL_HSI16   STM32_USART1SEL_FIELD(1U)

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
#define STM32_I3C2SEL_PCLK1     STM32_I3C2SEL_FIELD(0U)
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

#define STM32_AFD1SEL_POS       0
#define STM32_AFD1SEL_MASK      (3U << STM32_AFD1SEL_POS)
#define STM32_AFD1SEL_FIELD(n)  ((n) << STM32_AFD1SEL_POS)
#define STM32_AFD1SEL_HCLK      STM32_AFD1SEL_FIELD(0U)
#define STM32_AFD1SEL_AUDIOCLK  STM32_AFD1SEL_FIELD(1U)
#define STM32_AFD1SEL_HSE       STM32_AFD1SEL_FIELD(2U)

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
 * @brief   PWR CR3 register initialization value.
 */
#if !defined(STM32_PWR_CR3) || defined(__DOXYGEN__)
#define STM32_PWR_CR3
#endif

/**
 * @brief   PWR VOSR register initialization value.
 */
#if !defined(STM32_PWR_VOSR) || defined(__DOXYGEN__)
#define STM32_PWR_VOSR                      STM32_VOS_RANGE1
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
#if !defined(STM32_PWR_I3CPUCR1) || defined(__DOXYGEN__)
#define STM32_PWR_I3CPUCR2                  (0U)
#endif

/**
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM32_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI_ENABLED                   TRUE
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
#define STM32_STOPWUCK                      STM32_STOPWUCK_HSI
#endif

/**
 * @brief   Kernel clock source after STOP.
 */
#if !defined(STM32_STOPKERWUCK) || defined(__DOXYGEN__)
#define STM32_STOPKERWUCK                   STM32_STOPKERWUCK_HSI
#endif

/**
 * @brief   RTC prescaler value.
 */
#if !defined(STM32_RTCPRE_VALUE) || defined(__DOXYGEN__)
#define STM32_RTCPRE_VALUE                  STM32_RTCPRE_NOCLOCK
#endif

/**
 * @brief   TIMPRE timers clocks prescaler selection.
 */
#if !defined(STM32_TIMPRE) || defined(__DOXYGEN__)
#define STM32_TIMPRE                        STM32_TIMPRE_LOW
#endif

/**
 * @brief   MCO1 clock source.
 */
#if !defined(STM32_MCO1SEL) || defined(__DOXYGEN__)
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI
#endif

/**
 * @brief   MCO1 divider setting.
 */
#if !defined(STM32_MCO1PRE_VALUE) || defined(__DOXYGEN__)
#define STM32_MCO1PRE_VALUE                 STM32_MCO1PRE_NOCLOCK
#endif

/**
 * @brief   MCO2 clock source.
 */
#if !defined(STM32_MCO2SEL) || defined(__DOXYGEN__)
#define STM32_MCO2SEL                       STM32_MCO2SEL_SYSCLK
#endif

/**
 * @brief   MCO1 divider setting.
 */
#if !defined(STM32_MCO2PRE_VALUE) || defined(__DOXYGEN__)
#define STM32_MCO2PRE_VALUE                 STM32_MCO2PRE_NOCLOCK
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
#define STM32_TIMICSEL                      STM32_TIMICSEL_NOCLK
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
#define STM32_USART6SEL                     STM32_USART6SEL_PCLK1
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
 * @brief   UART9 clock source.
 */
#if !defined(STM32_UART9SEL) || defined(__DOXYGEN__)
#define STM32_UART9SEL                      STM32_UART9SEL_PCLK1
#endif

/**
 * @brief   USART10 clock source.
 */
#if !defined(STM32_USART10SEL) || defined(__DOXYGEN__)
#define STM32_USART10SEL                    STM32_USART10SEL_PCLK1
#endif

/**
 * @brief   USART11 clock source.
 */
#if !defined(STM32_USART11SEL) || defined(__DOXYGEN__)
#define STM32_USART11SEL                    STM32_USART11SEL_PCLK1
#endif

/**
 * @brief   UART12 clock source.
 */
#if !defined(STM32_UART12SEL) || defined(__DOXYGEN__)
#define STM32_UART12SEL                     STM32_UART12SEL_PCLK1
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
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK3
#endif

/**
 * @brief   LPTIM2 clock source.
 */
#if !defined(STM32_LPTIM2SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM2SEL                     STM32_LPTIM2SEL_PCLK1
#endif

/**
 * @brief   LPTIM3 clock source.
 */
#if !defined(STM32_LPTIM3SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM3SEL                     STM32_LPTIM3SEL_PCLK3
#endif

/**
 * @brief   LPTIM4 clock source.
 */
#if !defined(STM32_LPTIM4SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM4SEL                     STM32_LPTIM4SEL_PCLK3
#endif

/**
 * @brief   LPTIM5 clock source.
 */
#if !defined(STM32_LPTIM5SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM5SEL                     STM32_LPTIM5SEL_PCLK3
#endif

/**
 * @brief   LPTIM6 clock source.
 */
#if !defined(STM32_LPTIM6SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM6SEL                     STM32_LPTIM6SEL_PCLK3
#endif

/**
 * @brief   SPI1 clock source.
 */
#if !defined(STM32_SPI1SEL) || defined(__DOXYGEN__)
#define STM32_SPI1SEL                       STM32_SPI1SEL_PLL1Q
#endif

/**
 * @brief   SPI2 clock source.
 */
#if !defined(STM32_SPI2SEL) || defined(__DOXYGEN__)
#define STM32_SPI2SEL                       STM32_SPI2SEL_PLL1Q
#endif

/**
 * @brief   SPI3 clock source.
 */
#if !defined(STM32_SPI3SEL) || defined(__DOXYGEN__)
#define STM32_SPI3SEL                       STM32_SPI3SEL_PLL1Q
#endif

/**
 * @brief   SPI4 clock source.
 */
#if !defined(STM32_SPI4SEL) || defined(__DOXYGEN__)
#define STM32_SPI4SEL                       STM32_SPI4SEL_PCLK2
#endif

/**
 * @brief   SPI5 clock source.
 */
#if !defined(STM32_SPI5SEL) || defined(__DOXYGEN__)
#define STM32_SPI5SEL                       STM32_SPI5SEL_PCLK3
#endif

/**
 * @brief   SPI6 clock source.
 */
#if !defined(STM32_SPI6SEL) || defined(__DOXYGEN__)
#define STM32_SPI6SEL                       STM32_SPI6SEL_PCLK2
#endif

/**
 * @brief   QSPI clock source.
 */
#if !defined(STM32_OSPISEL) || defined(__DOXYGEN__)
#define STM32_OSPISEL                       STM32_OSPISEL_HCLK4
#endif

/**
 * @brief   SYSTICK clock source.
 */
#if !defined(STM32_SYSTICKSEL) || defined(__DOXYGEN__)
#define STM32_SYSTICKSEL                    STM32_SYSTICKSEL_HCLKDIV8
#endif

/**
 * @brief   USB clock source.
 */
#if !defined(STM32_USBSEL) || defined(__DOXYGEN__)
#define STM32_USBSEL                        STM32_USBSEL_NOCLOCK
#endif

/**
 * @brief   SDMMC1 clock source.
 */
#if !defined(STM32_SDMMC1SEL) || defined(__DOXYGEN__)
#define STM32_SDMMC1SEL                     STM32_SDMMC1SEL_PLL1Q
#endif

/**
 * @brief   SDMMC2 clock source.
 */
#if !defined(STM32_SDMMC2SEL) || defined(__DOXYGEN__)
#define STM32_SDMMC2SEL                     STM32_SDMMC2SEL_PLL1Q
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
 * @brief   I2C4 clock source.
 */
#if !defined(STM32_I2C4SEL) || defined(__DOXYGEN__)
#define STM32_I2C4SEL                       STM32_I2C4SEL_PCLK3
#endif

/**
 * @brief   I3C1 clock source.
 */
#if !defined(STM32_I3C1SEL) || defined(__DOXYGEN__)
#define STM32_I3C1SEL                       STM32_I3C1SEL_PCLK1
#endif

/**
 * @brief   ADCDACSEL clock source.
 */
#if !defined(STM32_ADCDACSEL) || defined(__DOXYGEN__)
#define STM32_ADCDACSEL                     STM32_ADCDACSEL_HCLK
#endif

/**
 * @brief   DACSEL clock source.
 */
#if !defined(STM32_DACSEL) || defined(__DOXYGEN__)
#define STM32_DACSEL                        STM32_DACSEL_LSE
#endif

/**
 * @brief   RNG clock source.
 */
#if !defined(STM32_RNGSEL) || defined(__DOXYGEN__)
#define STM32_RNGSEL                        STM32_RNGSEL_HSI48
#endif

/**
 * @brief   CEC clock source.
 */
#if !defined(STM32_CECSEL) || defined(__DOXYGEN__)
#define STM32_CECSEL                        STM32_CECSEL_LSE
#endif

/**
 * @brief   FDCAN clock source.
 */
#if !defined(STM32_FDCANSEL) || defined(__DOXYGEN__)
#define STM32_FDCANSEL                      STM32_FDCANSEL_HSE
#endif

/**
 * @brief   SAI1 clock source.
 */
#if !defined(STM32_SAI1SEL) || defined(__DOXYGEN__)
#define STM32_SAI1SEL                       STM32_SAI1SEL_PLL1Q
#endif

/**
 * @brief   SAI2 clock source.
 */
#if !defined(STM32_SAI2SEL) || defined(__DOXYGEN__)
#define STM32_SAI2SEL                       STM32_SAI2SEL_PLL1Q
#endif

/**
 * @brief   CKPERSEL clock source.
 */
#if !defined(STM32_CKPERSEL) || defined(__DOXYGEN__)
#define STM32_CKPERSEL                      STM32_CKPERSEL_HSI
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
#if !defined(STM32H5xx_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32H5xx_MCUCONF not defined"
#endif

#if defined(STM32H503xx) && !defined(STM32H503_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32H503_MCUCONF not defined"

#elif defined(STM32H562xx) && !defined(STM32H562_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32H562_MCUCONF not defined"

#elif defined(STM32H563xx) && !defined(STM32H563_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32H563_MCUCONF not defined"

#elif defined(STM32H573xx) && !defined(STM32H573_MCUCONF)
#error "Using a wrong mcuconf.h file, STM32H573_MCUCONF not defined"

#endif

/* Device limits.*/
#include "stm32_limits.h"

/* ICache handler.*/
#include "stm32_icache.inc"

/* Clock handlers.*/
#include "stm32_lsi.inc"
#include "stm32_csi.inc"
#include "stm32_hsi48.inc"
#include "stm32_hsidiv.inc"
#include "stm32_lse.inc"
#include "stm32_hse.inc"

/*
 * PLL1 enable check.
 */
#if (STM32_SW           == STM32_SW_PLL1P)          ||                      \
    (STM32_MCO1SEL      == STM32_MCO1SEL_PLL1P)     ||                      \
    (STM32_MCO2SEL      == STM32_MCO2SEL_PLL1P)     ||                      \
    (STM32_SPI1SEL      == STM32_SPI1SEL_PLL1Q)     ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_PLL1Q)     ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_PLL1Q)     ||                      \
    (STM32_OSPISEL      == STM32_OSPISEL_PLL1Q)     ||                      \
    (STM32_USBSEL       == STM32_USBSEL_PLL1Q)      ||                      \
    (STM32_SDMMC1SEL    == STM32_SDMMC1SEL_PLL1Q)   ||                      \
    (STM32_SDMMC2SEL    == STM32_SDMMC2SEL_PLL1Q)   ||                      \
    (STM32_RNGSEL       == STM32_RNGSEL_PLL1Q)      ||                      \
    (STM32_FDCANSEL     == STM32_FDCANSEL_PLL1Q)    ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_PLL1Q)     ||                      \
    (STM32_SAI2SEL      == STM32_SAI2SEL_PLL1Q)     ||                      \
    defined(__DOXYGEN__)
  /**
   * @brief   PLL1 activation flag.
   */
  #define STM32_ACTIVATE_PLL1       TRUE
#else

  #define STM32_ACTIVATE_PLL1       FALSE
#endif

/*
 * PLL2 enable check.
 */
#if (STM32_MCO2SEL      == STM32_MCO2SEL_PLL2P)     ||                      \
    (STM32_USART1SEL    == STM32_USART1SEL_PLL2Q)   ||                      \
    (STM32_USART2SEL    == STM32_USART2SEL_PLL2Q)   ||                      \
    (STM32_USART3SEL    == STM32_USART3SEL_PLL2Q)   ||                      \
    (STM32_UART4SEL     == STM32_UART4SEL_PLL2Q)    ||                      \
    (STM32_UART5SEL     == STM32_UART5SEL_PLL2Q)    ||                      \
    (STM32_USART6SEL    == STM32_USART6SEL_PLL2Q)   ||                      \
    (STM32_UART7SEL     == STM32_UART7SEL_PLL2Q)    ||                      \
    (STM32_UART8SEL     == STM32_UART8SEL_PLL2Q)    ||                      \
    (STM32_UART9SEL     == STM32_UART9SEL_PLL2Q)    ||                      \
    (STM32_USART10SEL   == STM32_USART10SEL_PLL2Q)  ||                      \
    (STM32_USART11SEL   == STM32_USART11SEL_PLL2Q)  ||                      \
    (STM32_UART12SEL    == STM32_UART12SEL_PLL2Q)   ||                      \
    (STM32_LPUART1SEL   == STM32_LPUART1SEL_PLL2Q)  ||                      \
    (STM32_LPTIM1SEL    == STM32_LPTIM1SEL_PLL2P)   ||                      \
    (STM32_LPTIM2SEL    == STM32_LPTIM2SEL_PLL2P)   ||                      \
    (STM32_LPTIM3SEL    == STM32_LPTIM3SEL_PLL2P)   ||                      \
    (STM32_LPTIM4SEL    == STM32_LPTIM4SEL_PLL2P)   ||                      \
    (STM32_LPTIM5SEL    == STM32_LPTIM5SEL_PLL2P)   ||                      \
    (STM32_LPTIM6SEL    == STM32_LPTIM6SEL_PLL2P)   ||                      \
    (STM32_SPI1SEL      == STM32_SPI1SEL_PLL2P)     ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_PLL2P)     ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_PLL2P)     ||                      \
    (STM32_SPI4SEL      == STM32_SPI4SEL_PLL2P)     ||                      \
    (STM32_SPI5SEL      == STM32_SPI5SEL_PLL2P)     ||                      \
    (STM32_SPI6SEL      == STM32_SPI6SEL_PLL2P)     ||                      \
    (STM32_OSPISEL      == STM32_OSPISEL_PLL2R )    ||                      \
    (STM32_SDMMC1SEL    == STM32_SDMMC1SEL_PLL2R)   ||                      \
    (STM32_SDMMC2SEL    == STM32_SDMMC2SEL_PLL2R)   ||                      \
    (STM32_ADCDACSEL    == STM32_ADCDACSEL_PLL2R)   ||                      \
    (STM32_FDCANSEL     == STM32_FDCANSEL_PLL2Q)    ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_PLL2P)     ||                      \
    (STM32_SAI2SEL      == STM32_SAI2SEL_PLL2P)     ||                      \
    defined(__DOXYGEN__)
  /**
   * @brief   PLL2 activation flag.
   */
  #define STM32_ACTIVATE_PLL2       TRUE
#else

  #define STM32_ACTIVATE_PLL2       FALSE
#endif

/*
 * PLL3 enable check.
 */
#if (STM32_USART1SEL    == STM32_USART1SEL_PLL3Q)   ||                      \
    (STM32_USART2SEL    == STM32_USART2SEL_PLL3Q)   ||                      \
    (STM32_USART3SEL    == STM32_USART3SEL_PLL3Q)   ||                      \
    (STM32_UART4SEL     == STM32_UART4SEL_PLL3Q)    ||                      \
    (STM32_UART5SEL     == STM32_UART5SEL_PLL3Q)    ||                      \
    (STM32_USART6SEL    == STM32_USART6SEL_PLL3Q)   ||                      \
    (STM32_UART7SEL     == STM32_UART7SEL_PLL3Q)    ||                      \
    (STM32_UART8SEL     == STM32_UART8SEL_PLL3Q)    ||                      \
    (STM32_UART9SEL     == STM32_UART9SEL_PLL3Q)    ||                      \
    (STM32_USART10SEL   == STM32_USART10SEL_PLL3Q)  ||                      \
    (STM32_USART11SEL   == STM32_USART11SEL_PLL3Q)  ||                      \
    (STM32_UART12SEL    == STM32_UART12SEL_PLL3Q)   ||                      \
    (STM32_LPUART1SEL   == STM32_LPUART1SEL_PLL3Q)  ||                      \
    (STM32_LPTIM1SEL    == STM32_LPTIM1SEL_PLL3R)   ||                      \
    (STM32_LPTIM2SEL    == STM32_LPTIM2SEL_PLL3R)   ||                      \
    (STM32_LPTIM3SEL    == STM32_LPTIM3SEL_PLL3R)   ||                      \
    (STM32_LPTIM4SEL    == STM32_LPTIM4SEL_PLL3R)   ||                      \
    (STM32_LPTIM5SEL    == STM32_LPTIM5SEL_PLL3R)   ||                      \
    (STM32_LPTIM6SEL    == STM32_LPTIM6SEL_PLL3R)   ||                      \
    (STM32_SPI1SEL      == STM32_SPI1SEL_PLL3P)     ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_PLL3P)     ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_PLL3P)     ||                      \
    (STM32_SPI4SEL      == STM32_SPI4SEL_PLL3P)     ||                      \
    (STM32_SPI5SEL      == STM32_SPI5SEL_PLL3P)     ||                      \
    (STM32_SPI6SEL      == STM32_SPI6SEL_PLL3P)     ||                      \
    (STM32_USBSEL       == STM32_USBSEL_PLL3Q)      ||                      \
    (STM32_I2C1SEL      == STM32_I2C1SEL_PLL3R)     ||                      \
    (STM32_I2C2SEL      == STM32_I2C2SEL_PLL3R)     ||                      \
    (STM32_I2C3SEL      == STM32_I2C3SEL_PLL3R)     ||                      \
    (STM32_I2C4SEL      == STM32_I2C4SEL_PLL3R)     ||                      \
    (STM32_I3C1SEL      == STM32_I3C1SEL_PLL3R)     ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_PLL3P)     ||                      \
    (STM32_SAI2SEL      == STM32_SAI2SEL_PLL3P)     ||                      \
    defined(__DOXYGEN__)
  /**
   * @brief   PLL3 activation flag.
   */
  #define STM32_ACTIVATE_PLL3       TRUE
#else

  #define STM32_ACTIVATE_PLL3       FALSE
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
    #error "HSI not enabled, required by STM32_LPTIM1SEL"
  #endif
  #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSI)
    #error "HSI not enabled, required by STM32_LPTIM2SEL"
  #endif
  #if (STM32_LPTIM3SEL == STM32_LPTIM3SEL_LSI)
    #error "HSI not enabled, required by STM32_LPTIM3SEL"
  #endif
  #if (STM32_LPTIM4SEL == STM32_LPTIM4SEL_LSI)
    #error "HSI not enabled, required by STM32_LPTIM4SEL"
  #endif
  #if (STM32_LPTIM5SEL == STM32_LPTIM5SEL_LSI)
    #error "HSI not enabled, required by STM32_LPTIM5SEL"
  #endif
  #if (STM32_LPTIM6SEL == STM32_LPTIM6SEL_LSI)
    #error "HSI not enabled, required by STM32_LPTIM6SEL"
  #endif

  #if STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSI
    #error "LSI not enabled, required by STM32_SYSTICKSEL"
  #endif

  #if STM32_RNGSEL == STM32_RNGSEL_LSI
    #error "LSI not enabled, required by STM32_RNGSEL"
  #endif

  #if STM32_DACSEL == STM32_DACSEL_LSI
    #error "LSI not enabled, required by STM32_DACSEL"
  #endif

  #if STM32_CECSEL == STM32_CECSEL_LSI
    #error "LSI not enabled, required by STM32_CECSEL"
  #endif

  #if HAL_USE_RTC && (STM32_RTCSEL == STM32_RTCSEL_LSI)
    #error "LSI not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_LSCOSEL == STM32_LSCOSEL_LSI
    #error "LSI not enabled, required by STM32_LSCOSEL"
  #endif

#endif /* !STM32_LSI_ENABLED */

/*
 * CSI related checks.
 */
#if STM32_CSI_ENABLED
#else /* !STM32_CSI_ENABLED */

  #if STM32_ACTIVATE_PLL1 && (STM32_PLL1SRC == STM32_PLL1SRC_CSI)
    #error "CSI not enabled, required by STM32_PLL1SRC"
  #endif
  #if STM32_ACTIVATE_PLL2 && (STM32_PLL1SRC == STM32_PLL2SRC_CSI)
    #error "CSI not enabled, required by STM32_PLL2SRC"
  #endif
  #if STM32_ACTIVATE_PLL3 && (STM32_PLL1SRC == STM32_PLL3SRC_CSI)
    #error "CSI not enabled, required by STM32_PLL3SRC"
  #endif

  #if STM32_SW == STM32_SW_CSI
    #error "CSI not enabled, required by STM32_SW"
  #endif

  #if STM32_MCO2SEL == STM32_MCO2SEL_CSI
    #error "CSI not enabled, required by STM32_MCO2SEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_CSI)
    #error "CSI not enabled, required by STM32_USART1SEL"
  #endif
  #if (STM32_USART2SEL == STM32_USART2SEL_CSI)
    #error "CSI not enabled, required by STM32_USART2SEL"
  #endif
  #if (STM32_USART3SEL == STM32_USART3SEL_CSI)
    #error "CSI not enabled, required by STM32_USART3SEL"
  #endif
  #if (STM32_UART4SEL == STM32_UART4SEL_CSI)
    #error "CSI not enabled, required by STM32_UART4SEL"
  #endif
  #if (STM32_UART5SEL == STM32_UART5SEL_CSI)
    #error "CSI not enabled, required by STM32_UART5SEL"
  #endif
  #if (STM32_USART6SEL == STM32_USART6SEL_CSI)
    #error "CSI not enabled, required by STM32_USART6SEL"
  #endif
  #if (STM32_UART7SEL == STM32_UART7SEL_CSI)
    #error "CSI not enabled, required by STM32_UART7SEL"
  #endif
  #if (STM32_UART8SEL == STM32_UART8SEL_CSI)
    #error "CSI not enabled, required by STM32_UART8SEL"
  #endif
  #if (STM32_UART9SEL == STM32_UART9SEL_CSI)
    #error "CSI not enabled, required by STM32_UART9SEL"
  #endif
  #if (STM32_USART10SEL == STM32_USART10SEL_CSI)
    #error "CSI not enabled, required by STM32_USART10SEL"
  #endif
  #if (STM32_USART11SEL == STM32_USART11SEL_CSI)
    #error "CSI not enabled, required by STM32_USART11SEL"
  #endif
  #if (STM32_UART12SEL == STM32_UART12SEL_CSI)
    #error "CSI not enabled, required by STM32_UART12SEL"
  #endif
  #if (STM32_LPUART1SEL == STM32_LPUART1SEL_CSI)
    #error "CSI not enabled, required by STM32_LPUART1SEL"
  #endif

  #if (STM32_SPI4SEL == STM32_SPI4SEL_CSI)
    #error "CSI not enabled, required by STM32_SPI4SEL"
  #endif
  #if (STM32_SPI5SEL == STM32_SPI5SEL_CSI)
    #error "CSI not enabled, required by STM32_SPI5SEL"
  #endif
  #if (STM32_SPI6SEL == STM32_SPI6SEL_CSI)
    #error "CSI not enabled, required by STM32_SPI6SEL"
  #endif

  #if (STM32_I2C1SEL == STM32_I2C1SEL_CSI)
    #error "CSI not enabled, required by STM32_I2C1SEL"
  #endif
  #if (STM32_I2C2SEL == STM32_I2C2SEL_CSI)
    #error "CSI not enabled, required by STM32_I2C2SEL"
  #endif
  #if (STM32_I2C3SEL == STM32_I2C3SEL_CSI)
    #error "CSI not enabled, required by STM32_I2C3SEL"
  #endif
  #if (STM32_I2C4SEL == STM32_I2C4SEL_CSI)
    #error "CSI not enabled, required by STM32_I2C4SEL"
  #endif

  #if (STM32_ADCDACSEL == STM32_ADCDACSEL_CSI)
    #error "CSI not enabled, required by STM32_ADCDACSEL"
  #endif

  #if STM32_CECSEL == STM32_CECSEL_CSIDIV128
    #error "CSI not enabled, required by STM32_CECSEL"
  #endif

  #if STM32_CKPERSEL == STM32_CKPERSEL_CSI
    #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PER)
      #error "CSI not enabled, required by STM32_LPTIM1SEL"
    #endif
    #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_PER)
      #error "CSI not enabled, required by STM32_LPTIM2SEL"
    #endif
    #if (STM32_LPTIM3SEL == STM32_LPTIM3SEL_PER)
      #error "CSI not enabled, required by STM32_LPTIM3SEL"
    #endif
    #if (STM32_LPTIM4SEL == STM32_LPTIM4SEL_PER)
      #error "CSI not enabled, required by STM32_LPTIM4SEL"
    #endif
    #if (STM32_LPTIM5SEL == STM32_LPTIM5SEL_PER)
      #error "CSI not enabled, required by STM32_LPTIM5SEL"
    #endif
    #if (STM32_LPTIM6SEL == STM32_LPTIM6SEL_PER)
      #error "CSI not enabled, required by STM32_LPTIM6SEL"
    #endif

    #if (STM32_SPI1SEL == STM32_SPI1SEL_PER)
      #error "CSI not enabled, required by STM32_SPI1SEL"
    #endif
    #if (STM32_SPI2SEL == STM32_SPI2SEL_PER)
      #error "CSI not enabled, required by STM32_SPI2SEL"
    #endif
    #if (STM32_SPI3SEL == STM32_SPI3SEL_PER)
      #error "CSI not enabled, required by STM32_SPI3SEL"
    #endif

    #if (STM32_OSPISEL == STM32_OSPISEL_PER)
      #error "CSI not enabled, required by STM32_OSPISEL"
    #endif

    #if (STM32_SAI1SEL == STM32_SAI1SEL_PER)
      #error "CSI not enabled, required by STM32_SAI1SEL"
    #endif
    #if (STM32_SAI2SEL == STM32_SAI2SEL_PER)
      #error "CSI not enabled, required by STM32_SAI2SEL"
    #endif
  #endif
#endif /* !STM32_CSI_ENABLED */

/*
 * HSI48 related checks.
 */
#if STM32_HSI48_ENABLED
#else /* !STM32_HSI48_ENABLED */

  #if STM32_MCO1SEL == STM32_MCO1SEL_HSI48
    #error "HSI48 not enabled, required by STM32_MCO1SEL"
  #endif

  #if STM32_USBSEL == STM32_USBSEL_HSI48
    #error "HSI48 not enabled, required by STM32_USBSEL"
  #endif

  #if STM32_RNGSEL == STM32_RNGSEL_HSI48
    #error "HSI48 not enabled, required by STM32_RNGSEL"
  #endif

#endif /* !STM32_HSI48_ENABLED */

/*
 * HSI related checks.
 */
#if STM32_HSI_ENABLED
#else /* !STM32_HSI_ENABLED */

  #if STM32_ACTIVATE_PLL1 && (STM32_PLL1SRC == STM32_PLL1SRC_HSI)
    #error "HSI not enabled, required by STM32_PLL1SRC"
  #endif
  #if STM32_ACTIVATE_PLL2 && (STM32_PLL1SRC == STM32_PLL2SRC_HSI)
    #error "HSI not enabled, required by STM32_PLL2SRC"
  #endif
  #if STM32_ACTIVATE_PLL3 && (STM32_PLL1SRC == STM32_PLL3SRC_HSI)
    #error "HSI not enabled, required by STM32_PLL3SRC"
  #endif

  #if STM32_SW == STM32_SW_HSI
    #error "HSI not enabled, required by STM32_SW"
  #endif

  #if STM32_MCO1SEL == STM32_MCO1SEL_HSI
    #error "HSI not enabled, required by STM32_MCO1SEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_HSI)
    #error "HSI not enabled, required by STM32_USART1SEL"
  #endif
  #if (STM32_USART2SEL == STM32_USART2SEL_HSI)
    #error "HSI not enabled, required by STM32_USART2SEL"
  #endif
  #if (STM32_USART3SEL == STM32_USART3SEL_HSI)
    #error "HSI not enabled, required by STM32_USART3SEL"
  #endif
  #if (STM32_UART4SEL == STM32_UART4SEL_HSI)
    #error "HSI not enabled, required by STM32_UART4SEL"
  #endif
  #if (STM32_UART5SEL == STM32_UART5SEL_HSI)
    #error "HSI not enabled, required by STM32_UART5SEL"
  #endif
  #if (STM32_USART6SEL == STM32_USART6SEL_HSI)
    #error "HSI not enabled, required by STM32_USART6SEL"
  #endif
  #if (STM32_UART7SEL == STM32_UART7SEL_HSI)
    #error "HSI not enabled, required by STM32_UART7SEL"
  #endif
  #if (STM32_UART8SEL == STM32_UART8SEL_HSI)
    #error "HSI not enabled, required by STM32_UART8SEL"
  #endif
  #if (STM32_UART9SEL == STM32_UART9SEL_HSI)
    #error "HSI not enabled, required by STM32_UART9SEL"
  #endif
  #if (STM32_USART10SEL == STM32_USART10SEL_HSI)
    #error "HSI not enabled, required by STM32_USART10SEL"
  #endif
  #if (STM32_USART11SEL == STM32_USART11SEL_HSI)
    #error "HSI not enabled, required by STM32_USART11SEL"
  #endif
  #if (STM32_UART12SEL == STM32_UART12SEL_HSI)
    #error "HSI not enabled, required by STM32_UART12SEL"
  #endif
  #if (STM32_LPUART1SEL == STM32_LPUART1SEL_HSI)
    #error "HSI not enabled, required by STM32_LPUART1SEL"
  #endif

  #if (STM32_SPI4SEL == STM32_SPI4SEL_HSI)
    #error "HSI not enabled, required by STM32_SPI4SEL"
  #endif
  #if (STM32_SPI5SEL == STM32_SPI5SEL_HSI)
    #error "HSI not enabled, required by STM32_SPI5SEL"
  #endif
  #if (STM32_SPI6SEL == STM32_SPI6SEL_HSI)
    #error "HSI not enabled, required by STM32_SPI6SEL"
  #endif

  #if (STM32_I2C1SEL == STM32_I2C1SEL_HSI)
    #error "HSI not enabled, required by STM32_I2C1SEL"
  #endif
  #if (STM32_I2C2SEL == STM32_I2C2SEL_HSI)
    #error "HSI not enabled, required by STM32_I2C2SEL"
  #endif
  #if (STM32_I2C3SEL == STM32_I2C3SEL_HSI)
    #error "HSI not enabled, required by STM32_I2C3SEL"
  #endif
  #if (STM32_I2C4SEL == STM32_I2C4SEL_HSI)
    #error "HSI not enabled, required by STM32_I2C4SEL"
  #endif

  #if (STM32_I3C1SEL == STM32_I3C1SEL_HSI)
    #error "HSI not enabled, required by STM32_I3C1SEL"
  #endif

  #if (STM32_ADCDACSEL == STM32_ADCDACSEL_HSI)
    #error "HSI not enabled, required by STM32_ADCDACSEL"
  #endif

  #if STM32_CKPERSEL == STM32_CKPERSEL_HSI
    #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PER)
      #error "HSI not enabled, required by STM32_LPTIM1SEL"
    #endif
    #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_PER)
      #error "HSI not enabled, required by STM32_LPTIM2SEL"
    #endif
    #if (STM32_LPTIM3SEL == STM32_LPTIM3SEL_PER)
      #error "HSI not enabled, required by STM32_LPTIM3SEL"
    #endif
    #if (STM32_LPTIM4SEL == STM32_LPTIM4SEL_PER)
      #error "HSI not enabled, required by STM32_LPTIM4SEL"
    #endif
    #if (STM32_LPTIM5SEL == STM32_LPTIM5SEL_PER)
      #error "HSI not enabled, required by STM32_LPTIM5SEL"
    #endif
    #if (STM32_LPTIM6SEL == STM32_LPTIM6SEL_PER)
      #error "HSI not enabled, required by STM32_LPTIM6SEL"
    #endif

    #if (STM32_SPI1SEL == STM32_SPI1SEL_PER)
      #error "HSI not enabled, required by STM32_SPI1SEL"
    #endif
    #if (STM32_SPI2SEL == STM32_SPI2SEL_PER)
      #error "HSI not enabled, required by STM32_SPI2SEL"
    #endif
    #if (STM32_SPI3SEL == STM32_SPI3SEL_PER)
      #error "HSI not enabled, required by STM32_SPI3SEL"
    #endif

    #if (STM32_OSPISEL == STM32_OSPISEL_PER)
      #error "HSI not enabled, required by STM32_OSPISEL"
    #endif

    #if (STM32_SAI1SEL == STM32_SAI1SEL_PER)
      #error "HSI not enabled, required by STM32_SAI1SEL"
    #endif
    #if (STM32_SAI2SEL == STM32_SAI2SEL_PER)
      #error "HSI not enabled, required by STM32_SAI2SEL"
    #endif
  #endif

#endif /* !STM32_HSI_ENABLED */

/*
 * LSE related checks.
 */
#if STM32_LSE_ENABLED
#else /* !STM32_LSE_ENABLED */

  #if STM32_MCO1SEL == STM32_MCO1SEL_LSE
    #error "LSE not enabled, required by STM32_MCO1SEL"
  #endif

  #if (STM32_USART1SEL == STM32_USART1SEL_LSE)
    #error "LSE not enabled, required by STM32_USART1SEL"
  #endif
  #if (STM32_USART2SEL == STM32_USART2SEL_LSE)
    #error "LSE not enabled, required by STM32_USART2SEL"
  #endif
  #if (STM32_USART3SEL == STM32_USART3SEL_LSE)
    #error "LSE not enabled, required by STM32_USART3SEL"
  #endif
  #if (STM32_UART4SEL == STM32_UART4SEL_LSE)
    #error "LSE not enabled, required by STM32_UART4SEL"
  #endif
  #if (STM32_UART5SEL == STM32_UART5SEL_LSE)
    #error "LSE not enabled, required by STM32_UART5SEL"
  #endif
  #if (STM32_USART6SEL == STM32_USART6SEL_LSE)
    #error "LSE not enabled, required by STM32_USART6SEL"
  #endif
  #if (STM32_UART7SEL == STM32_UART7SEL_LSE)
    #error "LSE not enabled, required by STM32_UART7SEL"
  #endif
  #if (STM32_UART8SEL == STM32_UART8SEL_LSE)
    #error "LSE not enabled, required by STM32_UART8SEL"
  #endif
  #if (STM32_UART9SEL == STM32_UART9SEL_LSE)
    #error "LSE not enabled, required by STM32_UART9SEL"
  #endif
  #if (STM32_USART10SEL == STM32_USART10SEL_LSE)
    #error "LSE not enabled, required by STM32_USART10SEL"
  #endif
  #if (STM32_USART11SEL == STM32_USART11SEL_LSE)
    #error "LSE not enabled, required by STM32_USART11SEL"
  #endif
  #if (STM32_UART12SEL == STM32_UART12SEL_LSE)
    #error "LSE not enabled, required by STM32_UART12SEL"
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
  #if (STM32_LPTIM3SEL == STM32_LPTIM3SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM3SEL"
  #endif
  #if (STM32_LPTIM4SEL == STM32_LPTIM4SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM4SEL"
  #endif
  #if (STM32_LPTIM5SEL == STM32_LPTIM5SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM5SEL"
  #endif
  #if (STM32_LPTIM6SEL == STM32_LPTIM6SEL_LSE)
    #error "LSE not enabled, required by STM32_LPTIM6SEL"
  #endif

  #if STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSE
    #error "LSE not enabled, required by STM32_SYSTICKSEL"
  #endif

  #if STM32_DACSEL == STM32_DACSEL_LSE
    #error "LSE not enabled, required by STM32_DACSEL"
  #endif

  #if STM32_CECSEL == STM32_CECSEL_LSE
    #error "LSE not enabled, required by STM32_CECSEL"
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

  #if STM32_ACTIVATE_PLL1 && (STM32_PLL1SRC == STM32_PLL1SRC_HSE)
    #error "HSE not enabled, required by STM32_PLL1SRC"
  #endif
  #if STM32_ACTIVATE_PLL2 && (STM32_PLL1SRC == STM32_PLL2SRC_HSE)
    #error "HSE not enabled, required by STM32_PLL2SRC"
  #endif
  #if STM32_ACTIVATE_PLL3 && (STM32_PLL1SRC == STM32_PLL3SRC_HSE)
    #error "HSE not enabled, required by STM32_PLL3SRC"
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

  #if (STM32_SPI4SEL == STM32_SPI4SEL_HSE)
    #error "HSE not enabled, required by STM32_SPI4SEL"
  #endif
  #if (STM32_SPI5SEL == STM32_SPI5SEL_HSE)
    #error "HSE not enabled, required by STM32_SPI5SEL"
  #endif
  #if (STM32_SPI6SEL == STM32_SPI6SEL_HSE)
    #error "HSE not enabled, required by STM32_SPI6SEL"
  #endif

  #if STM32_FDCANSEL == STM32_FDCANSEL_HSE
    #error "HSE not enabled, required by STM32_FDCANSEL"
  #endif

  #if STM32_RTCSEL == STM32_RTCSEL_HSEDIV
    #error "HSE not enabled, required by STM32_RTCSEL"
  #endif

  #if STM32_CKPERSEL == STM32_CKPERSEL_HSE
    #if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSE)
      #error "HSE not enabled, required by STM32_LPTIM1SEL"
    #endif
    #if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_HSE)
      #error "HSE not enabled, required by STM32_LPTIM2SEL"
    #endif
    #if (STM32_LPTIM3SEL == STM32_LPTIM3SEL_HSE)
      #error "HSE not enabled, required by STM32_LPTIM3SEL"
    #endif
    #if (STM32_LPTIM4SEL == STM32_LPTIM4SEL_HSE)
      #error "HSE not enabled, required by STM32_LPTIM4SEL"
    #endif
    #if (STM32_LPTIM5SEL == STM32_LPTIM5SEL_HSE)
      #error "HSE not enabled, required by STM32_LPTIM5SEL"
    #endif
    #if (STM32_LPTIM6SEL == STM32_LPTIM6SEL_HSE)
      #error "HSE not enabled, required by STM32_LPTIM6SEL"
    #endif

    #if (STM32_SPI1SEL == STM32_SPI1SEL_HSE)
      #error "HSE not enabled, required by STM32_SPI1SEL"
    #endif
    #if (STM32_SPI2SEL == STM32_SPI2SEL_HSE)
      #error "HSE not enabled, required by STM32_SPI2SEL"
    #endif
    #if (STM32_SPI3SEL == STM32_SPI3SEL_HSE)
      #error "HSE not enabled, required by STM32_SPI3SEL"
    #endif

    #if (STM32_OSPISEL == STM32_OSPISEL_HSE)
      #error "HSE not enabled, required by STM32_OSPISEL"
    #endif

    #if (STM32_SAI1SEL == STM32_SAI1SEL_HSE)
      #error "HSE not enabled, required by STM32_SAI1SEL"
    #endif
    #if (STM32_SAI2SEL == STM32_SAI2SEL_HSE)
      #error "HSE not enabled, required by STM32_SAI2SEL"
    #endif
  #endif
#endif /* !STM32_HSE_ENABLED */

/**
 * @brief   PLL1 input clock frequency.
 */
#if (STM32_PLL1SRC == STM32_PLL1SRC_HSE) || defined(__DOXYGEN__)
  #define STM32_PLL1IN              STM32_HSECLK

#elif STM32_PLL1SRC == STM32_PLL1SRC_CSI
  #define STM32_PLL1IN              STM32_CSICLK

#elif STM32_PLL1SRC == STM32_PLL1SRC_HSI
  #define STM32_PLL1IN              STM32_HSICLK

#elif STM32_PLL1SRC == STM32_PLL1SRC_NOCLOCK
  #define STM32_PLL1IN              0

#else
  #error "invalid STM32_PLL1SRC value specified"
#endif

/**
 * @brief   PLL2 input clock frequency.
 */
#if (STM32_PLL2SRC == STM32_PLL2SRC_HSE) || defined(__DOXYGEN__)
  #define STM32_PLL2IN              STM32_HSECLK

#elif STM32_PLL2SRC == STM32_PLL2SRC_CSI
  #define STM32_PLL2IN              STM32_CSICLK

#elif STM32_PLL2SRC == STM32_PLL2SRC_HSI
  #define STM32_PLL2IN              STM32_HSICLK

#elif STM32_PLL2SRC == STM32_PLL2SRC_NOCLOCK
  #define STM32_PLL2IN              0

#else
  #error "invalid STM32_PLL2SRC value specified"
#endif

/**
 * @brief   PLL3 input clock frequency.
 */
#if (STM32_PLL3SRC == STM32_PLL3SRC_HSE) || defined(__DOXYGEN__)
  #define STM32_PLL3IN              STM32_HSECLK

#elif STM32_PLL3SRC == STM32_PLL3SRC_CSI
  #define STM32_PLL3IN              STM32_CSICLK

#elif STM32_PLL3SRC == STM32_PLL3SRC_HSI
  #define STM32_PLL3IN              STM32_HSICLK

#elif STM32_PLL3SRC == STM32_PLL3SRC_NOCLOCK
  #define STM32_PLL3IN              0

#else
  #error "invalid STM32_PLL3SRC value specified"
#endif

/**
 * @brief   STM32_PLL1PEN field.
 */
#if (STM32_SW           == STM32_SW_PLL1P)          ||                      \
    (STM32_MCO1SEL      == STM32_MCO1SEL_PLL1P)     ||                      \
    (STM32_MCO2SEL      == STM32_MCO2SEL_PLL1P)     ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL1PEN             (1U << 16)

#else
  #define STM32_PLL1PEN             (0U << 16)
#endif

/**
 * @brief   STM32_PLL1QEN field.
 */
#if (STM32_SPI1SEL      == STM32_SPI1SEL_PLL1Q)     ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_PLL1Q)     ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_PLL1Q)     ||                      \
    (STM32_OSPISEL      == STM32_OSPISEL_PLL1Q)     ||                      \
    (STM32_USBSEL       == STM32_USBSEL_PLL1Q)      ||                      \
    (STM32_SDMMC1SEL    == STM32_SDMMC1SEL_PLL1Q)   ||                      \
    (STM32_SDMMC2SEL    == STM32_SDMMC2SEL_PLL1Q)   ||                      \
    (STM32_RNGSEL       == STM32_RNGSEL_PLL1Q)      ||                      \
    (STM32_FDCANSEL     == STM32_FDCANSEL_PLL1Q)    ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_PLL1Q)     ||                      \
    (STM32_SAI2SEL      == STM32_SAI2SEL_PLL1Q)     ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL1QEN             (1U << 17)

#else
  #define STM32_PLL1QEN             (0U << 17)
#endif

/**
 * @brief   STM32_PLL1REN field.
 */
#if FALSE                                           ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL1REN             (1U << 18)

#else
  #define STM32_PLL1REN             (0U << 18)
#endif

/**
 * @brief   STM32_PLL2PEN field.
 */
#if (STM32_SW           == STM32_SW_PLL1P)          ||                      \
    (STM32_MCO2SEL      == STM32_MCO2SEL_PLL1P)     ||                      \
    (STM32_LPTIM1SEL    == STM32_LPTIM1SEL_PLL2P)   ||                      \
    (STM32_LPTIM2SEL    == STM32_LPTIM2SEL_PLL2P)   ||                      \
    (STM32_LPTIM3SEL    == STM32_LPTIM3SEL_PLL2P)   ||                      \
    (STM32_LPTIM4SEL    == STM32_LPTIM4SEL_PLL2P)   ||                      \
    (STM32_LPTIM5SEL    == STM32_LPTIM5SEL_PLL2P)   ||                      \
    (STM32_LPTIM6SEL    == STM32_LPTIM6SEL_PLL2P)   ||                      \
    (STM32_SPI1SEL      == STM32_SPI1SEL_PLL2P)     ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_PLL2P)     ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_PLL2P)     ||                      \
    (STM32_SPI4SEL      == STM32_SPI4SEL_PLL2P)     ||                      \
    (STM32_SPI5SEL      == STM32_SPI5SEL_PLL2P)     ||                      \
    (STM32_SPI6SEL      == STM32_SPI6SEL_PLL2P)     ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_PLL2P)     ||                      \
    (STM32_SAI2SEL      == STM32_SAI2SEL_PLL2P)     ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL2PEN             (1U << 16)

#else
  #define STM32_PLL2PEN             (0U << 16)
#endif

/**
 * @brief   STM32_PLL2QEN field.
 */
#if (STM32_USART1SEL    == STM32_USART1SEL_PLL2Q)   ||                      \
    (STM32_USART2SEL    == STM32_USART2SEL_PLL2Q)   ||                      \
    (STM32_USART3SEL    == STM32_USART3SEL_PLL2Q)   ||                      \
    (STM32_UART4SEL     == STM32_UART4SEL_PLL2Q)    ||                      \
    (STM32_UART5SEL     == STM32_UART5SEL_PLL2Q)    ||                      \
    (STM32_USART6SEL    == STM32_USART6SEL_PLL2Q)   ||                      \
    (STM32_UART7SEL     == STM32_UART7SEL_PLL2Q)    ||                      \
    (STM32_UART8SEL     == STM32_UART8SEL_PLL2Q)    ||                      \
    (STM32_UART9SEL     == STM32_UART9SEL_PLL2Q)    ||                      \
    (STM32_USART10SEL   == STM32_USART10SEL_PLL2Q)  ||                      \
    (STM32_USART11SEL   == STM32_USART11SEL_PLL2Q)  ||                      \
    (STM32_UART12SEL    == STM32_UART12SEL_PLL2Q)   ||                      \
    (STM32_LPUART1SEL   == STM32_LPUART1SEL_PLL2Q)  ||                      \
    (STM32_FDCANSEL     == STM32_FDCANSEL_PLL2Q)    ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL2QEN             (1U << 17)

#else
  #define STM32_PLL2QEN             (0U << 17)
#endif

/**
 * @brief   STM32_PLL2REN field.
 */
#if (STM32_OSPISEL      == STM32_OSPISEL_PLL2R)     ||                      \
    (STM32_SDMMC1SEL    == STM32_SDMMC1SEL_PLL2R)   ||                      \
    (STM32_SDMMC2SEL    == STM32_SDMMC2SEL_PLL2R)   ||                      \
    (STM32_ADCDACSEL    == STM32_ADCDACSEL_PLL2R)   ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL2REN             (1U << 18)

#else
  #define STM32_PLL2REN             (0U << 18)
#endif

/**
 * @brief   STM32_PLL3PEN field.
 */
#if (STM32_SPI1SEL      == STM32_SPI1SEL_PLL3P)     ||                      \
    (STM32_SPI2SEL      == STM32_SPI2SEL_PLL3P)     ||                      \
    (STM32_SPI3SEL      == STM32_SPI3SEL_PLL3P)     ||                      \
    (STM32_SPI4SEL      == STM32_SPI4SEL_PLL3P)     ||                      \
    (STM32_SPI5SEL      == STM32_SPI5SEL_PLL3P)     ||                      \
    (STM32_SPI6SEL      == STM32_SPI6SEL_PLL3P)     ||                      \
    (STM32_SAI1SEL      == STM32_SAI1SEL_PLL3P)     ||                      \
    (STM32_SAI2SEL      == STM32_SAI2SEL_PLL3P)     ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL3PEN             (1U << 16)

#else
  #define STM32_PLL3PEN             (0U << 16)
#endif

/**
 * @brief   STM32_PLL3QEN field.
 */
#if (STM32_USART1SEL    == STM32_USART1SEL_PLL3Q)   ||                      \
    (STM32_USART2SEL    == STM32_USART2SEL_PLL3Q)   ||                      \
    (STM32_USART3SEL    == STM32_USART3SEL_PLL3Q)   ||                      \
    (STM32_UART4SEL     == STM32_UART4SEL_PLL3Q)    ||                      \
    (STM32_UART5SEL     == STM32_UART5SEL_PLL3Q)    ||                      \
    (STM32_USART6SEL    == STM32_USART6SEL_PLL3Q)   ||                      \
    (STM32_UART7SEL     == STM32_UART7SEL_PLL3Q)    ||                      \
    (STM32_UART8SEL     == STM32_UART8SEL_PLL3Q)    ||                      \
    (STM32_UART9SEL     == STM32_UART9SEL_PLL3Q)    ||                      \
    (STM32_USART10SEL   == STM32_USART10SEL_PLL3Q)  ||                      \
    (STM32_USART11SEL   == STM32_USART11SEL_PLL3Q)  ||                      \
    (STM32_UART12SEL    == STM32_UART12SEL_PLL3Q)   ||                      \
    (STM32_LPUART1SEL   == STM32_LPUART1SEL_PLL3Q)  ||                      \
    (STM32_USBSEL       == STM32_USBSEL_PLL3Q)  ||                          \
    defined(__DOXYGEN__)
  #define STM32_PLL3QEN             (1U << 17)

#else
  #define STM32_PLL3QEN             (0U << 17)
#endif

/**
 * @brief   STM32_PLL3REN field.
 */
#if (STM32_LPTIM1SEL    == STM32_LPTIM1SEL_PLL3R)   ||                      \
    (STM32_LPTIM2SEL    == STM32_LPTIM2SEL_PLL3R)   ||                      \
    (STM32_LPTIM3SEL    == STM32_LPTIM3SEL_PLL3R)   ||                      \
    (STM32_LPTIM4SEL    == STM32_LPTIM4SEL_PLL3R)   ||                      \
    (STM32_LPTIM5SEL    == STM32_LPTIM5SEL_PLL3R)   ||                      \
    (STM32_LPTIM6SEL    == STM32_LPTIM6SEL_PLL3R)   ||                      \
    (STM32_I2C1SEL      == STM32_I2C1SEL_PLL3R)     ||                      \
    (STM32_I2C2SEL      == STM32_I2C2SEL_PLL3R)     ||                      \
    (STM32_I2C3SEL      == STM32_I2C3SEL_PLL3R)     ||                      \
    (STM32_I2C4SEL      == STM32_I2C4SEL_PLL3R)     ||                      \
    (STM32_I3C1SEL      == STM32_I3C1SEL_PLL3R)     ||                      \
    defined(__DOXYGEN__)
  #define STM32_PLL3REN             (1U << 18)

#else
  #define STM32_PLL3REN             (0U << 18)
#endif

/* Inclusion of PLL-related checks and calculations.*/
#include <stm32_pll1.inc>
#include <stm32_pll2.inc>
#include <stm32_pll3.inc>

/**
 * @brief   System clock source.
 */
#if (STM32_SW == STM32_SW_HSI) || defined(__DOXYGEN__)
  #define STM32_SYSCLK              STM32_HSICLK

#elif STM32_SW == STM32_SW_CSI
  #define STM32_SYSCLK              STM32_CSICLK

#elif STM32_SW == STM32_SW_HSE
  #define STM32_SYSCLK              STM32_HSECLK

#elif STM32_SW == STM32_SW_PLL1P
  #define STM32_SYSCLK              STM32_PLL1_P_CLKOUT

#else
  #error "invalid STM32_SW value specified"
#endif

/* Bus handlers.*/
#include "stm32_ahb.inc"
#include "stm32_apb1.inc"
#include "stm32_apb2.inc"
#include "stm32_apb3.inc"

/**
 * @brief   STOPWUCK source clock.
 */
#if (STM32_STOPWUCK == STM32_STOPWUCK_HSI) || defined(__DOXYGEN__)

#elif STM32_STOPWUCK == STM32_STOPWUCK_CSI

#else
  #error "invalid STM32_STOPWUCK value specified"
#endif

/**
 * @brief   STOPKERWUCK source clock.
 */
#if (STM32_STOPKERWUCK == STM32_STOPKERWUCK_HSI) || defined(__DOXYGEN__)

#elif STM32_STOPKERWUCK == STM32_STOPKERWUCK_CSI

#else
  #error "invalid STM32_STOPKERWUCK value specified"
#endif

/**
 * @brief   RTCPRE clock frequency.
 */
#if (STM32_RTCPRE_VALUE == STM32_RTCPRE_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_RTCPRECLK           0U

#elif (STM32_RTCPRE_VALUE >= 2) && (STM32_RTCPRE_VALUE <= 63)
  #define STM32_RTCPRECLK           (STM32_HSECLK / STM32_RTCPRE_VALUE)

#else
  #error "invalid STM32_RTCPRECLK_VALUE value specified"
#endif

/**
 * @brief   RTCPRE field.
 */
#define STM32_RTCPRE                STM32_RTCPRE_FIELD(STM32_RTCPRE_VALUE)

/**
 * @brief   MCO1 source clock.
 */
#if (STM32_MCO1SEL == STM32_MCO1SEL_HSI) || defined(__DOXYGEN__)
  #define STM32_MCO1DIVCLK          STM32_HSICLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_LSE
  #define STM32_MCO1DIVCLK          STM32_LSECLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSI
  #define STM32_MCO1DIVCLK          STM32_HSICLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSE
  #define STM32_MCO1DIVCLK          STM32_HSECLK

#elif STM32_MCO1SEL == STM32_MCO1SEL_PLL1P
  #define STM32_MCO1DIVCLK          STM32_PLL1_P_CLKOUT

#elif STM32_MCO1SEL == STM32_MCO1SEL_HSI48
  #define STM32_MCO1DIVCLK          STM32_HSI48CLK

#else
  #error "invalid STM32_MCO1SEL value specified"
#endif

/**
 * @brief   MCO1 output pin clock frequency.
 */
#if (STM32_MCO1PRE_VALUE == STM32_MCO1PRE_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_MCO1CLK             0U

#elif (STM32_MCO1PRE_VALUE > STM32_MCO1PRE_NOCLOCK) &&                      \
      (STM32_MCO1PRE_VALUE <= 15)
  #define STM32_MCO1CLK             (STM32_MCO1DIVCLK / STM32_MCO1PRE_VALUE)

#else
#error "invalid STM32_MCO1PRE_VALUE value specified"
#endif

/**
 * @brief   MCO1PRE field.
 */
#define STM32_MCO1PRE               STM32_MCO1PRE_FIELD(STM32_MCO1PRE_VALUE)

/**
 * @brief   MCO2 source clock.
 */
#if (STM32_MCO2SEL == STM32_MCO2SEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_MCO2DIVCLK          STM32_SYSCLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_PLL2P
  #define STM32_MCO2DIVCLK          STM32_PLL2_P_CLKOUT

#elif STM32_MCO2SEL == STM32_MCO2SEL_HSE
  #define STM32_MCO2DIVCLK          STM32_HSECLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_PLL1P
  #define STM32_MCO2DIVCLK          STM32_PLL1_P_CLKOUT

#elif STM32_MCO2SEL == STM32_MCO2SEL_CSI
  #define STM32_MCO2DIVCLK          STM32_CSICLK

#elif STM32_MCO2SEL == STM32_MCO2SEL_LSI
  #define STM32_MCO2DIVCLK          STM32_LSICLK

#else
  #error "invalid STM32_MCO2SEL value specified"
#endif

/**
 * @brief   MCO2 output pin clock frequency.
 */
#if (STM32_MCO2PRE_VALUE == STM32_MCO2PRE_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_MCO2CLK             0U

#elif (STM32_MCO2PRE_VALUE > STM32_MCO2PRE_NOCLOCK) &&                      \
      (STM32_MCO2PRE_VALUE <= 15)
  #define STM32_MCO2CLK             (STM32_MCO2DIVCLK / STM32_MCO2PRE_VALUE)

#else
#error "invalid STM32_MCO2PRE_VALUE value specified"
#endif

/**
 * @brief   MCO2PRE field.
 */
#define STM32_MCO2PRE               STM32_MCO2PRE_FIELD(STM32_MCO2PRE_VALUE)

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
  #define STM32_RTCCLK              STM32_RTCPRECLK

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

#elif STM32_USART1SEL == STM32_USART1SEL_PLL2Q
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_USART1SEL == STM32_USART1SEL_PLL3Q
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USART1SEL == STM32_USART1SEL_HSI
  #define STM32_USART1CLK           STM32_HSICLK

#elif STM32_USART1SEL == STM32_USART1SEL_CSI
  #define STM32_USART1CLK           STM32_CSICLK

#elif STM32_USART1SEL == STM32_USART1SEL_LSE
  #define STM32_USART1CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART1 clock"
#endif

/**
 * @brief   USART2 clock frequency.
 */
#if (STM32_USART2SEL == STM32_USART2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART2CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_USART2SEL == STM32_USART2SEL_PLL2Q
  #define STM32_USART2CLK           hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_USART2SEL == STM32_USART2SEL_PLL3Q
  #define STM32_USART2CLK           hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USART2SEL == STM32_USART2SEL_HSI
  #define STM32_USART2CLK           STM32_HSICLK

#elif STM32_USART2SEL == STM32_USART2SEL_CSI
  #define STM32_USART2CLK           STM32_CSICLK

#elif STM32_USART2SEL == STM32_USART2SEL_LSE
  #define STM32_USART2CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART2 clock"
#endif

/**
 * @brief   USART3 clock frequency.
 */
#if (STM32_USART3SEL == STM32_USART3SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_USART3SEL == STM32_USART3SEL_PLL2Q
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_USART3SEL == STM32_USART3SEL_PLL3Q
  #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USART3SEL == STM32_USART3SEL_HSI
  #define STM32_USART3CLK           STM32_HSICLK

#elif STM32_USART3SEL == STM32_USART3SEL_CSI
  #define STM32_USART3CLK           STM32_CSICLK

#elif STM32_USART3SEL == STM32_USART3SEL_LSE
  #define STM32_USART3CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART3 clock"
#endif

/**
 * @brief   UART4 clock frequency.
 */
#if (STM32_UART4SEL == STM32_UART4SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART4CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART4SEL == STM32_UART4SEL_PLL2Q
  #define STM32_UART4CLK            hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_UART4SEL == STM32_UART4SEL_PLL3Q
  #define STM32_UART4CLK            hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_UART4SEL == STM32_UART4SEL_HSI
  #define STM32_UART4CLK            STM32_HSICLK

#elif STM32_UART4SEL == STM32_UART4SEL_CSI
  #define STM32_UART4CLK            STM32_CSICLK

#elif STM32_UART4SEL == STM32_UART4SEL_LSE
  #define STM32_UART4CLK            STM32_LSECLK

#else
  #error "invalid source selected for UART4 clock"
#endif

/**
 * @brief   UART5 clock frequency.
 */
#if (STM32_UART5SEL == STM32_UART5SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART5CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART5SEL == STM32_UART5SEL_PLL2Q
  #define STM32_UART5CLK            hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_UART5SEL == STM32_UART5SEL_PLL3Q
  #define STM32_UART5CLK            hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_UART5SEL == STM32_UART5SEL_HSI
  #define STM32_UART5CLK            STM32_HSICLK

#elif STM32_UART5SEL == STM32_UART5SEL_CSI
  #define STM32_UART5CLK            STM32_CSICLK

#elif STM32_UART5SEL == STM32_UART5SEL_LSE
  #define STM32_UART5CLK            STM32_LSECLK

#else
  #error "invalid source selected for UART5 clock"
#endif

/**
 * @brief   USART6 clock frequency.
 */
#if (STM32_USART6SEL == STM32_USART6SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART6CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_USART6SEL == STM32_USART6SEL_PLL2Q
  #define STM32_USART6CLK           hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_USART6SEL == STM32_USART6SEL_PLL3Q
  #define STM32_USART6CLK           hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USART6SEL == STM32_USART6SEL_HSI
  #define STM32_USART6CLK           STM32_HSICLK

#elif STM32_USART6SEL == STM32_USART6SEL_CSI
  #define STM32_USART6CLK           STM32_CSICLK

#elif STM32_USART6SEL == STM32_USART6SEL_LSE
  #define STM32_USART6CLK           STM32_LSECLK

#else
  #error "invalid source selected for USART6 clock"
#endif

/**
 * @brief   UART7 clock frequency.
 */
#if (STM32_UART7SEL == STM32_UART7SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART7CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART7SEL == STM32_UART7SEL_PLL2Q
  #define STM32_UART7CLK            hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_UART7SEL == STM32_UART7SEL_PLL3Q
  #define STM32_UART7CLK            hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_UART7SEL == STM32_UART7SEL_HSI
  #define STM32_UART7CLK            STM32_HSICLK

#elif STM32_UART7SEL == STM32_UART7SEL_CSI
  #define STM32_UART7CLK            STM32_CSICLK

#elif STM32_UART7SEL == STM32_UART7SEL_LSE
  #define STM32_UART7CLK            STM32_LSECLK

#else
  #error "invalid source selected for UART7 clock"
#endif

/**
 * @brief   UART8 clock frequency.
 */
#if (STM32_UART8SEL == STM32_UART8SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART8CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART8SEL == STM32_UART8SEL_PLL2Q
  #define STM32_UART8CLK            hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_UART8SEL == STM32_UART8SEL_PLL3Q
  #define STM32_UART8CLK            hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_UART8SEL == STM32_UART8SEL_HSI
  #define STM32_UART8CLK            STM32_HSICLK

#elif STM32_UART8SEL == STM32_UART8SEL_CSI
  #define STM32_UART8CLK            STM32_CSICLK

#elif STM32_UART8SEL == STM32_UART8SEL_LSE
  #define STM32_UART8CLK            STM32_LSECLK

#else
  #error "invalid source selected for UART8 clock"
#endif

/**
 * @brief   UART9 clock frequency.
 */
#if (STM32_UART9SEL == STM32_UART9SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART9CLK            hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART9SEL == STM32_UART9SEL_PLL2Q
  #define STM32_UART9CLK            hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_UART9SEL == STM32_UART9SEL_PLL3Q
  #define STM32_UART9CLK            hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_UART9SEL == STM32_UART9SEL_HSI
  #define STM32_UART9CLK            STM32_HSICLK

#elif STM32_UART9SEL == STM32_UART9SEL_CSI
  #define STM32_UART9CLK            STM32_CSICLK

#elif STM32_UART9SEL == STM32_UART9SEL_LSE
  #define STM32_UART9CLK            STM32_LSECLK

#else
  #error "invalid source selected for UART9 clock"
#endif

/**
 * @brief   USART10 clock frequency.
 */
#if (STM32_USART10SEL == STM32_USART10SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART10CLK          hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_USART10SEL == STM32_USART10SEL_PLL2Q
  #define STM32_USART10CLK          hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_USART10SEL == STM32_USART10SEL_PLL3Q
  #define STM32_USART10CLK          hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USART10SEL == STM32_USART10SEL_HSI
  #define STM32_USART10CLK          STM32_HSICLK

#elif STM32_USART10SEL == STM32_USART10SEL_CSI
  #define STM32_USART10CLK          STM32_CSICLK

#elif STM32_USART10SEL == STM32_USART10SEL_LSE
  #define STM32_USART10CLK          STM32_LSECLK

#else
  #error "invalid source selected for USART10 clock"
#endif

/**
 * @brief   USART11 clock frequency.
 */
#if (STM32_USART11SEL == STM32_USART11SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_USART11CLK          hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_USART11SEL == STM32_USART11SEL_PLL2Q
  #define STM32_USART11CLK          hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_USART11SEL == STM32_USART11SEL_PLL3Q
  #define STM32_USART11CLK          hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USART11SEL == STM32_USART11SEL_HSI
  #define STM32_USART11CLK          STM32_HSICLK

#elif STM32_USART11SEL == STM32_USART11SEL_CSI
  #define STM32_USART11CLK          STM32_CSICLK

#elif STM32_USART11SEL == STM32_USART11SEL_LSE
  #define STM32_USART11CLK          STM32_LSECLK

#else
  #error "invalid source selected for USART11 clock"
#endif

/**
 * @brief   UART12 clock frequency.
 */
#if (STM32_UART12SEL == STM32_UART12SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART12CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART12SEL == STM32_UART12SEL_PLL2Q
  #define STM32_UART12CLK           hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_UART12SEL == STM32_UART12SEL_PLL3Q
  #define STM32_UART12CLK           hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_UART12SEL == STM32_UART12SEL_HSI
  #define STM32_UART12CLK           STM32_HSICLK

#elif STM32_UART12SEL == STM32_UART12SEL_CSI
  #define STM32_UART12CLK           STM32_CSICLK

#elif STM32_UART12SEL == STM32_UART12SEL_LSE
  #define STM32_UART12CLK           STM32_LSECLK

#else
  #error "invalid source selected for UART12 clock"
#endif

/**
 * @brief   LPUART1 clock frequency.
 */
#if (STM32_LPUART1SEL == STM32_LPUART1SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_PLL2Q
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_PLL3Q
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_HSI
  #define STM32_LPUART1CLK          STM32_HSICLK

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_CSI
  #define STM32_LPUART1CLK          STM32_CSICLK

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_LSE
  #define STM32_LPUART1CLK          STM32_LSECLK

#else
  #error "invalid source selected for LPUART1 clock"
#endif

#if (STM32_TIMICSEL == STM32_TIMICSEL_NOCLK) || defined(__DOXYGEN__)
  /**
   * @brief   TIM12 internal capture clock frequency.
   */
  #define STM32_TIM12CCLK           0
  /**
   * @brief   TIM15 internal capture clock frequency.
   */
  #define STM32_TIM15CCLK           0
  /**
   * @brief   LPTIM2 internal capture clock frequency.
   */
  #define STM32_LPTIM2CCLK          0

#elif STM32_TIMICSEL == STM32_TIMICSEL_INTCLK
  #define STM32_TIM12CCLK           (STM32_HSICLK / 1024)
  #define STM32_TIM15CCLK           (STM32_HSICLK / 8)
  #define STM32_LPTIM2CCLK          (STM32_CSICLK / 128)

#else
  #error "invalid source selected for TIMICSEL clock"
#endif

/**
 * @brief   LPTIM1 clock frequency.
 */
#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPTIM1CLK           hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_PLL2P
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_PLL3R
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE
  #define STM32_LPTIM1CLK           STM32_LSECLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI
  #define STM32_LPTIM1CLK           STM32_LSICLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_PER
  #define STM32_LPTIM1CLK           STM32_PERCLK

#else
  #error "invalid source selected for LPTIM1 clock"
#endif

/**
 * @brief   LPTIM2 clock frequency.
 */
#if (STM32_LPTIM2SEL == STM32_LPTIM2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_LPTIM2CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_PLL2P
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_PLL3R
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSE
  #define STM32_LPTIM2CLK           STM32_LSECLK

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_LSI
  #define STM32_LPTIM2CLK           STM32_LSICLK

#elif STM32_LPTIM2SEL == STM32_LPTIM2SEL_PER
  #define STM32_LPTIM2CLK           STM32_PERCLK

#else
  #error "invalid source selected for LPTIM2 clock"
#endif

/**
 * @brief   LPTIM3 clock frequency.
 */
#if (STM32_LPTIM3SEL == STM32_LPTIM3SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPTIM3CLK           hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPTIM3SEL == STM32_LPTIM3SEL_PLL2P
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_LPTIM3SEL == STM32_LPTIM3SEL_PLL3R
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_LPTIM3SEL == STM32_LPTIM3SEL_LSE
  #define STM32_LPTIM3CLK           STM32_LSECLK

#elif STM32_LPTIM3SEL == STM32_LPTIM3SEL_LSI
  #define STM32_LPTIM3CLK           STM32_LSICLK

#elif STM32_LPTIM3SEL == STM32_LPTIM3SEL_PER
  #define STM32_LPTIM3CLK           STM32_PERCLK

#else
  #error "invalid source selected for LPTIM3 clock"
#endif

/**
 * @brief   LPTIM4 clock frequency.
 */
#if (STM32_LPTIM4SEL == STM32_LPTIM4SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPTIM4CLK           hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPTIM4SEL == STM32_LPTIM4SEL_PLL2P
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_LPTIM4SEL == STM32_LPTIM4SEL_PLL3R
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_LPTIM4SEL == STM32_LPTIM4SEL_LSE
  #define STM32_LPTIM4CLK           STM32_LSECLK

#elif STM32_LPTIM4SEL == STM32_LPTIM4SEL_LSI
  #define STM32_LPTIM4CLK           STM32_LSICLK

#elif STM32_LPTIM4SEL == STM32_LPTIM4SEL_PER
  #define STM32_LPTIM4CLK           STM32_PERCLK

#else
  #error "invalid source selected for LPTIM4 clock"
#endif

/**
 * @brief   LPTIM5 clock frequency.
 */
#if (STM32_LPTIM5SEL == STM32_LPTIM5SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPTIM5CLK           hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPTIM5SEL == STM32_LPTIM5SEL_PLL2P
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_LPTIM5SEL == STM32_LPTIM5SEL_PLL3R
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_LPTIM5SEL == STM32_LPTIM5SEL_LSE
  #define STM32_LPTIM5CLK           STM32_LSECLK

#elif STM32_LPTIM5SEL == STM32_LPTIM5SEL_LSI
  #define STM32_LPTIM5CLK           STM32_LSICLK

#elif STM32_LPTIM5SEL == STM32_LPTIM5SEL_PER
  #define STM32_LPTIM5CLK           STM32_PERCLK

#else
  #error "invalid source selected for LPTIM5 clock"
#endif

/**
 * @brief   LPTIM6 clock frequency.
 */
#if (STM32_LPTIM6SEL == STM32_LPTIM6SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_LPTIM6CLK           hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_LPTIM6SEL == STM32_LPTIM6SEL_PLL2P
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_LPTIM6SEL == STM32_LPTIM6SEL_PLL3R
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_LPTIM6SEL == STM32_LPTIM6SEL_LSE
  #define STM32_LPTIM6CLK           STM32_LSECLK

#elif STM32_LPTIM6SEL == STM32_LPTIM6SEL_LSI
  #define STM32_LPTIM6CLK           STM32_LSICLK

#elif STM32_LPTIM6SEL == STM32_LPTIM6SEL_PER
  #define STM32_LPTIM6CLK           STM32_PERCLK

#else
  #error "invalid source selected for LPTIM6 clock"
#endif

/**
 * @brief   SPI1 clock frequency.
 */
#if (STM32_SPI1SEL == STM32_SPI1SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SPI1CLK             hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SPI1SEL == STM32_SPI1SEL_PLL2P
  #define STM32_SPI1CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SPI1SEL == STM32_SPI1SEL_PLL3P
  #define STM32_SPI1CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SPI1SEL == STM32_SPI1SEL_AUDIOCLK
  #define STM32_SPI1CLK             0 /* TODO board.h */

#elif STM32_SPI1SEL == STM32_SPI2SEL_PER
  #define STM32_SPI1CLK             STM32_PERCLK

#else
  #error "invalid source selected for SPI1 clock"
#endif

/**
 * @brief   SPI2 clock frequency.
 */
#if (STM32_SPI2SEL == STM32_SPI2SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SPI2CLK             hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SPI2SEL == STM32_SPI2SEL_PLL2P
  #define STM32_SPI2CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SPI2SEL == STM32_SPI2SEL_PLL3P
  #define STM32_SPI2CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SPI2SEL == STM32_SPI2SEL_AUDIOCLK
  #define STM32_SPI2CLK             0 /* TODO board.h */

#elif STM32_SPI2SEL == STM32_SPI2SEL_PER
  #define STM32_SPI2CLK             STM32_PERCLK

#else
  #error "invalid source selected for SPI2 clock"
#endif

/**
 * @brief   SPI3 clock frequency.
 */
#if (STM32_SPI3SEL == STM32_SPI3SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SPI3CLK             hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SPI3SEL == STM32_SPI3SEL_PLL2P
  #define STM32_SPI3CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SPI3SEL == STM32_SPI3SEL_PLL3P
  #define STM32_SPI3CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SPI3SEL == STM32_SPI3SEL_AUDIOCLK
  #define STM32_SPI3CLK             0 /* TODO board.h */

#elif STM32_SPI3SEL == STM32_SPI2SEL_PER
  #define STM32_SPI3CLK             STM32_PERCLK

#else
  #error "invalid source selected for SPI3 clock"
#endif

/**
 * @brief   SPI4 clock frequency.
 */
#if (STM32_SPI4SEL == STM32_SPI4SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_SPI4CLK             hal_lld_get_clock_point(CLK_PCLK2)

#elif STM32_SPI4SEL == STM32_SPI4SEL_PLL2P
  #define STM32_SPI4CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SPI4SEL == STM32_SPI4SEL_PLL3P
  #define STM32_SPI4CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SPI4SEL == STM32_SPI4SEL_HSI
  #define STM32_SPI4CLK             STM32_HSICLK

#elif STM32_SPI4SEL == STM32_SPI4SEL_CSI
  #define STM32_SPI4CLK             STM32_CSICLK

#elif STM32_SPI4SEL == STM32_SPI4SEL_HSE
  #define STM32_SPI4CLK             STM32_HSECLK

#else
  #error "invalid source selected for SPI4 clock"
#endif

/**
 * @brief   SPI5 clock frequency.
 */
#if (STM32_SPI5SEL == STM32_SPI5SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_SPI5CLK             hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_SPI5SEL == STM32_SPI5SEL_PLL2P
  #define STM32_SPI5CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SPI5SEL == STM32_SPI5SEL_PLL3P
  #define STM32_SPI5CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SPI5SEL == STM32_SPI5SEL_HSI
  #define STM32_SPI5CLK             STM32_HSICLK

#elif STM32_SPI5SEL == STM32_SPI5SEL_CSI
  #define STM32_SPI5CLK             STM32_CSICLK

#elif STM32_SPI5SEL == STM32_SPI5SEL_HSE
  #define STM32_SPI5CLK             STM32_HSECLK

#else
  #error "invalid source selected for SPI5 clock"
#endif

/**
 * @brief   SPI6 clock frequency.
 */
#if (STM32_SPI6SEL == STM32_SPI6SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_SPI6CLK             hal_lld_get_clock_point(CLK_PCLK2)

#elif STM32_SPI6SEL == STM32_SPI6SEL_PLL2P
  #define STM32_SPI6CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SPI6SEL == STM32_SPI6SEL_PLL3P
  #define STM32_SPI6CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SPI6SEL == STM32_SPI6SEL_HSI
  #define STM32_SPI6CLK             STM32_HSICLK

#elif STM32_SPI6SEL == STM32_SPI6SEL_CSI
  #define STM32_SPI6CLK             STM32_CSICLK

#elif STM32_SPI6SEL == STM32_SPI6SEL_HSE
  #define STM32_SPI6CLK             STM32_HSECLK

#else
  #error "invalid source selected for SPI6 clock"
#endif

/**
 * @brief   OSPI clock frequency.
 */
#if (STM32_OSPISEL == STM32_OSPISEL_HCLK4) || defined(__DOXYGEN__)
  #define STM32_OSPICLK             hal_lld_get_clock_point(CLK_HCLK)

#elif STM32_OSPISEL == STM32_OSPISEL_PLL1Q
  #define STM32_OSPICLK             hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_OSPISEL == STM32_OSPISEL_PLL2R
  #define STM32_OSPICLK             hal_lld_get_clock_point(CLK_PLL2RCLK)

#elif STM32_OSPISEL == STM32_OSPISEL_PER
  #define STM32_OSPICLK             STM32_PERCLK

#else
  #error "invalid source selected for OSPI clock"
#endif

/**
 * @brief   SYSTICK clock frequency.
 */
#if (STM32_SYSTICKSEL == STM32_SYSTICKSEL_HCLKDIV8) || defined(__DOXYGEN__)
  #define STM32_SYSTICKCLK          (hal_lld_get_clock_point(CLK_HCLK) / 8)

#elif STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSI
  #define STM32_SYSTICKCLK          STM32_LSICLK

#elif STM32_SYSTICKSEL == STM32_SYSTICKSEL_LSE
  #define STM32_SYSTICKCLK          STM32_LSECLK

#elif STM32_SYSTICKSEL == STM32_SYSTICKSEL_NOCLOCK
  #define STM32_SYSTICKCLK          0

#else
  #error "invalid source selected for STM32_SYSTICKSEL clock"
#endif

/**
 * @brief   USB clock frequency.
 */
#if (STM32_USBSEL == STM32_USBSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_USBCLK              0

#elif STM32_USBSEL == STM32_USBSEL_PLL1Q
  #define STM32_USBCLK              hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_USBSEL == STM32_USBSEL_PLL3Q
  #define STM32_USBCLK              hal_lld_get_clock_point(CLK_PLL3QCLK)

#elif STM32_USBSEL == STM32_USBSEL_HSI48
  #define STM32_USBCLK              STM32_HSI48CLK

#else
  #error "invalid source selected for USB clock"
#endif

/**
 * @brief   SDMMC1 clock frequency.
 */
#if (STM32_SDMMC1SEL == STM32_SDMMC1SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SDMMC1CLK           hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SDMMC1SEL == STM32_SDMMC1SEL_PLL2R
  #define STM32_SDMMC1CLK           hal_lld_get_clock_point(CLK_PLL2RCLK)

#else
  #error "invalid source selected for SDMMC1 clock"
#endif

/**
 * @brief   SDMMC2 clock frequency.
 */
#if (STM32_SDMMC2SEL == STM32_SDMMC2SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SDMMC2CLK           hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SDMMC2SEL == STM32_SDMMC2SEL_PLL2R
  #define STM32_SDMMC2CLK           hal_lld_get_clock_point(CLK_PLL2RCLK)

#else
  #error "invalid source selected for SDMMC2 clock"
#endif

/**
 * @brief   I2C1 clock frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C1SEL == STM32_I2C1SEL_PLL3R
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_I2C1SEL == STM32_I2C1SEL_HSI
  #define STM32_I2C1CLK             STM32_HSICLK

#elif STM32_I2C1SEL == STM32_I2C1SEL_CSI
  #define STM32_I2C1CLK             STM32_CSICLK

#else
  #error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C2 clock frequency.
 */
#if (STM32_I2C2SEL == STM32_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C2CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C2SEL == STM32_I2C2SEL_PLL3R
  #define STM32_I2C2CLK             hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_I2C2SEL == STM32_I2C2SEL_HSI
  #define STM32_I2C2CLK             STM32_HSICLK

#elif STM32_I2C2SEL == STM32_I2C2SEL_CSI
  #define STM32_I2C2CLK             STM32_CSICLK

#else
  #error "invalid source selected for I2C2 clock"
#endif

/**
 * @brief   I2C3 clock frequency.
 */
#if (STM32_I2C3SEL == STM32_I2C3SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_I2C3CLK             hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_I2C3SEL == STM32_I2C3SEL_PLL3R
  #define STM32_I2C3CLK             hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_I2C3SEL == STM32_I2C3SEL_HSI
  #define STM32_I2C3CLK             STM32_HSICLK

#elif STM32_I2C3SEL == STM32_I2C3SEL_CSI
  #define STM32_I2C3CLK             STM32_CSICLK

#else
  #error "invalid source selected for I2C3 clock"
#endif

/**
 * @brief   I2C4 clock frequency.
 */
#if (STM32_I2C4SEL == STM32_I2C4SEL_PCLK3) || defined(__DOXYGEN__)
  #define STM32_I2C4CLK             hal_lld_get_clock_point(CLK_PCLK3)

#elif STM32_I2C4SEL == STM32_I2C4SEL_PLL3R
  #define STM32_I2C4CLK             hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_I2C4SEL == STM32_I2C4SEL_HSI
  #define STM32_I2C4CLK             STM32_HSICLK

#elif STM32_I2C4SEL == STM32_I2C4SEL_CSI
  #define STM32_I2C4CLK             STM32_CSICLK

#else
  #error "invalid source selected for I2C4 clock"
#endif

/**
 * @brief   I3C1 clock frequency.
 */
#if (STM32_I3C1SEL == STM32_I3C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I3C1CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I3C1SEL == STM32_I3C1SEL_PLL3R
  #define STM32_I3C1CLK             hal_lld_get_clock_point(CLK_PLL3RCLK)

#elif STM32_I3C1SEL == STM32_I3C1SEL_HSI
  #define STM32_I3C1CLK             STM32_HSICLK

#else
  #error "invalid source selected for I3C1 clock"
#endif

/**
 * @brief   ADCDACSEL clock frequency.
 */
#if (STM32_ADCDACSEL == STM32_ADCDACSEL_HCLK) || defined(__DOXYGEN__)
  #define STM32_ADCDACCLK           hal_lld_get_clock_point(CLK_HCLK)

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_SYSCLK
  #define STM32_ADCDACCLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_PLL2R
  #define STM32_ADCDACCLK           hal_lld_get_clock_point(CLK_PLL2RCLK)

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_HSE
  #define STM32_ADCDACCLK           STM32_HSECLK

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_HSI
  #define STM32_ADCDACCLK           STM32_HSICLK

#elif STM32_ADCDACSEL == STM32_ADCDACSEL_CSI
  #define STM32_ADCDACCLK           STM32_CSICLK

#else
  #error "invalid source selected for ADCDACSEL clock"
#endif

/**
 * @brief   DACSEL clock frequency.
 */
#if (STM32_DACSEL == STM32_DACSEL_LSI) || defined(__DOXYGEN__)
  #define STM32_DACSELCLK           STM32_LSICLK

#elif STM32_DACSEL == STM32_DACSEL_LSE
  #define STM32_DACSELCLK           STM32_LSECLK

#elif STM32_DACSEL == STM32_DACSEL_IGNORE
  #define STM32_DACSELCLK           0

#else
  #error "invalid source selected for DACSEL clock"
#endif

/**
 * @brief   RNG clock frequency.
 */
#if (STM32_RNGSEL == STM32_RNGSEL_HSI48) || defined(__DOXYGEN__)
  #define STM32_RNGCLK              STM32_HSI48CLK

#elif STM32_RNGSEL == STM32_RNGSEL_PLL1Q
  #define STM32_RNGCLK              hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_RNGSEL == STM32_RNGSEL_LSE
  #define STM32_RNGCLK              STM32_LSECLK

#elif STM32_RNGSEL == STM32_RNGSEL_LSI
  #define STM32_RNGCLK              STM32_LSICLK

#elif STM32_RNGSEL == STM32_RNGSEL_IGNORE
  #define STM32_RNGCLK              0

#else
  #error "invalid source selected for RNG clock"
#endif

/**
 * @brief   CEC clock frequency.
 */
#if (STM32_CECSEL == STM32_CECSEL_LSE) || defined(__DOXYGEN__)
  #define STM32_CECCLK              STM32_LSECLK

#elif STM32_CECSEL == STM32_CECSEL_LSI
  #define STM32_CECCLK              STM32_LSICLK

#elif STM32_CECSEL == STM32_CECSEL_CSIDIV128
  #define STM32_CECCLK              (STM32_CSICLK / 128)

#elif STM32_CECSEL == STM32_CECSEL_IGNORE
  #define STM32_CECCLK              0

#else
  #error "invalid source selected for CEC clock"
#endif

/**
 * @brief   FDCAN clock frequency.
 */
#if (STM32_FDCANSEL == STM32_FDCANSEL_HSE) || defined(__DOXYGEN__)
  #define STM32_FDCANCLK            STM32_HSECLK

#elif STM32_FDCANSEL == STM32_FDCANSEL_PLL1Q
  #define STM32_FDCANCLK            hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_FDCANSEL == STM32_FDCANSEL_PLL2Q
  #define STM32_FDCANCLK            hal_lld_get_clock_point(CLK_PLL2QCLK)

#elif STM32_FDCANSEL == STM32_FDCANSEL_IGNORE
  #define STM32_FDCANCLK            0

#else
  #error "invalid source selected for FDCAN clock"
#endif

/**
 * @brief   SAI1 clock frequency.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SAI1CLK             hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SAI1SEL == STM32_SAI2SEL_PLL2P
  #define STM32_SAI1CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SAI1SEL == STM32_SAI1SEL_PLL3P
  #define STM32_SAI1CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SAI1SEL == STM32_SAI1SEL_AUDIOCLK
  #define STM32_SAI1CLK             0 /* TODO board.h */

#elif STM32_SAI1SEL == STM32_SAI1SEL_PER
  #define STM32_SAI1CLK             STM32_PERCLK

#else
  #error "invalid source selected for SAI1 clock"
#endif

/**
 * @brief   SAI2 clock frequency.
 */
#if (STM32_SAI2SEL == STM32_SAI2SEL_PLL1Q) || defined(__DOXYGEN__)
  #define STM32_SAI2CLK             hal_lld_get_clock_point(CLK_PLL1QCLK)

#elif STM32_SAI2SEL == STM32_SAI2SEL_PLL2P
  #define STM32_SAI2CLK             hal_lld_get_clock_point(CLK_PLL2PCLK)

#elif STM32_SAI2SEL == STM32_SAI2SEL_PLL3P
  #define STM32_SAI2CLK             hal_lld_get_clock_point(CLK_PLL3PCLK)

#elif STM32_SAI2SEL == STM32_SAI2SEL_AUDIOCLK
  #define STM32_SAI2CLK             0 /* TODO board.h */

#elif STM32_SAI2SEL == STM32_SAI2SEL_PER
  #define STM32_SAI2CLK             STM32_PERCLK

#else
  #error "invalid source selected for SAI2 clock"
#endif

#if (STM32_TIMPRE == STM32_TIMPRE_LOW) || defined(__DOXYGEN__)
  /**
   * @brief   TIMP1CLK clock frequency.
   */
  #if (STM32_PPRE1 == STM32_PPRE1_DIV1) ||                                  \
      (STM32_PPRE1 == STM32_PPRE1_DIV2) || defined(__DOXYGEN__)
    #define STM32_TIMP1CLK            STM32_HCLK
  #else
    #define STM32_TIMP1CLK            (STM32_PCLK1 * 2)
  #endif

  /**
   * @brief   TIMP2CLK clock frequency.
   */
  #if (STM32_PPRE2 == STM32_PPRE2_DIV1) ||                                  \
      (STM32_PPRE2 == STM32_PPRE2_DIV2) || defined(__DOXYGEN__)
    #define STM32_TIMP2CLK            STM32_HCLK
  #else
    #define STM32_TIMP2CLK            (STM32_PCLK2 * 2)
  #endif

#else
  #if (STM32_PPRE1 == STM32_PPRE1_DIV1) ||                                  \
      (STM32_PPRE1 == STM32_PPRE1_DIV2) ||                                  \
      (STM32_PPRE1 == STM32_PPRE1_DIV4) || defined(__DOXYGEN__)
    #define STM32_TIMP1CLK            (STM32_PCLK1 * 2)
  #else
    #define STM32_TIMP1CLK            (STM32_PCLK1 * 4)
  #endif

  #if (STM32_PPRE2 == STM32_PPRE2_DIV1) ||                                  \
      (STM32_PPRE2 == STM32_PPRE2_DIV2) ||                                  \
      (STM32_PPRE2 == STM32_PPRE2_DIV4) || defined(__DOXYGEN__)
    #define STM32_TIMP2CLK            (STM32_PCLK2 * 2)
  #else
    #define STM32_TIMP2CLK            (STM32_PCLK2 * 4)
  #endif
#endif

/**
 * @brief   ETH clock frequency.
 */
#define STM32_ETHCLK               hal_lld_get_clock_point(CLK_PLL1QCLK)

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
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_0WS | 0U)

#elif STM32_HCLK <= STM32_1WS_THRESHOLD
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_1WS | 0U)

#elif STM32_HCLK <= STM32_2WS_THRESHOLD
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_2WS | FLASH_ACR_WRHIGHFREQ_0)

#elif STM32_HCLK <= STM32_3WS_THRESHOLD
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_3WS | FLASH_ACR_WRHIGHFREQ_0)

#elif STM32_HCLK <= STM32_4WS_THRESHOLD
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_4WS | FLASH_ACR_WRHIGHFREQ_1)

#else
  #define STM32_FLASHBITS           (FLASH_ACR_LATENCY_5WS | FLASH_ACR_WRHIGHFREQ_1)
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
#if STM32_RCC_HAS_PLL3 || defined(__DOXYGEN__)
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_SYSCLK    ? STM32_SYSCLK          :                       \
   (clkpt) == CLK_PLL1PCLK  ? STM32_PLL1_P_CLKOUT   :                       \
   (clkpt) == CLK_PLL1QCLK  ? STM32_PLL1_Q_CLKOUT   :                       \
   (clkpt) == CLK_PLL1RCLK  ? STM32_PLL1_R_CLKOUT   :                       \
   (clkpt) == CLK_PLL2PCLK  ? STM32_PLL2_P_CLKOUT   :                       \
   (clkpt) == CLK_PLL2QCLK  ? STM32_PLL2_Q_CLKOUT   :                       \
   (clkpt) == CLK_PLL2RCLK  ? STM32_PLL2_R_CLKOUT   :                       \
   (clkpt) == CLK_PLL3PCLK  ? STM32_PLL3_P_CLKOUT   :                       \
   (clkpt) == CLK_PLL3QCLK  ? STM32_PLL3_Q_CLKOUT   :                       \
   (clkpt) == CLK_PLL3RCLK  ? STM32_PLL3_R_CLKOUT   :                       \
   (clkpt) == CLK_HCLK      ? STM32_HCLK            :                       \
   (clkpt) == CLK_PCLK1     ? STM32_PCLK1           :                       \
   (clkpt) == CLK_PCLK1TIM  ? STM32_TIMP1CLK        :                       \
   (clkpt) == CLK_PCLK2     ? STM32_PCLK2           :                       \
   (clkpt) == CLK_PCLK2TIM  ? STM32_TIMP2CLK        :                       \
   (clkpt) == CLK_PCLK3     ? STM32_PCLK3           :                       \
   (clkpt) == CLK_MCO1      ? STM32_MCO1CLK         :                       \
   (clkpt) == CLK_MCO2      ? STM32_MCO2CLK         :                       \
   (clkpt) == CLK_HSI48     ? STM32_HSI48CLK        :                       \
   0U)
#else
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_SYSCLK    ? STM32_SYSCLK          :                       \
   (clkpt) == CLK_PLL1PCLK  ? STM32_PLL1_P_CLKOUT   :                       \
   (clkpt) == CLK_PLL1QCLK  ? STM32_PLL1_Q_CLKOUT   :                       \
   (clkpt) == CLK_PLL1RCLK  ? STM32_PLL1_R_CLKOUT   :                       \
   (clkpt) == CLK_PLL2PCLK  ? STM32_PLL2_P_CLKOUT   :                       \
   (clkpt) == CLK_PLL2QCLK  ? STM32_PLL2_Q_CLKOUT   :                       \
   (clkpt) == CLK_PLL2RCLK  ? STM32_PLL2_R_CLKOUT   :                       \
   (clkpt) == CLK_HCLK      ? STM32_HCLK            :                       \
   (clkpt) == CLK_PCLK1     ? STM32_PCLK1           :                       \
   (clkpt) == CLK_PCLK1TIM  ? STM32_TIMP1CLK        :                       \
   (clkpt) == CLK_PCLK2     ? STM32_PCLK2           :                       \
   (clkpt) == CLK_PCLK2TIM  ? STM32_TIMP2CLK        :                       \
   (clkpt) == CLK_PCLK3     ? STM32_PCLK3           :                       \
   (clkpt) == CLK_MCO1      ? STM32_MCO1CLK         :                       \
   (clkpt) == CLK_MCO2      ? STM32_MCO2CLK         :                       \
   (clkpt) == CLK_HSI48     ? STM32_HSI48CLK        :                       \
   0U)
#endif
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
