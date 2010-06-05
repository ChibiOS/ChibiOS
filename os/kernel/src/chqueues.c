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
 * @file    chqueues.c
 * @brief   I/O Queues code.
 *
 * @addtogroup io_queues
 * @details ChibiOS/RT queues are mostly used in serial-like device drivers.
 *          The device drivers are usually designed to have a lower side
 *          (lower driver, it is usually an interrupt service routine) and an
 *          upper side (upper driver, accessed by the application threads).<br>
 *          There are several kind of queues:<br>
 *          - <b>Input queue</b>, unidirectional queue where the writer is the
 *            lower side and the reader is the upper side.
 *          - <b>Output queue</b>, unidirectional queue where the writer is the
 *            upper side and the reader is the lower side.
 *          - <b>Full duplex queue</b>, bidirectional queue. Full duplex queues
 *            are implemented by pairing an input queue and an output queue
 *            together.
 *          .
 *          In order to use the I/O queues the @p CH_USE_QUEUES option must
 *          be enabled in @p chconf.h.<br>
 *          I/O queues are usually used as an implementation layer for the I/O
 *          channels interface, also see @ref io_channels.
 * @{
 */

#include "ch.h"

#if CH_USE_QUEUES

/**
 * @brief   Initializes an input queue.
 * @details A Semaphore is internally initialized and works as a counter of
 *          the bytes contained in the queue.
 * @note    The callback is invoked from within the S-Locked system state,
 *          see @ref system_states.
 *
 * @param[out] iqp      pointer to an @p InputQueue structure
 * @param[in] bp        pointer to a memory area allocated as queue buffer
 * @param[in] size      size of the queue buffer
 * @param[in] infy      pointer to a callback function that is invoked when
 *                      data is read from the queue. The value can be @p NULL.
 */
void chIQInit(InputQueue *iqp, uint8_t *bp, size_t size, qnotify_t infy) {

  iqp->q_buffer = iqp->q_rdptr = iqp->q_wrptr = bp;
  iqp->q_top = bp + size;
  iqp->q_notify = infy;
  chSemInit(&iqp->q_sem, 0);
}

/**
 * @brief   Resets an input queue.
 * @details All the data in the input queue is erased and lost, any waiting
 *          thread is resumed with status @p Q_RESET.
 * @note    A reset operation can be used by a low level driver in order to
 *          obtain immediate attention from the high level layers.
 *
 * @param[in] iqp       pointer to an @p InputQueue structure
 */
void chIQResetI(InputQueue *iqp) {

  iqp->q_rdptr = iqp->q_wrptr = iqp->q_buffer;
  chSemResetI(&iqp->q_sem, 0);
}

/**
 * @brief   Input queue write.
 * @details A byte value is written into the low end of an input queue.
 *
 * @param[in] iqp       pointer to an @p InputQueue structure
 * @param[in] b         the byte value to be written in the queue
 * @return              The operation status, it can be one of:
 * @retval Q_OK         if the operation has been completed with success.
 * @retval Q_FULL       if the queue is full and the operation cannot be
 *                      completed.
 */
msg_t chIQPutI(InputQueue *iqp, uint8_t b) {

  if (chIQIsFull(iqp))
    return Q_FULL;

  *iqp->q_wrptr++ = b;
  if (iqp->q_wrptr >= iqp->q_top)
    iqp->q_wrptr = iqp->q_buffer;
  chSemSignalI(&iqp->q_sem);
  return Q_OK;
}

/**
 * @brief   Input queue read with timeout.
 * @details This function reads a byte value from an input queue. If the queue
 *          is empty then the calling thread is suspended until a byte arrives
 *          in the queue or a timeout occurs.
 *
 * @param[in] iqp       pointer to an @p InputQueue structure
 * @param[in] time      the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              A byte value from the queue or:
 * @retval Q_TIMEOUT    if the specified time expired.
 * @retval Q_RESET      if the queue was reset.
 */
msg_t chIQGetTimeout(InputQueue *iqp, systime_t time) {
  uint8_t b;
  msg_t msg;

  chSysLock();
  if ((msg = chSemWaitTimeoutS(&iqp->q_sem, time)) < RDY_OK) {
    chSysUnlock();
    return msg;
  }
  b = *iqp->q_rdptr++;
  if (iqp->q_rdptr >= iqp->q_top)
    iqp->q_rdptr = iqp->q_buffer;

  if (iqp->q_notify)
    iqp->q_notify();

  chSysUnlock();
  return b;
}

/**
 * @brief   Input queue read with timeout.
 * @details The function reads data from an input queue into a buffer. The
 *          operation completes when the specified amount of data has been
 *          transferred or after the specified timeout or if the queue has
 *          been reset.
 * @note    The function is not atomic, if you need atomicity it is suggested
 *          to use a semaphore or a mutex for mutual exclusion.
 * @note    The queue callback is invoked before entering a sleep state and at
 *          the end of the transfer.
 *
 * @param[in] iqp       pointer to an @p InputQueue structure
 * @param[out] bp       pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @param[in] time      the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The number of bytes effectively transferred.
 */
size_t chIQReadTimeout(InputQueue *iqp, uint8_t *bp,
                       size_t n, systime_t time) {
  qnotify_t nfy = iqp->q_notify;
  size_t r = 0;

  chSysLock();
  while (TRUE) {
    if (chIQIsEmpty(iqp)) {
      if (nfy)
        nfy();
      if ((chSemWaitTimeoutS(&iqp->q_sem, time) != RDY_OK)) {
        chSysUnlock();
        return r;
      }
    }
    else
      chSemFastWaitI(&iqp->q_sem);
    *bp++ = *iqp->q_rdptr++;
    if (iqp->q_rdptr >= iqp->q_top)
      iqp->q_rdptr = iqp->q_buffer;
    if (nfy)
      nfy();
    chSysUnlock(); /* Gives a preemption chance in a controlled point.*/
    r++;
    if (--n == 0) {
      chSysLock();
      if (nfy)
        nfy();
      chSysUnlock();
      return r;
    }
    chSysLock();
  }
}

/**
 * @brief   Initializes an output queue.
 * @details A Semaphore is internally initialized and works as a counter of
 *          the free bytes in the queue.
 * @note    The callback is invoked from within the S-Locked system state,
 *          see @ref system_states.
 *
 * @param[out] oqp      pointer to an @p OutputQueue structure
 * @param[in] bp        pointer to a memory area allocated as queue buffer
 * @param[in] size      size of the queue buffer
 * @param[in] onfy      pointer to a callback function that is invoked when
 *                      data is written to the queue. The value can be @p NULL.
 */
void chOQInit(OutputQueue *oqp, uint8_t *bp, size_t size, qnotify_t onfy) {

  oqp->q_buffer = oqp->q_rdptr = oqp->q_wrptr = bp;
  oqp->q_top = bp + size;
  oqp->q_notify = onfy;
  chSemInit(&oqp->q_sem, (cnt_t)size);
}

/**
 * @brief   Resets an output queue.
 * @details All the data in the output queue is erased and lost, any waiting
 *          thread is resumed with status @p Q_RESET.
 * @note    A reset operation can be used by a low level driver in order to
 *          obtain immediate attention from the high level layers.
 *
 * @param[in] oqp       pointer to an @p OutputQueue structure
 */
void chOQResetI(OutputQueue *oqp) {

  oqp->q_rdptr = oqp->q_wrptr = oqp->q_buffer;
  chSemResetI(&oqp->q_sem, (cnt_t)(oqp->q_top - oqp->q_buffer));
}

/**
 * @brief   Output queue write with timeout.
 * @details This function writes a byte value to an output queue. If the queue
 *          is full then the calling thread is suspended until there is space
 *          in the queue or a timeout occurs.
 *
 * @param[in] oqp       pointer to an @p OutputQueue structure
 * @param[in] b         the byte value to be written in the queue
 * @param[in] time      the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status:
 * @retval Q_OK         if the operation succeeded.
 * @retval Q_TIMEOUT    if the specified time expired.
 * @retval Q_RESET      if the queue was reset.
 */
msg_t chOQPutTimeout(OutputQueue *oqp, uint8_t b, systime_t time) {
  msg_t msg;

  chSysLock();
  if ((msg = chSemWaitTimeoutS(&oqp->q_sem, time)) < RDY_OK) {
    chSysUnlock();
    return msg;
  }
  *oqp->q_wrptr++ = b;
  if (oqp->q_wrptr >= oqp->q_top)
    oqp->q_wrptr = oqp->q_buffer;

  if (oqp->q_notify)
    oqp->q_notify();

  chSysUnlock();
  return Q_OK;
}

/**
 * @brief   Output queue read.
 * @details A byte value is read from the low end of an output queue.
 *
 * @param[in] oqp       pointer to an @p OutputQueue structure
 * @return              The byte value from the queue or:
 * @retval Q_EMPTY      if the queue is empty.
 */
msg_t chOQGetI(OutputQueue *oqp) {
  uint8_t b;

  if (chOQIsEmpty(oqp))
    return Q_EMPTY;

  b = *oqp->q_rdptr++;
  if (oqp->q_rdptr >= oqp->q_top)
    oqp->q_rdptr = oqp->q_buffer;
  chSemSignalI(&oqp->q_sem);
  return b;
}

/**
 * @brief   Output queue write with timeout.
 * @details The function writes data from a buffer to an output queue. The
 *          operation completes when the specified amount of data has been
 *          transferred or after the specified timeout or if the queue has
 *          been reset.
 * @note    The function is not atomic, if you need atomicity it is suggested
 *          to use a semaphore or a mutex for mutual exclusion.
 * @note    The queue callback is invoked before entering a sleep state and at
 *          the end of the transfer.
 *
 * @param[in] oqp       pointer to an @p OutputQueue structure
 * @param[out] bp       pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @param[in] time      the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The number of bytes effectively transferred.
 */
size_t chOQWriteTimeout(OutputQueue *oqp, const uint8_t *bp,
                        size_t n, systime_t time) {
  qnotify_t nfy = oqp->q_notify;
  size_t w = 0;

  chSysLock();
  while (TRUE) {
    if (chOQIsFull(oqp)) {
      if (nfy)
        nfy();
      if ((chSemWaitTimeoutS(&oqp->q_sem, time) != RDY_OK)) {
        chSysUnlock();
        return w;
      }
    }
    else
      chSemFastWaitI(&oqp->q_sem);
    *oqp->q_wrptr++ = *bp++;
    if (oqp->q_wrptr >= oqp->q_top)
      oqp->q_wrptr = oqp->q_buffer;
    chSysUnlock(); /* Gives a preemption chance in a controlled point.*/
    w++;
    if (--n == 0) {
      chSysLock();
      if (nfy)
        nfy();
      chSysUnlock();
      return w;
    }
    chSysLock();
  }
}
#endif  /* CH_USE_QUEUES */

/** @} */
