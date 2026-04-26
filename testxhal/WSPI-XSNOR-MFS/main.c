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

#include "hal_xsnor_micron_n25q.h"
#include "hal_xsnor_macronix_mx25.h"
#include "hal_mfs.h"

#include "mfs_test_root.h"

#include "portab.h"

static union {
  hal_xsnor_micron_n25q_c       n25q;
  hal_xsnor_macronix_mx25_c     mx25;
} snor1;

static xsnor_buffers_t __nocache_snor1buf;

static const xsnor_config_t snorcfg_n25q = {
  .bus_type         = XSNOR_BUS_MODE_WSPI_4LINES,
  .bus.wspi.drv     = &PORTAB_WSPI1,
  .bus.wspi.cfg     = &WSPIcfg1,
  .buffers          = &__nocache_snor1buf,
  .options          = N25Q_OPT_DUMMY_CYCLES(8) |
                      N25Q_OPT_USE_SUBSECTORS |
                      N25Q_OPT_NICE_WAITING
};

static const xsnor_config_t snorcfg_mx25 = {
  .bus_type         = XSNOR_BUS_MODE_WSPI_8LINES,
  .bus.wspi.drv     = &PORTAB_WSPI1,
  .bus.wspi.cfg     = &WSPIcfg1,
  .buffers          = &__nocache_snor1buf,
  .options          = MX25_OPT_DUMMY_CYCLES(14) |
                      MX25_OPT_USE_SUBSECTORS |
                      MX25_OPT_NICE_WAITING
};

const MFSConfig mfscfg1 = {
  .flashp           = (BaseFlash *)&snor1.n25q,
  .erased           = 0xFFFFFFFFU,
  .bank_size        = 4096U,
  .bank0_start      = 0U,
  .bank0_sectors    = 1U,
  .bank1_start      = 1U,
  .bank1_sectors    = 1U
};

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

static hal_buffered_sio_c bsio1;
static uint8_t bsio1_ib[128];
static uint8_t bsio1_ob[512];
static volatile msg_t mfs_test_result;
static volatile bool mfs_test_done;

int main(void) {
  bool button_pressed;

  halInit();
  chSysInit();

  portab_setup();

  bsioObjectInit(&bsio1, &PORTAB_SIO1, bsio1_ib, sizeof bsio1_ib,
                 bsio1_ob, sizeof bsio1_ob);
  drvStart(&bsio1, NULL);

  n25qObjectInit(&snor1.n25q);
  if (drvStart(&snor1.n25q, &snorcfg_n25q) != HAL_RET_SUCCESS) {
    mx25ObjectInit(&snor1.mx25);
    if (drvStart(&snor1.mx25, &snorcfg_mx25) != HAL_RET_SUCCESS) {
      chSysHalt("device not found");
    }
  }

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  button_pressed = false;
  while (true) {
    bool current_pressed;

    current_pressed = palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED;
    if (current_pressed && !button_pressed) {
      mfs_test_result = test_execute((BaseSequentialStream *)&bsio1.chn,
                                     &mfs_test_suite);
      mfs_test_done = true;
    }
    button_pressed = current_pressed;
    chThdSleepMilliseconds(100);
  }
}
