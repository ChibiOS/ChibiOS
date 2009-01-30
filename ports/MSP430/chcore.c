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

#include <ch.h>

/**
 * This function implements the idle thread infinite loop. The function should
 * put the processor in the lowest power mode capable to serve interrupts.
 * The priority is internally set to the minimum system value so that this
 * thread is executed only if there are no other ready threads in the system.
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
 * Context switch.
 */
__attribute__((naked))
void chSysSwitchI(Thread *otp, Thread *ntp) {
  register struct intctx *sp asm("r1");

  asm volatile ("push    r11                                    \n\t" \
                "push    r10                                    \n\t" \
                "push    r9                                     \n\t" \
                "push    r8                                     \n\t" \
                "push    r7                                     \n\t" \
                "push    r6                                     \n\t" \
                "push    r6                                     \n\t" \
                "push    r4");
  otp->p_ctx.sp = sp;
  sp = ntp->p_ctx.sp;
  asm volatile ("pop     r4                                     \n\t" \
                "pop     r5                                     \n\t" \
                "pop     r6                                     \n\t" \
                "pop     r7                                     \n\t" \
                "pop     r8                                     \n\t" \
                "pop     r9                                     \n\t" \
                "pop     r10                                    \n\t" \
                "pop     r11                                    \n\t" \
                "ret" : : "r" (sp));
}

/**
 * Prints a message on the system console (if any).
 */
void chSysPuts(char *msg) {
}

void threadstart(void) {

  asm volatile ("eint                                           \n\t" \
                "mov     r11, r15                               \n\t" \
                "call    r10                                    \n\t" \
                "call    #chThdExit");
}
