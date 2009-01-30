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
 * @addtogroup Semaphores
 * @{
 */

#include <ch.h>

#ifdef CH_USE_SEMAPHORES
/**
 * Initializes a semaphore with the specified counter value.
 * @param sp pointer to a \p Semaphore structure
 * @param n initial value of the semaphore counter. Must be non-negative.
 * @note Can be called with interrupts disabled or enabled.
 */
void chSemInit(Semaphore *sp, cnt_t n) {

  chDbgAssert(n >= 0, "chsem.c, chSemInit()");
  queue_init(&sp->s_queue);
  sp->s_cnt = n;
}

/**
 * Performs a reset operation on the semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @param n the new value of the semaphore counter. The value must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the \p chSemWait() will return
 *       \p RDY_RESET instead of \p RDY_OK.
 */
void chSemReset(Semaphore *sp, cnt_t n) {

  chSysLock();

  chSemResetI(sp, n);
  chSchRescheduleS();

  chSysUnlock();
}

/**
 * Performs a reset operation on the semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @param n the new value of the semaphore counter. The value must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the \p chSemWait() will return
 *       \p RDY_RESET instead of \p RDY_OK.
 * @note This function does not reschedule.
 */
void chSemResetI(Semaphore *sp, cnt_t n) {
  cnt_t cnt;

  chDbgAssert(n >= 0, "chsem.c, chSemResetI()");
  cnt = sp->s_cnt;
  sp->s_cnt = n;
  while (cnt++ < 0)
    chSchReadyI(lifo_remove(&sp->s_queue))->p_rdymsg = RDY_RESET;
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using \p chSemReset().
 */
msg_t chSemWait(Semaphore *sp) {
  msg_t msg;

  chSysLock();

  msg = chSemWaitS(sp);

  chSysUnlock();
  return msg;
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using \p chSemReset().
 * @note This function must be called with interrupts disabled.
 * @note This function cannot be called by an interrupt handler.
 */
msg_t chSemWaitS(Semaphore *sp) {

  if (--sp->s_cnt < 0) {
    queue_insert(currp, &sp->s_queue);
    currp->p_wtsemp = sp;
    chSchGoSleepS(PRWTSEM);
    return currp->p_rdymsg;
  }
  return RDY_OK;
}

#ifdef CH_USE_SEMAPHORES_TIMEOUT
/**
 * Performs a wait operation on a semaphore with timeout specification.
 * @param sp pointer to a \p Semaphore structure
 * @param time the number of ticks before the operation fails
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using \p chSemReset().
 * @retval RDY_TIMEOUT if the semaphore was not signaled or reset within the
 *         specified timeout.
 * @note The function is available only if the \p CH_USE_SEMAPHORES_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
msg_t chSemWaitTimeout(Semaphore *sp, systime_t time) {
  msg_t msg;

  chSysLock();

  msg = chSemWaitTimeoutS(sp, time);

  chSysUnlock();
  return msg;
}

/**
 * Performs a wait operation on a semaphore with timeout specification.
 * @param sp pointer to a \p Semaphore structure
 * @param time the number of ticks before the operation fails
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using \p chSemReset().
 * @retval RDY_TIMEOUT if the semaphore was not signaled or reset within the specified
 *         timeout.
 * @note The function is available only if the \p CH_USE_SEMAPHORES_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
msg_t chSemWaitTimeoutS(Semaphore *sp, systime_t time) {

  if (--sp->s_cnt < 0) {
    queue_insert(currp, &sp->s_queue);
    currp->p_wtsemp = sp;
    return chSchGoSleepTimeoutS(PRWTSEM, time);
  }
  return RDY_OK;
}
#endif /* CH_USE_SEMAPHORES_TIMEOUT */

/**
 * Performs a signal operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @note The function is available only if the \p CH_USE_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemSignal(Semaphore *sp) {

  chSysLock();

  if (sp->s_cnt++ < 0)
    chSchWakeupS(fifo_remove(&sp->s_queue), RDY_OK);

  chSysUnlock();
}

/**
 * Performs a signal operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @note The function is available only if the \p CH_USE_SEMAPHORES
 *       option is enabled in \p chconf.h.
 * @note This function does not reschedule.
 */
void chSemSignalI(Semaphore *sp) {

  if (sp->s_cnt++ < 0) {
    /* NOTE: It is done this way in order to allow a tail call on
             chSchReadyI().*/
    Thread *tp = fifo_remove(&sp->s_queue);
    tp->p_rdymsg = RDY_OK;
    chSchReadyI(tp);
  }
}

#ifdef CH_USE_SEMSW
/**
 * Performs atomic signal and wait operations on two semaphores.
 * @param sps pointer to a \p Semaphore structure to be signaled
 * @param spw pointer to a \p Semaphore structure to be wait on
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using \p chSemReset().
 * @note The function is available only if the \p CH_USE_SEMSW
 *       option is enabled in \p chconf.h.
 */
msg_t chSemSignalWait(Semaphore *sps, Semaphore *spw) {
  msg_t msg;

  chSysLock();

  if (sps->s_cnt++ < 0)
    chSchReadyI(fifo_remove(&sps->s_queue))->p_rdymsg = RDY_OK;

  if (--spw->s_cnt < 0) {
    queue_insert(currp, &spw->s_queue);
    currp->p_wtsemp = spw;
    chSchGoSleepS(PRWTSEM);
    msg = currp->p_rdymsg;
  }
  else {
    chSchRescheduleS();
    msg = RDY_OK;
  }

  chSysUnlock();
  return msg;
}
#endif /* CH_USE_SEMSW */

#endif /* CH_USE_SEMAPHORES */

/** @} */
