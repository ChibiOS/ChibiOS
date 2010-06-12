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
#include "test.h"

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPad(GPIO0, GPIO0_LED2);
    chThdSleepMilliseconds(500);
    palSetPad(GPIO0, GPIO0_LED2);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * RGB LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    chThdSleepMilliseconds(250);
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    palSetPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B));
    chThdSleepMilliseconds(250);
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    palSetPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3R));
    chThdSleepMilliseconds(250);
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    palSetPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3G));
    chThdSleepMilliseconds(250);
  }
  return 0;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  (void)argc;
  (void)argv;

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  /*
   * Creates the blinker threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (!palReadPad(GPIO0, GPIO0_SW3))
      TestThread(&SD1);
    chThdSleepMilliseconds(500);
  }
  return 0;
}
