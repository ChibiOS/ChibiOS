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

/*
 * System idle thread loop.
 */
void _IdleThread(void *p) {

  while (TRUE) {
  }
}

/*
 * System console message (not implemented).
 */
void chSysPuts(char *msg) {
}

/*
 * Context switch.
 */
void chSysSwitchI(Thread *otp, Thread *ntp) {
  register struct intctx * volatile sp asm("sp"); /* Don't ask... */

#ifdef CH_CURRP_REGISTER_CACHE
  asm ("" : : : "r4", "r5", "r6", "r8", "r9", "r10", "r11", "lr");
#else
  asm ("" : : : "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "lr");
#endif
  otp->p_ctx.r13 = sp;
  sp = ntp->p_ctx.r13;
}

/*
 * System halt.
 */
__attribute__((naked, weak))
void chSysHalt(void) {

  while (TRUE) {
  }
}

__attribute__((naked, weak))
void threadstart(void) {

  chSysUnlock();
  asm volatile ("mov     r0, r5                                 \n\t" \
                "blx     r4                                     \n\t" \
                "bl      chThdExit                              ");

}
