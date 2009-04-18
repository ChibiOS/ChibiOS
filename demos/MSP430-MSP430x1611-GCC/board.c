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

#include <ch.h>
#include <signal.h>

#include "board.h"
#include "msp430_serial.h"

/*
 * Hardware initialization goes here.
 * NOTE: Interrupts are still disabled.
 */
void hwinit(void) {

  /*
   * Clock sources setup.
   */
  DCOCTL  = VAL_DCOCTL;
  BCSCTL1 = VAL_BCSCTL1;
#if defined(MSP_USE_XT2CLK)
  do {
    int i;
    IFG1 &= ~OFIFG;
     for (i = 255; i > 0; i--)
       asm("nop");
  } while (IFG1 & OFIFG);
#endif
  BCSCTL2 = VAL_BCSCTL2;

  /*
   * I/O ports initialization.
   */
  P1OUT = VAL_P1OUT;
  P1DIR = VAL_P1DIR;
  P1SEL = VAL_P1SEL;

  P2OUT = VAL_P2OUT;
  P2DIR = VAL_P2DIR;
  P2SEL = VAL_P2SEL;

  P3OUT = VAL_P3OUT;
  P3DIR = VAL_P3DIR;
  P3SEL = VAL_P3SEL;

  P4OUT = VAL_P4OUT;
  P4DIR = VAL_P4DIR;
  P4SEL = VAL_P4SEL;

  P5OUT = VAL_P5OUT;
  P5DIR = VAL_P5DIR;
  P5SEL = VAL_P5SEL;

  P6OUT = VAL_P6OUT;
  P6DIR = VAL_P6DIR;
  P6SEL = VAL_P6SEL;

  /*
   * Timer 0 setup, uses SMCLK as source.
   */
  TACCR0 = SMCLK / 4 / CH_FREQUENCY - 1;/* Counter limit.               */
  TACTL = TACLR;                        /* Clean start.                 */
  TACTL = TASSEL_2 | ID_2 | MC_1;       /* Src=SMCLK, ID=4, cmp=TACCR0. */
  TACCTL0 = CCIE;                       /* Interrupt on compare.        */

  /*
   * Other subsystems.
   */
  serial_init();
}

CH_IRQ_HANDLER(TIMERA0_VECTOR) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
