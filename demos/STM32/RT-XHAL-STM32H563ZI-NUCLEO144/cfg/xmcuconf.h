/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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

#ifndef XMCUCONF_H
#define XMCUCONF_H

/*
 * STM32H5xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define __STM32H5xx_XMCUCONF__
#define __STM32H562_XMCUCONF__
#define __STM32H563_XMCUCONF__
#define __STM32H573_XMCUCONF__

/*
 * HAL driver general settings.
 */
#define STM32_NO_INIT                       FALSE
#define STM32_CLOCK_DYNAMIC                 FALSE

/*
 * ICache settings.
 */
#define STM32_ICACHE_CR                     (ICACHE_CR_EN)
#define STM32_ICACHE_CRR0                   (0U)
#define STM32_ICACHE_CRR1                   (0U)
#define STM32_ICACHE_CRR2                   (0U)
#define STM32_ICACHE_CRR3                   (0U)

/*
 * PWR settings.
 */
#define STM32_PWR_VOSCR                     STM32_VOS_RANGE0
#define STM32_PWR_BDCR                      (0U)
#define STM32_PWR_UCPDR                     (0U)
#define STM32_PWR_SCCR                      (0U)
#define STM32_PWR_VMCR                      (0U)
#define STM32_PWR_USBSCR                    (0U)
#define STM32_PWR_WUCR                      (0U)
#define STM32_PWR_IORETR                    (0U)
#define STM32_PWR_SECCFGR                   (0U)
#define STM32_PWR_PRIVCFGR                  (0U)

/*
 * Clock settings.
 */
#define STM32_HSI_ENABLED                   TRUE
#define STM32_HSIDIV_VALUE                  2
#define STM32_HSI48_ENABLED                 FALSE
#define STM32_CSI_ENABLED                   FALSE
#define STM32_HSE_ENABLED                   TRUE
#define STM32_LSI_ENABLED                   FALSE
#define STM32_LSE_ENABLED                   FALSE
#define STM32_SW                            STM32_SW_PLL1P
#define STM32_PLL1SRC                       STM32_PLL1SRC_HSE
#define STM32_PLL1M_VALUE                   4
#define STM32_PLL1N_VALUE                   250
#define STM32_PLL1P_VALUE                   2
#define STM32_PLL1Q_VALUE                   2
#define STM32_PLL1R_VALUE                   2
#define STM32_PLL2SRC                       STM32_PLL2SRC_HSE
#define STM32_PLL2M_VALUE                   4
#define STM32_PLL2N_VALUE                   200
#define STM32_PLL2P_VALUE                   2
#define STM32_PLL2Q_VALUE                   2
#define STM32_PLL2R_VALUE                   2
#define STM32_PLL3SRC                       STM32_PLL3SRC_HSE
#define STM32_PLL3M_VALUE                   4
#define STM32_PLL3N_VALUE                   240
#define STM32_PLL3P_VALUE                   2
#define STM32_PLL3Q_VALUE                   10
#define STM32_PLL3R_VALUE                   2
#define STM32_HPRE                          STM32_HPRE_DIV1
#define STM32_PPRE1                         STM32_PPRE1_DIV1
#define STM32_PPRE2                         STM32_PPRE2_DIV1
#define STM32_PPRE3                         STM32_PPRE3_DIV1
#define STM32_STOPWUCK                      STM32_STOPWUCK_HSI
#define STM32_STOPKERWUCK                   STM32_STOPKERWUCK_HSI
#define STM32_RTCPRE_VALUE                  STM32_RTCPRE_NOCLOCK
#define STM32_MCO1SEL                       STM32_MCO1SEL_HSI
#define STM32_MCO1PRE_VALUE                 STM32_MCO1PRE_NOCLOCK
#define STM32_MCO2SEL                       STM32_MCO2SEL_SYSCLK
#define STM32_MCO2PRE_VALUE                 STM32_MCO2PRE_NOCLOCK
#define STM32_LSCOSEL                       STM32_LSCOSEL_NOCLOCK

/*
 * Peripherals clock sources.
 */
#define STM32_USART1SEL                     STM32_USART1SEL_PCLK2
#define STM32_USART2SEL                     STM32_USART2SEL_PCLK1
#define STM32_USART3SEL                     STM32_USART3SEL_PCLK1
#define STM32_UART4SEL                      STM32_UART4SEL_PCLK1
#define STM32_UART5SEL                      STM32_UART5SEL_PCLK1
#define STM32_USART6SEL                     STM32_USART6SEL_PCLK1
#define STM32_UART7SEL                      STM32_UART7SEL_PCLK1
#define STM32_UART8SEL                      STM32_UART8SEL_PCLK1
#define STM32_UART9SEL                      STM32_UART9SEL_PCLK1
#define STM32_USART10SEL                    STM32_USART10SEL_PCLK1
#define STM32_USART11SEL                    STM32_USART11SEL_PCLK1
#define STM32_UART12SEL                     STM32_UART12SEL_PCLK1
#define STM32_LPUART1SEL                    STM32_LPUART1SEL_PCLK3
#define STM32_TIMICSEL                      STM32_TIMICSEL_NOCLK
#define STM32_LPTIM1SEL                     STM32_LPTIM1SEL_PCLK3
#define STM32_LPTIM2SEL                     STM32_LPTIM2SEL_PCLK1
#define STM32_LPTIM3SEL                     STM32_LPTIM3SEL_PCLK3
#define STM32_LPTIM4SEL                     STM32_LPTIM4SEL_PCLK3
#define STM32_LPTIM5SEL                     STM32_LPTIM5SEL_PCLK3
#define STM32_LPTIM6SEL                     STM32_LPTIM6SEL_PCLK3
#define STM32_SPI1SEL                       STM32_SPI1SEL_PLL1Q
#define STM32_SPI2SEL                       STM32_SPI2SEL_PLL1Q
#define STM32_SPI3SEL                       STM32_SPI3SEL_PLL1Q
#define STM32_SPI4SEL                       STM32_SPI4SEL_PCLK2
#define STM32_SPI5SEL                       STM32_SPI5SEL_PCLK3
#define STM32_SPI6SEL                       STM32_SPI6SEL_PCLK2
#define STM32_OSPISEL                       STM32_OSPISEL_HCLK4
#define STM32_SYSTICKSEL                    STM32_SYSTICKSEL_HCLKDIV8
#define STM32_USBSEL                        STM32_USBSEL_NOCLOCK
#define STM32_SDMMC1SEL                     STM32_SDMMC1SEL_PLL1Q
#define STM32_SDMMC2SEL                     STM32_SDMMC2SEL_PLL1Q
#define STM32_I2C1SEL                       STM32_I2C1SEL_PCLK1
#define STM32_I2C2SEL                       STM32_I2C2SEL_PCLK1
#define STM32_I2C3SEL                       STM32_I2C3SEL_PCLK3
#define STM32_I2C4SEL                       STM32_I2C4SEL_PCLK3
#define STM32_I3C1SEL                       STM32_I3C1SEL_PCLK1
#define STM32_ADCDACSEL                     STM32_ADCDACSEL_HCLK
#define STM32_DACSEL                        STM32_DACSEL_IGNORE
#define STM32_RNGSEL                        STM32_RNGSEL_IGNORE
#define STM32_CECSEL                        STM32_CECSEL_IGNORE
#define STM32_FDCANSEL                      STM32_FDCANSEL_IGNORE
#define STM32_SAI1SEL                       STM32_SAI1SEL_PLL1Q
#define STM32_SAI2SEL                       STM32_SAI2SEL_PLL1Q
#define STM32_CKPERSEL                      STM32_CKPERSEL_HSI
#define STM32_RTCSEL                        STM32_RTCSEL_NOCLOCK

/*
 * IRQ system settings.
 */
#define STM32_IRQ_EXTI0_PRIORITY            6
#define STM32_IRQ_EXTI1_PRIORITY            6
#define STM32_IRQ_EXTI2_PRIORITY            6
#define STM32_IRQ_EXTI3_PRIORITY            6
#define STM32_IRQ_EXTI4_PRIORITY            6
#define STM32_IRQ_EXTI5_PRIORITY            6
#define STM32_IRQ_EXTI6_PRIORITY            6
#define STM32_IRQ_EXTI7_PRIORITY            6
#define STM32_IRQ_EXTI8_PRIORITY            6
#define STM32_IRQ_EXTI9_PRIORITY            6
#define STM32_IRQ_EXTI10_PRIORITY           6
#define STM32_IRQ_EXTI11_PRIORITY           6
#define STM32_IRQ_EXTI12_PRIORITY           6
#define STM32_IRQ_EXTI13_PRIORITY           6
#define STM32_IRQ_EXTI14_PRIORITY           6
#define STM32_IRQ_EXTI15_PRIORITY           6

#define STM32_IRQ_TIM1_BRK_PRIORITY         7
#define STM32_IRQ_TIM1_UP_PRIORITY          7
#define STM32_IRQ_TIM1_TRGCO_PRIORITY       7
#define STM32_IRQ_TIM1_CC_PRIORITY          7
#define STM32_IRQ_TIM2_PRIORITY             7
#define STM32_IRQ_TIM3_PRIORITY             7
#define STM32_IRQ_TIM4_PRIORITY             7
#define STM32_IRQ_TIM5_PRIORITY             7
#define STM32_IRQ_TIM6_PRIORITY             7
#define STM32_IRQ_TIM7_PRIORITY             7
#define STM32_IRQ_TIM8_BRK_PRIORITY         7
#define STM32_IRQ_TIM8_UP_PRIORITY          7
#define STM32_IRQ_TIM8_CC_PRIORITY          7
#define STM32_IRQ_TIM8_CC_PRIORITY          7
#define STM32_IRQ_TIM12_PRIORITY            7
#define STM32_IRQ_TIM13_PRIORITY            7
#define STM32_IRQ_TIM14_PRIORITY            7
#define STM32_IRQ_TIM15_PRIORITY            7
#define STM32_IRQ_TIM16_PRIORITY            7
#define STM32_IRQ_TIM17_PRIORITY            7

#define STM32_IRQ_USART1_PRIORITY           12
#define STM32_IRQ_USART2_PRIORITY           12
#define STM32_IRQ_USART3_PRIORITY           12
#define STM32_IRQ_UART4_PRIORITY            12
#define STM32_IRQ_UART5_PRIORITY            12
#define STM32_IRQ_USART6_PRIORITY           12
#define STM32_IRQ_UART7_PRIORITY            12
#define STM32_IRQ_UART8_PRIORITY            12
#define STM32_IRQ_UART9_PRIORITY            12
#define STM32_IRQ_USART10_PRIORITY          12
#define STM32_IRQ_USART11_PRIORITY          12
#define STM32_IRQ_UART12_PRIORITY           12
#define STM32_IRQ_LPUART1_PRIORITY          12

/*
 * SIO driver system settings.
 */
#define STM32_SIO_USE_USART1                FALSE
#define STM32_SIO_USE_USART2                FALSE
#define STM32_SIO_USE_USART3                TRUE
#define STM32_SIO_USE_UART4                 FALSE
#define STM32_SIO_USE_UART5                 FALSE
#define STM32_SIO_USE_USART6                FALSE
#define STM32_SIO_USE_UART7                 FALSE
#define STM32_SIO_USE_UART8                 FALSE
#define STM32_SIO_USE_UART9                 FALSE
#define STM32_SIO_USE_USART10               FALSE
#define STM32_SIO_USE_USART11               FALSE
#define STM32_SIO_USE_UART12                FALSE
#define STM32_SIO_USE_LPUART1               FALSE

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  FALSE
#define STM32_SPI_USE_SPI2                  FALSE
#define STM32_SPI_USE_SPI3                  FALSE
#define STM32_SPI_USE_SPI4                  FALSE
#define STM32_SPI_USE_SPI5                  FALSE
#define STM32_SPI_USE_SPI6                  FALSE
#define STM32_SPI_SPI1_RX_GPDMA_CHANNEL     STM32_GPDMA1_MASK_FIFO2
#define STM32_SPI_SPI1_TX_GPDMA_CHANNEL     STM32_GPDMA1_MASK_FIFO2
#define STM32_SPI_SPI2_RX_GPDMA_CHANNEL     STM32_GPDMA2_MASK_FIFO2
#define STM32_SPI_SPI2_TX_GPDMA_CHANNEL     STM32_GPDMA2_MASK_FIFO2
#define STM32_SPI_SPI3_RX_GPDMA_CHANNEL     STM32_GPDMA1_MASK_FIFO2
#define STM32_SPI_SPI3_TX_GPDMA_CHANNEL     STM32_GPDMA1_MASK_FIFO2
#define STM32_SPI_SPI4_RX_GPDMA_CHANNEL     STM32_GPDMA2_MASK_FIFO2
#define STM32_SPI_SPI4_TX_GPDMA_CHANNEL     STM32_GPDMA2_MASK_FIFO2
#define STM32_SPI_SPI5_RX_GPDMA_CHANNEL     STM32_GPDMA1_MASK_FIFO2
#define STM32_SPI_SPI5_TX_GPDMA_CHANNEL     STM32_GPDMA1_MASK_FIFO2
#define STM32_SPI_SPI6_RX_GPDMA_CHANNEL     STM32_GPDMA2_MASK_FIFO2
#define STM32_SPI_SPI6_TX_GPDMA_CHANNEL     STM32_GPDMA2_MASK_FIFO2
#define STM32_SPI_SPI1_DMA_PRIORITY         1
#define STM32_SPI_SPI2_DMA_PRIORITY         1
#define STM32_SPI_SPI3_DMA_PRIORITY         1
#define STM32_SPI_SPI4_DMA_PRIORITY         1
#define STM32_SPI_SPI5_DMA_PRIORITY         1
#define STM32_SPI_SPI6_DMA_PRIORITY         1
#define STM32_SPI_SPI1_IRQ_PRIORITY         10
#define STM32_SPI_SPI2_IRQ_PRIORITY         10
#define STM32_SPI_SPI3_IRQ_PRIORITY         10
#define STM32_SPI_SPI4_IRQ_PRIORITY         10
#define STM32_SPI_SPI5_IRQ_PRIORITY         10
#define STM32_SPI_SPI6_IRQ_PRIORITY         10
#define STM32_SPI_DMA_ERROR_HOOK(spip)      osalSysHalt("DMA failure")

/*
 * ST driver system settings.
 */
#define STM32_ST_IRQ_PRIORITY               8
#define STM32_ST_USE_TIMER                  2

#endif /* XMCUCONF_H */
