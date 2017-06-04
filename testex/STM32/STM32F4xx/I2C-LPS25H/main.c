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

#include "lps25h.h"

/*===========================================================================*/
/* LPS25H related.                                                           */
/*===========================================================================*/

/* LPS25H Driver: This object represent an LPS25H instance */
static  LPS25HDriver LPS25HD1;

static int32_t rawdata;
static float cookeddata;

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const LPS25HConfig lps25hcfg = {
  &I2CD1,
  &i2ccfg,
  NULL,                        /* Use default sensitivity.*/
  NULL,                        /* Use default bias.*/
  LPS25H_SAD_VCC,              /* SA0 connected to VCC */
  LPS25H_ODR_7HZ,              /* Output data rate 7 Hz.*/
#if LPS25H_USE_ADVANCED || defined(__DOXYGEN__)
  LPS25H_BDU_CONTINUOUS,
  LPS25H_AVGP_512,
  LPS25H_AVGT_512
#endif
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE      TRUE
#define SHELL_WA_SIZE               THD_WORKING_AREA_SIZE(2048)

static void cmd_read(BaseSequentialStream *chp, int argc, char *argv[]) {

  if (argc != 1) {
    chprintf(chp, "Usage: read [raw|cooked]\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, 150) == Q_TIMEOUT) {
    if (!strcmp (argv[0], "raw")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      barometerReadRaw(&LPS25HD1, &rawdata);
      chprintf(chp, "LPS25H Barometer raw data...\r\n");
      chprintf(chp, "RAW: %d\r\n", rawdata);
    }
    else if (!strcmp (argv[0], "cooked")) {
#if CHPRINTF_USE_ANSI_CODE
      chprintf(chp, "\033[2J\033[1;1H");
#endif
      barometerReadCooked(&LPS25HD1, &cookeddata);
      chprintf(chp, "LPS25H Barometer cooked data...\r\n");
      chprintf(chp, "COOCKED: %.4f hPa\r\n", cookeddata);
    }
    else {
      chprintf(chp, "Usage: read [raw|cooked]\r\n");
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
   * LPS25H Object Initialization
   */
  lps25hObjectInit(&LPS25HD1);

  /*
   * Activates the LPS25H driver.
   */
  lps25hStart(&LPS25HD1, &lps25hcfg);

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
  lps25hStop(&LPS25HD1);
}
