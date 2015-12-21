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
 * @file    hal_buffers.c
 * @brief   I/O Buffers code.
 *
 * @addtogroup HAL_BUFFERS
 * @{
 */

#include <string.h>

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an input buffers queue object.
 *
 * @param[out] ibqp     pointer to the @p io_buffers_queue_t object
 * @param[in] bp        pointer to a memory area allocated for buffers
 * @param[in] size      buffer size, including the size field which is the
 *                      size of a @p size_t
 * @param[in] n         number of buffers
 * @param[in] infy      callback called when a buffer is returned to the queue
 * @param[in] link      application defined pointer
 *
 * @init
 */
void ibqObjectInit(input_buffers_queue_t *ibqp, uint8_t *bp,
                   size_t size, size_t n,
                   dbnotify_t infy, void *link) {

  osalDbgCheck((ibqp != NULL) && (bp != NULL) && (size >= sizeof(size_t) + 2));

  osalThreadQueueObjectInit(&ibqp->waiting);
  ibqp->bcounter = 0;
  ibqp->brdptr   = bp;
  ibqp->bwrptr   = bp;
  ibqp->btop     = bp + (size * n);     /* Pre-calculated for speed.        */
  ibqp->bsize    = size;
  ibqp->bn       = n;
  ibqp->buffers  = bp;
  ibqp->ptr      = NULL;
  ibqp->top      = NULL;
  ibqp->notify   = infy;
  ibqp->link     = link;
}

/**
 * @brief   Resets an input buffers queue.
 * @details All the data in the input buffers queue is erased and lost, any
 *          waiting thread is resumed with status @p MSG_RESET.
 * @note    A reset operation can be used by a low level driver in order to
 *          obtain immediate attention from the high level layers.
 *
 * @param[in] ibqp      pointer to the @p input_buffers_queue_t object
 *
 * @iclass
 */
void ibqResetI(input_buffers_queue_t *ibqp) {

  osalDbgCheckClassI();

  ibqp->bcounter = 0;
  ibqp->brdptr   = ibqp->buffers;
  ibqp->bwrptr   = ibqp->buffers;
  ibqp->ptr      = NULL;
  ibqp->top      = NULL;
  osalThreadDequeueAllI(&ibqp->waiting, MSG_RESET);
}

/**
 * @brief   Gets the next empty buffer from the queue.
 * @note    The function always returns the same buffer if called repeatedly.
 *
 * @param[in] ibqp      pointer to the @p input_buffers_queue_t object
 * @return              A pointer to the next buffer to be filled.
 * @retval NULL         if the queue is full.
 *
 * @iclass
 */
uint8_t *ibqGetEmptyBufferI(input_buffers_queue_t *ibqp) {

  osalDbgCheckClassI();

  if (ibqIsFullI(ibqp)) {
    return NULL;
  }

  return ibqp->bwrptr + sizeof (size_t);
}

/**
 * @brief   Posts a new filled buffer in the queue.
 *
 * @param[in] ibqp      pointer to the @p input_buffers_queue_t object
 * @param[in] size      used size of the buffer
 *
 * @iclass
 */
void ibqPostBufferI(input_buffers_queue_t *ibqp, size_t size) {

  osalDbgCheckClassI();
  osalDbgAssert(!ibqIsFullI(ibqp), "buffers queue full");

  /* Writing size field in the buffer.*/
  *((size_t *)ibqp->bwrptr) = size;

  /* Posting the buffer in the queue.*/
  ibqp->bcounter++;
  ibqp->bwrptr += ibqp->bsize;
  if (ibqp->bwrptr >= ibqp->btop) {
    ibqp->bwrptr = ibqp->buffers;
  }

  /* Waking up one waiting thread, if any.*/
  osalThreadDequeueNextI(&ibqp->waiting, MSG_OK);
}

/**
 * @brief   Gets the next data-filled buffer from the queue.
 * @note    The function always acquires the same buffer if called repeatedly.
 * @post    After calling the function the fields @p ptr and @p top are set
 *          at beginning and end of the buffer data or @NULL if the queue
 *          is empty.
 *
 * @param[in] ibqp      pointer to the @p input_buffers_queue_t object
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if a buffer has been acquired.
 * @retval MSG_TIMEOUT  if the specified time expired.
 * @retval MSG_RESET    if the queue has been reset.
 *
 * @iclass
 */
msg_t ibqGetDataTimeoutI(input_buffers_queue_t *ibqp, systime_t timeout) {

  osalDbgCheckClassI();

  while (ibqIsEmptyI(ibqp)) {
    msg_t msg = osalThreadEnqueueTimeoutS(&ibqp->waiting, timeout);
    if (msg < MSG_OK) {
      return msg;
    }
  }

  /* Setting up the "current" buffer and its boundary.*/
  ibqp->ptr = ibqp->brdptr + sizeof (size_t);
  ibqp->top = ibqp->ptr + *((size_t *)ibqp->brdptr);

  return MSG_OK;
}

/**
 * @brief   Releases the data buffer back in the queue.
 * @note    The object callback is called after releasing the buffer.
 *
 * @param[out] ibqp     pointer to the @p input_buffers_queue_t object
 *
 * @iclass
 */
void ibqReleaseDataI(input_buffers_queue_t *ibqp) {

  osalDbgCheckClassI();
  osalDbgAssert(!ibqIsEmptyI(ibqp), "buffers queue empty");

  /* Freeing a buffer slot in the queue.*/
  ibqp->bcounter--;
  ibqp->brdptr += ibqp->bsize;
  if (ibqp->brdptr >= ibqp->btop) {
    ibqp->brdptr = ibqp->buffers;
  }

  /* No "current" buffer.*/
  ibqp->ptr = NULL;

  /* Notifying the buffer release.*/
  if (ibqp->notify != NULL) {
    ibqp->notify(ibqp);
  }
}

/**
 * @brief   Input queue read with timeout.
 * @details This function reads a byte value from an input queue. If
 *          the queue is empty then the calling thread is suspended until a
 *          new buffer arrives in the queue or a timeout occurs.
 *
 * @param[in] ibqp      pointer to the @p input_buffers_queue_t object
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              A byte value from the queue.
 * @retval MSG_TIMEOUT  if the specified time expired.
 * @retval MSG_RESET    if the queue has been reset.
 *
 * @api
 */
msg_t ibqGetTimeout(input_buffers_queue_t *ibqp, systime_t timeout) {
  msg_t msg;

  /* This condition indicates that a new buffer must be acquired.*/
  if (ibqp->ptr == NULL) {
    msg = ibqGetDataTimeoutI(ibqp, timeout);
    if (msg != MSG_OK) {
      return msg;
    }
  }

  /* Next byte from the buffer.*/
  msg = *ibqp->ptr;
  ibqp->ptr++;

  /* If the current buffer has been fully read then it is returned as
     empty in the queue.*/
  if (ibqp->ptr >= ibqp->top) {
    ibqReleaseDataI(ibqp);
  }

  return msg;
}

/**
 * @brief   Input queue read with timeout.
 * @details The function reads data from an input queue into a buffer.
 *          The operation completes when the specified amount of data has been
 *          transferred or after the specified timeout or if the queue has
 *          been reset.
 * @note    The function is not atomic, if you need atomicity it is suggested
 *          to use a semaphore or a mutex for mutual exclusion.
 *
 * @param[in] ibqp      pointer to the @p input_buffers_queue_t object
 * @param[out] bp       pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred, the
 *                      value 0 is reserved
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The number of bytes effectively transferred.
 *
 * @api
 */
size_t ibqReadTimeout(input_buffers_queue_t *ibqp, uint8_t *bp,
                      size_t n, systime_t timeout) {
  size_t r = 0;
  systime_t deadline;

  osalSysLock();

  /* Time window for the whole operation.*/
  deadline = osalOsGetSystemTimeX() + timeout;

  while (r < n) {
    size_t size;

    /* This condition indicates that a new buffer must be acquired.*/
    if (ibqp->ptr == NULL) {
      systime_t next_timeout = deadline - osalOsGetSystemTimeX();

      /* Handling the case where the system time went past the deadline,
         in this case next becomes a very high number because the system
         time is an unsigned type.*/
      if (next_timeout > timeout) {
        return MSG_TIMEOUT;
      }

      msg_t msg = ibqGetDataTimeoutI(ibqp, next_timeout);
      if (msg != MSG_OK) {
        return msg;
      }
    }

    /* Size of the data chunk present in the current buffer.*/
    size = ibqp->top - ibqp->ptr;
    if (size > n - r) {
      size = n - r;
    }

    /* Copying the chunk into the read buffer, the operation is performed
       outside the critical zone.*/
    osalSysUnlock();
    memcpy(bp, ibqp->ptr, size);
    osalSysLock();

    /* Updating the pointers and the counter.*/
    r         += size;
    bp        += size;
    ibqp->ptr += size;

    /* Has the current data buffer been finished? if so then release it.*/
    if (ibqp->ptr >= ibqp->top) {
      ibqReleaseDataI(ibqp);
    }
  }
  osalSysUnlock();

  return r;
}

/** @} */
