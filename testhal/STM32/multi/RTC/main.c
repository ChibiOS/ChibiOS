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

#include "shell.h"
#include "chprintf.h"

#include "portab.h"

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

static void cmd_date(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;

  if (argc > 0) {
    chprintf(chp, "Usage: date\r\n");
    return;
  }
}

static const ShellCommand commands[] = {
  {"date", cmd_date},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&PORTAB_SD1,
  commands
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
    palToggleLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
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

  /* Board-dependent GPIO setup code.*/
  portab_setup();

  /* Starting a serial port for shell.*/
  sdStart(&PORTAB_SD1, NULL);

  /* Shell manager initialization.*/
  shellInit();

  /* Creates the blinker thread.*/
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Normal main() thread activity, spawning shells.*/
  while (true) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                            "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);
    chThdWait(shelltp);               /* Waiting termination.             */
    chThdSleepMilliseconds(1000);
  }
}
