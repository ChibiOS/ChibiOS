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

#include <string.h>
#include "ch.h"
#include "hal.h"


#define SDC_DATA_DESTRUCTIVE_TEST   FALSE

#define SDC_BURST_SIZE      8
static uint8_t outbuf[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];
static uint8_t  inbuf[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];


/**
 * @brief   Clone of UNIX badblocks program.
 *
 * @param[in] start       first block to check
 * @param[in] end         last block to check
 * @param[in] blockatonce number of blocks to check at once
 * @param[in] pattern     check pattern
 *
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      read.
 * @retval TRUE         operation failed, the state of the buffer is uncertain.
 */
bool_t badblocks(uint32_t start, uint32_t end, uint32_t blockatonce, uint8_t pattern){
  uint32_t position = 0;
  uint32_t i = 0;

  chDbgCheck(blockatonce <= SDC_BURST_SIZE, "badblocks");

  /* fill control buffer */
  for (i=0; i < SDC_BLOCK_SIZE * blockatonce; i++)
    outbuf[i] = pattern;

  /* fill SD card with pattern. */
  position = start;
  while (position < end){
    if (sdcWrite(&SDCD1, position, outbuf, blockatonce))
      goto ERROR;
    position += blockatonce;
  }

  /* read and compare. */
  position = start;
  while (position < end){
    if (sdcRead(&SDCD1, position, inbuf, blockatonce))
      goto ERROR;
    if (memcmp(inbuf, outbuf, blockatonce * SDC_BLOCK_SIZE) != 0)
      goto ERROR;
    position += blockatonce;
  }
  return SDC_SUCCESS;

ERROR:
  return SDC_FAILED;
}

void clearbuffers(void){
  uint32_t i = 0;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    outbuf[i] = 0x55;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    inbuf[i] = 0x55;
}

/*
 * SDIO configuration.
 */
static const SDCConfig sdccfg = {
  0
};

/*
 * Application entry point.
 */
int main(void) {
  uint32_t i = 0;

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
  /*http://en.wikipedia.org/wiki/Secure_Digital#Storage_capacity_calculations*/

  if (!sdcConnect(&SDCD1)) {

    /* Single aligned read.*/

    /* Multiple aligned read from one place.*/
    clearbuffers();
    if (sdcRead(&SDCD1, 0, inbuf, SDC_BURST_SIZE))
      chSysHalt();
    for (i=0; i<1000; i++){
      if (sdcRead(&SDCD1, 0, outbuf, SDC_BURST_SIZE))
        chSysHalt();
      if (memcmp(inbuf, outbuf, SDC_BURST_SIZE * SDC_BLOCK_SIZE) != 0)
        chSysHalt();
    }

    /* Repeated multiple aligned reads.*/

    /* Repeated multiple unaligned reads.*/


#if SDC_DATA_DESTRUCTIVE_TEST
    clearbuffers();
    if (sdcWrite(&SDCD1, 0x10000, outbuf, SDC_BURST_SIZE))
      chSysHalt();

//      if(badblocks(0x10000, 0x11000, SDC_BURST_SIZE, 0xAA))
//        chSysHalt();
#endif /* !SDC_DATA_DESTRUCTIVE_TEST */

    if (sdcDisconnect(&SDCD1))
      chSysHalt();
  }
  else{
    chSysHalt();
  }
  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    palTogglePad(GPIOB, GPIOB_LED_R);
    chThdSleepMilliseconds(500);
  }
}
