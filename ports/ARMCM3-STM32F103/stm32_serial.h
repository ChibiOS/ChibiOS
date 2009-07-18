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
 * @file ports/ARMCM3-STM32F103/stm32_serial.h
 * @brief STM32F103 Serial driver macros and structures.
 * @addtogroup STM32F103_SERIAL
 * @{
 */

#ifndef _STM32_SERIAL_H_
#define _STM32_SERIAL_H_

/*
 * Tricks required to make the TRUE/FALSE declaration inside the library
 * compatible.
 */
#ifndef __STM32F10x_H
#undef FALSE
#undef TRUE
#include <stm32f10x.h>
#define FALSE 0
#define TRUE (!FALSE)
#endif

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
 * @details Configuration parameter, at startup the USARTs are configured at
 * this speed.
 * @note It is possible to use @p SetUSART() in order to change the working
 *       parameters at runtime.
 */
#if !defined(DEFAULT_USART_BITRATE) || defined(__DOXYGEN__)
#define DEFAULT_USART_BITRATE 38400
#endif

/**
 * @brief USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note The default is @p FALSE.
 */
#if !defined(USE_STM32_USART1) || defined(__DOXYGEN__)
#define USE_STM32_USART1 FALSE
#endif

/**
 * @brief USART2 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_USART2) || defined(__DOXYGEN__)
#define USE_STM32_USART2 TRUE
#endif

/**
 * @brief USART3 driver enable switch.
 * @details If set to @p TRUE the support for USART3 is included.
 * @note The default is @p FALSE.
 */
#if !defined(USE_STM32_USART3) || defined(__DOXYGEN__)
#define USE_STM32_USART3 FALSE
#endif

/*
 * Extra USARTs definitions here  (missing from the ST header file).
 */
#define USART_CR2_STOP1_BITS    (0 << 12)   /**< @brief CR2 1 stop bit value.*/
#define USART_CR2_STOP0P5_BITS  (1 << 12)   /**< @brief CR2 0.5 stop bit value.*/
#define USART_CR2_STOP2_BITS    (2 << 12)   /**< @brief CR2 2 stop bit value.*/
#define USART_CR2_STOP1P5_BITS  (3 << 12)   /**< @brief CR2 1.5 stop bit value.*/

/** @cond never*/
#if USE_STM32_USART1
extern FullDuplexDriver COM1;
#endif
#if USE_STM32_USART2
extern FullDuplexDriver COM2;
#endif
#if USE_STM32_USART3
extern FullDuplexDriver COM3;
#endif
/** @endcond*/

#ifdef __cplusplus
extern "C" {
#endif
  void serial_init(uint32_t prio1, uint32_t prio2, uint32_t prio3);
  void usart_setup(USART_TypeDef *u, uint32_t speed, uint16_t cr1,
                   uint16_t cr2, uint16_t cr3);
#ifdef __cplusplus
}
#endif

#endif /* _STM32_SERIAL_H_ */

/** @} */
