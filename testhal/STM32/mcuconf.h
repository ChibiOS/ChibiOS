/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/*
 * STM32 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 *
 * IRQ priorities:
 * 0xF0         Lowest, priority level reserved for PENDSV.
 * 0xE0...0x40  Normal IRQs priority levels (0x80 used by SYSTICK).
 * 0x30         Used by SVCALL, do not share.
 * 0x20...0x00  Fast interrupts, can preempt the kernel but cannot use it.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

/*
 * ADC driver system settings.
 */
#define USE_STM32_ADC1              TRUE
#define STM32_ADC1_DMA_PRIORITY     3
#define STM32_ADC1_IRQ_PRIORITY     0x50
#define STM32_ADC1_DMA_ERROR_HOOK() chSysHalt()

/*
 * CAN driver system settings.
 */
#define USE_STM32_CAN1              TRUE
#define STM32_CAN1_IRQ_PRIORITY     0xB0

/*
 * PWM driver system settings.
 */
#define USE_STM32_PWM1              TRUE
#define USE_STM32_PWM2              FALSE
#define USE_STM32_PWM3              FALSE
#define USE_STM32_PWM4              FALSE
#define STM32_PWM1_IRQ_PRIORITY     0x70
#define STM32_PWM2_IRQ_PRIORITY     0x70
#define STM32_PWM3_IRQ_PRIORITY     0x70
#define STM32_PWM4_IRQ_PRIORITY     0x70

/*
 * SERIAL driver system settings.
 */
#define USE_STM32_USART1            FALSE
#define USE_STM32_USART2            TRUE
#define USE_STM32_USART3            FALSE
#define STM32_USART1_PRIORITY       0xC0
#define STM32_USART2_PRIORITY       0xC0
#define STM32_USART3_PRIORITY       0xC0

/*
 * SPI driver system settings.
 */
#define USE_STM32_SPI1              TRUE
#define USE_STM32_SPI2              TRUE
#define STM32_SPI1_DMA_PRIORITY     2
#define STM32_SPI2_DMA_PRIORITY     2
#define STM32_SPI1_IRQ_PRIORITY     0xA0
#define STM32_SPI2_IRQ_PRIORITY     0xA0
#define STM32_SPI1_DMA_ERROR_HOOK() chSysHalt()
