/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#include "m25q.h"

const QSPIConfig qspicfg1 = {
  NULL,
  STM32_DCR_FSIZE(24) | STM32_DCR_CSHT(1)
};

qspi_command_t cmd_read_id = {
  QSPI_CFG_CMD(0x9E) | QSPI_CFG_CMD_MODE_ONE_LINE |
  QSPI_CFG_ADDR_MODE_NONE |
  QSPI_CFG_ALT_MODE_NONE |
  QSPI_CFG_DUMMY_CYCLES(0) |
  QSPI_CFG_DATA_MODE_ONE_LINE,
  0,
  0
};

uint8_t buffer[512];

M25QDriver m25q;

const M25QConfig m25qcfg1 = {
  &QSPID1,
  &qspicfg1
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
   * Initializing and starting M25Q driver.
   */
  m25qObjectInit(&m25q);
  m25qStart(&m25q, &m25qcfg1);

  /* Reading it back.*/
  err = flashRead(&m25q, 0, buffer, 128);
  if (err != FLASH_NO_ERROR)
    chSysHalt("read error");

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
