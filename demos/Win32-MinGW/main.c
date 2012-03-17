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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"

#define SHELL_WA_SIZE       THD_WA_SIZE(4096)
#define CONSOLE_WA_SIZE     THD_WA_SIZE(4096)
#define TEST_WA_SIZE        THD_WA_SIZE(4096)

#define cputs(msg) chMsgSend(cdtp, (msg_t)msg)

static Thread *cdtp;
static Thread *shelltp1;
static Thread *shelltp2;

static void cmd_mem(BaseChannel *chp, int argc, char *argv[]) {
  size_t n, size;
  char buf[52];

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: mem");
    return;
  }
  n = chHeapStatus(NULL, &size);
  sprintf(buf, "core free memory : %i bytes", chCoreStatus());
  shellPrintLine(chp, buf);
  sprintf(buf, "heap fragments   : %i", n);
  shellPrintLine(chp, buf);
  sprintf(buf, "heap free total  : %i bytes", size);
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
    sprintf(buf, "%8p %8p %4i %4i %9s %i",
            tp, tp->p_ctx.esp, tp->p_prio, tp->p_refs - 1,
            states[tp->p_state], tp->p_time);
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
    puts((char *)chMsgWait());
    fflush(stdout);
    chMsgRelease(RDY_OK);
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
