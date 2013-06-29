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
#include "chprintf.h"

static void gpt2cb(GPTDriver *gptp)
{
  palTogglePad(IOPORT2, 7);
}

static GPTConfig gpt2cfg =
{
  1000,         /* Timer clock.    */
  gpt2cb        /* Timer callback. */
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

  palSetPadMode(IOPORT2, 7, PAL_MODE_OUTPUT_PUSHPULL);

  sdStart(&SD1, NULL);
  gptStart(&GPTD1, &gpt2cfg);

  gptStartContinuous(&GPTD1, 500);
  while (1) {
    chprintf(&SD1, "OCR1A: %d, TCCR1B: %x, period: %d, counter: %d , TCNT1: %d\r\n",
                   OCR1A,
                   TCCR1B,
                   GPTD1.period,
                   GPTD1.counter,
                   TCNT1);
    chThdSleepMilliseconds(100);
  }
}
