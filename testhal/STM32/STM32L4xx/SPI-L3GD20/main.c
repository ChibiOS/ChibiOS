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
#include "l3gd20.h"

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE         TRUE

static BaseSequentialStream * chp = (BaseSequentialStream*) &SD2;

/* L3GD20 Driver: This object represent an L3GD20 instance */
static L3GD20Driver L3GD20D1;

static int32_t rawdata[L3GD20_NUMBER_OF_AXES];
static float cookeddata[L3GD20_NUMBER_OF_AXES];

static char axesID[L3GD20_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const SPIConfig spicfg = {
  NULL,
  GPIOD,                                     /* port of L3GD20 CS */
  GPIOD_GYRO_CS,                             /* pin of L3GD20 CS */
  SPI_CR1_BR | SPI_CR1_CPOL | SPI_CR1_CPHA,  /* CR1 register */
  0                                          /* CR2 register */
};

static L3GD20Config l3gd20cfg = {
  &SPID2,                                    /* Pointer to SPI Driver */
  &spicfg,                                   /* Pointer to SPI Configuration */
  L3GD20_FS_250DPS,                          /* Full scale value */
  L3GD20_ODR_760HZ_FC_100,                   /* Output data rate */
  L3GD20_AE_XYZ,                             /* Enabled axes */
  L3GD20_END_LITTLE,                         /* Endianness */
  L3GD20_BDU_BLOCKED                         /* Block data update */
};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(LINE_LED_GREEN);
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

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
   * L3GD20 Object Initialization
   */
  l3gd20ObjectInit(&L3GD20D1);

  /*
   * Activates the L3GD20 driver.
   */
  l3gd20Start(&L3GD20D1, &l3gd20cfg);

  while(!palReadLine(LINE_JOY_CENTER)){
    palToggleLine(LINE_LED_RED);
    gyroscopeReadRaw(&L3GD20D1, rawdata);
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\r\n", axesID[i], rawdata[i]);

    gyroscopeReadCooked(&L3GD20D1, cookeddata);
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%f\r\n", axesID[i], cookeddata[i]);

    chprintf(chp, "Press JOY to continue...\r\n");
    chThdSleepMilliseconds(150);
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  }

  palClearLine(LINE_LED_RED);
  chprintf(chp, "Calibrating Gyroscope...\r\n");
  chprintf(chp, "Keep it in the rest position while red LED is on\r\n");
  chThdSleepMilliseconds(3000);

  palSetLine(LINE_LED_RED);
  chThdSleepMilliseconds(1000);

  gyroscopeCalibrate(&L3GD20D1);
  palClearLine(LINE_LED_RED);
#if CHPRINTF_USE_ANSI_CODE
  chprintf(chp, "\033[2J\033[1;1H");
#endif

  while (TRUE) {
    palToggleLine(LINE_LED_RED);
    gyroscopeReadRaw(&L3GD20D1, rawdata);
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\r\n", axesID[i], rawdata[i]);

    gyroscopeReadCooked(&L3GD20D1, cookeddata);
    for(i = 0; i < L3GD20_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%f\r\n", axesID[i], cookeddata[i]);

    chThdSleepMilliseconds(150);
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  }
  l3gd20Stop(&L3GD20D1);
}
