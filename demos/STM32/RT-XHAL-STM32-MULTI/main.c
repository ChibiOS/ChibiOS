/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

static hal_buffered_sio_c bsio1;
static uint8_t rxbuf[32];
static uint8_t txbuf[32];

/* Testing in progress.*/
static bool testing = false;

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(testing ? 100 : 500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
#if defined (__TEST_RT) || defined (__TEST_OSLIB)
  BaseSequentialStream *stream;
#endif
  msg_t msg;

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
   * Activates the SIO driver using the default configuration.
   */
  bsioObjectInit(&bsio1, &PORTAB_SIO1,
                 rxbuf, sizeof rxbuf,
                 txbuf, sizeof txbuf);
  msg = drvStart(&bsio1, NULL);
  osalDbgAssert(msg == HAL_RET_SUCCESS, "buffered SIO start failed");
#if defined (__TEST_RT) || defined (__TEST_OSLIB)
  stream = (BaseSequentialStream *)&bsio1.chn;
#endif

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
   if (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
     testing = true;
#if defined (__TEST_RT)
      test_execute(stream, &rt_test_suite);
#endif
#if defined (__TEST_OSLIB)
      test_execute(stream, &oslib_test_suite);
#endif
      testing = false;
    }
#endif
    chThdSleepMilliseconds(500);
  }
}
