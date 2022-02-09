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

//#include "rt_test_root.h"
//#include "oslib_test_root.h"

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

/* VFS streams driver objects representing the /dev private directories.*/
static vfs_streams_driver_c sb1_dev_driver;

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

/*===========================================================================*/
/* SB-related.                                                               */
/*===========================================================================*/

/* Sandbox 1 configuration.*/
static const sb_config_t sb_config1 = {
  .code_region      = 0U,
  .data_region      = 0U,
  .regions          = {
    [0] = {
      .area         = {STARTUP_RAM1_BASE,   STARTUP_RAM1_SIZE},
      .used         = true,
      .writeable    = true
    },
    [1] = {
      .area         = {NULL,                (size_t)0},
      .used         = false,
      .writeable    = false,
    }
  },
  .mpuregs          = {
    [0] = {
      (uint32_t)STARTUP_RAM1_BASE,   MPU_RASR_ATTR_AP_RW_RW |
                                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                                     MPU_RASR_SIZE_128K |
                                     MPU_RASR_ENABLE
    },
    [1] = {
      (uint32_t)0,                   (uint32_t)0
    }
  },
  .vfs_driver       = (vfs_driver_c *)&sb1_root_overlay_driver
};

/* Sandbox objects.*/
sb_class_t sbx1;

static const char *sbx1_argv[] = {
  "msh",
  NULL
};

static const char *sbx1_envp[] = {
  "PATH=/bin",
  "PROMPT=sb1> ",
  "HOME=/",
  NULL
};

static THD_WORKING_AREA(waUnprivileged1, 2048);

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

  if (!sbIsThreadRunningX(&sbx1)) {
    msg_t msg = sbWaitThread(&sbx1);

    chprintf((BaseSequentialStream *)&SD2, "SB1 terminated (%08lx)\r\n", msg);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  event_listener_t elsb;
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
  drvOverlayObjectInit(&sb1_root_overlay_driver, (vfs_driver_c *)&fatfs_driver, "/sb1");
  ret = drvOverlayRegisterDriver(&sb1_root_overlay_driver,
                                 drvStreamsObjectInit(&sb1_dev_driver, &sb1_streams[0]),
                                 "dev");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }

  /*
   * Sandbox objects initialization.
   */
  sbObjectInit(&sbx1, &sb_config1);

  /*
   * Listening to sandbox events.
   */
  chEvtRegister(&sb.termination_es, &elsb, (eventid_t)2);

  /*
   * Normal main() thread activity, in this demo it monitors the user button
   * and checks for sandboxes state.
   */
  while (true) {
    chEvtDispatch(evhndl, chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)));

    if (sdmon_ready && !sbIsThreadRunningX(&sbx1)) {
      chThdSleepMilliseconds(1000);

      /*
       * Associating standard input, output and error to sandbox 1.
       */
      ret = vfsDrvOpen((vfs_driver_c *)&sb1_root_overlay_driver,
                       "/dev/VSD1", VO_RDWR, &np);
      if (CH_RET_IS_ERROR(ret)) {
        chprintf((BaseSequentialStream *)&SD2, "Opening /dev/VSD1 failed (%08lx)\r\n", ret);
        continue;
      }
      sbRegisterDescriptor(&sbx1, STDIN_FILENO, (vfs_node_c *)roAddRef(np));
      sbRegisterDescriptor(&sbx1, STDOUT_FILENO, (vfs_node_c *)roAddRef(np));
      sbRegisterDescriptor(&sbx1, STDERR_FILENO, (vfs_node_c *)roAddRef(np));
      vfsClose(np);

      /*
       * Running the sandbox.
       */
      ret = sbExec(&sbx1, "/bin/msh.elf",
                   waUnprivileged1, sizeof (waUnprivileged1), NORMALPRIO - 1,
                   sbx1_argv, sbx1_envp);
      if (CH_RET_IS_ERROR(ret)) {
        chprintf((BaseSequentialStream *)&SD2, "SBX1 launch failed (%08lx)\r\n", ret);
      }
    }
  }
}
