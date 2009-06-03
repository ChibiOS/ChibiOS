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
 * @file ports/ARM-LPC214x/ioports_lld.h
 * @brief LPC214x FIO low level driver
 * @addtogroup LPC214x_IOPORTS
 * @{
 */

#ifndef _IOPORTS_LLD_H_
#define _IOPORTS_LLD_H_

#ifndef _LPC214X_H_
#include "lpc214x.h"
#endif

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
 */
typedef FIO * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief FIO port 0 identifier.
 */
#define IOPORT_A        FIO0Base

/**
 * @brief FIO port 1 identifier.
 */
#define IOPORT_B        FIO1Base

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, please put them in a file named ioports_lld.c if so.           */
/*===========================================================================*/

/**
 * @brief FIO subsystem initialization.
 * @details Enables the access through the fast registers.
 */
#define ioport_init_lld() {                                             \
  SCS = 3;                                                              \
}

/**
 * @brief Writes a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_write_lld(port, bits) ((port)->FIO_PIN = (bits))

/**
 * @brief Reads an I/O port.
 *
 * @param[in] port the port identifier
 * @return the port bits
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_read_lld(port) ((port)->FIO_PIN)

/**
 * @brief Sets a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_set_lld(port, bits) ((port)->FIO_SET = (bits))

/**
 * @brief Clears a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_clear_lld(port, bits) ((port)->FIO_CLR = (bits))

/**
 * @brief Toggles a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be XORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define ioport_toggle_lld(port, bits) ((port)->FIO_PIN ^= (bits))

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
  (port)->FIO_MASK = (bus)->bus_mask;                                   \
  (port)->FIO_PIN = (bits) << (bus)->bus_offset;                        \
  (port)->FIO_MASK = 0;                                                 \
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
  (((bus)->bus_portid->FIO_PIN & (bus)->bus_mask) >> (bus)->bus_offset)

/**
 * @brief FIO port setup.
 * @details This function initializes a FIO port, note that this functionality
 *          is LPC214x specific and non portable.
 */
#define ioport_lpc214x_set_direction_lld(port, dir) {                   \
  (port)->FIO_DIR = (dir);                                              \
}

#endif /* _IOPORTS_LLD_H_ */

/** @} */
