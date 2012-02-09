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

/*
 * LED blinker1 thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker1");
  while (TRUE) {
    palClearPad(GPIO3, GPIO3_LED1);
    chThdSleepMilliseconds(500);
    palSetPad(GPIO3, GPIO3_LED1);
    chThdSleepMilliseconds(500);
  }
}

/*
 * LED blinker2 thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker2");
  while (TRUE) {
    palClearPad(GPIO3, GPIO3_LED2);
    chThdSleepMilliseconds(500);
    palSetPad(GPIO3, GPIO3_LED2);
    chThdSleepMilliseconds(480);
  }
}

/*
 * LED scanlight thread, times are in milliseconds.
 */
static WORKING_AREA(waThread3, 128);
static msg_t Thread3(void *arg) {

  (void)arg;
  chRegSetThreadName("scanner1");
  palSetPort(GPIO2, PAL_PORT_BIT(GPIO2_LED5) |
                    PAL_PORT_BIT(GPIO2_LED6) |
                    PAL_PORT_BIT(GPIO2_LED7) |
                    PAL_PORT_BIT(GPIO2_LED8));
  while (TRUE) {
    palClearPort( GPIO2, PAL_PORT_BIT(GPIO2_LED5));
    chThdSleepMilliseconds(50);
    palSetPort(   GPIO2, PAL_PORT_BIT(GPIO2_LED8));
    chThdSleepMilliseconds(150);
    palClearPort( GPIO2, PAL_PORT_BIT(GPIO2_LED6));
    chThdSleepMilliseconds(50);
    palSetPort(   GPIO2, PAL_PORT_BIT(GPIO2_LED5));
    chThdSleepMilliseconds(150);
    palClearPort( GPIO2, PAL_PORT_BIT(GPIO2_LED7));
    chThdSleepMilliseconds(50);
    palSetPort(   GPIO2, PAL_PORT_BIT(GPIO2_LED6));
    chThdSleepMilliseconds(150);
    palClearPort( GPIO2, PAL_PORT_BIT(GPIO2_LED8));
    chThdSleepMilliseconds(50);
    palSetPort(   GPIO2, PAL_PORT_BIT(GPIO2_LED7));
    chThdSleepMilliseconds(150);
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

  /*
   * Creates the LED threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
  chThdCreateStatic(waThread3, sizeof(waThread3), NORMALPRIO, Thread3, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
