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
 * @addtogroup Initialization
 * @{
 */

#include <ch.h>

/**
 * ChibiOS/RT initialization. After executing this function the current
 * instructions stream becomes the main thread.
 * @note Interrupts should be still disabled when \p chSysInit() is invoked
 *       and are internally enabled.
 * @note The main thread is created with priority \p NORMALPRIO.
 */
void chSysInit(void) {
  static Thread mainthread;
  static WORKING_AREA(idle_wa, IDLE_THREAD_STACK_SIZE);

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
  chThdCreateStatic(idle_wa, sizeof(idle_wa), IDLEPRIO, (tfunc_t)_idle, NULL);
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

/** @} */
