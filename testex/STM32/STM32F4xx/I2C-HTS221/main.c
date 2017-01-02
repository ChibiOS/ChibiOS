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

#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "hts221.h"

/*===========================================================================*/
/* HTS221 related.                                                           */
/*===========================================================================*/

/* HTS221 Driver: This object represent an HTS221 instance */
static  HTS221Driver HTS221D1;

static int32_t rawdata[HTS221_HYGRO_NUMBER_OF_AXES + 
                       HTS221_THERMO_NUMBER_OF_AXES];
static float cookeddata[HTS221_HYGRO_NUMBER_OF_AXES + 
                        HTS221_THERMO_NUMBER_OF_AXES];

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const HTS221Config hts221cfg = {
  &I2CD1,
  &i2ccfg,
  NULL,                        /* Use default sensitivity.*/
  NULL,                        /* Use default bias.*/
  HTS221_ODR_7HZ,              /* Output data rate 7 Hz.*/
#if HTS221_USE_ADVANCED || defined(__DOXYGEN__)
  HTS221_BDU_CONTINUOUS,
  HTS221_AVGH_256,
  HTS221_AVGT_256
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
    chprintf(chp, "Usage: read [hygro|thermo|both] [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "hygro")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        hygrometerReadRaw(&HTS221D1, rawdata);
        chprintf(chp, "HTS221 Hygrometer raw data...\r\n");
        chprintf(chp, "Raw humidity: %d\r\n", *rawdata);
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        hygrometerReadCooked(&HTS221D1, cookeddata);
        chprintf(chp, "HTS221 Hygrometer cooked data...\r\n");
        chprintf(chp, "Cooked humidity: %.2f %%\r\n", *cookeddata);
      }
      else {
        chprintf(chp, "Usage: read [hygro|thermo|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "thermo")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        thermometerReadRaw(&HTS221D1, rawdata);
        chprintf(chp, "HTS221 Thermometer raw data...\r\n");
        chprintf(chp, "Raw temperature: %d\r\n", *rawdata);
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        thermometerReadCooked(&HTS221D1, cookeddata);
        chprintf(chp, "HTS221 Thermometer cooked data...\r\n");
        chprintf(chp, "Cooked temperature: %.2f °C\r\n", *cookeddata);
      }
      else {
        chprintf(chp, "Usage: read [hygro|thermo|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "both")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadRaw(&HTS221D1, rawdata);
        chprintf(chp, "HTS221 Hygrometer raw data...\r\n");
        chprintf(chp, "Raw humidity: %d\r\n", rawdata[0]);
        chprintf(chp, "HTS221 Thermometer raw data...\r\n");
        chprintf(chp, "Raw temperature: %d\r\n", rawdata[1]);
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadCooked(&HTS221D1, cookeddata);
        chprintf(chp, "HTS221 Hygrometer cooked data...\r\n");
        chprintf(chp, "Cooked humidity: %.2f %%\r\n", cookeddata[0]);
        chprintf(chp, "HTS221 Thermometer cooked data...\r\n");
        chprintf(chp, "Cooked temperature: %.2f °C\r\n", cookeddata[1]);
      }
      else {
        chprintf(chp, "Usage: read [hygro|thermo|both] [raw|cooked]\r\n");
        return;
      }
    }
    else {
      chprintf(chp, "Usage: read [hygro|thermo|both] [raw|cooked]\r\n");
      return;
    }
  }
  chprintf(chp, "Stopped\r\n");
}

static const ShellCommand commands[] = {
  {"read", cmd_read},
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
    palClearPad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOA, GPIOA_LED_GREEN);
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
                 PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * HTS221 Object Initialization
   */
  hts221ObjectInit(&HTS221D1);

  /*
   * Activates the HTS221 driver.
   */
  hts221Start(&HTS221D1, &hts221cfg);

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
  hts221Stop(&HTS221D1);
}
