/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#define BUFFER_SIZE     20

static char lineBuffer[BUFFER_SIZE];

static void getInputString(BaseSequentialStream *chp, char *line, size_t size) {
  char *p = line;
  uint8_t bytes;
  do {
    char c;
    bytes = chnRead(chp, (uint8_t *)&c, 1);

    if(bytes == 0) {
      *p = 0;
      break;
    }
    if ((c == '\r') || (c == '\n')) {
      *p = 0;
      break;
    }
    if ((c >= 0x20) && (c <= 0x7E)) {
      if (p < (line + size - 1)) {
        *p++ = (char)c;
      }
    }
  } while(TRUE);
}

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(80);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(160);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(600);
  }
}

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
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
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, &sdcfg);

  /* Redirecting  UART1 RX on PD2 and UART1 TX on PD3. */
  palSetGroupMode(PIOD, PAL_PORT_BIT(2) | PAL_PORT_BIT(3), 0U,
                  PAL_SAMA_FUNC_PERIPH_A | PAL_MODE_SECURE);
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity.
   */
  while (true) {
    getInputString((BaseSequentialStream *)&SD1, lineBuffer, BUFFER_SIZE);
    chprintf((BaseSequentialStream *)&SD1, lineBuffer);
    chprintf((BaseSequentialStream *)&SD1, "\n\r");
    chThdSleepMilliseconds(500);
  }
}
