/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <stdio.h>

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"

#define SHELL_WA_SIZE   THD_WA_SIZE(1024)
#define TEST_WA_SIZE    THD_WA_SIZE(256)

static void cmd_mem(BaseChannel *chp, int argc, char *argv[]) {
  size_t n, size;
  char buf[52];

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: mem");
    return;
  }
  n = chHeapStatus(NULL, &size);
  siprintf(buf, "core free memory : %i bytes", chCoreStatus());
  shellPrintLine(chp, buf);
  siprintf(buf, "heap fragments   : %i", n);
  shellPrintLine(chp, buf);
  siprintf(buf, "heap free total  : %i bytes", size);
  shellPrintLine(chp, buf);
}

static void cmd_threads(BaseChannel *chp, int argc, char *argv[]) {
  static const char *states[] = {
    "READY",
    "CURRENT",
    "SUSPENDED",
    "WTSEM",
    "WTMTX",
    "WTCOND",
    "SLEEPING",
    "WTEXIT",
    "WTOREVT",
    "WTANDEVT",
    "SNDMSG",
    "WTMSG",
    "FINAL"
  };
  Thread *tp;
  char buf[60];

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: threads");
    return;
  }
  shellPrintLine(chp, "    addr    stack prio refs     state time");
  tp = chRegFirstThread();
  do {
    siprintf(buf, "%8lx %8lx %4u %4i %9s %u",
             (uint32_t)tp, (uint32_t)tp->p_ctx.sp,
             (unsigned int)tp->p_prio, tp->p_refs - 1,
             states[tp->p_state], (unsigned int)tp->p_time);
    shellPrintLine(chp, buf);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseChannel *chp, int argc, char *argv[]) {
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: test");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriority(),
                           TestThread, chp);
  if (tp == NULL) {
    shellPrintLine(chp, "out of memory");
    return;
  }
  chThdWait(tp);
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseChannel *)&SD1,
  commands
};

/*
 * LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;

  SIU.GPDO[GPIO_LED1].R = 1;
  SIU.GPDO[GPIO_LED2].R = 1;
  SIU.GPDO[GPIO_LED3].R = 1;
  SIU.GPDO[GPIO_LED4].R = 1;

  while (TRUE) {
    SIU.GPDO[GPIO_LED1].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED2].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED3].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED4].R = 0;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED1].R = 1;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED2].R = 1;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED3].R = 1;
    chThdSleepMilliseconds(100);
    SIU.GPDO[GPIO_LED4].R = 1;
    chThdSleepMilliseconds(300);
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {
  Thread *shelltp = NULL;

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
  sdStart(&SD1, NULL);

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity.
   */
  while (TRUE) {

    if (!shelltp)
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    else if (chThdTerminated(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
#if 0
    if (SIU.GPDI[GPIO_BUTTON1].B.PDI) {
      volatile msg_t result;
#if 0
      MemoryStream report;

      msObjectInit(&report, report_buffer, sizeof(report_buffer), 0);
      result = TestThread(&report);
#else
      result = TestThread(&SD1);
#endif
    }
#endif
    chThdSleepMilliseconds(1000);
  }
  return 0;
}
