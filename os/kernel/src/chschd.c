/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file chschd.c
 * @brief Scheduler code.
 * @addtogroup scheduler
 * @{
 */

#include "ch.h"

/** @cond never */
ReadyList rlist;
/** @endcond */

/**
 * @brief Scheduler initialization.
 *
 * @note Internally invoked by the @p chSysInit().
 */
void scheduler_init(void) {

  queue_init(&rlist.r_queue);
  rlist.r_prio = NOPRIO;
#if CH_TIME_QUANTUM > 0
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
}

/**
 * @brief Inserts a thread in the Ready List.
 *
 * @param[in] tp the Thread to be made ready
 * @return The Thread pointer.
 * @note The function does not reschedule, the @p chSchRescheduleS() should
 *       be called soon after.
 */
#if CH_OPTIMIZE_SPEED
/* NOTE: it is inlined in this module only.*/
INLINE Thread *chSchReadyI(Thread *tp) {
#else
Thread *chSchReadyI(Thread *tp) {
#endif
  Thread *cp;

  tp->p_state = PRREADY;
  cp = (Thread *)&rlist.r_queue;
  do {
    cp = cp->p_next;
  } while (cp->p_prio >= tp->p_prio);
  /* Insertion on p_prev.*/
  tp->p_prev = (tp->p_next = cp)->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
  return tp;
}

/**
 * @brief Puts the current thread to sleep into the specified state.
 * @details The thread goes into a sleeping state. The @ref thread_states are
 *          described into @p threads.h.
 *
 * @param[in] newstate the new thread state
 */
void chSchGoSleepS(tstate_t newstate) {
  Thread *otp;

  (otp = currp)->p_state = newstate;
  (currp = fifo_remove(&rlist.r_queue))->p_state = PRCURR;
#if CH_TIME_QUANTUM > 0
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
  chDbgTrace(otp, currp);
  chSysSwitchI(otp, currp);
}

/*
 * Timeout wakeup callback.
 */
static void wakeup(void *p) {
  Thread *tp = (Thread *)p;

#if CH_USE_SEMAPHORES || CH_USE_MUTEXES || CH_USE_CONDVARS
  switch (tp->p_state) {
#if CH_USE_SEMAPHORES
  case PRWTSEM:
    chSemFastSignalI(tp->p_wtsemp);
    /* Falls into, intentional. */
#endif
#if CH_USE_MUTEXES
  case PRWTMTX:
#endif
#if CH_USE_CONDVARS
  case PRWTCOND:
#endif
    /* States requiring dequeuing.*/
    dequeue(tp);
  }
#endif
  chSchReadyI(tp)->p_rdymsg = RDY_TIMEOUT;
}

/**
 * @brief Puts the current thread to sleep into the specified state with
 *        timeout specification.
 * @details The thread goes into a sleeping state, if it is not awakened
 *          explicitly within the specified timeout then it is forcibly
 *          awakened with a @p RDY_TIMEOUT low level message. The @ref
 *          thread_states are described into @p threads.h.
 *
 * @param[in] newstate the new thread state
 * @param[in] time the number of ticks before the operation timeouts, the
 *                 special values are handled as follow:
 *                 - @a TIME_INFINITE the thread enters an infinite sleep
 *                   state, this is equivalent to invoking @p chSchGoSleepS()
 *                   but, of course, less efficient.
 *                 - @a TIME_IMMEDIATE this value is accepted but interpreted
 *                   as a normal time specification not as an immediate timeout
 *                   specification.
 *                 .
 * @return The wakeup message.
 * @retval RDY_TIMEOUT if a timeout occurs.
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
 * @brief Wakes up a thread.
 * @details The thread is inserted into the ready list or immediately made
 *          running depending on its relative priority compared to the current
 *          thread.
 *
 * @param[in] ntp the Thread to be made ready
 * @param[in] msg message to the awakened thread
 * @note It is equivalent to a @p chSchReadyI() followed by a
 *       @p chSchRescheduleS() but much more efficient.
 * @note The function assumes that the current thread has the highest priority
 */
void chSchWakeupS(Thread *ntp, msg_t msg) {

  ntp->p_rdymsg = msg;
  /* If the waken thread has a not-greater priority than the current
   * one then it is just inserted in the ready list else it made
   * running immediately and the invoking thread goes in the ready
   * list instead.*/
  if (ntp->p_prio <= currp->p_prio)
    chSchReadyI(ntp);
  else {
    Thread *otp = currp;
    chSchReadyI(otp);
#if CH_TIME_QUANTUM > 0
    rlist.r_preempt = CH_TIME_QUANTUM;
#endif
    (currp = ntp)->p_state = PRCURR;
    chDbgTrace(otp, ntp);
    chSysSwitchI(otp, ntp);
  }
}

/**
 * @brief Switches to the first thread on the runnable queue.
 *
 * @note It is intended to be called if @p chSchRescRequiredI() evaluates to
 *       @p TRUE.
 */
void chSchDoRescheduleI(void) {

  Thread *otp = currp;
  /* Pick the first thread from the ready queue and makes it current.*/
  (currp = fifo_remove(&rlist.r_queue))->p_state = PRCURR;
  chSchReadyI(otp);
#if CH_TIME_QUANTUM > 0
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
  chDbgTrace(otp, currp);
  chSysSwitchI(otp, currp);
}

/**
 * @brief Performs a reschedule if a higher priority thread is runnable.
 * @details If a thread with a higher priority than the current thread is in
 *          the ready list then make the higher priority thread running.
 */
void chSchRescheduleS(void) {

  if (chSchIsRescRequiredI())
    chSchDoRescheduleI();
}

/**
 * @brief Evaluates if a reschedule is required.
 * @details The decision is taken by comparing the relative priorities and
 *          depending on the state of the round robin timeout counter.
 *
 * @retval TRUE if there is a thread that should go in running state.
 * @retval FALSE if a reschedule is not required.
 *
 * @note This function is meant to be used in the timer interrupt handler
 *       where @p chVTDoTickI() is invoked.
 */
bool_t chSchIsRescRequiredExI(void) {
  tprio_t p1 = firstprio(&rlist.r_queue);
  tprio_t p2 = currp->p_prio;
#if CH_TIME_QUANTUM > 0
  /* If the running thread has not reached its time quantum, reschedule only
   * if the first thread on the ready queue has a higher priority.
   * Otherwise, if the running thread has used up its time quantum, reschedule
   * if the first thread on the ready queue has equal or higher priority.*/
  return rlist.r_preempt ? p1 > p2 : p1 >= p2;
#else
  /* If the round robin feature is not enabled then performs a simpler
   * comparison.*/
  return p1 > p2;
#endif
}

/**
 * @brief Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal priority, if any.
 */
void chSchDoYieldS(void) {

  if (chSchCanYieldS()) {
    Thread *cp = (Thread *)&rlist.r_queue;
    Thread *otp = currp;

    /*
     * Note, the following insertion code works because we know that on the
     * ready list there is at least one thread with priority equal or higher
     * than the current one.
     */
    otp->p_state = PRREADY;
    do {
      cp = cp->p_prev;
    } while (cp->p_prio < otp->p_prio);
    /* Insertion on p_next.*/
    otp->p_next = (otp->p_prev = cp)->p_next;
    otp->p_next->p_prev = cp->p_next = otp;

    /* Pick the first thread from the ready queue and makes it current.*/
    (currp = fifo_remove(&rlist.r_queue))->p_state = PRCURR;
#if CH_TIME_QUANTUM > 0
    rlist.r_preempt = CH_TIME_QUANTUM;
#endif
    chDbgTrace(otp, currp);
    chSysSwitchI(otp, currp);
  }
}

/** @} */
