/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

#include <string.h>
#include <stdio.h>

#include <ch.h>

static uint32_t wdguard;
static WORKING_AREA(wdarea, 2048);

static uint32_t cdguard;
static WORKING_AREA(cdarea, 2048);
static Thread *cdtp;

static msg_t WatchdogThread(void *arg);
static msg_t ConsoleThread(void *arg);

msg_t TestThread(void *p);

extern FullDuplexDriver COM1, COM2;

#define cprint(msg) chMsgSend(cdtp, (msg_t)msg)

/*
 * Watchdog thread, it checks magic values located under the various stack
 * areas. The system is halted if something is wrong.
 */
static msg_t WatchdogThread(void *arg) {
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

  while (!chThdShouldTerminate()) {
    printf((char *)chMsgWait());
    fflush(stdout);
    chMsgRelease(RDY_OK);
  }
  return 0;
}

static void PrintLineFDD(FullDuplexDriver *sd, char *msg) {

  while (*msg)
    chFDDPut(sd, *msg++);
}

static bool_t GetLineFDD(FullDuplexDriver *sd, char *line, int size) {
  char *p = line;

  while (TRUE) {
    short c = chIQGet(&sd->sd_iqueue);
    if (c < 0)
      return TRUE;
    if (c == 4) {
      PrintLineFDD(sd, "^D\r\n");
      return TRUE;
    }
    if (c == 8) {
      if (p != line) {
        chFDDPut(sd, (uint8_t)c);
        chFDDPut(sd, 0x20);
        chFDDPut(sd, (uint8_t)c);
        p--;
      }
      continue;
    }
    if (c == '\r') {
      PrintLineFDD(sd, "\r\n");
      *p = 0;
      return FALSE;
    }
    if (c < 0x20)
      continue;
    if (p < line + size - 1) {
      chFDDPut(sd, (uint8_t)c);
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
  FullDuplexDriver *sd = (FullDuplexDriver *)arg;

  for (i = 0; i < 10; i++) {

    PrintLineFDD(sd, "Hello World\r\n");
    c = chFDDGetTimeout(sd, 333);
    switch (c) {
    case Q_TIMEOUT:
      continue;
    case Q_RESET:
      return 1;
    case 3:
      PrintLineFDD(sd, "^C\r\n");
      return 0;
    default:
      chThdSleep(333);
    }
  }
  return 0;
}

static bool_t checkend(FullDuplexDriver *sd) {

  char * lp = strtok(NULL, " \009"); /* It is not thread safe but this is a demo.*/
  if (lp) {
    PrintLineFDD(sd, lp);
    PrintLineFDD(sd, " ?\r\n");
    return TRUE;
  }
  return FALSE;
}

/*
 * Simple command shell thread, the argument is the serial line for the
 * standard input and output. It recognizes few simple commands.
 */
static msg_t ShellThread(void *arg) {
  FullDuplexDriver *sd = (FullDuplexDriver *)arg;
  char *lp, line[64];
  Thread *tp;
  WORKING_AREA(tarea, 2048);

  chIQReset(&sd->sd_iqueue);
  chOQReset(&sd->sd_oqueue);
  PrintLineFDD(sd, "ChibiOS/RT Command Shell\r\n\n");
  while (TRUE) {
    PrintLineFDD(sd, "ch> ");
    if (GetLineFDD(sd, line, sizeof(line))) {
      PrintLineFDD(sd, "\nlogout");
      break;
    }
    lp = strtok(line, " \009"); // Note: not thread safe but it is just a demo.
    if (lp) {
      if ((stricmp(lp, "help") == 0) ||
          (stricmp(lp, "h") == 0) ||
          (stricmp(lp, "?") == 0)) {
        if (checkend(sd))
          continue;
        PrintLineFDD(sd, "Commands:\r\n");
        PrintLineFDD(sd, "  help,h,? - This help\r\n");
        PrintLineFDD(sd, "  exit     - Logout from ChibiOS/RT\r\n");
        PrintLineFDD(sd, "  time     - Prints the system timer value\r\n");
        PrintLineFDD(sd, "  hello    - Runs the Hello World demo thread\r\n");
        PrintLineFDD(sd, "  test     - Runs the System Test thread\r\n");
      }
      else if (stricmp(lp, "exit") == 0) {
        if (checkend(sd))
          continue;
        PrintLineFDD(sd, "\nlogout");
        break;
      }
      else if (stricmp(lp, "time") == 0) {
        if (checkend(sd))
          continue;
        sprintf(line, "Time: %d\r\n", chTimeNow());
        PrintLineFDD(sd, line);
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
        PrintLineFDD(sd, lp);
        PrintLineFDD(sd, " ?\r\n");
      }
    }
  }
  return 0;
}

static WORKING_AREA(s1area, 4096);
static Thread *s1;
EventListener s1tel;

static void COM1Handler(eventid_t id) {
  dflags_t flags;

  if (s1 && chThdTerminated(s1)) {
    s1 = NULL;
    cprint("Init: disconnection on COM1\n");
  }
  flags = chFDDGetAndClearFlags(&COM1);
  if ((flags & SD_CONNECTED) && (s1 == NULL)) {
    cprint("Init: connection on COM1\n");
    s1 = chThdInit(s1area, sizeof(s1area),
                   NORMALPRIO, ShellThread, &COM1);
    chEvtRegister(chThdGetExitEventSource(s1), &s1tel, 0);
    chThdResume(s1);
  }
  if ((flags & SD_DISCONNECTED) && (s1 != NULL))
    chIQReset(&COM1.sd_iqueue);
}

static WORKING_AREA(s2area, 4096);
static Thread *s2;
EventListener s2tel;

static void COM2Handler(eventid_t id) {
  dflags_t flags;

  if (s2 && chThdTerminated(s2)) {
    s2 = NULL;
    cprint("Init: disconnection on COM2\n");
  }
  flags = chFDDGetAndClearFlags(&COM2);
  if ((flags & SD_CONNECTED) && (s2 == NULL)) {
    cprint("Init: connection on COM2\n");
    s2 = chThdInit(s2area, sizeof(s1area),
                   NORMALPRIO, ShellThread, &COM2);
    chEvtRegister(chThdGetExitEventSource(s2), &s2tel, 1);
    chThdResume(s2);
  }
  if ((flags & SD_DISCONNECTED) && (s2 != NULL))
    chIQReset(&COM2.sd_iqueue);
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

  // Startup ChibiOS/RT.
  chSysInit();

  chThdCreateStatic(wdarea, sizeof(wdarea), NORMALPRIO + 2, WatchdogThread, NULL);
  cdtp = chThdCreateStatic(cdarea, sizeof(cdarea), NORMALPRIO + 1, ConsoleThread, NULL);

  cprint("Console service started on COM1, COM2\n");
  cprint("  - Listening for connections on COM1\n");
  chFDDGetAndClearFlags(&COM1);
  chEvtRegister(&COM1.sd_sevent, &c1fel, 0);
  cprint("  - Listening for connections on COM2\n");
  chFDDGetAndClearFlags(&COM2);
  chEvtRegister(&COM2.sd_sevent, &c2fel, 1);
  while (!chThdShouldTerminate())
    chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));
  chEvtUnregister(&COM2.sd_sevent, &c2fel); // Never invoked but this is an example...
  chEvtUnregister(&COM1.sd_sevent, &c1fel); // Never invoked but this is an example...
  return 0;
}
