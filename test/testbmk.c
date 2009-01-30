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

#include <ch.h>

#include "test.h"

static Semaphore sem1;

static void empty(void) {}

static msg_t thread1(void *p) {
  msg_t msg;

  do {
    chMsgRelease(msg = chMsgWait());
  } while (msg);
  return 0;
}

__attribute__((noinline))
static unsigned int msg_loop_test(Thread *tp) {

  uint32_t n = 0;
  test_wait_tick();
  test_start_timer(1000);
  do {
    (void)chMsgSend(tp, 1);
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  } while (!test_timer_done);
  (void)chMsgSend(tp, 0);
  return n;
}

static char *bmk1_gettest(void) {

  return "Benchmark, context switch #1, optimal";
}

static void bmk1_execute(void) {
  uint32_t n;

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread1, NULL);
  n = msg_loop_test(threads[0]);
  chThdTerminate(threads[0]);
  test_wait_threads();
  test_print("--- Score : ");
  test_printn(n);
  test_print(" msgs/S, ");
  test_printn(n << 1);
  test_println(" ctxswc/S");
}

const struct testcase testbmk1 = {
  bmk1_gettest,
  empty,
  empty,
  bmk1_execute
};

static char *bmk2_gettest(void) {

  return "Benchmark, context switch #2, empty ready list";
}

static void bmk2_execute(void) {
  uint32_t n;

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread1, NULL);
  n = msg_loop_test(threads[0]);
  chThdTerminate(threads[0]);
  test_wait_threads();
  test_print("--- Score : ");
  test_printn(n);
  test_print(" msgs/S, ");
  test_printn(n << 1);
  test_println(" ctxswc/S");
}

const struct testcase testbmk2 = {
  bmk2_gettest,
  empty,
  empty,
  bmk2_execute
};

static msg_t thread2(void *p) {

  return (msg_t)p;
}

static char *bmk3_gettest(void) {

  return "Benchmark, context switch #3, 4 threads in ready list";
}

static void bmk3_execute(void) {
  uint32_t n;

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread1, NULL);
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-2, thread2, NULL);
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread2, NULL);
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-4, thread2, NULL);
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-5, thread2, NULL);
  n = msg_loop_test(threads[0]);
  chThdTerminate(threads[0]);
  test_wait_threads();
  test_print("--- Score : ");
  test_printn(n);
  test_print(" msgs/S, ");
  test_printn(n << 1);
  test_println(" ctxswc/S");
}

const struct testcase testbmk3 = {
  bmk3_gettest,
  empty,
  empty,
  bmk3_execute
};

static char *bmk4_gettest(void) {

  return "Benchmark, threads creation/termination, worst case";
}

static void bmk4_execute(void) {

  uint32_t n = 0;
  void *wap = wa[0];
  tprio_t prio = chThdGetPriority() - 1;
  test_wait_tick();
  test_start_timer(1000);
  do {
    chThdWait(chThdCreateStatic(wap, WA_SIZE, prio, thread2, NULL));
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  } while (!test_timer_done);
  test_print("--- Score : ");
  test_printn(n);
  test_println(" threads/S");
}

const struct testcase testbmk4 = {
  bmk4_gettest,
  empty,
  empty,
  bmk4_execute
};

static char *bmk5_gettest(void) {

  return "Benchmark, threads creation/termination, optimal";
}

static void bmk5_execute(void) {

  uint32_t n = 0;
  void *wap = wa[0];
  tprio_t prio = chThdGetPriority() + 1;
  test_wait_tick();
  test_start_timer(1000);
  do {
    chThdCreateStatic(wap, WA_SIZE, prio, thread2, NULL);
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  } while (!test_timer_done);
  test_print("--- Score : ");
  test_printn(n);
  test_println(" threads/S");
}

const struct testcase testbmk5 = {
  bmk5_gettest,
  empty,
  empty,
  bmk5_execute
};

static msg_t thread3(void *p) {

  while (!chThdShouldTerminate())
    chSemWait(&sem1);
  return 0;
}

static char *bmk6_gettest(void) {

  return "Benchmark, mass reschedulation, 5 threads";
}

static void bmk6_setup(void) {

  chSemInit(&sem1, 0);
}

static void bmk6_execute(void) {
  uint32_t n;

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread3, NULL);
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()+2, thread3, NULL);
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()+3, thread3, NULL);
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()+4, thread3, NULL);
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()+5, thread3, NULL);

  n = 0;
  test_wait_tick();
  test_start_timer(1000);
  do {
    chSemReset(&sem1, 0);
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  } while (!test_timer_done);
  test_terminate_threads();
  chSemReset(&sem1, 0);
  test_wait_threads();

  test_print("--- Score : ");
  test_printn(n);
  test_print(" reschedulations/S, ");
  test_printn(n * 6);
  test_println(" ctxswc/S");
}

const struct testcase testbmk6 = {
  bmk6_gettest,
  bmk6_setup,
  empty,
  bmk6_execute
};

static char *bmk7_gettest(void) {

  return "Benchmark, I/O Queues throughput";
}

static void bmk7_execute(void) {
  static uint8_t ib[16];
  static Queue iq;

  chIQInit(&iq, ib, sizeof(ib), NULL);
  uint32_t n = 0;
  test_wait_tick();
  test_start_timer(1000);
  do {
    chIQPutI(&iq, 0);
    chIQPutI(&iq, 1);
    chIQPutI(&iq, 2);
    chIQPutI(&iq, 3);
    (void)chIQGet(&iq);
    (void)chIQGet(&iq);
    (void)chIQGet(&iq);
    (void)chIQGet(&iq);
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  } while (!test_timer_done);
  test_print("--- Score : ");
  test_printn(n * 4);
  test_println(" bytes/S");
}

const struct testcase testbmk7 = {
  bmk7_gettest,
  empty,
  empty,
  bmk7_execute
};

static char *bmk8_gettest(void) {

  return "Benchmark, virtual timers set/reset";
}

static void tmo(void *param) {}

static void bmk8_execute(void) {
  static VirtualTimer vt1, vt2;
  uint32_t n = 0;

  test_wait_tick();
  test_start_timer(1000);
  do {
    chSysLock();
    chVTSetI(&vt1, 1, tmo, NULL);
    chVTSetI(&vt2, 10000, tmo, NULL);
    chVTResetI(&vt1);
    chVTResetI(&vt2);
    chSysUnlock();
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  } while (!test_timer_done);
  test_print("--- Score : ");
  test_printn(n * 2);
  test_println(" timers/S");
}

const struct testcase testbmk8 = {
  bmk8_gettest,
  empty,
  empty,
  bmk8_execute
};
