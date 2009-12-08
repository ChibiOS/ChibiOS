/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#include <string.h>
#include <stdio.h>

#include "ch.h"
#include "hal.h"

static uint32_t wdguard;
static WORKING_AREA(wdarea, 2048);

static uint32_t cdguard;
static WORKING_AREA(cdarea, 2048);
static Thread *cdtp;

static msg_t WatchdogThread(void *arg);
static msg_t ConsoleThread(void *arg);

msg_t TestThread(void *p);

#define cprint(msg) chMsgSend(cdtp, (msg_t)msg)

/*
 * Watchdog thread, it checks magic values located under the various stack
 * areas. The system is halted if something is wrong.
 */
static msg_t WatchdogThread(void *arg) {

  (void)arg;
  wdguard = 0xA51F2E3D;
  cdguard = 0xA51F2E3D;
  while (TRUE) {
    if ((wdguard != 0xA51F2E3D) ||
        (cdguard != 0xA51F2E3D)) {
      printf("Halted by watchdog");
      fflush(stdout);
      chSysHalt();
    }
    chThdSleep(50);
  }
  return 0;
}

/*
 * Console print server done using synchronous messages. This makes the access
 * to the C printf() thread safe and the print operation atomic among threads.
 * In this example the message is the zero termitated string itself.
 */
static msg_t ConsoleThread(void *arg) {

  (void)arg;
  while (!chThdShouldTerminate()) {
    printf((char *)chMsgWait());
    fflush(stdout);
    chMsgRelease(RDY_OK);
  }
  return 0;
}

static void PrintLineSD(SerialDriver *sd, char *msg) {

  while (*msg)
    sdPut(sd, *msg++);
}

static bool_t GetLineFDD(SerialDriver *sd, char *line, int size) {
  char *p = line;

  while (TRUE) {
    short c = chIQGet(&sd->d2.iqueue);
    if (c < 0)
      return TRUE;
    if (c == 4) {
      PrintLineSD(sd, "^D\r\n");
      return TRUE;
    }
    if (c == 8) {
      if (p != line) {
        sdPut(sd, (uint8_t)c);
        sdPut(sd, 0x20);
        sdPut(sd, (uint8_t)c);
        p--;
      }
      continue;
    }
    if (c == '\r') {
      PrintLineSD(sd, "\r\n");
      *p = 0;
      return FALSE;
    }
    if (c < 0x20)
      continue;
    if (p < line + size - 1) {
      sdPut(sd, (uint8_t)c);
      *p++ = (uint8_t)c;
    }
  }
}

/*
 * Example thread, not much to see here. It simulates the CTRL-C but there
 * are no real signals involved.
 */
static msg_t HelloWorldThread(void *arg) {
  int i;
  short c;
  SerialDriver *sd = (SerialDriver *)arg;

  for (i = 0; i < 10; i++) {

    PrintLineSD(sd, "Hello World\r\n");
    c = sdGetTimeout(sd, 333);
    switch (c) {
    case Q_TIMEOUT:
      continue;
    case Q_RESET:
      return 1;
    case 3:
      PrintLineSD(sd, "^C\r\n");
      return 0;
    default:
      chThdSleep(333);
    }
  }
  return 0;
}

static bool_t checkend(SerialDriver *sd) {

  char * lp = strtok(NULL, " \009"); /* It is not thread safe but this is a demo.*/
  if (lp) {
    PrintLineSD(sd, lp);
    PrintLineSD(sd, " ?\r\n");
    return TRUE;
  }
  return FALSE;
}

/*
 * Simple command shell thread, the argument is the serial line for the
 * standard input and output. It recognizes few simple commands.
 */
static msg_t ShellThread(void *arg) {
  SerialDriver *sd = (SerialDriver *)arg;
  char *lp, line[64];
  Thread *tp;
  WORKING_AREA(tarea, 2048);

  chSysLock();
  chIQResetI(&sd->d2.iqueue);
  chOQResetI(&sd->d2.oqueue);
  chSysUnlock();
  PrintLineSD(sd, "ChibiOS/RT Command Shell\r\n\n");
  while (TRUE) {
    PrintLineSD(sd, "ch> ");
    if (GetLineFDD(sd, line, sizeof(line))) {
      PrintLineSD(sd, "\nlogout");
      break;
    }
    lp = strtok(line, " \009"); // Note: not thread safe but it is just a demo.
    if (lp) {
      if ((stricmp(lp, "help") == 0) ||
          (stricmp(lp, "h") == 0) ||
          (stricmp(lp, "?") == 0)) {
        if (checkend(sd))
          continue;
        PrintLineSD(sd, "Commands:\r\n");
        PrintLineSD(sd, "  help,h,? - This help\r\n");
        PrintLineSD(sd, "  exit     - Logout from ChibiOS/RT\r\n");
        PrintLineSD(sd, "  time     - Prints the system timer value\r\n");
        PrintLineSD(sd, "  hello    - Runs the Hello World demo thread\r\n");
        PrintLineSD(sd, "  test     - Runs the System Test thread\r\n");
      }
      else if (stricmp(lp, "exit") == 0) {
        if (checkend(sd))
          continue;
        PrintLineSD(sd, "\nlogout");
        break;
      }
      else if (stricmp(lp, "time") == 0) {
        if (checkend(sd))
          continue;
        sprintf(line, "Time: %d\r\n", chTimeNow());
        PrintLineSD(sd, line);
      }
      else if (stricmp(lp, "hello") == 0) {
        if (checkend(sd))
          continue;
        tp = chThdCreateStatic(tarea, sizeof(tarea),
                               NORMALPRIO, HelloWorldThread, sd);
        if (chThdWait(tp))
          break;  // Lost connection while executing the hello thread.
      }
      else if (stricmp(lp, "test") == 0) {
        if (checkend(sd))
          continue;
        tp = chThdCreateStatic(tarea, sizeof(tarea),
                               NORMALPRIO, TestThread, arg);
        if (chThdWait(tp))
          break;  // Lost connection while executing the hello thread.
      }
      else {
        PrintLineSD(sd, lp);
        PrintLineSD(sd, " ?\r\n");
      }
    }
  }
  return 0;
}

static WORKING_AREA(s1area, 4096);
static Thread *s1;
EventListener s1tel;

static void COM1Handler(eventid_t id) {
  sdflags_t flags;

  (void)id;
  if (s1 && chThdTerminated(s1)) {
    s1 = NULL;
    cprint("Init: disconnection on SD1\n");
  }
  flags = sdGetAndClearFlags(&SD1);
  if ((flags & SD_CONNECTED) && (s1 == NULL)) {
    cprint("Init: connection on SD1\n");
    s1 = chThdInit(s1area, sizeof(s1area),
                   NORMALPRIO, ShellThread, &SD1);
    chEvtRegister(chThdGetExitEventSource(s1), &s1tel, 0);
    chThdResume(s1);
  }
  if ((flags & SD_DISCONNECTED) && (s1 != NULL)) {
    chSysLock();
    chIQResetI(&SD1.d2.iqueue);
    chSysUnlock();
  }
}

static WORKING_AREA(s2area, 4096);
static Thread *s2;
EventListener s2tel;

static void COM2Handler(eventid_t id) {
  sdflags_t flags;

  (void)id;
  if (s2 && chThdTerminated(s2)) {
    s2 = NULL;
    cprint("Init: disconnection on SD2\n");
  }
  flags = sdGetAndClearFlags(&SD2);
  if ((flags & SD_CONNECTED) && (s2 == NULL)) {
    cprint("Init: connection on SD2\n");
    s2 = chThdInit(s2area, sizeof(s1area),
                   NORMALPRIO, ShellThread, &SD2);
    chEvtRegister(chThdGetExitEventSource(s2), &s2tel, 1);
    chThdResume(s2);
  }
  if ((flags & SD_DISCONNECTED) && (s2 != NULL)) {
    chSysLock();
    chIQResetI(&SD2.d2.iqueue);
    chSysUnlock();
  }
}

static evhandler_t fhandlers[2] = {
  COM1Handler,
  COM2Handler
};

/*------------------------------------------------------------------------*
 * Simulator main, start here your threads, examples inside.              *
 *------------------------------------------------------------------------*/
int main(void) {
  EventListener c1fel, c2fel;

  /*
   * HAL initialization.
   */
  halInit();

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();

  sdStart(&SD1, NULL);
  sdStart(&SD2, NULL);

  chThdCreateStatic(wdarea, sizeof(wdarea), NORMALPRIO + 2, WatchdogThread, NULL);
  cdtp = chThdCreateStatic(cdarea, sizeof(cdarea), NORMALPRIO + 1, ConsoleThread, NULL);

  cprint("Console service started on SD1, SD2\n");
  cprint("  - Listening for connections on SD1\n");
  sdGetAndClearFlags(&SD1);
  chEvtRegister(&SD1.d2.sevent, &c1fel, 0);
  cprint("  - Listening for connections on SD2\n");
  sdGetAndClearFlags(&SD2);
  chEvtRegister(&SD2.d2.sevent, &c2fel, 1);
  while (!chThdShouldTerminate())
    chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));
  chEvtUnregister(&SD1.d2.sevent, &c2fel); // Never invoked but this is an example...
  chEvtUnregister(&SD2.d2.sevent, &c1fel); // Never invoked but this is an example...
  return 0;
}
