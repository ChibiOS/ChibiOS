/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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

#define BUFFER_SIZE  512

static uint8_t txbuf[BUFFER_SIZE];
static uint8_t rxbuf[BUFFER_SIZE];

/*
 * SPI high speed configuration (peripheral clock / 3 = 27,6 Mhz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig hs_spicfg = {
  NULL,                                       /* callback if present */
  0,                                          /* cs pad number       */
  SPI_MR_MODFDIS | SPI_MR_LLB,                /* mr register         */
  SPI_CSR_SCBR(3)                             /* csr                 */
};

/*
 * SPI low speed configuration (peripheral clock / 166 = 500KHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig ls_spicfg = {
  NULL,                                       /* callback if present */
  0,                                          /* cs pad number       */
  SPI_MR_MODFDIS | SPI_MR_LLB,                /* mr register         */
  SPI_CSR_SCBR(166)                           /* csr                 */
};

/*
 * SPI bus contender 1.
 */
static THD_WORKING_AREA(spi_thread_1_wa, 1024);
static THD_FUNCTION(spi_thread_1, p) {

  (void)p;

  chRegSetThreadName("SPI thread 1");
  while (true) {
    spiAcquireBus(&FSPID2);              /* Acquire ownership of the bus.    */
    palClearLine(LINE_LED_RED);          /* LED ON.                          */
    spiStart(&FSPID2, &hs_spicfg);       /* Setup transfer parameters.       */
    spiExchange(&FSPID2, BUFFER_SIZE,
                txbuf, rxbuf);           /* Atomic transfer operations.      */
    cacheInvalidateRegion(&rxbuf, sizeof(rxbuf));
    spiReleaseBus(&FSPID2);              /* Ownership release.               */
  }
}

/*
 * SPI bus contender 2.
 */
static THD_WORKING_AREA(spi_thread_2_wa, 1024);
static THD_FUNCTION(spi_thread_2, p) {

  (void)p;

  chRegSetThreadName("SPI thread 2");
  while (true) {
    spiAcquireBus(&FSPID2);              /* Acquire ownership of the bus.    */
    palSetLine(LINE_LED_RED);            /* LED OFF.                         */
    spiStart(&FSPID2, &ls_spicfg);       /* Setup transfer parameters.       */
    spiExchange(&FSPID2, BUFFER_SIZE,
                txbuf, rxbuf);           /* Atomic transfer operations.      */
    cacheInvalidateRegion(&rxbuf, sizeof(rxbuf));
    spiReleaseBus(&FSPID2);              /* Ownership release.               */
  }
}

/*
 * Application entry point.
 */
int main(void) {
  unsigned i;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /* Redirecting  FLEXCOM 2 SPI pins. */
  palSetGroupMode(PIOD, PAL_PORT_BIT(26) | PAL_PORT_BIT(27) |
                  PAL_PORT_BIT(28) | PAL_PORT_BIT(29) , 0U,
                  PAL_SAMA_FUNC_PERIPH_C | PAL_MODE_SECURE);

  /*
   * Prepare transmit pattern.
   */
  for (i = 0; i < sizeof(txbuf); i++)
    txbuf[i] = (uint8_t)i;

  chThdCreateStatic(spi_thread_1_wa, sizeof(spi_thread_1_wa),
                    NORMALPRIO + 1, spi_thread_1, NULL);
  chThdCreateStatic(spi_thread_2_wa, sizeof(spi_thread_2_wa),
                    NORMALPRIO + 1, spi_thread_2, NULL);

  while (true) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
