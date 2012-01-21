/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    Posix/hal_lld.c
 * @brief   Posix HAL subsystem low level driver code.
 *
 * @addtogroup POSIX_HAL
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static struct timeval nextcnt;
static struct timeval tick = {0, 1000000 / CH_FREQUENCY};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level HAL driver initialization.
 */
void hal_lld_init(void) {

#if defined(__APPLE__)
  puts("ChibiOS/RT simulator (OS X)\n");
#else
  puts("ChibiOS/RT simulator (Linux)\n");
#endif
  gettimeofday(&nextcnt, NULL);
  timeradd(&nextcnt, &tick, &nextcnt);
}

/**
 * @brief Interrupt simulation.
 */
void ChkIntSources(void) {
  struct timeval tv;

#if HAL_USE_SERIAL
  if (sd_lld_interrupt_pending()) {
    dbg_check_lock();
    if (chSchIsPreemptionRequired())
      chSchDoReschedule();
    dbg_check_unlock();
    return;
  }
#endif

  gettimeofday(&tv, NULL);
  if (timercmp(&tv, &nextcnt, >=)) {
    timeradd(&nextcnt, &tick, &nextcnt);

    CH_IRQ_PROLOGUE();

    chSysLockFromIsr();
    chSysTimerHandlerI();
    chSysUnlockFromIsr();

    CH_IRQ_EPILOGUE();

    dbg_check_lock();
    if (chSchIsPreemptionRequired())
      chSchDoReschedule();
    dbg_check_unlock();
  }
}

/** @} */
