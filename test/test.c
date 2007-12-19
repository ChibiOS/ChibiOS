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
static Mutex m1, m2;

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

static void CPU(t_time ms) {

  t_time time = chSysGetTime() + ms;
  while (chSysGetTime() != time)
    ;
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

  chMtxLock(&m1);
  chFDDPut(comp, *(BYTE8 *)p);
  chMtxUnlock();
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

void testrdy1(void) {

  println("*** Ready List, priority enqueuing test #1, you should read ABCDE:");
  t5 = chThdCreate(chThdGetPriority()-5, 0, wsT5, sizeof(wsT5), Thread1, "E");
  t4 = chThdCreate(chThdGetPriority()-4, 0, wsT4, sizeof(wsT4), Thread1, "D");
  t3 = chThdCreate(chThdGetPriority()-3, 0, wsT3, sizeof(wsT3), Thread1, "C");
  t2 = chThdCreate(chThdGetPriority()-2, 0, wsT2, sizeof(wsT2), Thread1, "B");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread1, "A");
  wait();
  println("");
}

void testrdy2(void) {

  println("*** Ready List, priority enqueuing test #2, you should read ABCDE:");
  t4 = chThdCreate(chThdGetPriority()-4, 0, wsT4, sizeof(wsT4), Thread1, "D");
  t5 = chThdCreate(chThdGetPriority()-5, 0, wsT5, sizeof(wsT5), Thread1, "E");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread1, "A");
  t2 = chThdCreate(chThdGetPriority()-2, 0, wsT2, sizeof(wsT2), Thread1, "B");
  t3 = chThdCreate(chThdGetPriority()-3, 0, wsT3, sizeof(wsT3), Thread1, "C");
  wait();
  println("");
}

void testsem1(void) {

  println("*** Semaphores, FIFO enqueuing test, you should read ABCDE:");
  chSemInit(&sem1, 0);
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
}

void testsem2(void) {
  unsigned int i;

  println("*** Semaphores, timeout test, you should read ABCDE (slowly):");
  chSemInit(&sem1, 0);
  for (i = 0; i < 5; i++) {
    chFDDPut(comp, 'A' + i);
    chSemWaitTimeout(&sem1, 500);
  }
  println("");
}

void testmtx1(void) {

  chMtxInit(&m1);
  println("*** Mutexes, priority enqueuing test, you should read ABCDE:");
  chMtxLock(&m1);
  t5 = chThdCreate(chThdGetPriority()+1, 0, wsT5, sizeof(wsT5), Thread3, "E");
  t4 = chThdCreate(chThdGetPriority()+3, 0, wsT4, sizeof(wsT4), Thread3, "D");
  t3 = chThdCreate(chThdGetPriority()+3, 0, wsT3, sizeof(wsT3), Thread3, "C");
  t2 = chThdCreate(chThdGetPriority()+4, 0, wsT2, sizeof(wsT2), Thread3, "B");
  t1 = chThdCreate(chThdGetPriority()+5, 0, wsT1, sizeof(wsT1), Thread3, "A");
  chMtxUnlock();
  wait();
  println("");
}

t_msg Thread8(void *p) {

  chThdSleep(5);
  chMtxLock(&m1);
  chMtxUnlock();
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread9(void *p) {

  chMtxLock(&m1);
  chThdSleep(20);
  chMtxUnlock();
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread10(void *p) {

  chThdSleep(10);
  CPU(50);
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread11(void *p) {

  chThdSleep(5);
  chSemWait(&sem1);
  chSemSignal(&sem1);
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread12(void *p) {

  chSemWait(&sem1);
  chThdSleep(20);
  chSemSignal(&sem1);
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

/*
 * Time
 *    0 ++++++++++++++++++AL+....2++++++++++++++AU0------------------------------
 *    1 .....................++--------------------------------------------------
 *    2 .......................++AL.............+++++++++AU++++++++++++++++++++++
 */
void testmtx2(void) {

  chMtxInit(&m1);
  println("*** Mutexes, mutex with inheritance (simple case), you should read ABC:");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread8, "A");
  t2 = chThdCreate(chThdGetPriority()-3, 0, wsT2, sizeof(wsT2), Thread9, "C");
  t3 = chThdCreate(chThdGetPriority()-2, 0, wsT3, sizeof(wsT3), Thread10, "B");
  chThdWait(t1);
  chThdWait(t2);
  chThdWait(t3);
  println("");
}

void testmtx3(void) {

  chSemInit(&sem1, 1);
  println("*** Mutexes, mutex without inheritance, inversion happens, you should read BAC:");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread11, "A");
  t2 = chThdCreate(chThdGetPriority()-3, 0, wsT2, sizeof(wsT2), Thread12, "C");
  t3 = chThdCreate(chThdGetPriority()-2, 0, wsT3, sizeof(wsT3), Thread10, "B");
  chThdWait(t1);
  chThdWait(t2);
  chThdWait(t3);
  println("");
}

t_msg Thread13(void *p) {

  chMtxLock(&m1);
  CPU(50);
  chMtxUnlock();
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread14(void *p) {

  chThdSleep(10);
  chMtxLock(&m2);
  CPU(20);
  chMtxLock(&m1);
  CPU(50);
  chMtxUnlock();
  CPU(20);
  chMtxUnlock();
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread15(void *p) {

  chThdSleep(20);
  chMtxLock(&m2);
  CPU(50);
  chMtxUnlock();
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread16(void *p) {

  chThdSleep(40);
  CPU(200);
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

t_msg Thread17(void *p) {

  chThdSleep(50);
  chMtxLock(&m2);
  CPU(50);
  chMtxUnlock();
  chFDDPut(comp, *(BYTE8 *)p);
  return 0;
}

/*
 * Time    0     10    20        30   40    50
 *    0 +++BL++------------------2++++------4+++++BU0--------------------------
 *    1 .......++AL++--2+++++++++BL.........4.....++++++++BU4++++AU1-----------
 *    2 .............++AL............................................------++AU
 *    3 ..............................++++-------------------------------++....
 *    4 ..................................++AL...................++++AU++......
 */
void testmtx4(void) {

  chMtxInit(&m1); /* B */
  chMtxInit(&m2); /* A */
  println("*** Mutexes, mutex with inheritance (complex case), you should read ABCDE:");
  t1 = chThdCreate(chThdGetPriority()-5, 0, wsT1, sizeof(wsT1), Thread13, "E");
  t2 = chThdCreate(chThdGetPriority()-4, 0, wsT2, sizeof(wsT2), Thread14, "D");
  t3 = chThdCreate(chThdGetPriority()-3, 0, wsT3, sizeof(wsT3), Thread15, "C");
  t4 = chThdCreate(chThdGetPriority()-2, 0, wsT4, sizeof(wsT4), Thread16, "B");
  t5 = chThdCreate(chThdGetPriority()-1, 0, wsT5, sizeof(wsT5), Thread17, "A");
  wait();
  println("");
}

void testmsg1(void) {
  t_msg msg;

  println("*** Messages, dispatch test, you should read AABBCCDDEE:");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread4, chThdSelf());
  do {
    chMsgRelease(msg = chMsgWait());
    if (msg)
      chFDDPut(comp, msg);
  } while (msg);
  chThdWait(t1);
  println("");
}

void testmsg2(void) {
  unsigned int i;

  println("*** Messages, timeout test, you should read ABCDE (slowly):");
  t1 = chThdCreate(chThdGetPriority()-1, 0, wsT1, sizeof(wsT1), Thread5, chThdSelf());
  for (i = 0; i < 5; i++) {
    chFDDPut(comp, 'A' + i);
    chMsgSendTimeout(t1, 'A' + i, 500);
  }
  chMsgSendTimeout(t1, 0, 500);
  chThdWait(t1);
  println("");
}

void bench1(void) {
  unsigned int i;
  t_time time;

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
}

void bench2(void) {
  unsigned int i;
  t_time time;

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
}

void bench3(void) {
  static BYTE8 ib[16];
  static Queue iq;
  unsigned int i;
  t_time time;

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
  println(" bytes/S");
}

/**
 * Tester thread, this thread must be created with priority \p NORMALPRIO.
 */
t_msg TestThread(void *p) {

  comp = p;
  println("*****************************");
  println("*** ChibiOS/RT test suite ***");
  println("*****************************");
  println("");

  /*
   * Ready list ordering tests.
   */
  testrdy1();
  testrdy2();

  /*
   * Semaphores tests.
   */
  testsem1();
  testsem2();

  /*
   * Mutexes tests.
   */
  testmtx1();
  testmtx2();
  testmtx3();
  testmtx4();

  /*
   * Messages tests.
   */
  testmsg1();
  testmsg2();

  /*
   * Kernel benchmarks.
   */
  bench1();
  bench2();
  bench3();

  println("\r\nTest complete");
  return 0;
}
