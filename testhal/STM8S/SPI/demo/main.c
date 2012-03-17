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

#include "ch.h"
#include "hal.h"

/*
 * SPI configuration (8MHz, CPHA=0, CPOL=0, MSb first).
 */
static ROMCONST SPIConfig spicfg = {
  NULL,
  GPIOD,
  PD_LD10,
  0
};

/*
 * Transmit data.
 */
static ROMCONST uint8_t digits[32] = {
  0x24, 0xAF, 0xE0, 0xA2, 0x2B, 0x32, 0x30, 0xA7,
  0x20, 0x22, 0x21, 0x38, 0x74, 0xA8, 0x70, 0x71,
  0x04, 0x8F, 0xC0, 0x82, 0x0B, 0x12, 0x10, 0x87,
  0x00, 0x02, 0x01, 0x18, 0x54, 0x88, 0x50, 0x51
};

/*
 * Receive buffer.
 */
static uint8_t buffer[32];

/*
 * Application entry point.
 */
void main(void) {

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
   * OS initialization.
   */
  chSysInit();

  /*
   * Activates the SPI driver 1 using the driver default configuration.
   */
  spiStart(&SPID1, &spicfg);

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {
    volatile uint8_t b;

    chThdSleepMilliseconds(1000);
    /* Exchanging data, if the pins MISO and MOSI are connected then the
       transmitted data is received back into the buffer. On the
       STM8S-Discovery board the pins are CN2-9 and CN2-10.*/
    spiSelect(&SPID1);
    spiExchange(&SPID1, sizeof(digits), digits, buffer);
    /* Polled transfers test.*/
    b = spiPolledExchange(&SPID1, 0x55);    
    b = spiPolledExchange(&SPID1, 0xAA);    
    spiUnselect(&SPID1);
  }
}
