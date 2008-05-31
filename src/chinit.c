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
  static WorkingArea(waIdleThread, IDLE_THREAD_STACK_SIZE);

  chSchInit();
  chDbgInit();
#ifdef CH_USE_VIRTUAL_TIMERS
  chVTInit();
#endif
  /*
   * Now this instructions flow becomes the main thread.
   */
  init_thread(NORMALPRIO, 0, &mainthread);
  mainthread.p_state = PRCURR;
  currp = &mainthread;

  chSysUnlock();

  /*
   * The idle thread is created using the port-provided implementation.
   * This thread has the lowest priority in the system, its role is just to
   * serve interrupts in its context while keeping the lowest energy saving
   * mode compatible with the system status.
   */
  chThdCreate(IDLEPRIO, 0, waIdleThread, sizeof(waIdleThread), (tfunc_t)_IdleThread, NULL);
}

/**
 * Preemption routine, this function must be called into an interrupt
 * handler invoked by a system timer.
 * The frequency of the timer determines the system tick granularity and,
 * together with the \p CH_TIME_QUANTUM macro, the round robin interval.
 */
void chSysTimerHandlerI(void) {

  if (rlist.r_preempt > 0)
    rlist.r_preempt--;
#ifdef CH_USE_SYSTEMTIME
  rlist.r_stime++;
#endif

#ifdef CH_USE_VIRTUAL_TIMERS
  chVTDoTickI();
#endif
}

/** @} */
