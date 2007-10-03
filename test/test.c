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

#if defined(WIN32) && defined(_DEBUG)
static BYTE8 wsT1[UserStackSize(512)];
static BYTE8 wsT2[UserStackSize(512)];
static BYTE8 wsT3[UserStackSize(512)];
static BYTE8 wsT4[UserStackSize(512)];
static BYTE8 wsT5[UserStackSize(512)];
#else
static BYTE8 wsT1[UserStackSize(64)];
static BYTE8 wsT2[UserStackSize(64)];
static BYTE8 wsT3[UserStackSize(64)];
static BYTE8 wsT4[UserStackSize(64)];
static BYTE8 wsT5[UserStackSize(64)];
#endif
static Thread *t1, *t2, *t3, *t4, *t5;

static FullDuplexDriver *comp;
static Semaphore sem1, sem2;

static void wait(void) {

  chThdWait(t1);
  chThdWait(t2);
  chThdWait(t3);
  chThdWait(t4);
  chThdWait(t5);
}

static void println(char *msgp) {

  while (*msgp)
    chFDDPut(comp, *msgp++);
  chFDDPut(comp, '\r');
  chFDDPut(comp, '\n');
}

t_msg Thread1(void *p) {

  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread2(void *p) {

  chSemWait(&sem1);
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread3(void *p) {

  chSemRaisePrioWait(&sem1);
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread4(void *p) {

  chSemWait(&sem1);
  chFDDPut(comp, *(BYTE8 *)p);
  /*
   * NOTE: chSemSignalWait() is not the same of chSemSignal()+chSemWait().
   *       The former is performed atomically, try it.
   */
  chSemSignalWait(&sem1, &sem2);
//  chSemSignal(&sem1);
//  chSemWait(&sem2);
  chFDDPut(comp, *(BYTE8 *)p);
  chSemSignal(&sem2);
  return 0;
}

t_msg Thread5(void *p) {

  chSemWait(&sem1);
  chFDDPut(comp, *(BYTE8 *)p);
  chSemRaisePrioSignalWait(&sem1, &sem2);
//  chSemSignal(&sem1);
//  chSemRaisePrioWait(&sem2);
  chFDDPut(comp, *(BYTE8 *)p);
  chSemLowerPrioSignal(&sem2);
  return 0;
}

t_msg Thread6(void *p) {
  t_msg msg;
  int i;

  for (i = 0; i < 5; i++) {
    msg = chMsgSend(p, 'A' + i);
    chFDDPut(comp, msg);
  }
  chMsgSend(p, 0);
  return 0;
}

t_msg Thread7(void *p) {

  // NOTE, this thread does not serve messages this causes the client to
  // timeout.
  chThdSleep(3000);
  return 0;
}

/**
 * Tester thread, this thread must be created with priority \p NORMALPRIO.
 */
t_msg TestThread(void *p) {
  t_msg msg;
  int i;

  comp = p;

  /*
   * Ready list ordering test.
   */
  println("*** Ready List, priority enqueuing test #1, you should read ABCDE:");
  t5 = chThdCreate(NORMALPRIO-5, 0, wsT5, sizeof(wsT5), Thread1, "E");
  t4 = chThdCreate(NORMALPRIO-4, 0, wsT4, sizeof(wsT4), Thread1, "D");
  t3 = chThdCreate(NORMALPRIO-3, 0, wsT3, sizeof(wsT3), Thread1, "C");
  t2 = chThdCreate(NORMALPRIO-2, 0, wsT2, sizeof(wsT2), Thread1, "B");
  t1 = chThdCreate(NORMALPRIO-1, 0, wsT1, sizeof(wsT1), Thread1, "A");
  wait();
  println("");
  println("*** Ready List, priority enqueuing test #2, you should read ABCDE:");
  t4 = chThdCreate(NORMALPRIO-4, 0, wsT4, sizeof(wsT4), Thread1, "D");
  t5 = chThdCreate(NORMALPRIO-5, 0, wsT5, sizeof(wsT5), Thread1, "E");
  t1 = chThdCreate(NORMALPRIO-1, 0, wsT1, sizeof(wsT1), Thread1, "A");
  t2 = chThdCreate(NORMALPRIO-2, 0, wsT2, sizeof(wsT2), Thread1, "B");
  t3 = chThdCreate(NORMALPRIO-3, 0, wsT3, sizeof(wsT3), Thread1, "C");
  wait();
  println("");

  /*
   * Semaphores test.
   */
  chSemInit(&sem1, 0);
  println("*** Semaphores, FIFO enqueuing test, you should read ABCDE:");
  t1 = chThdCreate(NORMALPRIO+5, 0, wsT1, sizeof(wsT1), Thread2, "A");
  t2 = chThdCreate(NORMALPRIO+1, 0, wsT2, sizeof(wsT2), Thread2, "B");
  t3 = chThdCreate(NORMALPRIO+3, 0, wsT3, sizeof(wsT3), Thread2, "C");
  t4 = chThdCreate(NORMALPRIO+4, 0, wsT4, sizeof(wsT4), Thread2, "D");
  t5 = chThdCreate(NORMALPRIO+2, 0, wsT5, sizeof(wsT5), Thread2, "E");
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, priority enqueuing test #1, you should read ABCDE:");
  t5 = chThdCreate(NORMALPRIO+1, 0, wsT5, sizeof(wsT5), Thread3, "E");
  t4 = chThdCreate(NORMALPRIO+2, 0, wsT4, sizeof(wsT4), Thread3, "D");
  t3 = chThdCreate(NORMALPRIO+3, 0, wsT3, sizeof(wsT3), Thread3, "C");
  t2 = chThdCreate(NORMALPRIO+4, 0, wsT2, sizeof(wsT2), Thread3, "B");
  t1 = chThdCreate(NORMALPRIO+5, 0, wsT1, sizeof(wsT1), Thread3, "A");
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, priority enqueuing test #2, you should read ABCDE:");
  t4 = chThdCreate(NORMALPRIO+2, 0, wsT4, sizeof(wsT4), Thread3, "D");
  t5 = chThdCreate(NORMALPRIO+1, 0, wsT5, sizeof(wsT5), Thread3, "E");
  t1 = chThdCreate(NORMALPRIO+5, 0, wsT1, sizeof(wsT1), Thread3, "A");
  t2 = chThdCreate(NORMALPRIO+4, 0, wsT2, sizeof(wsT2), Thread3, "B");
  t3 = chThdCreate(NORMALPRIO+3, 0, wsT3, sizeof(wsT3), Thread3, "C");
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, atomicity test #1, you should read ABCDEABCDE:");
  chSemInit(&sem1, 0);
  chSemInit(&sem2, 1);
  t1 = chThdCreate(NORMALPRIO+1, 0, wsT1, sizeof(wsT1), Thread4, "A");
  t2 = chThdCreate(NORMALPRIO+2, 0, wsT2, sizeof(wsT2), Thread4, "B");
  t3 = chThdCreate(NORMALPRIO+3, 0, wsT3, sizeof(wsT3), Thread4, "C");
  t4 = chThdCreate(NORMALPRIO+4, 0, wsT4, sizeof(wsT4), Thread4, "D");
  t5 = chThdCreate(NORMALPRIO+5, 0, wsT5, sizeof(wsT5), Thread4, "E");
  chSemSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, atomicity test #2, you should read ABCDEABCDE:");
  chSemInit(&sem1, 0);
  chSemInit(&sem2, 1);
  t1 = chThdCreate(NORMALPRIO+1, 0, wsT1, sizeof(wsT1), Thread4, "A");
  t2 = chThdCreate(NORMALPRIO+5, 0, wsT2, sizeof(wsT2), Thread4, "B");
  t3 = chThdCreate(NORMALPRIO+2, 0, wsT3, sizeof(wsT3), Thread4, "C");
  t4 = chThdCreate(NORMALPRIO+4, 0, wsT4, sizeof(wsT4), Thread4, "D");
  t5 = chThdCreate(NORMALPRIO+3, 0, wsT5, sizeof(wsT5), Thread4, "E");
  chSemSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, atomicity test #3, you should read AABBCCDDEE:");
  chSemInit(&sem1, 0);
  chSemInit(&sem2, 1);
  t1 = chThdCreate(NORMALPRIO+1, 0, wsT1, sizeof(wsT1), Thread5, "A");
  t2 = chThdCreate(NORMALPRIO+2, 0, wsT2, sizeof(wsT2), Thread5, "B");
  t3 = chThdCreate(NORMALPRIO+3, 0, wsT3, sizeof(wsT3), Thread5, "C");
  t4 = chThdCreate(NORMALPRIO+4, 0, wsT4, sizeof(wsT4), Thread5, "D");
  t5 = chThdCreate(NORMALPRIO+5, 0, wsT5, sizeof(wsT5), Thread5, "E");
  chSemSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, atomicity test #4, you should read AABBCCDDEE:");
  chSemInit(&sem1, 0);
  chSemInit(&sem2, 1);
  t1 = chThdCreate(NORMALPRIO+1, 0, wsT1, sizeof(wsT1), Thread5, "A");
  t2 = chThdCreate(NORMALPRIO+5, 0, wsT2, sizeof(wsT2), Thread5, "B");
  t3 = chThdCreate(NORMALPRIO+2, 0, wsT3, sizeof(wsT3), Thread5, "C");
  t4 = chThdCreate(NORMALPRIO+4, 0, wsT4, sizeof(wsT4), Thread5, "D");
  t5 = chThdCreate(NORMALPRIO+3, 0, wsT5, sizeof(wsT5), Thread5, "E");
  chSemSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, timeout test, you should read ABCDE (slowly):");
  chSemInit(&sem1, 0);
  for (i = 0; i < 5; i++) {
    chFDDPut(comp, 'A'+i);
    chSemWaitTimeout(&sem1, 500);
  }
  println("");

  /*
   * Messages test.
   */
  println("*** Messages, dispatch test, you should read AABBCCDDEE:");
  t1 = chThdCreate(NORMALPRIO-1, 0, wsT1, sizeof(wsT1), Thread6, chThdSelf());
  do {
    chMsgRelease(msg = chMsgWait());
    if (msg)
      chFDDPut(comp, msg);
  } while (msg);
  chThdWait(t1);
  println("");
  println("*** Messages, timeout test, you should read ABCDE (slowly):");
  t1 = chThdCreate(NORMALPRIO-1, 0, wsT1, sizeof(wsT1), Thread7, chThdSelf());
  for (i = 0; i < 5; i++) {
    chFDDPut(comp, 'A'+i);
    chMsgSendTimeout(t1, 'A'+i, 500);
  }
  chMsgSendTimeout(t1, 0, 500);
  chThdWait(t1);
  println("");

  println("\r\nTest complete");
  return 0;
}
