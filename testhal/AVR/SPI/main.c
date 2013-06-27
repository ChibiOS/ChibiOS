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

#include "ch.h"
#include "hal.h"
#include <util/delay.h>
#include "chprintf.h"

// #define SPI_ROLE SPI_ROLE_SLAVE
#define SPI_ROLE SPI_ROLE_MASTER
#if (SPI_ROLE == SPI_ROLE_MASTER)
	#define ROLE "Master"
    char msg[] = "Message from master\r\n";
#else
	#define ROLE "Slave"
    char msg[] = "Message from slave \r\n";
#endif

void spicallback(SPIDriver *spip){
  chSequentialStreamPut(&SD1, '.');
  spip->txbuf = 0; // terminating null
}

/*
 * Application entry point.
 */
int main(void) {

/*
 * SPI interface configuration
 */
  static SPIConfig spicfg = {
    SPI_ROLE,
    IOPORT2,
    SPI1_SS,
    SPI_MODE_0,
    SPI_MSB_FIRST,
    SPI_SCK_FOSC_128,
    spicallback
  };
 
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */

  halInit();
  chSysInit();

  spiStart(&SPID1, &spicfg);
  sdStart(&SD1, NULL);

  chprintf((BaseSequentialStream *) &SD1, "Start %s\r\n", ROLE);

  while(1) {
    char buf[24];

#if (SPI_ROLE == SPI_ROLE_MASTER)
    spiSelect(&SPID1);
    spiExchange(&SPID1, sizeof(msg), msg, buf);
    spiUnselect(&SPID1);

    chprintf((BaseSequentialStream *) &SD1,"%s: %s\r\n", ROLE, buf);      
    chThdSleepMilliseconds(500);
#else
    spiExchange(&SPID1, sizeof(msg), msg, buf);
    chprintf((BaseSequentialStream *) &SD1,"%s: %s\r\n", ROLE, buf);
#endif

  }
}
