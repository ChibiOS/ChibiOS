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
 * @addtogroup WIN32SIM_CORE
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
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
__attribute__((weak))
void sys_puts(char *msg) {
}

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
__attribute__((naked, weak))
void sys_switch(Thread *otp, Thread *ntp) {
  register struct intctx *esp asm("esp");

  asm volatile ("push    %ebp                                   \n\t" \
                "push    %esi                                   \n\t" \
                "push    %edi                                   \n\t" \
                "push    %ebx");
  otp->p_ctx.esp = esp;
  esp = ntp->p_ctx.esp;
  asm volatile ("pop     %ebx                                   \n\t" \
                "pop     %edi                                   \n\t" \
                "pop     %esi                                   \n\t" \
                "pop     %ebp                                   \n\t" \
                "ret" : : "r" (esp));
}

/**
 * Halts the system. In this implementation it just exits the simulation.
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
__attribute__((weak))
void sys_halt(void) {

  exit(2);
}

/**
 * Threads return point, it just invokes @p chThdExit().
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
__attribute__((naked, weak))
void threadexit(void) {

  asm volatile ("push    %eax                                   \n\t" \
                "call    _chThdExit");
}

}
/** @} */
