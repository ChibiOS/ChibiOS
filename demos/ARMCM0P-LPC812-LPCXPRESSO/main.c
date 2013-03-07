/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
#include "test.h"

/*
 * RGB LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);

static msg_t Thread1(void *arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  
  palTogglePort(GPIO0, PAL_PORT_BIT(LED_GREEN));

  while (TRUE) {
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_GREEN));
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_RED));
    chThdSleepMilliseconds(500);
    
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_RED));
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_BLUE));
    chThdSleepMilliseconds(500);
    
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_BLUE));
    palTogglePort(GPIO0, PAL_PORT_BIT(LED_GREEN));
    chThdSleepMilliseconds(500);
  }
  
  return 0;
}

int main(void){

  halInit();
  chSysInit();

  sdStart(&SD1, NULL);                  /* Default: 9600,8,N,1. */

  chThdCreateStatic(waThread1, sizeof(waThread1),
                    NORMALPRIO, Thread1, NULL);

  chnWrite( &SD1, (const uint8_t *)"\nhello\n", 8 ); 

  do {
    chThdSleepMilliseconds(500);
  } while (TRUE);
  
  return 0;
}

