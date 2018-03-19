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


BaseSequentialStream *serialp;

extern void wolfCrypt_Init(void);
extern void wolfcrypt_test(void);
/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palClearLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(500);
  }
}

static const SerialConfig sdcfg = { 115200, 0,UART_MR_PAR_NO };
static const CRYConfig cryptoconf = {
    TRANSFER_POLLING,
    AES_CFBS_128,       //cfbs
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

  sdStart(&SD1, &sdcfg);

  serialp =(BaseSequentialStream *)&SD1;

  	/* Redirecting  UART0 RX on PD2 and UART0 TX on PD3. */
  	palSetGroupMode(PIOD, PAL_PORT_BIT(2) | PAL_PORT_BIT(3), 0U,
  			PAL_SAMA_FUNC_PERIPH_A | PAL_MODE_SECURE);

  	cryStart(&CRYD1, &cryptoconf);
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  wolfCrypt_Init();
  wolfcrypt_test();
  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
