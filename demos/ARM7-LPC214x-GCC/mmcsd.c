/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#include <ch.h>

#include "lpc214x.h"
#include "lpc214x_ssp.h"

#include "mmcsd.h"

static EventSource MMCInsertEventSource;

void MMCInit(void) {

  chEvtInit(&MMCInsertEventSource);
}

/*
 * Initializes a card after the power up by selecting the SPI mode.
 */
BOOL mmcInit(void) {

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
    chThdSleep(10);
    if (mmcSendCommand(0, 0) == 0x01)
      break;
    if (++i >= CMD0_RETRY)
      return TRUE;
  }

  /*
   * Initialization.
   */
  i = 0;
  while (TRUE) {
    BYTE8 b;
    chThdSleep(10);
    b = mmcSendCommand(0, 0);
    if (b == 0x00)
      break;
    if (b != 0x01)
      return TRUE;
    if (++i >= CMD1_RETRY)
      return TRUE;
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
BYTE8 mmcSendCommand(BYTE8 cmd, ULONG32 arg) {
  BYTE8 buf[6];

  buf[0] = 0x40 | cmd;
  buf[1] = arg >> 24;
  buf[2] = arg >> 16;
  buf[3] = arg >> 8;
  buf[4] = arg;
  buf[5] = 0x95; /* Valid for CMD0 ingnored by other commands. */
  sspAcquireBus();
  sspRW(NULL, buf, 6);
  sspRW(buf, NULL, 1);
  sspReleaseBus();
  return buf[0];
}
