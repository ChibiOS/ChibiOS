/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    nil.c
 * @brief   Nil RTOS main source file.
 *
 * @defgroup nil
 * @details Nil RTOS services.
 * @{
 */

#include "nil.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   System data structures.
 */
nil_system_t nil;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the kernel.
 * @details Initializes the kernel structures, the current instructions flow
 *          becomes the idle thread upon return. The idle thread must not
 *          invoke any kernel primitive able to change state to not runnable.
 *
 * @special
 */
void chSysInit(void) {
  thread_reference_t tr;
  const thread_config_t *tcp;

  /* Port layer initialization.*/
  port_init();

  /* Iterates through the list of defined threads.*/
  for (tr = &nil.threads[0], tcp = nil_thd_configs;
       tr < &nil.threads[NIL_CFG_NUM_THREADS];
       tr++, tcp++) {
    tr->state = NIL_STATE_READY;
    tr->timeout = 0;

    /* Port dependent thread initialization.*/
    PORT_SETUP_CONTEXT(tr, tcp->wap, tcp->size, tcp->funcp, tcp->arg);

    /* Initialization hook.*/
#if defined(NIL_CFG_THREAD_EXT_INIT_HOOK)
    NIL_CFG_THREAD_EXT_INIT_HOOK(tr);
#endif
  }

  /* Runs the highest priority thread, the current one becomes the null
     thread.*/
  nil.current = nil.next = nil.threads;
  port_switch(nil.threads, &nil.threads[NIL_CFG_NUM_THREADS]);

  /* Interrupts enabled for the idle thread.*/
  chSysEnable();
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected, for example because a programming
 *          error in the application code that triggers an assertion while
 *          in debug mode.
 * @note    Can be invoked from any system state.
 *
 * @special
 */
void chSysHalt(const char *reason) {

  port_disable();

#if NIL_DBG_ENABLED
  nil.dbg_panic_msg = reason;
#else
  (void)reason;
#endif

#if defined(NIL_CFG_SYSTEM_HALT_HOOK) || defined(__DOXYGEN__)
  NIL_CFG_SYSTEM_HALT_HOOK(reason);
#endif

  /* Harmless infinite loop.*/
  while (true)
    ;
}

/**
 * @brief   Time management handler.
 * @note    This handler has to be invoked by a periodic ISR in order to
 *          reschedule the waiting threads.
 *
 * @iclass
 */
void chSysTimerHandlerI(void) {

#if NIL_CFG_TIMEDELTA == 0
  thread_reference_t tr = &nil.threads[0];
  nil.systime++;
  do {
    /* Is the thread in a wait state with timeout?.*/
    if (tr->timeout > 0) {

      chDbgAssert(!NIL_THD_IS_READY(tr), "is ready");

     /* Did the timer reach zero?*/
      if (--tr->timeout == 0) {
        /* Timeout on semaphores requires a special handling because the
           semaphore counter must be incremented.*/
        if (NIL_THD_IS_WTSEM(tr))
          tr->u1.semp->cnt++;
        else if (NIL_THD_IS_SUSP(tr))
          tr->u1.trp = NULL;
        chSchReadyI(tr, MSG_TIMEOUT);
      }
    }
    /* Lock released in order to give a preemption chance on those
       architectures supporting IRQ preemption.*/
    chSysUnlockFromISR();
    tr++;
    chSysLockFromISR();
  } while (tr < &nil.threads[NIL_CFG_NUM_THREADS]);
#else
  thread_reference_t tr = &nil.threads[0];
  systime_t next = 0;

  chDbgAssert(nil.nexttime == port_timer_get_alarm(),
               "chSysTimerHandlerI(), #1", "time mismatch");

  do {
    /* Is the thread in a wait state with timeout?.*/
    if (tr->timeout > 0) {

      chDbgAssert(!NIL_THD_IS_READY(tr),
                   "chSysTimerHandlerI(), #2", "is ready");
      chDbgAssert(tr->timeout >= nil.nexttime - nil.lasttime,
                   "chSysTimerHandlerI(), #3", "skipped one");

      tr->timeout -= nil.nexttime - nil.lasttime;
      if (tr->timeout == 0) {
        /* Timeout on semaphores requires a special handling because the
           semaphore counter must be incremented.*/
        if (NIL_THD_IS_WTSEM(tr))
          tr->u1.semp->cnt++;
        else if (NIL_THD_IS_SUSP(tr))
          tr->u1.trp = NULL;
        chSchReadyI(tr, NIL_MSG_TMO);
      }
      else {
        if (tr->timeout <= next - 1)
          next = tr->timeout;
      }
    }
    /* Lock released in order to give a preemption chance on those
       architectures supporting IRQ preemption.*/
    chSysUnlockFromISR();
    tr++;
    chSysLockFromISR();
  } while (tr < &nil.threads[NIL_CFG_NUM_THREADS]);
  nil.lasttime = nil.nexttime;
  if (next > 0) {
    nil.nexttime += next;
    port_timer_set_alarm(nil.nexttime);
  }
  else {
    /* No tick event needed.*/
    port_timer_stop_alarm();
  }
#endif
}

/**
 * @brief   Makes the specified thread ready for execution.
 *
 * @param[in] tr        reference to the @p thread_t object
 * @param[in] msg       the wakeup message
 *
 * @return              The same reference passed as parameter.
 */
thread_reference_t chSchReadyI(thread_reference_t tr, msg_t msg) {

  chDbgAssert((tr >= nil.threads) &&
               (tr < &nil.threads[NIL_CFG_NUM_THREADS]),
               "pointer out of range");
  chDbgAssert(!NIL_THD_IS_READY(tr), "already ready");
  chDbgAssert(nil.next <= nil.current, "priority ordering");

  tr->u1.msg = msg;
  tr->state = NIL_STATE_READY;
  tr->timeout = 0;
  if (tr < nil.next)
    nil.next = tr;
  return tr;
}

/**
 * @brief   Reschedules.
 *
 * @sclass
 */
void chSchRescheduleS() {
  thread_reference_t otr = nil.current;
  thread_reference_t ntr = nil.next;

  if (ntr != otr) {
    nil.current = ntr;
#if defined(NIL_CFG_IDLE_LEAVE_HOOK)
    if (otr == &nil.threads[NIL_CFG_NUM_THREADS]) {
      NIL_CFG_IDLE_LEAVE_HOOK();
    }
#endif
    port_switch(ntr, otr);
  }
}

/**
 * @brief   Puts the current thread to sleep into the specified state with
 *          timeout specification.
 * @details The thread goes into a sleeping state, if it is not awakened
 *          explicitly within the specified system time then it is forcibly
 *          awakened with a @p NIL_MSG_TMO low level message.
 *
 * @param[in] newstate  the new thread state or a semaphore pointer
 * @param[in] timeout   the number of ticks before the operation timeouts.
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The wakeup message.
 * @retval NIL_MSG_TMO  if a timeout occurred.
 *
 * @sclass
 */
msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t timeout) {
  thread_reference_t ntr, otr = nil.current;

  chDbgAssert(otr != &nil.threads[NIL_CFG_NUM_THREADS],
               "idle cannot sleep");

  /* Storing the wait object for the current thread.*/
  otr->state = newstate;

#if NIL_CFG_TIMEDELTA > 0
  if (timeout != TIME_INFINITE) {
    systime_t time = chTimeNowI() + timeout;

    /* TIMEDELTA makes sure to have enough time to reprogram the timer
       before the free-running timer counter reaches the selected timeout.*/
    if (timeout < NIL_CFG_TIMEDELTA)
      timeout = NIL_CFG_TIMEDELTA;

    if (nil.lasttime == nil.nexttime) {
      /* Special case, first thread asking for a timeout.*/
      port_timer_start_alarm(time);
      nil.nexttime = time;
    }
    else {
      /* Special case, there are already other threads with a timeout
         activated, evaluating the order.*/
      if (chTimeIsWithin(time, nil.lasttime, nil.nexttime)) {
        port_timer_set_alarm(time);
        nil.nexttime = time;
      }
    }

    /* Timeout settings.*/
    otr->timeout = time - nil.lasttime;
  }
#else

  /* Timeout settings.*/
  otr->timeout = timeout;
#endif

  /* Scanning the whole threads array.*/
  ntr = nil.threads;
  while (true) {
    /* Is this thread ready to execute?*/
    if (NIL_THD_IS_READY(ntr)) {
      nil.current = nil.next = ntr;
#if defined(NIL_CFG_IDLE_ENTER_HOOK)
      if (ntr == &nil.threads[NIL_CFG_NUM_THREADS]) {
        NIL_CFG_IDLE_ENTER_HOOK();
      }
#endif
      port_switch(ntr, otr);
      return nil.current->u1.msg;
    }

    /* Points to the next thread in lowering priority order.*/
    ntr++;
    chDbgAssert(ntr <= &nil.threads[NIL_CFG_NUM_THREADS],
                 "pointer out of range");
  }
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The wake up message.
 *
 * @sclass
 */
msg_t chThdSuspendTimeoutS(thread_reference_t *trp, systime_t timeout) {

  chDbgAssert(*trp == NULL, "not NULL");

  *trp = nil.current;
  nil.current->u1.trp = trp;
  return chSchGoSleepTimeoutS(NIL_STATE_SUSP, timeout);
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must not reschedule because it can be called from
 *          ISR context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void chThdResumeI(thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {
    thread_reference_t tr = *trp;

    chDbgAssert(NIL_THD_IS_SUSP(tr), "not suspended");

    *trp = NULL;
    chSchReadyI(tr, msg);
  }
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks
 *
 * @api
 */
void chThdSleep(systime_t time) {

  chSysLock();
  chThdSleepS(time);
  chSysUnlock();
}

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 *
 * @param[in] time      absolute system time
 *
 * @api
 */
void chThdSleepUntil(systime_t time) {

  chSysLock();
  chThdSleepUntilS(time);
  chSysUnlock();
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p chSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    This function is designed to work with the @p chThdSleepUntil().
 *
 * @return              The system time in ticks.
 *
 * @api
 */
systime_t chTimeNow(void) {
  systime_t time;

  chSysLock();
  time = chTimeNowI();
  chSysUnlock();
  return time;
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 *
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @api
 */
bool chTimeNowIsWithin(systime_t start, systime_t end) {

  systime_t time = chTimeNow();
  return chTimeIsWithin(time, start, end);
}

/**
 * @brief   Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p chSemReset().
 * @retval NIL_MSG_TMO  if the semaphore has not been signaled or reset within
 *                      the specified timeout.
 *
 * @api
 */
msg_t chSemWaitTimeout(semaphore_t *sp, systime_t timeout) {
  msg_t msg;

  chSysLock();
  msg = chSemWaitTimeoutS(sp, timeout);
  chSysUnlock();
  return msg;
}

/**
 * @brief   Performs a wait operation on a semaphore with timeout specification.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p chSemReset().
 * @retval NIL_MSG_TMO  if the semaphore has not been signaled or reset within
 *                      the specified timeout.
 *
 * @sclass
 */
msg_t chSemWaitTimeoutS(semaphore_t *sp, systime_t timeout) {

  /* Note, the semaphore counter is a volatile variable so accesses are
     manually optimized.*/
  cnt_t cnt = sp->cnt;
  if (cnt <= 0) {
    if (TIME_IMMEDIATE == timeout)
      return MSG_TIMEOUT;
    sp->cnt = cnt - 1;
    nil.current->u1.semp = sp;
    return chSchGoSleepTimeoutS(NIL_STATE_WTSEM, timeout);
  }
  sp->cnt = cnt - 1;
  return MSG_OK;
}

/**
 * @brief   Performs a signal operation on a semaphore.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp    pointer to a @p semaphore_t structure
 *
 * @api
 */
void chSemSignal(semaphore_t *sp) {

  chSysLock();
  chSemSignalI(sp);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief   Performs a signal operation on a semaphore.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp    pointer to a @p semaphore_t structure
 *
 * @iclass
 */
void chSemSignalI(semaphore_t *sp) {

  if (++sp->cnt <= 0) {
    thread_reference_t tr = nil.threads;
    while (true) {
      /* Is this thread waiting on this semaphore?*/
      if (tr->u1.semp == sp) {

        chDbgAssert(NIL_THD_IS_WTSEM(tr), "not waiting");

        chSchReadyI(tr, MSG_OK);
        return;
      }
      tr++;

      chDbgAssert(tr < &nil.threads[NIL_CFG_NUM_THREADS],
                  "pointer out of range");
    }
  }
}

/**
 * @brief   Performs a reset operation on the semaphore.
 * @post    After invoking this function all the threads waiting on the
 *          semaphore, if any, are released and the semaphore counter is set
 *          to the specified, non negative, value.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] n         the new value of the semaphore counter. The value must
 *                      be non-negative.
 *
 * @api
 */
void chSemReset(semaphore_t *sp, cnt_t n) {

  chSysLock();
  chSemResetI(sp, n);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief   Performs a reset operation on the semaphore.
 * @post    After invoking this function all the threads waiting on the
 *          semaphore, if any, are released and the semaphore counter is set
 *          to the specified, non negative, value.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @param[in] n         the new value of the semaphore counter. The value must
 *                      be non-negative.
 *
 * @iclass
 */
void chSemResetI(semaphore_t *sp, cnt_t n) {
  thread_reference_t tr;
  cnt_t cnt;

  cnt = sp->cnt;
  sp->cnt = n;
  tr = nil.threads;
  while (cnt < 0) {
    /* Is this thread waiting on this semaphore?*/
    if (tr->u1.semp == sp) {

      chDbgAssert(NIL_THD_IS_WTSEM(tr), "not waiting");

      cnt++;
      chSchReadyI(tr, MSG_RESET);
    }
    tr++;

    chDbgAssert(tr < &nil.threads[NIL_CFG_NUM_THREADS],
                "pointer out of range");
  }
}

/** @} */
