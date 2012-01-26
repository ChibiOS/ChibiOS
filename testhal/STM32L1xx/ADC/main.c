/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

#define ADC_GRP1_NUM_CHANNELS   1
#define ADC_GRP1_BUF_DEPTH      8

#define ADC_GRP2_NUM_CHANNELS   8
#define ADC_GRP2_BUF_DEPTH      16

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];

/*
 * ADC streaming callback.
 */
size_t nx = 0, ny = 0;
static void adccallback(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp;
  if (samples2 == buffer) {
    nx += n;
  }
  else {
    ny += n;
  }
}

static void adcerrorcallback(ADCDriver *adcp, adcerror_t err) {

  (void)adcp;
  (void)err;
}

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 8 samples of 1 channel, SW triggered.
 * Channels:    IN10.
 */
static const ADCConversionGroup adcgrpcfg1 = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  NULL,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  0,                        /* SMPR1 */
  ADC_SMPR2_SMP_AN10(ADC_SAMPLE_4),
  0,                        /* SMPR3 */
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0, 0, 0,                  /* SQR2, SQR3, SQR4 */
  ADC_SQR5_SQ1_N(ADC_CHANNEL_IN10)
};

/*
 * ADC conversion group.
 * Mode:        Continuous, 16 samples of 8 channels, SW triggered.
 * Channels:    IN10, IN11, IN10, IN11, IN10, IN11, Sensor, VRef.
 */
static const ADCConversionGroup adcgrpcfg2 = {
  TRUE,
  ADC_GRP2_NUM_CHANNELS,
  adccallback,
  adcerrorcallback,
  0,                        /* CR1 */
  ADC_CR2_SWSTART,          /* CR2 */
  0,                        /* SMPR1 */
  ADC_SMPR2_SMP_AN11(ADC_SAMPLE_48) | ADC_SMPR2_SMP_AN10(ADC_SAMPLE_48) |
  ADC_SMPR2_SMP_SENSOR(ADC_SAMPLE_192) | ADC_SMPR2_SMP_VREF(ADC_SAMPLE_192),
  0,                        /* SMPR3 */
  ADC_SQR1_NUM_CH(ADC_GRP2_NUM_CHANNELS),
  0, 0,                     /* SQR2, SQR3 */
  ADC_SQR4_SQ8_N(ADC_CHANNEL_SENSOR) | ADC_SQR4_SQ7_N(ADC_CHANNEL_VREFINT),
  ADC_SQR5_SQ6_N(ADC_CHANNEL_IN11)   | ADC_SQR5_SQ5_N(ADC_CHANNEL_IN10) |
  ADC_SQR5_SQ4_N(ADC_CHANNEL_IN11)   | ADC_SQR5_SQ3_N(ADC_CHANNEL_IN10) |
  ADC_SQR5_SQ2_N(ADC_CHANNEL_IN11)   | ADC_SQR5_SQ1_N(ADC_CHANNEL_IN10)
};

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOB, GPIOB_LED4);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOB, GPIOB_LED4);
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

  /*
   * Setting up analog inputs used by the demo.
   */
  palSetGroupMode(GPIOC, PAL_PORT_BIT(0) | PAL_PORT_BIT(1),
                  0, PAL_MODE_INPUT_ANALOG);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Activates the ADC1 driver and the thermal sensor.
   */
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();

  /*
   * Linear conversion.
   */
  adcConvert(&ADCD1, &adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
  chThdSleepMilliseconds(1000);

  /*
   * Starts an ADC continuous conversion.
   */
  adcStartConversion(&ADCD1, &adcgrpcfg2, samples2, ADC_GRP2_BUF_DEPTH);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON)) {
      adcStopConversion(&ADCD1);
      adcSTM32DisableTSVREFE();
    }
    chThdSleepMilliseconds(500);
  }
}
