/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <ch.h>

#include "lpc214x.h"
#include "lpc214x_ssp.h"

#include "mmcsd.h"

EventSource MMCInsertEventSource, MMCRemoveEventSource;

static VirtualTimer vt;
static int cnt;

/*
 * Subsystem initialization.
 */
void InitMMC(void) {

  chEvtInit(&MMCInsertEventSource);
  chEvtInit(&MMCRemoveEventSource);
  cnt = POLLING_INTERVAL;
}

void tmrfunc(void *par) {

  if (cnt) {
    if (!(IO1PIN & (1 << 25))) {
      if (!--cnt)
        chEvtBroadcastI(&MMCInsertEventSource);
    }
    else
      cnt = POLLING_INTERVAL;
  }
  else {
    if (IO1PIN & (1 << 25)) {
      cnt = POLLING_INTERVAL;
      chEvtBroadcastI(&MMCRemoveEventSource);
    }
  }
  chVTSetI(&vt, 10, tmrfunc, NULL);
}

/*
 * Starts the card polling service.
 */
void mmcStartPolling(void) {

  chSysLock();

  if (!chVTIsArmedI(&vt)) {
    chVTSetI(&vt, 10, tmrfunc, NULL);
    cnt = POLLING_INTERVAL;
  }

  chSysUnlock();
}

/*
 * Stops the card polling service.
 */
void mmcStopPolling(void) {

  chSysLock();

  if (chVTIsArmedI(&vt)) {
    chVTResetI(&vt);
    cnt = POLLING_INTERVAL;
  }

  chSysUnlock();
}

/*
 * Returns TRUE if the card is safely inserted in the reader.
 */
bool_t mmcCardInserted (void) {

  return cnt == 0;
}

static void wait(void) {
  int i;
  uint8_t buf[4];

  for (i = 0; i < 16; i++) {
    sspRW(buf, NULL, 1);
    if (buf[0] == 0xFF)
      break;
  }
  /* Looks like it is a loooong wait.*/
  while (TRUE) {
    sspRW(buf, NULL, 1);
    if (buf[0] == 0xFF)
      break;
#ifdef NICE_WAITING
    chThdSleep(1);      /* Trying to be nice with the other threads.*/
#endif
  }
}

static void sendhdr(uint8_t cmd, uint32_t arg) {
  uint8_t buf[6];

  /*
   * Wait for the bus to become idle if a write operation was in progress.
   */
  wait();

  buf[0] = 0x40 | cmd;
  buf[1] = arg >> 24;
  buf[2] = arg >> 16;
  buf[3] = arg >> 8;
  buf[4] = arg;
  buf[5] = 0x95; /* Valid for CMD0 ingnored by other commands. */
  sspRW(NULL, buf, 6);
}

static uint8_t recvr1(void) {
  int i;
  uint8_t r1[1];

  for (i = 0; i < 9; i++) {
    sspRW(r1, NULL, 1);
    if (r1[0] != 0xFF)
      return r1[0];
  }
  return 0xFF;                  /* Timeout.*/
}

static bool_t getdata(uint8_t *buf, uint32_t n) {
  int i;

  for (i = 0; i < MMC_WAIT_DATA; i++) {
    sspRW(buf, NULL, 1);
    if (buf[0] == 0xFE) {
      sspRW(buf, NULL, n);
      sspRW(NULL, NULL, 2);     /* CRC ignored.*/
      return FALSE;
    }
  }
  return TRUE;                  /* Timeout.*/
}

/*
 * Initializes a card after the power up by selecting the SPI mode.
 */
bool_t mmcInit(void) {

  /*
   * Starting initialization with slow clock mode.
   */
  SetSSP(254, CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0), 0);

  /*
   * SPI mode selection.
   */
  sspRW(NULL, NULL, 16);        /* 128 clock pulses without ~CS asserted. */
  int i = 0;
  while (TRUE) {
    if (mmcSendCommand(CMDGOIDLE, 0) == 0x01)
      break;
    if (++i >= CMD0_RETRY)
      return TRUE;
    chThdSleep(10);
  }

  /*
   * Initialization.
   */
  i = 0;
  while (TRUE) {
    uint8_t b = mmcSendCommand(CMDINIT, 0);
    if (b == 0x00)
      break;
    if (b != 0x01)
      return TRUE;
    if (++i >= CMD1_RETRY)
      return TRUE;
    chThdSleep(10);
  }

  /*
   * Full speed.
   */
  SetSSP(2, CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0), 0);
  return FALSE;
}

/*
 * Sends a simple command and returns a R1-type response.
 */
uint8_t mmcSendCommand(uint8_t cmd, uint32_t arg) {
  uint8_t r1;

  sspAcquireBus();
  sendhdr(cmd, arg);
  r1 = recvr1();
  sspReleaseBus();
  return r1;
}

/*
 * Reads the card info record.
 * @param data the pointer to a \p MMCCSD structure
 * @return \p TRUE if an error happened
 */
bool_t mmcGetSize(MMCCSD *data) {
  uint8_t buf[16];

  sspAcquireBus();
  sendhdr(CMDREADCSD, 0);
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  if (getdata(buf, 16)) {
    sspReleaseBus();
    return TRUE;
  }
  sspReleaseBus();

  /* csize * multiplier */
  data->csize    = (((buf[6] & 3) << 10) | (buf[7] << 2) | (buf[8] >> 6)) *
                   (1 << (2 + (((buf[9] & 3) << 1) | (buf[10] >> 7))));
  data->rdblklen = 1 << (buf[5] & 15);
  return FALSE;
}

/*
 * Reads a block.
 * @param blknum the block number
 * @param buf the pointer to the read buffer
 * @return \p TRUE if an error happened
 */
bool_t mmcRead(uint8_t *buf, uint32_t blknum) {

  sspAcquireBus();
  sendhdr(CMDREAD, blknum << 9);
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  if (getdata(buf, 512)) {
    sspReleaseBus();
    return TRUE;
  }
  sspReleaseBus();
  return FALSE;
}

/*
 * Reads multiple blocks.
 * @param blknum the initial block
 * @param n the number of blocks
 * @param buf the pointer to the read buffer
 * @return \p TRUE if an error happened
 */
bool_t mmcReadMultiple(uint8_t *buf, uint32_t blknum, uint32_t n) {
  static const uint8_t stopcmd[] = {0x40 | CMDSTOP, 0, 0, 0, 0, 1, 0xFF};

  sspAcquireBus();
  sendhdr(CMDREADMULTIPLE, blknum << 9);
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  while (n) {
    if (getdata(buf, 512)) {
      sspReleaseBus();
      return TRUE;
    }
    buf += 512;
    n--;
  }
  sspRW(NULL, (uint8_t *)stopcmd, sizeof(stopcmd));
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  sspReleaseBus();
  return FALSE;
}

/*
 * Writes a block.
 * @param blknum the block number
 * @param buf the pointer to the write buffer
 * @return \p TRUE if an error happened
 * @note The function DOES NOT wait for the SPI bus to become free after
 *       sending the data, the bus check is done before sending commands to
 *       the card, this allows to not make useless busy waiting. The invoking
 *       thread can do other things while the data is being written.
 */
bool_t mmcWrite(uint8_t *buf, uint32_t blknum) {
  static const uint8_t start[] = {0xFF, 0xFE};
  uint8_t b[4];

  sspAcquireBus();
  sendhdr(CMDWRITE, blknum << 9);
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  sspRW(NULL, (uint8_t *)start, 2);       /* Data prologue.*/
  sspRW(NULL, buf, 512);                /* Data.*/
  sspRW(NULL, NULL, 2);                 /* CRC ignored in this version.*/
  sspRW(b, NULL, 1);
  sspReleaseBus();
  if ((b[0] & 0x1F) != 0x05)
    return TRUE;
  return FALSE;
}

/*
 * Writes multiple blocks.
 * @param blknum the initial block
 * @param n the number of blocks
 * @param buf the pointer to the write buffer
 * @return \p TRUE if an error happened
 * @note The function DOES NOT wait for the SPI bus to become free after
 *       sending the data, the bus check is done before sending commands to
 *       the card, this allows to not make useless busy waiting. The invoking
 *       thread can do other things while the data is being written.
 */
bool_t mmcWriteMultiple(uint8_t *buf, uint32_t blknum, uint32_t n) {
  static const uint8_t start[] = {0xFF, 0xFC},
                     stop[] = {0xFD, 0xFF};
  uint8_t b[4];

  sspAcquireBus();
  sendhdr(CMDWRITEMULTIPLE, blknum << 9);
  if (recvr1() != 0x00) {
    sspReleaseBus();
    return TRUE;
  }
  while (n) {
    sspRW(NULL, (uint8_t *)start, sizeof(start)); /* Data prologue.*/
    sspRW(NULL, buf, 512);                /* Data.*/
    sspRW(NULL, NULL, 2);                 /* CRC ignored in this version.*/
    sspRW(b, NULL, 1);
    if ((b[0] & 0x1F) != 0x05) {
      sspReleaseBus();
      return TRUE;
    }
    wait();
    buf += 512;
    n--;
  }
  sspRW(NULL, (uint8_t *)stop, sizeof(stop)); /* Stops the transfer.*/
  sspReleaseBus();
  return FALSE;
}

/*
 * Makes sure that pending operations are completed before returning.
 */
void mmcSynch(void) {
  uint8_t buf[4];

  sspAcquireBus();
  while (TRUE) {
    sspRW(buf, NULL, 1);
    if (buf[0] == 0xFF)
      break;
#ifdef NICE_WAITING
    chThdSleep(1);      /* Trying to be nice with the other threads.*/
#endif
  }
  sspReleaseBus();
}
