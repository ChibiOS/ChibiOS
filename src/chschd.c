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
 * @addtogroup Scheduler
 * @{
 */

#include <ch.h>

/** @cond never */
ReadyList rlist;
/** @endcond */

/**
 * Scheduler initialization.
 * @note Internally invoked by the \p chSysInit().
 */
void chSchInit(void) {

  queue_init(&rlist);
  rlist.r_prio = NOPRIO;
#ifdef CH_USE_ROUNDROBIN
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
}

/**
 * Inserts a thread in the Ready List.
 *
 * @param tp the Thread to be made ready
 * @return The Thread pointer.
 * @note The function must be called in the system mutex zone.
 * @note The function does not reschedule, the \p chSchRescheduleS() should
 *       be called soon after.
 * @note The function is not meant to be used in the user code directly.
 */
#ifdef CH_OPTIMIZE_SPEED
/* NOTE: it is inlined in this module only.*/
INLINE Thread *chSchReadyI(Thread *tp) {
#else
Thread *chSchReadyI(Thread *tp) {
#endif
  Thread *cp;

  tp->p_state = PRREADY;
  cp = rlist.p_next;
  while (cp->p_prio >= tp->p_prio)
    cp = cp->p_next;
  /* Insertion on p_prev.*/
  tp->p_prev = (tp->p_next = cp)->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
  return tp;
}

/**
 * Puts the current thread to sleep into the specified state, the next highest
 * priority thread becomes running. The threads states are described into
 * \p threads.h
 * @param newstate the new thread state
 * @note The function must be called in the system mutex zone.
 * @note The function is not meant to be used in the user code directly.
 */
void chSchGoSleepS(tstate_t newstate) {
  Thread *otp;

  (otp = currp)->p_state = newstate;
  (currp = fifo_remove((void *)&rlist))->p_state = PRCURR;
#ifdef CH_USE_ROUNDROBIN
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
#ifdef CH_USE_TRACE
  chDbgTrace(otp, currp);
#endif
  chSysSwitchI(otp, currp);
}

/*
 * Timeout wakeup callback.
 */
static void wakeup(void *p) {
  Thread *tp = (Thread *)p;

  switch (tp->p_state) {
#ifdef CH_USE_SEMAPHORES
  case PRWTSEM:
    chSemFastSignalI(tp->p_wtsemp);
    /* Falls into, intentional. */
#endif
  case PRWTCOND:
  case PRWTMTX:
    /* States requiring dequeuing. */
    dequeue(tp);
    /* Falls into, intentional. */
  default:
    chSchReadyI(tp)->p_rdymsg = RDY_TIMEOUT;
  }
}

/**
 * Puts the current thread to sleep.
 *
 * Puts the current thread to sleep into the specified state. The next highest
 * priority thread becomes running. The thread put to sleep is awakened after
 * the specified time has elapsed.
 *
 * @param newstate the new thread state
 * @param time the number of ticks before the operation timeouts. the value
 *             zero (\p TIME_INFINITE) is allowed.
 * @return The wakeup message.
 * @retval RDY_TIMEOUT if a timeout occurs.
 * @note The function must be called in the system mutex zone.
 * @note The function is not meant to be used in the user code directly.
 */
msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t time) {

  if (TIME_INFINITE != time) {
    VirtualTimer vt;

    chVTSetI(&vt, time, wakeup, currp);
    chSchGoSleepS(newstate);
    if (chVTIsArmedI(&vt))
      chVTResetI(&vt);
  }
  else
    chSchGoSleepS(newstate);
  return currp->p_rdymsg;
}

/**
 * Wakes up a thread.
 *
 * The thread is inserted into the ready list or immediately made running
 * depending on its relative priority compared to the current thread.
 * @param ntp the Thread to be made ready
 * @param msg message to the awakened thread
 * @note The function must be called in the system mutex zone.
 * @note The function is not meant to be used in the user code directly.
 * @note It is equivalent to a \p chSchReadyI() followed by a
 *       \p chSchRescheduleS() but much more efficient.
 */
void chSchWakeupS(Thread *ntp, msg_t msg) {
  ntp->p_rdymsg = msg;
  if (ntp->p_prio <= currp->p_prio)
    /* the woken thread has equal or lower priority than the running thread */
    chSchReadyI(ntp);
  else {
    /* the woken thread has higher priority than the running thread and thus
     * preempts the currently running thread. */
    Thread *otp = currp;
    chSchReadyI(otp);
    /* change the to-be-run thread to running state */
    (currp = ntp)->p_state = PRCURR;
#ifdef CH_USE_ROUNDROBIN
    rlist.r_preempt = CH_TIME_QUANTUM;
#endif
#ifdef CH_USE_TRACE
    /* trace the context switch */
    chDbgTrace(otp, ntp);
#endif
    /* switch the thread context */
    chSysSwitchI(otp, ntp);
  }
}

/**
 * Switch to the first thread on the runnable queue.
 *
 * @note It is intended to be called if \p chSchRescRequiredI() evaluates to \p TRUE.
 */
void chSchDoRescheduleI(void) {

  Thread *otp = currp;
  /* pick the first thread from the ready queue and makes it current */
  (currp = fifo_remove((void *)&rlist))->p_state = PRCURR;
  chSchReadyI(otp);
#ifdef CH_USE_ROUNDROBIN
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
#ifdef CH_USE_TRACE
  chDbgTrace(otp, currp);
#endif
  chSysSwitchI(otp, currp);
}

/**
 * Reschedule only if a higher priority thread is runnable.
 *
 * If a thread with a higher priority than the current thread is in the
 * ready list then make the higher priority thread running.
 *
 * @note The function must be called in the system mutex zone.
 */
void chSchRescheduleS(void) {
  /* first thread in the runnable queue has higher priority than the running
   * thread? */
  if (firstprio(&rlist) > currp->p_prio)
    chSchDoRescheduleI();
}

/**
 * Evaluates if rescheduling is required.
 *
 * @retval TRUE if there is a thread that should go in running state.
 * @retval FALSE if a reschedulation is not required.
 */
bool_t chSchRescRequiredI(void) {
  tprio_t p1 = firstprio(&rlist);
  tprio_t p2 = currp->p_prio;
#ifdef CH_USE_ROUNDROBIN
  /* If the running thread has not reached its time quantum, reschedule only
   * if the first thread on the ready queue has a higher priority.
   * Otherwise, if the running thread has used up its time quantum, reschedule
   * if the first thread on the ready queue has equal or higher priority.
   */
  return rlist.r_preempt ? p1 > p2 : p1 >= p2;
#else
  return p1 > p2;
#endif
}

/** @} */
