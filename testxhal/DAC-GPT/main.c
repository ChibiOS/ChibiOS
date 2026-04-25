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

#define DAC_BUFFER_SIZE                     360U
#define DAC_STREAM_TIMEOUT_MS               2000U

#define TEST_STAGE_INIT                     0x00000000U
#define TEST_STAGE_STARTUP                  0x00000001U
#define TEST_STAGE_PUT                      0x00000100U
#define TEST_STAGE_STREAM                   0x00000200U
#define TEST_STAGE_RESTART                  0x00000300U
#define TEST_STAGE_DONE                     0x0000FFFFU
#define TEST_RESULT_PENDING                 0x00000000U
#define TEST_RESULT_PASSED                  0x13579BDFU
#define TEST_RESULT_FAILED                  0x2468ACE0U

static const dacsample_t dac_buffer[DAC_BUFFER_SIZE] = {
  2047, 2082, 2118, 2154, 2189, 2225, 2260, 2296, 2331, 2367, 2402, 2437,
  2472, 2507, 2542, 2576, 2611, 2645, 2679, 2713, 2747, 2780, 2813, 2846,
  2879, 2912, 2944, 2976, 3008, 3039, 3070, 3101, 3131, 3161, 3191, 3221,
  3250, 3278, 3307, 3335, 3362, 3389, 3416, 3443, 3468, 3494, 3519, 3544,
  3568, 3591, 3615, 3637, 3660, 3681, 3703, 3723, 3744, 3763, 3782, 3801,
  3819, 3837, 3854, 3870, 3886, 3902, 3917, 3931, 3944, 3958, 3970, 3982,
  3993, 4004, 4014, 4024, 4033, 4041, 4049, 4056, 4062, 4068, 4074, 4078,
  4082, 4086, 4089, 4091, 4092, 4093, 4094, 4093, 4092, 4091, 4089, 4086,
  4082, 4078, 4074, 4068, 4062, 4056, 4049, 4041, 4033, 4024, 4014, 4004,
  3993, 3982, 3970, 3958, 3944, 3931, 3917, 3902, 3886, 3870, 3854, 3837,
  3819, 3801, 3782, 3763, 3744, 3723, 3703, 3681, 3660, 3637, 3615, 3591,
  3568, 3544, 3519, 3494, 3468, 3443, 3416, 3389, 3362, 3335, 3307, 3278,
  3250, 3221, 3191, 3161, 3131, 3101, 3070, 3039, 3008, 2976, 2944, 2912,
  2879, 2846, 2813, 2780, 2747, 2713, 2679, 2645, 2611, 2576, 2542, 2507,
  2472, 2437, 2402, 2367, 2331, 2296, 2260, 2225, 2189, 2154, 2118, 2082,
  2047, 2012, 1976, 1940, 1905, 1869, 1834, 1798, 1763, 1727, 1692, 1657,
  1622, 1587, 1552, 1518, 1483, 1449, 1415, 1381, 1347, 1314, 1281, 1248,
  1215, 1182, 1150, 1118, 1086, 1055, 1024,  993,  963,  933,  903,  873,
   844,  816,  787,  759,  732,  705,  678,  651,  626,  600,  575,  550,
   526,  503,  479,  457,  434,  413,  391,  371,  350,  331,  312,  293,
   275,  257,  240,  224,  208,  192,  177,  163,  150,  136,  124,  112,
   101,   90,   80,   70,   61,   53,   45,   38,   32,   26,   20,   16,
    12,    8,    5,    3,    2,    1,    0,    1,    2,    3,    5,    8,
    12,   16,   20,   26,   32,   38,   45,   53,   61,   70,   80,   90,
   101,  112,  124,  136,  150,  163,  177,  192,  208,  224,  240,  257,
   275,  293,  312,  331,  350,  371,  391,  413,  434,  457,  479,  503,
   526,  550,  575,  600,  626,  651,  678,  705,  732,  759,  787,  816,
   844,  873,  903,  933,  963,  993, 1024, 1055, 1086, 1118, 1150, 1182,
  1215, 1248, 1281, 1314, 1347, 1381, 1415, 1449, 1483, 1518, 1552, 1587,
  1622, 1657, 1692, 1727, 1763, 1798, 1834, 1869, 1905, 1940, 1976, 2012
};

volatile uint32_t dac_gpt_test_stage = TEST_STAGE_INIT;
volatile uint32_t dac_gpt_test_failure = 0U;
volatile uint32_t dac_gpt_test_result = TEST_RESULT_PENDING;
volatile uint32_t dac_gpt_test_heartbeat = 0U;

static volatile unsigned dac_cb_count;
static volatile unsigned dac_half_count;
static volatile unsigned dac_full_count;
static volatile unsigned dac_error_count;
static volatile daceventflags_t dac_seen_events;
static volatile dacerror_t dac_seen_errors;
static volatile driver_state_t dac_last_state;
static volatile uint32_t dac_cb_anomalies;

static bool streaming;

static void test_fail(void) {

  while (true) {
    dac_gpt_test_heartbeat++;
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(100);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition, uint32_t code) {

  if (!condition) {
    dac_gpt_test_failure = code;
    dac_gpt_test_result = TEST_RESULT_FAILED;
    test_fail();
  }
}

static void reset_dac_counters(void) {

  dac_cb_count = 0U;
  dac_half_count = 0U;
  dac_full_count = 0U;
  dac_error_count = 0U;
  dac_seen_events = 0U;
  dac_seen_errors = 0U;
  dac_last_state = HAL_DRV_STATE_READY;
  dac_cb_anomalies = 0U;
}

static void dac_test_cb(void *ip) {
  DACDriver *dacp = (DACDriver *)ip;
  driver_state_t state;
  daceventflags_t events;
  dacerror_t errors;

  state = drvGetStateX(dacp);
  events = dacGetAndClearEventsX(dacp, (daceventflags_t)-1);
  errors = dacGetAndClearErrorsX(dacp, (dacerror_t)-1);

  dac_cb_count++;
  dac_last_state = state;
  dac_seen_events |= events;
  dac_seen_errors |= errors;

  if (state == HAL_DRV_STATE_HALF) {
    dac_half_count++;
    if ((events & DAC_EVENT_HALF) == 0U) {
      dac_cb_anomalies |= 0x01U;
    }
  }
  else if (state == HAL_DRV_STATE_FULL) {
    dac_full_count++;
    if ((events & DAC_EVENT_FULL) == 0U) {
      dac_cb_anomalies |= 0x02U;
    }
  }
  else if (state == HAL_DRV_STATE_ERROR) {
    dac_error_count++;
    if (errors == 0U) {
      dac_cb_anomalies |= 0x04U;
    }
  }
  else {
    dac_cb_anomalies |= 0x08U;
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
  while (((dac_half_count == 0U) || (dac_full_count == 0U)) &&
         (dac_error_count == 0U) &&
         (elapsed < DAC_STREAM_TIMEOUT_MS)) {
    chThdSleepMilliseconds(10);
    elapsed += 10U;
  }

  return elapsed;
}

static void stop_stream(void) {

  if (streaming) {
    gptStopTimer(&PORTAB_GPT1);
    dacStopConversion(&PORTAB_DAC1);
    streaming = false;
  }
}

static void start_stream(void) {

  test_assert(dacStartConversion(&PORTAB_DAC1, &portab_dacgrpcfg1,
                                 (dacsample_t *)dac_buffer,
                                 DAC_BUFFER_SIZE) == HAL_RET_SUCCESS,
              dac_gpt_test_stage | 0x20U);
  gptStartContinuous(&PORTAB_GPT1, PORTAB_GPT1_PERIOD);
  streaming = true;
}

static void dac_put_test(void) {

  dac_gpt_test_stage = TEST_STAGE_PUT;
  test_assert(dacPutChannelX(&PORTAB_DAC1, 0U, 2047U) == HAL_RET_SUCCESS,
              TEST_STAGE_PUT | 0x01U);
  test_assert(dacPutChannelX(&PORTAB_DAC1, 0U, 1024U) == HAL_RET_SUCCESS,
              TEST_STAGE_PUT | 0x02U);
  test_assert(dacPutChannelX(&PORTAB_DAC1, 0U, 3072U) == HAL_RET_SUCCESS,
              TEST_STAGE_PUT | 0x03U);
}

static void dac_stream_test(uint32_t stage_base) {
  unsigned elapsed;

  dac_gpt_test_stage = stage_base;
  reset_dac_counters();
  start_stream();
  elapsed = wait_for_stream_events();
  stop_stream();

  test_assert(elapsed < DAC_STREAM_TIMEOUT_MS, stage_base | 0x01U);
  test_assert(dac_cb_count >= 2U, stage_base | 0x02U);
  test_assert(dac_half_count > 0U, stage_base | 0x03U);
  test_assert(dac_full_count > 0U, stage_base | 0x04U);
  test_assert(dac_error_count == 0U, stage_base | 0x05U);
  test_assert(dac_seen_errors == 0U, stage_base | 0x06U);
  test_assert((dac_seen_events & DAC_EVENT_HALF) != 0U, stage_base | 0x07U);
  test_assert((dac_seen_events & DAC_EVENT_FULL) != 0U, stage_base | 0x08U);
  test_assert(dac_cb_anomalies == 0U, stage_base | 0x09U);
}

int main(void) {

  halInit();
  chSysInit();
  dac_gpt_test_stage = TEST_STAGE_STARTUP;
  dac_gpt_test_failure = 0U;
  dac_gpt_test_result = TEST_RESULT_PENDING;
  dac_gpt_test_heartbeat = 0U;
  streaming = false;

  portab_setup();
  wait_button_release();

  test_assert(drvStart(&PORTAB_DAC1, &portab_daccfg1) == HAL_RET_SUCCESS,
              TEST_STAGE_STARTUP | 0x01U);
  test_assert(drvStart(&PORTAB_GPT1, &portab_gptcfg1) == HAL_RET_SUCCESS,
              TEST_STAGE_STARTUP | 0x02U);
  drvSetCallbackX(&PORTAB_DAC1, dac_test_cb);
  drvSetCallbackX(&PORTAB_GPT1, NULL);

  dac_put_test();
  dac_stream_test(TEST_STAGE_STREAM);
  dac_stream_test(TEST_STAGE_RESTART);

  reset_dac_counters();
  dac_gpt_test_stage = TEST_STAGE_DONE;
  dac_gpt_test_result = TEST_RESULT_PASSED;
  start_stream();

  while (true) {
    dac_gpt_test_heartbeat++;

    if (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
      wait_button_release();
      if (streaming) {
        stop_stream();
      }
      else {
        reset_dac_counters();
        start_stream();
      }
    }

    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(streaming ? 500U : 125U);
  }
}
