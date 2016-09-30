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

#include "lis3dsh.h"

/*===========================================================================*/
/* LIS3DSH related.                                                          */
/*===========================================================================*/

/* LIS3DSH Driver: This object represent an LIS3DSH instance */
static LIS3DSHDriver LIS3DSHD1;

static int32_t rawdata[LIS3DSH_NUMBER_OF_AXES];
static float cookeddata[LIS3DSH_NUMBER_OF_AXES];

static char axisID[LIS3DSH_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const SPIConfig spicfg = {
  NULL,
  GPIOE,                                     /* port of LIS3DSH CS.*/
  GPIOE_CS_SPI,                              /* pin of LIS3DSH CS.*/
  SPI_CR1_BR_0 | SPI_CR1_CPOL | SPI_CR1_CPHA,/* CR1 register.*/
  0                                          /* CR2 register.*/
};

static LIS3DSHConfig lis3dshcfg = {
  &SPID1,                                    /* Pointer to SPI Driver.*/
  &spicfg,                                   /* Pointer to SPI Configuration.*/
  NULL,                                      /* Use default sensitivity.*/
  NULL,                                      /* Use default bias.*/
  LIS3DSH_FS_2G,                             /* Full scale value.*/
  LIS3DSH_ODR_100HZ,                         /* Output data rate.*/
#if LIS3DSH_USE_ADVANCED || defined(__DOXYGEN__)
  LIS3DSH_BW_400HZ,                          /* AA filter bandwidth.*/
  LIS3DSH_BDU_CONTINUOUS,                    /* Block data update continuous.*/
#endif
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/* Enable use of special ANSI escape sequences.*/
#define CHPRINTF_USE_ANSI_CODE      TRUE
#define SHELL_WA_SIZE               THD_WORKING_AREA_SIZE(2048)

static void cmd_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 1) {
    chprintf(chp, "Usage: read [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      accelerometerReadRaw(&LIS3DSHD1, rawdata);
      chprintf(chp, "LIS3DSH Accelerometer raw data...\r\n");
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
      }
    }
    else if (!strcmp (argv[0], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      accelerometerReadCooked(&LIS3DSHD1, cookeddata);
      chprintf(chp, "LIS3DSH Accelerometer cooked data...\r\n");
      for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %.4f mg\r\n", axisID[i], cookeddata[i]);
      }
    }
    else {
      chprintf(chp, "Usage: read [raw|cooked]\r\n");
      return;
    }
  }
  chprintf(chp, "Stopped\r\n");
}

static void cmd_fullscale(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 1) {
    chprintf(chp, "Usage: fullscale [2G|4G|6G|8G|16G]\r\n");
    return;
  }
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  if(!strcmp (argv[0], "2G")) {
    accelerometerSetFullScale(&LIS3DSHD1, LIS3DSH_FS_2G);
    chprintf(chp, "LIS3DSH Accelerometer full scale set to 2G...\r\n");
  }
  else if(!strcmp (argv[0], "4G")) {
    accelerometerSetFullScale(&LIS3DSHD1, LIS3DSH_FS_4G);
    chprintf(chp, "LIS3DSH Accelerometer full scale set to 4G...\r\n");
  }
  else if(!strcmp (argv[0], "6G")) {
    accelerometerSetFullScale(&LIS3DSHD1, LIS3DSH_FS_6G);
    chprintf(chp, "LIS3DSH Accelerometer full scale set to 6G...\r\n");
  }
  else if(!strcmp (argv[0], "8G")) {
    accelerometerSetFullScale(&LIS3DSHD1, LIS3DSH_FS_8G);
    chprintf(chp, "LIS3DSH Accelerometer full scale set to 8G...\r\n");
  }
  else if(!strcmp (argv[0], "16G")) {
    accelerometerSetFullScale(&LIS3DSHD1, LIS3DSH_FS_16G);
    chprintf(chp, "LIS3DSH Accelerometer full scale set to 16G...\r\n");
  }
  else {
    chprintf(chp, "Usage: fullscale [2G|4G|6G|8G|16G]\r\n");
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
    systime_t time;

    time = serusbcfg.usbp->state == USB_ACTIVE ? 250 : 500;
    palClearLine(LINE_LED6);
    chThdSleepMilliseconds(time);
    palSetLine(LINE_LED6);
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

  /* Initializes a serial-over-USB CDC driver.*/
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

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /* LIS3DSH Object Initialization.*/
  lis3dshObjectInit(&LIS3DSHD1);

  /* Activates the LIS3DSH driver.*/
  lis3dshStart(&LIS3DSHD1, &lis3dshcfg);

  /* Shell manager initialization.*/
  shellInit();

  while(TRUE) {
    if (SDU1.config->usbp->state == USB_ACTIVE) {
      thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                              "shell", NORMALPRIO + 1,
                                              shellThread, (void *)&shell_cfg1);
      chThdWait(shelltp);               /* Waiting termination.             */
    }
    chThdSleepMilliseconds(1000);
  }
  lis3dshStop(&LIS3DSHD1);
  return 0;
}
