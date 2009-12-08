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
 * @file templates/hal_lld.c
 * @brief HAL Driver subsystem low level driver source template.
 * @addtogroup HAL_LLD
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Low Level Driver exported variables.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local variables.                                         */
/*===========================================================================*/

static LARGE_INTEGER nextcnt;
static LARGE_INTEGER slice;

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level HAL driver initialization.
 */
void hal_lld_init(void) {
  WSADATA wsaData;

  /* Initialization.*/
  if (WSAStartup(2, &wsaData) != 0) {
    printf("Unable to locate a winsock DLL\n");
    exit(1);
  }

  printf("Win32 ChibiOS/RT simulator\n\n");
  if (!QueryPerformanceFrequency(&slice)) {
    printf("QueryPerformanceFrequency() error");
    exit(1);
  }
  printf("Core Frequency   %u Hz\n", (int)slice.LowPart);
  slice.QuadPart /= CH_FREQUENCY;
  QueryPerformanceCounter(&nextcnt);
  nextcnt.QuadPart += slice.QuadPart;

  fflush(stdout);
}

/**
 * @brief Interrupt simulation.
 */
void ChkIntSources(void) {
  LARGE_INTEGER n;

  if (sd_lld_interrupt_pending()) {
    if (chSchIsRescRequiredExI())
      chSchDoRescheduleI();
    return;
  }

  // Interrupt Timer simulation (10ms interval).
  QueryPerformanceCounter(&n);
  if (n.QuadPart > nextcnt.QuadPart) {
    nextcnt.QuadPart += slice.QuadPart;
    chSysTimerHandlerI();
    if (chSchIsRescRequiredExI())
      chSchDoRescheduleI();
  }
}

/** @} */
