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
#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "lsm303dlhc.h"


/*===========================================================================*/
/* LSM303DLHC related.                                                       */
/*===========================================================================*/

/* LSM303DLHC Driver: This object represent an LSM303DLHC instance */
static LSM303DLHCDriver LSM303DLHCD1;

static int32_t rawdata[LSM303DLHC_ACC_NUMBER_OF_AXES +
                       LSM303DLHC_COMP_NUMBER_OF_AXES];
static float cookeddata[LSM303DLHC_ACC_NUMBER_OF_AXES +
                        LSM303DLHC_COMP_NUMBER_OF_AXES];

static char axisID[LSM303DLHC_ACC_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
  STM32_TIMINGR_PRESC(15U) |
  STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
  STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
  0,
  0
};

static const LSM303DLHCAccConfig lsm303dlhcacccfg = {
  NULL,                             /* Use default sensitivity.*/
  NULL,                             /* Use default bias.*/
  LSM303DLHC_ACC_FS_4G,             /* Full scale value 2g.*/
  LSM303DLHC_ACC_ODR_100Hz,         /* Output data rate 100 Hz.*/
#if LSM303DLHC_ACC_USE_ADVANCED || defined(__DOXYGEN__)
  LSM303DLHC_ACC_LP_DISABLED,
  LSM303DLHC_ACC_HR_DISABLED,
  LSM303DLHC_ACC_BDU_BLOCK,
  LSM303DLHC_ACC_END_LITTLE,
#endif
};

static const LSM303DLHCCompConfig lsm303dlhccompcfg = {
  NULL,                             /* Use default sensitivity.*/
  NULL,                             /* Use default bias.*/
  LSM303DLHC_COMP_FS_1P3GA,         /* Full scale value 1.3 Gauss.*/
  LSM303DLHC_COMP_ODR_30HZ,         /* Output data rate 30 Hz.*/
#if LSM303DLHC_COMP_USE_ADVANCED || defined(__DOXYGEN__)
  LSM303DLHC_COMP_MD_BLOCK
#endif
};

static const LSM303DLHCConfig lsm303dlhccfg = {
  &I2CD1,
  &i2ccfg,
  &lsm303dlhcacccfg,
  &lsm303dlhccompcfg
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/*
 * DP resistor control is not possible on the STM32F3-Discovery, using stubs
 * for the connection macros.
 */
#define usb_lld_connect_bus(usbp)
#define usb_lld_disconnect_bus(usbp)

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE      TRUE
#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static void cmd_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    chprintf(chp, "Usage: read [acc|comp|both] [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "acc")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        accelerometerReadRaw(&LSM303DLHCD1, rawdata);
        chprintf(chp, "LSM303DLHC Accelerometer raw data...\r\n");
        for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
        }
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        accelerometerReadCooked(&LSM303DLHCD1, cookeddata);
        chprintf(chp, "LSM303DLHC Accelerometer cooked data...\r\n");
        for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f mG\r\n", axisID[i], cookeddata[i]);
        }
      }
      else {
        chprintf(chp, "Usage: read [acc|comp|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "comp")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        compassReadRaw(&LSM303DLHCD1, rawdata);
        chprintf(chp, "LSM303DLHC Compass raw data...\r\n");
        for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
        }
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        compassReadCooked(&LSM303DLHCD1, cookeddata);
        chprintf(chp, "LSM303DLHC Compass cooked data...\r\n");
        for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f Gauss\r\n", axisID[i], cookeddata[i]);
        }
      }
      else {
        chprintf(chp, "Usage: read [acc|comp|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "both")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadRaw(&LSM303DLHCD1, rawdata);
        chprintf(chp, "LSM303DLHC Accelerometer raw data...\r\n");
        for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
        }
        chprintf(chp, "LSM303DLHC Compass raw data...\r\n");
        for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %d\r\n", axisID[i],
                   rawdata[i + LSM303DLHC_ACC_NUMBER_OF_AXES]);
        }
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadCooked(&LSM303DLHCD1, cookeddata);
        chprintf(chp, "LSM303DLHC Accelerometer cooked data...\r\n");
        for(i = 0; i < LSM303DLHC_ACC_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f mG\r\n", axisID[i], cookeddata[i]);
        }
        chprintf(chp, "LSM303DLHC Compass cooked data...\r\n");
        for(i = 0; i < LSM303DLHC_COMP_NUMBER_OF_AXES; i++) {
          chprintf(chp, "%c-axis: %.4f Gauss\r\n", axisID[i],
                   cookeddata[i + LSM303DLHC_ACC_NUMBER_OF_AXES]);
        }
      }
      else {
        chprintf(chp, "Usage: read [acc|comp|both] [raw|cooked]\r\n");
        return;
      }
    }
    else {
      chprintf(chp, "Usage: read [acc|comp|both] [raw|cooked]\r\n");
      return;
    }
  }
  chprintf(chp, "Stopped\r\n");
}

static void cmd_fullscale(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc < 1) {
    chprintf(chp, "Usage: fullscale [acc|comp] [value]\r\n");
    return;
  }
  if (!strcmp (argv[0], "acc")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
    if(!strcmp (argv[1], "2G")) {
      accelerometerSetFullScale(&LSM303DLHCD1, LSM303DLHC_ACC_FS_2G);
      chprintf(chp, "LSM303DLHC Accelerometer full scale set to 2G...\r\n");
    }
    else if(!strcmp (argv[1], "4G")) {
      accelerometerSetFullScale(&LSM303DLHCD1, LSM303DLHC_ACC_FS_4G);
      chprintf(chp, "LSM303DLHC Accelerometer full scale set to 4G...\r\n");
    }
    else if(!strcmp (argv[1], "8G")) {
      accelerometerSetFullScale(&LSM303DLHCD1, LSM303DLHC_ACC_FS_8G);
      chprintf(chp, "LSM303DLHC Accelerometer full scale set to 8G...\r\n");
    }
    else if(!strcmp (argv[1], "16G")) {
      accelerometerSetFullScale(&LSM303DLHCD1, LSM303DLHC_ACC_FS_16G);
      chprintf(chp, "LSM303DLHC Accelerometer full scale set to 16G...\r\n");
    }
    else {
      chprintf(chp, "Usage: fullscale acc [2G|4G|8G|16G]\r\n");
      return;
    }
  }
  else if (!strcmp (argv[0], "comp")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      if(!strcmp (argv[1], "1.3")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_1P3GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 1.3 Gauss...\r\n");
      }
      else if(!strcmp (argv[1], "1.9")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_1P9GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 1.9 Gauss...\r\n");
      }
      else if(!strcmp (argv[1], "2.5")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_2P5GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 2.5 Gauss...\r\n");
      }
      else if(!strcmp (argv[1], "4.0")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_4P0GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 4.0 Gauss...\r\n");
      }
      else if(!strcmp (argv[1], "4.7")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_4P7GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 4.7 Gauss...\r\n");
      }
      else if(!strcmp (argv[1], "5.6")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_5P6GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 5.6 Gauss...\r\n");
      }
      else if(!strcmp (argv[1], "8.1")) {
        compassSetFullScale(&LSM303DLHCD1, LSM303DLHC_COMP_FS_8P1GA);
        chprintf(chp, "LSM303DLHC Compass full scale set to 8.1 Gauss...\r\n");
      }
      else {
        chprintf(chp, "Usage: fullscale comp [1.3|1.9|2.5|4.0|4.7|5.6|8.1]\r\n");
        return;
      }
  }
  else {
    chprintf(chp, "Usage: fullscale [acc|comp] [value]\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"read", cmd_read},
  {"fullscale", cmd_fullscale},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    systime_t time;

    time = serusbcfg.usbp->state == USB_ACTIVE ? 250 : 500;
    palClearLine(LINE_LED3_RED);
    chThdSleepMilliseconds(time);
    palSetLine(LINE_LED3_RED);
    chThdSleepMilliseconds(time);
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
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  
  /*
   * LSM303DLHC Object Initialization
   */
  lsm303dlhcObjectInit(&LSM303DLHCD1);

  /*
   * Activates the LSM303DLHC driver.
   */
  lsm303dlhcStart(&LSM303DLHCD1, &lsm303dlhccfg);
  
  /*
   * Normal main() thread activity, spawning shells.
   */   
  while (true) {
    if (SDU1.config->usbp->state == USB_ACTIVE) {
      thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                              "shell", NORMALPRIO + 1,
                                              shellThread, (void *)&shell_cfg1);
      chThdWait(shelltp);               /* Waiting termination.             */
    }
    chThdSleepMilliseconds(1000);
  }
  lsm303dlhcStop(&LSM303DLHCD1);
}
