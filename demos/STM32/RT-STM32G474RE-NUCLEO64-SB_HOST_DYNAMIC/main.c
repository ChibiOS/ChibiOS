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

/*===========================================================================*/
/* VFS-related.                                                              */
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
  .mpuregs          = {
    [0] = {
      (uint32_t)STARTUP_FLASH1_BASE, MPU_RASR_ATTR_AP_RO_RO |
                                     MPU_RASR_ATTR_CACHEABLE_WT_NWA |
                                     MPU_RASR_SIZE_32K |
                                     MPU_RASR_ENABLE
    },
    [1] = {
      (uint32_t)STARTUP_RAM1_BASE,   MPU_RASR_ATTR_AP_RW_RW |
                                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                                     MPU_RASR_SIZE_4K |
                                     MPU_RASR_ENABLE
    }
  },
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
  .mpuregs          = {
    [0] = {
      (uint32_t)STARTUP_FLASH2_BASE, MPU_RASR_ATTR_AP_RO_RO |
                                     MPU_RASR_ATTR_CACHEABLE_WT_NWA |
                                     MPU_RASR_SIZE_32K |
                                     MPU_RASR_ENABLE
    },
    [1] = {
      (uint32_t)STARTUP_RAM2_BASE,   MPU_RASR_ATTR_AP_RW_RW |
                                     MPU_RASR_ATTR_CACHEABLE_WB_WA |
                                     MPU_RASR_SIZE_4K |
                                     MPU_RASR_ENABLE
    }
  },
};

/* Sandbox objects.*/
sb_class_t sbx1, sbx2;

static THD_WORKING_AREA(waUnprivileged1, 512);
static THD_WORKING_AREA(waUnprivileged2, 512);

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
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  unsigned i = 1U;
  thread_t *utp1, *utp2;
  event_listener_t el1;
  vfs_node_c *np;
  msg_t ret;

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
   * Starting a serial port for I/O, initializing other streams too.
   */
  sdStart(&LPSD1, NULL);
  nullObjectInit(&nullstream);

  /*
   * Creating a blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+10, Thread1, NULL);

  /*
   * Initializing an overlay VFS object as a root, no overlaid driver,
   * registering a streams VFS driver on the VFS overlay root as "/dev".
   */
  drvOverlayObjectInit(&root_overlay_driver, NULL, NULL);
  ret = drvOverlayRegisterDriver(&root_overlay_driver,
                                 drvStreamsObjectInit(&dev_driver, &streams[0]),
                                 "dev");
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }

  /*
   * Sandbox objects initialization.
   */
  sbObjectInit(&sbx1, &sb_config1);
  sbObjectInit(&sbx2, &sb_config2);

  /*
   * Associating standard input, output and error to sandboxes. Both sandboxes
   * use the same serial port in this setup.
   */
  ret = vfsOpen("/dev/VSD1", 0, &np);
  if (CH_RET_IS_ERROR(ret)) {
    chSysHalt("VFS");
  }
  sbRegisterDescriptor(&sbx1, STDIN_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx1, STDOUT_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx1, STDERR_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx2, STDIN_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx2, STDOUT_FILENO, (vfs_node_c *)roAddRef(np));
  sbRegisterDescriptor(&sbx2, STDERR_FILENO, (vfs_node_c *)roAddRef(np));
  vfsClose(np);

  /* Starting sandboxed thread 1.*/
  utp1 = sbStartThread(&sbx1, "sbx1",
                       waUnprivileged1, sizeof (waUnprivileged1),
                       NORMALPRIO - 1, NULL, NULL);
  if (utp1 == NULL) {
    chSysHalt("sbx1 failed");
  }

  /* Starting sandboxed thread 2.*/
  utp2 = sbStartThread(&sbx2, "sbx2",
                       waUnprivileged2, sizeof (waUnprivileged2),
                       NORMALPRIO - 1, NULL, NULL);
  if (utp1 == NULL) {
    chSysHalt("sbx2 failed");
  }

  /*
   * Listening to sandbox events.
   */
  chEvtRegister(&sb.termination_es, &el1, (eventid_t)0);

  /*
   * Normal main() thread activity, in this demo it monitors the user button
   * and checks for sandboxes state.
   */
  while (true) {

    /* Checking for user button, launching test suite if pressed.*/
    if (palReadLine(LINE_BUTTON)) {
      test_execute((BaseSequentialStream *)&LPSD1, &rt_test_suite);
      test_execute((BaseSequentialStream *)&LPSD1, &oslib_test_suite);
    }

    /* Waiting for a sandbox event or timeout.*/
    if (chEvtWaitOneTimeout(ALL_EVENTS, TIME_MS2I(500)) != (eventmask_t)0) {

      if (chThdTerminatedX(utp1)) {
        chprintf((BaseSequentialStream *)&LPSD1, "SB1 terminated\r\n");
      }

      if (chThdTerminatedX(utp2)) {
        chprintf((BaseSequentialStream *)&LPSD1, "SB2 terminated\r\n");
      }
    }

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
  }
}
