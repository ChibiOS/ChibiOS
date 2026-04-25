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

#define TEST_STAGE_INIT                     0x00000000U
#define TEST_STAGE_START                    0x00000001U
#define TEST_STAGE_GENERATE_32              0x00000100U
#define TEST_STAGE_GENERATE_15              0x00000200U
#define TEST_STAGE_GENERATE_2               0x00000300U
#define TEST_STAGE_RESTART                  0x00000400U
#define TEST_STAGE_DONE                     0x0000FFFFU
#define TEST_RESULT_PENDING                 0x00000000U
#define TEST_RESULT_PASSED                  0x13579BDFU
#define TEST_RESULT_FAILED                  0x2468ACE0U

volatile uint32_t trng_test_stage = TEST_STAGE_INIT;
volatile uint32_t trng_test_failure = 0U;
volatile uint32_t trng_test_result = TEST_RESULT_PENDING;
volatile uint32_t trng_test_heartbeat = 0U;
volatile uint32_t trng_test_sample0 = 0U;
volatile uint32_t trng_test_sample1 = 0U;

static uint8_t random32[32];
static uint8_t random15[15];
static uint8_t random2[2];

static void test_fail(void) {

  while (true) {
    trng_test_heartbeat++;
    palWriteLine(PORTAB_LINE_LED1, PAL_HIGH);
    chThdSleepMilliseconds(100);
    palWriteLine(PORTAB_LINE_LED1, PAL_LOW);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition, uint32_t code) {

  if (!condition) {
    trng_test_failure = code;
    trng_test_result = TEST_RESULT_FAILED;
    test_fail();
  }
}

static bool buffer_has_activity(const uint8_t *buffer, size_t size) {
  uint8_t acc_or;
  uint8_t acc_and;
  size_t i;

  acc_or = 0U;
  acc_and = 0xFFU;
  for (i = 0U; i < size; i++) {
    acc_or |= buffer[i];
    acc_and &= buffer[i];
  }

  return (acc_or != 0U) && (acc_and != 0xFFU);
}

static bool buffers_differ(const uint8_t *a, const uint8_t *b, size_t size) {
  size_t i;

  for (i = 0U; i < size; i++) {
    if (a[i] != b[i]) {
      return true;
    }
  }

  return false;
}

static void generate_and_check(uint32_t stage, uint8_t *buffer, size_t size) {
  msg_t msg;

  trng_test_stage = stage;
  msg = trngGenerate(&TRNGD1, size, buffer);
  test_assert(msg == HAL_RET_SUCCESS, stage | 0x01U);
  if (size >= 4U) {
    test_assert(buffer_has_activity(buffer, size), stage | 0x02U);
  }
}

static void trng_sequence(void) {
  msg_t msg;

  trng_test_stage = TEST_STAGE_START;
  msg = drvStart(&TRNGD1, NULL);
  test_assert(msg == HAL_RET_SUCCESS, TEST_STAGE_START | 0x01U);
  test_assert(drvGetStateX(&TRNGD1) == HAL_DRV_STATE_READY,
              TEST_STAGE_START | 0x02U);

  generate_and_check(TEST_STAGE_GENERATE_32, random32, sizeof random32);
  generate_and_check(TEST_STAGE_GENERATE_15, random15, sizeof random15);
  generate_and_check(TEST_STAGE_GENERATE_2, random2, sizeof random2);

  test_assert(buffers_differ(random32, random15, sizeof random15),
              TEST_STAGE_GENERATE_15 | 0x03U);

  trng_test_sample0 = ((uint32_t)random32[0] << 24) |
                      ((uint32_t)random32[1] << 16) |
                      ((uint32_t)random32[2] <<  8) |
                      ((uint32_t)random32[3] <<  0);
  trng_test_sample1 = ((uint32_t)random15[0] << 24) |
                      ((uint32_t)random15[1] << 16) |
                      ((uint32_t)random15[2] <<  8) |
                      ((uint32_t)random15[3] <<  0);

  drvStop(&TRNGD1);
  test_assert(drvGetStateX(&TRNGD1) == HAL_DRV_STATE_STOP,
              TEST_STAGE_RESTART | 0x01U);

  trng_test_stage = TEST_STAGE_RESTART;
  msg = drvStart(&TRNGD1, NULL);
  test_assert(msg == HAL_RET_SUCCESS, TEST_STAGE_RESTART | 0x02U);
  generate_and_check(TEST_STAGE_RESTART, random32, sizeof random32);
  drvStop(&TRNGD1);
}

int main(void) {

  halInit();
  chSysInit();

  portab_setup();

  trng_sequence();

  trng_test_stage = TEST_STAGE_DONE;
  trng_test_result = TEST_RESULT_PASSED;

  while (true) {
    trng_test_heartbeat++;
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}
