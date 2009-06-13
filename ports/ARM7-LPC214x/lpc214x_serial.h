/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file ports/ARM7-LPC214x/lpc214x_serial.h
 * @brief LPC214x Serial driver macros and structures.
 * @addtogroup LPC214x_SERIAL
 * @{
 */

#ifndef _LPC214x_SERIAL_H_
#define _LPC214x_SERIAL_H_

/**
 * @brief Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 * buffers depending on the requirements of your application.
 * @note The default is 128 bytes for both the transmission and receive buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE 128
#endif

/**
 * @brief Default bit rate.
 * @details Configuration parameter, at startup the UARTs are configured at
 * this speed.
 * @note It is possible to use @p SetUART() in order to change the working
 *       parameters at runtime.
 */
#if !defined(DEFAULT_UART_BITRATE) || defined(__DOXYGEN__)
#define DEFAULT_UART_BITRATE 38400
#endif

/**
 * @brief FIFO preload parameter.
 * @details Configuration parameter, this values defines how many bytes are
 * preloaded in the HW transmit FIFO for each interrupt, the maximum value is
 * 16 the minimum is 2, the value 0 disables the feature.
 * @note An high value reduces the number of interrupts generated but can
 *       also increase the worst case interrupt response time because the
 *       preload loops.
 * @note The value zero disables the feature and reverts to a simpler code
 *       that will generate an interrupt for each output byte but is much
 *       smaller and simpler.
 */
#if !defined(UART_FIFO_PRELOAD) || defined(__DOXYGEN__)
#define UART_FIFO_PRELOAD 16
#endif

/**
 * @brief UART0 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note The default is @p TRUE .
 */
#if !defined(USE_LPC214x_UART0) || defined(__DOXYGEN__)
#define USE_LPC214x_UART0 TRUE
#endif

/**
 * @brief UART1 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_LPC214x_UART1) || defined(__DOXYGEN__)
#define USE_LPC214x_UART1 TRUE
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void serial_init(int vector1, int vector2);
  void uart_setup(UART *u, int speed, int lcr, int fcr);
  CH_IRQ_HANDLER(UART0IrqHandler);
  CH_IRQ_HANDLER(UART1IrqHandler);
#ifdef __cplusplus
}
#endif

/** @cond never*/
extern FullDuplexDriver COM1, COM2;
/** @endcond*/

#endif /* _LPC214x_SERIAL_H_*/

/** @} */
