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
#include "sb.h"
#include "chprintf.h"

#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "nullstreams.h"

#include "startup_defs.h"

#include "sdmon.h"

/*===========================================================================*/
/* VFS-related.                                                              */
/*===========================================================================*/

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
/* VFS FatFS driver used by all file systems.*/
static vfs_fatfs_driver_c fatfs_driver;
#endif

/* VFS overlay driver object representing the absolute root directory.*/
static vfs_overlay_driver_c root_overlay_driver;

/* Segregated roots for the two sandboxes.*/
static vfs_overlay_driver_c sb1_root_overlay_driver;
static vfs_overlay_driver_c sb2_root_overlay_driver;

/* Shared directory between the two sandboxes.*/
static vfs_overlay_driver_c sb_shared_overlay_driver;

/* VFS streams driver objects representing the /dev private directories.*/
static vfs_streams_driver_c sb1_dev_driver;
static vfs_streams_driver_c sb2_dev_driver;

/* VFS API will use this object as implicit root, defining this
   symbol is expected.*/
vfs_driver_c *vfs_root = (vfs_driver_c *)&root_overlay_driver;

/* Used for /dev/null.*/
static NullStream nullstream;

/* Streams to be exposed under /dev as files.*/
static const drv_streams_element_t sb1_streams[] = {
  {"VSD1", (BaseSequentialStream *)&SD2},
  {"null", (BaseSequentialStream *)&nullstream},
  {NULL, NULL}
};
static const drv_streams_element_t sb2_streams[] = {
  {"VSD1", (BaseSequentialStream *)&LPSD1},
  {"null", (BaseSequentialStream *)&nullstream},
  {NULL, NULL}
};

/*===========================================================================*/
/* SB-related.                                                               */
/*===========================================================================*/

/* Sandbox 1 configuration.*/
static const sb_config_t sb_config1 = {
  .code_region      = 0U,
  .data_region      = 1U,
  .regions          = {
    [0] = {
      .area         = {STARTUP_FLASH1_BASE, STARTUP_FLASH1_SIZE},
      .writeable    = false
    },
    [1] = {
      .area         = {STARTUP_RAM1_BASE,   STARTUP_RAM1_SIZE},
      .writeable    = true
    }
  },
  .vfs_driver       = (vfs_driver_c *)&sb1_root_overlay_driver
};

/* Sandbox 2 configuration.*/
static const sb_config_t sb_config2 = {
  .code_region      = 0U,
  .data_region      = 1U,
  .regions          = {
    [0] = {
      .area         = {STARTUP_FLASH2_BASE, STARTUP_FLASH2_SIZE},
      .writeable    = false
    },
    [1] = {
      .area         = {STARTUP_RAM2_BASE,   STARTUP_RAM2_SIZE},
      .writeable    = true
    }
  },
  .vfs_driver       = (vfs_driver_c *)&sb2_root_overlay_driver
};

/* Sandbox objects.*/
sb_class_t sbx1, sbx2;

static THD_WORKING_AREA(waUnprivileged1, 512);
static THD_WORKING_AREA(waUnprivileged2, 512);

static thread_t *sb1tp, *sb2tp;

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(sdmon_ready ? 250 : 500);
  }
}

/*
 * SB termination event.
 */
static void SBHandler(eventid_t id) {

  (void)id;

  if (chThdTerminatedX(sb1tp)) {
    chprintf((BaseSequentialStream *)&SD2, "SB1 terminated\r\n");
  }

  if (chThdTerminatedX(sb2tp)) {
    chprintf((BaseSequentialStream *)&SD2, "SB2 terminated\r\n");
  }
}

/*
 * Application entry point.
 */
int main(void) {
  event_listener_t el0, el1, el2;
  vfs_node_c *np;
  msg_t ret;
  static const evhandler_t evhndl[] = {
    sdmonInsertHandler,
    sdmonRemoveHandler,
    SBHandler
  };

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - Virtual File System initialization.
   * - SandBox manager initialization.
   */
  halInit();
  chSysInit();
  vfsInit();
  sbHostInit();

  /*
   * Starting a serial ports for I/O, initializing other streams too.
   */
  sdStart(&SD2, NULL);
  sdStart(&LPSD1, NULL);
  nullObjectInit(&nullstream);

  /*
   * Activates the card insertion monitor.
   */
  sdmonInit();

  /*
   * Creating a blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+10, Thread1, NULL);

  /*
   * Initializing an overlay VFS object as a root on top of a FatFS driver.
   * This is accessible from kernel space and covers the whole file system.
   */
  drvFatFSObjectInit(&fatfs_driver);
  drvOverlayObjectInit(&root_overlay_driver, (vfs_driver_c *)&fatfs_driver, NULL);

  /*
   * Initializing overlay drivers for the two sandbox roots. Those also use
   * the FatFS driver but are restricted to "/sb1" and "/sb2" directories.
   */
  drvOverlayObjectInit(&sb1_root_overlay_driver, (vfs_driver_c *)&fatfs_driver, "sb1");
  drvOverlayObjectInit(&sb2_root_overlay_driver, (vfs_driver_c *)&fatfs_driver, "sb2");
  ret = drvOverlayRegisterDriver(&sb1_root_overlay_driver,
                                 drvStreamsObjectInit(&sb1_dev_driver, &sb1_streams[0]),
                                 "dev");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }
  ret = drvOverlayRegisterDriver(&sb2_root_overlay_driver,
                                 drvStreamsObjectInit(&sb2_dev_driver, &sb2_streams[0]),
                                 "dev");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }

  /*
   * Initializing overlay driver for the directory shared among the sandboxes.
   * It is seen as "/shared".
   */
  drvOverlayObjectInit(&sb_shared_overlay_driver, (vfs_driver_c *)&fatfs_driver, "shared");
  ret = drvOverlayRegisterDriver(&sb1_root_overlay_driver,
                                 (vfs_driver_c *)&sb_shared_overlay_driver,
                                 "shared");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }
  ret = drvOverlayRegisterDriver(&sb2_root_overlay_driver,
                                 (vfs_driver_c *)&sb_shared_overlay_driver,
                                 "shared");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }

  /*
   * Sandbox objects initialization.
   */
  sbObjectInit(&sbx1, &sb_config1);
  sbObjectInit(&sbx2, &sb_config2);

  /*
   * Associating standard input, output and error to sandbox 1.
   */
  ret = vfsDrvOpen((vfs_driver_c *)&sb1_root_overlay_driver,
                   "/dev/VSD1", VO_RDWR, &np);
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }
  sbPosixRegisterDescriptor(&sbx1, STDIN_FILENO, (vfs_node_c *)roAddRef(np));
  sbPosixRegisterDescriptor(&sbx1, STDOUT_FILENO, (vfs_node_c *)roAddRef(np));
  sbPosixRegisterDescriptor(&sbx1, STDERR_FILENO, (vfs_node_c *)roAddRef(np));
  vfsClose(np);

  /*
   * Associating standard input, output and error to sandbox 1.
   */
  ret = vfsDrvOpen((vfs_driver_c *)&sb2_root_overlay_driver,
                   "/dev/VSD1", VO_RDWR, &np);
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }
  sbPosixRegisterDescriptor(&sbx2, STDIN_FILENO, (vfs_node_c *)roAddRef(np));
  sbPosixRegisterDescriptor(&sbx2, STDOUT_FILENO, (vfs_node_c *)roAddRef(np));
  sbPosixRegisterDescriptor(&sbx2, STDERR_FILENO, (vfs_node_c *)roAddRef(np));
  vfsClose(np);

  /*
   * Creating **static** boxes using MPU.
   */
  mpuConfigureRegion(MPU_REGION_0,
                     0x08180000,
                     MPU_RASR_ATTR_AP_RO_RO |
                     MPU_RASR_ATTR_CACHEABLE_WT_NWA |
                     MPU_RASR_SIZE_512K |
                     MPU_RASR_ENABLE);
  mpuConfigureRegion(MPU_REGION_1,
                     0x20060000,
                     MPU_RASR_ATTR_AP_RW_RW |
                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                     MPU_RASR_SIZE_128K |
                     MPU_RASR_ENABLE);
  mpuConfigureRegion(MPU_REGION_2,
                     0x20080000,
                     MPU_RASR_ATTR_AP_RW_RW |
                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                     MPU_RASR_SIZE_128K |
                     MPU_RASR_ENABLE);

  /* Starting sandboxed thread 1.*/
  sb1tp = sbStartThread(&sbx1, "sbx1",
                        waUnprivileged1, sizeof (waUnprivileged1),
                        NORMALPRIO - 1);
  if (sb1tp == NULL) {
    chSysHalt("sbx1 failed");
  }

  /* Starting sandboxed thread 2.*/
  sb2tp = sbStartThread(&sbx2, "sbx2",
                        waUnprivileged2, sizeof (waUnprivileged2),
                        NORMALPRIO - 1);
  if (sb2tp == NULL) {
    chSysHalt("sbx2 failed");
  }

  /*
   * Listening to sandbox and card events.
   */
  chEvtRegister(&sdmon_inserted_event, &el0, (eventid_t)0);
  chEvtRegister(&sdmon_removed_event, &el1, (eventid_t)1);
  chEvtRegister(&sb.termination_es, &el2, (eventid_t)2);

  /*
   * Normal main() thread activity, in this demo it monitors the user button
   * and checks for sandboxes state.
   */
  while (true) {
    chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));

    /* Checking for user button, launching test suite if pressed.*/
    if (palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&SD2, &rt_test_suite);
      test_execute((BaseSequentialStream *)&SD2, &oslib_test_suite);
    }

#if 0
    if ((i & 1) == 0U) {
      if (!chThdTerminatedX(utp1)) {
        (void) sbSendMessageTimeout(&sbx1, (msg_t)i, TIME_MS2I(10));
      }
    }
    else {
      if (!chThdTerminatedX(utp2)) {
        (void) sbSendMessageTimeout(&sbx2, (msg_t)i, TIME_MS2I(10));
      }
    }
    i++;
#endif
  }
}
