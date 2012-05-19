/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * STM32F0xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 3...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

/*
 * HAL driver system settings.
 */
#define STM32_NO_INIT                       FALSE
#define STM32_PVD_ENABLE                    FALSE
#define STM32_PLS                           STM32_PLS_LEV0
#define STM32_HSI_ENABLED                   TRUE
#define STM32_HSI14_ENABLED                 TRUE
#define STM32_LSI_ENABLED                   TRUE
#define STM32_HSE_ENABLED                   FALSE
#define STM32_LSE_ENABLED                   FALSE
#define STM32_SW                            STM32_SW_PLL
#define STM32_PLLSRC                        STM32_PLLSRC_HSI
#define STM32_PLLXTPRE                      STM32_PLLXTPRE_DIV1
#define STM32_PLLMUL_VALUE                  6
#define STM32_HPRE                          STM32_HPRE_DIV1
#define STM32_PPRE                          STM32_PPRE_DIV1
#define STM32_ADCSW                         STM32_ADCSW_HSI14
#define STM32_ADCPRE                        STM32_ADCPRE_DIV4
#define STM32_MCOSEL                        STM32_MCOSEL_NOCLOCK
#define STM32_ADCPRE                        STM32_ADCPRE_DIV4
#define STM32_ADCSW                         STM32_ADCSW_HSI14
#define STM32_CECSW                         STM32_CECSW_HSI
#define STM32_I2C1SW                        STM32_I2C1SW_HSI
#define STM32_USART1SW                      STM32_USART1SW_PCLK
#define STM32_RTCSEL                        STM32_RTCSEL_LSI

/*
 * SERIAL driver system settings.
 */
#define STM32_SERIAL_USE_USART1             TRUE
#define STM32_SERIAL_USE_USART2             FALSE
#define STM32_SERIAL_USART1_PRIORITY        3
#define STM32_SERIAL_USART2_PRIORITY        3
