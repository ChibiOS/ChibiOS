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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"

/*
 * GPT1 callback.
 */
static void gpt1cb(GPTDriver *gptp) {

  (void)gptp;
  palClearPad(IOPORT3, GPIOC_LED);
  chSysLockFromIsr();
  gptStartOneShotI(&GPTD2, 200);   /* 0.02 second pulse.*/
  chSysUnlockFromIsr();
}

/*
 * GPT2 callback.
 */
static void gpt2cb(GPTDriver *gptp) {

  (void)gptp;
  palSetPad(IOPORT3, GPIOC_LED);
}

/*
 * GPT1 configuration.
 */
static const GPTConfig gpt1cfg = {
  10000,    /* 10kHz timer clock.*/
  gpt1cb    /* Timer callback.*/
};

/*
 * GPT2 configuration.
 */
static const GPTConfig gpt2cfg = {
  10000,    /* 10kHz timer clock.*/
  gpt2cb    /* Timer callback.*/
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
   * Initializes the GPT drivers 1 and 2.
   */
  gptStart(&GPTD1, &gpt1cfg);
  gptPolledDelay(&GPTD1, 10); /* Small delay.*/
  gptStart(&GPTD2, &gpt2cfg);
  gptPolledDelay(&GPTD2, 10); /* Small delay.*/

  /*
   * Normal main() thread activity, it changes the GPT1 period every
   * five seconds.
   */
  while (TRUE) {
    gptStartContinuous(&GPTD1, 5000);
    chThdSleepMilliseconds(5000);
    gptStopTimer(&GPTD1);
    gptStartContinuous(&GPTD1, 2500);
    chThdSleepMilliseconds(5000);
    gptStopTimer(&GPTD1);
  }
  return 0;
}
