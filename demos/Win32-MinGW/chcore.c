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

/*
 * Core file for MingGW32 demo project.
 */

#include <windows.h>
#include <stdio.h>

#undef CDECL

#include <ch.h>

static LARGE_INTEGER nextcnt;
static LARGE_INTEGER slice;

void InitSimCom1(void);
void InitSimCom2(void);
BOOL Com1ConnInterruptSimCom(void);
BOOL Com2ConnInterruptSimCom(void);
BOOL Com1InInterruptSimCom(void);
BOOL Com2InInterruptSimCom(void);
BOOL Com1OutInterruptSimCom(void);
BOOL Com2OutInterruptSimCom(void);

/*
 * Simulated HW initialization.
 */
void InitCore(void) {
  WSADATA wsaData;

  // Initialization.
  if (WSAStartup(2, &wsaData) != 0) {
    printf("Unable to locate a winsock DLL\n");
    exit(1);
  }

  printf("Win32 ChibiOS/RT simulator\n\n");
  printf("Thread structure %d bytes\n", sizeof(Thread));
  if (!QueryPerformanceFrequency(&slice)) {
    printf("QueryPerformanceFrequency() error");
    exit(1);
  }
  printf("Core Frequency   %u Hz\n", (int)slice.LowPart);
  slice.QuadPart /= CH_FREQUENCY;
  QueryPerformanceCounter(&nextcnt);
  nextcnt.QuadPart += slice.QuadPart;

  InitSimCom1();
  InitSimCom2();
  fflush(stdout);
}

/*
 * Interrupt simulation.
 */
void ChkIntSources(void) {
  LARGE_INTEGER n;

  if (Com1InInterruptSimCom()   || Com2InInterruptSimCom()  ||
      Com1OutInterruptSimCom()  || Com2OutInterruptSimCom() ||
      Com1ConnInterruptSimCom() || Com2ConnInterruptSimCom()) {
    if (chSchRescRequiredI())
      chSchDoRescheduleI();
    return;
  }

  // Interrupt Timer simulation (10ms interval).
  QueryPerformanceCounter(&n);
  if (n.QuadPart > nextcnt.QuadPart) {
    nextcnt.QuadPart += slice.QuadPart;
    chSysTimerHandlerI();
    if (chSchRescRequiredI())
      chSchDoRescheduleI();
  }
}

msg_t _idle(void *p) {

  while (TRUE) {

    ChkIntSources();
    Sleep(0);
  }
}

__attribute__((fastcall)) void chSysHalt(void) {

  exit(2);
}
