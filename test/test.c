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

#if defined(WIN32)
void ChkIntSources(void);
#endif

#if defined(WIN32) && defined(_DEBUG)
static WorkingArea(wsT1, 512);
static WorkingArea(wsT2, 512);
static WorkingArea(wsT3, 512);
static WorkingArea(wsT4, 512);
static WorkingArea(wsT5, 512);
#else
static WorkingArea(wsT1, 64);
static WorkingArea(wsT2, 64);
static WorkingArea(wsT3, 64);
static WorkingArea(wsT4, 64);
static WorkingArea(wsT5, 64);
#endif
static Thread *t1, *t2, *t3, *t4, *t5;

static FullDuplexDriver *comp;
static Semaphore sem1;

static void wait(void) {

  chThdWait(t1);
  chThdWait(t2);
  chThdWait(t3);
  chThdWait(t4);
  chThdWait(t5);
}

static void printn(unsigned int n) {
  char buf[16], *p;

  if (!n)
    chFDDPut(comp, '0');
  else {
    p = buf;
    while (n)
          *p++ = (n % 10) + '0', n /= 10;
    while (p > buf)
      chFDDPut(comp, *--p);
  }
}

static void print(char *msgp) {

  while (*msgp)
    chFDDPut(comp, *msgp++);
}

static void println(char *msgp) {

  print(msgp);
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
  t_msg msg;
  int i;

  for (i = 0; i < 5; i++) {
    msg = chMsgSend(p, 'A' + i);
    chFDDPut(comp, msg);
  }
  chMsgSend(p, 0);
  return 0;
}

t_msg Thread5(void *p) {

  // NOTE, this thread does not serve messages this causes the client to
  // timeout.
  chThdSleep(3000);
  return 0;
}

t_msg Thread6(void *p) {

  while (!chThdShouldTerminate())
    chMsgRelease(chMsgWait() + 1);
  return 0;
}

t_msg Thread7(void *p) {

  return (unsigned int)p + 1;
}

/**
 * Tester thread, this thread must be created with priority \p NORMALPRIO.
 */
t_msg TestThread(void *p) {
  static BYTE8 ib[16];
  static Queue iq;
  t_msg msg;
  unsigned int i;
  t_time time;

  comp = p;
  println("*****************************");
  println("*** ChibiOS/RT test suite ***");
  println("*****************************");
  println("");

  /*
   * Ready list ordering test.
   */
  println("*** Ready List, priority enqueuing test #1, you should read ABCDE:");
  t5 = chThdCreate(chThdGetPriority()-5, 0, wsT5, sizeof(wsT5), Thread1, "E");
  t4 = chThdCreate(chThdGetPriority()-4, 0, wsT4, sizeof(wsT4), Thread1, "D");
  t3 = chThdCreate(chThdGetPriority()-3, 0, wsT3, sizeof(wsT3), Thread1, "C");
  t2 = chThdCreate(chThdGetPriority()-2, 0, wsT2, sizeof(wsT2), Thread1, "B");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread1, "A");
  wait();
  println("");
  println("*** Ready List, priority enqueuing test #2, you should read ABCDE:");
  t4 = chThdCreate(chThdGetPriority()-4, 0, wsT4, sizeof(wsT4), Thread1, "D");
  t5 = chThdCreate(chThdGetPriority()-5, 0, wsT5, sizeof(wsT5), Thread1, "E");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread1, "A");
  t2 = chThdCreate(chThdGetPriority()-2, 0, wsT2, sizeof(wsT2), Thread1, "B");
  t3 = chThdCreate(chThdGetPriority()-3, 0, wsT3, sizeof(wsT3), Thread1, "C");
  wait();
  println("");

  /*
   * Semaphores test.
   */
  chSemInit(&sem1, 0);
  println("*** Semaphores, FIFO enqueuing test, you should read ABCDE:");
  t1 = chThdCreate(chThdGetPriority()+5, 0, wsT1, sizeof(wsT1), Thread2, "A");
  t2 = chThdCreate(chThdGetPriority()+1, 0, wsT2, sizeof(wsT2), Thread2, "B");
  t3 = chThdCreate(chThdGetPriority()+3, 0, wsT3, sizeof(wsT3), Thread2, "C");
  t4 = chThdCreate(chThdGetPriority()+4, 0, wsT4, sizeof(wsT4), Thread2, "D");
  t5 = chThdCreate(chThdGetPriority()+2, 0, wsT5, sizeof(wsT5), Thread2, "E");
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, priority enqueuing test #1, you should read ABCDE:");
  chSemInit(&sem1, 0);
  t5 = chThdCreate(chThdGetPriority()+1, 0, wsT5, sizeof(wsT5), Thread3, "E");
  t4 = chThdCreate(chThdGetPriority()+2, 0, wsT4, sizeof(wsT4), Thread3, "D");
  t3 = chThdCreate(chThdGetPriority()+3, 0, wsT3, sizeof(wsT3), Thread3, "C");
  t2 = chThdCreate(chThdGetPriority()+4, 0, wsT2, sizeof(wsT2), Thread3, "B");
  t1 = chThdCreate(chThdGetPriority()+5, 0, wsT1, sizeof(wsT1), Thread3, "A");
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  wait();
  println("");
  println("*** Semaphores, priority enqueuing test #2, you should read ABCDE:");
  chSemInit(&sem1, 0);
  t4 = chThdCreate(chThdGetPriority()+2, 0, wsT4, sizeof(wsT4), Thread3, "D");
  t5 = chThdCreate(chThdGetPriority()+1, 0, wsT5, sizeof(wsT5), Thread3, "E");
  t1 = chThdCreate(chThdGetPriority()+5, 0, wsT1, sizeof(wsT1), Thread3, "A");
  t2 = chThdCreate(chThdGetPriority()+4, 0, wsT2, sizeof(wsT2), Thread3, "B");
  t3 = chThdCreate(chThdGetPriority()+3, 0, wsT3, sizeof(wsT3), Thread3, "C");
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
  chSemLowerPrioSignal(&sem1);
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
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread4, chThdSelf());
  do {
    chMsgRelease(msg = chMsgWait());
    if (msg)
      chFDDPut(comp, msg);
  } while (msg);
  chThdWait(t1);
  println("");
  println("*** Messages, timeout test, you should read ABCDE (slowly):");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread5, chThdSelf());
  for (i = 0; i < 5; i++) {
    chFDDPut(comp, 'A'+i);
    chMsgSendTimeout(t1, 'A'+i, 500);
  }
  chMsgSendTimeout(t1, 0, 500);
  chThdWait(t1);
  println("");

  /*
   * Kernel benchmarks.
   */
  println("*** Kernel Benchmark, context switch stress test:");
  time = chSysGetTime() + 1;
  while (chSysGetTime() < time) {
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  time += 1000;
  i = 0;
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread6, chThdSelf());
  while (chSysGetTime() < time) {
    i = chMsgSend(t1, i);
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  chThdTerminate(t1);
  chThdWait(t1);
  print("Messages throughput = ");
  printn(i);
  print(" msgs/S, ");
  printn(i << 1);
  println(" ctxsws/S");

  println("*** Kernel Benchmark, threads creation/termination:");
  time = chSysGetTime() + 1;
  while (chSysGetTime() < time) {
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  time += 1000;
  i = 0;
  while (chSysGetTime() < time) {
    t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread7, (void *)i);
    i = chThdWait(t1);
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  print("Threads throughput = ");
  printn(i);
  println(" threads/S");

  println("*** Kernel Benchmark, I/O Queues throughput:");
  chIQInit(&iq, ib, sizeof(ib), NULL);
  time = chSysGetTime() + 1;
  while (chSysGetTime() < time) {
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  time += 1000;
  i = 0;
  while (chSysGetTime() < time) {
    chIQPutI(&iq, i >> 24);
    chIQPutI(&iq, i >> 16);
    chIQPutI(&iq, i >> 8);
    chIQPutI(&iq, i);
    i = chIQGet(&iq) << 24;
    i |= chIQGet(&iq) << 16;
    i |= chIQGet(&iq) << 8;
    i |= chIQGet(&iq);
    i++;
  }
  print("Queues throughput = ");
  printn(i * 4);
  print(" bytes/S");

  println("\r\nTest complete");
  return 0;
}
