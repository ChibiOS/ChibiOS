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
 * @addtogroup Scheduler
 * @{
 */

#include <ch.h>

/** @cond never*/

static ReadyList rlist;

#ifndef CH_CURRP_REGISTER_CACHE
Thread *currp;
#endif

#ifdef CH_USE_SYSTEMTIME
volatile t_time stime;
#endif

/** @endcond */

/**
 * Scheduler initialization.
 * @note Internally invoked by the \p chSysInit().
 */
void chSchInit(void) {

  fifo_init(&rlist.r_queue);
  rlist.r_prio = ABSPRIO;
  rlist.r_preempt = CH_TIME_QUANTUM;
#ifdef CH_USE_SYSTEMTIME
  stime = 0;
#endif
}

/**
 * Inserts a thread in the Ready List.
 * @param tp the Thread to be made ready
 * @return the Thread pointer
 * @note The function must be called in the system mutex zone.
 * @note The function does not reschedule, the \p chSchRescheduleI() should
 *       be called soon after.
 * @note The function is not meant to be used in the user code directly.
 */
Thread *chSchReadyI(Thread *tp) {
  Thread *cp;
  t_prio prio = tp->p_prio;

  tp->p_state = PRREADY;
  tp->p_rdymsg = RDY_OK;
  cp = rlist.r_queue.p_prev;
  while (cp->p_prio < prio)
    cp = cp->p_prev;
  // Insertion on p_next
  tp->p_next = (tp->p_prev = cp)->p_next;
  tp->p_next->p_prev = cp->p_next = tp;
  return tp;
}

/*
 * Switches to the next thread in the ready list, the ready list is assumed
 * to contain at least a thread.
 */
#ifdef CH_OPTIMIZE_SPEED
static INLINE void nextready(void) {
#else
static void nextready(void) {
#endif
  Thread *otp = currp;

  (currp = fifo_remove(&rlist.r_queue))->p_state = PRCURR;
  rlist.r_preempt = CH_TIME_QUANTUM;
#ifdef CH_USE_DEBUG
  chDbgTrace(otp, currp);
#endif
  chSysSwitchI(&otp->p_ctx, &currp->p_ctx);
}

/**
 * Puts the current thread to sleep into the specified state, the next highest
 * priority thread becomes running. The threads states are described into
 * \p threads.h
 * @param newstate the new thread state
 * @return the wakeup message
 * @note The function must be called in the system mutex zone.
 * @note The function is not meant to be used in the user code directly.
 */
void chSchGoSleepS(t_tstate newstate) {

  currp->p_state = newstate;
  nextready();
}

/**
 * Wakeups a thread, the thread is inserted into the ready list or made
 * running directly depending on its relative priority compared to the current
 * thread.
 * @param ntp the Thread to be made ready
 * @param msg wakeup message to the awakened thread
 * @note The function must be called in the system mutex zone.
 * @note The function is not meant to be used in the user code directly.
 * @note It is equivalent to a \p chSchReadyI() followed by a
 *       \p chSchRescheduleI() but much more efficient.
 */
void chSchWakeupS(Thread *ntp, t_msg msg) {

  if (ntp->p_prio <= currp->p_prio)
    chSchReadyI(ntp)->p_rdymsg = msg;
  else {
    Thread *otp = currp;
    chSchReadyI(otp);
    (currp = ntp)->p_state = PRCURR;
    ntp->p_rdymsg = msg;
    rlist.r_preempt = CH_TIME_QUANTUM;
#ifdef CH_USE_DEBUG
    chDbgTrace(otp, ntp);
#endif
    chSysSwitchI(&otp->p_ctx, &ntp->p_ctx);
  }
}

/**
 * If a thread with an higher priority than the current thread is in the
 * ready list then it becomes running.
 * @note The function must be called in the system mutex zone.
 */
void chSchRescheduleS(void) {

  if (isempty(&rlist.r_queue) || firstprio(&rlist.r_queue) <= currp->p_prio)
    return;

  chSchDoRescheduleI();
}

/**
 * Performs a reschedulation. It is meant to be called if
 * \p chSchRescRequired() evaluates to \p TRUE.
 */
void chSchDoRescheduleI(void) {

  chSchReadyI(currp);
  nextready();
}

/**
 * Evaluates if a reschedulation is required.
 * @return \p TRUE if there is a thread that should go in running state
 *         immediatly else \p FALSE.
 */
BOOL chSchRescRequiredI(void) {

  if (isempty(&rlist.r_queue))
    return FALSE;

  if (rlist.r_preempt) {
    if (firstprio(&rlist.r_queue) <= currp->p_prio)
      return FALSE;
  }
  else { /* Time quantum elapsed. */
    if (firstprio(&rlist.r_queue) < currp->p_prio)
      return FALSE;
  }
  return TRUE;
}

/**
 * Preemption routine, this function must be called into an interrupt
 * handler invoked by a system timer.
 * The frequency of the timer determines the system tick granularity and,
 * together with the \p CH_TIME_QUANTUM macro, the round robin interval.
 */
void chSchTimerHandlerI(void) {

  if (rlist.r_preempt)
    rlist.r_preempt--;

#ifdef CH_USE_SYSTEMTIME
  stime++;
#endif

#ifdef CH_USE_VIRTUAL_TIMERS
  chVTDoTickI();
#endif
}

/** @} */
