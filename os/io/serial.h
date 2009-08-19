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
  /**
   * @brief Configures and starts the driver.
   *
   * @param[in] ip pointer to a @p SerialDriver or derived class
   * @param[in] config The configuration record.
   */
  void (*start)(void *ip, const SerialDriverConfig *config);

  /**
   * @brief Stops the driver.
   * @Details Any thread waiting on the driver's queues will be awakened with
   *          the message @p Q_RESET.
   *
   * @param[in] ip pointer to a @p SerialDriver or derived class
   */
  void (*stop)(void *ip);
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
  void sdInit(SerialDriver *sd, qnotify_t inotify, qnotify_t onotify);
  void sdIncomingDataI(SerialDriver *sd, uint8_t b);
  msg_t sdRequestDataI(SerialDriver *sd);
  void sdAddFlagsI(SerialDriver *sd, sdflags_t mask);
  sdflags_t sdGetAndClearFlags(SerialDriver *sd);
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
#define sdPutWouldBlock(sd) chOQIsFull(&(sd)->d2.oqueue)

/**
 * @brief Direct input check on a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          checks directly the input queue. This is faster but cannot
 *          be used to check different channels implementations.
 * @see chIOGetWouldBlock()
 */
#define sdGetWouldBlock(sd) chIQIsEmpty(&(sd)->d2.iqueue)

/**
 * @brief Direct blocking write to a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 * @see chIOPut()
 */
#define sdPut(sd, b) chOQPut(&(sd)->d2.oqueue, b)

/**
 * @brief Direct blocking write on a @p SerialDriver with timeout
 *        specification.
 * @details This function bypasses the indirect access to the channel and
 *          writes directly on the output queue. This is faster but cannot
 *          be used to write to different channels implementations.
 * @see chIOPutTimeout()
 */
#define sdPutTimeout(sd, b, t) chOQPutTimeout(&(sd)->d2.iqueue, b, t)

/**
 * @brief Direct blocking read from a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 * @see chIOGet()
 */
#define sdGet(sd) chIQGet(&(sd)->d2.iqueue)

/**
 * @brief Direct blocking read from a @p SerialDriver with timeout
 *        specification.
 * @details This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 * @see chIOGetTimeout()
 */
#define sdGetTimeout(sd, t) chIQGetTimeout(&(sd)->d2.iqueue, t)

/**
 * @brief Direct non-blocking write to a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          writes directly to the output queue. This is faster but cannot
 *          be used to write from different channels implementations.
 * @see chIOWrite()
 */
#define sdWrite(sd, b, n) chOQWrite(&(sd)->d2.oqueue, b, n)

/**
 * @brief Direct non-blocking read on a @p SerialDriver.
 * @details This function bypasses the indirect access to the channel and
 *          reads directly from the input queue. This is faster but cannot
 *          be used to read from different channels implementations.
 * @see chIORead()
 */
#define sdRead(sd, b, n) chIQRead(&(sd)->d2.iqueue, b, n)

#endif /* _SERIAL_H_ */

/** @} */
