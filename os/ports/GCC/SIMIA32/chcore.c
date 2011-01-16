/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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
 * @addtogroup SIMIA32_CORE
 * @{
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
__attribute__((used))
static void __dummy(Thread *ntp, Thread *otp) {
  (void)ntp; (void)otp;
#if defined(WIN32)
  asm volatile (".globl @port_switch@8                          \n\t" \
                "@port_switch@8:");
#elif defined(__APPLE__)
  asm volatile (".globl _port_switch                            \n\t" \
                "_port_switch:");
#else
  asm volatile (".globl port_switch                             \n\t" \
                "port_switch:");
#endif
  asm volatile ("push    %ebp                                   \n\t" \
                "push    %esi                                   \n\t" \
                "push    %edi                                   \n\t" \
                "push    %ebx                                   \n\t" \
                "movl    %esp, 12(%edx)                         \n\t" \
                "movl    12(%ecx), %esp                         \n\t" \
                "pop     %ebx                                   \n\t" \
                "pop     %edi                                   \n\t" \
                "pop     %esi                                   \n\t" \
                "pop     %ebp                                   \n\t" \
                "ret");
}

/**
 * Halts the system. In this implementation it just exits the simulation.
 */
__attribute__((fastcall))
void port_halt(void) {

  exit(2);
}

/**
 * Threads return point, it just invokes @p chThdExit().
 */
void threadexit(void) {

#if defined(WIN32) || defined (__APPLE__)
  asm volatile ("push    %eax                                   \n\t" \
                "call    _chThdExit");
#else
  asm volatile ("push    %eax                                   \n\t" \
                "call    chThdExit");
#endif
}

/** @} */
