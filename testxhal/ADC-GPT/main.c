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
#define ADC_GRP3_BUF_DEPTH                  16U
#define ADC_STREAM_TIMEOUT_MS               2000U

#define TEST_STAGE_INIT                     0x00000000U
#define TEST_STAGE_STARTUP                  0x00000001U
#define TEST_STAGE_LINEAR                   0x00000100U
#define TEST_STAGE_STREAM                   0x00000200U
#define TEST_STAGE_HW_TRIGGER               0x00000300U
#define TEST_STAGE_DONE                     0x0000FFFFU
#define TEST_RESULT_PENDING                 0x00000000U
#define TEST_RESULT_PASSED                  0x13579BDFU
#define TEST_RESULT_FAILED                  0x2468ACE0U

static adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
static adcsample_t samples2[ADC_GRP2_NUM_CHANNELS * ADC_GRP2_BUF_DEPTH];
static adcsample_t samples3[ADC_GRP3_NUM_CHANNELS * ADC_GRP3_BUF_DEPTH];

volatile uint32_t adc_gpt_test_stage = TEST_STAGE_INIT;
volatile uint32_t adc_gpt_test_failure = 0U;
volatile uint32_t adc_gpt_test_result = TEST_RESULT_PENDING;
volatile uint32_t adc_gpt_test_heartbeat = 0U;

static volatile unsigned adc_cb_count;
static volatile unsigned adc_half_count;
static volatile unsigned adc_full_count;
static volatile unsigned adc_complete_count;
static volatile unsigned adc_error_count;
static volatile adceventflags_t adc_seen_events;
static volatile adcerror_t adc_seen_errors;

static void test_fail(void) {

  while (true) {
    adc_gpt_test_heartbeat++;
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(100);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition, uint32_t code) {

  if (!condition) {
    adc_gpt_test_failure = code;
    adc_gpt_test_result = TEST_RESULT_FAILED;
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

static unsigned wait_for_stream_events(void) {
  unsigned elapsed;

  elapsed = 0U;
  while (((adc_half_count == 0U) || (adc_full_count == 0U)) &&
         (elapsed < ADC_STREAM_TIMEOUT_MS)) {
    chThdSleepMilliseconds(10);
    elapsed += 10U;
  }

  return elapsed;
}

static void adc_linear_test(void) {
  msg_t msg;

  adc_gpt_test_stage = TEST_STAGE_LINEAR;
  reset_adc_counters();
  msg = adcConvert(&ADCD1, ADC_GRP1, samples1, ADC_GRP1_BUF_DEPTH);
  test_assert(msg == HAL_RET_SUCCESS, TEST_STAGE_LINEAR | 0x01U);
  test_assert(adc_cb_count > 0U, TEST_STAGE_LINEAR | 0x02U);
  test_assert(adc_complete_count == 1U, TEST_STAGE_LINEAR | 0x03U);
  test_assert(adc_half_count == 0U, TEST_STAGE_LINEAR | 0x04U);
  test_assert(adc_full_count == 0U, TEST_STAGE_LINEAR | 0x05U);
  test_assert(adc_error_count == 0U, TEST_STAGE_LINEAR | 0x06U);
  test_assert(adc_seen_errors == 0U, TEST_STAGE_LINEAR | 0x07U);
  test_assert((adc_seen_events & ADC_EVENT_COMPLETE) != 0U,
              TEST_STAGE_LINEAR | 0x08U);
}

static void adc_stream_test(void) {
  msg_t msg;
  unsigned elapsed;

  adc_gpt_test_stage = TEST_STAGE_STREAM;
  reset_adc_counters();
  msg = adcStartConversionCircular(&ADCD1, ADC_GRP2, samples2,
                                   ADC_GRP2_BUF_DEPTH);
  test_assert(msg == HAL_RET_SUCCESS, TEST_STAGE_STREAM | 0x01U);

  elapsed = wait_for_stream_events();

  adcStopConversion(&ADCD1);

  test_assert(elapsed < ADC_STREAM_TIMEOUT_MS, TEST_STAGE_STREAM | 0x02U);
  test_assert(adc_cb_count >= 2U, TEST_STAGE_STREAM | 0x03U);
  test_assert(adc_half_count > 0U, TEST_STAGE_STREAM | 0x04U);
  test_assert(adc_full_count > 0U, TEST_STAGE_STREAM | 0x05U);
  test_assert(adc_complete_count == 0U, TEST_STAGE_STREAM | 0x06U);
  test_assert(adc_error_count == 0U, TEST_STAGE_STREAM | 0x07U);
  test_assert(adc_seen_errors == 0U, TEST_STAGE_STREAM | 0x08U);
  test_assert((adc_seen_events & ADC_EVENT_HALF) != 0U,
              TEST_STAGE_STREAM | 0x09U);
  test_assert((adc_seen_events & ADC_EVENT_FULL) != 0U,
              TEST_STAGE_STREAM | 0x0AU);
}

static void adc_hw_trigger_test(void) {
  msg_t msg;
  unsigned elapsed;

  adc_gpt_test_stage = TEST_STAGE_HW_TRIGGER;
  reset_adc_counters();
  msg = adcStartConversionCircular(&ADCD1, ADC_GRP3, samples3,
                                   ADC_GRP3_BUF_DEPTH);
  test_assert(msg == HAL_RET_SUCCESS, TEST_STAGE_HW_TRIGGER | 0x01U);

  gptStartContinuous(&PORTAB_GPT1, PORTAB_GPT1_PERIOD);
  elapsed = wait_for_stream_events();
  gptStopTimer(&PORTAB_GPT1);
  adcStopConversion(&ADCD1);

  test_assert(elapsed < ADC_STREAM_TIMEOUT_MS, TEST_STAGE_HW_TRIGGER | 0x02U);
  test_assert(adc_cb_count >= 2U, TEST_STAGE_HW_TRIGGER | 0x03U);
  test_assert(adc_half_count > 0U, TEST_STAGE_HW_TRIGGER | 0x04U);
  test_assert(adc_full_count > 0U, TEST_STAGE_HW_TRIGGER | 0x05U);
  test_assert(adc_complete_count == 0U, TEST_STAGE_HW_TRIGGER | 0x06U);
  test_assert(adc_error_count == 0U, TEST_STAGE_HW_TRIGGER | 0x07U);
  test_assert(adc_seen_errors == 0U, TEST_STAGE_HW_TRIGGER | 0x08U);
  test_assert((adc_seen_events & ADC_EVENT_HALF) != 0U,
              TEST_STAGE_HW_TRIGGER | 0x09U);
  test_assert((adc_seen_events & ADC_EVENT_FULL) != 0U,
              TEST_STAGE_HW_TRIGGER | 0x0AU);
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
  adc_gpt_test_stage = TEST_STAGE_STARTUP;
  adc_gpt_test_failure = 0U;
  adc_gpt_test_result = TEST_RESULT_PENDING;
  adc_gpt_test_heartbeat = 0U;

  portab_setup();
  wait_button_release();

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  test_assert(drvStart(&PORTAB_ADC1, &portab_adccfg1) == HAL_RET_SUCCESS,
              TEST_STAGE_STARTUP | 0x01U);
  test_assert(drvStart(&PORTAB_GPT1, &portab_gptcfg1) == HAL_RET_SUCCESS,
              TEST_STAGE_STARTUP | 0x02U);
  drvSetCallbackX(&PORTAB_GPT1, NULL);
  drvSetCallbackX(&ADCD1, adc_test_cb);

  adc_linear_test();
  adc_stream_test();
  adc_hw_trigger_test();
  adc_gpt_test_stage = TEST_STAGE_DONE;
  adc_gpt_test_result = TEST_RESULT_PASSED;

  while (true) {
    adc_gpt_test_heartbeat++;
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(50);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(950);
  }
}
