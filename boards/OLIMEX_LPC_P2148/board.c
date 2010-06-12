/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

#include "ch.h"
#include "hal.h"

#define VAL_TC0_PRESCALER 0

/*
 * Timer 0 IRQ handling here.
 */
static CH_IRQ_HANDLER(T0IrqHandler) {

  CH_IRQ_PROLOGUE();
  T0IR = 1;             /* Clear interrupt on match MR0. */

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  VICVectAddr = 0;
  CH_IRQ_EPILOGUE();
}

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {

  lpc214x_clock_init();
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {

  /*
   * HAL initialization.
   */
  halInit();

  /*
   * System Timer initialization, 1ms intervals.
   */
  SetVICVector(T0IrqHandler, 0, SOURCE_Timer0);
  VICIntEnable = INTMASK(SOURCE_Timer0);
  TC *timer = T0Base;
  timer->TC_PR = VAL_TC0_PRESCALER;
  timer->TC_MR0 = (PCLK / CH_FREQUENCY) / (VAL_TC0_PRESCALER + 1);
  timer->TC_MCR = 3;    /* Interrupt and clear TC on match MR0. */
  timer->TC_TCR = 2;    /* Reset counter and prescaler. */
  timer->TC_TCR = 1;    /* Timer enabled. */

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}
