/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

/**
 * @file ports/ARM7-AT91SAM7X/sam7x_serial.h
 * @brief AT91SAM7X Serial driver macros and structures.
 * @addtogroup AT91SAM7X_SERIAL
 * @{
 */

#ifndef _SAM7X_SERIAL_H_
#define _SAM7X_SERIAL_H_

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
#if !defined(AT91SAM7X_UART_BITRATE) || defined(__DOXYGEN__)
#define SAM7X_UART_BITRATE 38400
#endif

/**
 * @brief UART0 driver enable switch.
 * @details If set to 1 the support for USART1 is included.
 * @note The default is 1.
 */
#if !defined(USE_LPC214x_UART0) || defined(__DOXYGEN__)
#define USE_SAM7X_USART0 1
#endif

/**
 * @brief UART1 driver enable switch.
 * @details If set to 1 the support for USART2 is included.
 * @note The default is 1.
 */
#if !defined(USE_LPC214x_UART1) || defined(__DOXYGEN__)
#define USE_SAM7X_USART1 1
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void InitSerial(int prio0, int prio1);
  void SetUSART(AT91PS_USART u, int speed, int mode);
  CH_IRQ_HANDLER(UART0IrqHandler);
  CH_IRQ_HANDLER(UART1IrqHandler);
#ifdef __cplusplus
}
#endif

/** @cond never*/
extern FullDuplexDriver COM1, COM2;
/** @endcond*/

#endif /* _SAM7X_SERIAL_H_ */

/** @} */
