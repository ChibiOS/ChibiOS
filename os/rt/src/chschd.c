/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    rt/src/chschd.c
 * @brief   Scheduler code.
 *
 * @addtogroup scheduler
 * @details This module provides the default portable scheduler code.
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   System data structures.
 */
ch_system_t ch;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*
 * Timeout wakeup callback.
 */
static void wakeup(void *p) {
  thread_t *tp = (thread_t *)p;

  chSysLockFromISR();
  switch (tp->state) {
  case CH_STATE_READY:
    /* Handling the special case where the thread has been made ready by
       another thread with higher priority.*/
    chSysUnlockFromISR();
    return;
  case CH_STATE_SUSPENDED:
    *tp->u.wttrp = NULL;
    break;
#if CH_CFG_USE_SEMAPHORES == TRUE
  case CH_STATE_WTSEM:
    chSemFastSignalI(tp->u.wtsemp);
#endif
    /* Falls through.*/
  case CH_STATE_QUEUED:
    /* Falls through.*/
#if (CH_CFG_USE_CONDVARS == TRUE) && (CH_CFG_USE_CONDVARS_TIMEOUT == TRUE)
  case CH_STATE_WTCOND:
#endif
    /* States requiring dequeuing.*/
    (void) ch_queue_dequeue(&tp->hdr.queue);
    break;
  default:
    /* Any other state, nothing to do.*/
    break;
  }
  tp->u.rdymsg = MSG_TIMEOUT;
  (void) chSchReadyI(tp);
  chSysUnlockFromISR();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if (CH_CFG_OPTIMIZE_SPEED == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   Inserts a thread into a priority ordered queue.
 * @note    The insertion is done by scanning the list from the highest
 *          priority toward the lowest.
 *
 * @param[in] tp        the pointer to the thread to be inserted in the list
 * @param[in] qp        the pointer to the threads list header
 *
 * @notapi
 */
void ch_sch_prio_insert(ch_queue_t *tp, ch_queue_t *qp) {

  ch_queue_t *cp = qp;
  do {
    cp = cp->next;
  } while ((cp != qp) &&
           (((thread_t *)cp)->hdr.pqueue.prio >= ((thread_t *)tp)->hdr.pqueue.prio));
  tp->next       = cp;
  tp->prev       = cp->prev;
  tp->prev->next = tp;
  cp->prev       = tp;
}
#endif /* CH_CFG_OPTIMIZE_SPEED */

/**
 * @brief   Scheduler initialization.
 *
 * @notapi
 */
void _scheduler_init(void) {

  ch_pqueue_init(&ch.rlist.pqueue);
#if CH_CFG_USE_REGISTRY == TRUE
  ch.rlist.newer = (thread_t *)&ch.rlist;
  ch.rlist.older = (thread_t *)&ch.rlist;
#endif
}

/**
 * @brief   Inserts a thread in the Ready List placing it behind its peers.
 * @details The thread is positioned behind all threads with higher or equal
 *          priority.
 * @pre     The thread must not be already inserted in any list through its
 *          @p next and @p prev or list corruption would occur.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] tp        the thread to be made ready
 * @return              The thread pointer.
 *
 * @iclass
 */
thread_t *chSchReadyI(thread_t *tp) {

  chDbgCheckClassI();
  chDbgCheck(tp != NULL);
  chDbgAssert((tp->state != CH_STATE_READY) &&
              (tp->state != CH_STATE_FINAL),
              "invalid state");

  /* The thread is marked ready.*/
  tp->state = CH_STATE_READY;

  /* Insertion in the priority queue.*/
  return (thread_t *)ch_pqueue_insert_behind(&ch.rlist.pqueue,
                                             &tp->hdr.pqueue);
}

/**
 * @brief   Inserts a thread in the Ready List placing it ahead its peers.
 * @details The thread is positioned ahead all threads with higher or equal
 *          priority.
 * @pre     The thread must not be already inserted in any list through its
 *          @p next and @p prev or list corruption would occur.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] tp        the thread to be made ready
 * @return              The thread pointer.
 *
 * @iclass
 */
thread_t *chSchReadyAheadI(thread_t *tp) {

  chDbgCheckClassI();
  chDbgCheck(tp != NULL);
  chDbgAssert((tp->state != CH_STATE_READY) &&
              (tp->state != CH_STATE_FINAL),
              "invalid state");

  /* The thread is marked ready.*/
  tp->state = CH_STATE_READY;

  /* Insertion in the priority queue.*/
  return (thread_t *)ch_pqueue_insert_ahead(&ch.rlist.pqueue,
                                            &tp->hdr.pqueue);
}

/**
 * @brief   Puts the current thread to sleep into the specified state.
 * @details The thread goes into a sleeping state. The possible
 *          @ref thread_states are defined into @p threads.h.
 *
 * @param[in] newstate  the new thread state
 *
 * @sclass
 */
void chSchGoSleepS(tstate_t newstate) {
  thread_t *otp = currp;

  chDbgCheckClassS();

  /* New state.*/
  otp->state = newstate;

#if CH_CFG_TIME_QUANTUM > 0
  /* The thread is renouncing its remaining time slices so it will have a new
     time quantum when it will wakeup.*/
  otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif

  /* Next thread in ready list becomes current.*/
  currp = (thread_t *)ch_pqueue_remove_highest(&ch.rlist.pqueue);
  currp->state = CH_STATE_CURRENT;

  /* Handling idle-enter hook.*/
  if (currp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_ENTER_HOOK();
  }

  /* Swap operation as tail call.*/
  chSysSwitch(currp, otp);
}

/**
 * @brief   Puts the current thread to sleep into the specified state with
 *          timeout specification.
 * @details The thread goes into a sleeping state, if it is not awakened
 *          explicitly within the specified timeout then it is forcibly
 *          awakened with a @p MSG_TIMEOUT low level message. The possible
 *          @ref thread_states are defined into @p threads.h.
 *
 * @param[in] newstate  the new thread state
 * @param[in] timeout   the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state, this is equivalent to invoking
 *                        @p chSchGoSleepS() but, of course, less efficient.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @return              The wakeup message.
 * @retval MSG_TIMEOUT  if a timeout occurs.
 *
 * @sclass
 */
msg_t chSchGoSleepTimeoutS(tstate_t newstate, sysinterval_t timeout) {

  chDbgCheckClassS();

  if (TIME_INFINITE != timeout) {
    virtual_timer_t vt;

    chVTDoSetI(&vt, timeout, wakeup, currp);
    chSchGoSleepS(newstate);
    if (chVTIsArmedI(&vt)) {
      chVTDoResetI(&vt);
    }
  }
  else {
    chSchGoSleepS(newstate);
  }

  return currp->u.rdymsg;
}

/**
 * @brief   Wakes up a thread.
 * @details The thread is inserted into the ready list or immediately made
 *          running depending on its relative priority compared to the current
 *          thread.
 * @pre     The thread must not be already inserted in any list through its
 *          @p next and @p prev or list corruption would occur.
 * @note    It is equivalent to a @p chSchReadyI() followed by a
 *          @p chSchRescheduleS() but much more efficient.
 * @note    The function assumes that the current thread has the highest
 *          priority.
 *
 * @param[in] ntp       the thread to be made ready
 * @param[in] msg       the wakeup message
 *
 * @sclass
 */
void chSchWakeupS(thread_t *ntp, msg_t msg) {
  thread_t *otp = currp;

  chDbgCheckClassS();

  chDbgAssert((ch.rlist.pqueue.next == &ch.rlist.pqueue) ||
              (ch.rlist.current->hdr.pqueue.prio >= ch.rlist.pqueue.next->prio),
              "priority order violation");

  /* Storing the message to be retrieved by the target thread when it will
     restart execution.*/
  ntp->u.rdymsg = msg;

  /* If the waken thread has a not-greater priority than the current
     one then it is just inserted in the ready list else it made
     running immediately and the invoking thread goes in the ready
     list instead.*/
  if (ntp->hdr.pqueue.prio <= otp->hdr.pqueue.prio) {
    (void) chSchReadyI(ntp);
  }
  else {
    otp = chSchReadyAheadI(otp);

    /* Handling idle-leave hook.*/
    if (otp->hdr.pqueue.prio == IDLEPRIO) {
      CH_CFG_IDLE_LEAVE_HOOK();
    }

    /* The extracted thread is marked as current.*/
    currp = ntp;
    ntp->state = CH_STATE_CURRENT;

    /* Swap operation as tail call.*/
    chSysSwitch(ntp, otp);
  }
}

/**
 * @brief   Performs a reschedule if a higher priority thread is runnable.
 * @details If a thread with a higher priority than the current thread is in
 *          the ready list then make the higher priority thread running.
 *
 * @sclass
 */
void chSchRescheduleS(void) {

  chDbgCheckClassS();

  if (chSchIsRescRequiredI()) {
    chSchDoRescheduleAhead();
  }
}

#if !defined(CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED)
/**
 * @brief   Evaluates if preemption is required.
 * @details The decision is taken by comparing the relative priorities and
 *          depending on the state of the round robin timeout counter.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @retval true         if there is a thread that must go in running state
 *                      immediately.
 * @retval false        if preemption is not required.
 *
 * @special
 */
bool chSchIsPreemptionRequired(void) {
  tprio_t p1 = firstprio(&ch.rlist.pqueue);
  tprio_t p2 = currp->hdr.pqueue.prio;

#if CH_CFG_TIME_QUANTUM > 0
  /* If the running thread has not reached its time quantum, reschedule only
     if the first thread on the ready queue has a higher priority.
     Otherwise, if the running thread has used up its time quantum, reschedule
     if the first thread on the ready queue has equal or higher priority.*/
  return (currp->ticks > (tslices_t)0) ? (p1 > p2) : (p1 >= p2);
#else
  /* If the round robin preemption feature is not enabled then performs a
     simpler comparison.*/
  return p1 > p2;
#endif
}
#endif /* !defined(CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED) */

/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind all
 *          threads having the same priority. The thread regains its time
 *          quantum.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @special
 */
void chSchDoRescheduleBehind(void) {
  thread_t *otp = currp;

  /* Picks the first thread from the ready queue and makes it current.*/
  currp = (thread_t *)ch_pqueue_remove_highest(&ch.rlist.pqueue);
  currp->state = CH_STATE_CURRENT;

  /* Handling idle-leave hook.*/
  if (otp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

#if CH_CFG_TIME_QUANTUM > 0
  /* It went behind peers so it gets a new time quantum.*/
  otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif

  /* Placing in ready list behind peers.*/
  otp = chSchReadyI(otp);

  /* Swap operation as tail call.*/
  chSysSwitch(currp, otp);
}

/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list ahead of all
 *          threads having the same priority.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @special
 */
void chSchDoRescheduleAhead(void) {
  thread_t *otp = currp;

  /* Picks the first thread from the ready queue and makes it current.*/
  currp = (thread_t *)ch_pqueue_remove_highest(&ch.rlist.pqueue);
  currp->state = CH_STATE_CURRENT;

  /* Handling idle-leave hook.*/
  if (otp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

  /* Placing in ready list ahead of peers.*/
  otp = chSchReadyAheadI(otp);

  /* Swap operation as tail call.*/
  chSysSwitch(currp, otp);
}

#if !defined(CH_SCH_DO_RESCHEDULE_HOOKED)
/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind or
 *          ahead of all threads having the same priority depending on
 *          if it used its whole time slice.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself or from within the port layer.
 *
 * @special
 */
void chSchDoReschedule(void) {
  thread_t *otp = currp;

  /* Picks the first thread from the ready queue and makes it current.*/
  currp = (thread_t *)ch_pqueue_remove_highest(&ch.rlist.pqueue);
  currp->state = CH_STATE_CURRENT;

  /* Handling idle-leave hook.*/
  if (otp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

#if CH_CFG_TIME_QUANTUM > 0
  /* If CH_CFG_TIME_QUANTUM is enabled then there are two different scenarios
     to handle on preemption: time quantum elapsed or not.*/
  if (otp->ticks == (tslices_t)0) {

    /* The thread consumed its time quantum so it is enqueued behind threads
       with same priority level, however, it acquires a new time quantum.*/
    otp = chSchReadyI(otp);

    /* The thread being swapped out receives a new time quantum.*/
    otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
  }
  else {
    /* The thread didn't consume all its time quantum so it is put ahead of
       threads with equal priority and does not acquire a new time quantum.*/
    otp = chSchReadyAheadI(otp);
  }
#else /* !(CH_CFG_TIME_QUANTUM > 0) */
  /* If the round-robin mechanism is disabled then the thread goes always
     ahead of its peers.*/
  otp = chSchReadyAheadI(otp);
#endif /* !(CH_CFG_TIME_QUANTUM > 0) */

  /* Swap operation as tail call.*/
  chSysSwitch(currp, otp);
}
#endif /* !defined(CH_SCH_DO_RESCHEDULE_HOOKED) */

/** @} */
