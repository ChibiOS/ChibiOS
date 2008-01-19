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
void chSemInit(Semaphore *sp, t_cnt n) {

  chDbgAssert(n >= 0, "chsem.c, chSemInit()");
  fifo_init(&sp->s_queue);
  sp->s_cnt = n;
}

/**
 * Performs a reset operation on the semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @param n the new value of the semaphore counter. Must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the \p p_rdymsg field is set to
 *       \p RDY_RESET.
 */
void chSemReset(Semaphore *sp, t_cnt n) {
  t_cnt cnt;

  chDbgAssert(n >= 0, "chsem.c, chSemReset()");
  chSysLock();

  cnt = sp->s_cnt;
  sp->s_cnt = n;
  if (cnt < 0) {
    while (cnt++)
      chSchReadyI(fifo_remove(&sp->s_queue), RDY_RESET);
    chSchRescheduleS();
  }

  chSysUnlock();
}

/**
 * Performs a reset operation on the semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @param n the new value of the semaphore counter. Must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the \p p_rdymsg field is set to
 *       \p RDY_RESET.
 * @note This function does not reschedule.
 */
void chSemResetI(Semaphore *sp, t_cnt n) {
  t_cnt cnt;

  chDbgAssert(n >= 0, "chsem.c, chSemResetI()");
  cnt = sp->s_cnt;
  sp->s_cnt = n;
  while (cnt++ < 0)
    chSchReadyI(fifo_remove(&sp->s_queue), RDY_RESET);
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @return the function can return \p RDY_OK or \p RDY_RESET.
 */
t_msg chSemWait(Semaphore *sp) {
  t_msg msg;

  chSysLock();

  msg = chSemWaitS(sp);

  chSysUnlock();
  return msg;
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @return the function can return \p RDY_OK or \p RDY_RESET.
 * @note This function must be called with interrupts disabled.
 * @note This function cannot be called by an interrupt handler.
 */
t_msg chSemWaitS(Semaphore *sp) {

  if (--sp->s_cnt < 0) {
    fifo_insert(currp, &sp->s_queue);
    currp->p_wtsemp = sp;
    chSchGoSleepS(PRWTSEM);
    return currp->p_rdymsg;
  }
  return RDY_OK;
}

#ifdef CH_USE_SEMAPHORES_TIMEOUT
static void wakeup(void *p) {

  chDbgAssert(((Thread *)p)->p_state == PRWTSEM, "chsem.c, wakeup()");
  chSemFastSignalI(((Thread *)p)->p_wtsemp);
  chSchReadyI(dequeue(p), RDY_TIMEOUT);
}

/**
 * Performs a wait operation on a semaphore with timeout specification.
 * @param sp pointer to a \p Semaphore structure
 * @param time the number of ticks before the operation fails
 * @return the function can return \p RDY_OK, \p RDY_TIMEOUT or \p RDY_RESET.
 */
t_msg chSemWaitTimeout(Semaphore *sp, t_time time) {
  t_msg msg;

  chSysLock();

  msg = chSemWaitTimeoutS(sp, time);

  chSysUnlock();
  return msg;
}

/**
 * Performs a wait operation on a semaphore with timeout specification.
 * @param sp pointer to a \p Semaphore structure
 * @param time the number of ticks before the operation fails
 * @return the function can return \p RDY_OK, \p RDY_TIMEOUT or \p RDY_RESET.
 * @note This function must be called with interrupts disabled.
 * @note This function cannot be called by an interrupt handler.
 * @note The function is available only if the \p CH_USE_SEMAPHORES_TIMEOUT
 *       option is enabled in \p chconf.h.
 */
t_msg chSemWaitTimeoutS(Semaphore *sp, t_time time) {

  if (--sp->s_cnt < 0) {
    VirtualTimer vt;

    chVTSetI(&vt, time, wakeup, currp);
    fifo_insert(currp, &sp->s_queue);
    currp->p_wtsemp = sp;
    chSchGoSleepS(PRWTSEM);
    if (chVTIsArmedI(&vt))
      chVTResetI(&vt);
    return currp->p_rdymsg;
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
 * @note This function must be called with interrupts disabled.
 * @note The function is available only if the \p CH_USE_SEMAPHORES
 *       option is enabled in \p chconf.h.
 * @note This function does not reschedule.
 */
void chSemSignalI(Semaphore *sp) {

  if (sp->s_cnt++ < 0)
    chSchReadyI(fifo_remove(&sp->s_queue), RDY_OK);
}

#ifdef CH_USE_SEMSW
/**
 * Performs atomic signal and wait operations on two semaphores.
 * @param sps pointer to a \p Semaphore structure to be signaled
 * @param spw pointer to a \p Semaphore structure to be wait on
 * @note The function is available only if the \p CH_USE_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemSignalWait(Semaphore *sps, Semaphore *spw) {

  chSysLock();

  if (sps->s_cnt++ < 0)
    chSchReadyI(fifo_remove(&sps->s_queue), RDY_OK);

  if (--spw->s_cnt < 0) {
    fifo_insert(currp, &spw->s_queue);
    currp->p_wtsemp = spw;
    chSchGoSleepS(PRWTSEM);
  }
  else
    chSchRescheduleS();

  chSysUnlock();
}
#endif /* CH_USE_SEMSW */

#endif /* CH_USE_SEMAPHORES */

/** @} */
