/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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
#include "shell.h"
#include "evtimer.h"

#include "ff.h"

/*===========================================================================*/
/* MMC/SPI related.                                                          */
/*===========================================================================*/

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
static SPIConfig hs_spicfg = {NULL, IOPORT2, GPIOB_SPI2NSS, 0};

/* Low speed SPI configuration (281.250KHz, CPHA=0, CPOL=0, MSb first).*/
static SPIConfig ls_spicfg = {NULL, IOPORT2, GPIOB_SPI2NSS,
                              SPI_CR1_BR_2 | SPI_CR1_BR_1};

/* Card insertion verification.*/
static bool_t mmc_is_inserted(void) {return palReadPad(IOPORT3, GPIOC_MMCCP);}

/* Card protection verification.*/
static bool_t mmc_is_protected(void) {return !palReadPad(IOPORT3, GPIOC_MMCWP);}

/* Generic large buffer.*/
uint8_t fbuff[1024];

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

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WA_SIZE(2048)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

static void cmd_mem(BaseChannel *chp, int argc, char *argv[]) {
  size_t n, size;
  char buf[52];

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: mem");
    return;
  }
  n = chHeapStatus(NULL, &size);
  siprintf(buf, "core free memory : %u bytes", chCoreStatus());
  shellPrintLine(chp, buf);
  siprintf(buf, "heap fragments   : %u", n);
  shellPrintLine(chp, buf);
  siprintf(buf, "heap free total  : %u bytes", size);
  shellPrintLine(chp, buf);
}

static void cmd_threads(BaseChannel *chp, int argc, char *argv[]) {
  static const char *states[] = {
    "READY",
    "CURRENT",
    "SUSPENDED",
    "WTSEM",
    "WTMTX",
    "WTCOND",
    "SLEEPING",
    "WTEXIT",
    "WTOREVT",
    "WTANDEVT",
    "SNDMSG",
    "WTMSG",
    "WTQUEUE",
    "FINAL"
  };
  Thread *tp;
  char buf[60];

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: threads");
    return;
  }
  shellPrintLine(chp, "    addr    stack prio refs     state time");
  tp = chRegFirstThread();
  do {
    siprintf(buf, "%8lx %8lx %4u %4i %9s %u",
             (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
             (unsigned int)tp->p_prio, tp->p_refs - 1,
             states[tp->p_state], (unsigned int)tp->p_time);
    shellPrintLine(chp, buf);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseChannel *chp, int argc, char *argv[]) {
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: test");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriority(),
                           TestThread, chp);
  if (tp == NULL) {
    shellPrintLine(chp, "out of memory");
    return;
  }
  chThdWait(tp);
}

static void cmd_tree(BaseChannel *chp, int argc, char *argv[]) {
  FRESULT err;
  uint32_t clusters;
  FATFS *fsp;

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: tree");
    return;
  }
  if (!fs_ready) {
    shellPrintLine(chp, "File System not mounted");
    return;
  }
  err = f_getfree("/", &clusters, &fsp);
  if (err != FR_OK) {
    shellPrintLine(chp, "FS: f_getfree() failed");
    return;
  }
  siprintf((void *)fbuff,
           "FS: %lu free clusters, %lu sectors per cluster, %lu bytes free",
           clusters, (uint32_t)MMC_FS.csize,
           clusters * (uint32_t)MMC_FS.csize * (uint32_t)MMC_SECTOR_SIZE);
  shellPrintLine(chp, (void *)fbuff);
  fbuff[0] = 0;
  scan_files((char *)fbuff);
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"tree", cmd_tree},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseChannel *)&SD2,
  commands
};

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palTogglePad(IOPORT3, GPIOC_LED);
    if (fs_ready)
      chThdSleepMilliseconds(200);
    else
      chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * MMC card insertion event.
 */
static void InsertHandler(eventid_t id) {
  FRESULT err;

  (void)id;
  /*
   * On insertion MMC initialization and FS mount.
   */
  if (mmcConnect(&MMCD1)) {
    return;
  }
  err = f_mount(0, &MMC_FS);
  if (err != FR_OK) {
    mmcDisconnect(&MMCD1);
    return;
  }
  fs_ready = TRUE;
}

/*
 * MMC card removal event.
 */
static void RemoveHandler(eventid_t id) {

  (void)id;
  fs_ready = FALSE;
}

/*
 * Application entry point.
 */
int main(void) {
  static const evhandler_t evhndl[] = {
    InsertHandler,
    RemoveHandler
  };
  Thread *shelltp = NULL;
  struct EventListener el0, el1;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Initializes the MMC driver to work with SPI2.
   */
  palSetPadMode(IOPORT2, GPIOB_SPI2NSS, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(IOPORT2, GPIOB_SPI2NSS);
  mmcObjectInit(&MMCD1, &SPID2,
                &ls_spicfg, &hs_spicfg,
                mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and listen for events.
   */
  chEvtRegister(&MMCD1.mmc_inserted_event, &el0, 0);
  chEvtRegister(&MMCD1.mmc_removed_event, &el1, 1);
  while (TRUE) {
    if (!shelltp)
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    else if (chThdTerminated(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
  }
  return 0;
}
