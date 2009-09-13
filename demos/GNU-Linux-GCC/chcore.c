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

#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

/**
 * @addtogroup LINUXSIM_CORE
 * @{
 */

#include <ch.h>

static struct itimerval tempo;
static bool_t pending = FALSE;

void timer(int numSignal) {

  pending = TRUE;
}

/*
 * Simulated HW initialization.
 */
void _init_core(void) {

  signal(SIGALRM, timer);
  tempo.it_value.tv_sec = 0;
  tempo.it_value.tv_usec = 10000;
  tempo.it_interval.tv_sec = 0;
  tempo.it_interval.tv_usec = 10000;
  setitimer(ITIMER_REAL, &tempo, NULL);
}

/*
 * Interrupt simulation.
 */
void ChkIntSources(void) {

  if (pending) {
    chSysTimerHandlerI();
    pending = FALSE;
  }

  if (chSchIsRescRequiredExI())
    chSchDoRescheduleI();
}

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
__attribute__((used))
static void __dummy(Thread *otp, Thread *ntp) {
  asm volatile (".globl port_switch                             \n\t" \
                "port_switch:                                   \n\t" \
                "push    %ebp                                   \n\t" \
                "push    %esi                                   \n\t" \
                "push    %edi                                   \n\t" \
                "push    %ebx                                   \n\t" \
                "movl    %esp, 16(%ecx)                         \n\t" \
                "movl    16(%edx), %esp                         \n\t" \
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

  asm volatile ("push    %eax                                   \n\t" \
                "call    chThdExit");
}

/** @} */
