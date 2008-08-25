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

#include <ch.h>

#include "at91lib/AT91SAM7X256.h"

/*
 * System idle thread loop.
 */
void _IdleThread(void *p) {

  while (TRUE) {
// Note, it is disabled because it causes trouble with the JTAG probe.
// Enable it in the final code only.
//    PCON = 1;
  }
}

/*
 * System console message (not implemented).
 */
void chSysPuts(char *msg) {
}

/*
 * System halt.
 */
__attribute__((naked, weak))
void chSysHalt(void) {

#ifdef THUMB
  asm volatile ("ldr      r0, =_halt16");
  asm volatile ("bx       r0");
#else
  asm("b        _halt32");
#endif
}
