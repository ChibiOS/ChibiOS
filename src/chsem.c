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
 * @file chsem.c
 * @brief Semaphores code.
 * @addtogroup Semaphores
 * @{
 */

#include <ch.h>

#if CH_USE_SEMAPHORES

#if CH_USE_SEMAPHORES_PRIORITY
#define sem_insert(tp, qp) prio_insert(tp, qp)
#else
#define sem_insert(tp, qp) queue_insert(tp, qp)
#endif

/**
 * @brief Initializes a semaphore with the specified counter value.
 *
 * @param[out] sp pointer to a @p Semaphore structure
 * @param[in] n initial value of the semaphore counter. Must be non-negative.
 * @note This function can be invoked from within an interrupt handler even if
 *       it is not an I-Class API because it does not touch any critical kernel
 *       data structure.
 */
void chSemInit(Semaphore *sp, cnt_t n) {

  chDbgCheck((sp != NULL) && (n >= 0), "chSemInit");

  queue_init(&sp->s_queue);
  sp->s_cnt = n;
}

/**
 * @brief Performs a reset operation on the semaphore.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @param[in] n the new value of the semaphore counter. The value must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the @p chSemWait() will return
 *       @p RDY_RESET instead of @p RDY_OK.
 */
void chSemReset(Semaphore *sp, cnt_t n) {

  chSysLock();
  chSemResetI(sp, n);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief Performs a reset operation on the semaphore.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @param[in] n the new value of the semaphore counter. The value must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the @p chSemWait() will return
 *       @p RDY_RESET instead of @p RDY_OK.
 * @note This function does not reschedule.
 */
void chSemResetI(Semaphore *sp, cnt_t n) {
  cnt_t cnt;

  chDbgCheck((sp != NULL) && (n >= 0), "chSemResetI");

  cnt = sp->s_cnt;
  sp->s_cnt = n;
  while (cnt++ < 0)
    chSchReadyI(lifo_remove(&sp->s_queue))->p_rdymsg = RDY_RESET;
}

/**
 * @brief Performs a wait operation on a semaphore.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using @p chSemReset().
 */
msg_t chSemWait(Semaphore *sp) {
  msg_t msg;

  chSysLock();
  msg = chSemWaitS(sp);
  chSysUnlock();
  return msg;
}

/**
 * @brief Performs a wait operation on a semaphore.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using @p chSemReset().
 * @note This function must be called with interrupts disabled.
 * @note This function cannot be called by an interrupt handler.
 */
msg_t chSemWaitS(Semaphore *sp) {

  chDbgCheck(sp != NULL, "chSemWaitS");

  if (--sp->s_cnt < 0) {
    sem_insert(currp, &sp->s_queue);
    currp->p_wtsemp = sp;
    chSchGoSleepS(PRWTSEM);
    return currp->p_rdymsg;
  }
  return RDY_OK;
}

#if CH_USE_SEMAPHORES_TIMEOUT
/**
 * @brief Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @param[in] time the number of ticks before the operation timeouts,
 *             the following special values are allowed:
 *             - @a TIME_IMMEDIATE immediate timeout.
 *             - @a TIME_INFINITE no timeout.
 *             .
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using @p chSemReset().
 * @retval RDY_TIMEOUT if the semaphore was not signaled or reset within the
 *         specified timeout.
 * @note The function is available only if the @p CH_USE_SEMAPHORES_TIMEOUT
 *       option is enabled in @p chconf.h.
 */
msg_t chSemWaitTimeout(Semaphore *sp, systime_t time) {
  msg_t msg;

  chSysLock();
  msg = chSemWaitTimeoutS(sp, time);
  chSysUnlock();
  return msg;
}

/**
 * @brief Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @param[in] time the number of ticks before the operation timeouts,
 *                 the following special values are allowed:
 *                 - @a TIME_IMMEDIATE immediate timeout.
 *                 - @a TIME_INFINITE no timeout.
 *                 .
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using @p chSemReset().
 * @retval RDY_TIMEOUT if the semaphore was not signaled or reset within the specified
 *         timeout.
 * @note The function is available only if the @p CH_USE_SEMAPHORES_TIMEOUT
 *       option is enabled in @p chconf.h.
 */
msg_t chSemWaitTimeoutS(Semaphore *sp, systime_t time) {

  chDbgCheck(sp != NULL, "chSemWaitTimeoutS");

  if (--sp->s_cnt < 0) {
    if (TIME_IMMEDIATE == time) {
      sp->s_cnt++;
      return RDY_TIMEOUT;
    }
    sem_insert(currp, &sp->s_queue);
    currp->p_wtsemp = sp;
    return chSchGoSleepTimeoutS(PRWTSEM, time);
  }
  return RDY_OK;
}
#endif /* CH_USE_SEMAPHORES_TIMEOUT */

/**
 * @brief Performs a signal operation on a semaphore.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @note The function is available only if the @p CH_USE_SEMAPHORES
 *       option is enabled in @p chconf.h.
 */
void chSemSignal(Semaphore *sp) {

  chDbgCheck(sp != NULL, "chSemSignal");

  chSysLock();
  if (sp->s_cnt++ < 0)
    chSchWakeupS(fifo_remove(&sp->s_queue), RDY_OK);
  chSysUnlock();
}

/**
 * @brief Performs a signal operation on a semaphore.
 *
 * @param[in] sp pointer to a @p Semaphore structure
 * @note The function is available only if the @p CH_USE_SEMAPHORES
 *       option is enabled in @p chconf.h.
 * @note This function does not reschedule.
 */
void chSemSignalI(Semaphore *sp) {

  chDbgCheck(sp != NULL, "chSemSignalI");

  if (sp->s_cnt++ < 0) {
    /* NOTE: It is done this way in order to allow a tail call on
             chSchReadyI().*/
    Thread *tp = fifo_remove(&sp->s_queue);
    tp->p_rdymsg = RDY_OK;
    chSchReadyI(tp);
  }
}

#if CH_USE_SEMSW
/**
 * @brief Performs atomic signal and wait operations on two semaphores.
 *
 * @param[in] sps pointer to a @p Semaphore structure to be signaled
 * @param[in] spw pointer to a @p Semaphore structure to be wait on
 * @retval RDY_OK if the semaphore was signaled or not taken.
 * @retval RDY_RESET if the semaphore was reset using @p chSemReset().
 * @note The function is available only if the @p CH_USE_SEMSW
 *       option is enabled in @p chconf.h.
 */
msg_t chSemSignalWait(Semaphore *sps, Semaphore *spw) {
  msg_t msg;

  chDbgCheck((sps != NULL) && (spw != NULL), "chSemSignalWait");

  chSysLock();
  if (sps->s_cnt++ < 0)
    chSchReadyI(fifo_remove(&sps->s_queue))->p_rdymsg = RDY_OK;
  if (--spw->s_cnt < 0) {
    sem_insert(currp, &spw->s_queue);
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
