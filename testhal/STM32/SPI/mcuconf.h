/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * STM32 drivers configuration.
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

/*
 * HAL driver system settings.
 */
#define STM32_SW                            STM32_SW_PLL
#define STM32_PLLSRC                        STM32_PLLSRC_HSE
#define STM32_PLLXTPRE                      STM32_PLLXTPRE_DIV1
#define STM32_PLLMUL_VALUE                  9
#define STM32_HPRE                          STM32_HPRE_DIV1
#define STM32_PPRE1                         STM32_PPRE1_DIV2
#define STM32_PPRE2                         STM32_PPRE2_DIV2
#define STM32_ADCPRE                        STM32_ADCPRE_DIV4
#define STM32_USBPRE                        STM32_USBPRE_DIV1P5
#define STM32_MCO                           STM32_MCO_NOCLOCK

/*
 * ADC driver system settings.
 */
#define STM32_ADC_USE_ADC1                  TRUE
#define STM32_ADC_ADC1_DMA_PRIORITY         3
#define STM32_ADC_ADC1_IRQ_PRIORITY         5
#define STM32_ADC_ADC1_DMA_ERROR_HOOK()     chSysHalt()

/*
 * CAN driver system settings.
 */
#define STM32_CAN_USE_CAN1                  TRUE
#define STM32_CAN_CAN1_IRQ_PRIORITY         11

/*
 * PWM driver system settings.
 */
#define STM32_PWM_USE_TIM1                  TRUE
#define STM32_PWM_USE_TIM2                  FALSE
#define STM32_PWM_USE_TIM3                  FALSE
#define STM32_PWM_USE_TIM4                  FALSE
#define STM32_PWM_USE_TIM5                  FALSE
#define STM32_PWM_TIM1_IRQ_PRIORITY         7
#define STM32_PWM_TIM2_IRQ_PRIORITY         7
#define STM32_PWM_TIM3_IRQ_PRIORITY         7
#define STM32_PWM_TIM4_IRQ_PRIORITY         7
#define STM32_PWM_TIM5_IRQ_PRIORITY         7

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             FALSE
#define STM32_SERIAL_USE_USART2             TRUE
#define STM32_SERIAL_USE_USART3             FALSE
#define STM32_SERIAL_USE_UART4              FALSE
#define STM32_SERIAL_USE_UART5              FALSE
#define STM32_SERIAL_USART1_PRIORITY        12
#define STM32_SERIAL_USART2_PRIORITY        12
#define STM32_SERIAL_USART3_PRIORITY        12
#define STM32_SERIAL_UART4_PRIORITY         12
#define STM32_SERIAL_UART5_PRIORITY         12

/*
 * SPI driver system settings.
 */
#define STM32_SPI_USE_SPI1                  TRUE
#define STM32_SPI_USE_SPI2                  FALSE
#define STM32_SPI_USE_SPI3                  FALSE
#define STM32_SPI_SPI1_DMA_PRIORITY         2
#define STM32_SPI_SPI2_DMA_PRIORITY         2
#define STM32_SPI_SPI3_DMA_PRIORITY         2
#define STM32_SPI_SPI1_IRQ_PRIORITY         10
#define STM32_SPI_SPI2_IRQ_PRIORITY         10
#define STM32_SPI_SPI3_IRQ_PRIORITY         10
#define STM32_SPI_SPI1_DMA_ERROR_HOOK()     chSysHalt()
#define STM32_SPI_SPI2_DMA_ERROR_HOOK()     chSysHalt()
#define STM32_SPI_SPI3_DMA_ERROR_HOOK()     chSysHalt()

/*
 * UART driver system settings.
 */
#define STM32_UART_USE_USART1               FALSE
#define STM32_UART_USE_USART2               TRUE
#define STM32_UART_USE_USART3               FALSE
#define STM32_UART_USART1_IRQ_PRIORITY      12
#define STM32_UART_USART2_IRQ_PRIORITY      12
#define STM32_UART_USART3_IRQ_PRIORITY      12
#define STM32_UART_USART1_DMA_PRIORITY      0
#define STM32_UART_USART2_DMA_PRIORITY      0
#define STM32_UART_USART3_DMA_PRIORITY      0
#define STM32_UART_USART1_DMA_ERROR_HOOK()  chSysHalt()
#define STM32_UART_USART2_DMA_ERROR_HOOK()  chSysHalt()
#define STM32_UART_USART3_DMA_ERROR_HOOK()  chSysHalt()
