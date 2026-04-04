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

#include "hal_mfs.h"

#include "mfs_test_root.h"

#include "portab.h"

/*
 * MFS configuration.  Both RP2040 (2 MB, 512 sectors) and RP2350
 * (4 MB, 1024 sectors) use uniform 4 KB sectors, so a single
 * definition works for both: the last 2 sectors of flash are
 * reserved for two 4 KB MFS banks (one sector each).
 */
const MFSConfig mfscfg1 = {
  .flashp           = (BaseFlash *)&EFLD1,
  .erased           = 0xFFFFFFFFU,
  .bank_size        = 4096U,
  .bank0_start      = RP_FLASH_SECTORS_COUNT - 2U,
  .bank0_sectors    = 1U,
  .bank1_start      = RP_FLASH_SECTORS_COUNT - 1U,
  .bank1_sectors    = 1U
};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
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

  /* Board-dependent GPIO setup code.*/
  portab_setup();

  /* Starting EFL driver.*/
  eflStart(&EFLD1, NULL);

  /* Starting SIO driver for test report output.*/
  sioStart(&PORTAB_SD1, NULL);

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Small delay to let UART settle.*/
  chThdSleepMilliseconds(100);

  /* Running the MFS test suite.*/
  test_execute((BaseSequentialStream *)&PORTAB_SD1, &mfs_test_suite);

  /* Normal main() thread activity, in this demo it does nothing.*/
  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
