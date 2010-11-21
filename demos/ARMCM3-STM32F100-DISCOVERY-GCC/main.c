/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
#include "test.h"

static void pwmpcb(PWMDriver *pwmp);
static void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n);

#define ADC_GRP1_NUM_CHANNELS   2
#define ADC_GRP1_BUF_DEPTH      4

/*
 * ADC samples buffer.
 */
static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH] =
{
  2048, 0,
  2048, 0,
  2048, 0,
  2048, 0
};

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 4 samples of 2 channels, SW triggered.
 * Channels:    IN10, Sensor.
 */
static const ADCConversionGroup adcgrpcfg = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  adccb,
  0,
  ADC_CR2_EXTSEL_SWSTART | ADC_CR2_TSVREFE,
  0,
  0,
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR) | ADC_SQR3_SQ0_N(ADC_CHANNEL_IN10)
};

/*
 * ADC configuration structure, empty for STM32, there is nothing to configure.
 */
static const ADCConfig adccfg = {
};

/*
 * PWM configuration structure.
 */
static PWMConfig pwmcfg = {
  pwmpcb,
  {
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  PWM_COMPUTE_PSC(STM32_TIMCLK1, 10000),    /* 10KHz PWM clock frequency.   */
  PWM_COMPUTE_ARR(10000, 1000000000),       /* PWM period 1S (in nS).       */
  0
};

/*
 * PWM cyclic callback. PWM channels are reprogrammed using a duty cycle
 * calculated as average of the last sampling operations.
 */
static void pwmpcb(PWMDriver *pwmp) {
  adcsample_t avg_ch1, avg_ch2;

  /* Calculates the average values from the previous ADC sampling
     operation.*/
  avg_ch1 = (samples[0] + samples[2] + samples[4] + samples[6]) / 4;
  avg_ch2 = (samples[1] + samples[3] + samples[5] + samples[7]) / 4;
  chSysLockFromIsr();
  pwmEnableChannelI(pwmp, 2, PWM_FRACTION_TO_WIDTH(pwmp, 4096, avg_ch1));
  pwmEnableChannelI(pwmp, 3, PWM_FRACTION_TO_WIDTH(pwmp, 4096, avg_ch2));

  /* Starts an asynchronous ADC conversion operation, the conversion
     will be executed in parallel to the current PWM cycle and will
     terminate before the next PWM cycle.*/
//  adcStartConversionI(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);
  chSysUnlockFromIsr();
}

/*
 * ADC end conversion callback.
 */
void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void)adcp; (void) buffer; (void) n;
}

/*
 * Red and blue LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(250);
  }
  return 0;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {

  (void)argc;
  (void)argv;

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  /*
   * Initializes the ADC driver 1.
   */
  adcStart(&ADCD1, &adccfg);

  /*
   * Initializes the PWM driver 1, re-routes the TIM3 outputs, programs the
   * pins as alternate functions and finally enables channels with zero
   * initial duty cycle.
   * Note, the AFIO access routes the TIM3 output pins on the PC6...PC9
   * where the LEDs are connected.
   */
  pwmStart(&PWMD3, &pwmcfg);
  AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_0 | AFIO_MAPR_TIM3_REMAP_1;
  palSetGroupMode(GPIOC, PAL_PORT_BIT(GPIOC_LED3) | PAL_PORT_BIT(GPIOC_LED4),
                  PAL_MODE_STM32_ALTERNATE_PUSHPULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (palReadPad(IOPORT1, GPIOA_BUTTON))
      TestThread(&SD1);
    chThdSleepMilliseconds(500);
  }
  return 0;
}
