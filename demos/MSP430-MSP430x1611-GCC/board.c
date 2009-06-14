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

#include <ch.h>
#include <pal.h>

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
   * I/O ports initialization. PxSEL registers are assumed to be cleared after
   * the reset.
   */
  palInit();
  palWritePort(IOPORT_A, VAL_P1OUT);
  pal_lld_msp430_set_direction(IOPORT_A, VAL_P1DIR);

  palWritePort(IOPORT_B, VAL_P2OUT);
  pal_lld_msp430_set_direction(IOPORT_B, VAL_P2DIR);

  palWritePort(IOPORT_C, VAL_P3OUT);
  pal_lld_msp430_set_direction(IOPORT_C, VAL_P3DIR);

  palWritePort(IOPORT_D, VAL_P4OUT);
  pal_lld_msp430_set_direction(IOPORT_D, VAL_P4DIR);

  palWritePort(IOPORT_E, VAL_P5OUT);
  pal_lld_msp430_set_direction(IOPORT_E, VAL_P5DIR);

  palWritePort(IOPORT_F, VAL_P6OUT);
  pal_lld_msp430_set_direction(IOPORT_F, VAL_P6DIR);

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
