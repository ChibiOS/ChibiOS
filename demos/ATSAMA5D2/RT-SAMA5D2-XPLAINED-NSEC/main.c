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
#include "chprintf.h"
#include "smcclient.h"
/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_LED_RED);
    chThdSleepMilliseconds(80);
    palToggleLine(LINE_LED_RED);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_RED);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_RED);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_RED);
    chThdSleepMilliseconds(160);
    palToggleLine(LINE_LED_RED);
    chThdSleepMilliseconds(600);
  }
}

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
};

/*
 * Application entry point.
 */
int main(void) {
  smc_service_t smcsvc;
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
   * Activates the serial driver 0 using the driver default configuration.
   */
  sdStart(&SD0, &sdcfg);

  /* Redirecting  UART0 RX on PD2 and UART0 TX on PD3. */
  palSetGroupMode(PIOB, PAL_PORT_BIT(26) | PAL_PORT_BIT(27), 0U,
                  PAL_SAMA_FUNC_PERIPH_C);
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO-1, Thread1, NULL);

  /*
   * Call the dummy secure service
   */
  chprintf((BaseSequentialStream*)&SD0, "Calling the 'dummy' secure service\n\r");

  /* Retrieve the service handle by name */
  smcsvc = (smc_service_t) smcInvokeService(
      SMC_HND_GET, (smc_params_area_t)"DummyTrustedService",
      sizeof "DummyTrustedService");
  /*
   * Normal main() thread activity, in this demo it does nothing except
   * calling periodically the dummy service and check the button state.
   */
  while (true) {
    msg_t r;

    /* Invoke the service */
    r = smcInvokeService(smcsvc, (smc_params_area_t)"HELO", sizeof "HELO");
    chprintf((BaseSequentialStream*)&SD0, "Call result: %d\r\n", r);
    if(!palReadPad(PIOB, PIOB_USER_PB)) {
#if 1
      test_execute((BaseSequentialStream *)&SD0, &rt_test_suite);
      test_execute((BaseSequentialStream *)&SD0, &oslib_test_suite);
#endif
    }
    chThdSleepMilliseconds(500);
  }
}
