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
#include "rt_test_root.h"
#include "oslib_test_root.h"

/** @brief Driver default configuration.*/
static const SIOConfig siocfg1 = {
  .baud  = 38400,
  .presc = USART_PRESC1,
  .cr1   = USART_CR1_DATA8 | USART_CR1_OVER16,
  .cr2   = USART_CR2_STOP1_BITS,
  .cr3   = USART_CR3_TXFTCFG_1H | USART_CR3_RXFTCFG_1H
};

static const SIOOperation sioop1 = {
  .rx_cb      = NULL,
  .rx_idle_cb = NULL,
  .tx_cb      = NULL,
  .tx_end_cb  = NULL,
  .rx_evt_cb  = NULL
};

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
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

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sioStart(&LPSIOD1, &siocfg1);
  sioStartOperation(&LPSIOD1, &sioop1);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
   if (palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&LPSIOD1, &rt_test_suite);
      test_execute((BaseSequentialStream *)&LPSIOD1, &oslib_test_suite);
    }
    chThdSleepMilliseconds(500);
    chnWrite((BaseSequentialStream *)&LPSIOD1, (const uint8_t *)"Hello World!!\r\n", 15U);
  }
}

