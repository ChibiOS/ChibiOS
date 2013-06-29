/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
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
