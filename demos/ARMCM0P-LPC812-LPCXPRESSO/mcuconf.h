/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * LPC812 drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the driver
 * is enabled in halconf.h.
 *
 * IRQ priorities:
 * 3...0        Lowest...highest.
 */

/*
 * HAL driver system settings.
 */

/* Default: Run PLL @24MHz from 12MHz IRC
  #define LPC8xx_PLLCLK_SOURCE               SYSPLLCLKSEL_IRCOSC
  #define LPC8xx_SYSPLL_MUL                  4
  #define LPC8xx_SYSPLL_DIV                  4
  #define LPC8xx_MAINCLK_SOURCE              SYSMAINCLKSEL_PLLOUT
  #define LPC8xx_SYSABHCLK_DIV               1
*/

/*run directly from internal 12M osc...*/
#define LPC8xx_MAINCLK_SOURCE              SYSMAINCLKSEL_IRCOSC

/*
 * GPT driver system settings.
 */
/* Defaults:
  #define LPC8xx_GPT_USE_MRT0              TRUE
  #define LPC8xx_GPT_USE_MRT1              FALSE
  #define LPC8xx_GPT_USE_MRT2              FALSE
  #define LPC8xx_GPT_USE_MRT3              FALSE
  #define LPC8xx_GPT_MRT_IRQ_PRIORITY      2
*/

/*
 * PWM driver system settings.
 */

/*
 * SERIAL driver system settings.
 */
/* Defaults:
  #define LPC8xx_SERIAL_USE_UART0            TRUE
  #define LPC8xx_SERIAL_USE_UART1            FALSE
  #define LPC8xx_SERIAL_USE_UART2            FALSE
  #define LPC8xx_SERIAL_UART0_IRQ_PRIORITY   3
  #define LPC8xx_SERIAL_UART1_IRQ_PRIORITY   3
  #define LPC8xx_SERIAL_UART2_IRQ_PRIORITY   3

  #define LPC8xx_SERIAL_U_PCLK              11059200
  #define LPC8xx_SERIAL_UARTCLKDIV          !!Calculated!!
  #define LPC8xx_SERIAL_UARTFRGMUL          !!Calculated!!
  #define LPC8xx_SERIAL_UARTFRGDIV          !!Calculated!!
*/

/* change default baudrate to 9600 */
#define SERIAL_DEFAULT_BITRATE              9600


