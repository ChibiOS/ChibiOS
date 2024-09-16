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

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "xshell.h"
#include "chprintf.h"

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

/* Can be measured using dd if=/dev/xxxx of=/dev/null bs=512 count=10000.*/
static void cmd_write(xshell_manager_t *smp, BaseSequentialStream *stream,
                      int argc, char *argv[]) {
  static uint8_t buf[] =
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

  (void)smp;
  (void)argv;

  if (argc != 1) {
    xshellUsage(stream, "write");
    return;
  }

  while (chnGetTimeout((BaseChannel *)stream, TIME_IMMEDIATE) == Q_TIMEOUT) {
    chnWrite(stream, buf, sizeof buf - 1);
  }
  chprintf(stream, XSHELL_NEWLINE_STR "stopped" XSHELL_NEWLINE_STR);
}

#if STM32_CLOCK_DYNAMIC == TRUE
static void cmd_clock(xshell_manager_t *smp, BaseSequentialStream *stream,
                      int argc, char *argv[]) {
  bool result;
  const halclkcfg_t *ccp;

  (void)smp;
  (void)argv;

  if (argc != 2) {
    xshellUsage(stream, "clock reset|default");
    return;
  }

  if (strcmp(argv[1], "reset") == 0) {
    chprintf(stream, XSHELL_NEWLINE_STR "Switching to post-reset clocks: ");
    ccp = &hal_clkcfg_reset;
  }
  else if (strcmp(argv[1], "default") == 0) {
    chprintf(stream, XSHELL_NEWLINE_STR "Switching to default mcuconf.h clocks: ");
    ccp = &hal_clkcfg_default;
  }
  else {
    xshellUsage(stream, "clock reset|default");
    return;
  }

  /* Time for the serial TX buffer to flush.*/
  chThdSleepMilliseconds(100);

  /* Switching clocks.*/
  result = halClockSwitchMode(ccp);

  /* Reconfiguring the peripherals because clocks frequencies could have
     changed.*/
  sioStart(&LPSIOD1, NULL);

  /* Printing result.*/
  if (result) {
    chprintf(stream, "failed" XSHELL_NEWLINE_STR);
  }
  else {
    chprintf(stream, "done" XSHELL_NEWLINE_STR);
  }
}
#endif

static void cmd_clocks(xshell_manager_t *smp, BaseSequentialStream *stream,
                       int argc, char *argv[]) {
  const char *swp;

  (void)smp;
  (void)argv;

  if (argc != 1) {
    xshellUsage(stream, "clocks");
    return;
  }

#if STM32_SW == STM32_SW_HSI16
  swp = "HSI16";
#elif STM32_SW == STM32_SW_HSE
  swp = "HSE";
#elif STM32_SW == STM32_SW_PLLRCLK
  swp = "PLLR";
#else
  #error "invalid STM32_SW value specified"
#endif

  chprintf(stream, "HSI16:    %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_HSI16));
  chprintf(stream, "HSI48:    %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_HSI48));
  chprintf(stream, "HSE:      %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_HSE));
  chprintf(stream, "SYSCLK:   %10u (%s)" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_SYSCLK), swp);
  chprintf(stream, "PLLPCLK:  %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PLLPCLK));
  chprintf(stream, "PLLRCLK:  %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PLLRCLK));
  chprintf(stream, "PLLQCLK:  %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PLLQCLK));
  chprintf(stream, "HCLK:     %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_HCLK));
  chprintf(stream, "PCLK1:    %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PCLK1));
  chprintf(stream, "PCLK1TIM: %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PCLK1TIM));
  chprintf(stream, "PCLK2:    %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PCLK2));
  chprintf(stream, "PCLK2TIM: %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_PCLK2TIM));
  chprintf(stream, "MCO:      %10u" XSHELL_NEWLINE_STR, halClockGetPointX(CLK_MCO));
}

static const xshell_command_t commands[] = {
  {"write", cmd_write},
#if STM32_CLOCK_DYNAMIC == TRUE
  {"clock", cmd_clock},
#endif
  {"clocks", cmd_clocks},
  {NULL, NULL}
};

static const xshell_manager_config_t cfg1 = {
  .thread_name      = "shell",
  .banner           = XSHELL_DEFAULT_BANNER_STR,
  .prompt           = XSHELL_DEFAULT_PROMPT_STR,
  .commands         = commands,
  .use_heap         = true,
  .stack.size       = THD_STACK_SIZE(512)
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_STACK(thd1_stack, 256);
static THD_FUNCTION(thd1_func, arg) {

  (void)arg;

  while (true) {
    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED_GREEN);
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

  /*
   * Activates the Serial or SIO driver using the default configuration.
   */
  sioStart(&LPSIOD1, NULL);

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
   * Normal main() thread activity, spawning shells.
   */
  while (true) {
    thread_t *shelltp = xshellSpawn(&sm1,
                                    (BaseSequentialStream *)&LPSIOD1,
                                    NORMALPRIO + 1);
    chThdWait(shelltp);               /* Waiting termination.             */
    chThdSleepMilliseconds(500);
  }
}

