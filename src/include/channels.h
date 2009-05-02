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
   * @brief Channel synchronous put method.
   * @param[in] b the byte value to be written in the queue
   * @param[in] timeout the number of ticks before the operation timeouts,
   *             the following special values are allowed:
   *             - @a TIME_IMMEDIATE immediate timeout.
   *             - @a TIME_INFINITE no timeout.
   *             .
   * @return The operation status:
   * @retval Q_OK if the operation succeeded.
   * @retval Q_TIMEOUT if the specified time expired.
   * @retval Q_RESET if the queue was reset.
   */
  msg_t (*put)(void *instance, uint8_t b, systime_t timeout);
  /**
   * Channel synchronous get method.
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
 * @details This class represents a generic, synchronous, byte-wide,
 *          I/O channel.
 */
typedef struct {
  struct _base_channel_vmt *vmt;                /**< Virtual Methods Table. */
  struct _base_channel_data d0;                 /**< Class data. */
} BaseChannel;

#if CH_USE_EVENTS
/**
 * @brief Virtual methods table for base asynchronous channels.
 */
struct _base_asynchronous_channel_methods {
  /**
   * Channel asynchronous write method.
   */
  size_t (*write)(void *instance, uint8_t *bp, size_t n);
  /**
   * Channel asynchronous read method.
   */
  size_t (*read)(void *instance, uint8_t *bp, size_t n);
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
#endif /* CH_USE_EVENTS */

#endif /* _CHANNELS_H_ */

/** @} */
