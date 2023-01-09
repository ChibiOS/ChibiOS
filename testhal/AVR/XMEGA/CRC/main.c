/*
    ChibiOS - Copyright (C) 2016..2018 Theodore Ateba

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
#include "chprintf.h"
#include "hal_crc.h"

BaseSequentialStream *chp = (BaseSequentialStream *) &SD1;

/* Here is an array of data to use with the CRC module. The resul tof the CRC checksum must be 0x31C3*/
uint8_t crcInputData[9] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/* CRC config. */
CRCConfig config = {
  CRC_SOURCE_IO,    /* Source of the data that will use the CRC.  */
  CRC_TYPE_CRC16,   /* CRC algorythm.                             */
};

/**
 * @brief   Blink thread.
 */
static THD_WORKING_AREA(waThread1, 32);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("Blinker");

  while (true) {
    palSetPad(IOPORT5, PORTE_LED);
    chThdSleepMilliseconds(100);
    palClearPad(IOPORT5, PORTE_LED);
    chThdSleepMilliseconds(1000);
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
   * Turn off the LED.
   */
  palClearPad(IOPORT5, PORTE_LED);

  /*
   * Configure TX (PINC3) and RX (PIN2) for the USART1.
   */
  palSetPadMode(IOPORT3, PIN3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(IOPORT3, PIN2, PAL_MODE_INPUT_PULLUP);

  /*
   * Initalization of the CRC module.
   */
  crcInit();

  /*
   * Start the serial driver with's default configuration.
   */
  sdStart(&SD1, NULL);

  chprintf(chp, "\r\n");
  chprintf(chp, "***********************************************\r\n");
  chprintf(chp, "*** ChibiOS/RT testhal/AVR/XMEGA/CRC.\r\n");
  chprintf(chp, "*** Test the CRC module.\r\n");

  /*
   * Reset the CRC module.
   */
  chprintf(chp, "--- Reset the CRC module  result register.\r\n");
  crcReset(&CRCD1, CRC_RESET_RESET0);

  /*
   * Start the CRC module.
   */
  chprintf(chp, "--- Start the CRC module.\r\n");
  crcStart(&CRCD1, &config);

  /*
   * Start CRC generation.
   */
  chprintf(chp, "--- Start the CRC16 generation.\r\n");
  uint16_t retVal = crcComputeCRC16OnData(&CRCD1, crcInputData, sizeof(crcInputData));

  if (retVal == 0x31C3) {
    chprintf(chp, "--- CRC16 generated correspond to what we expected.\r\n");
  }
  else {
    chprintf(chp, "--- CRC ERROR: CRC16 generated do not correspond to what we expected.\r\n");
  }

  /*
   * Starts the LED blinker thread.
   */
  chprintf(chp, "--- Start the LED thread.\r\n");
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

 /*
  * Normal main() thread activity, in this demo it does nothing.
  */
  chprintf(chp, "--- Start the main thread.\r\n");
  while (true) {
    chThdSleepMilliseconds(100);
  }

  return 0;
}
