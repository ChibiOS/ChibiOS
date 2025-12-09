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

#include "chprintf.h"
#include "xshell.h"

#include "clock_cmds.h"
#include "portab.h"

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE       THD_STACK_SIZE(1024)

static void cmd_halt(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc != 1) {
    xshellUsage(xshp, "halt");
    return;
  }

  chprintf(xshp->stream, XSHELL_NEWLINE_STR "halted");
  chThdSleepMilliseconds(10);
  chSysHalt("shell halt");
}

/* Can be measured using dd if=/dev/xxxx of=/dev/null bs=512 count=10000.*/
static void cmd_write(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  static const uint8_t buf[] =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

  (void)argv;
  (void)envp;

  if (argc != 1) {
    xshellUsage(xshp, "write");
    return;
  }

  while (chnGetTimeout((BaseChannel *)xshp->stream, TIME_IMMEDIATE) == Q_TIMEOUT) {
    chnWrite(xshp->stream, buf, sizeof buf - 1);
  }
  chprintf(xshp->stream, XSHELL_NEWLINE_STR "stopped" XSHELL_NEWLINE_STR);
}

static const xshell_command_t commands[] = {
#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT)
  {"clock",     cmd_clock},
#endif
  {"clocks",    cmd_clocks},
  {"halt",      cmd_halt},
  {"write",     cmd_write},
  {NULL, NULL}
};

static const xshell_manager_config_t cfg1 = {
  .thread_name      = "shell",
  .banner           = XSHELL_DEFAULT_BANNER_STR,
  .prompt           = XSHELL_DEFAULT_PROMPT_STR,
  .commands         = commands,
  .use_heap         = true,
  .stack.size       = SHELL_WA_SIZE
};

/*===========================================================================*/
/* Application code.                                                         */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_STACK(thd1_stack, 256);
static THD_FUNCTION(thd1_func, arg) {

  (void)arg;

  while (true) {
    palSetLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
    palClearLine(PORTAB_LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  xshell_manager_t sm1;

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

  /*
   * Spawning a blinker thread.
   */
  static thread_t thd1;
  static const THD_DECL_STATIC(thd1_desc, "blinker", thd1_stack,
                               NORMALPRIO + 10, thd1_func, NULL, NULL);
  chThdSpawnRunning(&thd1, &thd1_desc);

  /*
   * Shell manager initialization.
   */
  xshellObjectInit(&sm1, &cfg1);

  /*
   * Activates the SIO driver using the driver default configuration.
   */
  sioStart(&PORTAB_SIOD1, NULL);

  /*
   * Normal main() thread activity, spawning shells.
   */
  while (true) {
    xshell_t *xshp = xshellSpawn(&sm1,
                                 (BaseSequentialStream *)&PORTAB_SIOD1,
                                 NORMALPRIO + 1, NULL);
    xshellWait(xshp);
    chThdSleepMilliseconds(500);
  }

  return 0;
}
