/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
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
 * @brief I/O queue structure.
 * @details This structure is used by both Input and Output Queues,
 * the difference is on how the semaphore is initialized.
 */
typedef struct {
  uint8_t               *q_buffer;      /**< Pointer to the queue buffer.*/
  uint8_t               *q_top;         /**< Pointer to the first location
                                             after the buffer.*/
  uint8_t               *q_wrptr;       /**< Write pointer.*/
  uint8_t               *q_rdptr;       /**< Read pointer.*/
  Semaphore             q_sem;          /**< Counter @p Semaphore.*/
  qnotify_t             q_notify;       /**< Data notification callback.*/
} Queue;

/** Returns the queue's buffer size. */
#define chQSize(q) ((q)->q_top - (q)->q_buffer)

/**
 * Returns the used space if used on an Input Queue and the empty space if
 * used on an Output Queue.
 * @note The returned value can be less than zero when there are waiting
 *       threads on the internal semaphore.
 */
#define chQSpace(q) chSemGetCounterI(&(q)->q_sem)

/** Evaluates to TRUE if the specified Input Queue is empty. */
#define chIQIsEmpty(q) (chQSpace(q) <= 0)

/** Evaluates to TRUE if the specified Input Queue is full. */
#define chIQIsFull(q) (chQSpace(q) >= chQSize(q))

/** Evaluates to TRUE if the specified Output Queue is empty. */
#define chOQIsEmpty(q) (chQSpace(q) >= chQSize(q))

/** Evaluates to TRUE if the specified Output Queue is full. */
#define chOQIsFull(q) (chQSpace(q) <= 0)

#ifdef __cplusplus
extern "C" {
#endif
  /*
   * Input Queues functions. An Input Queue is usually written into by an
   * interrupt handler and read from a thread.
   */
  void chIQInit(Queue *qp, uint8_t *buffer, size_t size, qnotify_t inotify);
  void chIQReset(Queue *qp);
  msg_t chIQPutI(Queue *qp, uint8_t b);
  msg_t chIQGet(Queue *qp);
  size_t chIQRead(Queue *qp, uint8_t *buffer, size_t n);
#if CH_USE_QUEUES_TIMEOUT
  msg_t chIQGetTimeout(Queue *qp, systime_t time);
#endif

  /*
   * Output Queues functions. An Output Queue is usually written into by a
   * thread and read from an interrupt handler.
   */
  void chOQInit(Queue *queue, uint8_t *buffer, size_t size, qnotify_t onotify);
  void chOQReset(Queue *queue);
  void chOQPut(Queue *queue, uint8_t b);
  msg_t chOQGetI(Queue *queue);
  size_t chOQWrite(Queue *queue, uint8_t *buffer, size_t n);
#ifdef __cplusplus
}
#endif
#endif  /* CH_USE_QUEUES */

#if CH_USE_QUEUES_HALFDUPLEX
/**
 * @brief Half duplex queue structure.
 */
typedef struct {
  uint8_t               *hdq_buffer;    /**< Pointer to the queue buffer.*/
  uint8_t               *hdq_top;       /**< Pointer to the first location
                                             after the buffer. */
  uint8_t               *hdq_wrptr;     /**< Write pointer.*/
  uint8_t               *hdq_rdptr;     /**< Read pointer.*/
  Semaphore             hdq_isem;       /**< Input counter @p Semaphore.*/
  Semaphore             hdq_osem;       /**< Output counter @p Semaphore.*/
  qnotify_t             hdq_inotify;    /**< Input data notification
                                             callback.*/
  qnotify_t             hdq_onotify;    /**< Output data notification
                                             callback.*/
} HalfDuplexQueue;

/** Returns the queue's buffer size. */
#define chHDQSize(q) ((q)->hdq_top - (q)->hdq_buffer)

/**
 * Returns the queue space when in transmission mode.
 * @note The returned value can be less than zero when there are waiting
 *       threads on the internal semaphore.
 */
#define chHDQEmptySpace(q) chSemGetCounterI(&(q)->hdq_osem)

/**
 * Returns the number of the bytes in the queue when in receive mode.
 * @note The returned value can be less than zero when there are waiting
 *       threads on the internal semaphore.
 */
#define chHDQFilledSpace(q) chSemGetCounterI(&(q)->hdq_isem)

/** Evaluates to TRUE if the queue is in transmit mode. */
#define chHDQIsTransmitting(q) (chHDQEmptySpace(q) < chHDQSize(q))

/** Evaluates to TRUE if the queue is in receive mode. */
#define chHDQIsReceiving(q) (chHDQEmptySpaceQ(q) >= chHDQSize(q))

/** Evaluates to TRUE if the receive queue is full. */
#define chHDQIsFullReceive(q) (chHDQFilledSpace(q) >= chHDQSize(q))

#ifdef __cplusplus
extern "C" {
#endif
  void chHDQInit(HalfDuplexQueue *qp, uint8_t *buffer, size_t size,
                 qnotify_t inotify, qnotify_t onotify);
  msg_t chHDQGetReceive(HalfDuplexQueue *qp);
  void chHDQPutTransmit(HalfDuplexQueue *qp, uint8_t b);
  msg_t chHDQGetTransmitI(HalfDuplexQueue *qp);
  msg_t chHDQPutReceiveI(HalfDuplexQueue *qp, uint8_t b);
#if CH_USE_QUEUES_TIMEOUT
  msg_t chHDQGetReceiveTimeout(HalfDuplexQueue *qp, systime_t time);
#endif
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_QUEUES_HALFDUPLEX */

#endif /* _QUEUES_H_ */

/** @} */
