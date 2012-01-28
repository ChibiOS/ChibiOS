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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "i2c_pns.h"
#include "lis3.h"
#include "tmp75.h"
#include "fake.h"


/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(BlinkWA, 64);
static msg_t Blink(void *arg) {
  chRegSetThreadName("Blink");
  (void)arg;
  while (TRUE) {
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Accelerometer thread
 */
static WORKING_AREA(PollAccelThreadWA, 256);
static msg_t PollAccelThread(void *arg) {
  chRegSetThreadName("PollAccel");
  (void)arg;
  while (TRUE) {
    /*chThdSleepMilliseconds(rand() & 31);*/
    chThdSleepMilliseconds(32);
    request_acceleration_data();
  }
  return 0;
}


/* Temperature polling thread */
static WORKING_AREA(PollTmp75ThreadWA, 256);
static msg_t PollTmp75Thread(void *arg) {
  chRegSetThreadName("PollTmp75");
  (void)arg;
  while (TRUE) {
    /*chThdSleepMilliseconds(rand() & 31);*/
    chThdSleepMilliseconds(15);
    /* Call reading function */
    request_temperature();
  }
  return 0;
}


/* Temperature polling thread */
static WORKING_AREA(PollFakeThreadWA, 256);
static msg_t PollFakeThread(void *arg) {
  chRegSetThreadName("PollFake");
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(16);
    /* Call reading function */
    request_fake();
  }
  return 0;
}


/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(void) {

  halInit();
  chSysInit();

  chThdSleepMilliseconds(200);
  I2CInit_pns();

  /* Create accelerometer thread */
  chThdCreateStatic(PollAccelThreadWA,
          sizeof(PollAccelThreadWA),
          NORMALPRIO,
          PollAccelThread,
          NULL);

  /* Create temperature thread */
  chThdCreateStatic(PollTmp75ThreadWA,
          sizeof(PollTmp75ThreadWA),
          NORMALPRIO,
          PollTmp75Thread,
          NULL);

  /* Create not responding thread */
  chThdCreateStatic(PollFakeThreadWA,
          sizeof(PollFakeThreadWA),
          NORMALPRIO,
          PollFakeThread,
          NULL);

  /* Creates the blinker thread. */
  chThdCreateStatic(BlinkWA, sizeof(BlinkWA), HIGHPRIO, Blink, NULL);

  /* main loop that do nothing */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }

  return 0;
}
