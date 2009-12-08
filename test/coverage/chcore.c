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

#include <windows.h>
#include <stdio.h>

/**
 * @addtogroup WIN32SIM_CORE
 * @{
 */

#include "ch.h"
#include "hal.h"

static LARGE_INTEGER nextcnt;
static LARGE_INTEGER slice;

/*
 * Simulated HW initialization.
 */
void InitCore(void) {
  printf("ChibiOS/RT test simulator\n\n");
  printf("Thread structure %d bytes\n", sizeof(Thread));
  if (!QueryPerformanceFrequency(&slice)) {
    fprintf(stderr, "QueryPerformanceFrequency() error");
    fflush(stderr);
    exit(1);
  }
  printf("Core Frequency   %u Hz\n", (int)slice.LowPart);
  slice.QuadPart /= CH_FREQUENCY;
  QueryPerformanceCounter(&nextcnt);
  nextcnt.QuadPart += slice.QuadPart;

  sdInit();

  fflush(stdout);
}

/*
 * Interrupt simulation.
 */
void ChkIntSources(void) {
  LARGE_INTEGER n;
  bool_t rflag = FALSE;

  if (sd_lld_interrupt_pending()) {
    if (chSchIsRescRequiredExI())
      rflag = TRUE;
  }

  // Interrupt Timer simulation (10ms interval).
  QueryPerformanceCounter(&n);
  if (n.QuadPart > nextcnt.QuadPart) {
    nextcnt.QuadPart += slice.QuadPart;
    chSysTimerHandlerI();
    if (chSchIsRescRequiredExI())
      rflag = TRUE;
  }

  if (rflag)
    chSchDoRescheduleI();
}

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
__attribute__((used))
static void __dummy(Thread *otp, Thread *ntp) {
  (void)otp; (void)ntp;
  asm volatile (".globl @port_switch@8                          \n\t" \
                "@port_switch@8:                                \n\t" \
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

  fprintf(stderr, "\nHalted\n");
  fflush(stderr);
  exit(2);
}

/**
 * Threads return point, it just invokes @p chThdExit().
 */
void threadexit(void) {

  asm volatile ("push    %eax                                   \n\t" \
                "call    _chThdExit");
}

/** @} */
