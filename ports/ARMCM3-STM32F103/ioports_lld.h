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
 * @file ports/ARMCM3-STM32F103/ioports_lld.h
 * @brief STM32 GPIO low level driver
 * @addtogroup STM32F103_IOPORTS
 * @{
 */

#ifndef _IOPORTS_LLD_H_
#define _IOPORTS_LLD_H_

/*
 * Tricks required to make the TRUE/FALSE declaration inside the library
 * compatible.
 */
#ifndef __STM32F10x_MAP_H
#undef FALSE
#undef TRUE
#include "stm32f10x_map.h"
#define FALSE 0
#define TRUE (!FALSE)
#endif

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief Width, in bits, of an I/O port.
 */
#define IOPORTS_WIDTH 16

/**
 * @brief Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef GPIO_TypeDef * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the STM32   */
/* firmware library.                                                         */
/*===========================================================================*/

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOA) || defined(__DOXYGEN__)
#define IOPORT_A        GPIOA
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOB) || defined(__DOXYGEN__)
#define IOPORT_B        GPIOB
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOC) || defined(__DOXYGEN__)
#define IOPORT_C        GPIOC
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOD) || defined(__DOXYGEN__)
#define IOPORT_D        GPIOD
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOE) || defined(__DOXYGEN__)
#define IOPORT_E        GPIOE
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, please put them in a file named ioports_lld.c if so.           */
/*===========================================================================*/

/**
 * @brief Writes a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_write_lld(port, bits) ((port)->ODR = (bits))

/**
 * @brief Reads an I/O port.
 *
 * @param[in] port the port identifier
 * @return the port bits
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_read_lld(port) ((port)->IDR)

/**
 * @brief Sets a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_set_lld(port, bits) ((port)->BSRR = (bits))

/**
 * @brief Clears a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_clear_lld(port, bits) ((port)->BRR = (bits))

/**
 * @brief Toggles a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be XORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_toggle_lld(port, bits) ((port)->ODR ^= (bits))

/**
 * @brief Writes a value on an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @param[in] bits the bits to be written on the I/O bus. Values exceeding
 *            the bus width are masked so most significant bits are lost.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_writebus_lld(bus, bits) {                                \
  (bus)->bus_portid->BSRR = ((((~(bits)) << (bus)->bus_offset) &        \
                             (bus)->bus_mask) << 16) |                  \
                            (((bits) << (bus)->bus_offset) &            \
                             (bus)->bus_mask);                          \
}

/**
 * @brief Reads a value from an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @return the bus bits
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_readbus_lld(bus) \
  (((bus)->bus_portid->IDR & (bus)->bus_mask) >> (bus)->bus_offset)

#endif /* _IOPORTS_LLD_H_ */

/** @} */
