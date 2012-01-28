/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/**
 * Not responding slave test
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "fake.h"


/* input buffer */
static uint8_t rx_data[2];

/* temperature value */
static int16_t temperature = 0;

static i2cflags_t errors = 0;

#define addr 0b1001100

/* This is main function. */
void request_fake(void){
  msg_t status = RDY_OK;
  systime_t tmo = MS2ST(4);

  i2cAcquireBus(&I2CD1);
  status = i2cMasterReceiveTimeout(&I2CD1, addr, rx_data, 2, tmo);
  i2cReleaseBus(&I2CD1);

  if (status == RDY_RESET){
    errors = i2cGetErrors(&I2CD1);
    if (errors == I2CD_ACK_FAILURE){
      /* there is no slave with given address on the bus, or it was die */
      return;
    }
  }

  else{
    temperature = (rx_data[0] << 8) + rx_data[1];
  }
}


