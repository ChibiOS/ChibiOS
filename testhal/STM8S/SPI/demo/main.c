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
