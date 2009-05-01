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
 * @file queues.h I/O
 * @brief Queues macros and structures.
 * @addtogroup IOQueues
 * @{
 */

#ifndef _QUEUES_H_
#define _QUEUES_H_

/** Queue notification callback type. */
typedef void (*qnotify_t)(void);

/** Returned by the queue functions if the operation is successful. */
#define Q_OK            RDY_OK
/** Returned by the queue functions if a timeout occurs. */
#define Q_TIMEOUT       RDY_TIMEOUT
/** Returned by the queue functions if the queue is reset. */
#define Q_RESET         RDY_RESET
/** Returned by the queue functions if the queue is empty. */
#define Q_EMPTY         -3
/** Returned by the queue functions if the queue is full. */
#define Q_FULL          -4

#if CH_USE_QUEUES
/**
 * @brief Generic I/O queue structure.
 * @details This structure represents a generic Input or Output asymmetrical
 *          queue. The queue is asymmetrical because one end is meant to be
 *          accessed from a thread context, and thus can be blocking, the other
 *          end is accessible from interrupt handlers or from within a kernel
 *          lock zone (see <b>I-Locked</b> and <b>S-Locked</b> states in
 *          @ref system_states) and is non-blocking.
 */
typedef struct {
  uint8_t               *q_buffer;      /**< Pointer to the queue buffer.*/
  uint8_t               *q_top;         /**< Pointer to the first location
                                             after the buffer.*/
  uint8_t               *q_wrptr;       /**< Write pointer.*/
  uint8_t               *q_rdptr;       /**< Read pointer.*/
  Semaphore             q_sem;          /**< Counter @p Semaphore.*/
  qnotify_t             q_notify;       /**< Data notification callback.*/
} GenericQueue;

/** Returns the queue's buffer size. */
#define chQSize(q) ((q)->q_top - (q)->q_buffer)

/**
 * Returns the used space if used on an Input Queue and the empty space if
 * used on an Output Queue.
 * @note The returned value can be less than zero when there are waiting
 *       threads on the internal semaphore.
 */
#define chQSpace(q) chSemGetCounterI(&(q)->q_sem)

/**
 * @brief Input queue structure.
 * @details This structure represents a generic asymmetrical input queue.
 *          Writing in the queue is non-blocking and can be performed from
 *          interrupt handlers or from within a kernel lock zone (see
 *          <b>I-Locked</b> and <b>S-Locked</b> states in @ref system_states).
 *          Reading the queue can be a blocking operation and is supposed to
 *          be performed by a system thread.
 * @extends GenericQueue
 */
typedef GenericQueue InputQueue;

/** Evaluates to @p TRUE if the specified Input Queue is empty. */
#define chIQIsEmpty(q) (chQSpace(q) <= 0)

/** Evaluates to @p TRUE if the specified Input Queue is full. */
#define chIQIsFull(q) (chQSpace(q) >= chQSize(q))

#if CH_USE_SEMAPHORES_TIMEOUT
/*
 * When semaphores timeout is available this API is implemented as a
 * special case of the more general chIQGetTimeout().
 */
#define chIQGet(iqp) chIQGetTimeout(iqp, TIME_INFINITE)
#endif

/**
 * @brief Output queue structure.
 * @details This structure represents a generic asymmetrical output queue.
 *          Reading from the queue is non-blocking and can be performed from
 *          interrupt handlers or from within a kernel lock zone (see
 *          <b>I-Locked</b> and <b>S-Locked</b> states in @ref system_states).
 *          Writing the queue can be a blocking operation and is supposed to
 *          be performed by a system thread.
 * @extends GenericQueue
 */
typedef GenericQueue OutputQueue;

/** Evaluates to @p TRUE if the specified Output Queue is empty. */
#define chOQIsEmpty(q) (chQSpace(q) >= chQSize(q))

/** Evaluates to @p TRUE if the specified Output Queue is full. */
#define chOQIsFull(q) (chQSpace(q) <= 0)

#if CH_USE_SEMAPHORES_TIMEOUT
/*
 * When semaphores timeout is available this API is implemented as a
 * special case of the more general chOQPutTimeout().
 */
#define chOQPut(oqp, b) chOQPutTimeout(oqp, b, TIME_INFINITE)
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void chIQInit(InputQueue *qp, uint8_t *buffer, size_t size, qnotify_t inotify);
  void chIQResetI(InputQueue *qp);
  msg_t chIQPutI(InputQueue *qp, uint8_t b);
#if !CH_USE_SEMAPHORES_TIMEOUT
  msg_t chIQGet(InputQueue *qp);
#endif
  msg_t chIQGetTimeout(InputQueue *qp, systime_t timeout);
  size_t chIQRead(InputQueue *qp, uint8_t *buffer, size_t n);

  void chOQInit(OutputQueue *queue, uint8_t *buffer, size_t size, qnotify_t onotify);
  void chOQResetI(OutputQueue *queue);
#if !CH_USE_SEMAPHORES_TIMEOUT
  msg_t chOQPut(OutputQueue *queue, uint8_t b);
#endif
  msg_t chOQPutTimeout(OutputQueue *queue, uint8_t b, systime_t timeout);
  msg_t chOQGetI(OutputQueue *queue);
  size_t chOQWrite(OutputQueue *queue, uint8_t *buffer, size_t n);
#ifdef __cplusplus
}
#endif
#endif  /* CH_USE_QUEUES */

#endif /* _QUEUES_H_ */

/** @} */
