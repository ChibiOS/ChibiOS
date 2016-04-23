/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
#include "lsm6ds0.h"

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE         TRUE

static BaseSequentialStream * chp = (BaseSequentialStream*) &SD2;

/* LSM6DS0 Driver: This object represent an LSM6DS0 instance */
static LSM6DS0Driver LSM6DS0D1;

static int32_t rawdata[LSM6DS0_ACC_NUMBER_OF_AXES + LSM6DS0_GYRO_NUMBER_OF_AXES];
static float cookeddata[LSM6DS0_ACC_NUMBER_OF_AXES + LSM6DS0_GYRO_NUMBER_OF_AXES];
static float temp;

static char axesID[LSM6DS0_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
   STM32_TIMINGR_PRESC(15U) |
   STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
   STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
   0,
   0
};

static const LSM6DS0AccConfig lsm6ds0acccfg = {
  LSM6DS0_ACC_FS_2G,
  LSM6DS0_ACC_ODR_50Hz,
  LSM6DS0_ACC_AE_XYZ,
  LSM6DS0_ACC_OBW_AA,
  LSM6DS0_ACC_HR_ENABLED,
  LSM6DS0_ACC_FDS_DISABLED,
  LSM6DS0_ACC_DCF_9,
  LSM6DS0_ACC_AABW_AUTO,
  LSM6DS0_ACC_DEC_DISABLED,
  LSM6DS0_ACC_UNIT_SI
};

static const LSM6DS0GyroConfig lsm6ds0gyrocfg = {
  LSM6DS0_GYRO_FS_245DSP,
  LSM6DS0_GYRO_ODR_119HZ_FC_31,
  LSM6DS0_GYRO_AE_XYZ,
  LSM6DS0_GYRO_LP_DISABLED,
  LSM6DS0_GYRO_OUT_SEL_0,
  LSM6DS0_GYRO_HP_DISABLED,
  LSM6DS0_GYRO_HPCF_0
};

static const LSM6DS0Config lsm6ds0cfg = {
  &I2CD1,
  &i2ccfg,
  &lsm6ds0acccfg,
  &lsm6ds0gyrocfg,
  LSM6DS0_SAD_VCC,
  LSM6DS0_BDU_BLOCKED,
  LSM6DS0_END_LITTLE
};

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

  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) | PAL_STM32_OSPEED_HIGH);
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) | PAL_STM32_OSPEED_HIGH);

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * L3GD20 Object Initialization
   */
  lsm6ds0ObjectInit(&LSM6DS0D1);

  lsm6ds0Start(&LSM6DS0D1, &lsm6ds0cfg);

  while (TRUE) {
    palToggleLine(LINE_LED_GREEN);
    sensorReadRaw(&LSM6DS0D1, rawdata);
    sensorReadCooked(&LSM6DS0D1, cookeddata);
    gyroscopeGetTemp(&LSM6DS0D1, &temp);
    chprintf(chp, "ACCELEROMETER DATA\r\n");
    for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\t\t", axesID[i], rawdata[i]);
    chprintf(chp, "\r\n");
    for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%.3f g\t", axesID[i], cookeddata[i]);

    chprintf(chp, "\r\nGYROSCOPE DATA\r\n");
    for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\t\t", axesID[i], rawdata[i + 3]);
    chprintf(chp, "\r\n");
    for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%.3f dps\t", axesID[i], cookeddata[i + 3]);
    chprintf(chp, "\r\n");
    chprintf(chp, "TEMPERATURE DATA\r\n");
    chprintf(chp, "LSM6DS0:%.3f C°\t", temp);
  chprintf(chp, "\r\n");
    chThdSleepMilliseconds(100);
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  }
}
