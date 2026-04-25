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

#include "hal_mmc_spi.h"
#include "portab.h"

CC_ALIGN_DATA(32) static uint8_t mmc_buffer[MMC_SPI_BUFFER_SIZE];
static MMCSPIDriver MMCD1;

static const MMCSPIConfig mmc_config = {
  .spip               = &PORTAB_SPI,
  .lscfg              = &ls_spicfg,
  .hscfg              = &hs_spicfg,
  .is_inserted        = portab_mmc_is_inserted,
  .is_write_protected = portab_mmc_is_write_protected,
  .arg                = NULL
};

static THD_WORKING_AREA(waBlinker, 256);
static THD_FUNCTION(Blinker, arg) {
  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    systime_t delay;

    if (MMCD1.media_state == MMC_SPI_MEDIA_READY) {
      delay = TIME_MS2I(100);
    }
    else if (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
      delay = TIME_MS2I(50);
    }
    else {
      delay = TIME_MS2I(300);
    }

    palToggleLine(PORTAB_LINE_LED1);
    chThdSleep(delay);
  }
}

int main(void) {
  hal_blk_info_t info;

  halInit();
  chSysInit();

  portab_setup();
  mmcSpiObjectInit(&MMCD1, mmc_buffer);
  (void)drvStart(&MMCD1, &mmc_config);

  chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, Blinker, NULL);

  while (true) {
    if ((MMCD1.media_state != MMC_SPI_MEDIA_READY) &&
        (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED)) {
      if (mmcSpiConnect(&MMCD1) == HAL_SUCCESS) {
        (void)mmcSpiGetInfo(&MMCD1, &info);
      }
    }

    chThdSleepMilliseconds(100);
  }
}
