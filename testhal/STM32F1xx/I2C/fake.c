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


#define addr 0b1001100

/* This is main function. */
void request_fake(void){
  i2cflags_t errors = 0;

  i2cAcquireBus(&I2CD1);
  errors = i2cMasterReceive(&I2CD1, addr, rx_data, 2);
  i2cReleaseBus(&I2CD1);

  if (errors == I2CD_ACK_FAILURE){
    __NOP();
  }
  else{
    temperature = (rx_data[0] << 8) + rx_data[1];
  }
}


