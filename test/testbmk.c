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

#include "test.h"

__attribute__((noinline))
static unsigned int msg_loop_test(Thread *tp) {

  uint32_t n = 0;
  test_wait_tick();
  test_start_timer(1000);
  while (!test_timer_done) {
    (void)chMsgSend(tp, 0);
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  return n;
}

static char *bmk1_gettest(void) {

  return "Benchmark, context switch #1, optimal";
}

static void bmk1_setup(void) {
}

static void bmk1_teardown(void) {
}

static msg_t thread1(void *p) {

  while (!chThdShouldTerminate())
    chMsgRelease(chMsgWait());
  return 0;
}

static void bmk1_execute(void) {
  uint32_t n;

  threads[0] = chThdCreate(chThdGetPriority()-1, 0, wa[0], STKSIZE, thread1, 0);
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
  bmk1_setup,
  bmk1_teardown,
  bmk1_execute
};

static char *bmk2_gettest(void) {

  return "Benchmark, context switch #2, empty ready list";
}

static void bmk2_setup(void) {
}

static void bmk2_teardown(void) {
}

static void bmk2_execute(void) {
  uint32_t n;

  threads[0] = chThdCreate(chThdGetPriority()+1, 0, wa[0], STKSIZE, thread1, 0);
  n = msg_loop_test(threads[0]);
  chThdTerminate(threads[0]);
  chMsgSend(threads[0], 0);
  test_wait_threads();
  test_print("--- Score : ");
  test_printn(n);
  test_print(" msgs/S, ");
  test_printn(n << 1);
  test_println(" ctxswc/S");
}

const struct testcase testbmk2 = {
  bmk2_gettest,
  bmk2_setup,
  bmk2_teardown,
  bmk2_execute
};

static char *bmk3_gettest(void) {

  return "Benchmark, context switch #3, 4 threads in ready list";
}

static void bmk3_setup(void) {
}

static void bmk3_teardown(void) {
}

static msg_t thread2(void *p) {

  return 0;
}

static void bmk3_execute(void) {
  uint32_t n;

  threads[0] = chThdCreate(chThdGetPriority()+1, 0, wa[0], STKSIZE, thread1, 0);
  threads[1] = chThdCreate(chThdGetPriority()-2, 0, wa[1], STKSIZE, thread2, 0);
  threads[2] = chThdCreate(chThdGetPriority()-3, 0, wa[2], STKSIZE, thread2, 0);
  threads[3] = chThdCreate(chThdGetPriority()-4, 0, wa[3], STKSIZE, thread2, 0);
  threads[4] = chThdCreate(chThdGetPriority()-5, 0, wa[4], STKSIZE, thread2, 0);
  n = msg_loop_test(threads[0]);
  chThdTerminate(threads[0]);
  chMsgSend(threads[0], 0);
  test_wait_threads();
  test_print("--- Score : ");
  test_printn(n);
  test_print(" msgs/S, ");
  test_printn(n << 1);
  test_println(" ctxswc/S");
}

const struct testcase testbmk3 = {
  bmk3_gettest,
  bmk3_setup,
  bmk3_teardown,
  bmk3_execute
};

static char *bmk4_gettest(void) {

  return "Benchmark, threads creation/termination";
}

static void bmk4_setup(void) {
}

static void bmk4_teardown(void) {
}

static void bmk4_execute(void) {

  uint32_t n = 0;
  test_wait_tick();
  test_start_timer(1000);
  while (!test_timer_done) {
    threads[0] = chThdCreate(chThdGetPriority()-1, 0, wa[0], STKSIZE, thread2, NULL);
    chThdWait(threads[0]);
    n++;
#if defined(WIN32)
    ChkIntSources();
#endif
  }
  test_print("--- Score : ");
  test_printn(n);
  test_println(" threads/S");
}

const struct testcase testbmk4 = {
  bmk4_gettest,
  bmk4_setup,
  bmk4_teardown,
  bmk4_execute
};

static char *bmk5_gettest(void) {

  return "Benchmark, I/O Queues throughput";
}

static void bmk5_setup(void) {
}

static void bmk5_teardown(void) {
}

static void bmk5_execute(void) {
  static uint8_t ib[16];
  static Queue iq;

  chIQInit(&iq, ib, sizeof(ib), NULL);
  uint32_t n = 0;
  test_wait_tick();
  test_start_timer(1000);
  while (!test_timer_done) {
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
  }
  test_print("--- Score : ");
  test_printn(n * 4);
  test_println(" bytes/S");
}

const struct testcase testbmk5 = {
  bmk5_gettest,
  bmk5_setup,
  bmk5_teardown,
  bmk5_execute
};
