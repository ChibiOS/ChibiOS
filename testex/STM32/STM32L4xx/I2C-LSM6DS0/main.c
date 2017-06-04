/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in gyroliance with the License.
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

#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "lsm6ds0.h"

/*===========================================================================*/
/* LSM6DS0 related.                                                          */
/*===========================================================================*/
/* LSM6DS0 Driver: This object represent an LSM6DS0 instance */
static  LSM6DS0Driver LSM6DS0D1;

static int32_t rawdata[LSM6DS0_ACC_NUMBER_OF_AXES +
                       LSM6DS0_GYRO_NUMBER_OF_AXES];
static float cookeddata[LSM6DS0_ACC_NUMBER_OF_AXES +
                        LSM6DS0_GYRO_NUMBER_OF_AXES];

static char axisID[LSM6DS0_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
	STM32_TIMINGR_PRESC(15U) |
	STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
	STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
	0,
	0
};

static const LSM6DS0AccConfig lsm6ds0acccfg = {
  NULL,                         /* Use default sensitivity.*/
  NULL,                         /* Use default bias.*/
  LSM6DS0_ACC_FS_2G,            /* Full scale value 2g.*/
  LSM6DS0_ACC_ODR_50Hz,         /* Output data rate 100 Hz.*/
#if LSM6DS0_ACC_USE_ADVANCED || defined(__DOXYGEN__)
  LSM6DS0_ACC_OBW_AA,
  LSM6DS0_ACC_HR_ENABLED,
  LSM6DS0_ACC_FDS_DISABLED,
  LSM6DS0_ACC_DCF_9,
  LSM6DS0_ACC_AABW_AUTO,
  LSM6DS0_ACC_DEC_DISABLED,
#endif
};

static const LSM6DS0GyroConfig lsm6ds0gyrocfg = {
  {0, 0, 0},                     /* Use default sensitivity.*/
  {0, 0, 0},                     /* Use default bias.*/
  LSM6DS0_GYRO_FS_245DPS,        /* Full scale value 245DPS.*/
  LSM6DS0_GYRO_ODR_119HZ_FC_31   /* Output data rate 119 Hz.*/
#if LSM6DS0_GYRO_USE_ADVANCED || defined(__DOXYGEN__)
  LSM6DS0_GYRO_LP_DISABLED,
  LSM6DS0_GYRO_OUT_SEL_0,
  LSM6DS0_GYRO_HP_DISABLED,
  LSM6DS0_GYRO_HPCF_0
#endif
};

static const LSM6DS0Config lsm6ds0cfg = {
  &I2CD1,
  &i2ccfg,
  &lsm6ds0acccfg,
  &lsm6ds0gyrocfg,
  LSM6DS0_SAD_VCC,
#if LSM6DS0_USE_ADVANCED || defined(__DOXYGEN__)
  LSM6DS0_BDU_BLOCKED,
  LSM6DS0_END_LITTLE
#endif
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE      TRUE
#define SHELL_WA_SIZE               THD_WORKING_AREA_SIZE(2048)

static void cmd_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    chprintf(chp, "Usage: read [acc|gyro|both] [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "acc")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        accelerometerReadRaw(&LSM6DS0D1, rawdata);
        chprintf(chp, "LSM6DS0 Accelerometer raw data...\r\n");
        for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
        }
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        accelerometerReadCooked(&LSM6DS0D1, cookeddata);
        chprintf(chp, "LSM6DS0 Accelerometer cooked data...\r\n");
        for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f mG\r\n", axisID[i], cookeddata[i]);
        }
      }
      else {
        chprintf(chp, "Usage: read [acc|gyro|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "gyro")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        gyroscopeReadRaw(&LSM6DS0D1, rawdata);
        chprintf(chp, "LSM6DS0 Gyroscope raw data...\r\n");
        for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
        }
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        gyroscopeReadCooked(&LSM6DS0D1, cookeddata);
        chprintf(chp, "LSM6DS0 Gyroscope cooked data...\r\n");
        for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f DPS\r\n", axisID[i], cookeddata[i]);
        }
      }
      else {
        chprintf(chp, "Usage: read [acc|gyro|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "both")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadRaw(&LSM6DS0D1, rawdata);
        chprintf(chp, "LSM6DS0 Accelerometer raw data...\r\n");
        for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
        }
        chprintf(chp, "LSM6DS0 Gyroscope raw data...\r\n");
        for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i],
                   rawdata[i + LSM6DS0_ACC_NUMBER_OF_AXES]);
        }
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadCooked(&LSM6DS0D1, cookeddata);
        chprintf(chp, "LSM6DS0 Accelerometer cooked data...\r\n");
        for(i = 0; i < LSM6DS0_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f mG\r\n", axisID[i], cookeddata[i]);
        }
        chprintf(chp, "LSM6DS0 Gyroscope cooked data...\r\n");
        for(i = 0; i < LSM6DS0_GYRO_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f DPS\r\n", axisID[i],
                   cookeddata[i + LSM6DS0_ACC_NUMBER_OF_AXES]);
        }
      }
      else {
        chprintf(chp, "Usage: read [acc|gyro|both] [raw|cooked]\r\n");
        return;
      }
    }
    else {
      chprintf(chp, "Usage: read [acc|gyro|both] [raw|cooked]\r\n");
      return;
    }
  }
  chprintf(chp, "Stopped\r\n");
}

static void cmd_fullscale(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc < 1) {
    chprintf(chp, "Usage: fullscale [acc|gyro] [value]\r\n");
    return;
  }
  if (!strcmp (argv[0], "acc")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
    if(!strcmp (argv[1], "2G")) {
      accelerometerSetFullScale(&LSM6DS0D1, LSM6DS0_ACC_FS_2G);
      chprintf(chp, "LSM6DS0 Accelerometer full scale set to 2G...\r\n");
    }
    else if(!strcmp (argv[1], "4G")) {
      accelerometerSetFullScale(&LSM6DS0D1, LSM6DS0_ACC_FS_4G);
      chprintf(chp, "LSM6DS0 Accelerometer full scale set to 4G...\r\n");
    }
    else if(!strcmp (argv[1], "8G")) {
      accelerometerSetFullScale(&LSM6DS0D1, LSM6DS0_ACC_FS_8G);
      chprintf(chp, "LSM6DS0 Accelerometer full scale set to 8G...\r\n");
    }
    else if(!strcmp (argv[1], "16G")) {
      accelerometerSetFullScale(&LSM6DS0D1, LSM6DS0_ACC_FS_16G);
      chprintf(chp, "LSM6DS0 Accelerometer full scale set to 16G...\r\n");
    }
    else {
      chprintf(chp, "Usage: fullscale acc [2G|4G|8G|16G]\r\n");
      return;
    }
  }
  else if (!strcmp (argv[0], "gyro")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      if(!strcmp (argv[1], "245")) {
        gyroscopeSetFullScale(&LSM6DS0D1, LSM6DS0_GYRO_FS_245DPS);
        chprintf(chp, "LSM6DS0 Gyroscope full scale set to 245 DPS...\r\n");
      }
      else if(!strcmp (argv[1], "500")) {
        gyroscopeSetFullScale(&LSM6DS0D1, LSM6DS0_GYRO_FS_500DPS);
        chprintf(chp, "LSM6DS0 Gyroscope full scale set to 500 DPS...\r\n");
      }
      else if(!strcmp (argv[1], "2000")) {
        gyroscopeSetFullScale(&LSM6DS0D1, LSM6DS0_GYRO_FS_2000DPS);
        chprintf(chp, "LSM6DS0 Gyroscope full scale set to 2000 DPS...\r\n");
      }
      else {
        chprintf(chp, "Usage: fullscale gyro [245|500|2000]\r\n");
        return;
      }
  }
  else {
    chprintf(chp, "Usage: fullscale [acc|gyro] [value]\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"read", cmd_read},
  {"fullscale", cmd_fullscale},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD2,
  commands
};

/*===========================================================================*/
/* Main code.                                                                */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
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

  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGH | PAL_STM32_OTYPE_OPENDRAIN);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGH | PAL_STM32_OTYPE_OPENDRAIN);

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
   * LSM6DS0 Object Initialization
   */
  lsm6ds0ObjectInit(&LSM6DS0D1);

  /*
   * Activates the LSM6DS0 driver.
   */
  lsm6ds0Start(&LSM6DS0D1, &lsm6ds0cfg);

  /*
   * Shell manager initialization.
   */
  shellInit();

  while(TRUE) {

    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);                  /* Waiting termination.             */
    chThdSleepMilliseconds(1000);
  }
  lsm6ds0Stop(&LSM6DS0D1);
  return 0;
}
