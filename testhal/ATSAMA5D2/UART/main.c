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

#define BUFFER_SIZE 5

static virtual_timer_t vt3, vt4, vt5;
static const uint8_t message[] = "ABCDE";
static uint8_t buffer[BUFFER_SIZE];

static void led3off(void *p) {

  (void)p;
  palSetLine(LINE_LED_RED);
}

static void led4off(void *p) {

  (void)p;
  palSetLine(LINE_LED_GREEN);
}

static void led5off(void *p) {

  (void)p;
  palSetLine(LINE_LED_BLUE);
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
  palClearLine(LINE_LED_BLUE);
  chSysLockFromISR();
  chVTResetI(&vt5);
  chVTSetI(&vt5, TIME_MS2I(200), led5off, NULL);
  chSysUnlockFromISR();
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
  palClearLine(LINE_LED_GREEN);
  chSysLockFromISR();
  chVTResetI(&vt4);
  chVTSetI(&vt4, TIME_MS2I(200), led4off, NULL);
  chSysUnlockFromISR();
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;

  palClearLine(LINE_LED_RED);
  chSysLockFromISR();
  chVTResetI(&vt3);
  chVTSetI(&vt3, TIME_MS2I(200), led3off, NULL);
  chSysUnlockFromISR();
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
  NULL,
  0,
  38400,
  0,
  US_MR_CHRL_8_BIT | US_MR_PAR_NO
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
   * Activates the flexcom uart driver 0, PB28(RX) and PB29(RX) are routed to FLEXCOM0.
   */
  uartStart(&FUARTD0, &uart_cfg_1);

  /*
   * Activates the serial driver 0 using the driver default configuration.
   */
  sdStart(&SD0, NULL);

  /* Redirecting  SERIAL RX on PB26 and UART0 TX on PB27. */
  palSetGroupMode(PIOB, PAL_PORT_BIT(26) | PAL_PORT_BIT(27), 0U,
                  PAL_SAMA_FUNC_PERIPH_C | PAL_MODE_SECURE);

  /* Redirecting  UART FLEXCOM0 RX on PB28 and UART FLEXCOM0 TX on PB29. */
  palSetGroupMode(PIOB, PAL_PORT_BIT(28) | PAL_PORT_BIT(29), 0U,
                  PAL_SAMA_FUNC_PERIPH_C | PAL_MODE_SECURE);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * increasing the minutes counter.
   */
  while (true) {
    if (!palReadPad(PIOB, PIOB_USER_PB)) {
  /*
   * Starts both a transmission and a receive operations, both will be
   * handled entirely in background.
   */
      uartStopReceive(&FUARTD0);
      uartStopSend(&FUARTD0);
      uartStartReceive(&FUARTD0, BUFFER_SIZE, buffer);
      uartStartSend(&FUARTD0, BUFFER_SIZE, message);
    }
    chThdSleepMilliseconds(500);
  }
}
