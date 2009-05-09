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

#if CH_USE_MUTEXES

#define ALLOWED_DELAY 5

static Mutex m1, m2;
static CondVar c1;

static char *mtx1_gettest(void) {

  return "Mutexes, priority enqueuing test";
}

static void mtx1_setup(void) {

  chMtxInit(&m1);
}

static msg_t thread1(void *p) {

  chMtxLock(&m1);
  test_emit_token(*(char *)p);
  chMtxUnlock();
  return 0;
}

static void mtx1_execute(void) {

  tprio_t prio = chThdGetPriority(); // Because priority inheritance.
  chMtxLock(&m1);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread1, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread1, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread1, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread1, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread1, "A");
  chMtxUnlock();
  test_wait_threads();
  test_assert(prio == chThdGetPriority(), "#1"); /* Priority return failure.*/
  test_assert_sequence("ABCDE");
}

const struct testcase testmtx1 = {
  mtx1_gettest,
  mtx1_setup,
  NULL,
  mtx1_execute
};

#if CH_DBG_THREADS_PROFILING
static char *mtx2_gettest(void) {

  return "Mutexes, priority inheritance, simple case";
}

static void mtx2_setup(void) {

  chMtxInit(&m1);
}

/* Low priority thread */
static msg_t thread2L(void *p) {

  chMtxLock(&m1);
  test_cpu_pulse(40);
  chMtxUnlock();
  test_cpu_pulse(10);
  test_emit_token('C');
  return 0;
}

/* Medium priority thread */
static msg_t thread2M(void *p) {

  chThdSleepMilliseconds(20);
  test_cpu_pulse(40);
  test_emit_token('B');
  return 0;
}

/* High priority thread */
static msg_t thread2H(void *p) {

  chThdSleepMilliseconds(40);
  chMtxLock(&m1);
  test_cpu_pulse(10);
  chMtxUnlock();
  test_emit_token('A');
  return 0;
}

/*
 * Time ----> 0     10    20    30    40    50    60    70    80    90    100
 *    0 ......AL++++++++++............2+++++++++++AU0---------------++++++G...
 *    1 ..................++++++++++++------------------++++++++++++G.........
 *    2  .............................AL..........++++++AUG...................
 */
static void mtx2_execute(void) {
  systime_t time;

  test_wait_tick();
  time = chTimeNow();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread2H, 0);
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-2, thread2M, 0);
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread2L, 0);
  test_wait_threads();
  test_assert_sequence("ABC");
  test_assert_time_window(time + MS2ST(100), time + MS2ST(100) + ALLOWED_DELAY);
}

const struct testcase testmtx2 = {
  mtx2_gettest,
  mtx2_setup,
  NULL,
  mtx2_execute
};

static char *mtx3_gettest(void) {

  return "Mutexes, priority inheritance, complex case";
}

static void mtx3_setup(void) {

  chMtxInit(&m1); // Mutex B
  chMtxInit(&m2); // Mutex A
}

/* Lowest priority thread */
static msg_t thread3LL(void *p) {

  chMtxLock(&m1);
  test_cpu_pulse(30);
  chMtxUnlock();
  test_emit_token('E');
  return 0;
}

/* Low priority thread */
static msg_t thread3L(void *p) {

  chThdSleepMilliseconds(10);
  chMtxLock(&m2);
  test_cpu_pulse(20);
  chMtxLock(&m1);
  test_cpu_pulse(10);
  chMtxUnlock();
  test_cpu_pulse(10);
  chMtxUnlock();
  test_emit_token('D');
  return 0;
}

/* Medium priority thread */
static msg_t thread3M(void *p) {

  chThdSleepMilliseconds(20);
  chMtxLock(&m2);
  test_cpu_pulse(10);
  chMtxUnlock();
  test_emit_token('C');
  return 0;
}

/* High priority thread */
static msg_t thread3H(void *p) {

  chThdSleepMilliseconds(40);
  test_cpu_pulse(20);
  test_emit_token('B');
  return 0;
}

/* Highest priority thread */
static msg_t thread3HH(void *p) {

  chThdSleepMilliseconds(50);
  chMtxLock(&m2);
  test_cpu_pulse(10);
  chMtxUnlock();
  test_emit_token('A');
  return 0;
}

/*
 * Time ----> 0     10    20    30    40    50    60    70    80    90    100   110
 *    0 ......BL++++------------2+++++------4+++++BU0---------------------------G.....
 *    1 ............AL++++2+++++BL----------4-----++++++BU4+++AU1---------------G.....
 *    2 ..................AL----------------------------------------------++++++AUG...
 *    3 ..............................+++++++-----------------------++++++G...........
 *    4 ....................................AL................++++++AUG...............
 */
static void mtx3_execute(void) {
  systime_t time;

  test_wait_tick();
  time = chTimeNow();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-5, thread3LL, 0);
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-4, thread3L, 0);
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread3M, 0);
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-2, thread3H, 0);
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread3HH, 0);
  test_wait_threads();
  test_assert_sequence("ABCDE");
  test_assert_time_window(time + MS2ST(110), time + MS2ST(110) + ALLOWED_DELAY);
}

const struct testcase testmtx3 = {
  mtx3_gettest,
  mtx3_setup,
  NULL,
  mtx3_execute
};
#endif /* CH_DBG_THREADS_PROFILING */

#if CH_USE_CONDVARS
static char *mtx4_gettest(void) {

  return "CondVar, signal test";
}

static void mtx4_setup(void) {

  chCondInit(&c1);
  chMtxInit(&m1);
}

static msg_t thread10(void *p) {

  chMtxLock(&m1);
  chCondWait(&c1);
  test_emit_token(*(char *)p);
  chMtxUnlock();
  return 0;
}

static void mtx4_execute(void) {

  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread10, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread10, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread10, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread10, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread10, "A");
  chCondSignal(&c1);
  chCondSignal(&c1);
  chCondSignal(&c1);
  chCondSignal(&c1);
  chCondSignal(&c1);
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testmtx4 = {
  mtx4_gettest,
  mtx4_setup,
  NULL,
  mtx4_execute
};

static char *mtx5_gettest(void) {

  return "CondVar, broadcast test";
}

static void mtx5_setup(void) {

  chCondInit(&c1);
  chMtxInit(&m1);
}

static void mtx5_execute(void) {

  // Bacause priority inheritance.
  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread10, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread10, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread10, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread10, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread10, "A");
  chCondBroadcast(&c1);
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testmtx5 = {
  mtx5_gettest,
  mtx5_setup,
  NULL,
  mtx5_execute
};

static char *mtx6_gettest(void) {

  return "CondVar, inheritance boost test";
}

static void mtx6_setup(void) {

  chCondInit(&c1);
  chMtxInit(&m1);
  chMtxInit(&m2);
}

static msg_t thread11(void *p) {

  chMtxLock(&m2);
  chMtxLock(&m1);
  chCondWait(&c1);
  test_emit_token(*(char *)p);
  chMtxUnlock();
  chMtxUnlock();
  return 0;
}

static msg_t thread12(void *p) {

  chMtxLock(&m2);
  test_emit_token(*(char *)p);
  chMtxUnlock();
  return 0;
}

static void mtx6_execute(void) {

  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread11, "A");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread10, "C");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread12, "B");
  chCondSignal(&c1);
  chCondSignal(&c1);
  test_wait_threads();
  test_assert_sequence("ABC");
}

const struct testcase testmtx6 = {
  mtx6_gettest,
  mtx6_setup,
  NULL,
  mtx6_execute
};
#endif /* CH_USE_CONDVARS */
#endif /* CH_USE_MUTEXES */

/*
 * Test sequence for mutexes pattern.
 */
const struct testcase * const patternmtx[] = {
#if CH_USE_MUTEXES
  &testmtx1,
#if CH_DBG_THREADS_PROFILING
  &testmtx2,
  &testmtx3,
#endif
#if CH_USE_CONDVARS
  &testmtx4,
  &testmtx5,
  &testmtx6,
#endif
#endif
  NULL
};
