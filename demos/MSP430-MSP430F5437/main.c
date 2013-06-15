/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
#include "test.h"
#include "chprintf.h"
#include "shell.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 64);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("red blinker");
  
  while (TRUE) {
    palSetPad(IOPORT6, P6_O_RED_LED);
    chThdSleepMilliseconds(500);
    palClearPad(IOPORT6, P6_O_RED_LED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

/*
 * Green LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread2, 64);
static msg_t Thread2(void *arg) {

  (void)arg;
  chRegSetThreadName("green blinker");
  
  while (TRUE) {
    palSetPad(IOPORT6, P6_O_GREEN_LED);
    chThdSleepMilliseconds(1500);
    palClearPad(IOPORT6, P6_O_GREEN_LED);
    chThdSleepMilliseconds(1500);
  }
  return 0;
}

static void cmd_mem(BaseChannel *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseChannel *chp, int argc, char *argv[]) {
  static const char *states[] = {THD_STATE_NAMES};
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.sp,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state], (uint32_t)tp->p_time);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseChannel *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }

  TestThread(chp);
}

static void cmd_reboot(BaseChannel *chp, int argc, char *argv[]){
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: rboot\r\n");
    return;
  }
  chprintf(chp, "rebooting...\r\n");
  chThdSleepMilliseconds(100);
  
  WDTCTL = 0; /* Giving invalid value to watchdog cause reboot. */
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"reboot", cmd_reboot},
  {NULL, NULL}
};

static const ShellConfig shell_cfg[] = {
  {(BaseChannel *)&SD1, commands},
  {(BaseChannel *)&SD2, commands},
};

/*
 * Application entry point.
 */
int main(void) { 

  /* RTC initially stopped.*/
  WDTCTL = WDTPW | WDTHOLD;

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
   * The main() function becomes a thread here then the interrupts are
   * enabled and ChibiOS/RT goes live.
   */  

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD1, NULL);
  sdStart(&SD2, NULL);
  
  /* Shell manager initialization.*/
  shellInit();
  static WORKING_AREA(waShell1, 512);
  shellCreateStatic(&shell_cfg[0], waShell1, sizeof(waShell1), NORMALPRIO);
  static WORKING_AREA(waShell2, 512);
  shellCreateStatic(&shell_cfg[1], waShell2, sizeof(waShell2), NORMALPRIO);

  /* Creates the blinker thread. */
  Thread *th[] = {
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO+1, Thread1, NULL),
    chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO+2, Thread2, NULL),
  };
  
  cnt_t idx;
  for( idx = 0; idx < sizeof(th)/sizeof(th[0]); ++idx ) {
      msg_t msg = chThdWait(th[idx]);
  } 

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
