/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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

#define BOTH_BUTTONS (PAL_PORT_BIT(PA_BUTTON1) | PAL_PORT_BIT(PA_BUTTON2))

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
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

  (void)arg;
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

  (void)argc;
  (void)argv;

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  /*
   * If a button is pressed during the reset then the blinking leds threads
   * are not started in order to make accurate benchmarks.
   */
  if ((palReadPort(IOPORT1) & BOTH_BUTTONS) == BOTH_BUTTONS) {
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
    chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);
  }

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the buttons state.
   */
  while (TRUE) {
    if (!palReadPad(IOPORT1, PA_BUTTON1))
      sdWrite(&SD1, (uint8_t *)"Hello World!\r\n", 14);
    if (!palReadPad(IOPORT1, PA_BUTTON2))
      TestThread(&SD1);
    chThdSleepMilliseconds(500);
  }
  return 0;
}
