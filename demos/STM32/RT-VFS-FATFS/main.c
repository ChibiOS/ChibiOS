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
#include "drvoverlay.h"
#include "drvstreams.h"

#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "nullstreams.h"
#include "chprintf.h"
#include "shell.h"

#include "portab.h"

/*===========================================================================*/
/* Card insertion monitor.                                                   */
/*===========================================================================*/

static event_source_t inserted_event, removed_event;

/*===========================================================================*/
/* VFS related.                                                              */
/*===========================================================================*/

/* VFS overlay driver object representing the root directory.*/
static vfs_overlay_driver_t vfs_root;

/* VFS streams driver object representing the /dev directory.*/
static vfs_streams_driver_t vfs_dev;

static NullStream nullstream;

/* Stream to be exposed under /dev as files.*/
static const drv_stream_element_t streams[] = {
  {"VSD1", (BaseSequentialStream *)&PORTAB_SD1},
  {"null", (BaseSequentialStream *)&nullstream},
  {NULL, NULL}
};

/* Generic large buffer.*/
static char pathbuf[1024];

static msg_t scan_nodes(BaseSequentialStream *chp, char *path) {
  msg_t res;
  vfs_directory_node_t *dirp;
  static vfs_node_info_t ni;

  chprintf(chp, "%s\r\n", path);
  res = vfsOpenDirectory((vfs_driver_t *)&vfs_root, path, &dirp);
  if (res == VFS_RET_SUCCESS) {
    size_t i = strlen(path);

    while (true) {
      char *fn = ni.name;
      res = vfsReadDirectoryNext(dirp, &ni);
      if (res != VFS_RET_SUCCESS) {
        break;
      }

      fn = ni.name;
      if (ni.attr & VFS_NODE_ATTR_ISDIR) {
        strcat(path + i, fn);
        strcat(path + i, "/");
        res = scan_nodes(chp, path);
        if (res != VFS_RET_SUCCESS) {
          break;
        }
      }
      else {
        chprintf(chp, "%s%s\r\n", path, fn);
      }
    }

    vfsCloseDirectory(dirp);
  }

  return res;
}

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static void cmd_tree(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;

  if (argc > 0) {
    chprintf(chp, "Usage: tree\r\n");
    return;
  }

  strcpy(pathbuf, "/");
  scan_nodes(chp, (char *)pathbuf);
}

static const ShellCommand commands[] = {
  {"tree", cmd_tree},
  {NULL, NULL}
};

static ShellConfig shell_cfg1 = {
  NULL,
  commands
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

#if defined(DEMO_USE_FATFS)
#if HAL_USE_SDC
  if (sdcConnect(&PORTAB_SDCD1)) {
    return;
  }
#else
  if (mmcConnect(&MMCD1)) {
    return;
  }
#endif
#endif
}

/*
 * Card removal event.
 */
static void RemoveHandler(eventid_t id) {

  (void)id;

#if defined(DEMO_USE_FATFS)
#if HAL_USE_SDC
    sdcDisconnect(&PORTAB_SDCD1);
#else
    mmcDisconnect(&MMCD1);
#endif
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
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  msg_t msg;
  vfs_file_node_t *file;
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
   * - Shell manager initialization.
   */
  halInit();
  chSysInit();
  shellInit();

  /* Board-dependent setup code.*/
  portab_setup();

  /* Starting a serial port for the shell, initializing other streams too.*/
  sdStart(&PORTAB_SD1, NULL);
  nullObjectInit(&nullstream);

  /* Initializing an overlay VFS object as a root, no need for a name.*/
  drvOverlayObjectInit(&vfs_root, "");

  /* Registering a streams VFS driver on the VFS overlay root as "/dev".*/
  msg = drvOverlayRegisterDriver(&vfs_root,
                                 drvStreamsObjectInit(&vfs_dev,
                                                      "dev",
                                                      &streams[0]));
  if (msg != VFS_RET_SUCCESS) {
    chSysHalt("VFS");
  }

  /* Opening a file for shell I/O.*/
  msg = vfsOpenFile((vfs_driver_t *)&vfs_root,
                    "/dev/VSD1",
                    MODE_OPEN | MODE_RDWR,
                    &file);
  if (msg != VFS_RET_SUCCESS) {
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
