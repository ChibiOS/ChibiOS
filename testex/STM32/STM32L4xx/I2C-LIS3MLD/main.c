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

#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "lis3mdl.h"

/*===========================================================================*/
/* LIS3MDL related.                                                           */
/*===========================================================================*/

/* LIS3MDL Driver: This object represent an LIS3MDL instance.*/
static LIS3MDLDriver LIS3MDLD1;

static int32_t rawdata[LIS3MDL_NUMBER_OF_AXES];
static float cookeddata[LIS3MDL_NUMBER_OF_AXES];

static char axisID[LIS3MDL_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const I2CConfig i2ccfg = {
  STM32_TIMINGR_PRESC(15U) |
  STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
  STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
  0,
  0
};

static LIS3MDLConfig LIS3MDLcfg = {
  &I2CD1,                                    /* Pointer to I2C Driver.*/
  &i2ccfg,                                   /* Pointer to I2C Configuration.*/
  NULL,                                      /* Use default sensitivity.*/
  NULL,                                      /* Use default bias.*/
  LIS3MDL_SAD_VCC,                           /* Slave address.*/
  LIS3MDL_FS_4GA,                            /* Full scale value.*/
  LIS3MDL_ODR_40HZ,                          /* Output data rate.*/
#if LIS3MDL_USE_ADVANCED || defined(__DOXYGEN__)
  LIS3MDL_LP_T,
  LIS3MDL_MD_T,
  LIS3MDL_OMXY_T,
  LIS3MDL_OMZ_T,
  LIS3MDL_BDU_T,
  LIS3MDL_END_T
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
      compassReadRaw(&LIS3MDLD1, rawdata);
      chprintf(chp, "LIS3MDL compass raw data...\r\n");
      for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %d\r\n", axisID[i], rawdata[i]);
      }
    }
    else if (!strcmp (argv[0], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      compassReadCooked(&LIS3MDLD1, cookeddata);
      chprintf(chp, "LIS3MDL compass cooked data...\r\n");
      for(i = 0; i < LIS3MDL_NUMBER_OF_AXES; i++) {
        chprintf(chp, "%c-axis: %.3f Gauss\r\n", axisID[i], cookeddata[i]);
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
    chprintf(chp, "Usage: fullscale [4|8|12|16]\r\n");
    return;
  }
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  if(!strcmp (argv[0], "4")) {
    compassSetFullScale(&LIS3MDLD1, LIS3MDL_FS_4GA);
    chprintf(chp, "LIS3MDL compass full scale set to 4 Gauss...\r\n");
  }
  else if(!strcmp (argv[0], "8")) {
    compassSetFullScale(&LIS3MDLD1, LIS3MDL_FS_8GA);
    chprintf(chp, "LIS3MDL compass full scale set to 8 Gauss...\r\n");
  }
  else if(!strcmp (argv[0], "12")) {
    compassSetFullScale(&LIS3MDLD1, LIS3MDL_FS_12GA);
    chprintf(chp, "LIS3MDL compass full scale set to 12 Gauss...\r\n");
  }
  else if(!strcmp (argv[0], "16")) {
    compassSetFullScale(&LIS3MDLD1, LIS3MDL_FS_16GA);
    chprintf(chp, "LIS3MDL compass full scale set to 16 Gauss...\r\n");
  }
  else {
    chprintf(chp, "Usage: fullscale [4|8|12|16]\r\n");
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
    chThdSleepMilliseconds(250);
    palSetLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(250);
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


  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /* LIS3MDL Object Initialization.*/
  lis3mdlObjectInit(&LIS3MDLD1);

  /* Activates the LIS3MDL driver.*/
  lis3mdlStart(&LIS3MDLD1, &LIS3MDLcfg);

  /* Shell manager initialization.*/
  shellInit();

  while(TRUE) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);                    /* Waiting termination.        */
  }
  lis3mdlStop(&LIS3MDLD1);
  return 0;
}
