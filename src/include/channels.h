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
 * @file channels.h
 * @brief I/O channels
 * @addtogroup Channels
 * @{
 */

#ifndef _CHANNELS_H_
#define _CHANNELS_H_

/**
 * @brief Base channels methods.
 */
struct _base_channel_methods {
  /**
   * @brief Channel put method with timeout specification.
   * @see chIOPut()
   */
  msg_t (*put)(void *instance, uint8_t b, systime_t timeout);
  /**
   * @brief Channel get method with timeout specification.
   * @see chIOGet()
   */
  msg_t (*get)(void *instance, systime_t timeout);
};

/**
 * @brief Base channels data.
 * @note It is empty because @p BaseChannel is only an interface without
 *       implementation.
 */
struct _base_channel_data {
};

/**
 * @brief Virtual methods table for base channels.
 */
struct _base_channel_vmt {
  struct _base_channel_methods m0;              /**< Class methods. */
};

/**
 * @brief Base channel class.
 * @details This class represents a generic, byte-wide, I/O channel.
 */
typedef struct {
  struct _base_channel_vmt *vmt;                /**< Virtual Methods Table. */
  struct _base_channel_data d0;                 /**< Class data. */
} BaseChannel;

/**
 * @brief Channel blocking byte write.
 * @details This function writes a byte value to a channel. If the channel
 *          is not ready to accept data then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[in] b the byte value to be written to the channel
 * @return The operation status:
 * @retval Q_OK if the operation succeeded.
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOPut(ip, b) ((ip)->vmt.m0->put(ip, b, TIME_INFINITE))

/**
 * @brief Channel blocking byte write with timeout.
 * @details This function writes a byte value to a channel. If the channel
 *          is not ready to accept data then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[in] b the byte value to be written to the channel
 * @param[in] timeout the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @return The operation status:
 * @retval Q_OK if the operation succeeded.
 * @retval Q_TIMEOUT if the specified time expired.
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOPutTimeout(ip, b, timeout) ((ip)->vmt.m0->put(ip, b, timeout))

/**
 * @brief Channel blocking byte read.
 * @details This function reads a byte value from a channel. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return A byte value from the queue or:
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOGet(ip) ((ip)->vmt.m0->put(ip, TIME_INFINITE))

/**
 * @brief Channel blocking byte read with timeout.
 * @details This function reads a byte value from a channel. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[in] timeout the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @return A byte value from the queue or:
 * @retval Q_TIMEOUT if the specified time expired.
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOGetTimeout(ip, timeout) ((ip)->vmt.m0->put(ip, timeout))

#if CH_USE_EVENTS
/**
 * @brief Virtual methods table for base asynchronous channels.
 */
struct _base_asynchronous_channel_methods {
  /**
   * Channel asynchronous write method.
   * @see chIOWrite()
   */
  size_t (*write)(void *instance, uint8_t *buffer, size_t n);
  /**
   * Channel asynchronous read method.
   * @see chIORead()
   */
  size_t (*read)(void *instance, uint8_t *buffer, size_t n);
};

/**
 * @brief Base asynchronous channels data.
 */
struct _base_asynchronous_channel_data {
  /**
   * Data Available @p EventSource. This event is generated when some incoming
   * data is inserted in the input queue.
   */
  EventSource           ievent;
  /**
   * Data Transmitted @p EventSource. This event is generated when the
   * output queue is empty.
   */
  EventSource           oevent;
};

/**
 * @brief Virtual methods table for base asynchronous channels.
 */
struct _base_asynchronous_channel_vmt {
  struct _base_channel_methods m0;              /**< Super class methods. */
  struct _base_asynchronous_channel_methods m1; /**< Class methods. */
};

/**
 * @extends BaseChannel
 *
 * @brief Base asynchronous channel class.
 * @details This class extends @p BaseChannel by adding methods for
 *          asynchronous I/O in an event-driven environment.
 */
typedef struct {
  struct _base_asynchronous_channel_vmt *vmt;   /**< Virtual Methods Table. */
  struct _base_channel_data d0;                 /**< Super class data. */
  struct _base_asynchronous_channel_data d1;    /**< Class data. */
} BaseAsynchronousChannel;

/**
 * @brief Channel non-blocking write.
 * @details The function writes data from a buffer to a channel. The
 *          transfer is non-blocking and can return zero if the channel is
 *          not read to accept data.
 *
 * @param[in] ip pointer to a @p BaseAsynchronousChannel or derived class
 * @param[out] bp pointer to the buffer where the data is stored
 * @param[in] n the maximum amount of data to be transferred
 * @return The number of bytes transferred.
 */
#define chIOWrite(ip, bp, n) ((ip)->vmt.m1->write(ip, bp, n))

/**
 * @brief Channel non-blocking read.
 * @details The function reads data from a channel into a buffer. The
 *          transfer is non-blocking and can return zero if the channel has
 *          no data immediately available.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[out] bp pointer to the buffer where the input data is copied
 * @param[in] n the maximum amount of data to be transferred
 * @return The number of bytes transferred.
 */
#define chIORead(ip, bp, n) ((ip)->vmt.m1->read(ip, bp, n))

#endif /* CH_USE_EVENTS */

#endif /* _CHANNELS_H_ */

/** @} */
