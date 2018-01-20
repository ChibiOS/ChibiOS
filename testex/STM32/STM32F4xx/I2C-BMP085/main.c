/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

/*==========================================================================*/
/* Include files.                                                           */
/*==========================================================================*/

#include "ch.h"
#include "hal.h"

#include "string.h"
#include "shell.h"
#include "chprintf.h"

#include "bmp085.h"

/*==========================================================================*/
/* Variables and Driver configuration for the demo.                         */
/*==========================================================================*/

/* BMP085 Driver: This object represent an BMP085 instance.                 */
static BMP085Driver BMP085D1;

/* Raw data buffer for the BMP sensor.                                      */
static int32_t  rawdata[BMP085_BARO_NUMBER_OF_AXES +
                        BMP085_THERMO_NUMBER_OF_AXES];

/* Cooked data buffer for the BMP sensor.                                   */
static float    cookeddata[BMP085_BARO_NUMBER_OF_AXES +
                            BMP085_THERMO_NUMBER_OF_AXES];

/**
 * @brief I2C configuration structure.
 */
static const I2CConfig i2cConfig = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

/**
 * @brief BMP085 configuration structure.
 */
static const BMP085Config bmp085Config = {
  &I2CD1,
  &i2cConfig,
  NULL,
  NULL,
  NULL,
#if BMP085_USE_ADVANCED
  BMP085_BARO_CT_LOW,
  BMP085_BARO_MODE_LOW,
  BMP085_BARO_OSS_0,
  BMP085_THERMO_CT_LOW,
#endif
};

/*==========================================================================*/
/* Command line related.                                                    */
/*==========================================================================*/

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE      TRUE
#define SHELL_WA_SIZE               THD_WORKING_AREA_SIZE(2048)

static void cmd_read(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  if (argc != 2) {
    chprintf(chp, "Usage: read [baro|thermo|both] [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "baro")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        barometerReadRaw(&BMP085D1, rawdata);
        chprintf(chp, "BMP085 Barometer raw data...\r\n");
        chprintf(chp, "Raw pressure: %d\r\n", *rawdata);
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        barometerReadCooked(&BMP085D1, cookeddata);
        chprintf(chp, "BMP085 Barometer cooked data...\r\n");
        chprintf(chp, "Cooked pressure: %.2f %%\r\n", *cookeddata);
      }
      else {
        chprintf(chp, "Usage: read [baro|thermo|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "thermo")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        thermometerReadRaw(&BMP085D1, rawdata);
        chprintf(chp, "BMP085 Thermometer raw data...\r\n");
        chprintf(chp, "Raw temperature: %d\r\n", *rawdata);
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        thermometerReadCooked(&BMP085D1, cookeddata);
        chprintf(chp, "BMP085 Thermometer cooked data...\r\n");
        chprintf(chp, "Cooked temperature: %.2f °C\r\n", *cookeddata);
      }
      else {
        chprintf(chp, "Usage: read [baro|thermo|both] [raw|cooked]\r\n");
        return;
      }
    }
    else if (!strcmp (argv[0], "both")) {
      if (!strcmp (argv[1], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadRaw(&BMP085D1, rawdata);
        chprintf(chp, "BMP085 Barometer raw data...\r\n");
        chprintf(chp, "Raw pressure: %d\r\n", rawdata[0]);
        chprintf(chp, "BMP085 Thermometer raw data...\r\n");
        chprintf(chp, "Raw temperature: %d\r\n", rawdata[1]);
      }
      else if (!strcmp (argv[1], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
        chprintf(chp, "\033[2J\033[1;1H");
#endif
        sensorReadCooked(&BMP085D1, cookeddata);
        chprintf(chp, "BMP085 Barometer cooked data...\r\n");
        chprintf(chp, "Cooked pressure: %.2f %%\r\n", cookeddata[0]);
        chprintf(chp, "BMP085 Thermometer cooked data...\r\n");
        chprintf(chp, "Cooked temperature: %.2f °C\r\n", cookeddata[1]);
      }
      else {
        chprintf(chp, "Usage: read [baro|thermo|both] [raw|cooked]\r\n");
        return;
      }
    }
    else {
      chprintf(chp, "Usage: read [baro|thermo|both] [raw|cooked]\r\n");
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

/*==========================================================================*/
/* Main code.                                                               */
/*==========================================================================*/

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

  /* Configure the I2C Driver and I2C Pins.   */
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
   * BMP085 Object Initialization.
   */
  bmp085ObjectInit(&BMP085D1);

  /*
   * Activates the BMP085 driver.
   */
  bmp085Start(&BMP085D1, &bmp085Config);

  /*
   * Shell manager initialization.
   */
  shellInit();

  while(TRUE) {

    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);                  /* Waiting termination.            */
    chThdSleepMilliseconds(1000);
  }
  bmp085Stop(&BMP085D1);
}
