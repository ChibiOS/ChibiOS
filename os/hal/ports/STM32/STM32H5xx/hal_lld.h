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
 *          .
 *          One of the following macros must also be defined:
 *          - STM32H503xx.
 *          - STM32H562xx, STM32H563xx, STM32H573xx.
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
#if defined(STM32H503xx) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "STM32H5 High-performance"

#elif defined(STM32H562xx) || defined(STM32H563xx)
#define PLATFORM_NAME           "STM32H5 High-performance"

#elif defined(STM32H573xx)
#define PLATFORM_NAME           "STM32H5 High-performance with security"

#else
#error "STM32H5 device not specified"
#endif

/**
 * @brief   Sub-family identifier.
 */
#if !defined(STM32H5XX) || defined(__DOXYGEN__)
#define STM32H5XX
#endif
/** @} */

/**
 * @name   Clock points names
 * @{
 */
#define CLK_SYSCLK              0U
#define CLK_PLL1PCLK            1U
#define CLK_PLL1QCLK            2U
#define CLK_PLL1RCLK            3U
#define CLK_PLL2PCLK            4U
#define CLK_PLL2QCLK            5U
#define CLK_PLL2RCLK            6U
#define CLK_PLL3PCLK            7U
#define CLK_PLL3QCLK            8U
#define CLK_PLL3RCLK            9U
#define CLK_HCLK                10U
#define CLK_PCLK1               11U
#define CLK_PCLK1TIM            12U
#define CLK_PCLK2               13U
#define CLK_PCLK2TIM            14U
#define CLK_MCO1                15U
#define CLK_MCO2                16U
#define CLK_LSCO                17U
#define CLK_ARRAY_SIZE          18U
/** @} */

/**
 * @name    VOS field definitions
 * @{
 */
#define STM32_VOS_MASK          (3U << 14)  /**< Core voltage mask.         */
#define STM32_VOS_RANGE3        (0U << 14)  /**< Core voltage 1 Volt.       */
#define STM32_VOS_RANGE2        (1U << 14)  /**< Core voltage 1.1 Volts.    */
#define STM32_VOS_RANGE1        (2U << 14)  /**< Core voltage 1.2 Volts.    */
#define STM32_VOS_RANGE0        (3U << 14)  /**< Core voltage 1.35 Volts.   */
/** @} */

/**
 * @name    RCC_CR register bits definitions
 * @{
 */
#define STM32_HSIDIV_MASK       (3U << 3)
#define STM32_HSIDIV_FIELD(n)   ((n) << 3)
#define STM32_HSIDIV_DIV1       STM32_HSIDIV_FIELD(0U)
#define STM32_HSIDIV_DIV2       STM32_HSIDIV_FIELD(1U)
#define STM32_HSIDIV_DIV4       STM32_HSIDIV_FIELD(2U)
#define STM32_HSIDIV_DIV8       STM32_HSIDIV_FIELD(3U)
/** @} */

/**
 * @name    RCC_CFGR1 register bits definitions
 * @{
 */
#define STM32_SW_MASK           (3U << 0)
#define STM32_SW_FIELD(n)       ((n) << 0)
#define STM32_SW_HSI            STM32_SW_FIELD(1U)
#define STM32_SW_CSI            STM32_SW_FIELD(2U)
#define STM32_SW_HSE            STM32_SW_FIELD(2U)
#define STM32_SW_PLL1P          STM32_SW_FIELD(3U)

#define STM32_MCO1SEL_MASK      (7U << 22)
#define STM32_MCO1PRE_FIELD(n)  ((n) << 22)
#define STM32_MCO1SEL_HSI       STM32_MCO1PRE_FIELD(0U)
#define STM32_MCO1SEL_LSE       STM32_MCO1PRE_FIELD(1U)
#define STM32_MCO1SEL_HSE       STM32_MCO1PRE_FIELD(2U)
#define STM32_MCO1SEL_PLL1P     STM32_MCO1PRE_FIELD(3U)
#define STM32_MCO1SEL_HSI48     STM32_MCO1PRE_FIELD(4U)

#define STM32_MCO1PRE_MASK      (7U << 18)
#define STM32_MCO1PRE_FIELD(n)  ((n) << 18)
#define STM32_MCO1PRE_DISABLED  STM32_MCOPRE_FIELD(0U)
#define STM32_MCO1PRE_DIV1      STM32_MCOPRE_FIELD(1U)
#define STM32_MCO1PRE_DIV2      STM32_MCOPRE_FIELD(2U)
#define STM32_MCO1PRE_DIV3      STM32_MCOPRE_FIELD(3U)
#define STM32_MCO1PRE_DIV4      STM32_MCOPRE_FIELD(4U)
#define STM32_MCO1PRE_DIV5      STM32_MCOPRE_FIELD(5U)
#define STM32_MCO1PRE_DIV6      STM32_MCOPRE_FIELD(6U)
#define STM32_MCO1PRE_DIV7      STM32_MCOPRE_FIELD(7U)
#define STM32_MCO1PRE_DIV8      STM32_MCOPRE_FIELD(8U)
#define STM32_MCO1PRE_DIV9      STM32_MCOPRE_FIELD(9U)
#define STM32_MCO1PRE_DIV10     STM32_MCOPRE_FIELD(10U)
#define STM32_MCO1PRE_DIV11     STM32_MCOPRE_FIELD(11U)
#define STM32_MCO1PRE_DIV12     STM32_MCOPRE_FIELD(12U)
#define STM32_MCO1PRE_DIV13     STM32_MCOPRE_FIELD(13U)
#define STM32_MCO1PRE_DIV14     STM32_MCOPRE_FIELD(14U)
#define STM32_MCO1PRE_DIV15     STM32_MCOPRE_FIELD(15U)

#define STM32_MCO2SEL_MASK      (7U << 29)
#define STM32_MCO2PRE_FIELD(n)  ((n) << 29)
#define STM32_MCO2SEL_SYSCLK    STM32_MCO2PRE_FIELD(0U)
#define STM32_MCO2SEL_PLL2P     STM32_MCO2PRE_FIELD(1U)
#define STM32_MCO2SEL_HSE       STM32_MCO2PRE_FIELD(2U)
#define STM32_MCO2SEL_PLL1P     STM32_MCO2PRE_FIELD(3U)
#define STM32_MCO2SEL_CSI       STM32_MCO2PRE_FIELD(4U)
#define STM32_MCO2SEL_LSI       STM32_MCO2PRE_FIELD(5U)

#define STM32_MCO2PRE_MASK      (7U << 18)
#define STM32_MCO2PRE_FIELD(n)  ((n) << 18)
#define STM32_MCO2PRE_DISABLED  STM32_MCO2PRE_FIELD(0U)
#define STM32_MCO2PRE_DIV1      STM32_MCO2PRE_FIELD(1U)
#define STM32_MCO2PRE_DIV2      STM32_MCO2PRE_FIELD(2U)
#define STM32_MCO2PRE_DIV3      STM32_MCO2PRE_FIELD(3U)
#define STM32_MCO2PRE_DIV4      STM32_MCO2PRE_FIELD(4U)
#define STM32_MCO2PRE_DIV5      STM32_MCO2PRE_FIELD(5U)
#define STM32_MCO2PRE_DIV6      STM32_MCO2PRE_FIELD(6U)
#define STM32_MCO2PRE_DIV7      STM32_MCO2PRE_FIELD(7U)
#define STM32_MCO2PRE_DIV8      STM32_MCO2PRE_FIELD(8U)
#define STM32_MCO2PRE_DIV9      STM32_MCO2PRE_FIELD(9U)
#define STM32_MCO2PRE_DIV10     STM32_MCO2PRE_FIELD(10U)
#define STM32_MCO2PRE_DIV11     STM32_MCO2PRE_FIELD(11U)
#define STM32_MCO2PRE_DIV12     STM32_MCO2PRE_FIELD(12U)
#define STM32_MCO2PRE_DIV13     STM32_MCO2PRE_FIELD(13U)
#define STM32_MCO2PRE_DIV14     STM32_MCO2PRE_FIELD(14U)
#define STM32_MCO2PRE_DIV15     STM32_MCO2PRE_FIELD(15U)
/** @} */

/**
 * @name    RCC_PLLxCFGR register bits definitions
 * @{
 */
#define STM32_PLL1SRC_MASK       (3 << 0)
#define STM32_PLL1SRC_FIELD(n)   ((n) << 0)
#define STM32_PLL1SRC_NOCLOCK    STM32_PLL1SRC_FIELD(0U)
#define STM32_PLL1SRC_HSI        STM32_PLL1SRC_FIELD(1U)
#define STM32_PLL1SRC_CSI        STM32_PLL1SRC_FIELD(2U)
#define STM32_PLL1SRC_HSE        STM32_PLL1SRC_FIELD(3U)

#define STM32_PLL2SRC_MASK       (3 << 0)
#define STM32_PLL2SRC_FIELD(n)   ((n) << 0)
#define STM32_PLL2SRC_NOCLOCK    STM32_PLL2SRC_FIELD(0U)
#define STM32_PLL2SRC_HSI        STM32_PLL2SRC_FIELD(1U)
#define STM32_PLL2SRC_CSI        STM32_PLL2SRC_FIELD(2U)
#define STM32_PLL2SRC_HSE        STM32_PLL2SRC_FIELD(3U)

#define STM32_PLL3SRC_MASK       (3 << 0)
#define STM32_PLL3SRC_FIELD(n)   ((n) << 0)
#define STM32_PLL3SRC_NOCLOCK    STM32_PLL3SRC_FIELD(0U)
#define STM32_PLL3SRC_HSI        STM32_PLL3SRC_FIELD(1U)
#define STM32_PLL3SRC_CSI        STM32_PLL3SRC_FIELD(2U)
#define STM32_PLL3SRC_HSE        STM32_PLL3SRC_FIELD(3U)
/** @} */

/**
 * @name    RCC_CCIPR1 register bits definitions
 * @{
 */
#define STM32_USART1SEL_MASK    (7U << 0)
#define STM32_USART1SEL_FIELD(n) ((n) << 0)
#define STM32_USART1SEL_PCLK2   STM32_USART1SEL_FIELD(0U)
#define STM32_USART1SEL_PLL2Q   STM32_USART1SEL_FIELD(1U)
#define STM32_USART1SEL_PLL3Q   STM32_USART1SEL_FIELD(2U)
#define STM32_USART1SEL_HSI     STM32_USART1SEL_FIELD(3U)
#define STM32_USART1SEL_CSI     STM32_USART1SEL_FIELD(4U)
#define STM32_USART1SEL_LSE     STM32_USART1SEL_FIELD(5U)

#define STM32_USART2SEL_MASK    (7U << 3)
#define STM32_USART2SEL_FIELD(n) ((n) << 3)
#define STM32_USART2SEL_PCLK1   STM32_USART2SEL_FIELD(0U)
#define STM32_USART2SEL_PLL2Q   STM32_USART2SEL_FIELD(1U)
#define STM32_USART2SEL_PLL3Q   STM32_USART2SEL_FIELD(2U)
#define STM32_USART2SEL_HSI     STM32_USART2SEL_FIELD(3U)
#define STM32_USART2SEL_CSI     STM32_USART2SEL_FIELD(4U)
#define STM32_USART2SEL_LSE     STM32_USART2SEL_FIELD(5U)

#define STM32_USART3SEL_MASK    (7U << 6)
#define STM32_USART3SEL_FIELD(n) ((n) << 6)
#define STM32_USART3SEL_PCLK1   STM32_USART3SEL_FIELD(0U)
#define STM32_USART3SEL_PLL2Q   STM32_USART3SEL_FIELD(1U)
#define STM32_USART3SEL_PLL3Q   STM32_USART3SEL_FIELD(2U)
#define STM32_USART3SEL_HSI     STM32_USART3SEL_FIELD(3U)
#define STM32_USART3SEL_CSI     STM32_USART3SEL_FIELD(4U)
#define STM32_USART3SEL_LSE     STM32_USART3SEL_FIELD(5U)

#define STM32_UART4SEL_MASK     (7U << 9)
#define STM32_UART4SEL_FIELD(n) ((n) << 9)
#define STM32_UART4SEL_PCLK1    STM32_UART4SEL_FIELD(0U)
#define STM32_UART4SEL_PLL2Q    STM32_UART4SEL_FIELD(1U)
#define STM32_UART4SEL_PLL3Q    STM32_UART4SEL_FIELD(2U)
#define STM32_UART4SEL_HSI      STM32_UART4SEL_FIELD(3U)
#define STM32_UART4SEL_CSI      STM32_UART4SEL_FIELD(4U)
#define STM32_UART4SEL_LSE      STM32_UART4SEL_FIELD(5U)

#define STM32_UART5SEL_MASK     (7U << 12)
#define STM32_UART5SEL_FIELD(n) ((n) << 12)
#define STM32_UART5SEL_PCLK1    STM32_UART5SEL_FIELD(0U)
#define STM32_UART5SEL_PLL2Q    STM32_UART5SEL_FIELD(1U)
#define STM32_UART5SEL_PLL3Q    STM32_UART5SEL_FIELD(2U)
#define STM32_UART5SEL_HSI      STM32_UART5SEL_FIELD(3U)
#define STM32_UART5SEL_CSI      STM32_UART5SEL_FIELD(4U)
#define STM32_UART5SEL_LSE      STM32_UART5SEL_FIELD(5U)

#define STM32_USART6SEL_MASK    (7U << 15)
#define STM32_USART6SEL_FIELD(n) ((n) << 15)
#define STM32_USART6SEL_PCLK1   STM32_USART6SEL_FIELD(0U)
#define STM32_USART6SEL_PLL2Q   STM32_USART6SEL_FIELD(1U)
#define STM32_USART6SEL_PLL3Q   STM32_USART6SEL_FIELD(2U)
#define STM32_USART6SEL_HSI     STM32_USART6SEL_FIELD(3U)
#define STM32_USART6SEL_CSI     STM32_USART6SEL_FIELD(4U)
#define STM32_USART6SEL_LSE     STM32_USART6SEL_FIELD(5U)

#define STM32_UART7SEL_MASK     (7U << 18)
#define STM32_UART7SEL_FIELD(n) ((n) << 18)
#define STM32_UART7SEL_PCLK1    STM32_UART7SEL_FIELD(0U)
#define STM32_UART7SEL_PLL2Q    STM32_UART7SEL_FIELD(1U)
#define STM32_UART7SEL_PLL3Q    STM32_UART7SEL_FIELD(2U)
#define STM32_UART7SEL_HSI      STM32_UART7SEL_FIELD(3U)
#define STM32_UART7SEL_CSI      STM32_UART7SEL_FIELD(4U)
#define STM32_UART7SEL_LSE      STM32_UART7SEL_FIELD(5U)

#define STM32_UART8SEL_MASK     (7U << 21)
#define STM32_UART8SEL_FIELD(n) ((n) << 21)
#define STM32_UART8SEL_PCLK1    STM32_UART8SEL_FIELD(0U)
#define STM32_UART8SEL_PLL2Q    STM32_UART8SEL_FIELD(1U)
#define STM32_UART8SEL_PLL3Q    STM32_UART8SEL_FIELD(2U)
#define STM32_UART8SEL_HSI      STM32_UART8SEL_FIELD(3U)
#define STM32_UART8SEL_CSI      STM32_UART8SEL_FIELD(4U)
#define STM32_UART8SEL_LSE      STM32_UART8SEL_FIELD(5U)

#define STM32_UART9SEL_MASK     (7U << 24)
#define STM32_UART9SEL_FIELD(n) ((n) << 24)
#define STM32_UART9SEL_PCLK1    STM32_UART9SEL_FIELD(0U)
#define STM32_UART9SEL_PLL2Q    STM32_UART9SEL_FIELD(1U)
#define STM32_UART9SEL_PLL3Q    STM32_UART9SEL_FIELD(2U)
#define STM32_UART9SEL_HSI      STM32_UART9SEL_FIELD(3U)
#define STM32_UART9SEL_CSI      STM32_UART9SEL_FIELD(4U)
#define STM32_UART9SEL_LSE      STM32_UART9SEL_FIELD(5U)

#define STM32_USART10SEL_MASK   (7U << 27)
#define STM32_USART10SEL_FIELD(n) ((n) << 27)
#define STM32_USART10SEL_PCLK1  STM32_USART10SEL_FIELD(0U)
#define STM32_USART10SEL_PLL2Q  STM32_USART10SEL_FIELD(1U)
#define STM32_USART10SEL_PLL3Q  STM32_USART10SEL_FIELD(2U)
#define STM32_USART10SEL_HSI    STM32_USART10SEL_FIELD(3U)
#define STM32_USART10SEL_CSI    STM32_USART10SEL_FIELD(4U)
#define STM32_USART10SEL_LSE    STM32_USART10SEL_FIELD(5U)

#define STM32_TIMICSEL_NOCLK    0U
#define STM32_TIMICSEL_INTCLK   (1U << 31)
/** @} */

/**
 * @name    RCC_CCIPR2 register bits definitions
 * @{
 */
#define STM32_USART11SEL_MASK   (7U << 0)
#define STM32_USART11SEL_FIELD(n) ((n) << 0)
#define STM32_USART11SEL_PCLK1  STM32_USART11SEL_FIELD(0U)
#define STM32_USART11SEL_PLL2Q  STM32_USART11SEL_FIELD(1U)
#define STM32_USART11SEL_PLL3Q  STM32_USART11SEL_FIELD(2U)
#define STM32_USART11SEL_HSI    STM32_USART11SEL_FIELD(3U)
#define STM32_USART11SEL_CSI    STM32_USART11SEL_FIELD(4U)
#define STM32_USART11SEL_LSE    STM32_USART11SEL_FIELD(5U)

#define STM32_UART12SEL_MASK    (7U << 4)
#define STM32_UART12SEL_FIELD(n) ((n) << 4)
#define STM32_UART12SEL_PCLK1   STM32_UART12SEL_FIELD(0U)
#define STM32_UART12SEL_PLL2Q   STM32_UART12SEL_FIELD(1U)
#define STM32_UART12SEL_PLL3Q   STM32_UART12SEL_FIELD(2U)
#define STM32_UART12SEL_HSI     STM32_UART12SEL_FIELD(3U)
#define STM32_UART12SEL_CSI     STM32_UART12SEL_FIELD(4U)
#define STM32_UART12SEL_LSE     STM32_UART12SEL_FIELD(5U)

#define STM32_LPTIM1SEL_MASK    (7U << 8)
#define STM32_LPTIM1SEL_FIELD(n) ((n) << 8)
#define STM32_LPTIM1SEL_PCLK3   STM32_LPTIM1SEL_FIELD(0U)
#define STM32_LPTIM1SEL_PLL2P   STM32_LPTIM1SEL_FIELD(1U)
#define STM32_LPTIM1SEL_PLL3R   STM32_LPTIM1SEL_FIELD(2U)
#define STM32_LPTIM1SEL_LSE     STM32_LPTIM1SEL_FIELD(3U)
#define STM32_LPTIM1SEL_LSI     STM32_LPTIM1SEL_FIELD(4U)
#define STM32_LPTIM1SEL_PER     STM32_LPTIM1SEL_FIELD(5U)

#define STM32_LPTIM2SEL_MASK    (7U << 12)
#define STM32_LPTIM2SEL_FIELD(n) ((n) << 12)
#define STM32_LPTIM2SEL_PCLK1   STM32_LPTIM2SEL_FIELD(0U)
#define STM32_LPTIM2SEL_PLL2P   STM32_LPTIM2SEL_FIELD(1U)
#define STM32_LPTIM2SEL_PLL3R   STM32_LPTIM2SEL_FIELD(2U)
#define STM32_LPTIM2SEL_LSE     STM32_LPTIM2SEL_FIELD(3U)
#define STM32_LPTIM2SEL_LSI     STM32_LPTIM2SEL_FIELD(4U)
#define STM32_LPTIM2SEL_PER     STM32_LPTIM2SEL_FIELD(5U)

#define STM32_LPTIM3SEL_MASK    (7U << 16)
#define STM32_LPTIM3SEL_FIELD(n) ((n) << 16)
#define STM32_LPTIM3SEL_PCLK3   STM32_LPTIM3SEL_FIELD(0U)
#define STM32_LPTIM3SEL_PLL2P   STM32_LPTIM3SEL_FIELD(1U)
#define STM32_LPTIM3SEL_PLL3R   STM32_LPTIM3SEL_FIELD(2U)
#define STM32_LPTIM3SEL_LSE     STM32_LPTIM3SEL_FIELD(3U)
#define STM32_LPTIM3SEL_LSI     STM32_LPTIM3SEL_FIELD(4U)
#define STM32_LPTIM3SEL_PER     STM32_LPTIM3SEL_FIELD(5U)

#define STM32_LPTIM4SEL_MASK    (7U << 20)
#define STM32_LPTIM4SEL_FIELD(n) ((n) << 20)
#define STM32_LPTIM4SEL_PCLK3   STM32_LPTIM4SEL_FIELD(0U)
#define STM32_LPTIM4SEL_PLL2P   STM32_LPTIM4SEL_FIELD(1U)
#define STM32_LPTIM4SEL_PLL3R   STM32_LPTIM4SEL_FIELD(2U)
#define STM32_LPTIM4SEL_LSE     STM32_LPTIM4SEL_FIELD(3U)
#define STM32_LPTIM4SEL_LSI     STM32_LPTIM4SEL_FIELD(4U)
#define STM32_LPTIM4SEL_PER     STM32_LPTIM4SEL_FIELD(5U)

#define STM32_LPTIM5SEL_MASK    (7U << 24)
#define STM32_LPTIM5SEL_FIELD(n) ((n) << 24)
#define STM32_LPTIM5SEL_PCLK3   STM32_LPTIM5SEL_FIELD(0U)
#define STM32_LPTIM5SEL_PLL2P   STM32_LPTIM5SEL_FIELD(1U)
#define STM32_LPTIM5SEL_PLL3R   STM32_LPTIM5SEL_FIELD(2U)
#define STM32_LPTIM5SEL_LSE     STM32_LPTIM5SEL_FIELD(3U)
#define STM32_LPTIM5SEL_LSI     STM32_LPTIM5SEL_FIELD(4U)
#define STM32_LPTIM5SEL_PER     STM32_LPTIM5SEL_FIELD(5U)

#define STM32_LPTIM6SEL_MASK    (7U << 28)
#define STM32_LPTIM6SEL_FIELD(n) ((n) << 28)
#define STM32_LPTIM6SEL_PCLK3   STM32_LPTIM6SEL_FIELD(0U)
#define STM32_LPTIM6SEL_PLL2P   STM32_LPTIM6SEL_FIELD(1U)
#define STM32_LPTIM6SEL_PLL3R   STM32_LPTIM6SEL_FIELD(2U)
#define STM32_LPTIM6SEL_LSE     STM32_LPTIM6SEL_FIELD(3U)
#define STM32_LPTIM6SEL_LSI     STM32_LPTIM6SEL_FIELD(4U)
#define STM32_LPTIM6SEL_PER     STM32_LPTIM6SEL_FIELD(5U)
/** @} */

/**
 * @name    RCC_CCIPR3 register bits definitions
 * @{
 */
#define STM32_SPI1SEL_MASK      (7U << 0)
#define STM32_SPI1SEL_FIELD(n)  ((n) << 0)
#define STM32_SPI1SEL_PLL1Q     STM32_SPI1SEL_FIELD(0U)
#define STM32_SPI1SEL_PLL2P     STM32_SPI1SEL_FIELD(1U)
#define STM32_SPI1SEL_PLL3P     STM32_SPI1SEL_FIELD(2U)
#define STM32_SPI1SEL_AUDIOCLK  STM32_SPI1SEL_FIELD(3U)
#define STM32_SPI1SEL_PER       STM32_SPI1SEL_FIELD(4U)

#define STM32_SPI2SEL_MASK      (7U << 3)
#define STM32_SPI2SEL_FIELD(n)  ((n) << 3)
#define STM32_SPI2SEL_PLL1Q     STM32_SPI2SEL_FIELD(0U)
#define STM32_SPI2SEL_PLL2P     STM32_SPI2SEL_FIELD(1U)
#define STM32_SPI2SEL_PLL3P     STM32_SPI2SEL_FIELD(2U)
#define STM32_SPI2SEL_AUDIOCLK  STM32_SPI2SEL_FIELD(3U)
#define STM32_SPI2SEL_PER       STM32_SPI2SEL_FIELD(4U)

#define STM32_SPI3SEL_MASK      (7U << 6)
#define STM32_SPI3SEL_FIELD(n)  ((n) << 6)
#define STM32_SPI3SEL_PLL1Q     STM32_SPI3SEL_FIELD(0U)
#define STM32_SPI3SEL_PLL2P     STM32_SPI3SEL_FIELD(1U)
#define STM32_SPI3SEL_PLL3P     STM32_SPI3SEL_FIELD(2U)
#define STM32_SPI3SEL_AUDIOCLK  STM32_SPI3SEL_FIELD(3U)
#define STM32_SPI3SEL_PER       STM32_SPI3SEL_FIELD(4U)

#define STM32_SPI4SEL_MASK      (7U << 9)
#define STM32_SPI4SEL_FIELD(n)  ((n) << 9)
#define STM32_SPI4SEL_PCLK2     STM32_SPI4SEL_FIELD(0U)
#define STM32_SPI4SEL_PLL2P     STM32_SPI4SEL_FIELD(1U)
#define STM32_SPI4SEL_PLL3P     STM32_SPI4SEL_FIELD(2U)
#define STM32_SPI4SEL_HSI       STM32_SPI4SEL_FIELD(3U)
#define STM32_SPI4SEL_CSI       STM32_SPI4SEL_FIELD(4U)
#define STM32_SPI4SEL_HSE       STM32_SPI4SEL_FIELD(5U)

#define STM32_SPI5SEL_MASK      (7U << 12)
#define STM32_SPI5SEL_FIELD(n)  ((n) << 12)
#define STM32_SPI5SEL_PCLK3     STM32_SPI5SEL_FIELD(0U)
#define STM32_SPI5SEL_PLL2P     STM32_SPI5SEL_FIELD(1U)
#define STM32_SPI5SEL_PLL3P     STM32_SPI5SEL_FIELD(2U)
#define STM32_SPI5SEL_HSI       STM32_SPI5SEL_FIELD(3U)
#define STM32_SPI5SEL_CSI       STM32_SPI5SEL_FIELD(4U)
#define STM32_SPI5SEL_HSE       STM32_SPI5SEL_FIELD(5U)

#define STM32_SPI6SEL_MASK      (7U << 15)
#define STM32_SPI6SEL_FIELD(n)  ((n) << 15)
#define STM32_SPI6SEL_PCLK2     STM32_SPI6SEL_FIELD(0U)
#define STM32_SPI6SEL_PLL2P     STM32_SPI6SEL_FIELD(1U)
#define STM32_SPI6SEL_PLL3P     STM32_SPI6SEL_FIELD(2U)
#define STM32_SPI6SEL_HSI       STM32_SPI6SEL_FIELD(3U)
#define STM32_SPI6SEL_CSI       STM32_SPI6SEL_FIELD(4U)
#define STM32_SPI6SEL_HSE       STM32_SPI6SEL_FIELD(5U)

#define STM32_LPUART1SEL_MASK   (7U << 24)
#define STM32_LPUART1SEL_FIELD(n) ((n) << 24)
#define STM32_LPUART1SEL_PCLK3  STM32_LPUART1SEL_FIELD(0U)
#define STM32_LPUART1SEL_PLL2Q  STM32_LPUART1SEL_FIELD(1U)
#define STM32_LPUART1SEL_PLL3Q  STM32_LPUART1SEL_FIELD(2U)
#define STM32_LPUART1SEL_HSI    STM32_LPUART1SEL_FIELD(3U)
#define STM32_LPUART1SEL_CSI    STM32_LPUART1SEL_FIELD(4U)
#define STM32_LPUART1SEL_LSE    STM32_LPUART1SEL_FIELD(5U)
/** @} */

/**
 * @name    RCC_CCIPR4 register bits definitions
 * @{
 */
#define STM32_OSPISEL_MASK      (3U << 0)
#define STM32_OSPISEL_FIELD(n)  ((n) << 0)
#define STM32_OSPISEL_HCLK4     STM32_OSPISEL_FIELD(0U)
#define STM32_OSPISEL_PLL1Q     STM32_OSPISEL_FIELD(1U)
#define STM32_OSPISEL_PLL2R     STM32_OSPISEL_FIELD(2U)
#define STM32_OSPISEL_PER       STM32_OSPISEL_FIELD(3U)

#define STM32_SYSTICKSEL_MASK   (3U << 2)
#define STM32_SYSTICKSEL_FIELD(n) ((n) << 2)
#define STM32_SYSTICKSEL_HCLKDIV8 STM32_SYSTICKSEL_FIELD(0U)
#define STM32_SYSTICKSEL_LSI    STM32_SYSTICKSEL_FIELD(1U)
#define STM32_SYSTICKSEL_LSE    STM32_SYSTICKSEL_FIELD(2U)
#define STM32_SYSTICKSEL_NOCLOCK STM32_SYSTICKSEL_FIELD(3U)

#define STM32_USBSEL_MASK      (3U << 4)
#define STM32_USBSEL_FIELD(n)  ((n) << 4)
#define STM32_USBSEL_NOCLOCK   STM32_USBSEL_FIELD(0U)
#define STM32_USBSEL_PLL1Q     STM32_USBSEL_FIELD(1U)
#define STM32_USBSEL_PLL3Q     STM32_USBSEL_FIELD(2U)
#define STM32_USBSEL_HSI48     STM32_USBSEL_FIELD(3U)

#define STM32_SDMMC1SEL_MASK   (1U << 6)
#define STM32_SDMMC1SEL_FIELD(n) ((n) << 6)
#define STM32_SDMMC1SEL_PLL1Q  STM32_SDMMC1SEL_FIELD(0U)
#define STM32_SDMMC1SEL_PLL2R  STM32_SDMMC1SEL_FIELD(1U)

#define STM32_SDMMC2SEL_MASK   (1U << 7)
#define STM32_SDMMC2SEL_FIELD(n) ((n) << 7)
#define STM32_SDMMC2SEL_PLL1Q  STM32_SDMMC2SEL_FIELD(0U)
#define STM32_SDMMC2SEL_PLL2R  STM32_SDMMC2SEL_FIELD(1U)

#define STM32_I2C1SEL_MASK      (3U << 16)
#define STM32_I2C1SEL_FIELD(n)  ((n) << 16)
#define STM32_I2C1SEL_PCLK1     STM32_I2C1SEL_FIELD(0U)
#define STM32_I2C1SEL_PLL3R     STM32_I2C1SEL_FIELD(1U)
#define STM32_I2C1SEL_HSI       STM32_I2C1SEL_FIELD(2U)
#define STM32_I2C1SEL_CSI       STM32_I2C1SEL_FIELD(3U)

#define STM32_I2C2SEL_MASK      (3U << 18)
#define STM32_I2C2SEL_FIELD(n)  ((n) << 18)
#define STM32_I2C2SEL_PCLK1     STM32_I2C2SEL_FIELD(0U)
#define STM32_I2C2SEL_PLL3R     STM32_I2C2SEL_FIELD(1U)
#define STM32_I2C2SEL_HSI       STM32_I2C2SEL_FIELD(2U)
#define STM32_I2C2SEL_CSI       STM32_I2C2SEL_FIELD(3U)

#define STM32_I2C3SEL_MASK      (3U << 20)
#define STM32_I2C3SEL_FIELD(n)  ((n) << 20)
#define STM32_I2C3SEL_PCLK3     STM32_I2C3SEL_FIELD(0U)
#define STM32_I2C3SEL_PLL3R     STM32_I2C3SEL_FIELD(1U)
#define STM32_I2C3SEL_HSI       STM32_I2C3SEL_FIELD(2U)
#define STM32_I2C3SEL_CSI       STM32_I2C3SEL_FIELD(3U)

#define STM32_I2C4SEL_MASK      (3U << 22)
#define STM32_I2C4SEL_FIELD(n)  ((n) << 22)
#define STM32_I2C4SEL_PCLK3     STM32_I2C4SEL_FIELD(0U)
#define STM32_I2C4SEL_PLL3R     STM32_I2C4SEL_FIELD(1U)
#define STM32_I2C4SEL_HSI       STM32_I2C4SEL_FIELD(2U)
#define STM32_I2C4SEL_CSI       STM32_I2C4SEL_FIELD(3U)

#define STM32_I3C1SEL_MASK      (3U << 24)
#define STM32_I3C1SEL_FIELD(n)  ((n) << 24)
#define STM32_I3C1SEL_PCLK1     STM32_I3C1SEL_FIELD(0U)
#define STM32_I3C1SEL_PLL3R     STM32_I3C1SEL_FIELD(1U)
#define STM32_I3C1SEL_HSI       STM32_I3C1SEL_FIELD(2U)
/** @} */

/**
 * @name    RCC_CCIPR5 register bits definitions
 * @{
 */
#define STM32_ADCDACSEL_MASK    (3U << 0)
#define STM32_ADCDACSEL_FIELD(n) ((n) << 0)
#define STM32_ADCDACSEL_HCLK    STM32_ADCDACSEL_FIELD(0U)
#define STM32_ADCDACSEL_SYSCLK  STM32_ADCDACSEL_FIELD(1U)
#define STM32_ADCDACSEL_PLL2R   STM32_ADCDACSEL_FIELD(2U)
#define STM32_ADCDACSEL_HSE     STM32_ADCDACSEL_FIELD(3U)
#define STM32_ADCDACSEL_HSI     STM32_ADCDACSEL_FIELD(4U)
#define STM32_ADCDACSEL_CSI     STM32_ADCDACSEL_FIELD(5U)

#define STM32_DACSEL_MASK       (1U << 3)
#define STM32_DACSEL_FIELD(n)   ((n) << 3)
#define STM32_DACSEL_LSI        STM32_DACSEL_FIELD(0U)
#define STM32_DACSEL_LSE        STM32_DACSEL_FIELD(1U)

#define STM32_RNGSEL_MASK      (3U << 4)
#define STM32_RNGSEL_FIELD(n)  ((n) << 4)
#define STM32_RNGSEL_HSI48     STM32_RNGSEL_FIELD(0U)
#define STM32_RNGSEL_PLL1Q     STM32_RNGSEL_FIELD(1U)
#define STM32_RNGSEL_LSE       STM32_RNGSEL_FIELD(2U)
#define STM32_RNGSEL_LSI       STM32_RNGSEL_FIELD(3U)

#define STM32_CECSEL_MASK      (3U << 6)
#define STM32_CECSEL_FIELD(n)  ((n) << 6)
#define STM32_CECSEL_HSI48     STM32_CECSEL_FIELD(0U)
#define STM32_CECSEL_PLL1Q     STM32_CECSEL_FIELD(1U)
#define STM32_CECSEL_LSE       STM32_CECSEL_FIELD(2U)
#define STM32_CECSEL_LSI       STM32_CECSEL_FIELD(3U)

#define STM32_FDCANSEL_MASK    (3U << 8)
#define STM32_FDCANSEL_FIELD(n) ((n) << 8)
#define STM32_FDCANSEL_HSE     STM32_FDCANSEL_FIELD(0U)
#define STM32_FDCANSEL_PLL1Q   STM32_FDCANSEL_FIELD(1U)
#define STM32_FDCANSEL_PLL2Q   STM32_FDCANSEL_FIELD(2U)

#define STM32_SAI1SEL_MASK     (7U << 16)
#define STM32_SAI1SEL_FIELD(n) ((n) << 16)
#define STM32_SAI1SEL_PLL1Q    STM32_SAI1SEL_FIELD(0U)
#define STM32_SAI1SEL_PLL2P    STM32_SAI1SEL_FIELD(1U)
#define STM32_SAI1SEL_PLL3P    STM32_SAI1SEL_FIELD(2U)
#define STM32_SAI1SEL_AUDIOCLK STM32_SAI1SEL_FIELD(3U)
#define STM32_SAI1SEL_PER      STM32_SAI1SEL_FIELD(4U)

#define STM32_SAI2SEL_MASK     (7U << 19)
#define STM32_SAI2SEL_FIELD(n) ((n) << 19)
#define STM32_SAI2SEL_PLL1Q    STM32_SAI2SEL_FIELD(0U)
#define STM32_SAI2SEL_PLL2P    STM32_SAI2SEL_FIELD(1U)
#define STM32_SAI2SEL_PLL3P    STM32_SAI2SEL_FIELD(2U)
#define STM32_SAI2SEL_AUDIOCLK STM32_SAI2SEL_FIELD(3U)
#define STM32_SAI2SEL_PER      STM32_SAI2SEL_FIELD(4U)

#define STM32_CKPERSEL_MASK      (3U << 30)
#define STM32_CKPERSEL_FIELD(n)  ((n) << 30)
#define STM32_CKPERSEL_HSI       STM32_CKPERSEL_FIELD(0U)
#define STM32_CKPERSEL_CSI       STM32_CKPERSEL_FIELD(1U)
#define STM32_CKPERSEL_HSE       STM32_CKPERSEL_FIELD(2U)
/** @} */

/**
 * @name    RCC_BDCR register bits definitions
 * @{
 */
#define STM32_RTCSEL_MASK       (3U << 8)
#define STM32_RTCSEL_FIELD(n)   ((n) << 8)
#define STM32_RTCSEL_NOCLOCK    STM32_RTCSEL_FIELD(0U)
#define STM32_RTCSEL_LSE        STM32_RTCSEL_FIELD(1U)
#define STM32_RTCSEL_LSI        STM32_RTCSEL_FIELD(2U)
#define STM32_RTCSEL_HSEDIV     STM32_RTCSEL_FIELD(3U)

#define STM32_LSCOSEL_MASK      (2U << 24)
#define STM32_LSCOSEL_FIELD(n)  ((n) << 24)
#define STM32_LSCOSEL_NOCLOCK   STM32_LSCOSEL_FIELD(0U)
#define STM32_LSCOSEL_LSI       STM32_LSCOSEL_FIELD(1U)
#define STM32_LSCOSEL_LSE       STM32_LSCOSEL_FIELD(3U)
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
 * @brief   PWR VOSCR register initialization value.
 */
#if !defined(STM32_PWR_VOSCR) || defined(__DOXYGEN__)
#define STM32_PWR_VOSCR                     STM32_VOS_RANGE3
#endif

/**
 * @brief   PWR BDCR register initialization value.
 */
#if !defined(STM32_PWR_BDCR) || defined(__DOXYGEN__)
#define STM32_PWR_BDCR                      (0U)
#endif

/**
 * @brief   PWR UCPDR register initialization value.
 */
#if !defined(STM32_PWR_UCPDR) || defined(__DOXYGEN__)
#define STM32_PWR_UCPDR                     (0U)
#endif

/**
 * @brief   PWR SCCR register initialization value.
 */
#if !defined(STM32_PWR_SCCR) || defined(__DOXYGEN__)
#define STM32_PWR_SCCR                      (0U)
#endif

/**
 * @brief   PWR VMCR register initialization value.
 */
#if !defined(STM32_PWR_VMCR) || defined(__DOXYGEN__)
#define STM32_PWR_VMCR                      (0U)
#endif

/**
 * @brief   PWR USBSCR register initialization value.
 */
#if !defined(STM32_PWR_USBSCR) || defined(__DOXYGEN__)
#define STM32_PWR_USBSCR                    (0U)
#endif

/**
 * @brief   PWR WUCR register initialization value.
 */
#if !defined(STM32_PWR_WUCR) || defined(__DOXYGEN__)
#define STM32_PWR_WUCR                      (0U)
#endif

/**
 * @brief   PWR IORETR register initialization value.
 */
#if !defined(STM32_PWR_IORETR) || defined(__DOXYGEN__)
#define STM32_PWR_IORETR                    (0U)
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
 * @brief   Enables or disables the HSI clock source.
 */
#if !defined(STM32_HSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI_ENABLED                   TRUE
#endif

/**
 * @brief   HSIDIV divider value.
 * @note    The allowed values are 1, 2, 4, 8.
 */
#if !defined(STM32_HSIDIV_VALUE) || defined(__DOXYGEN__)
#define STM32_HSIDIV_VALUE                  2
#endif

/**
 * @brief   Enables or disables the HSI48 clock source.
 */
#if !defined(STM32_HSI48_ENABLED) || defined(__DOXYGEN__)
#define STM32_HSI48_ENABLED                 FALSE
#endif

/**
 * @brief   Enables or disables the CSI clock source.
 */
#if !defined(STM32_CSI_ENABLED) || defined(__DOXYGEN__)
#define STM32_CSI_ENABLED                   FALSE
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
#define STM32_SW                            STM32_SW_PLL1PCLK
#endif

/**
 * @brief   Clock source for PLL1.
 */
#if !defined(STM32_PLL1SRC) || defined(__DOXYGEN__)
#define STM32_PLL1SRC                       STM32_PLL1SRC_HSI
#endif

/**
 * @brief   PLL1M divider value.
 * @note    The allowed values are 1..63.
 */
#if !defined(STM32_PLLM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1M_VALUE                   4
#endif

/**
 * @brief   PLL1N multiplier value.
 * @note    The allowed values are 4..512.
 */
#if !defined(STM32_PLL1N_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1N_VALUE                   84
#endif

/**
 * @brief   PLL1P divider value.
 * @note    The allowed values are 4..128 (odd values forbidden).
 */
#if !defined(STM32_PLL1P_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1P_VALUE                   7
#endif

/**
 * @brief   PLL1Q divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL1Q_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1Q_VALUE                   8
#endif

/**
 * @brief   PLL1R divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL1R_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL1R_VALUE                   2
#endif

/**
 * @brief   Clock source for PLL2.
 */
#if !defined(STM32_PLL2SRC) || defined(__DOXYGEN__)
#define STM32_PLL2SRC                       STM32_PLL2SRC_HSI
#endif

/**
 * @brief   PLL2M divider value.
 * @note    The allowed values are 1..63.
 */
#if !defined(STM32_PLLM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2M_VALUE                   4
#endif

/**
 * @brief   PLL2N multiplier value.
 * @note    The allowed values are 4..512.
 */
#if !defined(STM32_PLL2N_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2N_VALUE                   84
#endif

/**
 * @brief   PLL2P divider value.
 * @note    The allowed values are 4..128.
 */
#if !defined(STM32_PLL2P_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2P_VALUE                   7
#endif

/**
 * @brief   PLL2Q divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL2Q_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2Q_VALUE                   8
#endif

/**
 * @brief   PLL2R divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL2R_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL2R_VALUE                   2
#endif

/**
 * @brief   Clock source for PLL3.
 */
#if !defined(STM32_PLL3SRC) || defined(__DOXYGEN__)
#define STM32_PLL3SRC                       STM32_PLL3SRC_HSI
#endif

/**
 * @brief   PLL3M divider value.
 * @note    The allowed values are 1..63.
 */
#if !defined(STM32_PLLM_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3M_VALUE                   4
#endif

/**
 * @brief   PLL3N multiplier value.
 * @note    The allowed values are 4..512.
 */
#if !defined(STM32_PLL3N_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3N_VALUE                   84
#endif

/**
 * @brief   PLL3P divider value.
 * @note    The allowed values are 4..128.
 */
#if !defined(STM32_PLL3P_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3P_VALUE                   7
#endif

/**
 * @brief   PLL3Q divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL3Q_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3Q_VALUE                   8
#endif

/**
 * @brief   PLL3R divider value.
 * @note    The allowed values are 1..128.
 */
#if !defined(STM32_PLL3R_VALUE) || defined(__DOXYGEN__)
#define STM32_PLL3R_VALUE                   2
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
 * @brief   MCO1 clock source.
 */
#if !defined(STM32_MCO1SEL) || defined(__DOXYGEN__)
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI
#endif

/**
 * @brief   MCO1 divider setting.
 */
#if !defined(STM32_MCO1PRE) || defined(__DOXYGEN__)
#define STM32_MCO1PRE                       STM32_MCO1PRE_DISABLED
#endif

/**
 * @brief   MCO2 clock source.
 */
#if !defined(STM32_MCO2SEL) || defined(__DOXYGEN__)
#define STM32_MCO2SEL                       STM32_MCO2SEL_CSI
#endif

/**
 * @brief   MCO1 divider setting.
 */
#if !defined(STM32_MCO2PRE) || defined(__DOXYGEN__)
#define STM32_MCO2PRE                       STM32_MCO2PRE_DISABLED
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
 * @brief   USART10 clock source.
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
#define STM32_LPUART1SEL                    STM32_LPUART1SEL_SYSCLK
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
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK3
#endif

/**
 * @brief   LPTIM2 clock source.
 */
#if !defined(STM32_LPTIM2SEL) || defined(__DOXYGEN__)
#define STM32_LPTIM2SEL                     STM32_LPTIM2SEL_PCLK3
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
#define STM32_DACSEL                        STM32_DACSEL_LSI
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
#define STM32_CECSEL                        STM32_CECSEL_HSI48
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

/**
 * @name    System Limits for VOS range 0
 * @{
 */
#define STM32_VOS0_SYSCLK_MAX           250000000
#define STM32_VOS0_HSECLK_MAX           50000000
#define STM32_VOS0_HSECLK_BYP_MAX       50000000
#define STM32_VOS0_HSECLK_MIN           4000000
#define STM32_VOS0_HSECLK_BYP_MIN       4000000
#define STM32_VOS0_LSECLK_MAX           32768
#define STM32_VOS0_LSECLK_BYP_MAX       1000000
#define STM32_VOS0_LSECLK_MIN           32768
#define STM32_VOS0_LSECLK_BYP_MIN       32768
#define STM32_VOS0_PLLIN_MAX            16000000
#define STM32_VOS0_PLLIN_MIN            2000000
#define STM32_VOS0_PLLVCO_MAX           560000000
#define STM32_VOS0_PLLVCO_MIN           128000000
#define STM32_VOS0_PLLP_MAX             250000000
#define STM32_VOS0_PLLP_MIN             1000000
#define STM32_VOS0_PLLQ_MAX             250000000
#define STM32_VOS0_PLLQ_MIN             1000000
#define STM32_VOS0_PLLR_MAX             250000000
#define STM32_VOS0_PLLR_MIN             1000000
#define STM32_VOS0_PCLK1_MAX            250000000
#define STM32_VOS0_PCLK2_MAX            250000000
#define STM32_VOS0_PCLK2_MAX            250000000
#define STM32_VOS0_ADCCLK_MAX           125000000

#define STM32_VOS0_0WS_THRESHOLD        42000000
#define STM32_VOS0_1WS_THRESHOLD        84000000
#define STM32_VOS0_2WS_THRESHOLD        126000000
#define STM32_VOS0_3WS_THRESHOLD        168000000
#define STM32_VOS0_4WS_THRESHOLD        210000000
/** @} */

/**
 * @name    System Limits for VOS range 1
 * @{
 */
#define STM32_VOS1_SYSCLK_MAX           200000000
#define STM32_VOS1_HSECLK_MAX           50000000
#define STM32_VOS1_HSECLK_BYP_MAX       50000000
#define STM32_VOS1_HSECLK_MIN           4000000
#define STM32_VOS1_HSECLK_BYP_MIN       4000000
#define STM32_VOS1_LSECLK_MAX           32768
#define STM32_VOS1_LSECLK_BYP_MAX       1000000
#define STM32_VOS1_LSECLK_MIN           32768
#define STM32_VOS1_LSECLK_BYP_MIN       32768
#define STM32_VOS1_PLLIN_MAX            16000000
#define STM32_VOS1_PLLIN_MIN            2000000
#define STM32_VOS1_PLLVCO_MAX           560000000
#define STM32_VOS1_PLLVCO_MIN           128000000
#define STM32_VOS1_PLLP_MAX             200000000
#define STM32_VOS1_PLLP_MIN             1000000
#define STM32_VOS1_PLLQ_MAX             200000000
#define STM32_VOS1_PLLQ_MIN             1000000
#define STM32_VOS1_PLLR_MAX             200000000
#define STM32_VOS1_PLLR_MIN             1000000
#define STM32_VOS1_PCLK1_MAX            200000000
#define STM32_VOS1_PCLK2_MAX            200000000
#define STM32_VOS1_PCLK2_MAX            200000000
#define STM32_VOS1_ADCCLK_MAX           100000000

#define STM32_VOS1_0WS_THRESHOLD        34000000
#define STM32_VOS1_1WS_THRESHOLD        68000000
#define STM32_VOS1_2WS_THRESHOLD        102000000
#define STM32_VOS1_3WS_THRESHOLD        136000000
#define STM32_VOS1_4WS_THRESHOLD        170000000
/** @} */

/**
 * @name    System Limits for VOS range 2
 * @{
 */
#define STM32_VOS2_SYSCLK_MAX           150000000
#define STM32_VOS2_HSECLK_MAX           50000000
#define STM32_VOS2_HSECLK_BYP_MAX       50000000
#define STM32_VOS2_HSECLK_MIN           4000000
#define STM32_VOS2_HSECLK_BYP_MIN       4000000
#define STM32_VOS2_LSECLK_MAX           32768
#define STM32_VOS2_LSECLK_BYP_MAX       1000000
#define STM32_VOS2_LSECLK_MIN           32768
#define STM32_VOS2_LSECLK_BYP_MIN       32768
#define STM32_VOS2_PLLIN_MAX            16000000
#define STM32_VOS2_PLLIN_MIN            2000000
#define STM32_VOS2_PLLVCO_MAX           560000000
#define STM32_VOS2_PLLVCO_MIN           128000000
#define STM32_VOS2_PLLP_MAX             150000000
#define STM32_VOS2_PLLP_MIN             1000000
#define STM32_VOS2_PLLQ_MAX             150000000
#define STM32_VOS2_PLLQ_MIN             1000000
#define STM32_VOS2_PLLR_MAX             150000000
#define STM32_VOS2_PLLR_MIN             1000000
#define STM32_VOS2_PCLK1_MAX            150000000
#define STM32_VOS2_PCLK2_MAX            150000000
#define STM32_VOS2_PCLK2_MAX            150000000
#define STM32_VOS2_ADCCLK_MAX           75000000

#define STM32_VOS2_0WS_THRESHOLD        30000000
#define STM32_VOS2_1WS_THRESHOLD        60000000
#define STM32_VOS2_2WS_THRESHOLD        90000000
#define STM32_VOS2_3WS_THRESHOLD        120000000
#define STM32_VOS2_4WS_THRESHOLD        0
/** @} */

/**
 * @name    System Limits for VOS range 3
 * @{
 */
#define STM32_VOS3_SYSCLK_MAX           100000000
#define STM32_VOS3_HSECLK_MAX           50000000
#define STM32_VOS3_HSECLK_BYP_MAX       50000000
#define STM32_VOS3_HSECLK_MIN           4000000
#define STM32_VOS3_HSECLK_BYP_MIN       4000000
#define STM32_VOS3_LSECLK_MAX           32768
#define STM32_VOS3_LSECLK_BYP_MAX       1000000
#define STM32_VOS3_LSECLK_MIN           32768
#define STM32_VOS3_LSECLK_BYP_MIN       32768
#define STM32_VOS3_PLLIN_MAX            16000000
#define STM32_VOS3_PLLIN_MIN            2000000
#define STM32_VOS3_PLLVCO_MAX           560000000
#define STM32_VOS3_PLLVCO_MIN           128000000
#define STM32_VOS3_PLLP_MAX             100000000
#define STM32_VOS3_PLLP_MIN             1000000
#define STM32_VOS3_PLLQ_MAX             100000000
#define STM32_VOS3_PLLQ_MIN             1000000
#define STM32_VOS3_PLLR_MAX             100000000
#define STM32_VOS3_PLLR_MIN             1000000
#define STM32_VOS3_PCLK1_MAX            100000000
#define STM32_VOS3_PCLK2_MAX            100000000
#define STM32_VOS3_PCLK2_MAX            100000000
#define STM32_VOS3_ADCCLK_MAX           50000000

#define STM32_VOS3_0WS_THRESHOLD        20000000
#define STM32_VOS3_1WS_THRESHOLD        40000000
#define STM32_VOS3_2WS_THRESHOLD        60000000
#define STM32_VOS3_3WS_THRESHOLD        80000000
#define STM32_VOS3_4WS_THRESHOLD        0
/** @} */

/* Voltage related limits.*/
#if ((STM32_PWR_VOSCR & STM32_VOS_MASK) == STM32_VOS_RANGE0) || defined(__DOXYGEN__)
#define STM32_SYSCLK_MAX                STM32_VOS0_SYSCLK_MAX
#define STM32_HSECLK_MAX                STM32_VOS0_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX            STM32_VOS0_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                STM32_VOS0_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN            STM32_VOS0_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                STM32_VOS0_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX            STM32_VOS0_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                STM32_VOS0_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN            STM32_VOS0_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                 STM32_VOS0_PLLIN_MAX
#define STM32_PLLIN_MIN                 STM32_VOS0_PLLIN_MIN
#define STM32_PLLVCO_MAX                STM32_VOS0_PLLVCO_MAX
#define STM32_PLLVCO_MIN                STM32_VOS0_PLLVCO_MIN
#define STM32_PLLP_MAX                  STM32_VOS0_PLLP_MAX
#define STM32_PLLP_MIN                  STM32_VOS0_PLLP_MIN
#define STM32_PLLQ_MAX                  STM32_VOS0_PLLQ_MAX
#define STM32_PLLQ_MIN                  STM32_VOS0_PLLQ_MIN
#define STM32_PLLR_MAX                  STM32_VOS0_PLLR_MAX
#define STM32_PLLR_MIN                  STM32_VOS0_PLLR_MIN
#define STM32_PCLK1_MAX                 STM32_VOS0_PCLK1_MAX
#define STM32_PCLK2_MAX                 STM32_VOS0_PCLK2_MAX
#define STM32_PCLK3_MAX                 STM32_VOS0_PCLK3_MAX
#define STM32_ADCCLK_MAX                STM32_VOS0_ADCCLK_MAX

#define STM32_0WS_THRESHOLD             STM32_VOS0_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD             STM32_VOS0_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD             STM32_VOS0_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD             STM32_VOS0_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD             STM32_VOS0_4WS_THRESHOLD
#define STM32_5WS_THRESHOLD             STM32_VOS0_5WS_THRESHOLD
#define STM32_6WS_THRESHOLD             STM32_VOS0_6WS_THRESHOLD
#define STM32_7WS_THRESHOLD             STM32_VOS0_7WS_THRESHOLD
#define STM32_8WS_THRESHOLD             STM32_VOS0_8WS_THRESHOLD

#elif (STM32_PWR_VOSCR & STM32_VOS_MASK) == STM32_VOS_RANGE1
#define STM32_SYSCLK_MAX                STM32_VOS1_SYSCLK_MAX
#define STM32_HSECLK_MAX                STM32_VOS1_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX            STM32_VOS1_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                STM32_VOS1_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN            STM32_VOS1_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                STM32_VOS1_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX            STM32_VOS1_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                STM32_VOS1_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN            STM32_VOS1_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                 STM32_VOS1_PLLIN_MAX
#define STM32_PLLIN_MIN                 STM32_VOS1_PLLIN_MIN
#define STM32_PLLVCO_MAX                STM32_VOS1_PLLVCO_MAX
#define STM32_PLLVCO_MIN                STM32_VOS1_PLLVCO_MIN
#define STM32_PLLP_MAX                  STM32_VOS1_PLLP_MAX
#define STM32_PLLP_MIN                  STM32_VOS1_PLLP_MIN
#define STM32_PLLQ_MAX                  STM32_VOS1_PLLQ_MAX
#define STM32_PLLQ_MIN                  STM32_VOS1_PLLQ_MIN
#define STM32_PLLR_MAX                  STM32_VOS1_PLLR_MAX
#define STM32_PLLR_MIN                  STM32_VOS1_PLLR_MIN
#define STM32_PCLK1_MAX                 STM32_VOS1_PCLK1_MAX
#define STM32_PCLK2_MAX                 STM32_VOS1_PCLK2_MAX
#define STM32_PCLK3_MAX                 STM32_VOS1_PCLK3_MAX
#define STM32_ADCCLK_MAX                STM32_VOS1_ADCCLK_MAX

#define STM32_0WS_THRESHOLD             STM32_VOS1_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD             STM32_VOS1_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD             STM32_VOS1_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD             STM32_VOS1_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD             STM32_VOS1_4WS_THRESHOLD
#define STM32_5WS_THRESHOLD             STM32_VOS1_5WS_THRESHOLD
#define STM32_6WS_THRESHOLD             STM32_VOS1_6WS_THRESHOLD
#define STM32_7WS_THRESHOLD             STM32_VOS1_7WS_THRESHOLD
#define STM32_8WS_THRESHOLD             STM32_VOS1_8WS_THRESHOLD

#elif (STM32_PWR_VOSCR & STM32_VOS_MASK) == STM32_VOS_RANGE2
#define STM32_SYSCLK_MAX                STM32_VOS2_SYSCLK_MAX
#define STM32_HSECLK_MAX                STM32_VOS2_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX            STM32_VOS2_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                STM32_VOS2_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN            STM32_VOS2_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                STM32_VOS2_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX            STM32_VOS2_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                STM32_VOS2_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN            STM32_VOS2_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                 STM32_VOS2_PLLIN_MAX
#define STM32_PLLIN_MIN                 STM32_VOS2_PLLIN_MIN
#define STM32_PLLVCO_MAX                STM32_VOS2_PLLVCO_MAX
#define STM32_PLLVCO_MIN                STM32_VOS2_PLLVCO_MIN
#define STM32_PLLP_MAX                  STM32_VOS2_PLLP_MAX
#define STM32_PLLP_MIN                  STM32_VOS2_PLLP_MIN
#define STM32_PLLQ_MAX                  STM32_VOS2_PLLQ_MAX
#define STM32_PLLQ_MIN                  STM32_VOS2_PLLQ_MIN
#define STM32_PLLR_MAX                  STM32_VOS2_PLLR_MAX
#define STM32_PLLR_MIN                  STM32_VOS2_PLLR_MIN
#define STM32_PCLK1_MAX                 STM32_VOS2_PCLK1_MAX
#define STM32_PCLK2_MAX                 STM32_VOS2_PCLK2_MAX
#define STM32_ADCCLK_MAX                STM32_VOS2_ADCCLK_MAX

#define STM32_0WS_THRESHOLD             STM32_VOS2_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD             STM32_VOS2_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD             STM32_VOS2_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD             STM32_VOS2_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD             STM32_VOS2_4WS_THRESHOLD
#define STM32_5WS_THRESHOLD             STM32_VOS2_5WS_THRESHOLD
#define STM32_6WS_THRESHOLD             STM32_VOS2_6WS_THRESHOLD
#define STM32_7WS_THRESHOLD             STM32_VOS2_7WS_THRESHOLD
#define STM32_8WS_THRESHOLD             STM32_VOS2_8WS_THRESHOLD

#elif (STM32_PWR_VOSCR & STM32_VOS_MASK) == STM32_VOS_RANGE3
#define STM32_SYSCLK_MAX                STM32_VOS3_SYSCLK_MAX
#define STM32_HSECLK_MAX                STM32_VOS3_HSECLK_MAX
#define STM32_HSECLK_BYP_MAX            STM32_VOS3_HSECLK_BYP_MAX
#define STM32_HSECLK_MIN                STM32_VOS3_HSECLK_MIN
#define STM32_HSECLK_BYP_MIN            STM32_VOS3_HSECLK_BYP_MIN
#define STM32_LSECLK_MAX                STM32_VOS3_LSECLK_MAX
#define STM32_LSECLK_BYP_MAX            STM32_VOS3_LSECLK_BYP_MAX
#define STM32_LSECLK_MIN                STM32_VOS3_LSECLK_MIN
#define STM32_LSECLK_BYP_MIN            STM32_VOS3_LSECLK_BYP_MIN
#define STM32_PLLIN_MAX                 STM32_VOS3_PLLIN_MAX
#define STM32_PLLIN_MIN                 STM32_VOS3_PLLIN_MIN
#define STM32_PLLVCO_MAX                STM32_VOS3_PLLVCO_MAX
#define STM32_PLLVCO_MIN                STM32_VOS3_PLLVCO_MIN
#define STM32_PLLP_MAX                  STM32_VOS3_PLLP_MAX
#define STM32_PLLP_MIN                  STM32_VOS3_PLLP_MIN
#define STM32_PLLQ_MAX                  STM32_VOS3_PLLQ_MAX
#define STM32_PLLQ_MIN                  STM32_VOS3_PLLQ_MIN
#define STM32_PLLR_MAX                  STM32_VOS3_PLLR_MAX
#define STM32_PLLR_MIN                  STM32_VOS3_PLLR_MIN
#define STM32_PCLK1_MAX                 STM32_VOS3_PCLK1_MAX
#define STM32_PCLK2_MAX                 STM32_VOS3_PCLK2_MAX
#define STM32_ADCCLK_MAX                STM32_VOS3_ADCCLK_MAX

#define STM32_0WS_THRESHOLD             STM32_VOS3_0WS_THRESHOLD
#define STM32_1WS_THRESHOLD             STM32_VOS3_1WS_THRESHOLD
#define STM32_2WS_THRESHOLD             STM32_VOS3_2WS_THRESHOLD
#define STM32_3WS_THRESHOLD             STM32_VOS3_3WS_THRESHOLD
#define STM32_4WS_THRESHOLD             STM32_VOS3_4WS_THRESHOLD
#define STM32_5WS_THRESHOLD             STM32_VOS3_5WS_THRESHOLD
#define STM32_6WS_THRESHOLD             STM32_VOS3_6WS_THRESHOLD
#define STM32_7WS_THRESHOLD             STM32_VOS3_7WS_THRESHOLD
#define STM32_8WS_THRESHOLD             STM32_VOS3_8WS_THRESHOLD

#else
#error "invalid STM32_VOS value specified"
#endif

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
#if (STM32_SW         == STM32_SW_PLL1P)         ||                         \
    (STM32_MCO1SEL    == STM32_MCO1SEL_PLL1P)    ||                         \
    (STM32_MCO2SEL    == STM32_MCO2SEL_PLL1P)    ||                         \
    (STM32_SPI1SEL    == STM32_SPI1SEL_PLL1Q)    ||                         \
    (STM32_SPI2SEL    == STM32_SPI2SEL_PLL1Q)    ||                         \
    (STM32_SPI3SEL    == STM32_SPI3SEL_PLL1Q)    ||                         \
    (STM32_OSPISEL    == STM32_OSPISEL_PLL1Q)    ||                         \
    (STM32_USBSEL     == STM32_USBSEL_PLL1Q)     ||                         \
    (STM32_SDMMC1SEL  == STM32_SDMMC1SEL_PLL1Q)  ||                         \
    (STM32_SDMMC2SEL  == STM32_SDMMC2SEL_PLL1Q)  ||                         \
    (STM32_RNGSEL     == STM32_RNGSEL_PLL1Q)     ||                         \
    (STM32_CECSEL     == STM32_CECSEL_PLL1Q)     ||                         \
    (STM32_FDCANSEL   == STM32_FDCANSEL_PLL1Q)   ||                         \
    (STM32_SAI1SEL    == STM32_SAI1SEL_PLL1Q)    ||                         \
    (STM32_SAI2SEL    == STM32_SAI2SEL_PLL1Q)    ||                         \
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
#if (STM32_MCO2SEL    == STM32_MCO2SEL_PLL2P)    ||                         \
    (STM32_USART1SEL  == STM32_USART1SEL_PLL2Q)  ||                         \
    (STM32_USART2SEL  == STM32_USART2SEL_PLL2Q)  ||                         \
    (STM32_USART3SEL  == STM32_USART3SEL_PLL2Q)  ||                         \
    (STM32_UART4SEL   == STM32_UART4SEL_PLL2Q)   ||                         \
    (STM32_UART5SEL   == STM32_UART5SEL_PLL2Q)   ||                         \
    (STM32_USART6SEL  == STM32_USART6SEL_PLL2Q)  ||                         \
    (STM32_UART7SEL   == STM32_UART7SEL_PLL2Q)   ||                         \
    (STM32_UART8SEL   == STM32_UART8SEL_PLL2Q)   ||                         \
    (STM32_UART9SEL   == STM32_UART9SEL_PLL2Q)   ||                         \
    (STM32_USART10SEL == STM32_USART10SEL_PLL2Q) ||                         \
    (STM32_USART11SEL == STM32_USART11SEL_PLL2Q) ||                         \
    (STM32_UART12SEL  == STM32_UART12SEL_PLL2Q)  ||                         \
    (STM32_LPUART1SEL == STM32_LPUART1SEL_PLL2Q) ||                         \
    (STM32_LPTIM1SEL  == STM32_LPTIM1SEL_PLL2P)  ||                         \
    (STM32_LPTIM2SEL  == STM32_LPTIM2SEL_PLL2P)  ||                         \
    (STM32_LPTIM3SEL  == STM32_LPTIM3SEL_PLL2P)  ||                         \
    (STM32_LPTIM4SEL  == STM32_LPTIM4SEL_PLL2P)  ||                         \
    (STM32_LPTIM5SEL  == STM32_LPTIM5SEL_PLL2P)  ||                         \
    (STM32_LPTIM6SEL  == STM32_LPTIM6SEL_PLL2P)  ||                         \
    (STM32_SPI1SEL    == STM32_SPI1SEL_PLL2P)    ||                         \
    (STM32_SPI2SEL    == STM32_SPI2SEL_PLL2P)    ||                         \
    (STM32_SPI3SEL    == STM32_SPI3SEL_PLL2P)    ||                         \
    (STM32_SPI4SEL    == STM32_SPI4SEL_PLL2P)    ||                         \
    (STM32_SPI5SEL    == STM32_SPI5SEL_PLL2P)    ||                         \
    (STM32_SPI6SEL    == STM32_SPI6SEL_PLL2P)    ||                         \
    (STM32_OSPISEL    == STM32_OSPISEL_PLL2R )   ||                         \
    (STM32_SDMMC1SEL  == STM32_SDMMC1SEL_PLL2R)  ||                         \
    (STM32_SDMMC2SEL  == STM32_SDMMC2SEL_PLL2R)  ||                         \
    (STM32_ADCDACSEL  == STM32_ADCDACSEL_PLL2R)  ||                         \
    (STM32_FDCANSEL   == STM32_FDCANSEL_PLL2Q)   ||                         \
    (STM32_SAI1SEL    == STM32_SAI1SEL_PLL2P)    ||                         \
    (STM32_SAI2SEL    == STM32_SAI2SEL_PLL2P)    ||                         \
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
#if (STM32_USART1SEL  == STM32_USART1SEL_PLL3Q)  ||                         \
    (STM32_USART2SEL  == STM32_USART2SEL_PLL3Q)  ||                         \
    (STM32_USART3SEL  == STM32_USART3SEL_PLL3Q)  ||                         \
    (STM32_UART4SEL   == STM32_UART4SEL_PLL3Q)   ||                         \
    (STM32_UART5SEL   == STM32_UART5SEL_PLL3Q)   ||                         \
    (STM32_USART6SEL  == STM32_USART6SEL_PLL3Q)  ||                         \
    (STM32_UART7SEL   == STM32_UART7SEL_PLL3Q)   ||                         \
    (STM32_UART8SEL   == STM32_UART8SEL_PLL3Q)   ||                         \
    (STM32_UART9SEL   == STM32_UART9SEL_PLL3Q)   ||                         \
    (STM32_USART10SEL == STM32_USART10SEL_PLL3Q) ||                         \
    (STM32_USART11SEL == STM32_USART11SEL_PLL3Q) ||                         \
    (STM32_UART12SEL  == STM32_UART12SEL_PLL3Q)  ||                         \
    (STM32_LPUART1SEL == STM32_LPUART1SEL_PLL3Q) ||                         \
    (STM32_LPTIM1SEL  == STM32_LPTIM1SEL_PLL3R)  ||                         \
    (STM32_LPTIM2SEL  == STM32_LPTIM2SEL_PLL3R)  ||                         \
    (STM32_LPTIM3SEL  == STM32_LPTIM3SEL_PLL3R)  ||                         \
    (STM32_LPTIM4SEL  == STM32_LPTIM4SEL_PLL3R)  ||                         \
    (STM32_LPTIM5SEL  == STM32_LPTIM5SEL_PLL3R)  ||                         \
    (STM32_LPTIM6SEL  == STM32_LPTIM6SEL_PLL3R)  ||                         \
    (STM32_SPI1SEL    == STM32_SPI1SEL_PLL3P)    ||                         \
    (STM32_SPI2SEL    == STM32_SPI2SEL_PLL3P)    ||                         \
    (STM32_SPI3SEL    == STM32_SPI3SEL_PLL3P)    ||                         \
    (STM32_SPI4SEL    == STM32_SPI4SEL_PLL3P)    ||                         \
    (STM32_SPI5SEL    == STM32_SPI5SEL_PLL3P)    ||                         \
    (STM32_SPI6SEL    == STM32_SPI6SEL_PLL3P)    ||                         \
    (STM32_MCO1SEL    == STM32_MCO1SEL_PLL1P)    ||                         \
    (STM32_MCO2SEL    == STM32_MCO2SEL_PLL1P)    ||                         \
    (STM32_SPI1SEL    == STM32_SPI1SEL_PLL1Q)    ||                         \
    (STM32_SPI2SEL    == STM32_SPI2SEL_PLL1Q)    ||                         \
    (STM32_SPI3SEL    == STM32_SPI3SEL_PLL1Q)    ||                         \
    (STM32_USBSEL     == STM32_USBSEL_PLL3Q)     ||                         \
    (STM32_I2C1SEL    == STM32_I2C1SEL_PLL3R)    ||                         \
    (STM32_I2C2SEL    == STM32_I2C2SEL_PLL3R)    ||                         \
    (STM32_I2C3SEL    == STM32_I2C3SEL_PLL3R)    ||                         \
    (STM32_I2C4SEL    == STM32_I2C4SEL_PLL3R)    ||                         \
    (STM32_I3C1SEL    == STM32_I3C1SEL_PLL3R)    ||                         \
    (STM32_SAI1SEL    == STM32_SAI1SEL_PLL3P)    ||                         \
    (STM32_SAI2SEL    == STM32_SAI2SEL_PLL3P)    ||                         \
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

  #if STM32_ACTIVATE_PLL1 && (STM32_PLL1SRC == STM32_PLL1SRC_CSI))
    #error "CSI not enabled, required by STM32_PLL1SRC"
  #endif
  #if STM32_ACTIVATE_PLL2 && (STM32_PLL1SRC == STM32_PLL2SRC_CSI))
    #error "CSI not enabled, required by STM32_PLL2SRC"
  #endif
  #if STM32_ACTIVATE_PLL3 && (STM32_PLL1SRC == STM32_PLL3SRC_CSI))
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

  #if STM32_PERSEL == STM32_PERSEL_CSI
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

  #if STM32_CECSEL == STM32_CECSEL_HSI48
    #error "HSI48 not enabled, required by STM32_CECSEL"
  #endif

#endif /* !STM32_HSI48_ENABLED */

/*
 * HSI related checks.
 */
#if STM32_HSI_ENABLED
#else /* !STM32_HSI_ENABLED */

  #if STM32_ACTIVATE_PLL1 && (STM32_PLL1SRC == STM32_PLL1SRC_HSI))
    #error "HSI not enabled, required by STM32_PLL1SRC"
  #endif
  #if STM32_ACTIVATE_PLL2 && (STM32_PLL1SRC == STM32_PLL2SRC_HSI))
    #error "HSI not enabled, required by STM32_PLL2SRC"
  #endif
  #if STM32_ACTIVATE_PLL3 && (STM32_PLL1SRC == STM32_PLL3SRC_HSI))
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

  #if STM32_PERSEL == STM32_PERSEL_HSI
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

  #if STM32_ACTIVATE_PLL1 && (STM32_PLL1SRC == STM32_PLL1SRC_HSE))
    #error "HSE not enabled, required by STM32_PLL1SRC"
  #endif
  #if STM32_ACTIVATE_PLL2 && (STM32_PLL1SRC == STM32_PLL2SRC_HSE))
    #error "HSE not enabled, required by STM32_PLL2SRC"
  #endif
  #if STM32_ACTIVATE_PLL3 && (STM32_PLL1SRC == STM32_PLL3SRC_HSE))
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
 * @brief   PLL input clock frequency.
 */
#if (STM32_PLLSRC == STM32_PLLSRC_HSE) || defined(__DOXYGEN__)
  #define STM32_PLLCLKIN            (STM32_HSECLK / STM32_PLLM_VALUE)

#elif STM32_PLLSRC == STM32_PLLSRC_HSI
  #define STM32_PLLCLKIN            (STM32_HSICLK / STM32_PLLM_VALUE)

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
    (STM32_ADC12SEL == STM32_ADC12SEL_PLLPCLK) ||                           \
    (STM32_ADC345SEL == STM32_ADC345SEL_PLLPCLK) ||                         \
    (STM32_SAI1SEL == STM32_SAI1SEL_PLLQCLK) ||                             \
    (STM32_I2S23SEL == STM32_I2S23SEL_PLLQCLK) ||                           \
    (STM32_FDCANSEL == STM32_FDCANSEL_PLLQCLK) ||                           \
    (STM32_CLK48SEL == STM32_CLK48SEL_PLLQCLK) ||                           \
    (STM32_OSPISEL == STM32_OSPISEL_PLLQCLK) ||                             \
    defined(__DOXYGEN__)
  /**
   * @brief   PLL activation flag.
   */
  #define STM32_ACTIVATE_PLL        TRUE
#else

  #define STM32_ACTIVATE_PLL        FALSE
#endif

/**
 * @brief   STM32_PLLPEN field.
 */
#if (STM32_ADC12SEL == STM32_ADC12SEL_PLLPCLK) ||                           \
    (STM32_ADC345SEL == STM32_ADC345SEL_PLLPCLK) ||                         \
    defined(__DOXYGEN__)
  #define STM32_PLLPEN              (1 << 16)

#else
  #define STM32_PLLPEN              (0 << 16)
#endif

/**
 * @brief   STM32_PLLQEN field.
 */
#if (STM32_OSPISEL == STM32_OSPISEL_PLLQCLK) ||                             \
    (STM32_FDCANSEL == STM32_FDCANSEL_PLLQCLK) ||                           \
    (STM32_CLK48SEL == STM32_CLK48SEL_PLLQCLK) ||                           \
    (STM32_SAI1SEL == STM32_SAI1SEL_PLLQCLK) ||                             \
    (STM32_I2S23SEL == STM32_I2S23SEL_PLLQCLK) ||                           \
    defined(__DOXYGEN__)
  #define STM32_PLLQEN              (1 << 20)

#else
  #define STM32_PLLQEN              (0 << 20)
#endif

/**
 * @brief   STM32_PLLREN field.
 */
#if (STM32_SW == STM32_SW_PLLRCLK) ||                                       \
    (STM32_MCOSEL == STM32_MCOSEL_PLLRCLK) ||                               \
    defined(__DOXYGEN__)
  #define STM32_PLLREN              (1 << 24)

#else
  #define STM32_PLLREN              (0 << 24)
#endif

/* Inclusion of PLL-related checks and calculations.*/
#include <stm32_pll.inc>

/**
 * @brief   System clock source.
 */
#if STM32_NO_INIT || defined(__DOXYGEN__)
  #define STM32_SYSCLK              STM32_HSICLK

#elif (STM32_SW == STM32_SW_HSI)
  #define STM32_SYSCLK              STM32_HSICLK

#elif (STM32_SW == STM32_SW_HSE)
  #define STM32_SYSCLK              STM32_HSECLK

#elif (STM32_SW == STM32_SW_PLLRCLK)
  #define STM32_SYSCLK              STM32_PLL_R_CLKOUT

#else
  #error "invalid STM32_SW value specified"
#endif

/* Bus handlers.*/
#include "stm32_ahb.inc"
#include "stm32_apb1.inc"
#include "stm32_apb2.inc"

/**
 * @brief   MCO divider clock frequency.
 */
#if (STM32_MCOSEL == STM32_MCOSEL_NOCLOCK) || defined(__DOXYGEN__)
  #define STM32_MCODIVCLK           0

#elif STM32_MCOSEL == STM32_MCOSEL_SYSCLK
  #define STM32_MCODIVCLK           STM32_SYSCLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSI
  #define STM32_MCODIVCLK           STM32_HSICLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSE
  #define STM32_MCODIVCLK           STM32_HSECLK

#elif STM32_MCOSEL == STM32_MCOSEL_PLLRCLK
  #define STM32_MCODIVCLK           STM32_PLL_R_CLKOUT

#elif STM32_MCOSEL == STM32_MCOSEL_LSI
  #define STM32_MCODIVCLK           STM32_LSICLK

#elif STM32_MCOSEL == STM32_MCOSEL_LSE
  #define STM32_MCODIVCLK           STM32_LSECLK

#elif STM32_MCOSEL == STM32_MCOSEL_HSI48
  #define STM32_MCODIVCLK           STM32_HSI48CLK

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
#if (STM32_USART1SEL == STM32_USART1SEL_PCLK2) || defined(__DOXYGEN__)
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_PCLK2)

#elif STM32_USART1SEL == STM32_USART1SEL_SYSCLK
  #define STM32_USART1CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_USART1SEL == STM32_USART1SEL_HSI
  #define STM32_USART1CLK           STM32_HSICLK

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

 #elif STM32_USART2SEL == STM32_USART2SEL_SYSCLK
   #define STM32_USART2CLK           hal_lld_get_clock_point(CLK_SYSCLK)

 #elif STM32_USART2SEL == STM32_USART2SEL_HSI
   #define STM32_USART2CLK           STM32_HSICLK

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

 #elif STM32_USART3SEL == STM32_USART3SEL_SYSCLK
   #define STM32_USART3CLK           hal_lld_get_clock_point(CLK_SYSCLK)

 #elif STM32_USART3SEL == STM32_USART3SEL_HSI
   #define STM32_USART3CLK           STM32_HSICLK

 #elif STM32_USART3SEL == STM32_USART3SEL_LSE
   #define STM32_USART3CLK           STM32_LSECLK

 #else
   #error "invalid source selected for USART3 clock"
 #endif

/**
 * @brief   UART4 clock frequency.
 */
#if (STM32_UART4SEL == STM32_UART4SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART4CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART4SEL == STM32_UART4SEL_SYSCLK
  #define STM32_UART4CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_UART4SEL == STM32_UART4SEL_HSI
  #define STM32_UART4CLK           STM32_HSICLK

#elif STM32_UART4SEL == STM32_UART4SEL_LSE
  #define STM32_UART4CLK           STM32_LSECLK

#else
  #error "invalid source selected for UART4 clock"
#endif

/**
 * @brief   UART5 clock frequency.
 */
#if (STM32_UART5SEL == STM32_UART5SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_UART5CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_UART5SEL == STM32_UART5SEL_SYSCLK
  #define STM32_UART5CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_UART5SEL == STM32_UART5SEL_HSI
  #define STM32_UART5CLK           STM32_HSICLK

#elif STM32_UART5SEL == STM32_UART5SEL_LSE
  #define STM32_UART5CLK           STM32_LSECLK

#else
  #error "invalid source selected for UART5 clock"
#endif

/**
 * @brief   LPUART1 clock frequency.
 */
#if (STM32_LPUART1SEL == STM32_LPUART1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_SYSCLK
  #define STM32_LPUART1CLK          hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_HSI
  #define STM32_LPUART1CLK          STM32_HSICLK

#elif STM32_LPUART1SEL == STM32_LPUART1SEL_LSE
  #define STM32_LPUART1CLK          STM32_LSECLK

#else
#error "invalid source selected for LPUART1 clock"
#endif

/**
 * @brief   I2C1 clock frequency.
 */
#if (STM32_I2C1SEL == STM32_I2C1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C1SEL == STM32_I2C1SEL_SYSCLK
  #define STM32_I2C1CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2C1SEL == STM32_I2C1SEL_HSI
  #define STM32_I2C1CLK             STM32_HSICLK

#else
  #error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C2 clock frequency.
 */
#if (STM32_I2C2SEL == STM32_I2C2SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C2CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C2SEL == STM32_I2C2SEL_SYSCLK
  #define STM32_I2C2CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2C2SEL == STM32_I2C2SEL_HSI
  #define STM32_I2C2CLK             STM32_HSICLK

#else
  #error "invalid source selected for I2C1 clock"
#endif

/**
 * @brief   I2C3 clock frequency.
 */
#if (STM32_I2C3SEL == STM32_I2C3SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C3CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C3SEL == STM32_I2C3SEL_SYSCLK
  #define STM32_I2C3CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2C3SEL == STM32_I2C3SEL_HSI
  #define STM32_I2C3CLK             STM32_HSICLK

#else
  #error "invalid source selected for I2C3 clock"
#endif

/**
 * @brief   I2C4 clock frequency.
 */
#if (STM32_I2C4SEL == STM32_I2C4SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_I2C4CLK             hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_I2C4SEL == STM32_I2C4SEL_SYSCLK
  #define STM32_I2C4CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2C4SEL == STM32_I2C4SEL_HSI
  #define STM32_I2C4CLK             STM32_HSICLK

#else
  #error "invalid source selected for I2C4 clock"
#endif

/**
 * @brief   LPTIM1 clock frequency.
 */
#if (STM32_LPTIM1SEL == STM32_LPTIM1SEL_PCLK1) || defined(__DOXYGEN__)
  #define STM32_LPTIM1CLK           hal_lld_get_clock_point(CLK_PCLK1)

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSI
  #define STM32_LPTIM1CLK           STM32_LSICLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_HSI
  #define STM32_LPTIM1CLK           STM32_HSICLK

#elif STM32_LPTIM1SEL == STM32_LPTIM1SEL_LSE
  #define STM32_LPTIM1CLK           STM32_LSECLK

#else
  #error "invalid source selected for LPTIM1 clock"
#endif

/**
 * @brief   SAI1 clock frequency.
 */
#if (STM32_SAI1SEL == STM32_SAI1SEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_SAI1CLK             hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_SAI1SEL == STM32_SAI1SEL_PLLQCLK
  #define STM32_SAI1CLK             hal_lld_get_clock_point(CLK_PLLQCLK)

#elif STM32_SAI1SEL == STM32_SAI1SEL_HSI
  #define STM32_SAI1CLK             STM32_HSICLK

#elif STM32_SAI1SEL == STM32_SAI1SEL_CKIN
  #define STM32_SAI1CLK             0 /* Unknown, would require a board value */

#else
  #error "invalid source selected for SAI1 clock"
#endif

/**
 * @brief   I2S23 clock frequency.
 */
#if (STM32_I2S23SEL == STM32_I2S23SEL_SYSCLK) || defined(__DOXYGEN__)
  #define STM32_I2S23CLK            hal_lld_get_clock_point(CLK_SYSCLK)

#elif STM32_I2S23SEL == STM32_I2S23SEL_PLLPCLK
  #define STM32_I2S23CLK            hal_lld_get_clock_point(CLK_PLLPCLK)

#elif STM32_I2S23SEL == STM32_I2S23SEL_HSI
  #define STM32_I2S23CLK            STM32_HSICLK

#elif STM32_I2S23SEL == STM32_I2S23SEL_CKIN
  #define STM32_I2S23CLK            0 /* Unknown, would require a board value */

#else
  #error "invalid source selected for SAI1 clock"
#endif

/**
 * @brief   FDCAN clock frequency.
 */
#if (STM32_FDCANSEL == STM32_FDCANSEL_HSE) || defined(__DOXYGEN__)
  #define STM32_FDCANCLK            STM32_HSECLK

#elif STM32_FDCANSEL == STM32_FDCANSEL_PLLQCLK
  #define STM32_FDCANCLK            hal_lld_get_clock_point(CLK_PLLQCLK)

#elif STM32_FDCANSEL == STM32_FDCANSEL_PCLK1
  #define STM32_FDCANCLK            hal_lld_get_clock_point(CLK_PCLK1)

#else
  #error "invalid source selected for FDCAN clock"
#endif

/**
 * @brief   48MHz clock frequency.
 */
#if (STM32_CLK48SEL == STM32_CLK48SEL_HSI48) || defined(__DOXYGEN__)
  #define STM32_48CLK               STM32_HSI48CLK

#elif STM32_CLK48SEL == STM32_CLK48SEL_PLLQCLK
  #define STM32_48CLK               hal_lld_get_clock_point(CLK_PLLQCLK)

#else
  #error "invalid source selected for 48MHz clock"
#endif

/**
 * @brief   ADC12 clock frequency.
 */
#if (STM32_ADC12SEL == STM32_ADC12SEL_NOCLK) || defined(__DOXYGEN__)
  #define STM32_ADC12CLK            0

#elif STM32_ADC12SEL == STM32_ADC12SEL_PLLPCLK
  #define STM32_ADC12CLK            hal_lld_get_clock_point(CLK_PLLPCLK)

#elif STM32_ADC12SEL == STM32_ADC12SEL_SYSCLK
  #define STM32_ADC12CLK            hal_lld_get_clock_point(CLK_SYSCLK)

#else
  #error "invalid source selected for ADC clock"
#endif

/**
 * @brief   ADC345 clock frequency.
 */
#if (STM32_ADC345SEL == STM32_ADC345SEL_NOCLK) || defined(__DOXYGEN__)
  #define STM32_ADC345CLK           0

#elif STM32_ADC345SEL == STM32_ADC345SEL_PLLPCLK
  #define STM32_ADC345CLK           hal_lld_get_clock_point(CLK_PLLPCLK)

#elif STM32_ADC345SEL == STM32_ADC345SEL_SYSCLK
  #define STM32_ADC345CLK           hal_lld_get_clock_point(CLK_SYSCLK)

#else
  #error "invalid source selected for ADC clock"
#endif

/**
 * @brief   TIMP1CLK clock frequency.
 */
#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
  #define STM32_TIMP1CLK            (STM32_PCLK1 * 1)
#else
  #define STM32_TIMP1CLK            (STM32_PCLK1 * 2)
#endif

/**
 * @brief   TIMP2CLK clock frequency.
 */
#if (STM32_PPRE2 == STM32_PPRE2_DIV1) || defined(__DOXYGEN__)
  #define STM32_TIMP2CLK            (STM32_PCLK2 * 1)
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
 * @brief   RNG clock point.
 */
#define STM32_RNGCLK                STM32_48CLK

/**
 * @brief   USB clock point.
 */
#define STM32_USBCLK                STM32_48CLK

/**
 * @brief   Voltage boost settings.
 */
#if STM32_PWR_BOOST || defined(__DOXYGEN__)
#define STM32_CR5BITS               PWR_CR5_R1MODE
#else
#define STM32_CR5BITS               0U
#endif

/**
 * @brief   Flash settings.
 */
#if (STM32_HCLK <= STM32_0WS_THRESHOLD) || defined(__DOXYGEN__)
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_0WS

#elif STM32_HCLK <= STM32_1WS_THRESHOLD
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_1WS

#elif STM32_HCLK <= STM32_2WS_THRESHOLD
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_2WS

#elif STM32_HCLK <= STM32_3WS_THRESHOLD
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_3WS

#elif STM32_HCLK <= STM32_4WS_THRESHOLD
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_4WS

#else
  #define STM32_FLASHBITS           FLASH_ACR_LATENCY_5WS
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
  ((clkpt) == CLK_SYSCLK   ? STM32_SYSCLK        :                          \
   (clkpt) == CLK_PLLPCLK  ? STM32_PLL_P_CLKOUT  :                          \
   (clkpt) == CLK_PLLQCLK  ? STM32_PLL_Q_CLKOUT  :                          \
   (clkpt) == CLK_PLLRCLK  ? STM32_PLL_R_CLKOUT  :                          \
   (clkpt) == CLK_HCLK     ? STM32_HCLK          :                          \
   (clkpt) == CLK_PCLK1    ? STM32_PCLK1         :                          \
   (clkpt) == CLK_PCLK1TIM ? STM32_TIMP1CLK      :                          \
   (clkpt) == CLK_PCLK2    ? STM32_PCLK2         :                          \
   (clkpt) == CLK_PCLK2TIM ? STM32_TIMP2CLK      :                          \
   (clkpt) == CLK_MCO      ? STM32_MCOCLK        :                          \
   0U)
#endif /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

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
