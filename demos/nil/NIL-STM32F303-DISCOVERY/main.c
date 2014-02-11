/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include "nil.h"
#include "ch_test.h"

/*
 * Blinker thread #1.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  while (true) {
    palSetPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED7_GREEN);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED7_GREEN);
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED10_RED);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED10_RED);
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED6_GREEN);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED6_GREEN);
    chThdSleepMilliseconds(125);
  }
}

/*
 * Blinker thread #2.
 */
static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {

  (void)arg;

  while (true) {
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED5_ORANGE);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED5_ORANGE);
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED9_BLUE);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED9_BLUE);
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED8_ORANGE);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED8_ORANGE);
    chThdSleepMilliseconds(125);
    palSetPad(GPIOE, GPIOE_LED4_BLUE);
    chThdSleepMilliseconds(125);
    palClearPad(GPIOE, GPIOE_LED4_BLUE);
  }
}

/*
 * Tester thread.
 */
THD_WORKING_AREA(waThread3, 128);
THD_FUNCTION(Thread3, arg) {

  (void)arg;

  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PA9 and PA10 are routed to USART1.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(7));       /* USART1 TX.       */
  palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(7));      /* USART1 RX.       */

  /* Welcome message.*/
  chnWrite(&SD1, (const uint8_t *)"Hello World!\r\n", 14);

  /* Waiting for button push and activation of the test suite.*/
  while (true) {
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      test_execute((BaseSequentialStream *)&SD1);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
THD_TABLE_BEGIN
  THD_TABLE_ENTRY(waThread1, "blinker1", Thread1, NULL)
  THD_TABLE_ENTRY(waThread2, "blinker2", Thread2, NULL)
  THD_TABLE_ENTRY(wa_test_support, "test_support", test_support, NULL)
  THD_TABLE_ENTRY(waThread3, "tester", Thread3, NULL)
THD_TABLE_END

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

  /* This is now the idle thread loop, you may perform here a low priority
     task but you must never try to sleep or wait in this loop. Note that
     this tasks runs at the lowest priority level so any instruction added
     here will be executed after all other tasks have been started.*/
  while (true) {
  }
}
