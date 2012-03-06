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


#define SDC_DATA_DESTRUCTIVE_TEST   TRUE

#define SDC_BURST_SIZE      8 /* how many sectors reads at once */
static uint8_t outbuf[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];
static uint8_t  inbuf[SDC_BLOCK_SIZE * SDC_BURST_SIZE + 1];


/**
 * @brief   Parody of UNIX badblocks program.
 *
 * @param[in] start       first block to check
 * @param[in] end         last block to check
 * @param[in] blockatonce number of blocks to check at once
 * @param[in] pattern     check pattern
 *
 * @return              The operation status.
 * @retval SDC_SUCCESS  operation succeeded, the requested blocks have been
 *                      read.
 * @retval SDC_FAILED   operation failed, the state of the buffer is uncertain.
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

/**
 *
 */
void fillbuffer(uint8_t pattern, uint8_t *b){
  uint32_t i = 0;
  for (i=0; i < SDC_BLOCK_SIZE * SDC_BURST_SIZE; i++)
    b[i] = pattern;
}

/**
 *
 */
void fillbuffers(uint8_t pattern){
  fillbuffer(pattern, inbuf);
  fillbuffer(pattern, outbuf);
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

  halInit();
  chSysInit();

  /*
   * Initializes the SDIO drivers.
   */
  sdcStart(&SDCD1, &sdccfg);

  if (!sdcConnect(&SDCD1)) {

    /* Single aligned read.*/
    if (sdcRead(&SDCD1, 0, inbuf, 1))
      chSysHalt();

    /* Single unaligned read.*/
    if (sdcRead(&SDCD1, 0, inbuf + 1, 1))
      chSysHalt();
    if (sdcRead(&SDCD1, 0, inbuf + 2, 1))
      chSysHalt();
    if (sdcRead(&SDCD1, 0, inbuf + 3, 1))
      chSysHalt();

    /* Multiple aligned reads from one place to ensure in bus stability */
    fillbuffers(0x55);
    /* fill reference buffer from SD card */
    if (sdcRead(&SDCD1, 0, inbuf, SDC_BURST_SIZE))
      chSysHalt();
    for (i=0; i<1000; i++){
      if (sdcRead(&SDCD1, 0, outbuf, SDC_BURST_SIZE))
        chSysHalt();
      if (memcmp(inbuf, outbuf, SDC_BURST_SIZE * SDC_BLOCK_SIZE) != 0)
        chSysHalt();
    }

    /* Repeated multiple unaligned reads.*/
    fillbuffers(0x55);
    /* fill reference buffer from SD card */
    if (sdcRead(&SDCD1, 0, inbuf + 1, SDC_BURST_SIZE))
      chSysHalt();
    for (i=0; i<1000; i++){
      if (sdcRead(&SDCD1, 0, outbuf + 1, SDC_BURST_SIZE))
        chSysHalt();
      if (memcmp(inbuf, outbuf, SDC_BURST_SIZE * SDC_BLOCK_SIZE) != 0)
        chSysHalt();
    }







#if SDC_DATA_DESTRUCTIVE_TEST

    /* Single aligned write.*/
    fillbuffer(0xAA, inbuf);
    if (sdcWrite(&SDCD1, 0, inbuf, 1))
      chSysHalt();
    fillbuffer(0, outbuf);
    if (sdcRead(&SDCD1, 0, outbuf, 1))
      chSysHalt();
    if (memcmp(inbuf, outbuf, SDC_BLOCK_SIZE) != 0)
      chSysHalt();

    /* Single unaligned write.*/
    fillbuffer(0xFF, inbuf);
    if (sdcWrite(&SDCD1, 0, inbuf+1, 1))
      chSysHalt();
    fillbuffer(0, outbuf);
    if (sdcRead(&SDCD1, 0, outbuf+1, 1))
      chSysHalt();
    if (memcmp(inbuf+1, outbuf+1, SDC_BLOCK_SIZE) != 0)
      chSysHalt();


    if(badblocks(0x10000, 0x11000, SDC_BURST_SIZE, 0xAA))
      chSysHalt();
#endif /* !SDC_DATA_DESTRUCTIVE_TEST */







    if (sdcDisconnect(&SDCD1))
      chSysHalt();
  }
  else{
    chSysHalt();
  }

  /*
   * Normal main() thread activity.
   * Blinking signaling about successful passing.
   */
  while (TRUE) {
    palTogglePad(GPIOB, GPIOB_LED_R);
    chThdSleepMilliseconds(500);
  }
}
