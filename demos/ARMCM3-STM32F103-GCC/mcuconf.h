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
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
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

/*
 * ADC driver system settings.
 */
#define USE_STM32_ADC1              TRUE
#define STM32_ADC1_DMA_PRIORITY     3
#define STM32_ADC1_IRQ_PRIORITY     5
#define STM32_ADC1_DMA_ERROR_HOOK() chSysHalt()

/*
 * CAN driver system settings.
 */
#define USE_STM32_CAN1              TRUE
#define STM32_CAN1_IRQ_PRIORITY     11

/*
 * PWM driver system settings.
 */
#define USE_STM32_PWM1              TRUE
#define USE_STM32_PWM2              FALSE
#define USE_STM32_PWM3              FALSE
#define USE_STM32_PWM4              FALSE
#define STM32_PWM1_IRQ_PRIORITY     7
#define STM32_PWM2_IRQ_PRIORITY     7
#define STM32_PWM3_IRQ_PRIORITY     7
#define STM32_PWM4_IRQ_PRIORITY     7

/*
 * SERIAL driver system settings.
 */
#define USE_STM32_USART1            FALSE
#define USE_STM32_USART2            TRUE
#define USE_STM32_USART3            FALSE
#if defined(STM32F10X_HD) || defined(STM32F10X_CL)
#define USE_STM32_UART4             FALSE
#define USE_STM32_UART5             FALSE
#endif
#define STM32_USART1_PRIORITY       12
#define STM32_USART2_PRIORITY       12
#define STM32_USART3_PRIORITY       12
#if defined(STM32F10X_HD) || defined(STM32F10X_CL)
#define STM32_UART4_PRIORITY        12
#define STM32_UART5_PRIORITY        12
#endif

/*
 * SPI driver system settings.
 */
#define USE_STM32_SPI1              TRUE
#define USE_STM32_SPI2              TRUE
#define STM32_SPI1_DMA_PRIORITY     2
#define STM32_SPI2_DMA_PRIORITY     2
#define STM32_SPI1_IRQ_PRIORITY     10
#define STM32_SPI2_IRQ_PRIORITY     10
#define STM32_SPI1_DMA_ERROR_HOOK() chSysHalt()
