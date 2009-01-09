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

#include <ch.h>

/*
 * This file is a template of the system driver functions provided by a port.
 * Some of the following functions may be implemented as macros in chcore.h if
 * the implementer decides that there is an advantage in doing so, as example
 * because performance concerns.
 */

/**
 * Prints a message on the system console.
 * @param msg pointer to the message
 */
void sys_puts(char *msg) {
}

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
void sys_switch(Thread *otp, Thread *ntp) {
}

/**
 * Kernel-unlock action. Usually this function just enables interrupts.
 */
void sys_enable(void) {
}

/**
 * Kernel-lock action. Usually this function just disables interrupts.
 */
void sys_disable(void) {
}

/**
 * Kernel-lock action from an interrupt handler. This function is invoked
 * before invoking I-class APIs from interrupt handlers. The implementation
 * is architecture dependent, in its simplest form it is void.
 */
void sys_disable_from_isr(void) {
}

/**
 * Kernel-unlock action from an interrupt handler. This function is invoked
 * after invoking I-class APIs from interrupt handlers. The implementation
 * is architecture dependent, in its simplest form it is void.
 */
void sys_enable_from_isr(void) {
}

/**
 * Enters an architecture-dependent halt mode. The function is meant to return
 * when an interrupt becomes pending. The simplest implementation is an empty
 * function but this will not take advantage of architecture-specific power
 * saving modes.
 */
void sys_wait_for_interrupt(void) {
}

/**
 * Halts the system. This function is invoked by the operating system when an
 * unrecoverable error is detected (as example because a programming error in
 * the application code that triggers an assertion while in debug mode).
 */
void sys_halt(void) {

  sys_disable();
  while (TRUE) {
  }
}

/** @} */
