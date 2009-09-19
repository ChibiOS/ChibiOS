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
 * @file serial.h
 * @brief Serial Driver macros and structures.
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

/** No pending conditions.*/
#define SD_NO_ERROR             0
/** Connection happened.*/
#define SD_CONNECTED            1
/** Disconnection happened.*/
#define SD_DISCONNECTED         2
/** Parity error happened.*/
#define SD_PARITY_ERROR         4
/** Framing error happened.*/
#define SD_FRAMING_ERROR        8
/** Overflow happened.*/
#define SD_OVERRUN_ERROR        16
/** Break detected.*/
#define SD_BREAK_DETECTED       32

typedef struct _SerialDriver SerialDriver;

#include "serial_lld.h"

/**
 * @brief @p SerialDriver specific methods.
 */
struct _serial_driver_methods {
};

/**
 * @brief @p SerialDriver virtual methods table.
 */
struct SerialDriverVMT {
  /**
   * @p BaseChannel class inherited methods.
   */
  struct _base_channel_methods m0;
  /**
   * @p BaseAsynchronousChannel class inherited methods.
   */
  struct _base_asynchronous_channel_methods m1;
  /**
   * @p SerialDriver specific methods.
   */
  struct _serial_driver_methods m2;
};

/**
 * @extends BaseAsynchronousChannel
 *
 * @brief Full duplex serial driver class.
 * @details This class extends @p BaseAsynchronousChannel by adding physical
 *          I/O queues.
 */
struct _SerialDriver {
  /**
   * Virtual Methods Table.
   */
  const struct SerialDriverVMT *vmt;
  /**
   * @p BaseChannel class inherited data.
   */
  struct _base_channel_data d0;
  /**
   * @p BaseAsynchronousChannel class inherited data.
   */
  struct _base_asynchronous_channel_data d1;
  /**
   * @p SerialDriver specific data.
   */
  struct _serial_driver_data d2;
};

#ifdef __cplusplus
extern "C" {
#endif
  void sdInit(void);
  void sdObjectInit(SerialDriver *sdp, qnotify_t inotify, qnotify_t onotify);
  void sdStart(SerialDriver *sdp, const SerialDriverConfig *config);
  void sdStop(SerialDriver *sdp);
  void sdIncomingDataI(SerialDriver *sdp, uint8_t b);
  msg_t sdRequestDataI(SerialDriver *sdp);
  void sdAddFlagsI(SerialDriver *sdp, sdflags_t mask);
  sdflags_t sdGetAndClearFlags(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

/**
 * @brief Direct output check on a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          checks directly the output queue. This is faster but cannot
 *          be used to check different channels implementations.
 * @see chIOPutWouldBlock()
 */
#define sdPutWouldBlock(sdp) chOQIsFull(&(sdp)->d2.oqueue)

/**
 * @brief Direct input check on a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          checks directly the input queue. This is faster but cannot
 *          be used to check different channels implementations.
 * @see chIOGetWouldBlock()
 */
#define sdGetWouldBlock(sdp) chIQIsEmpty(&(sdp)->d2.iqueue)

/**
 * @brief Direct blocking write to a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 * @see chIOPut()
 */
#define sdPut(sdp, b) chOQPut(&(sdp)->d2.oqueue, b)

/**
 * @brief Direct blocking write on a @p SerialDriver with timeout
 *        specification.
 * @details This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 * @see chIOPutTimeout()
 */
#define sdPutTimeout(sdp, b, t) chOQPutTimeout(&(sdp)->d2.iqueue, b, t)

/**
 * @brief Direct blocking read from a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 * @see chIOGet()
 */
#define sdGet(sdp) chIQGet(&(sdp)->d2.iqueue)

/**
 * @brief Direct blocking read from a @p SerialDriver with timeout
 *        specification.
 * @details This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 * @see chIOGetTimeout()
 */
#define sdGetTimeout(sdp, t) chIQGetTimeout(&(sdp)->d2.iqueue, t)

/**
 * @brief Direct non-blocking write to a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          writes directly to the output queue. This is faster but cannot
 *          be used to write from different channels implementations.
 * @see chIOWrite()
 */
#define sdWrite(sdp, b, n) chOQWrite(&(sdp)->d2.oqueue, b, n)

/**
 * @brief Direct non-blocking read on a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 * @see chIORead()
 */
#define sdRead(sdp, b, n) chIQRead(&(sdp)->d2.iqueue, b, n)

#endif /* _SERIAL_H_ */

/** @} */
