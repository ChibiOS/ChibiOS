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
#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "shell.h"
#include "chprintf.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static const ShellCommand commands[] = {
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SIOD1,
  commands
};

#if 0
/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palClearLine(25U);
    chThdSleepMilliseconds(500);
    palSetLine(25U);
    chThdSleepMilliseconds(500);
  }
}
#endif

/* Courtesy of Pico-SDK.*/
static void start_core1(void) {
  extern uint32_t __c1_main_stack_end__, _vectors;
  extern void _crt0_c1_entry(void);
  uint32_t cmd_sequence[] = {0, 0, 1,
                             (uint32_t)&_vectors,
                             (uint32_t)&__c1_main_stack_end__,
                             (uint32_t)_crt0_c1_entry};
  unsigned seq;

  seq = 0;
  do {
    uint32_t response;
    uint32_t cmd = cmd_sequence[seq];

    /* Flushing the FIFO state before sending a zero.*/
    if (!cmd) {
      fifoFlushRead();
    }
    fifoBlockingWrite(cmd);
    response = fifoBlockingRead();
    /* Checking response, going forward or back to first step.*/
    seq = cmd == response ? seq + 1U : 0U;
  } while (seq < count_of(cmd_sequence));
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
   * Starting core 1 after performing all OS-related initializations.
   */
  start_core1();

  /*
   * Setting up GPIOs.
   */
  palSetLineMode(0U, PAL_MODE_ALTERNATE_UART);
  palSetLineMode(1U, PAL_MODE_ALTERNATE_UART);
  palSetLineMode(25U, PAL_MODE_OUTPUT_PUSHPULL | PAL_RP_PAD_DRIVE12);

  /*
   * Activates the Serial or SIO driver using the default configuration.
   */
  sioStart(&SIOD1, NULL);
  sioStartOperation(&SIOD1, NULL);

  /*
   * Creates the blinker thread.
   */
//  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop (re)spawning a shell.
   */
  while (true) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);               /* Waiting termination.             */
    chThdSleepMilliseconds(500);
  }
}
