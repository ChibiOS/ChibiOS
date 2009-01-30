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
 * @addtogroup Core
 * @{
 */

#include <ch.h>

/*
 * This file is just a template, it contains the function prototypes and the
 * doxigen documentation. The implementation of the following functions is
 * architecture/compiler specific.
 */

/**
 * This function implements the idle thread infinite loop. The function should
 * put the processor in the lowest power mode capable to serve interrupts.
 * The priority is internally set to the minimum system value so that this
 * thread is executed only if there are no other ready threads in the system.
 * @param p the thread parameter, unused in this scenario
 * @note Implementation should declare this function as a weak symbol in order
 *       to allow applications to re-implement it.
 */
void _idle(void *p) {

  while (TRUE)
    ;
}

/**
 * Abonormal system termination handler. Invoked by the ChibiOS/RT when an
 * abnormal unrecoverable condition is met.
 */
void chSysHalt(void) {

  chSysLock();

  while (TRUE)
    ;
}

/**
 * Enables the interrupts, it is only invoked once into \p chSysInit().
 */
void chSysEnable(void) {
}

/**
 * Enters the ChibiOS/RT system mutual exclusion zone. The implementation is
 * architecture dependent, on single core systems usually this function usually
 * just disables the interrupts.
 * @note The code in the system mutual exclusion zone must be as light and
 *       fast as possible, the system performance is affected by this.
 * @note The use of system mutual exclusion zones are not recommended in
 *       the user code, it is a better idea to use the Semaphores instead.
 */
void chSysLock(void) {
}

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone. The implementation is
 * architecture dependent, on single core systems usually this function usually
 * just enables the interrupts.
 * @note The code in the system mutual exclusion zone must be as light and
 *       fast as possible, the system performance is affected by this.
 * @note The use of system mutual exclusion zones are not recommended in
 *       the user code, it is a better idea to use the Semaphores instead.
 */
void chSysUnlock(void) {
}

/**
 * Performs a context switch.
 * This is the most critical code in any port, this function is responsible
 * for the context switch between 2 threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 * @note The implementation of this code affects <b>directly</b> the context
 *       switch performance so optimize here as much as you can.
 */
void chSysSwitchI(Thread *otp, Thread *ntp) {}

/**
 * Prints a message on the system console (if any).
 * @param msg the message to be printed on the system console
 */
void chSysPuts(char *msg) {
}

/** @} */
