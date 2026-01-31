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

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "vfs.h"

#include "chprintf.h"
#include "nullstreams.h"
#include "xshell.h"

#include "lfs.h"
#include "lfs_hal.h"

#include "hal_xsnor_micron_n25q.h"
#include "hal_xsnor_macronix_mx25.h"

#include "portab.h"

/*===========================================================================*/
/* XSNOR-related.                                                            */
/*===========================================================================*/

/* XSNOR configuration.*/
static union {
  hal_xsnor_micron_n25q_c           n25q;
  hal_xsnor_macronix_mx25_c         mx25;
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
  .options          = MX25_OPT_DUMMY_CYCLES(14) |   /* Very conservative.*/
                      MX25_OPT_USE_SUBSECTORS |
                      MX25_OPT_NICE_WAITING
};

/*===========================================================================*/
/* LittleFS-related.                                                         */
/*===========================================================================*/

/* LFS configuration.*/
static uint8_t __nocache_lfs_read_buffer[16];
static uint8_t __nocache_lfs_prog_buffer[16];
static uint8_t __nocache_lfs_lookahead_buffer[16];

static const hal_lfs_binding_t binding1 = {
  .base                 = 0,
  .flp                  = (BaseFlash *)&snor1.n25q.fls, /* Dirty trick, "fls" at same offset by design.*/
};

static const struct lfs_config lfscfg = {
    /* Link to the flash device driver.*/
    .context            = (void *)&binding1,

    /* Block device operations.*/
    .read               = __lfs_read,
    .prog               = __lfs_prog,
    .erase              = __lfs_erase,
    .sync               = __lfs_sync,
    .lock               = __lfs_lock,
    .unlock             = __lfs_unlock,

    /* Block device configuration.*/
    .read_size          = 16,
    .prog_size          = 16,
    .block_size         = 4096,
    .block_count        = 128,
    .block_cycles       = 500,
    .cache_size         = 16,
    .lookahead_size     = 16,
    .read_buffer        = __nocache_lfs_read_buffer,
    .prog_buffer        = __nocache_lfs_prog_buffer,
    .lookahead_buffer   = __nocache_lfs_lookahead_buffer,
    .name_max           = 0,
    .file_max           = 0,
    .attr_max           = 0,
    .metadata_max       = 0
};

/*===========================================================================*/
/* VFS-related.                                                              */
/*===========================================================================*/

/* VFS LFS driver object to be mounted as /.*/
static vfs_littlefs_driver_c lfs_driver;

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
  {"VSIOD1", (sequential_stream_i *)&PORTAB_SIOD1, VFS_MODE_S_IFCHR},
  {"null", (sequential_stream_i *)&nullstream, VFS_MODE_S_IFCHR},
  {NULL, NULL, 0}
};

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE       THD_STACK_SIZE(2048)

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

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_STACK(thd1_stack, 256);
static THD_FUNCTION(thd1_func, arg) {

  (void)arg;

  while (true) {
    palSetLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
    palClearLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  xshell_manager_t sm1;
  vfs_file_node_c *file1;
  msg_t msg;

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

  /* Board-dependent GPIO setup code.*/
  portab_setup();

  /*
   * Spawning a blinker thread.
   */
  static thread_t thd1;
  static const THD_DECL_STATIC(thd1_desc, "blinker", thd1_stack,
                               NORMALPRIO + 10, thd1_func, NULL, NULL);
  chThdSpawnRunning(&thd1, &thd1_desc);

  /* Trying N25Q.*/
  n25qObjectInit(&snor1.n25q);
  if (xsnorStart(&snor1.n25q, &snorcfg_n25q) != FLASH_NO_ERROR) {

    /* Trying MX25.*/
    mx25ObjectInit(&snor1.mx25);
    if (xsnorStart(&snor1.mx25, &snorcfg_mx25) != FLASH_NO_ERROR) {
      chSysHalt("device not found");
    }
  }

  /* Activates the SIO driver and a null stream.*/
  sioStart(&PORTAB_SIOD1, NULL);
  nullObjectInit(&nullstream);

  /* Initialization of the VFS LFS driver object.*/
  lfsdrvObjectInit(&lfs_driver, &lfscfg);

  /* Initialization of the VFS stream driver object.*/
  stmdrvObjectInit(&dev_driver, &streams[0]);

  /* Mounting the file system, if it fails then formatting and retrying.*/
  msg = lfsdrvMount(&lfs_driver);
  if (CH_RET_IS_ERROR(msg)) {
      msg = lfsdrvFormat(&lfs_driver);
      if (CH_RET_IS_ERROR(msg)) {
        chSysHalt("LFS format failed");
      }
      msg = lfsdrvMount(&lfs_driver);
      if (CH_RET_IS_ERROR(msg)) {
        chSysHalt("LFS mount failed");
      }
  }

  /* Initializing an overlay VFS object overlaying the LittleFS driver.*/
  ovldrvObjectInit(&root_overlay_driver, (vfs_driver_c *)&lfs_driver, NULL);

  /* Registering the streams VFS driver on the VFS overlay root as "/dev".*/
  msg = ovldrvRegisterDriver(&root_overlay_driver, (vfs_driver_c *)&dev_driver, "dev");
  if (CH_RET_IS_ERROR(msg)) {
    chSysHalt("VFS");
  }

  /* Opening a file for shell I/O.*/
  msg = vfsOpenFile("/dev/VSIOD1", VO_RDWR, &file1);
  if (CH_RET_IS_ERROR(msg)) {
    chSysHalt("VFS");
  }

  /* Shell manager initialization.*/
  xshellObjectInit(&sm1, &cfg1);

  /* Normal main() thread activity, spawning shells.*/
  while (true) {
    xshell_t *xshp = xshellSpawn(&sm1, (BaseSequentialStream *)vfsGetFileStream(file1),
                                 NORMALPRIO + 1, NULL);
    chThdWait(&xshp->thread);
    chThdSleepMilliseconds(500);
  }

  return 0;
}
