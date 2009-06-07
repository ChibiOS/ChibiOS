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
 * @file pal.h
 * @brief I/O Ports Abstraction Layer macros, types and structures
 * @addtogroup PAL
 * @{
 */

#ifndef _PAL_H_
#define _PAL_H_

#ifndef _PAL_LLD_H_
#include "pal_lld.h"
#endif

/**
 * @brief Port bit helper macro.
 * @details This macro calculates the mask of a bit within a port.
 *
 * @param[in] n the bit position within the port
 * @return The bit mask.
 */
#define PAL_PORT_BIT(n) ((ioportmask_t)(1 << (n)))


/**
 * @brief Bits group mask helper.
 * @details This macro calculates the mask of a bits group.
 *
 * @param[in] the group width
 * @return The group mask.
 */
#define PAL_GROUP_MASK(width) ((ioportmask_t)(1 << (width)) - 1)

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
  /** Bus mask aligned to port bit 0. The bus mask implicitly define the bus
   *  width. */
  ioportmask_t          bus_mask;
  /** Offset, within the port, of the least significant bit of the bus. */
  uint_fast8_t          bus_offset;
} IOBus;

/**
 * @brief PAL subsystem initialization.
 */
#define palInit() pal_lld_init()

/**
 * @brief Reads the physical I/O port states.
 *
 * @param[in] port the port identifier
 * @return The port logical states.
 *
 * @note The default implementation always return zero and computes the
 *       parameter eventual side effects.
 */
#if !defined(pal_lld_readport) || defined(__DOXYGEN__)
#define palReadPort(port) ((void)(port), 0)
#else
#define palReadPort(port) pal_lld_readport(port)
#endif

/**
 * @brief Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 *
 * @param[in] port the port identifier
 * @return The latched logical states.
 *
 * @note The default implementation always return zero and computes the
 *       parameter eventual side effects.
 */
#if !defined(pal_lld_readlatch) || defined(__DOXYGEN__)
#define palReadLatch(port) ((void)(port), 0)
#else
#define palReadLatch(port) pal_lld_readlatch(port)
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
#if !defined(pal_lld_writeport) || defined(__DOXYGEN__)
#define palWritePort(port, bits) ((void)(port), (void)(bits))
#else
#define palWritePort(port, bits) pal_lld_writeport(port, bits)
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
#if !defined(pal_lld_setport) || defined(__DOXYGEN__)
#define palSetPort(port, bits) {                                        \
  palWritePort(port, palReadLatch(p) | (bits));                         \
}
#else
#define palSetPort(port, bits) pal_lld_setport(port, bits)
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
#if !defined(pal_lld_clearport) || defined(__DOXYGEN__)
#define palClearPort(port, bits) {                                      \
  palWritePort(port, palReadLatch(p) & ~(bits));                        \
}
#else
#define palClearPort(port, bits) pal_lld_clearport(port, bits)
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
#if !defined(pal_lld_toggleport) || defined(__DOXYGEN__)
#define palTogglePort(port, bits) {                                     \
  palWritePort(port, palReadLatch(port) ^ (bits));                      \
}
#else
#define palTogglePort(port, bits) pal_lld_toggleport(port, bits)
#endif

/**
 * @brief Reads a group of bits.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] offset the group bit offset within the port
 * @return The group logical states.
 */
#if !defined(pal_lld_readgroup) || defined(__DOXYGEN__)
#define palReadGroup(port, mask, offset)                               \
  ((palReadPort(port) >> (offset)) & (mask))
#else
#define palReadGroup(port, mask, offset) pal_lld_readgroup(port, mask, offset)
#endif

/**
 * @brief Writes a group of bits.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] offset the group bit offset within the port
 * @param[in] bits the bits to be written. Values exceeding the group width
 *            are masked.
 * @return The group logical states.
 */
#if !defined(pal_lld_writegroup) || defined(__DOXYGEN__)
#define palWriteGroup(port, mask, offset, bits) {                       \
  palWritePort(port, (palReadLatch(port) & ~((mask) << (offset))) |     \
                     (((bits) & (mask)) << (offset)));                  \
}
#else
#define palWriteGroup(port, mask, offset, bits)                         \
  pal_lld_writegroup(port, mask, offset, bits)
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
 * @note The default implementation internally uses the @p palReadPort().
 */
#if !defined(pal_lld_readpad) || defined(__DOXYGEN__)
#define palReadPad(port, pad) ((palReadPort(port) >> (pad)) & 1)
#else
#define palReadPad(port, pad) pal_lld_readpad(port, pad)
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
 * @note The default implementation internally uses the @p palReadLatch() and
 *       @p palWritePort().
 */
#if !defined(pal_lld_writepad) || defined(__DOXYGEN__)
#define palWritePad(port, pad, value) {                                 \
  palWritePort(port, (palReadLatch(port) & ~PAL_PORT_BIT(pad)) |        \
                     (((value) & 1) << pad));                           \
}
#else
#define palWritePad(port, pad, value) pal_lld_writepad(port, pad, value)
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
 * @note The default implementation internally uses the @p palSetPort().
 */
#if !defined(pal_lld_setpad) || defined(__DOXYGEN__)
#define palSetPad(port, pad) palSetPort(port, PAL_PORT_BIT(pad))
#else
#define palSetPad(port, pad) pal_lld_setpad(port, pad)
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
 * @note The default implementation internally uses the @p palClearPort().
 */
#if !defined(pal_lld_clearpad) || defined(__DOXYGEN__)
#define palClearPad(port, pad) palClearPort(port, PAL_PORT_BIT(pad))
#else
#define palClearPad(port, pad) pal_lld_clearpad(port, pad)
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
 * @note The default implementation internally uses the @p palTogglePort().
 */
#if !defined(pal_lld_togglepad) || defined(__DOXYGEN__)
#define palTogglePad(port, pad) palTogglePort(port, PAL_PORT_BIT(pad))
#else
#define palTogglePad(port, pad) pal_lld_togglepad(port, pad)
#endif

#ifdef __cplusplus
extern "C" {
#endif
  ioportmask_t palReadBus(IOBus *bus);
  void palWriteBus(IOBus *bus, ioportmask_t bits);
#ifdef __cplusplus
}
#endif

#endif /* _PAL_H_ */

/** @} */
