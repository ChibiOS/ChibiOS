/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

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

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
/**
 * @brief   This function implements the idle thread infinite loop.
 * @details The function puts the processor in the lowest power mode capable
 *          to serve interrupts.<br>
 *          The priority is internally set to the minimum system value so
 *          that this thread is executed only if there are no other ready
 *          threads in the system.
 *
 * @param[in] p         the thread parameter, unused in this scenario
 */
static void __idle_thread(void *p) {

  (void)p;

  while (true) {
    /*lint -save -e522 [2.2] Apparently no side effects because it contains
      an asm instruction.*/
    port_wait_for_interrupt();
    /*lint -restore*/
    CH_CFG_IDLE_LOOP_HOOK();
  }
}
#endif /* CH_CFG_NO_IDLE_THREAD == FALSE */

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
 * @param[in] oip       pointer to the OS instance
 * @param[in] tp        the thread to be made ready
 * @return              The thread pointer.
 *
 * @notapi
 */
__attribute__((noinline))
static thread_t *__sch_ready_behind(os_instance_t *oip, thread_t *tp) {

  chDbgAssert((tp->state != CH_STATE_READY) &&
              (tp->state != CH_STATE_FINAL),
              "invalid state");

  /* Tracing the event.*/
  __trace_ready(tp, tp->u.rdymsg);

  /* The thread is marked ready.*/
  tp->state = CH_STATE_READY;

  /* Insertion in the priority queue.*/
  return (thread_t *)ch_pqueue_insert_behind(&oip->rlist.pqueue,
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
 * @param[in] oip       pointer to the OS instance
 * @param[in] tp        the thread to be made ready
 * @return              The thread pointer.
 *
 * @notapi
 */
__attribute__((noinline))
static thread_t *__sch_ready_ahead(os_instance_t *oip, thread_t *tp) {

  chDbgAssert((tp->state != CH_STATE_READY) &&
              (tp->state != CH_STATE_FINAL),
              "invalid state");

  /* Tracing the event.*/
  __trace_ready(tp, tp->u.rdymsg);

  /* The thread is marked ready.*/
  tp->state = CH_STATE_READY;

  /* Insertion in the priority queue.*/
  return (thread_t *)ch_pqueue_insert_ahead(&oip->rlist.pqueue,
                                            &tp->hdr.pqueue);
}

/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind all
 *          threads having the same priority. The thread regains its time
 *          quantum.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @param[in] oip       pointer to the OS instance
 *
 * @notapi
 */
static void __sch_reschedule_behind(os_instance_t *oip) {
  thread_t *otp = __sch_get_currthread(oip);
  thread_t *ntp;

  /* Picks the first thread from the ready queue and makes it current.*/
  ntp = (thread_t *)ch_pqueue_remove_highest(&oip->rlist.pqueue);
  ntp->state = CH_STATE_CURRENT;
  __sch_set_currthread(oip, ntp);

  /* Handling idle-leave hook.*/
  if (otp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

#if CH_CFG_TIME_QUANTUM > 0
  /* It went behind peers so it gets a new time quantum.*/
  otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif

  /* Placing in ready list behind peers.*/
  otp = __sch_ready_behind(oip, otp);

  /* Swap operation as tail call.*/
  chSysSwitch(ntp, otp);
}

/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list ahead of all
 *          threads having the same priority.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @param[in] oip       pointer to the OS instance
 *
 * @notapi
 */
static void __sch_reschedule_ahead(os_instance_t *oip) {
  thread_t *otp = __sch_get_currthread(oip);
  thread_t *ntp;

  /* Picks the first thread from the ready queue and makes it current.*/
  ntp = (thread_t *)ch_pqueue_remove_highest(&oip->rlist.pqueue);
  ntp->state = CH_STATE_CURRENT;
  __sch_set_currthread(oip, ntp);

  /* Handling idle-leave hook.*/
  if (otp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

  /* Placing in ready list ahead of peers.*/
  otp = __sch_ready_ahead(oip, otp);

  /* Swap operation as tail call.*/
  chSysSwitch(ntp, otp);
}

/*
 * Timeout wakeup callback.
 */
static void __sch_wakeup(void *p) {
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

  /* Standard message for timeout conditions.*/
  tp->u.rdymsg = MSG_TIMEOUT;

  /* Goes behind peers because it went to sleep voluntarily.*/
  (void) __sch_ready_behind(currcore, tp);
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
 * @param[in] tqp       the pointer to the threads list header
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
 * @brief   Initializes a system instance.
 * @note    The system instance is in I-Lock state after initialization.
 *
 * @param[out] oip      pointer to the @p os_instance_t structure
 * @param[in] oicp      pointer to the @p os_instance_config_t structure
 *
 * @special
 */
void chSchObjectInit(os_instance_t *oip,
                     const os_instance_config_t *oicp) {

  /* Port initialization for the current instance.*/
  port_init(oip);

  /* Ready list initialization.*/
  ch_pqueue_init(&oip->rlist.pqueue);

  /* Registry initialization.*/
#if CH_CFG_USE_REGISTRY == TRUE
  oip->rlist.newer = (thread_t *)&oip->rlist;
  oip->rlist.older = (thread_t *)&oip->rlist;
#endif

  /* Virtual timers list initialization.*/
  __vt_object_init(&oip->vtlist);

  /* Debug support initialization.*/
  __dbg_object_init(&oip->dbg);

#if CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_DISABLED
  /* Trace buffer initialization.*/
  __trace_init(oip);
#endif

  /* Time Measurement initialization.*/
#if CH_CFG_USE_TM == TRUE
  __tm_calibration_init(&oip->tmc);
#endif

  /* Statistics initialization.*/
#if CH_DBG_STATISTICS == TRUE
  __stats_object_init(&oip->kernel_stats);
#endif

#if CH_CFG_NO_IDLE_THREAD == FALSE
  /* Now this instructions flow becomes the main thread.*/
#if CH_CFG_USE_REGISTRY == TRUE
  oip->rlist.current = __thd_object_init(oip, &oip->mainthread,
                                         (const char *)&ch_debug, NORMALPRIO);
#else
  oip->rlist.current = __thd_object_init(oip, &oip->mainthread,
                                         "main", NORMALPRIO);
#endif
#else
  /* Now this instructions flow becomes the idle thread.*/
  oip->rlist.current = __thd_object_init(oip, &oip->mainthread,
                                         "idle", IDLEPRIO);
#endif

#if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE)
  oip->rlist.current->wabase = oicp->mainthread_base;
#endif

  /* Setting up the caller as current thread.*/
  oip->rlist.current->state = CH_STATE_CURRENT;

  /* User instance initialization hook.*/
  CH_CFG_OS_INSTANCE_INIT_HOOK(oip);

#if CH_CFG_NO_IDLE_THREAD == FALSE
  {
    thread_descriptor_t idle_descriptor = {
      .name     = "idle",
      .wbase    = oicp->idlethread_base,
      .wend     = oicp->idlethread_end,
      .prio     = IDLEPRIO,
      .funcp    = __idle_thread,
      .arg      = NULL,
    };

    /* This thread has the lowest priority in the system, its role is just to
       serve interrupts in its context while keeping the lowest energy saving
       mode compatible with the system status.*/
    (void) chThdCreateI(&idle_descriptor);
  }
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
  os_instance_t *oip = currcore;

  chDbgCheckClassI();
  chDbgCheck(tp != NULL);

  /* The thread is handled by the local core.*/
  return __sch_ready_behind(oip, tp);
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
  os_instance_t *oip = currcore;
  thread_t *otp = __sch_get_currthread(oip);
  thread_t *ntp;

  chDbgCheckClassS();

  /* New state.*/
  otp->state = newstate;

#if CH_CFG_TIME_QUANTUM > 0
  /* The thread is renouncing its remaining time slices so it will have a new
     time quantum when it will wakeup.*/
  otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif

  /* Next thread in ready list becomes current.*/
  ntp = (thread_t *)ch_pqueue_remove_highest(&oip->rlist.pqueue);
  ntp->state = CH_STATE_CURRENT;
  __sch_set_currthread(oip, ntp);

  /* Handling idle-enter hook.*/
  if (ntp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_ENTER_HOOK();
  }

  /* Swap operation as tail call.*/
  chSysSwitch(ntp, otp);
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
  thread_t *tp = __sch_get_currthread(currcore);

  chDbgCheckClassS();

  if (TIME_INFINITE != timeout) {
    virtual_timer_t vt;

    chVTDoSetI(&vt, timeout, __sch_wakeup, (void *)tp);
    chSchGoSleepS(newstate);
    if (chVTIsArmedI(&vt)) {
      chVTDoResetI(&vt);
    }
  }
  else {
    chSchGoSleepS(newstate);
  }

  return tp->u.rdymsg;
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
  os_instance_t *oip = currcore;
  thread_t *otp = __sch_get_currthread(oip);

  chDbgCheckClassS();

  chDbgAssert((oip->rlist.pqueue.next == &oip->rlist.pqueue) ||
              (oip->rlist.current->hdr.pqueue.prio >= oip->rlist.pqueue.next->prio),
              "priority order violation");

  /* Storing the message to be retrieved by the target thread when it will
     restart execution.*/
  ntp->u.rdymsg = msg;

  /* If the waken thread has a not-greater priority than the current
     one then it is just inserted in the ready list else it made
     running immediately and the invoking thread goes in the ready
     list instead.*/
  if (ntp->hdr.pqueue.prio <= otp->hdr.pqueue.prio) {
    (void) __sch_ready_behind(oip, ntp);
  }
  else {
    /* The old thread goes back in the ready list ahead of its peers
       because it has not exhausted its time slice.*/
    otp = __sch_ready_ahead(oip, otp);

    /* Handling idle-leave hook.*/
    if (otp->hdr.pqueue.prio == IDLEPRIO) {
      CH_CFG_IDLE_LEAVE_HOOK();
    }

    /* The extracted thread is marked as current.*/
    ntp->state = CH_STATE_CURRENT;
    __sch_set_currthread(oip, ntp);

    /* Swap operation as tail call.*/
    chSysSwitch(ntp, otp);
  }
}

/**
 * @brief   Performs a reschedule if a higher priority thread is runnable.
 * @details If a thread with a higher priority than the current thread is in
 *          the ready list then make the higher priority thread running.
 * @note    Only local threads are considered, other cores are signaled
 *          and perform a reschedule locally.
 *
 * @sclass
 */
void chSchRescheduleS(void) {
  os_instance_t *oip = currcore;
  thread_t *tp = __sch_get_currthread(oip);

  chDbgCheckClassS();

  if (firstprio(&oip->rlist.pqueue) > tp->hdr.pqueue.prio) {
    __sch_reschedule_ahead(oip);
  }
}

#if !defined(CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED)
/**
 * @brief   Evaluates if preemption is required.
 * @details The decision is taken by comparing the relative priorities and
 *          depending on the state of the round robin timeout counter.
 * @note    Not a user function, it is meant to be invoked from within
 *          the port layer in the IRQ-related preemption code.
 *
 * @retval true         if there is a thread that must go in running state
 *                      immediately.
 * @retval false        if preemption is not required.
 *
 * @special
 */
bool chSchIsPreemptionRequired(void) {
  os_instance_t *oip = currcore;
  thread_t *tp = __sch_get_currthread(oip);

  tprio_t p1 = firstprio(&oip->rlist.pqueue);
  tprio_t p2 = tp->hdr.pqueue.prio;

#if CH_CFG_TIME_QUANTUM > 0
  /* If the running thread has not reached its time quantum, reschedule only
     if the first thread on the ready queue has a higher priority.
     Otherwise, if the running thread has used up its time quantum, reschedule
     if the first thread on the ready queue has equal or higher priority.*/
  return (tp->ticks > (tslices_t)0) ? (p1 > p2) : (p1 >= p2);
#else
  /* If the round robin preemption feature is not enabled then performs a
     simpler comparison.*/
  return p1 > p2;
#endif
}
#endif /* !defined(CH_SCH_IS_PREEMPTION_REQUIRED_HOOKED) */

#if !defined(CH_SCH_DO_PREEMPTION_HOOKED)
/**
 * @brief   Switches to the first thread on the runnable queue.
 * @details The current thread is positioned in the ready list behind or
 *          ahead of all threads having the same priority depending on
 *          if it used its whole time slice.
 * @note    Not a user function, it is meant to be invoked from within
 *          the port layer in the IRQ-related preemption code.
 *
 * @special
 */
void chSchDoPreemption(void) {
  os_instance_t *oip = currcore;
  thread_t *otp = __sch_get_currthread(oip);
  thread_t *ntp;

  /* Picks the first thread from the ready queue and makes it current.*/
  ntp = (thread_t *)ch_pqueue_remove_highest(&oip->rlist.pqueue);
  ntp->state = CH_STATE_CURRENT;
  __sch_set_currthread(oip, ntp);

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
    otp = __sch_ready_behind(oip, otp);

    /* The thread being swapped out receives a new time quantum.*/
    otp->ticks = (tslices_t)CH_CFG_TIME_QUANTUM;
  }
  else {
    /* The thread didn't consume all its time quantum so it is put ahead of
       threads with equal priority and does not acquire a new time quantum.*/
    otp = __sch_ready_ahead(oip, otp);
  }
#else /* !(CH_CFG_TIME_QUANTUM > 0) */
  /* If the round-robin mechanism is disabled then the thread goes always
     ahead of its peers.*/
  otp = __sch_ready_ahead(oip, otp);
#endif /* !(CH_CFG_TIME_QUANTUM > 0) */

  /* Swap operation as tail call.*/
  chSysSwitch(ntp, otp);
}
#endif /* !defined(CH_SCH_DO_PREEMPTION_HOOKED) */

#if !defined(CH_SCH_PREEMPTION_HOOKED)
/**
 * @brief   All-in-one preemption code.
 * @note    Not a user function, it is meant to be invoked from within
 *          the port layer in the IRQ-related preemption code.
 *
 * @special
 */
void chSchPreemption(void) {
  os_instance_t *oip = currcore;
  thread_t *tp = __sch_get_currthread(oip);
  tprio_t p1 = firstprio(&oip->rlist.pqueue);
  tprio_t p2 = tp->hdr.pqueue.prio;

#if CH_CFG_TIME_QUANTUM > 0
  if (tp->ticks > (tslices_t)0) {
    if (p1 > p2) {
      __sch_reschedule_ahead(oip);
    }
  }
  else {
    if (p1 >= p2) {
      __sch_reschedule_behind(oip);
    }
  }
#else /* CH_CFG_TIME_QUANTUM == 0 */
  if (p1 > p2) {
    __sch_reschedule_ahead(oip);
  }
#endif /* CH_CFG_TIME_QUANTUM == 0 */
}
#endif /* !defined(CH_SCH_PREEMPTION_HOOKED) */

/**
 * @brief   Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal or higher priority, if any.
 *
 * @sclass
 */
void chSchDoYieldS(void) {
  os_instance_t *oip = currcore;
  thread_t *tp = __sch_get_currthread(oip);

  chDbgCheckClassS();

  if (firstprio(&oip->rlist.pqueue) >= tp->hdr.pqueue.prio) {
    __sch_reschedule_behind(oip);
  }
}

/**
 * @brief   Makes runnable the fist thread in the ready list, does not
 *          reschedule internally.
 * @details The current thread is positioned in the ready list ahead of all
 *          threads having the same priority.
 * @note    Not a user function, it is meant to be invoked by the scheduler
 *          itself.
 *
 * @return              The pointer to the thread being switched in.
 *
 * @special
 */
thread_t *chSchSelectFirstI(void) {
  os_instance_t *oip = currcore;
  thread_t *otp = __sch_get_currthread(oip);
  thread_t *ntp;

  /* Picks the first thread from the ready queue and makes it current.*/
  ntp = (thread_t *)ch_pqueue_remove_highest(&oip->rlist.pqueue);
  ntp->state = CH_STATE_CURRENT;
  __sch_set_currthread(oip, ntp);

  /* Handling idle-leave hook.*/
  if (otp->hdr.pqueue.prio == IDLEPRIO) {
    CH_CFG_IDLE_LEAVE_HOOK();
  }

  /* Placing in ready list ahead of peers.*/
  __sch_ready_ahead(oip, otp);

  return ntp;
}

/** @} */
