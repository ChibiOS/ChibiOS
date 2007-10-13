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
 * @addtogroup IOQueues
 * @{
 */

#include <ch.h>

#ifdef CH_USE_QUEUES

/**
 * Initializes an input queue. A Semaphore is internally initialized
 * and works as a counter of the bytes contained in the queue.
 * @param qp pointer to a \p Queue structure
 * @param buffer pointer to a memory area allocated as queue buffer
 * @param size size of the queue buffer
 * @param inotify pointer to a callback function that is invoked when
 *        some data is read from the Queue. The value can be \p NULL.
 */
void chIQInit(Queue *qp, BYTE8 *buffer, t_size size, t_qnotify inotify) {

  qp->q_buffer = qp->q_rdptr = qp->q_wrptr = buffer;
  qp->q_top = buffer + size;
  chSemInit(&qp->q_sem, 0);
  qp->q_notify = inotify;
}

/**
 * Resets an input queue. All the data is lost and the waiting threads
 * resumed.
 * @param qp pointer to a \p Queue structure
 */
void chIQReset(Queue *qp) {

  chSysLock();

  qp->q_rdptr = qp->q_wrptr = qp->q_buffer;
  chSemResetI(&qp->q_sem, 0);

  chSysUnlock();
}

/**
 * Inserts a byte into an input queue.
 * @param qp pointer to a \p Queue structure
 * @param b the byte value to be written
 * @return \p Q_OK if the operation is successful or \p Q_FULL if the queue is
 *         full.
 * @note This function is the lower side endpoint of the Input Queue.
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
t_msg chIQPutI(Queue *qp, BYTE8 b) {

  if (chIQIsFull(qp))
    return Q_FULL;

  *qp->q_wrptr++ = b;
  if (qp->q_wrptr >= qp->q_top)
    qp->q_wrptr = qp->q_buffer;
  chSemSignalI(&qp->q_sem);
  return Q_OK;
}

/**
 * Gets a byte from the input queue, if the queue is empty then the
 * calling thread is suspended until a byte arrives in the queue.
 * @param qp pointer to a \p Queue structure
 * @return a byte from the queue or \p Q_RESET if the queue was reset
 */
t_msg chIQGet(Queue *qp) {
  BYTE8 b;

  chSysLock();

  chSemWaitS(&qp->q_sem);
  if (currp->p_rdymsg < RDY_OK) {

    chSysUnlock();
    return currp->p_rdymsg;
  }
  b = *qp->q_rdptr++;
  if (qp->q_rdptr >= qp->q_top)
    qp->q_rdptr = qp->q_buffer;

  if (qp->q_notify)
    qp->q_notify();

  chSysUnlock();
  return b;
}

#ifdef CH_USE_QUEUES_TIMEOUT
/**
 * Gets a byte from the input queue, if the queue is empty then the
 * calling thread is suspended until a byte arrives in the queue or the
 * specified time expires.
 * @param qp pointer to a \p Queue structure
 * @param time the number of ticks before the operation timouts
 * @return a byte from the queue, \p Q_TIMEOUT if the specified time expired
 *         or \p Q_RESET if the queue was reset
 * @note The function is available only if the \p CH_USE_QUEUES_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
t_msg chIQGetTimeout(Queue *qp, t_time time) {
  BYTE8 b;
  t_msg msg;

  chSysLock();

  if ((msg = chSemWaitTimeoutS(&qp->q_sem, time)) < RDY_OK)
    return msg;
  b = *qp->q_rdptr++;
  if (qp->q_rdptr >= qp->q_top)
    qp->q_rdptr = qp->q_buffer;

  if (qp->q_notify)
    qp->q_notify();

  chSysUnlock();
  return b;
}
#endif /* CH_USE_QUEUES_TIMEOUTS */

/**
 * Reads some data from the input queue into the specified buffer. The function
 * is non-blocking and can return zero if the queue is empty.
 * @param qp pointer to a \p Queue structure
 * @param buffer the data buffer
 * @param n the maximum amount of data to be read
 * @return the number of bytes read
 * @note This function is the upper side endpoint of the input queue.
 * @note The function is not atomic, if you need atomicity it is suggested
 *       to use a semaphore for mutual exclusion.
 */
t_size chIQRead(Queue *qp, BYTE8 *buffer, t_size n) {

  t_size r = 0;
  while (n--) {
    chSysLock();

    if (chIQIsEmpty(qp)) {

      chSysUnlock();
      break;
    }
    chSemFastWaitS(&qp->q_sem);
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
 * Initializes an output queue. A Semaphore is internally initialized
 * and works as a counter of the free bytes in the queue.
 * @param qp pointer to a \p Queue structure
 * @param buffer pointer to a memory area allocated as queue buffer
 * @param size size of the queue buffer
 * @param onotify pointer to a callback function that is invoked when
 *        some data is written in the Queue. The value can be \p NULL.
 */
void chOQInit(Queue *qp, BYTE8 *buffer, t_size size, t_qnotify onotify) {

  qp->q_buffer = qp->q_rdptr = qp->q_wrptr = buffer;
  qp->q_top = buffer + size;
  chSemInit(&qp->q_sem, size);
  qp->q_notify = onotify;
}

/**
 * Resets an Output Queue. All the data is lost and the waiting threads
 * resumed.
 * @param qp pointer to a \p Queue structure
 */
void chOQReset(Queue *qp) {

  chSysLock();

  qp->q_rdptr = qp->q_wrptr = qp->q_buffer;
  chSemResetI(&qp->q_sem, (t_cnt)(qp->q_top - qp->q_buffer));

  chSysUnlock();
}

/**
 * Inserts a byte in the output queue, if the queue is full then the thread
 * is suspended until the queue has free space available.
 * @param qp pointer to a \p Queue structure
 * @param b the byte value to be written
 */
void chOQPut(Queue *qp, BYTE8 b) {

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
 * Gets a byte from an output queue.
 * @param qp pointer to a \p Queue structure
 * @return the byte value or \p Q_EMPTY if the queue is empty
 * @note This function is the lower side endpoint of the output queue.
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
t_msg chOQGetI(Queue *qp) {
  BYTE8 b;

  if (chOQIsEmpty(qp))
    return Q_EMPTY;

  b = *qp->q_rdptr++;
  if (qp->q_rdptr >= qp->q_top)
    qp->q_rdptr = qp->q_buffer;
  chSemSignalI(&qp->q_sem);
  return b;
}

/**
 * Writes some data from the specified buffer into the queue. The function
 * is non-blocking and can return zero if the queue is full.
 * @param qp pointer to a \p Queue structure
 * @param buffer the data buffer
 * @param n the maximum amount of data to be written
 * @note This function is the upper side endpoint of the output queue.
 * @note The function is not atomic, if you need atomicity it is suggested
 *       to use a semaphore for mutual exclusion.
 */
t_size chOQWrite(Queue *qp, BYTE8 *buffer, t_size n) {

  t_size w = 0;
  while (n--) {
    chSysLock();

    if (chOQIsFull(qp)) {

      chSysUnlock();
      break;
    }
    chSemFastWaitS(&qp->q_sem);
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

#ifdef CH_USE_QUEUES_HALFDUPLEX
 /**
 * Initializes an half duplex queue.
 * @param qp pointer to the \p HalfDuplexQueue structure
 * @param buffer pointer to a memory area allocated as buffer for the queue
 * @param size the size of the queue buffer
 * @param inotify pointer to a callback function that is invoked when
 *        some data is read from the queue. The value can be \p NULL.
 * @param onotify pointer to a callback function that is invoked when
 *        some data is written to the queue. The value can be \p NULL.
 */
void chHDQInit(HalfDuplexQueue *qp, BYTE8 *buffer, t_size size,
               t_qnotify inotify, t_qnotify onotify) {

  qp->hdq_buffer = qp->hdq_rdptr = qp->hdq_wrptr = buffer;
  qp->hdq_top = buffer + size;
  chSemInit(&qp->hdq_isem, 0);
  chSemInit(&qp->hdq_osem, size);
  qp->hdq_inotify = inotify;
  qp->hdq_onotify = onotify;
}

/**
 * Reads a byte from the receive queue, if the queue is empty or is in
 * transmission mode then the invoking thread is suspended.
 * @param qp pointer to a \p HalfDuplexQueue structure
 * @return the byte value
 */
t_msg chHDQGetReceive(HalfDuplexQueue *qp) {
  BYTE8 b;

  chSysLock();

  chSemWaitS(&qp->hdq_isem);
  if (currp->p_rdymsg < RDY_OK) {

    chSysUnlock();
    return currp->p_rdymsg;
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

#ifdef CH_USE_QUEUES_TIMEOUT
/**
 * Reads a byte from the receive queue, if the queue is empty or is in
 * transmission mode then the invoking thread is suspended.
 * @param qp pointer to a \p HalfDuplexQueue structure
 * @param time the number of ticks before the operation timouts
 * @return the byte value or \p Q_TIMEOUT if a timeout occurs
 * @note The function is available only if the \p CH_USE_QUEUES_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
t_msg chHDQGetReceiveTimeout(HalfDuplexQueue *qp, t_time time) {
  BYTE8 b;
  t_msg msg;

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
#endif

/**
 * Writes a byte into the transmit queue. If the buffer contains unread
 * input data then the the buffer is cleared and the queue goes in
 * transmission mode.
 * @param qp pointer to a \p HalfDuplexQueue structure
 * @param b the byte value to be written
 */
void chHDQPutTransmit(HalfDuplexQueue *qp, BYTE8 b) {

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
 * Gets a byte from the transmit queue.
 * @param qp pointer to a \p HalfDuplexQueue structure
 * @return the byte value or \p Q_EMPTY if the transmit queue is empty (not in
 *         transmission mode)
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
t_msg chHDQGetTransmitI(HalfDuplexQueue *qp) {
  BYTE8 b;

  if (!chHDQIsTransmitting(qp))
    return Q_EMPTY;

  b = *qp->hdq_rdptr++;
  if (qp->hdq_rdptr >= qp->hdq_top)
    qp->hdq_rdptr = qp->hdq_buffer;
  chSemSignalI(&qp->hdq_osem);
  return b;
}

/**
 * Writes a byte into the receive queue. If the queue is in transmission mode
 * then the byte is lost.
 * @param qp pointer to a \p HalfDuplexQueue structure
 * @param b the byte value to be written
 * @return \p Q_OK if the operation is successful or \p Q_FULL if the driver
 *         is in transmit mode or the receive queue is full.
 * @note This function must be called with interrupts disabled or from an
 *       interrupt handler.
 */
t_msg chHDQPutReceiveI(HalfDuplexQueue *qp, BYTE8 b) {

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
