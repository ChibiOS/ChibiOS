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

#include <string.h>

#include "ch.h"
#include "hal.h"

#include "serial_nor.h"

/* 16MB device, 2 cycles delay after NCS.*/
const WSPIConfig wspicfg1 = {
  NULL,
  STM32_DCR_FSIZE(24) | STM32_DCR_CSHT(1)
};

wspi_command_t cmd_read_id = {
  .cmd      = 0x9E,
  .cfg      = WSPI_CFG_CMD_MODE_ONE_LINE |
              WSPI_CFG_ADDR_MODE_NONE |
              WSPI_CFG_ALT_MODE_NONE |
              WSPI_CFG_DATA_MODE_ONE_LINE,
  .addr     = 0,
  .alt      = 0,
  .dummy    = 0
};

/*
 * Generic buffer.
 */
uint8_t buffer[2048];

const uint8_t pattern[128] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

SNORDriver snor1;

const SNORConfig snorcfg1 = {
  &WSPID1,
  &wspicfg1
};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  flash_error_t err;
  uint8_t *addr;

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
   * LED line as output.
   */
  palSetLineMode(LINE_LED_GREEN, PAL_MODE_OUTPUT_PUSHPULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
   * Initializing and starting SNOR driver.
   */
  snorObjectInit(&snor1);
  snorStart(&snor1, &snorcfg1);

  /* Reading.*/
  err = flashRead(&snor1, 0, 128, buffer);
  if (err != FLASH_NO_ERROR)
    chSysHalt("read error");

  /* Erasing the first sector and waiting for completion.*/
  (void) flashStartEraseSector(&snor1, 0);
  err = flashWaitErase((BaseFlash *)&snor1);
  if (err != FLASH_NO_ERROR)
    chSysHalt("erase error");

  /* Verifying the erase operation.*/
  err = flashVerifyErase(&snor1, 0);
  if (err != FLASH_NO_ERROR)
    chSysHalt("verify erase error");

  /* Programming a pattern.*/
  err = flashProgram(&snor1, 0, 128, pattern);
  if (err != FLASH_NO_ERROR)
    chSysHalt("program error");

  /* Verifying the erase operation.*/
  err = flashVerifyErase(&snor1, 0);
  if (err != FLASH_ERROR_VERIFY)
    chSysHalt("verify non-erase error");

  /* Memory mapping the device.*/
  snorMemoryMap(&snor1, &addr);

  /* Unmapping the device.*/
  snorMemoryUnmap(&snor1);

  /* Reading it back.*/
  memset(buffer, 0, 128);
  err = flashRead(&snor1, 16, 128, buffer);
  if (err != FLASH_NO_ERROR)
    chSysHalt("read error");

  /* Reading it back.*/
  memset(buffer, 0, 128);
  err = flashRead(&snor1, 0, 128, buffer);
  if (err != FLASH_NO_ERROR)
    chSysHalt("read error");

  /* Erasing again.*/
  (void) flashStartEraseSector(&snor1, 0);
  err = flashWaitErase((BaseFlash *)&snor1);
  if (err != FLASH_NO_ERROR)
    chSysHalt("erase error");

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
