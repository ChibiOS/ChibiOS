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
*/

/**
 * TMP75 is most simple I2C device in our case. It is already useful with
 * default settings after powerup.
 * You only must read 2 sequential bytes from it.
 */

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "tmp75.h"


/* input buffer */
static uint8_t tmp75_rx_data[TMP75_RX_DEPTH];

/* temperature value */
static int16_t temperature = 0;

static i2cflags_t errors = 0;

#define tmp75_addr 0b1001000

/* This is main function. */
void request_temperature(void){
  int16_t t_int = 0, t_frac = 0;
  msg_t status = RDY_OK;
  systime_t tmo = MS2ST(4);

  i2cAcquireBus(&I2CD1);
  status = i2cMasterReceiveTimeout(&I2CD1, tmp75_addr, tmp75_rx_data, 2, tmo);
  i2cReleaseBus(&I2CD1);

  if (status != RDY_OK){
    errors = i2cGetErrors(&I2CD1);
  }

  t_int = tmp75_rx_data[0] * 100;
  t_frac = (tmp75_rx_data[1] * 100) >> 8;
  temperature = t_int + t_frac;
}


