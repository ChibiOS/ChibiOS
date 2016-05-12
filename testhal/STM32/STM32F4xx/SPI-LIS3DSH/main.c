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
#include "lis3dsh.h"

/* Enable use of special ANSI escape sequences */
#define CHPRINTF_USE_ANSI_CODE         TRUE

static BaseSequentialStream * chp = (BaseSequentialStream*) &SDU1;

/* LIS3DSH Driver: This object represent an LIS3DSH instance */
static LIS3DSHDriver LIS3DSHD1;

static int32_t rawdata[LIS3DSH_NUMBER_OF_AXES];
static float cookeddata[LIS3DSH_NUMBER_OF_AXES];
static int8_t temperature;

static char axesID[LIS3DSH_NUMBER_OF_AXES] = {'X', 'Y', 'Z'};
static uint32_t i;

static const SPIConfig spicfg = {
  NULL,
  GPIOE,                                     /* port of LIS3DSH CS */
  GPIOE_CS_SPI,                              /* pin of LIS3DSH CS */
  SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA,/*   CR1 register*/                                        /* CR2 register */
};

static LIS3DSHConfig lis3dshcfg = {
  &SPID1,                                    /* Pointer to SPI Driver */
  &spicfg,                                   /* Pointer to SPI Configuration */
  LIS3DSH_FS_2G,                             /* Full scale value */
  LIS3DSH_ODR_100HZ,                         /* Output data rate */
  LIS3DSH_BW_200HZ,                          /* AA filter BW 200 Hz */
  LIS3DSH_AE_XYZ,                            /* Enabled axes */
  LIS3DSH_BDU_BLOCKED,                       /* Block data update blocked */
  LIS3DSH_ACC_UNIT_G                         /* Measurement unit for cooked */
};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(LINE_LED6);
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
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
   * LIS3DSH Object Initialization
   */
  lis3dshObjectInit(&LIS3DSHD1);

  /*
   * Activates the LIS3DSH driver.
   */
  lis3dshStart(&LIS3DSHD1, &lis3dshcfg);

  while (TRUE) {
    palToggleLine(LINE_LED3);
    accelerometerReadRaw(&LIS3DSHD1, rawdata);
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      chprintf(chp, "RAW-%c:%d\r\n", axesID[i], rawdata[i]);

    accelerometerReadCooked(&LIS3DSHD1, cookeddata);
    for(i = 0; i < LIS3DSH_NUMBER_OF_AXES; i++)
      chprintf(chp, "COOKED-%c:%.3f g\r\n", axesID[i], cookeddata[i]);

    accelerometerGetTemp(&LIS3DSHD1, &temperature);
    chprintf(chp, "TEMP:%d \r\n", temperature);

    chThdSleepMilliseconds(150);
#if CHPRINTF_USE_ANSI_CODE
    chprintf(chp, "\033[2J\033[1;1H");
#endif
  }
  lis3dshStop(&LIS3DSHD1);
}
