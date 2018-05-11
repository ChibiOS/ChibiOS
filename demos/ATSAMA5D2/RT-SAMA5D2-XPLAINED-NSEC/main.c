/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
#include "tsclient.h"
#include "daemons/tssockskel.h"
#include "daemons/tsioblksskel.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include "chprintf.h"
#include "lwipthread.h"
#include "sama_sdmmc_lld.h"

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_KEYBACK_LED_RED);
    chThdSleepMilliseconds(80);
    palToggleLine(LINE_KEYBACK_LED_RED);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_KEYBACK_LED_RED);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_KEYBACK_LED_RED);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_KEYBACK_LED_RED);
    chThdSleepMilliseconds(160);
    palToggleLine(LINE_KEYBACK_LED_RED);
    chThdSleepMilliseconds(600);
  }
}

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
};

#define BLOCK_CNT_MAX               32u
#define DMADL_CNT_MAX               64u
#define BLOCK_CNT                   3u

CACHE_ALIGNED uint8_t data_buf[BLOCK_CNT_MAX * 512ul];
CACHE_ALIGNED static uint32_t dma_table[DMADL_CNT_MAX * SDMMC_DMADL_SIZE];
CACHE_ALIGNED uint8_t sdmmcbuffer[ROUND_UP_MULT(SDMMC_BUFFER_SIZE, L1_CACHE_BYTES)];

static const SamaSDMMCConfig sdmmc_cfg = {
    SDMMC_SLOT0,
    sdmmcbuffer,
    data_buf,
    sizeof data_buf,
    dma_table,
    DMADL_CNT_MAX
};

bool sdmmcGetInstance(uint8_t index, SdmmcDriver **sdmmcp)
{
  (void)index;

  *sdmmcp = &SDMMCD1;
  return true;
}

/*
 * Application entry point.
 */
int main(void) {
  ts_service_t tssvc;
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  lwipInit(NULL);

  /*
   * Activates the serial driver 0 using the driver default configuration.
   */
  sdStart(&SD1, &sdcfg);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof waThread1, NORMALPRIO-1, Thread1, NULL);

  /*
   * Init and open the sdmmc device.
   */
  sdmmcInit();
  sdmmcStart(&SDMMCD1, &sdmmc_cfg);
  if (SDMMCD1.state != MCID_INIT_ERROR) {
    sdmmcOpenDevice(&SDMMCD1);
  } else {
    chprintf((BaseSequentialStream *)&SD1, "Cannot start sdmmc device.\r\n");
    sdmmcStop(&SDMMCD1);
  }

  tsSocksSkelInit();
  tsIOBlksSkelInit();

  /*
   * Call the dummy secure service
   */
  chprintf((BaseSequentialStream*)&SD1, "Calling the secure service\n\r");

  /* Retrieve the service handle by name */
  tssvc = (ts_service_t) tsInvokeServiceNoYield(TS_HND_DISCOVERY,
      (ts_params_area_t)"TsSimpleService", sizeof "TsSimpleService");
  if ((int32_t)tssvc < 0) {
    chprintf((BaseSequentialStream*)&SD1, "Cannot get the handle of '%s': %d\r\n",
        "TsSimpleService", tssvc);
  }
  /*
   * Normal main() thread activity, in this demo it does nothing except
   * calling periodically the dummy service and check the button state.
   */
  while (true) {
    msg_t r;

    /* Invoke the service */
    r = tsInvokeServiceNoYield(tssvc, (ts_params_area_t)"HELO", sizeof "HELO");
    chprintf((BaseSequentialStream*)&SD1, "Call result: %d\r\n", r);
#if 0
    if(!palReadPad(PIOB, PIOB_USER_PB)) {
      test_execute((BaseSequentialStream *)&SD1, &rt_test_suite);
      test_execute((BaseSequentialStream *)&SD1, &oslib_test_suite);
    }
#endif
    chThdSleepMilliseconds(500);
  }
}
