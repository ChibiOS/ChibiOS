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
 * @brief @p BaseChannel specific methods.
 */
struct _base_channel_methods {
  /**
   * @brief Channel output check.
   * @see chIOPutWouldBlock()
   */
  bool_t (*putwouldblock)(void *instance);
  /**
   * @brief Channel input check.
   * @see chIOGetWouldBlock()
   */
  bool_t (*getwouldblock)(void *instance);
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
 * @brief @p BaseChannel specific data.
 * @note It is empty because @p BaseChannel is only an interface without
 *       implementation.
 */
struct _base_channel_data {
};

/**
 * @brief @p BaseChannel virtual methods table.
 */
struct BaseChannelVMT {
  /**
   * @p BaseChannel class specific methods.
   */
  struct _base_channel_methods m0;
};

/**
 * @brief Base channel class.
 * @details This class represents a generic, byte-wide, I/O channel.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  const struct BaseChannelVMT *vmt;
  /**
   * @p BaseChannel class specific data.
   */
  struct _base_channel_data d0;
} BaseChannel;

/**
 * @brief Channel output check.
 * @details This function verifies if a subsequent @p chIOPut() would block.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return The output queue status:
 * @retval FALSE if the output queue has space and would not block a write
 *         operation.
 * @retval TRUE if the output queue is full and would block a write operation.
 */
#define chIOPutWouldBlock(ip) ((ip)->vmt->m0.putwouldblock(ip))

/**
 * @brief Channel input check.
 * @details This function verifies if a subsequent @p chIOGett() would block.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return The input queue status:
 * @retval FALSE if the input queue contains data and would not block a read
 *         operation.
 * @retval TRUE if the input queue is empty and would block a read operation.
 */
#define chIOGetWouldBlock(ip) ((ip)->vmt->m0.getwouldblock(ip))

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
#define chIOPut(ip, b) ((ip)->vmt->m0.put(ip, b, TIME_INFINITE))

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
#define chIOPutTimeout(ip, b, timeout) ((ip)->vmt->m0.put(ip, b, timeout))

/**
 * @brief Channel blocking byte read.
 * @details This function reads a byte value from a channel. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return A byte value from the queue or:
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOGet(ip) ((ip)->vmt->m0.put(ip, TIME_INFINITE))

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
#define chIOGetTimeout(ip, timeout) ((ip)->vmt->m0.put(ip, timeout))

#if CH_USE_EVENTS
/**
 * @brief @p BaseAsynchronousChannel specific methods.
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
 * @brief @p BaseAsynchronousChannel specific data.
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
 * @brief @p BaseAsynchronousChannel virtual methods table.
 */
struct BaseAsynchronousChannelVMT {
  /**
   * @p BaseChannel class inherited methods.
   */
  struct _base_channel_methods m0;
  /**
   * @p BaseAsynchronousChannel class specific methods.
   */
  struct _base_asynchronous_channel_methods m1;
};

/**
 * @extends BaseChannel
 *
 * @brief Base asynchronous channel class.
 * @details This class extends @p BaseChannel by adding methods for
 *          asynchronous I/O in an event-driven environment.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  const struct BaseAsynchronousChannelVMT *vmt;
  /**
   * @p BaseChannel class inherited data.
   */
  struct _base_channel_data d0;
  /**
   * @p BaseAsynchronousChannel class specific data.
   */
  struct _base_asynchronous_channel_data d1;
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
#define chIOWrite(ip, bp, n) ((ip)->vmt->m1.write(ip, bp, n))

/**
 * @brief Channel non-blocking read.
 * @details The function reads data from a channel into a buffer. The
 *          transfer is non-blocking and can return zero if the channel has
 *          no data immediately available.
 *
 * @param[in] ip pointer to a @p BaseAsynchronousChannel or derived class
 * @param[out] bp pointer to the buffer where the input data is copied
 * @param[in] n the maximum amount of data to be transferred
 * @return The number of bytes transferred.
 */
#define chIORead(ip, bp, n) ((ip)->vmt->m1.read(ip, bp, n))

/**
 * @brief Returns the write event source.
 * @details The write event source is broadcasted when the channel is ready
 *          for write operations. This usually happens when the internal
 *          output queue becomes empty.
 * @param[in] ip pointer to a @p BaseAsynchronousChannel or derived class
 * @return A pointer to an @p EventSource object.
 */
#define chIOGetWriteEventSource(ip) (&((ip)->vmt->d1.oevent))

/**
 * @brief Returns the read event source.
 * @details The read event source is broadcasted when the channel is ready
 *          for read operations. This usually happens when the internal
 *          input queue becomes non-empty.
 * @param[in] ip pointer to a @p BaseAsynchronousChannel or derived class
 * @return A pointer to an @p EventSource object.
 */
#define chIOGetReadEventSource(ip) (&((ip)->vmt->d1.ievent))

#endif /* CH_USE_EVENTS */

#endif /* _CHANNELS_H_ */

/** @} */
