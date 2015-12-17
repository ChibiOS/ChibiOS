/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_buffers.h
 * @brief   I/O Buffers macros and structures.
 *
 * @addtogroup HAL_BUFFERS
 * @{
 */

#ifndef _HAL_BUFFERS_H_
#define _HAL_BUFFERS_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define HAL_BUFFERS_QUEUE_SIZE              2

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an I/O buffer.
 */
typedef struct hal_buffer io_buffer_t;

/**
 * @brief   Structure of a generic buffer.
 */
struct hal_buffer {
  /**
   * @brief   Pointer to the buffer in memory.
   */
  uint8_t               *buffer;
  /**
   * @brief   Pointer to the first non-used location in the buffer.
   */
  uint8_t               *limit;
  /**
   * @brief   Pointer to the buffer boundary.
   */
  uint8_t               *top;
};

/**
 * @brief   Type of a generic double buffer.
 */
typedef struct io_double_buffer io_double_buffer_t;

/**
 * @brief   Double buffer notification callback type.
 *
 * @param[in] iodbp     the double buffer pointer
 */
typedef void (*dbnotify_t)(io_double_buffer_t *iodbp);

/**
 * @brief   Structure of a generic double buffer.
 */
struct io_double_buffer {
  /**
   * @brief   Queue of waiting threads.
   */
  threads_queue_t       waiting;
  /**
   * @brief   Active buffers counter.
   */
  volatile size_t       counter;
  /**
   * @brief   Buffer write pointer.
   */
  io_buffer_t           *bwrptr;
  /**
   * @brief   Buffer read pointer.
   */
  io_buffer_t           *brdptr;
  /**
   * @brief   Pointer for R/W sequential access.
   */
  uint8_t               *ptr;
  /**
   * @brief   Queue of buffer objects.
   */
  io_buffer_t           buffers[HAL_BUFFERS_QUEUE_SIZE];
  /**
   * @brief   Data notification callback.
   */
  dbnotify_t            notify;
  /**
   * @brief   Application defined field.
   */
  void                  *link;
};

/**
 * @brief   Type of an input double buffer.
 */
typedef io_double_buffer_t input_double_buffer_t;

/**
 * @brief   Type of an output double buffer.
 */
typedef io_double_buffer_t output_double_buffer_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void iobInit(io_buffer_t *iobp, uint8_t *bp, size_t size);
  void idbObjectInit(input_double_buffer_t *idbp,
                     uint8_t *b1p, uint8_t *b2p, size_t size,
                     dbnotify_t infy, void *link);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_BUFFERS_H_ */

/** @} */
