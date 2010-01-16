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
 * @file streams.h
 * @brief Data streams.
 * @addtogroup data_streams
 * @{
 */

#ifndef _STREAMS_H_
#define _STREAMS_H_

/**
 * @brief BaseSequentialStream specific methods.
 */
struct _base_sequental_stream_methods {
  /**
   * @brief Stream write buffer method.
   */
  size_t (*write)(void *instance, const uint8_t *bp, size_t n);
  /**
   * @brief Stream read buffer method.
   */
  size_t (*read)(void *instance, uint8_t *bp, size_t n);
};

/**
 * @brief @p BaseSequentialStream specific data.
 * @note It is empty because @p BaseSequentialStream is only an interface
 *       without implementation.
 */
struct _base_sequental_stream_data {
};

/**
 * @brief @p BaseSequentialStream virtual methods table.
 */
struct BaseSequentialStreamVMT {
  /**
   * @p BaseSequentialStream class specific methods.
   */
  struct _base_sequental_stream_methods bss;
};

/**
 * @brief Base stream class.
 * @details This class represents a generic blocking unbuffered sequential
 *          data stream.
 */
typedef struct {
  /**
   * Virtual Methods Table.
   */
  const struct BaseSequentialStreamVMT *vmt;
  /**
   * @p BaseSequentialStream class specific data.
   */
  struct _base_sequental_stream_data bss;
} BaseSequentialStream;

/**
 * @brief Sequential Stream write.
 * @details The function writes data from a buffer to a stream.
 *
 * @param[in] ip pointer to a @p BaseSequentialStream or derived class
 * @param[in] bp pointer to the data buffer
 * @param[in] n the maximum amount of data to be transferred
 * @return The number of bytes transferred. The return value can be less
 *         than the specified number of bytes if the stream reaches a
 *         physical end of file and cannot be extended.
 */
#define chSequentialStreamWrite(ip, bp, n) ((ip)->vmt->bss.write(ip, bp, n))

/**
 * @brief Sequential Stream read.
 * @details The function reads data from a stream into a buffer.
 *
 * @param[in] ip pointer to a @p BaseSequentialStream or derived class
 * @param[out] bp pointer to the data buffer
 * @param[in] n the maximum amount of data to be transferred
 * @return The number of bytes transferred. The return value can be less
 *         than the specified number of bytes if the stream reaches the end
 *         of the available data.
 */
#define chSequentialStreamRead(ip, bp, n) ((ip)->vmt->bss.read(ip, bp, n))

#endif /* _STREAMS_H_ */

/** @} */
