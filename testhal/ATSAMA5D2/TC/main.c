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

static TCConfig tccfg = {
  {
   {TC_OUTPUT_DISABLED, 0, NULL},    /* Channel 0.   */
   {TC_OUTPUT_ACTIVE, 440 , NULL},   /* Channel 1, 440 hz clock frequency. */
   {TC_OUTPUT_DISABLED, 0, NULL}     /* Channel 2.   */
  }
};

/*
 * Buzzer thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("buzzer");

  while (true) {
    tcEnableChannel(&TCD0, 1, 50);
    palClearLine(LINE_LED_RED);
    chThdSleepMilliseconds(500);
    tcDisableChannel(&TCD0, 1);
    palSetLine(LINE_LED_RED);
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
  tcInit();
  chSysInit();

  /*
   * Initializes the TC0 driver.
   * PD11 is the TC0 channel 1 output.
   */
  palSetGroupMode(PIOD, PAL_PORT_BIT(11), 0U,
                   PAL_SAMA_FUNC_PERIPH_A | PAL_MODE_SECURE);

  tcStart(&TCD0, &tccfg);
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
