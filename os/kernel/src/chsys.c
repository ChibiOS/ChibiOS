/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    chsys.c
 * @brief   System related code.
 *
 * @addtogroup system
 * @details System related APIs and services:
 *          - Initialization.
 *          - Locks.
 *          - Interrupt Handling.
 *          - Power Management.
 *          - Abnormal Termination.
 *          .
 * @{
 */

#include "ch.h"

#if !CH_NO_IDLE_THREAD || defined(__DOXYGEN__)
/**
 * @brief   Idle thread working area.
 * @see     IDLE_THREAD_STACK_SIZE
 */
WORKING_AREA(_idle_thread_wa, IDLE_THREAD_STACK_SIZE);

/**
 * @brief   This function implements the idle thread infinite loop.
 * @details The function puts the processor in the lowest power mode capable
 *          to serve interrupts.<br>
 *          The priority is internally set to the minimum system value so
 *          that this thread is executed only if there are no other ready
 *          threads in the system.
 *
 * @param[in] p the thread parameter, unused in this scenario
 */
void _idle_thread(void *p) {

  (void)p;
  chRegSetThreadName("idle");
  while (TRUE) {
    port_wait_for_interrupt();
    IDLE_LOOP_HOOK();
  }
}
#endif /* CH_NO_IDLE_THREAD */

/**
 * @brief   ChibiOS/RT initialization.
 * @details After executing this function the current instructions stream
 *          becomes the main thread.
 * @pre     Interrupts must be still disabled when @p chSysInit() is invoked
 *          and are internally enabled.
 * @post    The main thread is created with priority @p NORMALPRIO.
 * @note    This function has special, architecture-dependent, requirements,
 *          see the notes into the various port reference manuals.
 *
 * @special
 */
void chSysInit(void) {
  static Thread mainthread;

  port_init();
  scheduler_init();
  vt_init();
#if CH_USE_MEMCORE
  core_init();
#endif
#if CH_USE_HEAP
  heap_init();
#endif
#if CH_DBG_ENABLE_TRACE
  trace_init();
#endif

  /* Now this instructions flow becomes the main thread.*/
  setcurrp(_thread_init(&mainthread, NORMALPRIO));
  currp->p_state = THD_STATE_CURRENT;
  chSysEnable();

  chRegSetThreadName("main");

#if !CH_NO_IDLE_THREAD
  /* This thread has the lowest priority in the system, its role is just to
     serve interrupts in its context while keeping the lowest energy saving
     mode compatible with the system status.*/
  chThdCreateStatic(_idle_thread_wa, sizeof(_idle_thread_wa), IDLEPRIO,
                    (tfunc_t)_idle_thread, NULL);
#endif
}

/**
 * @brief   Handles time ticks for round robin preemption and timer increments.
 * @details Decrements the remaining time quantum of the running thread
 *          and preempts it when the quantum is used up. Increments system
 *          time and manages the timers.
 * @note    The frequency of the timer determines the system tick granularity
 *          and, together with the @p CH_TIME_QUANTUM macro, the round robin
 *          interval.
 *
 * @iclass
 */
void chSysTimerHandlerI(void) {

#if CH_TIME_QUANTUM > 0
  /* Running thread has not used up quantum yet? */
  if (rlist.r_preempt > 0)
    /* Decrement remaining quantum.*/
    rlist.r_preempt--;
#endif
#if CH_DBG_THREADS_PROFILING
  currp->p_time++;
#endif
  chVTDoTickI();
#if defined(SYSTEM_TICK_EVENT_HOOK)
  SYSTEM_TICK_EVENT_HOOK();
#endif
}

#if CH_USE_NESTED_LOCKS && !CH_OPTIMIZE_SPEED
void chSysLock(void) {

  chDbgAssert(currp->p_locks >= 0,
              "chSysLock(), #1",
              "negative nesting counter");
  if (currp->p_locks++ == 0)
    port_lock();
}

void chSysUnlock(void) {

  chDbgAssert(currp->p_locks > 0,
              "chSysUnlock(), #1",
              "non-positive nesting counter");
  if (--currp->p_locks == 0)
    port_unlock();
}
#endif /* CH_USE_NESTED_LOCKS && !CH_OPTIMIZE_SPEED */

/** @} */
