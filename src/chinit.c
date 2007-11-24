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
  static BYTE8 waIdleThread[UserStackSize(IDLE_THREAD_STACK_SIZE)];

  chSchInit();
  chDbgInit();
#ifdef CH_USE_VIRTUAL_TIMERS
  chVTInit();
#endif
  /*
   * Now this instructions flow becomes the main thread.
   */
  _InitThread(NORMALPRIO, 0, &mainthread);
  mainthread.p_state = PRCURR;
  currp = &mainthread;

  chSysUnlock();

  /*
   * The idle thread is created using the port-provided implementation.
   * This thread has the lowest priority in the system, its role is just to
   * execute the chSysPause() and serve interrupts in its context.
   * In ChibiOS/RT at least one thread in the system *must* execute
   * chThdPause(), it can be done in a dedicated thread or in the main()
   * function (that would never exit the call).
   */
  chThdCreate(IDLEPRIO, 0, waIdleThread, sizeof(waIdleThread), (t_tfunc)_IdleThread, NULL);
}

/** @} */
