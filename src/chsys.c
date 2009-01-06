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
 * @addtogroup System
 * @{
 */

#include <ch.h>

static WORKING_AREA(idle_thread_wa, IDLE_THREAD_STACK_SIZE);

/**
 * This function implements the idle thread infinite loop. The function should
 * put the processor in the lowest power mode capable to serve interrupts.
 * The priority is internally set to the minimum system value so that this
 * thread is executed only if there are no other ready threads in the system.
 * @param p the thread parameter, unused in this scenario
 * @note Implementation should declare this function as a weak symbol in order
 *       to allow applications to re-implement it.
 */
static void idle_thread(void *p) {

  while (TRUE) {
    sys_wait_for_interrupt();
  }
}

/**
 * ChibiOS/RT initialization. After executing this function the current
 * instructions stream becomes the main thread.
 * @note Interrupts should be still disabled when \p chSysInit() is invoked
 *       and are internally enabled.
 * @note The main thread is created with priority \p NORMALPRIO.
 */
void chSysInit(void) {
  static Thread mainthread;

  chSchInit();
  chDbgInit();
  chVTInit();
#ifdef CH_USE_HEAP
  chHeapInit();
#endif
  /*
   * Now this instructions flow becomes the main thread.
   */
  (currp = init_thread(&mainthread, NORMALPRIO))->p_state = PRCURR;

  chSysEnable();

  /*
   * The idle thread is created using the port-provided implementation.
   * This thread has the lowest priority in the system, its role is just to
   * serve interrupts in its context while keeping the lowest energy saving
   * mode compatible with the system status.
   */
  chThdCreateStatic(idle_thread_wa, sizeof(idle_thread_wa), IDLEPRIO,
                    (tfunc_t)idle_thread, NULL);
}

/**
 * Handles time ticks for round robin preemption and timer increments.
 * Decrements the remaining time quantum of the running thread and preempts
 * it when the quantum is used up. Increments system time and manages the
 * timers.
 * @note The frequency of the timer determines the system tick granularity and,
 * together with the \p CH_TIME_QUANTUM macro, the round robin interval.
 */
void chSysTimerHandlerI(void) {

#ifdef CH_USE_ROUNDROBIN
  /* running thread has not used up quantum yet? */
  if (rlist.r_preempt > 0)
    /* decrement remaining quantum */
    rlist.r_preempt--;
#endif
  chVTDoTickI();
}

/**
 * Abonormal system termination handler. Invoked by the ChibiOS/RT when an
 * abnormal unrecoverable condition is met.
 */
void chSysHalt(void) {

  chSysDisable();
  sys_halt();
}

#if !defined(CH_OPTIMIZE_SPEED)
/**
 * Enters the ChibiOS/RT system mutual exclusion zone.
 * @note The use of system mutual exclusion zone is not recommended in
 *       the user code, it is a better idea to use the semaphores or mutexes
 *       instead.
 * @note The code of this API is may be inlined or not depending on the
 *       @p CH_OPTIMIZE_SPEED setting.
 * @see CH_USE_NESTED_LOCKS, chSysLockInline()
 */
void chSysLock(void) {

  chSysLockInline();
}

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone.
 * @note The use of system mutual exclusion zone is not recommended in
 *       the user code, it is a better idea to use the semaphores or mutexes
 *       instead.
 * @note The code of this API is may be inlined or not depending on the
 *       @p CH_OPTIMIZE_SPEED setting.
 * @see CH_USE_NESTED_LOCKS, chSysUnlockInline()
 */
void chSysUnlock(void) {

  chDbgAssert(currp->p_locks > 0, "chinit.c, chSysUnlock()");
  chSysUnlockInline();
}
#endif /* !CH_OPTIMIZE_SPEED */

/** @} */
