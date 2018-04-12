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
#include "lwipthread.h"

#include "web/web.h"

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
};

RTCDateTime cfg_time = {
  38,
  4,
  0,
  1,
  9,
  3362400
};

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palClearLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(500);
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
  lwipInit(NULL);

  /* Set current Time */
  rtcSetTime(&RTCD0, &cfg_time);

  /*
   * Activates the serial driver 1 using the driver defined configuration.
   */
  sdStart(&SD1, &sdcfg);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    if(!palReadPad(PIOB, PIOB_USER_PB)) {
      https_client();
    }
    chThdSleepMilliseconds(500);
  }
}
