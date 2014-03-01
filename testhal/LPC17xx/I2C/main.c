/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

#include <stdlib.h>

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

BaseSequentialStream  * chp = (BaseSequentialStream  *)&SD1;

/* buffers depth */
#define EEPROM_RX_DEPTH 10
#define EEPROM_TX_DEPTH 12

#define EEPROM_WR_ADRESS_H  0x00
#define EEPROM_WR_ADRESS_L  0x00

#define EEPROM_RD_ADRESS_H  0x00
#define EEPROM_RD_ADRESS_L  0x00

static uint8_t rxbuf[EEPROM_RX_DEPTH];
static uint8_t txbuf[EEPROM_TX_DEPTH];

static i2cflags_t errors = 0;
#define EEPROM_24LC64_ADDR 0b1010000


/* I2C interface #2 */
static const I2CConfig i2ccfg2 = {
    I2C_FAST_MODE,
    400000
};

/*
 * Application entry point.
 */
int main(void) {
  msg_t status = RDY_OK;
  systime_t tmo = MS2ST(4);
  uint8_t i;

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
   * Starts I2C
   */
  i2cStart(&I2CD2, &i2ccfg2);

  /*
   * Prepares the Serial driver 1
   */
  sdStart(&SD1, NULL);          /* Default is 38400-8-N-1.*/

  txbuf[0] = EEPROM_WR_ADRESS_H; /* register address */
  txbuf[1] = EEPROM_WR_ADRESS_L;
  for (i = 2; i < EEPROM_TX_DEPTH; i++)
    txbuf[i] = i - 2;

  i2cAcquireBus(&I2CD2);
  status = i2cMasterTransmitTimeout(&I2CD2, EEPROM_24LC64_ADDR, txbuf, EEPROM_TX_DEPTH, rxbuf, 0, tmo);
  i2cReleaseBus(&I2CD2);

  if (status != RDY_OK){
    errors = i2cGetErrors(&I2CD2);
    chprintf(chp, "EEPROM write error code: %d\n\r", errors);
  }
  else {
    chprintf(chp, "\n\rEEPROM write:\t");
    for (i = 2; i < EEPROM_TX_DEPTH; i++) {
      chprintf(chp, "0x%02x ", txbuf[i]);
    }
  }

  chThdSleepMilliseconds(5);

  /* Read */
  txbuf[0] = EEPROM_RD_ADRESS_H; /* register address */
  txbuf[1] = EEPROM_RD_ADRESS_L;
  i2cAcquireBus(&I2CD2);
  status = i2cMasterTransmitTimeout(&I2CD2, EEPROM_24LC64_ADDR, txbuf, 2, rxbuf, EEPROM_RX_DEPTH, tmo);
  i2cReleaseBus(&I2CD2);

  if (status != RDY_OK) {
      errors = i2cGetErrors(&I2CD2);
      chprintf(chp, "EEPROM read error code: %d\n\r", errors);
   }

  chprintf(chp, "\n\rEEPROM read:\t ");
  for (i = 0; i < EEPROM_RX_DEPTH; i++) {
    chprintf(chp, "0x%02x ", rxbuf[i]);
  }
  /*
   * Normal main() thread activity, nothing in this test.
   */
  while (TRUE) {
    palTogglePad(GPIO0, GPIO0_LED2_RED);
    chThdSleepMilliseconds(100);
  }
}



