/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/*
 * LPC17xx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 *
 * IRQ priorities:
 * 7...0        Lowest...highest.
 */

/*
 * HAL driver system settings.
 */
#define LPC17xx_MAINOSC_ENABLE              TRUE
#define LPC17xx_SYSCLK_SELECT               CLKSRCSEL_MAINOSC
#define LPC17xx_MAINPLL_ENABLE              TRUE
#define LPC17xx_MAINPLL_MUL                 30
#define LPC17xx_MAINPLL_PREDIV              1
#define LPC17xx_USBPLL_ENABLE               FALSE
#define LPC17xx_USBPLL_MUL                  4
#define LPC17xx_USBPLL_DIV                  4
#define LPC17xx_CCLK_DIV                    3
#define LPC17xx_PCLK_SELECT                 PCLKSEL_CCLK
#define LPC17xx_CLKOUT_ENABLE               FALSE
#define LPC17xx_CLKOUT_DIV                  4
#define LPC17xx_CLKOUT_SELECT               CLKOUTSEL_CCLK

/*
 * GPT driver system settings.
 */
#define LPC17xx_GPT_USE_TIM0                TRUE
#define LPC17xx_GPT_USE_TIM1                TRUE
#define LPC17xx_GPT_USE_TIM2                TRUE
#define LPC17xx_GPT_USE_TIM3                TRUE
#define LPC17xx_GPT_TIM0_IRQ_PRIORITY       2
#define LPC17xx_GPT_TIM1_IRQ_PRIORITY       6
#define LPC17xx_GPT_TIM2_IRQ_PRIORITY       2
#define LPC17xx_GPT_TIM3_IRQ_PRIORITY       2

/*
 * SERIAL driver system settings.
 */
#define LPC17xx_SERIAL_USE_UART0            TRUE
#define LPC17xx_SERIAL_USE_UART1            FALSE
#define LPC17xx_SERIAL_USE_UART2            FALSE
#define LPC17xx_SERIAL_USE_UART3            FALSE
#define LPC17xx_SERIAL_FIFO_PRELOAD         16
#define LPC17xx_SERIAL_UART0_IRQ_PRIORITY   3
#define LPC17xx_SERIAL_UART1_IRQ_PRIORITY   3
#define LPC17xx_SERIAL_UART2_IRQ_PRIORITY   3
#define LPC17xx_SERIAL_UART3_IRQ_PRIORITY   3

/*
 * I2C driver system settings.
 */
#define LPC17xx_I2C_USE_I2C0                FALSE
#define LPC17xx_I2C_USE_I2C1                TRUE
#define LPC17xx_I2C_USE_I2C2                FALSE
#define LPC17xx_I2C_I2C0_IRQ_PRIORITY       3
#define LPC17xx_I2C_I2C1_IRQ_PRIORITY       3
#define LPC17xx_I2C_I2C2_IRQ_PRIORITY       3

/*
 * SPI driver system settings.
 */
#define LPC17xx_SPI_USE_SSP0                FALSE
#define LPC17xx_SPI_USE_SSP1                FALSE
#define LPC17xx_SPI_SSP0CLKDIV              1
#define LPC17xx_SPI_SSP1CLKDIV              1
#define LPC17xx_SPI_SSP0_IRQ_PRIORITY       5
#define LPC17xx_SPI_SSP1_IRQ_PRIORITY       5

/*
 * RTC driver system settings.
 */
#define LPC17xx_RTC_IS_CALENDAR             TRUE
#define LPC17xx_RTC_USE_ALARM               TRUE
#define LPC17xx_RTC_IRQ_PRIORITY            3

/*
 * CAN driver system settings.
 */
#define LPC17xx_CAN_USE_CAN1                TRUE
#define LPC17xx_CAN_USE_CAN2                FALSE
#define LPC17xx_CAN_IRQ_PRIORITY            11
#define LPC17xx_CAN_USE_LOCAL_SELF_TEST     TRUE
#define LPC17XX_CAN_USE_FILTER              FALSE
