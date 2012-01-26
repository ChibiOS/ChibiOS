/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"
#include "chprintf.h"

#define SHELL_WA_SIZE       THD_WA_SIZE(4096)
#define CONSOLE_WA_SIZE     THD_WA_SIZE(4096)
#define TEST_WA_SIZE        THD_WA_SIZE(4096)

#define cputs(msg) chMsgSend(cdtp, (msg_t)msg)

static Thread *cdtp;
static Thread *shelltp1;
static Thread *shelltp2;

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
            (uint32_t)tp, (uint32_t)tp->p_ctx.esp,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state], (uint32_t)tp->p_time);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseChannel *chp, int argc, char *argv[]) {
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriority(),
                           TestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory\r\n");
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

static const ShellConfig shell_cfg2 = {
  (BaseChannel *)&SD2,
  commands
};

/*
 * Console print server done using synchronous messages. This makes the access
 * to the C printf() thread safe and the print operation atomic among threads.
 * In this example the message is the zero termitated string itself.
 */
static msg_t console_thread(void *arg) {

  (void)arg;
  while (!chThdShouldTerminate()) {
    Thread *tp = chMsgWait();
    puts((char *)chMsgGet(tp));
    fflush(stdout);
    chMsgRelease(tp, RDY_OK);
  }
  return 0;
}

/**
 * @brief Shell termination handler.
 *
 * @param[in] id event id.
 */
static void termination_handler(eventid_t id) {

  (void)id;
  if (shelltp1 && chThdTerminated(shelltp1)) {
    chThdWait(shelltp1);
    shelltp1 = NULL;
    chThdSleepMilliseconds(10);
    cputs("Init: shell on SD1 terminated");
    chSysLock();
    chOQResetI(&SD1.oqueue);
    chSysUnlock();
  }
  if (shelltp2 && chThdTerminated(shelltp2)) {
    chThdWait(shelltp2);
    shelltp2 = NULL;
    chThdSleepMilliseconds(10);
    cputs("Init: shell on SD2 terminated");
    chSysLock();
    chOQResetI(&SD2.oqueue);
    chSysUnlock();
  }
}

/**
 * @brief SD1 status change handler.
 *
 * @param[in] id event id.
 */
static void sd1_handler(eventid_t id) {
  ioflags_t flags;

  (void)id;
  flags = chIOGetAndClearFlags(&SD1);
  if ((flags & IO_CONNECTED) && (shelltp1 == NULL)) {
    cputs("Init: connection on SD1");
    shelltp1 = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO + 1);
  }
  if (flags & IO_DISCONNECTED) {
    cputs("Init: disconnection on SD1");
    chSysLock();
    chIQResetI(&SD1.iqueue);
    chSysUnlock();
  }
}

/**
 * @brief SD2 status change handler.
 *
 * @param[in] id event id.
 */
static void sd2_handler(eventid_t id) {
  ioflags_t flags;

  (void)id;
  flags = chIOGetAndClearFlags(&SD2);
  if ((flags & IO_CONNECTED) && (shelltp2 == NULL)) {
    cputs("Init: connection on SD2");
    shelltp2 = shellCreate(&shell_cfg2, SHELL_WA_SIZE, NORMALPRIO + 10);
  }
  if (flags & IO_DISCONNECTED) {
    cputs("Init: disconnection on SD2");
    chSysLock();
    chIQResetI(&SD2.iqueue);
    chSysUnlock();
  }
}

static evhandler_t fhandlers[] = {
  termination_handler,
  sd1_handler,
  sd2_handler
};

/*------------------------------------------------------------------------*
 * Simulator main.                                                        *
 *------------------------------------------------------------------------*/
int main(void) {
  EventListener sd1fel, sd2fel, tel;

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
   * Serial ports (simulated) initialization.
   */
  sdStart(&SD1, NULL);
  sdStart(&SD2, NULL);

  /*
   * Shell manager initialization.
   */
  shellInit();
  chEvtRegister(&shell_terminated, &tel, 0);

  /*
   * Console thread started.
   */
  cdtp = chThdCreateFromHeap(NULL, CONSOLE_WA_SIZE, NORMALPRIO + 1,
                             console_thread, NULL);

  /*
   * Initializing connection/disconnection events.
   */
  cputs("Shell service started on SD1, SD2");
  cputs("  - Listening for connections on SD1");
  (void) chIOGetAndClearFlags(&SD1);
  chEvtRegister(chIOGetEventSource(&SD1), &sd1fel, 1);
  cputs("  - Listening for connections on SD2");
  (void) chIOGetAndClearFlags(&SD2);
  chEvtRegister(chIOGetEventSource(&SD2), &sd2fel, 2);

  /*
   * Events servicing loop.
   */
  while (!chThdShouldTerminate())
    chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));

  /*
   * Clean simulator exit.
   */
  chEvtUnregister(chIOGetEventSource(&SD1), &sd1fel);
  chEvtUnregister(chIOGetEventSource(&SD2), &sd2fel);
  return 0;
}
