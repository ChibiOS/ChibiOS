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
 * @file ports/AVR/avr_serial.h
 * @brief AVR Serial driver macros and structures.
 * @addtogroup AVR_SERIAL
 * @{
 */

#ifndef _AVR_SERIAL_H_
#define _AVR_SERIAL_H_

/**
 * @brief Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 * buffers depending on the requirements of your application.
 * @note The default is 32 bytes for both the transmission and receive buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE 32
#endif

/**
 * @brief Default bit rate.
 * @details Configuration parameter, at startup the UARTs are configured at
 * this speed.
 * @note It is possible to use @p SetUART() in order to change the working
 *       parameters at runtime.
 */
#if !defined(DEFAULT_USART_BITRATE) || defined(__DOXYGEN__)
#define DEFAULT_USART_BITRATE 38400
#endif


/**
 * @brief USART0 driver enable switch.
 * @details If set to @p TRUE the support for USART0 is included.
 * @note The default is @p FALSE.
 */
#if !defined(USE_AVR_USART0) || defined(__DOXYGEN__)
#define USE_AVR_USART0 FALSE
#endif

/**
 * @brief USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_AVR_USART1) || defined(__DOXYGEN__)
#define USE_AVR_USART1 TRUE
#endif

/**
 * @brief Macro for baud rate computation.
 * @note Make sure the final baud rate is within tolerance.
 */
#define UBRR(b) ((F_CPU / (b << 4)) - 1)

#ifdef __cplusplus
extern "C" {
#endif
  void serial_init(void);
  void usart0_setup(uint16_t brr, uint8_t csrc);
  void usart1_setup(uint16_t brr, uint8_t csrc);
#ifdef __cplusplus
}
#endif

/** @cond never*/
extern FullDuplexDriver SER1, SER2;
/** @endcond*/

#endif /* _AVR_SERIAL_H_ */

/** @} */
