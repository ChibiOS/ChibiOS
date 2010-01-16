/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file MSP430/chcore.c
 * @brief MSP430 architecture port code.
 * @addtogroup MSP430_CORE
 * @{
 */

#include "ch.h"

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
/** @cond never */
__attribute__((naked, weak))
/** @endcond */
void port_switch(Thread *otp, Thread *ntp) {
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
 * Disables the interrupts and halts the system.
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
/** @cond never */
__attribute__((weak))
/** @endcond */
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/**
 * Start a thread by invoking its work function.
 * If the work function returns @p chThdExit() is automatically invoked.
 */
void threadstart(void) {

  asm volatile ("eint                                           \n\t" \
                "mov     r11, r15                               \n\t" \
                "call    r10                                    \n\t" \
                "call    #chThdExit");
}

/** @} */
