/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "evtimer.h"

#include "ff.h"

/**
 * @brief FS object.
 */
FATFS MMC_FS;

/**
 * MMC driver instance.
 */
MMCDriver MMCD1;

/* FS mounted and ready.*/
static bool_t fs_ready = FALSE;

/* Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig hs_spicfg = {IOPORT2, GPIOB_SPI2NSS, 0};

/* Low speed SPI configuration (281.250KHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig ls_spicfg = {IOPORT2, GPIOB_SPI2NSS, SPI_CR1_BR_2 | SPI_CR1_BR_1};

/* MMC configuration (empty).*/
static const MMCConfig mmc_cfg = {};

/* Card insertion verification.*/
static bool_t mmc_is_inserted(void) {return palReadPad(IOPORT3, GPIOC_MMCCP);}

/* Card protection verification.*/
static bool_t mmc_is_protected(void) {return !palReadPad(IOPORT3, GPIOC_MMCWP);}

/* Generic large buffer.*/
uint8_t fbuff[1024];

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

static FRESULT scan_files(char *path)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  char *fn;

  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    i = strlen(path);
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        siprintf(&path[i], "/%s", fn);
        res = scan_files(path);
        if (res != FR_OK)
          break;
        path[i] = 0;
      }
      else {
        iprintf("%s/%s\r\n", path, fn);
      }
    }
  }
  return res;
}

/*
 * Executed as event handler at 500mS intervals.
 */
static void TimerHandler(eventid_t id) {

  (void)id;
  if (palReadPad(IOPORT1, GPIOA_BUTTON)) {
    if (fs_ready) {
      FRESULT err;
      uint32_t clusters;
      FATFS *fsp;

      err = f_getfree("/", &clusters, &fsp);
      if (err != FR_OK) {
        iprintf("FS: f_getfree() failed\r\n");
        return;
      }
      iprintf("FS: %lu free clusters, %u sectors per cluster, %lu bytes free\r\n",
              clusters, MMC_FS.csize,
              clusters * (uint32_t)MMC_FS.csize * (uint32_t)MMC_SECTOR_SIZE);
      fbuff[0] = 0;
      scan_files((char *)fbuff);
    }
    else
      TestThread(&SD2);
  }
}

/*
 * MMC card insertion event.
 */
static void InsertHandler(eventid_t id) {
  FRESULT err;

  (void)id;
  iprintf("MMC: inserted\r\n");
  /*
   * On insertion MMC initialization and FS mount.
   */
  iprintf("MMC: initialization ");
  if (mmcConnect(&MMCD1)) {
    iprintf("failed\r\n");
    return;
  }
  iprintf("ok\r\n");
  iprintf("FS: mount ");
  err = f_mount(0, &MMC_FS);
  if (err != FR_OK) {
    iprintf("failed\r\n");
    mmcDisconnect(&MMCD1);
    return;
  }
  fs_ready = TRUE;
  iprintf("ok\r\n");
}

/*
 * MMC card removal event.
 */
static void RemoveHandler(eventid_t id) {

  (void)id;
  iprintf("MMC: removed\r\n");
  fs_ready = FALSE;
}

/*
 * Entry point, note, the main() function is already a thread in the system
 * on entry.
 */
int main(int argc, char **argv) {
  static const evhandler_t evhndl[] = {
    TimerHandler,
    InsertHandler,
    RemoveHandler
  };
  static EvTimer evt;
  struct EventListener el0, el1, el2;

  (void)argc;
  (void)argv;

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * Initializes the MMC driver to work with SPI2.
   */
  palSetPadMode(IOPORT2, GPIOB_SPI2NSS, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(IOPORT2, GPIOB_SPI2NSS);
  mmcObjectInit(&MMCD1, &SPID2,
                &ls_spicfg, &hs_spicfg,
                mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, &mmc_cfg);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and listen for events.
   */
  evtInit(&evt, MS2ST(500));            /* Initializes an event timer object.   */
  evtStart(&evt);                       /* Starts the event timer.              */
  chEvtRegister(&evt.et_es, &el0, 0);   /* Registers on the timer event source. */
  chEvtRegister(&MMCD1.mmc_inserted_event, &el1, 1);
  chEvtRegister(&MMCD1.mmc_removed_event, &el2, 2);
  while (TRUE)
    chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
  return 0;
}
