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
#include "chprintf.h"
#include "string.h"

#define BUFFER_SIZE  16

static virtual_timer_t vt3;
static char txbuf[BUFFER_SIZE] = "0123456789ABCDEF";
static char rxbuf[BUFFER_SIZE];

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
};

static void led3off(void *p) {

  (void)p;
  palSetLine(LINE_LED_RED);
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(SPIDriver *spip) {

  (void)spip;

  palClearLine(LINE_LED_RED);
  chSysLockFromISR();
  chVTResetI(&vt3);
  chVTSetI(&vt3, TIME_MS2I(50), led3off, NULL);
  chSysUnlockFromISR();
}

/*
 * Master SPI configuration (83MHz (MCK/3/2), CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig mst_spicfg = {
  rxend,                                       /* callback if present */
  0,                                           /* cs pad number       */
  SPI_MR_MODFDIS | SPI_MR_LLB,                 /* mr register         */
  SPI_CSR_SCBR(1)                              /* csr                 */
};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {

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
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, &sdcfg);
  spiStart(&SPID1, &mst_spicfg);       /* Setup transfer parameters.       */


  /* Redirecting  SPI1 pins. */
  palSetGroupMode(PIOD, PAL_PORT_BIT(25) | PAL_PORT_BIT(26) |
                  PAL_PORT_BIT(27) | PAL_PORT_BIT(28) , 0U,
                  PAL_SAMA_FUNC_PERIPH_A | PAL_MODE_SECURE);

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO , Thread1, NULL);

  while (true) {
    if(!palReadPad(PIOB, PIOB_USER_PB)) {
      /* SPI operation in loopback*/
      spiExchange(&SPID1, BUFFER_SIZE, &txbuf, &rxbuf);

      /* D-Cache L1 is enabled */
      cacheInvalidateRegion(&rxbuf, sizeof(rxbuf));

      if (!memcmp(txbuf, rxbuf, BUFFER_SIZE)){
        chprintf((BaseSequentialStream*)&SD1, "Transfer complete\n\r");
      }
      else {
        chprintf((BaseSequentialStream*)&SD1, "ERROR: Buffers are not the same!\n\r");
      }
    }
    chThdSleepMilliseconds(500);
  }
}
