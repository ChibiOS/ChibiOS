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

void *retaddr;

/*
 * Software-generated interrupt, it must have the lowest possible priority so
 * it is executed last in the interrupts tail-chain.
 */
void PendSVVector(void) {

  chSysLock();

  if (!chSchRescRequiredI()) {

    chSysUnlock();
    return;
  }

  asm volatile ("mrs     r0, PSP                                \n\t" \
                "ldr     r1, =retaddr                           \n\t" \
                "ldr     r2, [r0, #18]                          \n\t" \
                "str     r2, [r1]                               \n\t" \
                "ldr     r1, =threadswitch                      \n\t" \
                "str     r1, [r0, #18]                          ");
}

/*
 * This code is execute in thread mode when exiting from an ISR routine that
 * requires rescheduling.
 */
__attribute__((naked, weak))
void threadswitch(void) {

  asm volatile ("sub     sp, sp, #4                             \n\t" \
                "push    {r0-r3, r12, lr}                       \n\t" \
                "mrs     r0, XPSR                               \n\t" \
                "push    {r0}                                   \n\t" \
                "ldr     r0, =retaddr                           \n\t" \
                "str     r0, [sp, #28]                          \n\t" \
                "b       chSchDoRescheduleI                     \n\t" \
                "pop     {r0}                                   \n\t" \
                "msr     XPSR, r0                               \n\t" \
                "pop     {r0-r3, r12, lr}                       \n\t" \
                "cpsie   i                                      \n\t" \
                "pop     {pc}                                   ");
}
