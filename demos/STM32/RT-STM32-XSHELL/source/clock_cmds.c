/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
#include "xshell.h"

#include "clock_cmds.h"
#include "portab.h"

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT)
void cmd_clock(xshell_manager_t *smp, BaseSequentialStream *stream,
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
    chprintf(stream, XSHELL_NEWLINE_STR "Switching to post-reset clock settings: ");
    ccp = &hal_clkcfg_reset;
  }
  else if (strcmp(argv[1], "default") == 0) {
    chprintf(stream, XSHELL_NEWLINE_STR "Switching to default mcuconf.h clock settings: ");
    ccp = &hal_clkcfg_default;
  }
  else {
    xshellUsage(stream, "clock reset|default");
    return;
  }

  /* Time for the serial TX buffer to flush.*/
  chThdSleepMilliseconds(10);

  /* Switching clocks.*/
  result = halClockSwitchMode(ccp);

  /* Reconfiguring the peripherals because clocks frequencies could have
     changed.*/
  sioStart(&PORTAB_SIOD1, NULL);

  /* Printing result.*/
  if (result) {
    chprintf(stream, "failed" XSHELL_NEWLINE_STR);
  }
  else {
    chprintf(stream, "done" XSHELL_NEWLINE_STR);
  }
}
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

void cmd_clocks(xshell_manager_t *smp, BaseSequentialStream *stream,
                int argc, char *argv[]) {
  static const char *names[CLK_ARRAY_SIZE] = CLK_POINT_NAMES;
  unsigned i;

  (void)smp;
  (void)argv;

  if (argc != 1) {
    xshellUsage(stream, "clocks");
    return;
  }

#if 0
#if STM32_SW == STM32_SW_HSI16
  swp = "HSI16";
#elif STM32_SW == STM32_SW_HSE
  swp = "HSE";
#elif STM32_SW == STM32_SW_PLLRCLK
  swp = "PLLR";
#else
  #error "invalid STM32_SW value specified"
#endif
#endif

  for (i = 0U; i < CLK_ARRAY_SIZE; i++) {
    chprintf(stream, "%10s: %10u" XSHELL_NEWLINE_STR, names[i], halClockGetPointX(i));
  }
}
