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
 * @file chqueues.c
 * @brief I/O Queues code.
 * @addtogroup IOQueues
 * @{
 */

#include <ch.h>

#if CH_USE_QUEUES

/**
 * @brief Initializes an input queue.
 * @details A Semaphore is internally initialized and works as a counter of
 *          the bytes contained in the queue.
 *
 * @param[out] qp pointer to a @p Queue structure
 * @param[in] buffer pointer to a memory area allocated as queue buffer
 * @param[in] size size of the queue buffer
 * @param[in] inotify pointer to a callback function that is invoked when
 *                    some data is read from the Queue. The value can be
*                     @p NULL.
 */
void chIQInit(Queue *qp, uint8_t *buffer, size_t size, qnotify_t inotify) {

  qp->q_buffer = qp->q_rdptr = qp->q_wrptr = buffer;
  qp->q_top = buffer + size;
  chSemInit(&qp->q_sem, 0);
  qp->q_notify = inotify;
}

/**
 * @brief Resets an input queue.
 * @details All the data is lost and the waiting threads resumed.
 *
 * @param[in] qp pointer to a @p Queue structure
 */
void chIQReset(Queue *qp) {

  chSysLock();

  qp->q_rdptr = qp->q_wrptr = qp->q_buffer;
  chSemResetI(&qp->q_sem, 0);

  chSysUnlock();
}

/**
 * @brief Inserts a byte into an input queue.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @param[in] b the byte value to be written
 * @retval Q_OK if the operation is successful.
 * @retval Q_FULL if the queue is full.
 * @note This function is the lower side endpoint of the Input Queue.
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
msg_t chIQPutI(Queue *qp, uint8_t b) {

  if (chIQIsFull(qp))
    return Q_FULL;

  *qp->q_wrptr++ = b;
  if (qp->q_wrptr >= qp->q_top)
    qp->q_wrptr = qp->q_buffer;
  chSemSignalI(&qp->q_sem);
  return Q_OK;
}

/**
 * @brief Gets a byte from the input queue.
 * @details If the queue is empty then the calling thread is suspended until
 *          a byte arrives in the queue.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @return A byte value from the queue.
 * @retval Q_RESET if the queue was reset.
 */
msg_t chIQGet(Queue *qp) {
  uint8_t b;

  chSysLock();

  if (chSemWaitS(&qp->q_sem) < RDY_OK) {

    chSysUnlock();
    return Q_RESET;
  }
  b = *qp->q_rdptr++;
  if (qp->q_rdptr >= qp->q_top)
    qp->q_rdptr = qp->q_buffer;

  if (qp->q_notify)
    qp->q_notify();

  chSysUnlock();
  return b;
}

#if CH_USE_QUEUES_TIMEOUT && CH_USE_SEMAPHORES_TIMEOUT
/**
 * @brief Gets a byte from the input queue.
 * @details If the queue is empty then the calling thread is suspended until
 *          a byte arrives in the queue or the specified time expires.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @param[in] time the number of ticks before the operation timeouts
 * @return A byte value from the queue.
 * @retval Q_TIMEOUT if the specified time expired.
 * @retval Q_RESET if the queue was reset.
 * @note The function is available only if the @p CH_USE_QUEUES_TIMEOUT and
 *       @p CH_USE_SEMAPHORES_TIMEOUT options are enabled in @p chconf.h.
 */
msg_t chIQGetTimeout(Queue *qp, systime_t time) {
  uint8_t b;
  msg_t msg;

  chSysLock();

  if ((msg = chSemWaitTimeoutS(&qp->q_sem, time)) < RDY_OK) {

    chSysUnlock();
    return msg;
  }
  b = *qp->q_rdptr++;
  if (qp->q_rdptr >= qp->q_top)
    qp->q_rdptr = qp->q_buffer;

  if (qp->q_notify)
    qp->q_notify();

  chSysUnlock();
  return b;
}
#endif /* (CH_USE_QUEUES_TIMEOUT && CH_USE_SEMAPHORES_TIMEOUT */

/**
 * @brief Reads some data from the input queue into the specified buffer.
 * @details The function is non-blocking and can return zero if the queue is
 *          empty.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @param[out] buffer the data buffer
 * @param[in] n the maximum amount of data to be read
 * @return The number of bytes read.
 * @note This function is the upper side endpoint of the input queue.
 * @note The function is not atomic, if you need atomicity it is suggested
 *       to use a semaphore for mutual exclusion.
 */
size_t chIQRead(Queue *qp, uint8_t *buffer, size_t n) {

  size_t r = 0;
  while (n--) {
    chSysLock();

    if (chIQIsEmpty(qp)) {

      chSysUnlock();
      break;
    }
    chSemFastWaitI(&qp->q_sem);
    *buffer++ = *qp->q_rdptr++;
    if (qp->q_rdptr >= qp->q_top)
      qp->q_rdptr = qp->q_buffer;

    chSysUnlock();
    r++;
  }

  if (r && qp->q_notify) {
    chSysLock();

    qp->q_notify();

    chSysUnlock();
  }

  return r;
}

/**
 * @brief Initializes an output queue.
 * @details A Semaphore is internally initialized and works as a counter of the
 *          free bytes in the queue.
 *
 * @param[out] qp pointer to a @p Queue structure
 * @param[in] buffer pointer to a memory area allocated as queue buffer
 * @param[in] size size of the queue buffer
 * @param[in] onotify pointer to a callback function that is invoked when
 *                    some data is written in the Queue. The value can be
 *                    @p NULL.
 */
void chOQInit(Queue *qp, uint8_t *buffer, size_t size, qnotify_t onotify) {

  qp->q_buffer = qp->q_rdptr = qp->q_wrptr = buffer;
  qp->q_top = buffer + size;
  chSemInit(&qp->q_sem, size);
  qp->q_notify = onotify;
}

/**
 * @brief Resets an Output Queue.
 * @details All the data is lost and the waiting threads resumed.
 *
 * @param[in] qp pointer to a @p Queue structure
 */
void chOQReset(Queue *qp) {

  chSysLock();

  qp->q_rdptr = qp->q_wrptr = qp->q_buffer;
  chSemResetI(&qp->q_sem, (cnt_t)(qp->q_top - qp->q_buffer));

  chSysUnlock();
}

/**
 * @brief Inserts a byte in the output queue.
 * @details If the queue is full then the thread is suspended until the queue
 *          has free space available.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @param[in] b the byte value to be written
 */
void chOQPut(Queue *qp, uint8_t b) {

  chSysLock();

  chSemWaitS(&qp->q_sem);
  *qp->q_wrptr++ = b;
  if (qp->q_wrptr >= qp->q_top)
    qp->q_wrptr = qp->q_buffer;

  if (qp->q_notify)
    qp->q_notify();

  chSysUnlock();
}

/**
 * @brief Gets a byte from an output queue.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @return The byte value from the queue.
 * @retval Q_EMPTY if the queue is empty.
 * @note This function is the lower side endpoint of the output queue.
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
msg_t chOQGetI(Queue *qp) {
  uint8_t b;

  if (chOQIsEmpty(qp))
    return Q_EMPTY;

  b = *qp->q_rdptr++;
  if (qp->q_rdptr >= qp->q_top)
    qp->q_rdptr = qp->q_buffer;
  chSemSignalI(&qp->q_sem);
  return b;
}

/**
 * @brief Writes some data from the specified buffer into the queue.
 * @details The function is non-blocking and can return zero if the queue is
 *          full.
 *
 * @param[in] qp pointer to a @p Queue structure
 * @param[in] buffer the data buffer
 * @param[in] n the maximum amount of data to be written
 * @return The number of written bytes.
 * @note This function is the upper side endpoint of the output queue.
 * @note The function is not atomic, if you need atomicity it is suggested
 *       to use a semaphore for mutual exclusion.
 */
size_t chOQWrite(Queue *qp, uint8_t *buffer, size_t n) {

  size_t w = 0;
  while (n--) {
    chSysLock();

    if (chOQIsFull(qp)) {

      chSysUnlock();
      break;
    }
    chSemFastWaitI(&qp->q_sem);
    *qp->q_wrptr++ = *buffer++;
    if (qp->q_wrptr >= qp->q_top)
      qp->q_wrptr = qp->q_buffer;

    chSysUnlock();
    w++;
  }

  if (w && qp->q_notify) {
    chSysLock();

    qp->q_notify();

    chSysUnlock();
  }

  return w;
}
#endif  /* CH_USE_QUEUES */

#if CH_USE_QUEUES_HALFDUPLEX
 /**
 * @brief Initializes an half duplex queue.
 *
 * @param[out] qp pointer to the @p HalfDuplexQueue structure
 * @param[in] buffer pointer to a memory area allocated as buffer for the queue
 * @param[in] size the size of the queue buffer
 * @param[in] inotify pointer to a callback function that is invoked when
 *                    some data is read from the queue. The value can be
 *                    @p NULL.
 * @param[in] onotify pointer to a callback function that is invoked when
 *                    some data is written to the queue. The value can be
 *                    @p NULL.
 */
void chHDQInit(HalfDuplexQueue *qp, uint8_t *buffer, size_t size,
               qnotify_t inotify, qnotify_t onotify) {

  qp->hdq_buffer = qp->hdq_rdptr = qp->hdq_wrptr = buffer;
  qp->hdq_top = buffer + size;
  chSemInit(&qp->hdq_isem, 0);
  chSemInit(&qp->hdq_osem, size);
  qp->hdq_inotify = inotify;
  qp->hdq_onotify = onotify;
}

/**
 * @brief Reads a byte from the receive queue.
 * @details If the queue is empty or is in transmission mode then the invoking
 *          thread is suspended.
 *
 * @param[in] qp pointer to a @p HalfDuplexQueue structure
 * @return The byte value.
 * @retval Q_RESET if the queue was reset.
 */
msg_t chHDQGetReceive(HalfDuplexQueue *qp) {
  uint8_t b;

  chSysLock();

  if (chSemWaitS(&qp->hdq_isem) < RDY_OK) {

    chSysUnlock();
    return Q_RESET;
  }
  /*
   * NOTE: The semaphore can be signaled only if the queue is in
   *       receive mode.
   */
  b = *qp->hdq_rdptr++;
  if (qp->hdq_rdptr >= qp->hdq_top)
    qp->hdq_rdptr = qp->hdq_buffer;

  if (qp->hdq_inotify)
    qp->hdq_inotify();

  chSysUnlock();
  return b;
}

#if CH_USE_QUEUES_TIMEOUT && CH_USE_SEMAPHORES_TIMEOUT
/**
 * @brief Reads a byte from the receive queue.
 * @details If the queue is empty or is in transmission mode then the invoking
 *          thread is suspended.
 *
 * @param[in] qp pointer to a @p HalfDuplexQueue structure
 * @param[in] time the number of ticks before the operation timouts
 * @return The byte value.
 * @retval Q_TIMEOUT if a timeout occurs.
 * @note The function is available only if the @p CH_USE_QUEUES_TIMEOUT and
 *       @p CH_USE_SEMAPHORES_TIMEOUT options are enabled in @p chconf.h.
 */
msg_t chHDQGetReceiveTimeout(HalfDuplexQueue *qp, systime_t time) {
  uint8_t b;
  msg_t msg;

  chSysLock();

  if ((msg = chSemWaitTimeoutS(&qp->hdq_isem, time)) < RDY_OK) {

    chSysUnlock();
    return msg;
  }
  /*
   * NOTE: The semaphore can be signaled only if the queue is in
   *       receive mode.
   */
  b = *qp->hdq_rdptr++;
  if (qp->hdq_rdptr >= qp->hdq_top)
    qp->hdq_rdptr = qp->hdq_buffer;

  if (qp->hdq_inotify)
    qp->hdq_inotify();

  chSysUnlock();
  return b;
}
#endif /* CH_USE_QUEUES_TIMEOUT && CH_USE_SEMAPHORES_TIMEOUT */

/**
 * @brief Writes a byte into the transmit queue.
 * @details If the buffer contains unread input data then the the buffer is
 *          cleared and the queue goes in transmission mode.
 *
 * @param[in] qp pointer to a @p HalfDuplexQueue structure
 * @param[in] b the byte value to be written
 */
void chHDQPutTransmit(HalfDuplexQueue *qp, uint8_t b) {

  chSysLock();

  /*
   * Transmission mode requires that all the unread data must be destroyed.
   */
  if (qp->hdq_isem.s_cnt > 0) {
    qp->hdq_isem.s_cnt = 0;
    qp->hdq_rdptr = qp->hdq_wrptr = qp->hdq_buffer;
  }

  /*
   * Goes in transmission mode.
   */
  chSemWaitS(&qp->hdq_osem);
  *qp->hdq_wrptr++ = b;
  if (qp->hdq_wrptr >= qp->hdq_top)
    qp->hdq_wrptr = qp->hdq_buffer;

  if (qp->hdq_onotify)
    qp->hdq_onotify();

  chSysUnlock();
}

/**
 * @brief Gets a byte from the transmit queue.
 *
 * @param[in] qp pointer to a @p HalfDuplexQueue structure
 * @return The byte value.
 * @retval Q_EMPTY if the transmit queue is empty (not in transmission mode).
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
msg_t chHDQGetTransmitI(HalfDuplexQueue *qp) {
  uint8_t b;

  if (!chHDQIsTransmitting(qp))
    return Q_EMPTY;

  b = *qp->hdq_rdptr++;
  if (qp->hdq_rdptr >= qp->hdq_top)
    qp->hdq_rdptr = qp->hdq_buffer;
  chSemSignalI(&qp->hdq_osem);
  return b;
}

/**
 * @brief Writes a byte into the receive queue.
 * @details If the queue is in transmission mode then the byte is lost.
 *
 * @param[in] qp pointer to a @p HalfDuplexQueue structure
 * @param[in] b the byte value to be written
 * @retval Q_OK if the operation is successful.
 * @retval Q_FULL if the driver is in transmit mode or the receive queue is full.
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
msg_t chHDQPutReceiveI(HalfDuplexQueue *qp, uint8_t b) {

  if (chHDQIsTransmitting(qp))
    return Q_FULL;

  if (chHDQIsFullReceive(qp))
    return Q_FULL;

  *qp->hdq_wrptr++ = b;
  if (qp->hdq_wrptr >= qp->hdq_top)
    qp->hdq_wrptr = qp->hdq_buffer;
  chSemSignalI(&qp->hdq_isem);
  return Q_OK;
}
#endif /* CH_USE_QUEUES_HALFDUPLEX */

/** @} */
