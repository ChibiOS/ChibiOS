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

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "vfs.h"

#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "nullstreams.h"
#include "chprintf.h"
#include "shell.h"

#include "portab.h"

/*===========================================================================*/
/* Card insertion monitor.                                                   */
/*===========================================================================*/

/**
 * @brief   Card event sources.
 */
static event_source_t inserted_event, removed_event;

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

/**
 * @brief   Card monitor timer.
 */
static virtual_timer_t tmr;

/**
 * @brief   Debounce counter.
 */
static unsigned cnt;

/**
 * @brief   Insertion monitor timer callback function.
 *
 * @param[in] p         pointer to the @p BaseBlockDevice object
 *
 * @notapi
 */
static void tmrfunc(virtual_timer_t *vtp, void *p) {
  BaseBlockDevice *bbdp = p;

  chSysLockFromISR();
  if (cnt > 0) {
    if (blkIsInserted(bbdp)) {
      if (--cnt == 0) {
        chEvtBroadcastI(&inserted_event);
      }
    }
    else
      cnt = POLLING_INTERVAL;
  }
  else {
    if (!blkIsInserted(bbdp)) {
      cnt = POLLING_INTERVAL;
      chEvtBroadcastI(&removed_event);
    }
  }
  chVTSetI(vtp, TIME_MS2I(POLLING_DELAY), tmrfunc, bbdp);
  chSysUnlockFromISR();
}

/**
 * @brief   Polling monitor start.
 *
 * @param[in] p         pointer to an object implementing @p BaseBlockDevice
 *
 * @notapi
 */
static void tmr_init(void *p) {

  chEvtObjectInit(&inserted_event);
  chEvtObjectInit(&removed_event);
  chSysLock();
  cnt = POLLING_INTERVAL;
  chVTSetI(&tmr, TIME_MS2I(POLLING_DELAY), tmrfunc, p);
  chSysUnlock();
}
#endif

/*===========================================================================*/
/* FatFS related.                                                            */
/*===========================================================================*/

/* FS mounted and ready.*/
static bool fs_ready = false;

/*===========================================================================*/
/* VFS related.                                                              */
/*===========================================================================*/

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
/* VFS FatFS driver object representing the root directory.*/
static vfs_fatfs_driver_c root_driver;
#endif

/* VFS overlay driver object representing the root directory.*/
static vfs_overlay_driver_c root_overlay_driver;

/* VFS streams driver object representing the /dev directory.*/
static vfs_streams_driver_c dev_driver;

/* VFS API will use this object as implicit root, defining this
   symbol is expected.*/
vfs_driver_c *vfs_root = (vfs_driver_c *)&root_overlay_driver;

static NullStream nullstream;

/* Stream to be exposed under /dev as files.*/
static const drv_streams_element_t streams[] = {
  {"VSD1", (BaseSequentialStream *)&PORTAB_SD1},
  {"null", (BaseSequentialStream *)&nullstream},
  {NULL, NULL}
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static ShellConfig shell_cfg1 = {
  NULL,
  NULL
};

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * Pointer to the shell thread, if active, else NULL.
 */
static thread_t *shelltp = NULL;

/*
 * Card insertion event.
 */
static void InsertHandler(eventid_t id) {

  (void)id;

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
  msg_t err;

#if HAL_USE_SDC
  if (sdcConnect(&PORTAB_SDCD1)) {
    return;
  }
#else
  if (mmcConnect(&MMCD1)) {
    return;
  }
#endif

  err = drvFatFSMount("0:", 1);
  if (CH_RET_IS_ERROR(err)) {
#if HAL_USE_SDC
    sdcDisconnect(&PORTAB_SDCD1);
#else
  if (mmcDisconnect(&MMCD1)) {
#endif
   return;
  }
  fs_ready = true;
#endif
}

/*
 * Card removal event.
 */
static void RemoveHandler(eventid_t id) {

  (void)id;

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
#if HAL_USE_SDC
    sdcDisconnect(&PORTAB_SDCD1);
#else
    mmcDisconnect(&MMCD1);
#endif
    fs_ready = false;
#endif
}

/*
 * Shell exit event.
 */
static void ShellHandler(eventid_t id) {

  (void)id;
  if (chThdTerminatedX(shelltp)) {
    chThdRelease(shelltp);
    shelltp = NULL;
  }
}

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(fs_ready ? 250 : 500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  msg_t msg;
  vfs_file_node_c *file;
  static const evhandler_t evhndl[] = {
    InsertHandler,
    RemoveHandler,
    ShellHandler
  };
  event_listener_t el0, el1, el2;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - Virtual File System initialization.
   * - Shell manager initialization.
   */
  halInit();
  chSysInit();
  vfsInit();
  shellInit();

  /* Board-dependent setup code.*/
  portab_setup();

  /* Starting a serial port for the shell, initializing other streams too.*/
  sdStart(&PORTAB_SD1, NULL);
  nullObjectInit(&nullstream);

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
  /* Activates the  SDC driver using default configuration.*/
  sdcStart(&PORTAB_SDCD1, NULL);

  /* Activates the card insertion monitor.*/
  tmr_init(&PORTAB_SDCD1);

  /* Initializing an overlay VFS object overlaying a FatFS driver. Note
     that this virtual file system can only access the "/sb1" sub-directory
     on the physical FatFS volume.*/
  drvOverlayObjectInit(&root_overlay_driver,
                       drvFatFSObjectInit(&root_driver),
                       "/sb1");
#else
  /* Initializing an overlay VFS object as a root, no overlaid driver.*/
  drvOverlayObjectInit(&root_overlay_driver, NULL, NULL);
#endif

  /* Registering a streams VFS driver on the VFS overlay root as "/dev".*/
  msg = drvOverlayRegisterDriver(&root_overlay_driver,
                                 drvStreamsObjectInit(&dev_driver, &streams[0]),
                                 "dev");
  if (CH_RET_IS_ERROR(msg)) {
    chSysHalt("VFS");
  }

  /* Opening a file for shell I/O.*/
  msg = vfsOpenFile("/dev/VSD1", VO_RDWR, &file);
  if (CH_RET_IS_ERROR(msg)) {
    chSysHalt("VFS");
  }
  shell_cfg1.sc_channel = vfsGetFileStream(file);

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Normal main() thread activity, handling shell start/exit.*/
  chEvtRegister(&inserted_event, &el0, 0);
  chEvtRegister(&removed_event, &el1, 1);
  chEvtRegister(&shell_terminated, &el2, 2);
  while (true) {
    if (shelltp == NULL) {
      /* Spawning a shell.*/
      shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                    "shell", NORMALPRIO + 1,
                                    shellThread, (void *)&shell_cfg1);
    }
    chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));
  }
}
