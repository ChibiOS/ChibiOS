/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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

/*
 * Simple LED blink demo for Raspberry Pi RP2350 Pico 2.
 * This is a minimal single threaded example that blinks the onboard LED
 * and outputs "Hello world!" over serial.
 */

#include "ch.h"
#include "hal.h"

#include <string.h>

/* Onboard LED is on GPIO 25 */
#define LED_PIN     25U

/* UART0 pins */
#define UART_TX_PIN 0U
#define UART_RX_PIN 1U

/* UART configuration: 115200 baud, 8-N-1, FIFO enabled */
static const SIOConfig sio_config = {
  .baud      = 115200,
  .UARTLCR_H = UART_UARTLCR_H_WLEN_8BITS | UART_UARTLCR_H_FEN,
  .UARTCR    = 0U,
  .UARTIFLS  = UART_UARTIFLS_RXIFLSEL_1_2F | UART_UARTIFLS_TXIFLSEL_1_2E,
  .UARTDMACR = 0U
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   */
  halInit();
  chSysInit();

  /*
   * Configure LED pin as output.
   */
  palSetLineMode(LED_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_RP_PAD_DRIVE12);

  /*
   * Configure UART pins and start the serial driver.
   */
  palSetLineMode(UART_TX_PIN, PAL_MODE_ALTERNATE_UART);
  palSetLineMode(UART_RX_PIN, PAL_MODE_ALTERNATE_UART);
  sioStart(&SIOD0, &sio_config);

  /*
   * Main loop - blink the LED every 500ms and output "Hello world!" when LED turns on.
   */
  while (true) {
    static const char msg[] = "Hello RP2350 world!\r\n";

    /* Turn LED on and send message */
    palSetLine(LED_PIN);
    sioAsyncWrite(&SIOD0, (const uint8_t *)msg, strlen(msg));
    chThdSleepMilliseconds(500);

    /* Turn LED off */
    palClearLine(LED_PIN);
    chThdSleepMilliseconds(500);
  }
}
