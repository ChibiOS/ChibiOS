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
 * GPT2 callback.
 */
static void gpt2cb(GPTDriver *gptp) {

  (void)gptp;
  palSetPad(GPIOD, GPIOD_LED5);
  chSysLockFromIsr();
  gptStartOneShotI(&GPTD3, 1000);   /* 0.1 second pulse.*/
  chSysUnlockFromIsr();
}

/*
 * GPT3 callback.
 */
static void gpt3cb(GPTDriver *gptp) {

  (void)gptp;
  palClearPad(GPIOD, GPIOD_LED5);
}

/*
 * GPT2 configuration.
 */
static const GPTConfig gpt2cfg = {
  10000,    /* 10kHz timer clock.*/
  gpt2cb    /* Timer callback.*/
};

/*
 * GPT3 configuration.
 */
static const GPTConfig gpt3cfg = {
  10000,    /* 10kHz timer clock.*/
  gpt3cb    /* Timer callback.*/
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
   * Initializes the GPT drivers 2 and 3.
   */
  gptStart(&GPTD2, &gpt2cfg);
  gptPolledDelay(&GPTD2, 10); /* Small delay.*/
  gptStart(&GPTD3, &gpt3cfg);
  gptPolledDelay(&GPTD3, 10); /* Small delay.*/

  /*
   * Normal main() thread activity, it changes the GPT1 period every
   * five seconds.
   */
  while (TRUE) {
    palSetPad(GPIOD, GPIOD_LED4);
    gptStartContinuous(&GPTD2, 5000);
    chThdSleepMilliseconds(5000);
    gptStopTimer(&GPTD2);
    palClearPad(GPIOD, GPIOD_LED4);
    gptStartContinuous(&GPTD2, 2500);
    chThdSleepMilliseconds(5000);
    gptStopTimer(&GPTD2);
  }
}
