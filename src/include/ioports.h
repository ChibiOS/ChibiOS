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
 * @file ioports.h
 * @brief I/O ports
 * @addtogroup IOPorts
 * @{
 */

#ifndef _IOPORTS_H_
#define _IOPORTS_H_

#ifndef _IOPORTS_LLD_H_
#include "ioports_lld.h"
#endif

/**
 * @brief Port bit helper macro.
 * @details This macro calculates the mask of a bit within a port.
 */
#define IOPORT_BIT(n)   ((ioportmask_t)(1 << (n)))

/**
 * @brief Port bus mask helper macro.
 * @details This macro calculates the proper bus mask starting from the width
 *          and the offset.
 *
 * @param[in] width the width, in bits, of the I/O bus
 * @param[in] offset the offset, within the port, of the I/O bus. The offset
 *            must be specified as offset from the least significant bit.
 */
#define IOPORT_BUS_MASK(width, offset) \
  ((ioportmask_t)(((1 << (width)) - 1) << (offset)))

/**
 * @brief I/O bus descriptor.
 * @details This structure describes a group of contiguous digital I/O lines
 *          that have to be handled as bus.
 * @note I/O operations on a bus do not affect I/O lines on the same port but
 *       not belonging to the bus.
 */
typedef struct {
  /** Port identifier. */
  ioportid_t            bus_portid;
  /** Mask of the I/O lines that form the bus. The lines must be contiguous.
   *  The mask must be pre-shifted and also defines the bus size. */
  ioportmask_t          bus_mask;
  /** Offset, within the port, of the least significant bit of the bus. */
  uint_fast8_t          bus_offset;
} IOBus;

/**
 * @brief Writes a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] value the value to be written on the specified port
 */
#define chPortWrite(port, value) ioport_write_lld(port, value)

/**
 * @brief Reads an I/O port.
 *
 * @param[in] port the port identifier
 * @return the port bits
 */
#define chPortRead(port) ioport_read_lld(port)

/**
 * @brief Sets a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] mask the mask to be ORed on the specified port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 */
#define chPortSet(port, mask) ioport_set_lld(port, mask)

/**
 * @brief Clears a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] mask the mask to be cleared on the specified port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 */
#define chPortClear(port, mask) ioport_clear_lld(port mask)

/**
 * @brief Toggles a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] mask the mask to be XORed on the specified port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 */
#define chPortToggle(port, mask) ioport_clear_lld(port mask)

/**
 * @brief Writes a value on an I/O bus.
 *
 * @param[in] bus the I/O bus
 * @param[in] value the value to be written on the I/O bus. Values exceeding
 *            the bus width are masked so most significant bits may be lost.
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 */
#define chPortWriteBus(bus, value) ioport_writebus_lld(bus, value)

/**
 * @brief Reads a value from an I/O bus.
 *
 * @param[in] bus the I/O bus
 * @return the bus bits
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 */
#define chPortReadBus(bus) ioport_readbus_lld(bus)

#endif /* _IOPORTS_H_ */

/** @} */
