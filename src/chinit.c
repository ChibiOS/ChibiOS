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

static Thread idlethread;

/**
 * ChibiOS/RT initialization. After executing this function the current
 * instructions stream becomes the idle thread. The thread must execute the
 * first user thread and then go to sleep into the \p chSysPause() where it
 * will just serve the interrupts while keeping the lowest possible power
 * mode.<br><br>
 * @code
 *   chSysInit();
 *   chThdCreate(...); // Starts one or more user threads.
 *   chSysPause();
 * @endcode
 * @note Interrupts should be still disabled when \p chSysInit() is invoked and
 *       are internally enabled.
 */
void chSysInit(void) {

  chSchInit();
#ifdef CH_USE_VIRTUAL_TIMERS
  chVTInit();
#endif
  /*
   * Now this instructions flow becomes the idle thread.
   */
  _InitThread(IDLEPRIO, 0, &idlethread);
  idlethread.p_state = PRCURR;
  currp = &idlethread;

  chSysUnlock();
}

/** @} */
