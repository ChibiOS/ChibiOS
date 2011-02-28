/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * GPT1 callback.
 */
static void gpt1cb(GPTDriver *gptp) {

  (void)gptp;
  palTogglePad(IOPORT3, GPIOC_LED);
}

/*
 * GPT1 configuration.
 */
static const GPTConfig gpt1cfg = {
  10000,    /* 10KHz timer clock.*/
  gpt1cb    /* Timer callback.*/
};

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
   * Initializes the GPT driver 1.
   */
  gptStart(&GPTD1, &gpt1cfg);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    gptStartContinuous(&GPTD1, 5000);
    chThdSleepMilliseconds(5000);
    gptStartContinuous(&GPTD1, 2500);
    chThdSleepMilliseconds(5000);
  }
  return 0;
}
