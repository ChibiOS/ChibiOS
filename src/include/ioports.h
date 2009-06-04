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
   *  The mask must be pre-shifted and also defines the bus width. */
  ioportmask_t          bus_mask;
  /** Offset, within the port, of the least significant bit of the bus. */
  uint_fast8_t          bus_offset;
} IOBus;

/**
 * @brief Reads an I/O port.
 *
 * @param[in] port the port identifier
 * @return The port logical states.
 *
 * @note The default implementation always return zero and computes the
 *       parameter eventual side effects.
 */
#if !defined(ioport_read_lld) || defined(__DOXYGEN__)
#define chPortRead(port) ((void)(port), 0)
#else
#define chPortRead(port) ioport_read_lld(port)
#endif

/**
 * @brief Writes a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note The default implementation does nothing except computing the
 *       parameters eventual side effects.
 */
#if !defined(ioport_write_lld) || defined(__DOXYGEN__)
#define chPortWrite(port, bits) ((void)(port), (void)(bits))
#else
#define chPortWrite(port, bits) ioport_write_lld(port, bits)
#endif

/**
 * @brief Sets a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 */
#if !defined(ioport_set_lld) || defined(__DOXYGEN__)
#define chPortSet(port, bits) {                                         \
  ioport_t p = (port);                                                  \
  chPortWrite(p, chPortRead(p) | (bits));                               \
}
#else
#define chPortSet(port, bits) ioport_set_lld(port, bits)
#endif

/**
 * @brief Clears a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 */
#if !defined(ioport_clear_lld) || defined(__DOXYGEN__)
#define chPortClear(port, bits) {                                       \
  ioport_t p = (port);                                                  \
  chPortWrite(p, chPortRead(p) & ~(bits));                              \
}
#else
#define chPortClear(port, bits) ioport_clear_lld(port, bits)
#endif

/**
 * @brief Toggles a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be XORed on the specified port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 */
#if !defined(ioport_toggle_lld) || defined(__DOXYGEN__)
#define chPortToggle(port, bits) {                                      \
  ioport_t p = (port);                                                  \
  chPortWrite(p, chPortRead(p) ^ (bits));                               \
}
#else
#define chPortToggle(port, bits) ioport_toggle_lld(port, bits)
#endif

/**
 * @brief Reads a value from an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @return The bus logical states.
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation not necessarily optimal. Low level drivers
 *       may  optimize the function by using specific hardware or coding.
 * @note The default implementation evaluates the parameter three times, be
 *       careful with side effects.
 */
#if !defined(ioport_readbus_lld) || defined(__DOXYGEN__)
#define chPortReadBus(bus) \
  ((chPortRead((bus)->bus_port) >> (bus)->bus_offset) & (bus)->bus_mask)
#else
#define chPortReadBus(bus) ioport_readbus_lld(bus)
#endif

/**
 * @brief Writes a value on an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @param[in] bits the bits to be written on the I/O bus. Values exceeding
 *            the bus width are masked so most significant bits are lost.
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 */
#if !defined(ioport_writebus_lld) || defined(__DOXYGEN__)
#define chPortWriteBus(bus, bits) {                                     \
  IOBus *b = (bus);                                                     \
  chPortWrite(b->bus_port, (chPortRead(b->bus_port) & ~b->bus_mask) |   \
                            (((bits) << b->bus_offset) & b->bus_mask)); \
}
#else
#define chPortWriteBus(bus, bits) ioport_writebus_lld(bus, bits)
#endif

/**
 * @brief Reads an input pad logical state.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 * @return The logical state.
 * @retval 0 low logical state.
 * @retval 1 high logical state.
 *
 * @note The default implementation not necessarily optimal. Low level drivers
 *       may  optimize the function by using specific hardware or coding.
 * @note The default implementation internally uses the @p chPortRead(). 
 */
#if !defined(ioport_readpad_lld) || defined(__DOXYGEN__)
#define chPortReadPad(port, pad) ((chPortRead(port) >> (pad)) & 1)
#else
#define chPortReadPad(port, pad) ioport_readpad_lld(port, pad)
#endif

/**
 * @brief Writes a logical state on an output pad.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 * @param[out] value the logical value, the value must be @p 0 or @p 1
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 * @note The default implementation internally uses the @p chPortRead() and
 *       @p chPortWrite(). 
 */
#if !defined(ioport_writepad_lld) || defined(__DOXYGEN__)
#define chPortWritePad(port, pad, value) {
  ioport_t p = (port);                                                  \
  chPortWrite(p, (chPortRead(p) & ~IOPORT_BIT(pad)) |                   \
                 ((value & 1) << pad));                                 \
}
#else
#define chPortWritePad(port, pad, value) ioport_writepad_lld(port, pad, value)
#endif

/**
 * @brief Sets a pad logical state to @p 1.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 * @note The default implementation internally uses the @p chPortSet(). 
 */
#if !defined(ioport_setpad_lld) || defined(__DOXYGEN__)
#define chPortSetPad(port, pad) chPortSet(port, IOPORT_BIT(pad))
#else
#define chPortSetPad(port, pad) ioport_setpad_lld(port, pad)
#endif

/**
 * @brief Clears a pad logical state to @p 0.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 * @note The default implementation internally uses the @p chPortClear().
 */
#if !defined(ioport_clearpad_lld) || defined(__DOXYGEN__)
#define chPortClearPad(port, pad) chPortClear(port, IOPORT_BIT(pad))
#else
#define chPortClearPad(port, pad) ioport_clearpad_lld(port, pad)
#endif

/**
 * @brief Toggles a pad logical state.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 * @note The default implementation internally uses the @p chPortToggle().
 */
#if !defined(ioport_togglepad_lld) || defined(__DOXYGEN__)
#define chPortTogglePad(port, pad) ioport_toggle_lld(port, IOPORT_BIT(pad))
#else
#define chPortTogglePad(port, pad) ioport_togglepad_lld(port, pad)
#endif

#endif /* _IOPORTS_H_ */

/** @} */
