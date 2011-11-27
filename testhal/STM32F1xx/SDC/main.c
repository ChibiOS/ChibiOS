/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

/*
 * SDIO configuration.
 */
static const SDCConfig sdccfg = {
  0
};

static uint8_t blkbuf[SDC_BLOCK_SIZE * 4 + 1];

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
   */
  halInit();
  chSysInit();

  /*
   * Initializes the SDIO drivers.
   */
  sdcStart(&SDCD1, &sdccfg);
  if (!sdcConnect(&SDCD1)) {
    int i;

    /* Single aligned read.*/
    if (sdcRead(&SDCD1, 0, blkbuf, 1))
      chSysHalt();

    /* Single unaligned read.*/
    if (sdcRead(&SDCD1, 0, blkbuf + 1, 1))
      chSysHalt();

    /* Multiple aligned read.*/
    if (sdcRead(&SDCD1, 0, blkbuf, 4))
      chSysHalt();

    /* Multiple unaligned read.*/
    if (sdcRead(&SDCD1, 0, blkbuf + 1, 4))
      chSysHalt();

    /* Repeated multiple aligned reads.*/
    for (i = 0; i < 1000; i++) {
      if (sdcRead(&SDCD1, 0, blkbuf, 4))
        chSysHalt();
    }

    /* Repeated multiple unaligned reads.*/
    for (i = 0; i < 1000; i++) {
      if (sdcRead(&SDCD1, 0, blkbuf + 1, 4))
        chSysHalt();
    }

    /* Repeated multiple aligned writes.*/
    for (i = 0; i < 100; i++) {
      if (sdcRead(&SDCD1, 0x10000, blkbuf, 4))
        chSysHalt();
      if (sdcWrite(&SDCD1, 0x10000, blkbuf, 4))
        chSysHalt();
      if (sdcWrite(&SDCD1, 0x10000, blkbuf, 4))
        chSysHalt();
    }

    /* Repeated multiple unaligned writes.*/
    for (i = 0; i < 100; i++) {
      if (sdcRead(&SDCD1, 0x10000, blkbuf + 1, 4))
        chSysHalt();
      if (sdcWrite(&SDCD1, 0x10000, blkbuf + 1, 4))
        chSysHalt();
      if (sdcWrite(&SDCD1, 0x10000, blkbuf + 1, 4))
        chSysHalt();
    }

    if (sdcDisconnect(&SDCD1))
      chSysHalt();
  }

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
}
