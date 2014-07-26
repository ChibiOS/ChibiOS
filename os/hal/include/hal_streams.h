/*
    ChibiOS/HAL - Copyright (C) 2006,2007,2008,2009,2010,
                  2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/HAL 

    ChibiOS/HAL is free software; you can redistribute it and/or modify
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
 * @file    hal_streams.h
 * @brief   Data streams.
 * @details This header defines abstract interfaces useful to access generic
 *          data streams in a standardized way.
 *
 * @addtogroup data_streams
 * @details This module define an abstract interface for generic data streams.
 *          Note that no code is present, just abstract interfaces-like
 *          structures, you should look at the system as to a set of
 *          abstract C++ classes (even if written in C). This system
 *          has then advantage to make the access to data streams
 *          independent from the implementation logic.<br>
 *          The stream interface can be used as base class for high level
 *          object types such as files, sockets, serial ports, pipes etc.
 * @{
 */

#ifndef _HAL_STREAMS_H_
#define _HAL_STREAMS_H_

/**
 * @name    Streams return codes
 * @{
 */
#define STM_OK               Q_OK
#define STM_TIMEOUT          Q_TIMEOUT
#define STM_RESET            Q_RESET
/** @} */

/* The ChibiOS/RT kernel provides the following definitions by itself, this
   check is performed in order to avoid conflicts. */
#if !defined(_CHIBIOS_RT_) || defined(__DOXYGEN__)

/**
 * @brief   BaseSequentialStream specific methods.
 */
#define _base_sequential_stream_methods                                     \
  /* Stream write buffer method.*/                                          \
  size_t (*write)(void *instance, const uint8_t *bp, size_t n);             \
  /* Stream read buffer method.*/                                           \
  size_t (*read)(void *instance, uint8_t *bp, size_t n);                    \
  /* Channel put method, blocking.*/                                        \
  msg_t (*put)(void *instance, uint8_t b);                                  \
  /* Channel get method, blocking.*/                                        \
  msg_t (*get)(void *instance);                                             \

/**
 * @brief   @p BaseSequentialStream specific data.
 * @note    It is empty because @p BaseSequentialStream is only an interface
 *          without implementation.
 */
#define _base_sequential_stream_data

/**
 * @brief   @p BaseSequentialStream virtual methods table.
 */
struct BaseSequentialStreamVMT {
  _base_sequential_stream_methods
};

/**
 * @brief   Base stream class.
 * @details This class represents a generic blocking unbuffered sequential
 *          data stream.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseSequentialStreamVMT *vmt;
  _base_sequential_stream_data
} BaseSequentialStream;

#endif /* !defined(_CHIBIOS_RT_)*/

/**
 * @name    Macro Functions (BaseSequentialStream)
 * @{
 */
/**
 * @brief   Sequential Stream write.
 * @details The function writes data from a buffer to a stream.
 *
 * @param[in] ip        pointer to a @p BaseSequentialStream or derived class
 * @param[in] bp        pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The number of bytes transferred. The return value can
 *                      be less than the specified number of bytes if an
 *                      end-of-file condition has been met.
 *
 * @api
 */
#define streamWrite(ip, bp, n) ((ip)->vmt->write(ip, bp, n))

/**
 * @brief   Sequential Stream read.
 * @details The function reads data from a stream into a buffer.
 *
 * @param[in] ip        pointer to a @p BaseSequentialStream or derived class
 * @param[out] bp       pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The number of bytes transferred. The return value can
 *                      be less than the specified number of bytes if an
 *                      end-of-file condition has been met.
 *
 * @api
 */
#define streamRead(ip, bp, n) ((ip)->vmt->read(ip, bp, n))

/**
 * @brief   Sequential Stream blocking byte write.
 * @details This function writes a byte value to a channel. If the channel
 *          is not ready to accept data then the calling thread is suspended.
 *
 * @param[in] ip        pointer to a @p BaseChannel or derived class
 * @param[in] b         the byte value to be written to the channel
 *
 * @return              The operation status.
 * @retval STM_OK       if the operation succeeded.
 * @retval STM_RESET    if an end-of-file condition has been met.
 *
 * @api
 */
#define streamPut(ip, b) ((ip)->vmt->put(ip, b))

/**
 * @brief   Sequential Stream blocking byte read.
 * @details This function reads a byte value from a channel. If the data
 *          is not available then the calling thread is suspended.
 *
 * @param[in] ip        pointer to a @p BaseChannel or derived class
 *
 * @return              A byte value from the queue.
 * @retval STM_RESET    if an end-of-file condition has been met.
 *
 * @api
 */
#define streamGet(ip) ((ip)->vmt->get(ip))
/** @} */

#endif /* _HAL_STREAMS_H_ */

/** @} */
