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
 * @addtogroup Core
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*
 * Stack saved context.
 */
struct stackregs {
};

typedef struct {
  struct stackregs *sp;
} Context;

/**
 * Platform dependent part of the \p chThdCreate() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg)                   \
{                                                                  \
}

#define INT_REQUIRED_STACK 0  // Must include registers and stack frames.

#define UserStackSize(n) (sizeof(Thread) + \
                          sizeof(struct stackregs) + (n) + (INT_REQUIRED_STACK))

/**
 * Enters the ChibiOS/RT system mutual exclusion zone, the implementation is
 * architecture dependent, on single core systems usually this function
 * just disables the interrupts.
 * @note The code in the system mutual exclusion zone must be as light and
 *       fast as possible, the system performance is affected by this.
 * @note The use of system mutual exclusion zones are not recommended in
 *       the user code, it is a better idea to use the Semaphores instead.
 */
#define chSysLock()

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone, the implementation is
 * architecture dependent, on single core systems usually this function
 * just enables the interrupts.
 * @note The code in the system mutual exclusion zone must be as light and
 *       fast as possible, the system performance is affected by this.
 * @note The use of system mutual exclusion zones are not recommended in
 *       the user code, it is a better idea to use the Semaphores instead.
 */
#define chSysUnlock()

/**
 * Prints a message on the system console (if any).
 */
#define chSysPuts(msg) {}

void chSysHalt(void);
void chSysPause(void);
void chSysSwitchI(Context *oldp, Context *newp);

#endif /* _CHCORE_H_ */

/** @} */
