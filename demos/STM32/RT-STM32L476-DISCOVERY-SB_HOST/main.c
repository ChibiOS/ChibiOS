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
#include "sbhost.h"

/*
 * LEDs blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED_RED);
    chThdSleepMilliseconds(200);
    palSetLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED_RED);
    chThdSleepMilliseconds(200);
  }
}

/*
 * Unprivileged thread.
 */
static THD_WORKING_AREA(waUnprivileged1, 128);
static THD_FUNCTION(Unprivileged1, arg) {
  extern uint32_t __flash7_start__, __flash7_end__,
                  __ram7_start__, __ram7_end__;
  static const sb_config_t sb_config = {
    .r0_base        = (uint32_t)&__flash7_start__,
    .r0_end         = (uint32_t)&__flash7_end__,
    .r1_base        = (uint32_t)&__ram7_start__,
    .r1_end         = (uint32_t)&__ram7_end__,
    .stdio_stream   = (SandboxStream *)&SD2
  };
  sb_class_t sbx1;

  (void)arg;
  chRegSetThreadName("unprivileged");

  /* Sandbox object initialization.*/
  sbObjectInit(&sbx1);

  /* Static MPU setup for the sandbox, both regions are used because in this
     demo it requires both a flash and a RAM regions.*/
  mpuConfigureRegion(MPU_REGION_0,
                     sb_config.r0_base,
                     MPU_RASR_ATTR_AP_RO_RO |
                     MPU_RASR_ATTR_CACHEABLE_WT_NWA |
                     MPU_RASR_SIZE_16K |
                     MPU_RASR_ENABLE);
  mpuConfigureRegion(MPU_REGION_1,
                     sb_config.r1_base,
                     MPU_RASR_ATTR_AP_RW_RW |
                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                     MPU_RASR_SIZE_4K |
                     MPU_RASR_ENABLE);

  /* This thread goes in the sandbox and is trapped there, it cannot
     return, only invoke the sandbox API.*/
  sbStart(&sbx1, &sb_config);
  chSysHalt("zombies");
}

/*
 * Application entry point.
 */
int main(void) {
  thread_t *tp;
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

  /* Activating the serial driver 2 using the driver default configuration.*/
  sdStart(&SD2, NULL);

  /* Creating the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO,
                    Thread1, NULL);

  /* Creating the unprivileged thread.*/
  chprintf((BaseSequentialStream *)&SD2, "Starting unprivileged thread\r\n");
  tp = chThdCreateStatic(waUnprivileged1, sizeof(waUnprivileged1),
                         NORMALPRIO - 10U, Unprivileged1, NULL);

  /* Waiting for the unprivileged thread to exit or fail.*/
  msg = chThdWait(tp);
  chprintf((BaseSequentialStream *)&SD2, "Exit code 0x%x\r\n", msg);

  /* Normal main() thread activity, in this demo it does nothing except
     sleeping in a loop and check the button state.*/
  while (true) {
    if (palReadLine(LINE_JOY_CENTER)) {
      test_execute((BaseSequentialStream *)&SD2, &rt_test_suite);
      test_execute((BaseSequentialStream *)&SD2, &oslib_test_suite);
    }
    chThdSleepMilliseconds(500);
  }
}
