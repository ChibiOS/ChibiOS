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
 * The STM32G474 internal flash is uniform 2KB sectors, reserve the final
 * 8KB of the device for the two 4KB MFS banks used by the test suite.
 */
const MFSConfig mfscfg1 = {
  .flashp           = (BaseFlash *)&EFLD1,
  .erased           = 0xFFFFFFFFU,
  .bank_size        = 4096U,
  .bank0_start      = 252U,
  .bank0_sectors    = 2U,
  .bank1_start      = 254U,
  .bank1_sectors    = 2U
};

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

static hal_buffered_sio_c bsio1;
static uint8_t bsio1_ib[128];
static uint8_t bsio1_ob[512];

int main(void) {
  bool button_pressed;

  halInit();
  chSysInit();

  portab_setup();

  bsioObjectInit(&bsio1, &PORTAB_SIO1, bsio1_ib, sizeof bsio1_ib,
                 bsio1_ob, sizeof bsio1_ob);

  drvStart(&EFLD1, NULL);
  drvStart(&bsio1, NULL);

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  button_pressed = false;
  while (true) {
    bool current_pressed;

    current_pressed = palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED;
    if (current_pressed && !button_pressed) {
      test_execute((BaseSequentialStream *)&bsio1.chn, &mfs_test_suite);
    }
    button_pressed = current_pressed;
    chThdSleepMilliseconds(100);
  }
}
