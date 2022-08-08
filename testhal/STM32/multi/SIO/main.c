/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

#include "portab.h"

/*
 * RX consumer thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {
  size_t n;

  (void)arg;

  chRegSetThreadName("consumer");

  while (true) {
    sioevents_t errors;
    uint8_t buf[16];

    n = chnRead(&PORTAB_SIO2, buf, 16);
    if (n > 0) {
      chnWrite(&PORTAB_SIO1, buf, n);
    }
    errors = sioGetAndClearErrors(&PORTAB_SIO2);
    (void) errors;

    sioSynchronizeRXIdle(&PORTAB_SIO2, TIME_INFINITE);
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
   * Board-dependent setup code.
   */
  portab_setup();

  /*
   * Activates the SIO drivers using the default configuration.
   */
  sioStart(&PORTAB_SIO1, NULL);
  sioStartOperation(&PORTAB_SIO1, NULL);
  sioStart(&PORTAB_SIO2, NULL);
  sioStartOperation(&PORTAB_SIO2, NULL);

  /*
   * Creates the RX consumer thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Short TX writes.
   */
  do {
    char c;

    for (c = 'A'; c <= 'Z'; c++) {
      chnWrite(&PORTAB_SIO2, (const uint8_t *)&c, 1);
      sioSynchronizeTXEnd(&PORTAB_SIO2, TIME_INFINITE);
      chThdSleepMilliseconds(100);
    }
  } while (palReadLine(PORTAB_LINE_BUTTON) != PORTAB_BUTTON_PRESSED);

  /* Waiting button release.*/
  while (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
    chThdSleepMilliseconds(100);
  }

  /*
   * Long TX writes.
   */
  do {
    chnWrite(&PORTAB_SIO2, (const uint8_t *)"Hello World!!!\r\n", 16);
    sioSynchronizeTXEnd(&PORTAB_SIO2, TIME_INFINITE);
    chThdSleepMilliseconds(100);
  } while (palReadLine(PORTAB_LINE_BUTTON) != PORTAB_BUTTON_PRESSED);

  /* Waiting button release.*/
  while (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
    chThdSleepMilliseconds(100);
  }

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
