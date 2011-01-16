/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"

#define ADC_GRP1_NUM_CHANNELS   8
#define ADC_GRP1_BUF_DEPTH      16

/*
 * ADC configuration, empty for STM32, nothing to configure.
 */
static const ADCConfig adccfg = {};

static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

/*
 * ADC streaming callback.
 */
size_t nx = 0, ny = 0;
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  if (samples == buffer) {
    nx += n;
  }
  else {
    ny += n;
  }
}

/*
 * ADC conversion group.
 * Mode:        Streaming, continuous, 16 samples of 8 channels, SW triggered.
 * Channels:    IN10, IN11, IN10, IN11, IN10, IN11, Sensor, VRef.
 */
static const ADCConversionGroup adcgrpcfg = {
  TRUE,
  ADC_GRP1_NUM_CHANNELS,
  adccallback,
  0,
  ADC_CR2_TSVREFE,
  0,
  0,
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  ADC_SQR2_SQ7_N(ADC_CHANNEL_SENSOR) | ADC_SQR2_SQ6_N(ADC_CHANNEL_VREFINT),
  ADC_SQR3_SQ5_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ4_N(ADC_CHANNEL_IN10) |
  ADC_SQR3_SQ3_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10) |
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN11)   | ADC_SQR3_SQ0_N(ADC_CHANNEL_IN10)
};

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
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
   * Setting up analog inputs used by the demo.
   */
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1),
                  PAL_MODE_INPUT_ANALOG);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Starts an ADC continuous conversion.
   */
  adcStart(&ADCD1, &adccfg);
  adcStartConversion(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      adcStopConversion(&ADCD1);
    chThdSleepMilliseconds(500);
  }
  return 0;
}
