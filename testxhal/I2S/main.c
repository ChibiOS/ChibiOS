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

#define I2S_BUFFER_SIZE                     256U
#define I2S_EVENT_TIMEOUT_MS               2000U

#define TEST_I2SCFGR_I2SSTD_MSB           (1U << SPI_I2SCFGR_I2SSTD_Pos)
#define TEST_I2SCFGR_I2SCFG_MASTER_RX     (3U << SPI_I2SCFGR_I2SCFG_Pos)
#define TEST_I2SCFGR_I2SMOD_I2S           (1U << SPI_I2SCFGR_I2SMOD_Pos)

static uint16_t i2s_rx_buf[I2S_BUFFER_SIZE];

volatile uint32_t i2s_test_failure = 0U;
volatile uint32_t i2s_test_result = 0U;
volatile uint32_t i2s_test_heartbeat = 0U;

static volatile unsigned i2s_cb_count;
static volatile unsigned i2s_half_count;
static volatile unsigned i2s_full_count;
static volatile unsigned i2s_error_count;
static volatile i2seventflags_t i2s_seen_events;
static volatile i2serror_t i2s_seen_errors;
static volatile driver_state_t i2s_last_state;
static volatile uint32_t i2s_cb_anomalies;

static bool streaming;

static const I2SConfig i2scfg = {
  .tx_buffer = NULL,
  .rx_buffer = i2s_rx_buf,
  .size = I2S_BUFFER_SIZE,
  .i2scfgr = TEST_I2SCFGR_I2SSTD_MSB |
             TEST_I2SCFGR_I2SCFG_MASTER_RX |
             TEST_I2SCFGR_I2SMOD_I2S,
  .i2spr = 0x117U
};

static void test_fail(uint32_t code) {

  i2s_test_failure = code;
  i2s_test_result = 0x2468ACE0U;
  while (true) {
    i2s_test_heartbeat++;
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_ON);
    chThdSleepMilliseconds(100);
    palWriteLine(PORTAB_LINE_LED1, PORTAB_LED_OFF);
    chThdSleepMilliseconds(100);
  }
}

static void test_assert(bool condition, uint32_t code) {

  if (!condition) {
    test_fail(code);
  }
}

static void reset_i2s_counters(void) {

  i2s_cb_count = 0U;
  i2s_half_count = 0U;
  i2s_full_count = 0U;
  i2s_error_count = 0U;
  i2s_seen_events = 0U;
  i2s_seen_errors = 0U;
  i2s_last_state = HAL_DRV_STATE_READY;
  i2s_cb_anomalies = 0U;
}

static void i2s_test_cb(void *ip) {
  I2SDriver *i2sp = (I2SDriver *)ip;
  driver_state_t state;
  i2seventflags_t events;
  i2serror_t errors;

  state = drvGetStateX(i2sp);
  events = i2sGetAndClearEventsX(i2sp, (i2seventflags_t)-1);
  errors = i2sGetAndClearErrorsX(i2sp, (i2serror_t)-1);

  i2s_cb_count++;
  i2s_last_state = state;
  i2s_seen_events |= events;
  i2s_seen_errors |= errors;

  if (state == HAL_DRV_STATE_HALF) {
    i2s_half_count++;
    if ((events & I2S_EVENT_HALF) == 0U) {
      i2s_cb_anomalies |= 0x01U;
    }
  }
  else if (state == HAL_DRV_STATE_FULL) {
    i2s_full_count++;
    if ((events & I2S_EVENT_FULL) == 0U) {
      i2s_cb_anomalies |= 0x02U;
    }
  }
  else if (state == HAL_DRV_STATE_ERROR) {
    i2s_error_count++;
    if (errors == 0U) {
      i2s_cb_anomalies |= 0x04U;
    }
  }
  else {
    i2s_cb_anomalies |= 0x08U;
  }
}

static void wait_button_release(void) {

  while (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
    chThdSleepMilliseconds(20);
  }
}

static void i2s_setup_pins(void) {

  palSetPadMode(GPIOB, 12U, PAL_MODE_ALTERNATE(5));
  palSetPadMode(GPIOB, 13U, PAL_MODE_ALTERNATE(5));
  palSetPadMode(GPIOC, 3U, PAL_MODE_ALTERNATE(5));
}

static void stop_stream(void) {

  if (streaming) {
    i2sStopExchange(&I2SD2);
    streaming = false;
  }
}

static void start_stream(void) {

  test_assert(i2sStartExchange(&I2SD2) == HAL_RET_SUCCESS, 0x00000020U);
  streaming = true;
}

static void verify_stream_start(uint32_t code_base) {
  unsigned elapsed = 0U;

  while (((i2s_half_count == 0U) || (i2s_full_count == 0U)) &&
         (i2s_error_count == 0U) &&
         (elapsed < I2S_EVENT_TIMEOUT_MS)) {
    chThdSleepMilliseconds(10);
    elapsed += 10U;
  }

  test_assert(elapsed < I2S_EVENT_TIMEOUT_MS, code_base | 0x01U);
  test_assert(i2s_cb_count >= 2U, code_base | 0x02U);
  test_assert(i2s_half_count > 0U, code_base | 0x03U);
  test_assert(i2s_full_count > 0U, code_base | 0x04U);
  test_assert(i2s_error_count == 0U, code_base | 0x05U);
  test_assert(i2s_seen_errors == 0U, code_base | 0x06U);
  test_assert((i2s_seen_events & I2S_EVENT_HALF) != 0U, code_base | 0x07U);
  test_assert((i2s_seen_events & I2S_EVENT_FULL) != 0U, code_base | 0x08U);
  test_assert(i2s_cb_anomalies == 0U, code_base | 0x09U);
}

int main(void) {
  bool button_was_pressed;
  systime_t led_time;

  halInit();
  chSysInit();

  i2s_test_failure = 0U;
  i2s_test_result = 0U;
  i2s_test_heartbeat = 0U;
  streaming = false;
  button_was_pressed = false;
  led_time = chVTGetSystemTimeX();

  portab_setup();
  wait_button_release();
  drvSetCallbackX(&I2SD2, i2s_test_cb);
  test_assert(drvStart(&I2SD2, &i2scfg) == HAL_RET_SUCCESS, 0x00000001U);
  i2s_setup_pins();

  reset_i2s_counters();
  start_stream();
  verify_stream_start(0x00000100U);
  i2s_test_result = 0x13579BDFU;

  while (true) {
    bool button_pressed;

    i2s_test_heartbeat++;
    if (i2s_error_count != 0U) {
      test_fail(0x00000200U);
    }

    button_pressed = palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED;
    if (button_pressed && !button_was_pressed) {
      if (streaming) {
        stop_stream();
      }
      else {
        reset_i2s_counters();
        start_stream();
        verify_stream_start(0x00000300U);
      }
    }
    button_was_pressed = button_pressed;

    if (chVTTimeElapsedSinceX(led_time) >= TIME_MS2I(streaming ? 250U : 1000U)) {
      palToggleLine(PORTAB_LINE_LED1);
      led_time = chVTGetSystemTimeX();
    }

    chThdSleepMilliseconds(20);
  }
}
