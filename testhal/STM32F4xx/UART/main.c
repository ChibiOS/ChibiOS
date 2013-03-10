/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

static VirtualTimer vt3, vt4, vt5;

static const uint8_t message[] = "0123456789ABCDEF";
static uint8_t buffer[16];

static void led3off(void *p) {

  (void)p;
  palClearPad(GPIOD, GPIOD_LED3);
}

static void led4off(void *p) {

  (void)p;
  palClearPad(GPIOD, GPIOD_LED4);
}

static void led5off(void *p) {

  (void)p;
  palClearPad(GPIOD, GPIOD_LED5);
}

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;
  palSetPad(GPIOD, GPIOD_LED5);
  chSysLockFromIsr();
  if (chVTIsArmedI(&vt5))
    chVTResetI(&vt5);
  chVTSetI(&vt5, MS2ST(200), led5off, NULL);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  (void)c;
  /* Flashing the LED each time a character is received.*/
  palSetPad(GPIOD, GPIOD_LED4);
  chSysLockFromIsr();
  if (chVTIsArmedI(&vt4))
    chVTResetI(&vt4);
  chVTSetI(&vt4, MS2ST(200), led4off, NULL);
  chSysUnlockFromIsr();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;

  /* Flashing the LED each time a character is received.*/
  palSetPad(GPIOD, GPIOD_LED3);
  chSysLockFromIsr();
  if (chVTIsArmedI(&vt3))
    chVTResetI(&vt3);
  chVTSetI(&vt3, MS2ST(200), led3off, NULL);
  chSysUnlockFromIsr();
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  38400,
  0,
  USART_CR2_LINEN,
  0
};

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
   * Activates the UART driver 2, PA2(TX) and PA3(RX) are routed to USART2.
   */
  uartStart(&UARTD2, &uart_cfg_1);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));


  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    if (palReadPad(GPIOA, GPIOA_BUTTON)) {
      /*
       * Starts both a transmission and a receive operations, both will be
       * handled entirely in background.
       */
      uartStopReceive(&UARTD2);
//      uartStopSend(&UARTD2);
      uartStartReceive(&UARTD2, 16, buffer);
//      uartStartSend(&UARTD2, 16, message);
    }
    chThdSleepMilliseconds(500);
  }
}
