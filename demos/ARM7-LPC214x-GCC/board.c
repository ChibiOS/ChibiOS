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

#include "lpc214x.h"
#include "vic.h"
#include "lpc214x_serial.h"
#include "lpc214x_ssp.h"

#include "board.h"
#include "mmcsd.h"
#include "buzzer.h"

/*
 * Non-vectored IRQs handling here.
 */
__attribute__((naked))
static void IrqHandler(void) {

  chSysIRQEnterI();

  /* nothing */
  VICVectAddr = 0;

  chSysIRQExitI();
}

/*
 * Timer 0 IRQ handling here.
 */
__attribute__((naked))
static void T0IrqHandler(void) {

  chSysIRQEnterI();

  T0IR = 1;             /* Clear interrupt on match MR0. */
  chSysTimerHandlerI();
  VICVectAddr = 0;

  chSysIRQExitI();
}

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {

  /*
   * All peripherals clock disabled by default in order to save power.
   */
  PCONP = PCRTC | PCTIM0;

  /*
   * MAM setup.
   */
  MAMTIM = 0x3;                 /* 3 cycles for flash accesses. */
  MAMCR  = 0x2;                 /* MAM fully enabled. */

  /*
   * PLL setup for Fosc=12MHz and CCLK=48MHz.
   * P=2 M=3.
   */
  PLL *pll = PLL0Base;
  pll->PLL_CFG  = 0x23;         /* P and M values. */
  pll->PLL_CON  = 0x1;          /* Enables the PLL 0. */
  pll->PLL_FEED = 0xAA;
  pll->PLL_FEED = 0x55;
  while (!(pll->PLL_STAT & 0x400))
    ;                           /* Wait for PLL lock. */

  pll->PLL_CON  = 0x3;          /* Connects the PLL. */
  pll->PLL_FEED = 0xAA;
  pll->PLL_FEED = 0x55;

  /*
   * VPB setup.
   * PCLK = CCLK / 4.
   */
  VPBDIV = VPD_D4;

  /*
   * I/O pins configuration.
   */
  PINSEL0 = VAL_PINSEL0;
  PINSEL1 = VAL_PINSEL1;
  PINSEL2 = VAL_PINSEL2;
  IO0DIR = VAL_FIO0DIR;
  IO0SET = 0xFFFFFFFF;
  IO1DIR = VAL_FIO1DIR;
  IO1SET = 0xFFFFFFFF;
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {

  /*
   * Interrupt vectors assignment.
   */
  InitVIC();
  VICDefVectAddr = (IOREG32)IrqHandler;

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
   * Other subsystems.
   */
  InitSerial(1, 2);
  InitSSP();
  InitMMC();
  InitBuzzer();

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}
