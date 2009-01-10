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
 * @addtogroup ARM7_CORE
 * @{
 */

#include <ch.h>

#include "at91lib/AT91SAM7X256.h"

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
__attribute__((weak))
void sys_puts(char *msg) {
}

/**
 * Enters an architecture-dependent halt mode. The function is meant to return
 * when an interrupt becomes pending.
 */
__attribute__((weak))
void sys_wait_for_interrupt(void) {

  PCON = 1;
}

/**
 * Halts the system. This function is invoked by the operating system when an
 * unrecoverable error is detected (as example because a programming error in
 * the application code that triggers an assertion while in debug mode).
 */
__attribute__((weak))
void sys_halt(void) {

  sys_disable_all();
  while (TRUE) {
  }
}

/** @} */
