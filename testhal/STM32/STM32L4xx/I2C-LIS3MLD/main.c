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
#include "lis3mdl.h"

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE         TRUE

static BaseSequentialStream * chp = (BaseSequentialStream*) &SD2;

/* LIS3MDL Driver: This object represent an LIS3MDL instance */
static LIS3MDLDriver LIS3MDLD1;

static int32_t rawdata[LIS3MDL_NUMBER_OF_AXES];
static float cookeddata[LIS3MDL_NUMBER_OF_AXES];

static char axesID[LIS3MDL_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
   STM32_TIMINGR_PRESC(15U) |
   STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
   STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
   0,
   0
};

static const LIS3MDLConfig lis3mdlcfg = {
  &I2CD1,
  &i2ccfg,
  LIS3MDL_SAD_VCC,
  LIS3MDL_FS_4GA,
  LIS3MDL_ODR_40HZ,
  LIS3MDL_LP_DISABLED,
  LIS3MDL_MD_CONTINUOUS,
  LIS3MDL_OMXY_ULTRA,
  LIS3MDL_OMZ_ULTRA,
  LIS3MDL_TEMP_ENABLED,
  LIS3MDL_BDU_BLOCKED,
  LIS3MDL_END_LITTLE
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
   * LIS3MDL Object Initialization
   */
  lis3mdlObjectInit(&LIS3MDLD1);

  lis3mdlStart(&LIS3MDLD1, &lis3mdlcfg);

  while (TRUE) {
    palToggleLine(LINE_LED_GREEN);
    compassReadRaw(&LIS3MDLD1, rawdata);
    compassReadCooked(&LIS3MDLD1, cookeddata);
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
    chprintf(chp, "COMPASS DATA\r\n");
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\t\t", axesID[i], rawdata[i]);
    chprintf(chp, "\r\n");
    for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%.3f Gauss\t", axesID[i], cookeddata[i]);
    chprintf(chp, "\r\n");
    chThdSleepMilliseconds(100);
  }
}
