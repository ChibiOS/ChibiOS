/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    chqueues.h I/O
 * @brief   Queues macros and structures.
 *
 * @addtogroup io_queues
 * @{
 */

#ifndef _CHQUEUES_H_
#define _CHQUEUES_H_

#if CH_USE_QUEUES

/*
 * Module dependencies check.
 */
#if !CH_USE_SEMAPHORES
#error "CH_USE_QUEUES requires CH_USE_SEMAPHORES"
#endif

/** @brief Queue notification callback type.*/
typedef void (*qnotify_t)(void);

/** @brief Returned by the queue functions if the operation is successful.*/
#define Q_OK            RDY_OK
/** @brief Returned by the queue functions if a timeout occurs.*/
#define Q_TIMEOUT       RDY_TIMEOUT
/** @brief Returned by the queue functions if the queue is reset.*/
#define Q_RESET         RDY_RESET
/** @brief Returned by the queue functions if the queue is empty.*/
#define Q_EMPTY         -3
/** @brief Returned by the queue functions if the queue is full.*/
#define Q_FULL          -4

/**
 * @brief   Generic I/O queue structure.
 * @details This structure represents a generic Input or Output asymmetrical
 *          queue. The queue is asymmetrical because one end is meant to be
 *          accessed from a thread context, and thus can be blocking, the other
 *          end is accessible from interrupt handlers or from within a kernel
 *          lock zone (see <b>I-Locked</b> and <b>S-Locked</b> states in
 *          @ref system_states) and is non-blocking.
 */
typedef struct {
  uint8_t               *q_buffer;  /**< @brief Pointer to the queue buffer.*/
  uint8_t               *q_top;     /**< @brief Pointer to the first location
                                                after the buffer.           */
  uint8_t               *q_wrptr;   /**< @brief Write pointer.              */
  uint8_t               *q_rdptr;   /**< @brief Read pointer.               */
  Semaphore             q_sem;      /**< @brief Counter @p Semaphore.       */
  qnotify_t             q_notify;   /**< @brief Data notification callback. */
} GenericQueue;

/**
 * @brief   Returns the queue's buffer size.
 */
#define chQSize(q) ((q)->q_top - (q)->q_buffer)

/**
 * @brief   Queue space.
 * @details Returns the used space if used on an Input Queue and the empty
 *          space if used on an Output Queue.
 * @note    The returned value can be less than zero when there are waiting
 *          threads on the internal semaphore.
 */
#define chQSpace(q) chSemGetCounterI(&(q)->q_sem)

/**
 * @extends GenericQueue
 *
 * @brief   Input queue structure.
 * @details This structure represents a generic asymmetrical input queue.
 *          Writing in the queue is non-blocking and can be performed from
 *          interrupt handlers or from within a kernel lock zone (see
 *          <b>I-Locked</b> and <b>S-Locked</b> states in @ref system_states).
 *          Reading the queue can be a blocking operation and is supposed to
 *          be performed by a system thread.
 */
typedef GenericQueue InputQueue;

/** @brief Evaluates to @p TRUE if the specified Input Queue is empty.*/
#define chIQIsEmpty(q) ((bool_t)(chQSpace(q) <= 0))

/** @brief Evaluates to @p TRUE if the specified Input Queue is full.*/
#define chIQIsFull(q) ((bool_t)(chQSpace(q) >= chQSize(q)))

/**
 * @brief   Input queue read.
 * @details This function reads a byte value from an input queue. If the queue
 *          is empty then the calling thread is suspended until a byte arrives
 *          in the queue.
 *
 * @param[in] iqp       pointer to an @p InputQueue structure
 * @return              A byte value from the queue or:
 * @retval Q_RESET      if the queue was reset.
 */
#define chIQGet(iqp) chIQGetTimeout(iqp, TIME_INFINITE)

/**
 * @brief   Data part of a static input queue initializer.
 * @details This macro should be used when statically initializing an
 *          input queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the input queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] inotify   input notification callback pointer
 */
#define _INPUTQUEUE_DATA(name, buffer, size, inotify) {                 \
  (uint8_t *)(buffer),                                                  \
  (uint8_t *)(buffer) + size,                                           \
  (uint8_t *)(buffer),                                                  \
  (uint8_t *)(buffer),                                                  \
  _SEMAPHORE_DATA(name.q_sem, 0),                                       \
  inotify                                                               \
}

/**
 * @brief   Static input queue initializer.
 * @details Statically initialized input queues require no explicit
 *          initialization using @p chIQInit().
 *
 * @param[in] name      the name of the input queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] inotify   input notification callback pointer
 */
#define INPUTQUEUE_DECL(name, buffer, size, inotify)                    \
  InputQueue name = _INPUTQUEUE_DATA(name, buffer, size, inotify)

/**
 * @extends GenericQueue
 *
 * @brief   Output queue structure.
 * @details This structure represents a generic asymmetrical output queue.
 *          Reading from the queue is non-blocking and can be performed from
 *          interrupt handlers or from within a kernel lock zone (see
 *          <b>I-Locked</b> and <b>S-Locked</b> states in @ref system_states).
 *          Writing the queue can be a blocking operation and is supposed to
 *          be performed by a system thread.
 */
typedef GenericQueue OutputQueue;

/**
 * @brief   Evaluates to @p TRUE if the specified Output Queue is empty.
 */
#define chOQIsEmpty(q) ((bool_t)(chQSpace(q) >= chQSize(q)))

/**
 * @brief   Evaluates to @p TRUE if the specified Output Queue is full.
 */
#define chOQIsFull(q) ((bool_t)(chQSpace(q) <= 0))

/**
 * @brief   Output queue write.
 * @details This function writes a byte value to an output queue. If the queue
 *          is full then the calling thread is suspended until there is space
 *          in the queue.
 *
 * @param[in] oqp       pointer to an @p OutputQueue structure
 * @param[in] b         the byte value to be written in the queue
 * @return              The operation status:
 * @retval Q_OK         if the operation succeeded.
 * @retval Q_RESET      if the queue was reset.
 */
#define chOQPut(oqp, b) chOQPutTimeout(oqp, b, TIME_INFINITE)

/**
 * @brief   Data part of a static output queue initializer.
 * @details This macro should be used when statically initializing an
 *          output queue that is part of a bigger structure.
 *
 * @param[in] name      the name of the output queue variable.
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] onotify   output notification callback pointer
 */
#define _OUTPUTQUEUE_DATA(name, buffer, size, onotify) {                \
  (uint8_t *)(buffer),                                                  \
  (uint8_t *)(buffer) + size,                                           \
  (uint8_t *)(buffer),                                                  \
  (uint8_t *)(buffer),                                                  \
  _SEMAPHORE_DATA(name.q_sem, size),                                    \
  onotify                                                               \
}

/**
 * @brief Static output queue initializer.
 * @details Statically initialized output queues require no explicit
 *          initialization using @p chOQInit().
 *
 * @param[in] name      the name of the output queue variable
 * @param[in] buffer    pointer to the queue buffer area
 * @param[in] size      size of the queue buffer area
 * @param[in] onotify   output notification callback pointer
 */
#define OUTPUTQUEUE_DECL(name, buffer, size, onotify)                   \
  InputQueue name = _OUTPUTQUEUE_DATA(name, buffer, size, onotify)

#ifdef __cplusplus
extern "C" {
#endif
  void chIQInit(InputQueue *qp, uint8_t *bp, size_t size, qnotify_t infy);
  void chIQResetI(InputQueue *qp);
  msg_t chIQPutI(InputQueue *qp, uint8_t b);
  msg_t chIQGetTimeout(InputQueue *qp, systime_t time);
  size_t chIQReadTimeout(InputQueue *qp, uint8_t *bp,
                         size_t n, systime_t time);

  void chOQInit(OutputQueue *queue, uint8_t *bp, size_t size, qnotify_t onfy);
  void chOQResetI(OutputQueue *queue);
  msg_t chOQPutTimeout(OutputQueue *queue, uint8_t b, systime_t time);
  msg_t chOQGetI(OutputQueue *queue);
  size_t chOQWriteTimeout(OutputQueue *queue, const uint8_t *bp,
                          size_t n, systime_t time);
#ifdef __cplusplus
}
#endif
#endif /* CH_USE_QUEUES */

#endif /* _CHQUEUES_H_ */

/** @} */
