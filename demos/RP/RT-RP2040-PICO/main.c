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

void spi_cb(SPIDriver *spip) {

  (void)spip;
}

semaphore_t blinker_sem;

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    chSemWait(&blinker_sem);
    palToggleLine(25U);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * Shared objects initialization.
   */
  chSemObjectInit(&blinker_sem, 0);

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
   * Setting up GPIOs.
   */
  palSetLineMode(25U, PAL_MODE_OUTPUT_PUSHPULL | PAL_RP_PAD_DRIVE12);

  /*
   * Setting up SPI0.
   */
  {
    static SPIConfig spicfg1 = {
      .end_cb   = spi_cb,
      .ssline   = 4U,
      .SSPCR0   = SPI_SSPCR0_FRF_MOTOROLA | SPI_SSPCR0_DSS_8BIT,
      .SSPCPSR  = 0U
    };
    spicfg1.SSPCPSR = hal_lld_get_clock(clk_peri) / 1000000U;
    spiStart(&SPID0, &spicfg1);

    static const uint16_t txbuf[16] = {0, 1, 2, 3, 4, 5, 6, 7,
                                       0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF};
    static uint16_t rxbuf[16];
    spiExchange(&SPID0, 16U, txbuf, rxbuf);
  }

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
