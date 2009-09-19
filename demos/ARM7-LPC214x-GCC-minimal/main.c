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

#include "board.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    palClearPort(IOPORT1, PAL_PORT_BIT(PA_LED2));
    chThdSleepMilliseconds(200);
    palSetPort(IOPORT1, PAL_PORT_BIT(PA_LED1) | PAL_PORT_BIT(PA_LED2));
    chThdSleepMilliseconds(800);
    palClearPort(IOPORT1, PAL_PORT_BIT(PA_LED1));
    chThdSleepMilliseconds(200);
    palSetPort(IOPORT1, PAL_PORT_BIT(PA_LED1) | PAL_PORT_BIT(PA_LED2));
    chThdSleepMilliseconds(800);
  }
  return 0;
}

/*
 * Yellow LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {

  while (TRUE) {
    palClearPad(IOPORT1, PA_LEDUSB);
    chThdSleepMilliseconds(200);
    palSetPad(IOPORT1, PA_LEDUSB);
    chThdSleepMilliseconds(300);
  }
  return 0;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  /*
   * Creates the blinker threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  while (TRUE)
    chThdSleepMilliseconds(1000);
  return 0;
}
