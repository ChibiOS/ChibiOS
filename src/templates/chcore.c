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
 * the implementer decides there is an advantage in doing so, as example
 * because performance concerns.
 */

void sys_puts(char *msg) {
}

void sys_switch(Thread *otp, Thread *ntp) {
}

void sys_enable(void) {
}

void sys_disable(void) {
}

void sys_disable_from_isr(void) {
}

void sys_enable_from_isr(void) {
}

void sys_wait_for_interrupt(void) {
}

void sys_halt(void) {

  while(TRUE) {
  }
}

/** @} */
