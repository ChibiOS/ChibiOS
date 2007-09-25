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
void chSemInit(Semaphore *sp, t_semcnt n) {

  sp->s_cnt = n;
  sp->s_queue.p_next = sp->s_queue.p_prev = (Thread *)&sp->s_queue;
}

/**
 * Performs a reset operation on the semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @param n the new value of the semaphore counter. Must be non-negative.
 * @note The released threads can recognize they were waked up by a reset
 *       instead than a signal because the \p p_rdymsg field is set to
 *       \p RDY_RESET.
 */
void chSemReset(Semaphore *sp, t_semcnt n) {
  t_semcnt cnt;

  chSysLock();

  cnt = sp->s_cnt;
  sp->s_cnt = n;
  if (cnt < 0) {
    while (cnt++)
      chSchReadyI(dequeue(sp->s_queue.p_next))->p_rdymsg = RDY_RESET;
    chSchRescheduleI();
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
 * @note This function must be called with interrupts disabled.
 */
void chSemResetI(Semaphore *sp, t_semcnt n) {
  t_semcnt cnt;

  cnt = sp->s_cnt;
  sp->s_cnt = n;
  while (cnt++ < 0)
    chSchReadyI(dequeue(sp->s_queue.p_next))->p_rdymsg = RDY_RESET;
}

/**
 * Performs a wait operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 */
void chSemWait(Semaphore *sp) {

  chSysLock();

  if (--sp->s_cnt < 0) {
    enqueue(currp, &sp->s_queue);
    chSchGoSleepI(PRWTSEM);
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
    enqueue(currp, &sp->s_queue);
    chSchGoSleepI(PRWTSEM);
  }
}

#ifdef CH_USE_SEMAPHORES_TIMEOUT
static void unwait(void *p) {

// Test removed, it should never happen.
//  if (((Thread *)p)->p_state == PRWTSEM)
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

    chVTSetI(&vt, time, unwait, currp);
    enqueue(currp, &sp->s_queue);
    chSchGoSleepI(PRWTSEM);
    msg = currp->p_rdymsg; // Note, got value *before* invoking CH_LEAVE_SYSTEM().
    if (!vt.vt_func) {
      sp->s_cnt++;

      chSysUnlock();
      return msg;
    }
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

    chVTSetI(&vt, time, unwait, currp);
    enqueue(currp, &sp->s_queue);
    chSchGoSleepI(PRWTSEM);
    if (!vt.vt_func) {
      sp->s_cnt++;
      return currp->p_rdymsg;
    }
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
    chSchWakeupI(dequeue(sp->s_queue.p_next), RDY_OK);

  chSysUnlock();
}

/**
 * Performs a signal operation on a semaphore.
 * @param sp pointer to a \p Semaphore structure
 * @note This function must be called with interrupts disabled.
 * @note The function is available only if the \p CH_USE_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemSignalI(Semaphore *sp) {

  if (sp->s_cnt++ < 0)
    chSchReadyI(dequeue(sp->s_queue.p_next));
}

/**
 * Performs atomic signal and wait operations on two semaphores.
 * @param sps pointer to a \p Semaphore structure to be signaled
 * @param spw pointer to a \p Semaphore structure to be wait on
 * @note The function is available only if the \p CH_USE_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemSignalWait(Semaphore *sps, Semaphore *spw) {
  BOOL flag;

  chSysLock();

  if (sps->s_cnt++ < 0)
    chSchReadyI(dequeue(sps->s_queue.p_next)), flag = TRUE;
  else
    flag = FALSE;

  if (--spw->s_cnt < 0) {
    enqueue(currp, &spw->s_queue);
    chSchGoSleepI(PRWTSEM);
  }
  else if (flag)
    chSchRescheduleI();

  chSysUnlock();
}

#ifdef CH_USE_RT_SEMAPHORES
/*
 * Inserts a thread into a list ordering it by priority.
 * @param tp the pointer to the thread to be inserted in the list
 * @param tqp the pointer to the threads list header
 * @note Usually you dont need to use this function in the user code unless
 *       you want to create some custom threads synchronization mechanism.
 */
static void prioenq(Thread *tp, ThreadsQueue *tqp) {
  Thread *p;

  p = tqp->p_next;
  while ((p != (Thread *)tqp) && (p->p_prio >= tp->p_prio))
    p = p->p_next;
  tp->p_next = p;
  tp->p_prev = tqp->p_prev;
  p->p_prev->p_next = tp;
  p->p_prev = tp;
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
    chSchGoSleepI(PRWTSEM);
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
    currp->p_prio = currp->p_bakprio;
    if (sp->s_cnt++ < 0)
      chSchReadyI(dequeue(sp->s_queue.p_next));
    chSchRescheduleI();
  }
  else if (sp->s_cnt++ < 0)
    chSchWakeupI(dequeue(sp->s_queue.p_next), RDY_OK);

  chSysUnlock();
}

/**
 * Performs atomic signal and wait operations on two semaphores with priority
 * boost.
 * @param sps pointer to a \p Semaphore structure to be signaled
 * @param spw pointer to a \p Semaphore structure to be wait on
 * @note The function is available only if the \p CH_USE_RT_SEMAPHORES
 *       option is enabled in \p chconf.h.
 */
void chSemRaisePrioSignalWait(Semaphore *sps, Semaphore *spw) {
  BOOL flag;

  chSysLock();

  if (sps->s_cnt++ < 0)
    chSchReadyI(dequeue(sps->s_queue.p_next)), flag = TRUE;
  else
    flag = FALSE;

  if (--spw->s_cnt < 0) {
    prioenq(currp, &spw->s_queue);
    chSchGoSleepI(PRWTSEM);

    if (!currp->p_rtcnt++)
      currp->p_prio += MEPRIO;

    chSysUnlock();
    return;
  }

  if (!currp->p_rtcnt++) {
    currp->p_bakprio = currp->p_prio;
    currp->p_prio += MEPRIO;
    flag = TRUE;
  }

  if( flag)
    chSchRescheduleI();

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
  BOOL flag = FALSE;

  chSysLock();

  if (!--currp->p_rtcnt)
    currp->p_prio = currp->p_bakprio, flag = TRUE;

  if (sps->s_cnt++ < 0)
    chSchReadyI(dequeue(sps->s_queue.p_next)), flag = TRUE;

  if (--spw->s_cnt < 0) {
    enqueue(currp, &spw->s_queue); // enqueue() because the spw is a normal sem.
    chSchGoSleepI(PRWTSEM);
  }
  else if (flag)
    chSchRescheduleI();

  chSysUnlock();
}

#endif /* CH_USE_RT_SEMAPHORES */

#endif /* CH_USE_SEMAPHORES */

/** @} */
