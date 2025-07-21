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

//#include <pico/runtime_init.h>
#include <stdio.h>
#include "pico/stdlib.h"

#if USE_WIFI_VARIANT == 1
#include "pico/cyw43_arch.h" // for cyw43_arch_gpio_put etc
#endif

semaphore_t blinker_sem;

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static CH_SYS_CORE0_MEMORY THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    chSemWait(&blinker_sem);
    palToggleLine(25U); // on non-"W" boards, pin25 is the led.
    #if USE_WIFI_VARIANT == 1
    cyw43_arch_gpio_put(1, 1); // on "W" boards,   WL0/WL_GPIO0 is the led, so we do both
    #endif
  }
}

/*
 * Application entry point.
 */
int main(void) {

  stdio_init_all(); //init all the pico-sdk.

      // these are pico sdk calls, and we can call function/s from any .c file mentioned in 
      // os/various/pico_bindings/pico-sdk.mk which right now includes gpio,clocks,pll.
      gpio_init(PICO_DEFAULT_LED_PIN);
      gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

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
