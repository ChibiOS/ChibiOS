/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * This is a periodic thread that does absolutely nothing except increasing
 * a seconds counter.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOB, GPIOB_LED3);
    chThdSleepMilliseconds(250);
    palClearPad(GPIOB, GPIOB_LED3);
    chThdSleepMilliseconds(250);
    palSetPad(GPIOB, GPIOB_LED4);
    chThdSleepMilliseconds(250);
    palClearPad(GPIOB, GPIOB_LED4);
    chThdSleepMilliseconds(250);
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
   * PA9 and PA10 are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOA, 9, PAL_MODE_ALT_OUTPUT_PUSHPULL(7));
  palSetPadMode(GPIOA, 10, PAL_MODE_ALT_INPUT(7));

  /*
   * If the user button is pressed after the reset then the test suite is
   * executed immediately before activating the various device drivers in
   * order to not alter the benchmark scores.
   */
  if (palReadPad(GPIOA, GPIOA_BUTTON))
    TestThread(&SD1);

  /*
   * Initializes the SPI driver 1.
   */
//  spiStart(&SPID1, &spicfg);

  /*
   * Initializes the ADC driver 1.
   * The pin PC0 on the port GPIOC is programmed as analog input.
   */
//  adcStart(&ADCD1, NULL);
//  palSetGroupMode(GPIOC, PAL_PORT_BIT(0), PAL_MODE_INPUT_ANALOG);

  /*
   * Initializes the PWM driver 1, re-routes the TIM3 outputs, programs the
   * pins as alternate functions.
   * Note, the AFIO access routes the TIM3 output pins on the PC6...PC9
   * where the LEDs are connected.
   */
//  pwmStart(&PWMD3, &pwmcfg);
//  AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_0 | AFIO_MAPR_TIM3_REMAP_1;
//  palSetGroupMode(GPIOC, PAL_PORT_BIT(GPIOC_LED3) | PAL_PORT_BIT(GPIOC_LED4),
//                  PAL_MODE_STM32_ALTERNATE_PUSHPULL);

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 1.
   */
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      TestThread(&SD1);
    chThdSleepMilliseconds(500);
  }
}
