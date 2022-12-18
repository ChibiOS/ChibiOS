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

#include <stdint.h>
#include <stdbool.h>

#include "ch.h"
#include "hal.h"

#include "chprintf.h"

#include "rt_test_root.h"
#include "oslib_test_root.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED3);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED3);
    chThdSleepMilliseconds(200);
  }
}

#include "arm_cmse.h"
#include "startup_defs.h"
cmse_address_info_t info;

/*
 * Application entry point.
 */
int main(void) {
  thread_t *ns_tp = NULL;

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
   * Activates the serial driver 3 using the driver default configuration.
   */
  sdStart(&LPSD1, NULL);

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /* Fixed regions.*/
  sauEnableRegion(0, 0x40000000, 0x50000000, SAU_REGION_NOT_NSC);   /* Non secure peripherals.*/
  sauEnableRegion(1, 0x60000000, 0xA0000000, SAU_REGION_NOT_NSC);   /* Non secure external.*/
  sauEnableRegion(2, 0x0BF90000, 0x0BFA9000, SAU_REGION_NOT_NSC);   /* Non secure system memory.*/

  /* Regions depending on scatter file configuration.*/
  sauEnableRegion(3, (uint32_t)STARTUP_FLASH1_BASE, (uint32_t)STARTUP_FLASH1_END, SAU_REGION_NSC);
  sauEnableRegion(4, (uint32_t)STARTUP_FLASH2_BASE, (uint32_t)STARTUP_FLASH2_END, SAU_REGION_NOT_NSC);
  sauEnableRegion(5, (uint32_t)STARTUP_RAM1_BASE, (uint32_t)STARTUP_RAM1_END, SAU_REGION_NOT_NSC);
  sauEnable();

  info = cmse_TT((void *)&__flash0_base__);
  chprintf((BaseSequentialStream *)&LPSD1, "%08x: %08x\r\n", &__flash0_base__, info);
  info = cmse_TT((void *)&__flash2_base__);
  chprintf((BaseSequentialStream *)&LPSD1, "%08x: %08x\r\n", &__flash2_base__, info);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    if (palReadLine(LINE_BUTTON)) {
      static THD_WORKING_AREA(ns_thd_wa, 256);
      void *ns_entry = (void *)STARTUP_FLASH2_BASE;

//      test_execute((BaseSequentialStream *)&LPSD1, &rt_test_suite);
//      test_execute((BaseSequentialStream *)&LPSD1, &oslib_test_suite);
      if (ns_tp == NULL) {
        ns_tp = chThdCreateStatic(ns_thd_wa, sizeof ns_thd_wa, LOWPRIO,
                                  __port_ns_boot, ns_entry);
        chThdWait(ns_tp);
        ns_tp = NULL;
      }
    }
    chThdSleepMilliseconds(500);
  }
}
