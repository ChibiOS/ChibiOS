/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"

/*
 * Conversion table from hex digit to 7 segments encoding, bit 5 controls the
 * dot.
 * 8 = LU, 4 = RL, 2 = D, 1 = RU, 8 = U, 4 = M, 2 = LL, 1 = L.
 */
static uint8_t digits[32] = {
  0x24, 0xAF, 0xE0, 0xA2, 0x2B, 0x32, 0x30, 0xA7,
  0x20, 0x22, 0x21, 0x38, 0x74, 0xA8, 0x70, 0x71,
  0x04, 0x8F, 0xC0, 0x82, 0x0B, 0x12, 0x10, 0x87,
  0x00, 0x02, 0x01, 0x18, 0x54, 0x88, 0x50, 0x51
};

/*
 * SPI configuration (1MHz, CPHA=0, CPOL=0).
 */
static SPIConfig spicfg = {
  NULL,
  GPIO1,
  GPIO1_SPI0SEL,
  CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0),
  72
};

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker1");
  while (TRUE) {
    palClearPad(GPIO0, GPIO0_LED2);
    chThdSleepMilliseconds(500);
    palSetPad(GPIO0, GPIO0_LED2);
    chThdSleepMilliseconds(500);
  }
}

/*
 * RGB LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 128);
static msg_t Thread2(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker2");
  while (TRUE) {
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    chThdSleepMilliseconds(250);
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    palSetPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B));
    chThdSleepMilliseconds(250);
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    palSetPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3R));
    chThdSleepMilliseconds(250);
    palClearPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3B) |
                        PAL_PORT_BIT(GPIO1_LED3R) |
                        PAL_PORT_BIT(GPIO1_LED3G));
    palSetPort(GPIO1, PAL_PORT_BIT(GPIO1_LED3G));
    chThdSleepMilliseconds(250);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  uint8_t i;

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
   * Activates the SD1 and SPI1 drivers.
   */
  sdStart(&SD1, NULL);                  /* Default: 38400,8,N,1.            */
  spiStart(&SPID1, &spicfg);

  /*
   * Creates the blinker threads.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

  /*
   * Normal main() thread activity, in this demo it updates the 7-segments
   * display on the LPCXpresso main board using the SPI driver.
   */
  i = 0;
  while (TRUE) {
    if (!palReadPad(GPIO0, GPIO0_SW3))
      TestThread(&SD1);
    spiSelect(&SPID1);
    spiSend(&SPID1, 1, &digits[i]);                 /* Non polled method.   */
    spiUnselect(&SPID1);
    chThdSleepMilliseconds(500);
    spiSelect(&SPID1);
    spiPolledExchange(&SPID1, digits[i | 0x10]);    /* Polled method.       */
    spiUnselect(&SPID1);
    chThdSleepMilliseconds(500);
    i = (i + 1) & 15;
  }
}
