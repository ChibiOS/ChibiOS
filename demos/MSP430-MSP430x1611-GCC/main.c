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
#include <test.h>

#include "board.h"
#include "msp430_serial.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 64);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    P6OUT |= P6_O_LED;
    chThdSleepMilliseconds(500);
    P6OUT &= ~P6_O_LED;
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Entry point, the interrupts are disabled on entry.
 */
int main(int argc, char **argv) {

  /*
   * Hardware initialization, see board.c.
   */
  hwinit();

  /*
   * The main() function becomes a thread here then the interrupts are
   * enabled and ChibiOS/RT goes live.
   */
  chSysInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  while (TRUE) {
    if (!(P6IN & P6_I_BUTTON))
      TestThread(&COM1);
    chThdSleepMilliseconds(500);
  }
  return 0;
}
