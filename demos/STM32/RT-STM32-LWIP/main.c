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

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "vfs.h"

#include "lwipthread.h"
#include "lwip/apps/httpd.h"

#include "httpd_vfs.h"
#include "http_romfs.h"
#include "portab.h"

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/* VFS ROMFS driver object exported as the root file system.*/
static vfs_rom_driver_c rom_driver;

/* Global pointer to the root VFS driver.*/
vfs_driver_c *vfs_root = (vfs_driver_c *)&rom_driver;

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 256);
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

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   * - Virtual File System initialization.
   * - lwIP subsystem initialization using the default configuration.
   */
  halInit();
  chSysInit();
  vfsInit();

  /*
   * Initializes the ROMFS image exposed through the HTTPD custom file hooks.
   */
  romdrvObjectInit(&rom_driver, &http_romfs);
  httpd_vfs_init();

  lwipInit(NULL);

  /*
   * Target-dependent setup code.
   */
  portab_setup();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Starts the HTTP server.
   */
  httpd_init();

  /*
   * Normal main() thread activity.
   */
  while (true) {

    chThdSleepMilliseconds(500);
  }
}
