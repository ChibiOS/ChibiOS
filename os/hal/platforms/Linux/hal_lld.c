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
 * @file Linux/hal_lld.c
 * @brief Linux HAL subsystem low level driver code.
 * @addtogroup LINUX_HAL
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Low Level Driver exported variables.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local variables.                                         */
/*===========================================================================*/

static struct timeval nextcnt;
static struct timeval tick = {0, 1000000 / CH_FREQUENCY};

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

  puts("Win32 ChibiOS/RT simulator (Linux)\n");
  gettimeofday(&nextcnt, NULL);
  timeradd(&nextcnt, &tick, &nextcnt);
}

/**
 * @brief Interrupt simulation.
 */
void ChkIntSources(void) {
  struct timeval tv;

#if CH_HAL_USE_SERIAL
  if (sd_lld_interrupt_pending()) {
    if (chSchIsRescRequiredExI())
      chSchDoRescheduleI();
    return;
  }
#endif

  gettimeofday(&tv, NULL);
  if (timercmp(&tv, &nextcnt, >=)) {
    timeradd(&nextcnt, &tick, &nextcnt);
    chSysTimerHandlerI();
    if (chSchIsRescRequiredExI())
      chSchDoRescheduleI();
  }
}

/** @} */
