/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file channels.h
 * @brief I/O channels.
 * @addtogroup io_channels
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
  msg_t (*put)(void *instance, uint8_t b, systime_t time);
  /**
   * @brief Channel get method with timeout specification.
   * @see chIOGet()
   */
  msg_t (*get)(void *instance, systime_t time);
  /**
   * @brief Channel write method with timeout specification.
   * @see chIOWrite()
   */
  size_t (*write)(void *instance, const uint8_t *bp, size_t n, systime_t time);
  /**
   * @brief Channel read method with timeout specification.
   * @see chIORead()
   */
  size_t (*read)(void *instance, uint8_t *bp, size_t n, systime_t time);
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
   * @p BaseSequentialStream class inherited methods.
   */
  struct _base_sequental_stream_methods bss;
  /**
   * @p BaseChannel class specific methods.
   */
  struct _base_channel_methods bc;
};

/**
 * @extends BaseSequentialStream
 *
 * @brief Base channel class.
 * @details This class represents a generic, byte-wide, I/O channel. This class
 *          introduces generic I/O primitives with timeout specification.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  const struct BaseChannelVMT *vmt;
  /**
   * @p BaseSequentialStream class inherited data.
   */
  struct _base_sequental_stream_data bss;
  /**
   * @p BaseChannel class specific data.
   */
  struct _base_channel_data bc;
} BaseChannel;

/**
 * @brief Channel output check.
 * @details This function verifies if a subsequent put/write operation would
 *          block.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return The output queue status:
 * @retval FALSE if the output queue has space and would not block a write
 *         operation.
 * @retval TRUE if the output queue is full and would block a write operation.
 */
#define chIOPutWouldBlock(ip) ((ip)->vmt->bc.putwouldblock(ip))

/**
 * @brief Channel input check.
 * @details This function verifies if a subsequent get/read operation would
 *          block.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return The input queue status:
 * @retval FALSE if the input queue contains data and would not block a read
 *         operation.
 * @retval TRUE if the input queue is empty and would block a read operation.
 */
#define chIOGetWouldBlock(ip) ((ip)->vmt->bc.getwouldblock(ip))

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
#define chIOPut(ip, b) ((ip)->vmt->bc.put(ip, b, TIME_INFINITE))

/**
 * @brief Channel blocking byte write with timeout.
 * @details This function writes a byte value to a channel. If the channel
 *          is not ready to accept data then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[in] b the byte value to be written to the channel
 * @param[in] time the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @return The operation status:
 * @retval Q_OK if the operation succeeded.
 * @retval Q_TIMEOUT if the specified time expired.
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOPutTimeout(ip, b, time) ((ip)->vmt->bc.put(ip, b, time))

/**
 * @brief Channel blocking byte read.
 * @details This function reads a byte value from a channel. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @return A byte value from the queue or:
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOGet(ip) ((ip)->vmt->bc.get(ip, TIME_INFINITE))

/**
 * @brief Channel blocking byte read with timeout.
 * @details This function reads a byte value from a channel. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[in] time the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @return A byte value from the queue or:
 * @retval Q_TIMEOUT if the specified time expired.
 * @retval Q_RESET if the channel associated queue (if any) was reset.
 */
#define chIOGetTimeout(ip, time) ((ip)->vmt->bc.get(ip, time))

/**
 * @brief Channel blocking write with timeout.
 * @details The function writes data from a buffer to a channel. If the channel
 *          is not ready to accept data then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[out] bp pointer to the data buffer
 * @param[in] n the maximum amount of data to be transferred
 * @param[in] time the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @return The number of bytes transferred.
 */
#define chIOWriteTimeout(ip, bp, n, time)                                   \
  ((ip)->vmt->bac.write(ip, bp, n, time))

/**
 * @brief Channel blocking read with timeout.
 * @details The function reads data from a channel into a buffer. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip pointer to a @p BaseChannel or derived class
 * @param[in] bp pointer to the data buffer
 * @param[in] n the maximum amount of data to be transferred
 * @param[in] time the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @return The number of bytes transferred.
 */
#define chIOReadTimeout(ip, bp, n, time)                                    \
  ((ip)->vmt->bac.read(ip, bp, n, time))

#if CH_USE_EVENTS
/**
 * @brief @p BaseAsynchronousChannel specific methods.
 */
struct _base_asynchronous_channel_methods {
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
   * @p BaseSequentialStream class inherited methods.
   */
  struct _base_sequental_stream_methods bss;
  /**
   * @p BaseChannel class inherited methods.
   */
  struct _base_channel_methods bc;
  /**
   * @p BaseAsynchronousChannel class specific methods.
   */
  struct _base_asynchronous_channel_methods bac;
};

/**
 * @extends BaseChannel
 *
 * @brief Base asynchronous channel class.
 * @details This class extends @p BaseChannel by adding event sources fields
 *          for asynchronous I/O for use in an event-driven environment.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  const struct BaseAsynchronousChannelVMT *vmt;
  /**
   * @p BaseSequentialStream class inherited data.
   */
  struct _base_sequental_stream_data bss;
  /**
   * @p BaseChannel class inherited data.
   */
  struct _base_channel_data bc;
  /**
   * @p BaseAsynchronousChannel class specific data.
   */
  struct _base_asynchronous_channel_data bac;
} BaseAsynchronousChannel;

/**
 * @brief Returns the write event source.
 * @details The write event source is broadcasted when the channel is ready
 *          for write operations. This usually happens when the internal
 *          output queue becomes empty.
 * @param[in] ip pointer to a @p BaseAsynchronousChannel or derived class
 * @return A pointer to an @p EventSource object.
 */
#define chIOGetWriteEventSource(ip) (&((ip)->vmt->bac.oevent))

/**
 * @brief Returns the read event source.
 * @details The read event source is broadcasted when the channel is ready
 *          for read operations. This usually happens when the internal
 *          input queue becomes non-empty.
 * @param[in] ip pointer to a @p BaseAsynchronousChannel or derived class
 * @return A pointer to an @p EventSource object.
 */
#define chIOGetReadEventSource(ip) (&((ip)->vmt->bac.ievent))

#endif /* CH_USE_EVENTS */

#endif /* _CHANNELS_H_ */

/** @} */
