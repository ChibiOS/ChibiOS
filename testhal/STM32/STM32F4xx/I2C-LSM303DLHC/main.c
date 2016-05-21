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

#include "usbcfg.h"
#include "chprintf.h"
#include "lsm303dlhc.h"

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE         TRUE

static BaseSequentialStream * chp = (BaseSequentialStream*) &SDU1;

/* LSM303DLHC Driver: This object represent an LSM303DLHC instance */
static LSM303DLHCDriver LSM303DLHCD1;

static int32_t rawdata[LSM303DLHC_ACC_NUMBER_OF_AXES + LSM303DLHC_COMP_NUMBER_OF_AXES];
static float cookeddata[LSM303DLHC_ACC_NUMBER_OF_AXES + LSM303DLHC_COMP_NUMBER_OF_AXES];

static char axesID[LSM303DLHC_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const LSM303DLHCAccConfig lsm303dlhcacccfg = {
  LSM303DLHC_ACC_FS_2G,
  LSM303DLHC_ACC_ODR_100Hz,
  LSM303DLHC_ACC_AE_XYZ,
  LSM303DLHC_ACC_LP_DISABLED,
  LSM303DLHC_ACC_HR_DISABLED,
  LSM303DLHC_ACC_BDU_BLOCK,
  LSM303DLHC_ACC_END_LITTLE,
  LSM303DLHC_ACC_UNIT_G
};

static const LSM303DLHCCompConfig lsm303dlhccompcfg = {
  LSM303DLHC_COMP_FS_1_3_GA,
  LSM303DLHC_COMP_ODR_30HZ,
  LSM303DLHC_COMP_MD_BLOCK
};

static const LSM303DLHCConfig lsm303dlhccfg = {
  &I2CD1,
  &i2ccfg,
  &lsm303dlhcacccfg,
  &lsm303dlhccompcfg
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

  /*
   * Initializes a serial-over-USB CDC driver.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  /*
   * LSM303DLHC Object Initialization
   */
  lsm303dlhcObjectInit(&LSM303DLHCD1);

  lsm303dlhcStart(&LSM303DLHCD1, &lsm303dlhccfg);

  while (TRUE) {
    palToggleLine(LINE_LED3);
    sensorReadRaw(&LSM303DLHCD1, rawdata);
    sensorReadCooked(&LSM303DLHCD1, cookeddata);
    chprintf(chp, "ACCELEROMETER DATA\r\n");
    for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\t\t", axesID[i], rawdata[i]);
    chprintf(chp, "\r\n");
    for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%.3f g\t", axesID[i], cookeddata[i]);

    chprintf(chp, "\r\nCOMPSCOPE DATA\r\n");
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\t\t", axesID[i], rawdata[i + 3]);
    chprintf(chp, "\r\n");
    for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%.3f Gauss\t", axesID[i], cookeddata[i + 3]);
    chprintf(chp, "\r\n");
    chThdSleepMilliseconds(100);
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  }
}
