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
static void cmd_write(struct xshell_manager *smp, BaseSequentialStream *chp,
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

  if (argc > 1) {
    chprintf(chp, "Usage: write\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
#if 1
    /* Writing in channel mode.*/
    chnWrite(chp, buf, sizeof buf - 1);
#else
    /* Writing in buffer mode.*/
    (void) obqGetEmptyBufferTimeout(&PORTAB_SDU1.obqueue, TIME_INFINITE);
    memcpy(PORTAB_SDU1.obqueue.ptr, buf, SERIAL_USB_BUFFERS_SIZE);
    obqPostFullBuffer(&PORTAB_SDU1.obqueue, SERIAL_USB_BUFFERS_SIZE);
#endif
  }
  chprintf(chp, "\r\n\nstopped\r\n");
}

#if STM32_CLOCK_DYNAMIC == TRUE
static void cmd_clock(struct xshell_manager *smp, BaseSequentialStream *chp,
                      int argc, char *argv[]) {
  bool result;
  const halclkcfg_t *ccp;
  static const char usage[] = "Usage: clock [reset|default]\r\n";

  (void)smp;

  if (argc != 2) {
    chprintf(chp, usage);
    return;
  }

  if (strcmp(argv[1], "reset") == 0) {
    chprintf(chp, "\r\nSwitching to post-reset clocks: ");
    ccp = &hal_clkcfg_reset;
  }
  else if (strcmp(argv[1], "default") == 0) {
    chprintf(chp, "\r\nSwitching to default mcuconf.h clocks: ");
    ccp = &hal_clkcfg_default;
  }
  else {
    chprintf(chp, usage);
    return;
  }

  /* Time for the serial TX buffer to flush.*/
  chThdSleepMilliseconds(100);

  /* Switching clocks.*/
  result = halClockSwitchMode(ccp);

  /* Reconfiguring the peripherals because clocks frequencies could have changed.*/
  sioStart(&SIOD3, NULL);

  /* Printing result.*/
  if (result) {
    chprintf(chp, "failed\r\n");
  }
  else {
    chprintf(chp, "done\r\n");
  }
}
#endif

static void cmd_clocks(struct xshell_manager *smp,
                       BaseSequentialStream *chp, int argc, char *argv[]) {
  const char *swp;

  (void)smp;
  (void)argv;

  if (argc > 1) {
    chprintf(chp, "Usage: clocks\r\n");
    return;
  }

#if STM32_SW == RCC_CFGR1_SW_HSI
  swp = "HSI";
#elif STM32_SW == RCC_CFGR1_SW_CSI
  swp = "CSI";
#elif STM32_SW == RCC_CFGR1_SW_HSE
  swp = "HSE";
#elif STM32_SW == RCC_CFGR1_SW_PLL1P
  swp = "PLL1P";
#else
  #error "invalid STM32_SW value specified"
#endif

  chprintf(chp, "HSI:      %10u\r\n", halClockGetPointX(CLK_HSI));
  chprintf(chp, "CSI:      %10u\r\n", halClockGetPointX(CLK_CSI));
  chprintf(chp, "HSI48:    %10u\r\n", halClockGetPointX(CLK_HSI48));
  chprintf(chp, "HSE:      %10u\r\n", halClockGetPointX(CLK_HSE));
  chprintf(chp, "SYSCLK:   %10u (%s)\r\n", halClockGetPointX(CLK_SYSCLK), swp);
  chprintf(chp, "PLL1PCLK: %10u\r\n", halClockGetPointX(CLK_PLL1PCLK));
  chprintf(chp, "PLL1RCLK: %10u\r\n", halClockGetPointX(CLK_PLL1RCLK));
  chprintf(chp, "PLL1QCLK: %10u\r\n", halClockGetPointX(CLK_PLL1QCLK));
  chprintf(chp, "PLL2PCLK: %10u\r\n", halClockGetPointX(CLK_PLL2PCLK));
  chprintf(chp, "PLL2RCLK: %10u\r\n", halClockGetPointX(CLK_PLL2RCLK));
  chprintf(chp, "PLL2QCLK: %10u\r\n", halClockGetPointX(CLK_PLL2QCLK));
  chprintf(chp, "PLL3PCLK: %10u\r\n", halClockGetPointX(CLK_PLL3PCLK));
  chprintf(chp, "PLL3RCLK: %10u\r\n", halClockGetPointX(CLK_PLL3RCLK));
  chprintf(chp, "PLL3QCLK: %10u\r\n", halClockGetPointX(CLK_PLL3QCLK));
  chprintf(chp, "HCLK:     %10u\r\n", halClockGetPointX(CLK_HCLK));
  chprintf(chp, "PCLK1:    %10u\r\n", halClockGetPointX(CLK_PCLK1));
  chprintf(chp, "PCLK1TIM: %10u\r\n", halClockGetPointX(CLK_PCLK1TIM));
  chprintf(chp, "PCLK2:    %10u\r\n", halClockGetPointX(CLK_PCLK2));
  chprintf(chp, "PCLK2TIM: %10u\r\n", halClockGetPointX(CLK_PCLK2TIM));
  chprintf(chp, "PCLK3:    %10u\r\n", halClockGetPointX(CLK_PCLK3));
  chprintf(chp, "MCO1:     %10u\r\n", halClockGetPointX(CLK_MCO1));
  chprintf(chp, "MCO1:     %10u\r\n", halClockGetPointX(CLK_MCO1));
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
  .stack.size       = SHELL_WA_SIZE
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
    palSetLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED3);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED1);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED3);
    chThdSleepMilliseconds(200);
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
  sioStart(&SIOD3, NULL);

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
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    thread_t *shelltp = xshellSpawn(&sm1,
                                    (BaseSequentialStream *)&SIOD3,
                                    NORMALPRIO + 1);
    chThdWait(shelltp);               /* Waiting termination.             */
    chThdSleepMilliseconds(500);
  }
}
