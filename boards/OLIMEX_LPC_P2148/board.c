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

#include "ch.h"
#include "hal.h"

#define VAL_TC0_PRESCALER 0

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config =
{
  VAL_PINSEL0,
  VAL_PINSEL1,
  VAL_PINSEL2,
  {VAL_FIO0PIN, VAL_FIO0DIR},
  {VAL_FIO1PIN, VAL_FIO1DIR}
};
#endif

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
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  lpc214x_clock_init();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

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
}
