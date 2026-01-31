/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "vfs.h"

#include "chprintf.h"
#include "nullstreams.h"
#include "xshell.h"

#include "portab.h"

/*===========================================================================*/
/* Card insertion monitor.                                                   */
/*===========================================================================*/

/* FS mounted and ready.*/
static bool fs_ready = false;

/**
 * @brief   Card event sources.
 */
static event_source_t inserted_event, removed_event;

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

/*===========================================================================*/
/* VFS related.                                                              */
/*===========================================================================*/

/* VFS FatFS driver object to be mounted as /.*/
static vfs_chfs_driver_c chfs_driver;

/* VFS streams driver object to be mounted as /dev.*/
static vfs_streams_driver_c dev_driver;

/* VFS overlay driver object representing the root directory.*/
static vfs_overlay_driver_c root_overlay_driver;

/* Global pointer to the root VFS driver.*/
vfs_driver_c *vfs_root = (vfs_driver_c *)&root_overlay_driver;

/* A null stream object.*/
static NullStream nullstream;

/* Stream to be exposed under /dev as files.*/
static const drv_streams_element_t streams[] = {
  {"VSD1", (sequential_stream_i *)&PORTAB_SD1, VFS_MODE_S_IFCHR},
  {"null", (sequential_stream_i *)&nullstream, VFS_MODE_S_IFCHR},
  {NULL, NULL, 0}
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE       THD_STACK_SIZE(1024)

static const xshell_command_t commands[] = {
  {NULL, NULL}
};

static const xshell_manager_config_t cfg1 = {
  .thread_name      = "shell",
  .banner           = XSHELL_DEFAULT_BANNER_STR,
  .prompt           = XSHELL_DEFAULT_PROMPT_STR,
  .commands         = commands,
  .use_heap         = true,
  .stack.size       = SHELL_WA_SIZE
};

static xshell_manager_t sm1;

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

static xshell_t *xshp = NULL;

#if !HAL_USE_SDC
static uint8_t __nocache_mmcbuf[MMC_BUFFER_SIZE];
mmc_spi_driver_t MMCD1;

/* MMC/SD over SPI driver configuration.*/
static MMCConfig mmccfg = {&PORTAB_SPI1, &ls_spicfg, &hs_spicfg};
#endif

/*
 * Card insertion event.
 */
static void InsertHandler(eventid_t id) {
  msg_t err;

  (void)id;

#if HAL_USE_SDC
  if (sdcConnect(&PORTAB_SDCD1)) {
    return;
  }
#else
  if (mmcConnect(&MMCD1)) {
    return;
  }
#endif

  err = chfsdrvMount(&chfs_driver);
  if (CH_RET_IS_ERROR(err)) {
#if HAL_USE_SDC
    sdcDisconnect(&PORTAB_SDCD1);
#else
    mmcDisconnect(&MMCD1);
#endif
    return;
  }
  fs_ready = true;
}

/*
 * Card removal event.
 */
static void RemoveHandler(eventid_t id) {

  (void)id;

#if HAL_USE_SDC
  sdcDisconnect(&PORTAB_SDCD1);
#else
  mmcDisconnect(&MMCD1);
#endif
  fs_ready = false;
}

/*
 * Shell exit event.
 */
static void ShellHandler(eventid_t id) {

  (void)id;

  (void) chThdWait(&xshp->thread);
  xshp = NULL;
}

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_STACK(thd1_stack, 256);
static THD_FUNCTION(thd1_func, arg) {

  (void)arg;

  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(fs_ready ? 100 : 500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  vfs_file_node_c *file1;
  msg_t msg;
  event_listener_t el0, el1, el2;
  static const evhandler_t evhndl[] = {
    InsertHandler,
    RemoveHandler,
    ShellHandler
  };

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - Virtual File System initialization.
   */
  halInit();
  chSysInit();
  vfsInit();

  /* Board-dependent setup code.*/
  portab_setup();

  /* Spawning a blinker thread.*/
  static thread_t thd1;
  static const THD_DECL_STATIC(thd1_desc, "blinker", thd1_stack,
                               NORMALPRIO + 10, thd1_func, NULL, NULL);
  chThdSpawnRunning(&thd1, &thd1_desc);

#if HAL_USE_SDC
  /* Activates the SDC driver using default configuration.*/
  sdcStart(&PORTAB_SDCD1, NULL);

  /* Activates the card insertion monitor.*/
  tmr_init(&PORTAB_SDCD1);
#else
  /* Activates the MMC_SPI driver.*/
  mmcObjectInit(&MMCD1, __nocache_mmcbuf);
  mmcStart(&MMCD1, &mmccfg);

  /* Activates the card insertion monitor.*/
  tmr_init(&MMCD1);
#endif

  /* Activates the SIO driver and a null stream.*/
  sdStart(&PORTAB_SD1, NULL);
  nullObjectInit(&nullstream);

  /* Initialization of the VFS ChibiFS driver object.*/
  chfsdrvObjectInit(&chfs_driver, NULL);

  /* Initialization of the VFS stream driver object.*/
  stmdrvObjectInit(&dev_driver, &streams[0]);

  /* Initializing an overlay VFS object overlaying the LittleFS driver.*/
  ovldrvObjectInit(&root_overlay_driver, (vfs_driver_c *)&chfs_driver, NULL);

  /* Registering the streams VFS driver on the VFS overlay root as "/dev".*/
  msg = ovldrvRegisterDriver(&root_overlay_driver, (vfs_driver_c *)&dev_driver, "dev");
  if (CH_RET_IS_ERROR(msg)) {
    chSysHalt("VFS");
  }

  /* Opening a file for shell I/O.*/
  msg = vfsOpenFile("/dev/VSD1", VO_RDWR, &file1);
  if (CH_RET_IS_ERROR(msg)) {
    chSysHalt("VFS");
  }

  /* Shell manager initialization.*/
  xshellObjectInit(&sm1, &cfg1);

  /* Normal main() thread activity, spawning shells.*/
  chEvtRegister(&inserted_event, &el0, 0);
  chEvtRegister(&removed_event, &el1, 1);
  chEvtRegister(&sm1.events, &el2, 2);
  while (true) {
    if (xshp == NULL) {
      /* Spawning a shell.*/
      xshp = xshellSpawn(&sm1, (BaseSequentialStream *)vfsGetFileStream(file1),
                         NORMALPRIO + 1, NULL);
    }
    chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));
  }
}
