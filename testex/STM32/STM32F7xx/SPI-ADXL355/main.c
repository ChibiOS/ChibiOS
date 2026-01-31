/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
#include "adxl355.h"
#include "ccportab.h"

/*===========================================================================*/
/* ADXL355 related.                                                          */
/*===========================================================================*/

/*
 * SPI TX and RX buffers.
 */
CC_ALIGN_DATA(32) static uint8_t txbuf[ADXL355_COMM_BUFF_SIZE];
CC_ALIGN_DATA(32) static uint8_t rxbuf[ADXL355_COMM_BUFF_SIZE];

/* ADXL355 Driver: This object represent an ADXL355 instance */
static ADXL355Driver ADXL355D1;
static BaseAccelerometer * accp = &(ADXL355D1.acc_if);

static uint32_t axes_count;
static int32_t accraw[ADXL355_ACC_NUMBER_OF_AXES];
static float acccooked[ADXL355_ACC_NUMBER_OF_AXES];

static char axisID[ADXL355_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static const char device_name[] = "ADXL355";
static uint32_t i;

static const SPIConfig spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = NULL,
  .ssline           = LINE_ARD_D9,
  .cr1              = SPI_CR1_BR_1,
  .cr2              = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

static ADXL355Config adxl355cfg = {
  .spip             = &SPID2,
  .spicfg           = &spicfg,
  .accsensitivity   = NULL,
  .accbias          = NULL,
  .accfullscale     = ADXL355_ACC_FS_2G,
  .accodr           = ADXL355_ACC_ODR_125HZ,
#if ADXL355_USE_ADVANCED
  .acchighpass      = ADXL355_ACC_HP_LEV_3
#endif
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

static BaseSequentialStream* chp = (BaseSequentialStream*)&SD1;

static THD_WORKING_AREA(waThreadBlinker, 128);
static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;

  chRegSetThreadName("blinker");

  while (true) {
    palSetLine(LINE_ARD_D13);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_ARD_D13);
    chThdSleepMilliseconds(200);
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

  sdStart(&SD1, NULL);

  palSetLineMode(LINE_ARD_D9, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_ARD_D11, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(LINE_ARD_D12, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(LINE_ARD_D13, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);

  /* Creating LED thread.*/
  chThdCreateStatic(waThreadBlinker, sizeof(waThreadBlinker), NORMALPRIO + 1,
                    ThreadBlinker, NULL);

  /* ADXL355 Object Initialization.*/
  adxl355ObjectInit(&ADXL355D1, txbuf, rxbuf);

  /* Activates the ADXL355 driver.*/
  adxl355Start(&ADXL355D1, &adxl355cfg);

  /* Getting the count of axes. */
  axes_count = accelerometerGetAxesNumber(accp);

  /* Normal main() thread activity, printing MEMS data on the SD1.*/
  while (true) {
    accelerometerReadRaw(accp, accraw);
    chprintf(chp, "%s raw data...\r\n", device_name);
    for(i = 0; i < axes_count; i++) {
      chprintf(chp, "%c-axis: %d\r\n", axisID[i], accraw[i]);
    }

    accelerometerReadCooked(accp, acccooked);
    chprintf(chp, "%s cooked data...\r\n", device_name);
    for(i = 0; i < axes_count; i++) {
      chprintf(chp, "%c-axis: %.3f\r\n", axisID[i], acccooked[i]);
    }
    chThdSleepMilliseconds(100);
  }
}
