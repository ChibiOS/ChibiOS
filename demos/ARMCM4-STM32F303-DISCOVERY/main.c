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
//#include "test.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED3_RED);
    palSetPad(GPIOE, GPIOE_LED5_ORANGE);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED5_ORANGE);
    palSetPad(GPIOE, GPIOE_LED7_GREEN);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED7_GREEN);
    palSetPad(GPIOE, GPIOE_LED9_BLUE);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED9_BLUE);
    palSetPad(GPIOE, GPIOE_LED10_RED);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED10_RED);
    palSetPad(GPIOE, GPIOE_LED8_ORANGE);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED8_ORANGE);
    palSetPad(GPIOE, GPIOE_LED6_GREEN);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED6_GREEN);
    palSetPad(GPIOE, GPIOE_LED4_BLUE);
    chThdSleepMilliseconds(100);
    palClearPad(GPIOE, GPIOE_LED4_BLUE);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /* TODO: inialize serial driver 1 or 2 */

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched.
   */
  while (TRUE) {
    /* TODO */
//    if (palReadPad(GPIOA, GPIOA_BUTTON))
//      TestThread(&SD2);
    chThdSleepMilliseconds(500);
  }
}
