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

#include <signal.h>

#include "ch.h"
#include "hal.h"

CH_IRQ_HANDLER(TIMERA0_VECTOR) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/*
 * Hardware initialization goes here.
 * NOTE: Interrupts are still disabled.
 */
void hwinit(void) {

  /*
   * HAL initialization.
   */
  halInit();

  /*
   * Timer 0 setup, uses SMCLK as source.
   */
  TACCR0 = SMCLK / 4 / CH_FREQUENCY - 1;/* Counter limit.               */
  TACTL = TACLR;                        /* Clean start.                 */
  TACTL = TASSEL_2 | ID_2 | MC_1;       /* Src=SMCLK, ID=4, cmp=TACCR0. */
  TACCTL0 = CCIE;                       /* Interrupt on compare.        */
}
