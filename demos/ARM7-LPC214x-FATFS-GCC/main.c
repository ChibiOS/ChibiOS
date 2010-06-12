/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
#include "buzzer.h"

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

/* Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0).*/
static SPIConfig hs_spicfg = {
  IOPORT1,
  PA_SSEL1,
  CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0),
  0,
  2
};

/* Low speed SPI configuration (281.250KHz, CPHA=0, CPOL=0).*/
static SPIConfig ls_spicfg = {
  IOPORT1,
  PA_SSEL1,
  CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0),
  0,
  254
};

/* MMC configuration (empty).*/
static const MMCConfig mmc_cfg = {};

/* Card insertion verification.*/
static bool_t mmc_is_inserted(void) {
  return !palReadPad(IOPORT2, PB_CP1);
}

/* Card protection verification.*/
static bool_t mmc_is_protected(void) {
  return palReadPad(IOPORT2, PB_WP1);
}

/* Generic large buffer.*/
uint8_t fbuff[1024];

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPort(IOPORT1, PAL_PORT_BIT(PA_LED2));
    chThdSleepMilliseconds(200);
    palSetPort(IOPORT1, PAL_PORT_BIT(PA_LED1) | PAL_PORT_BIT(PA_LED2));
    chThdSleepMilliseconds(800);
    palClearPort(IOPORT1, PAL_PORT_BIT(PA_LED1));
    chThdSleepMilliseconds(200);
    palSetPort(IOPORT1, PAL_PORT_BIT(PA_LED1) | PAL_PORT_BIT(PA_LED2));
    chThdSleepMilliseconds(800);
  }
  return 0;
}

/*
 * Yellow LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPad(IOPORT1, PA_LEDUSB);
    chThdSleepMilliseconds(200);
    palSetPad(IOPORT1, PA_LEDUSB);
    chThdSleepMilliseconds(300);
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
  if (!palReadPad(IOPORT1, PA_BUTTON1)) {
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
  }
  else if (!palReadPad(IOPORT1, PA_BUTTON2)) {
    static WORKING_AREA(waTestThread, 256);
    Thread *tp = chThdCreateStatic(waTestThread, sizeof(waTestThread),
                                   NORMALPRIO, TestThread, &SD1);
    chThdWait(tp);
    buzzPlay(500, MS2ST(100));
  }
}

/*
 * MMC card insertion event.
 */
static void InsertHandler(eventid_t id) {
  FRESULT err;

  (void)id;
  buzzPlayWait(1000, MS2ST(100));
  buzzPlayWait(2000, MS2ST(100));
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
  buzzPlay(440, MS2ST(200));
}

/*
 * MMC card removal event.
 */
static void RemoveHandler(eventid_t id) {

  (void)id;
  iprintf("MMC: removed\r\n");
  fs_ready = FALSE;
  buzzPlayWait(2000, MS2ST(100));
  buzzPlayWait(1000, MS2ST(100));
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
  sdStart(&SD1, NULL);

  /*
   * Buzzer driver initialization.
   */
  buzzInit();

  /*
   * Initializes the MMC driver to work with SPI2.
   */
  mmcObjectInit(&MMCD1, &SPID1,
                &ls_spicfg, &hs_spicfg,
                mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, &mmc_cfg);

  /*
   * Creates the blinker threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and listen for events.
   */
  evtInit(&evt, MS2ST(500));            /* Initializes an event timer object.   */
  evtStart(&evt);                       /* Starts the event timer.              */
  chEvtRegister(&evt.et_es, &el0, 0);   /* Registers on the timer event source. */
  chEvtRegister(&MMCD1.mmc_inserted_event, &el1, 1);
  chEvtRegister(&MMCD1.mmc_removed_event, &el2, 2);
  while (TRUE)// chThdSleepMilliseconds(50);
    chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
  return 0;
}
