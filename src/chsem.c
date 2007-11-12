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

  chSysLock();

  cnt = sp->s_cnt;
  sp->s_cnt = n;
  if (cnt < 0) {
    while (cnt++)
      chSchReadyI(fifo_remove(&sp->s_queue))->p_rdymsg = RDY_RESET;
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

  cnt = sp->s_cnt;
  sp->s_cnt = n;
  while (cnt++ < 0)
    chSchReadyI(fifo_remove(&sp->s_queue))->p_rdymsg = RDY_RESET;
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 */
void chSemWait(Semaphore *sp) {

  chSysLock();

  if (--sp->s_cnt < 0) {
    fifo_insert(currp, &sp->s_queue);
    currp->p_semp = sp;
    chSchGoSleepS(PRWTSEM);
  }

  chSysUnlock();
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @note This function must be called with interrupts disabled.
 * @note This function cannot be called by an interrupt handler.
 */
void chSemWaitS(Semaphore *sp) {

  if (--sp->s_cnt < 0) {
    fifo_insert(currp, &sp->s_queue);
    currp->p_semp = sp;
    chSchGoSleepS(PRWTSEM);
  }
}

#ifdef CH_USE_SEMAPHORES_TIMEOUT
static void wakeup(void *p) {
#ifdef CH_USE_DEBUG
  if (((Thread *)p)->p_state != PRWTSEM)
    chDbgPanic("chsem.c, wakeup()\r\n");
#endif
  chSemFastSignalI(((Thread *)p)->p_semp);
  chSchReadyI(dequeue(p))->p_rdymsg = RDY_TIMEOUT;
}

/**
 * Performs a wait operation on a semaphore with timeout specification.
 * @param sp pointer to a \p Semaphore structure
 * @param time the number of ticks before the operation fails
 * @return the function can return \p RDY_OK. \p RDY_TIMEOUT or \p RDY_RESET.
 */
t_msg chSemWaitTimeout(Semaphore *sp, t_time time) {
  t_msg msg;

  chSysLock();

  if (--sp->s_cnt < 0) {
    VirtualTimer vt;

    chVTSetI(&vt, time, wakeup, currp);
    fifo_insert(currp, &sp->s_queue);
    currp->p_semp = sp;
    chSchGoSleepS(PRWTSEM);
    msg = currp->p_rdymsg;
    if (chVTIsArmedI(&vt))
      chVTResetI(&vt);

    chSysUnlock();
    return msg;
  }

  chSysUnlock();
  return RDY_OK;
}

/**
 * Performs a wait operation on a semaphore with timeout specification.
 * @param sp pointer to a \p Semaphore structure
 * @param time the number of ticks before the operation fails
 * @return the function can return \p RDY_OK. \p RDY_TIMEOUT or \p RDY_RESET.
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
    currp->p_semp = sp;
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
    chSchReadyI(fifo_remove(&sp->s_queue));
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
    chSchReadyI(fifo_remove(&sps->s_queue));

  if (--spw->s_cnt < 0) {
    fifo_insert(currp, &spw->s_queue);
    currp->p_semp = spw;
    chSchGoSleepS(PRWTSEM);
  }
  else
    chSchRescheduleS();

  chSysUnlock();
}
#endif /* CH_USE_SEMSW */

#ifdef CH_USE_RT_SEMAPHORES
/*
 * Inserts a thread into a list ordering it by priority.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tqp the pointer to the threads list header
 * @note Usually you dont need to use this function in the user code unless
 *       you want to create some custom threads synchronization mechanism.
 */
static void prioenq(Thread *tp, ThreadsQueue *tqp) {
  Thread *cp;

  cp = tqp->p_next;
  while ((cp != (Thread *)tqp) && (cp->p_prio >= tp->p_prio))
    cp = cp->p_next;
  // Insertion on p_prev
  tp->p_prev = (tp->p_next = cp)->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

/**
 * Performs a wait operation on a semaphore with priority boost.
 * @param sp pointer to a \p Semaphore structure
 * @note The function is available only if the \p CH_USE_RT_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemRaisePrioWait(Semaphore *sp) {

  chSysLock();

  if (--sp->s_cnt < 0) {
    prioenq(currp, &sp->s_queue);
    currp->p_semp = sp;
    chSchGoSleepS(PRWTSEM);
  }

  if (!currp->p_rtcnt++)
    currp->p_prio += MEPRIO;

  chSysUnlock();
}

/**
 * Performs a signal operation on a semaphore with return to normal priority.
 * @param sp pointer to a \p Semaphore structure
 * @note The function is available only if the \p CH_USE_RT_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemLowerPrioSignal(Semaphore *sp) {

  chSysLock();

  if (!--currp->p_rtcnt) {
    currp->p_prio -= MEPRIO;
    if (sp->s_cnt++ < 0)
      chSchReadyI(fifo_remove(&sp->s_queue));
    chSchRescheduleS();
  }
  else if (sp->s_cnt++ < 0)
    chSchWakeupS(fifo_remove(&sp->s_queue), RDY_OK);

  chSysUnlock();
}

#ifdef CH_USE_SEMSW
/**
 * Performs atomic signal and wait operations on two semaphores with priority
 * boost.
 * @param sps pointer to a \p Semaphore structure to be signaled
 * @param spw pointer to a \p Semaphore structure to be wait on
 * @note The function is available only if the \p CH_USE_RT_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemRaisePrioSignalWait(Semaphore *sps, Semaphore *spw) {

  chSysLock();

  if (sps->s_cnt++ < 0)
    chSchReadyI(fifo_remove(&sps->s_queue));

  if (--spw->s_cnt < 0) {
    prioenq(currp, &spw->s_queue);
    currp->p_semp = spw;
    chSchGoSleepS(PRWTSEM);

    if (!currp->p_rtcnt++)
      currp->p_prio += MEPRIO;
  }
  else {
    if (!currp->p_rtcnt++)
      currp->p_prio += MEPRIO;

    chSchRescheduleS(); // Really needed ?
  }

  chSysUnlock();
}

/**
 * Performs atomic signal and wait operations on two semaphores with return
 * to normal priority.
 * @param sps pointer to a \p Semaphore structure to be signaled
 * @param spw pointer to a \p Semaphore structure to be wait on
 * @note The function is available only if the \p CH_USE_RT_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemLowerPrioSignalWait(Semaphore *sps, Semaphore *spw) {

  chSysLock();

  if (!--currp->p_rtcnt)
    currp->p_prio -= MEPRIO;

  if (sps->s_cnt++ < 0)
    chSchReadyI(fifo_remove(&sps->s_queue));

  if (--spw->s_cnt < 0) {
    fifo_insert(currp, &spw->s_queue); // fifo_insert() because the spw is a normal sem.
    currp->p_semp = spw;
    chSchGoSleepS(PRWTSEM);
  }
  else
    chSchRescheduleS();

  chSysUnlock();
}
#endif /* CH_USE_SEMSW */

#endif /* CH_USE_RT_SEMAPHORES */

#endif /* CH_USE_SEMAPHORES */

/** @} */
