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

#include <ch.h>

#include "lpc214x.h"
#include "lpc214x_serial.h"
#include "mmcsd.h"
#include "buzzer.h"
#include "evtimer.h"

static BYTE8 waThread1[UserStackSize(32)];

static t_msg Thread1(void *arg) {

  while (TRUE) {
    IO0CLR = 0x00000800;
    chThdSleep(200);
    IO0SET = 0x00000C00;
    chThdSleep(800);
    IO0CLR = 0x00000400;
    chThdSleep(200);
    IO0SET = 0x00000C00;
    chThdSleep(800);
  }
  return 0;
}

static BYTE8 waThread2[UserStackSize(32)];

static t_msg Thread2(void *arg) {

  while (TRUE) {
    IO0CLR = 0x80000000;
    chThdSleep(200);
    IO0SET = 0x80000000;
    chThdSleep(300);
  }
  return 0;
}

static void TimerHandler(t_eventid id) {
  t_msg TestThread(void *p);

  if (!(IO0PIN & 0x00018000)) { // Both buttons
    TestThread(&COM1);
    PlaySound(500, 100);
  }
  else {
    if (!(IO0PIN & 0x00008000)) // Button 1
      PlaySound(1000, 100);
    if (!(IO0PIN & 0x00010000)) { // Button 2
      chFDDWrite(&COM1, (BYTE8 *)"Hello World!\r\n", 14);
      PlaySound(2000, 100);
    }
  }
}

static void InsertHandler(t_eventid id) {
  static BYTE8 rwbuf[512];
  MMCCSD data;

  PlaySoundWait(1000, 100);
  PlaySoundWait(2000, 100);
  if (mmcInit())
    return;
  /* Card ready, do stuff.*/
  if (mmcGetSize(&data))
    return;
  if (mmcRead(rwbuf, 0))
    return;
  PlaySound(440, 200);
}

static void RemoveHandler(t_eventid id) {

  PlaySoundWait(2000, 100);
  PlaySoundWait(1000, 100);
}

static BYTE8 waThread3[UserStackSize(256)];
static EvTimer evt;
static t_evhandler evhndl[] = {
  TimerHandler,
  InsertHandler,
  RemoveHandler
};

static t_msg Thread3(void *arg) {
  struct EventListener el0, el1, el2;

  evtInit(&evt, 500);
  evtStart(&evt);
  mmcStartPolling();
  evtRegister(&evt, &el0, 0);
  chEvtRegister(&MMCInsertEventSource, &el1, 1);
  chEvtRegister(&MMCRemoveEventSource, &el2, 2);
  while (TRUE)
    chEvtWait(ALL_EVENTS, evhndl);
  return 0;
}

int main(int argc, char **argv) {

  chSysInit();
  /*
   * If a button is pressed during the reset then the blinking leds are not
   * started in order to make accurate benchmarks.
   */
  if ((IO0PIN & 0x00018000) == 0x00018000) {
    chThdCreate(NORMALPRIO, 0, waThread1, sizeof(waThread1), Thread1, NULL);
    chThdCreate(NORMALPRIO, 0, waThread2, sizeof(waThread2), Thread2, NULL);
  }
  chThdCreate(NORMALPRIO, 0, waThread3, sizeof(waThread3), Thread3, NULL);
  chSysPause();
  return 0;
}
