/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#include "portab.h"

#define ADC_GRP1_BUF_DEPTH                  1U
#define ADC_GRP2_BUF_DEPTH                  16U
#define ADC_STREAM_TIMEOUT_MS               2000U

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];

static volatile unsigned adc_cb_count;
static volatile unsigned adc_half_count;
static volatile unsigned adc_full_count;
static volatile unsigned adc_complete_count;
static volatile unsigned adc_error_count;
static volatile adceventflags_t adc_seen_events;
static volatile adcerror_t adc_seen_errors;

static void test_fail(void) {

  while (true) {
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(100);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition) {

  if (!condition) {
    test_fail();
  }
}

static void reset_adc_counters(void) {

  adc_cb_count = 0U;
  adc_half_count = 0U;
  adc_full_count = 0U;
  adc_complete_count = 0U;
  adc_error_count = 0U;
  adc_seen_events = 0U;
  adc_seen_errors = 0U;
}

static void adc_test_cb(void *ip) {
  hal_adc_driver_c *adcp = (hal_adc_driver_c *)ip;
  adceventflags_t events;
  adcerror_t errors;

  adc_cb_count++;
  events = adcGetAndClearEventsX(adcp, (adceventflags_t)-1);
  errors = adcGetAndClearErrorsX(adcp, (adcerror_t)-1);
  adc_seen_events |= events;
  adc_seen_errors |= errors;

  if ((events & ADC_EVENT_HALF) != 0U) {
    adc_half_count++;
  }
  if ((events & ADC_EVENT_FULL) != 0U) {
    adc_full_count++;
  }
  if ((events & ADC_EVENT_COMPLETE) != 0U) {
    adc_complete_count++;
  }
  if (errors != 0U) {
    adc_error_count++;
  }
}

static void wait_button_release(void) {

  while (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
    chThdSleepMilliseconds(50);
  }
}

static void adc_linear_test(void) {
  msg_t msg;

  reset_adc_counters();
  msg = adcConvert(&ADCD1, &portab_adcgrpcfg1, samples1, ADC_GRP1_BUF_DEPTH);
  test_assert(msg == HAL_RET_SUCCESS);
  test_assert(adc_cb_count > 0U);
  test_assert(adc_complete_count == 1U);
  test_assert(adc_half_count == 0U);
  test_assert(adc_full_count == 0U);
  test_assert(adc_error_count == 0U);
  test_assert(adc_seen_errors == 0U);
  test_assert((adc_seen_events & ADC_EVENT_COMPLETE) != 0U);
}

static void adc_stream_test(void) {
  msg_t msg;
  unsigned elapsed;

  reset_adc_counters();
  msg = adcStartConversion(&ADCD1, &portab_adcgrpcfg2,
                           samples2, ADC_GRP2_BUF_DEPTH);
  test_assert(msg == HAL_RET_SUCCESS);

  elapsed = 0U;
  while (((adc_half_count == 0U) || (adc_full_count == 0U)) &&
         (elapsed < ADC_STREAM_TIMEOUT_MS)) {
    chThdSleepMilliseconds(10);
    elapsed += 10U;
  }

  adcStopConversion(&ADCD1);

  test_assert(elapsed < ADC_STREAM_TIMEOUT_MS);
  test_assert(adc_cb_count >= 2U);
  test_assert(adc_half_count > 0U);
  test_assert(adc_full_count > 0U);
  test_assert(adc_complete_count == 0U);
  test_assert(adc_error_count == 0U);
  test_assert(adc_seen_errors == 0U);
  test_assert((adc_seen_events & ADC_EVENT_HALF) != 0U);
  test_assert((adc_seen_events & ADC_EVENT_FULL) != 0U);
}

static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

int main(void) {

  halInit();
  chSysInit();

  portab_setup();
  wait_button_release();

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  test_assert(adcStart(&ADCD1, &portab_adccfg1) == HAL_RET_SUCCESS);
  drvSetCallbackX(&ADCD1, adc_test_cb);

  adc_linear_test();
  adc_stream_test();

  while (true) {
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(50);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(950);
  }
}
