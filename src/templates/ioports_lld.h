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
 * @file ioports_lld.h
 * @brief I/O ports low level driver
 * @addtogroup IOPortsLLD
 * @{
 */

#ifndef _IOPORTS_LLD_H_
#define _IOPORTS_LLD_H_

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief Width, in bits, of an I/O port.
 */
#define IOPORTS_WIDTH 32

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
typedef uint32_t ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief First I/O port identifier.
 * @details Low level drivers can define multiple ports, please use this
 *          naming convention.
 */
#define IOPORT_A        0

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
#define ioport_write_lld(port, bits)

/**
 * @brief Reads an I/O port.
 *
 * @param[in] port the port identifier
 * @return the port bits
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_read_lld(port)

/**
 * @brief Sets a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_set_lld(port, bits)

/**
 * @brief Clears a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_clear_lld(port, bits)

/**
 * @brief Toggles a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be XORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_clear_lld(port, bits)

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
#define ioport_writebus_lld(bus, bits)

/**
 * @brief Reads a value from an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @return the bus bits
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_readbus_lld(bus)

#endif /* _IOPORTS_LLD_H_ */

/** @} */
