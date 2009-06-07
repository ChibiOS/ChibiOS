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
#include <test.h>

#include "board.h"
#include "stm32_serial.h"

static IOBUS_DECL(LedBus, IOPORT_C, 1, GPIOC_LED);

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    palClearPad(IOPORT_C, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT_C, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palTogglePad(IOPORT_C, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palTogglePad(IOPORT_C, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palWritePad(IOPORT_C, GPIOC_LED, PAL_LOW);
    chThdSleepMilliseconds(500);
    palWritePad(IOPORT_C, GPIOC_LED, PAL_HIGH);
    chThdSleepMilliseconds(500);
    palWriteGroup(IOPORT_C, 1, GPIOC_LED, PAL_LOW);
    chThdSleepMilliseconds(500);
    palWriteGroup(IOPORT_C, 1, GPIOC_LED, PAL_HIGH);
    chThdSleepMilliseconds(500);
    palClearPort(IOPORT_C, PAL_PORT_BIT(GPIOC_LED));
    chThdSleepMilliseconds(500);
    palSetPort(IOPORT_C, PAL_PORT_BIT(GPIOC_LED));
    chThdSleepMilliseconds(500);
    palTogglePort(IOPORT_C, PAL_PORT_BIT(GPIOC_LED));
    chThdSleepMilliseconds(500);
    palTogglePort(IOPORT_C, PAL_PORT_BIT(GPIOC_LED));
    chThdSleepMilliseconds(500);
    palWriteBus(&LedBus, PAL_LOW);
    chThdSleepMilliseconds(500);
    palWriteBus(&LedBus, PAL_HIGH);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (palReadPad(IOPORT_A, GPIOA_BUTTON))
      TestThread(&COM2);
    chThdSleepMilliseconds(500);
  }
  return 0;
}
