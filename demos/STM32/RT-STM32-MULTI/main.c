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

#if defined (__TEST_RT)
#include "rt_test_root.h"
#endif
#if defined (__TEST_OSLIB)
#include "oslib_test_root.h"
#endif

#include "portab.h"

/* Testing in progress.*/
static bool testing = false;

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(testing ? 100 : 500);
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
   * Initialization of portability code, could be empty.
   */
  portab_setup();

  /*
   * Activates the Serial or SIO driver using the default configuration.
   */
  sdStart(&PORTAB_SD1, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 10, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
#if defined (__TEST_RT) || defined (__TEST_OSLIB)
   if (palReadLine(LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
     testing = true;
#if defined (__TEST_RT)
      test_execute((BaseSequentialStream *)&PORTAB_SD1, &rt_test_suite);
#endif
#if defined (__TEST_OSLIB)
      test_execute((BaseSequentialStream *)&PORTAB_SD1, &oslib_test_suite);
#endif
      testing = false;
    }
#endif
    chThdSleepMilliseconds(500);
  }
}

