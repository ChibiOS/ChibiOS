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
 * Maximum speed SPI configuration.
 */
static const SPIConfig hs_spicfg = {
  NULL,
  0,
  0,
  SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 | SPC5_CTAR_FMSZ(8) |
  SPC5_CTAR_PBR_PRE2 | SPC5_CTAR_BR_DIV2,                       /* CTAR0.   */
  SPC5_PUSHR_CONT | SPC5_PUSHR_PCS(1)                           /* PUSHR.   */
};

/*
 * Low speed SPI configuration (328.125kHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig ls_spicfg = {
  NULL,
  0,
  0,
  SPC5_CTAR_CSSCK_DIV64 | SPC5_CTAR_ASC_DIV64 | SPC5_CTAR_FMSZ(8) |
  SPC5_CTAR_PBR_PRE2 | SPC5_CTAR_BR_DIV256,                     /* CTAR0.   */
  SPC5_PUSHR_CONT | SPC5_PUSHR_PCS(0)                           /* PUSHR.   */
};

/*
 * SPI TX and RX buffers.
 */
static uint8_t txbuf[512];
static uint8_t rxbuf[512];

/*
 * SPI bus contender 1.
 */
static WORKING_AREA(spi_thread_1_wa, 256);
static msg_t spi_thread_1(void *p) {

  (void)p;
  chRegSetThreadName("SPI thread 1");
  while (TRUE) {
    spiAcquireBus(&SPID1);              /* Acquire ownership of the bus.    */
    palClearPad(PORT_D, PD_LED1);       /* LED ON.                          */
    spiStart(&SPID1, &hs_spicfg);       /* Setup transfer parameters.       */
    spiSelect(&SPID1);                  /* Slave Select assertion.          */
    spiExchange(&SPID1, 512,
                txbuf, rxbuf);          /* Atomic transfer operations.      */
    spiUnselect(&SPID1);                /* Slave Select de-assertion.       */
    spiReleaseBus(&SPID1);              /* Ownership release.               */
  }
  return 0;
}

/*
 * SPI bus contender 2.
 */
static WORKING_AREA(spi_thread_2_wa, 256);
static msg_t spi_thread_2(void *p) {

  (void)p;
  chRegSetThreadName("SPI thread 2");
  while (TRUE) {
    spiAcquireBus(&SPID1);              /* Acquire ownership of the bus.    */
    palSetPad(PORT_D, PD_LED1);         /* LED OFF.                         */
    spiStart(&SPID1, &ls_spicfg);       /* Setup transfer parameters.       */
    spiSelect(&SPID1);                  /* Slave Select assertion.          */
    spiExchange(&SPID1, 512,
                txbuf, rxbuf);          /* Atomic transfer operations.      */
    spiUnselect(&SPID1);                /* Slave Select de-assertion.       */
    spiReleaseBus(&SPID1);              /* Ownership release.               */
  }
  return 0;
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

  /*
   * Prepare transmit pattern.
   */
  for (i = 0; i < sizeof(txbuf); i++)
    txbuf[i] = (uint8_t)i;

  /* Starting driver for test, DSPI_1 I/O pins setup.*/
  spiStart(&SPID1, &ls_spicfg);
  SIU.PCR[37].R = PAL_MODE_OUTPUT_ALTERNATE(1);     /* SCK    */
  SIU.PCR[38].R = PAL_MODE_OUTPUT_ALTERNATE(1);     /* SOUT   */
  SIU.PCR[36].R = PAL_MODE_OUTPUT_ALTERNATE(1);     /* CS[0] */
  SIU.PCR[35].R = PAL_MODE_OUTPUT_ALTERNATE(1);     /* CS[1] */

  /* Testing sending and receiving at the same time.*/
  spiExchange(&SPID1, 4, txbuf, rxbuf);
  spiExchange(&SPID1, 32, txbuf, rxbuf);
  spiExchange(&SPID1, 512, txbuf, rxbuf);

  /* Testing clock pulses without data buffering.*/
  spiIgnore(&SPID1, 4);
  spiIgnore(&SPID1, 32);

  /* Testing sending data ignoring incoming data.*/
  spiSend(&SPID1, 4, txbuf);
  spiSend(&SPID1, 32, txbuf);

  /* Testing receiving data while sending idle bits (high level).*/
  spiReceive(&SPID1, 4, rxbuf);
  spiReceive(&SPID1, 32, rxbuf);

  /* Testing stop procedure.*/
  spiStop(&SPID1);

  /*
   * Starting the transmitter and receiver threads.
   */
  chThdCreateStatic(spi_thread_1_wa, sizeof(spi_thread_1_wa),
                    NORMALPRIO + 1, spi_thread_1, NULL);
  chThdCreateStatic(spi_thread_2_wa, sizeof(spi_thread_2_wa),
                    NORMALPRIO + 1, spi_thread_2, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
    palTogglePad(PORT_D, PD_LED2);
  }
  return 0;
}
