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

static char *mtx2_gettest(void) {

  return "Mutexes, priority inheritance, simple case";
}

static void mtx2_setup(void) {

  chMtxInit(&m1);
}

static msg_t thread2(void *p) {

  chThdSleepMilliseconds(10);
  chMtxLock(&m1);
  chMtxUnlock();
  test_emit_token(*(char *)p);
  return 0;
}

static msg_t thread3(void *p) {

  chMtxLock(&m1);
  chThdSleepMilliseconds(40);
  chMtxUnlock();
  test_emit_token(*(char *)p);
  return 0;
}

static msg_t thread4(void *p) {

  chThdSleepMilliseconds(20);
  test_cpu_pulse(50);
  test_emit_token(*(char *)p);
  return 0;
}

/*
 * Time
 *    0 ++++++++++++++++++AL+....2++++++++++++++AU0------------------------------
 *    1 .....................++--------------------------------------------------
 *    2 .......................++AL.............+++++++++AU++++++++++++++++++++++
 */
static void mtx2_execute(void) {

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread2, "A");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-3, thread3, "C");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-2, thread4, "B");
  test_wait_threads();
  test_assert_sequence("ABC");
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

  chMtxInit(&m1);
  chMtxInit(&m2);
}

static msg_t thread5(void *p) {

  chMtxLock(&m1);
  test_cpu_pulse(50);
  chMtxUnlock();
  test_emit_token(*(char *)p);
  return 0;
}

static msg_t thread6(void *p) {

  chThdSleepMilliseconds(10);
  chMtxLock(&m2);
  test_cpu_pulse(20);
  chMtxLock(&m1);
  test_cpu_pulse(50);
  chMtxUnlock();
  test_cpu_pulse(20);
  chMtxUnlock();
  test_emit_token(*(char *)p);
  return 0;
}

static msg_t thread7(void *p) {

  chThdSleepMilliseconds(20);
  chMtxLock(&m2);
  test_cpu_pulse(50);
  chMtxUnlock();
  test_emit_token(*(char *)p);
  return 0;
}

static msg_t thread8(void *p) {

  chThdSleepMilliseconds(40);
  test_cpu_pulse(200);
  test_emit_token(*(char *)p);
  return 0;
}

static msg_t thread9(void *p) {

  chThdSleepMilliseconds(50);
  chMtxLock(&m2);
  test_cpu_pulse(50);
  chMtxUnlock();
  test_emit_token(*(char *)p);
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
static void mtx3_execute(void) {

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-5, thread5, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-4, thread6, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread7, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-2, thread8, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread9, "A");
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testmtx3 = {
  mtx3_gettest,
  mtx3_setup,
  NULL,
  mtx3_execute
};

static char *mtx4_gettest(void) {

  return "Mutexes, priority return";
}

static void mtx4_setup(void) {

  chMtxInit(&m1);
  chMtxInit(&m2);
}

static msg_t thread13(void *p) {

  chThdSleepMilliseconds(50);
  chMtxLock(&m2);
  chMtxUnlock();
  return 0;
}

static msg_t thread14(void *p) {

  chThdSleepMilliseconds(150);
  chMtxLock(&m1);
  chMtxUnlock();
  return 0;
}

static void mtx4_execute(void) {
  tprio_t p, p1, p2;

  p = chThdGetPriority();
  p1 = p + 1;
  p2 = p + 2;
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, p1, thread13, "B");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, p2, thread14, "A");
  chMtxLock(&m2);
  test_assert(chThdGetPriority() == p, "#1");
  chThdSleepMilliseconds(100);
  test_assert(chThdGetPriority() == p1, "#2");
  chMtxLock(&m1);
  test_assert(chThdGetPriority() == p1, "#3");
  chThdSleepMilliseconds(100);
  test_assert(chThdGetPriority() == p2, "#4");
  chMtxUnlock();
  test_assert(chThdGetPriority() == p1, "#5");
  chThdSleepMilliseconds(100);
  test_assert(chThdGetPriority() == p1, "#6");
  chMtxUnlockAll();
  test_assert(chThdGetPriority() == p, "#7");
  test_wait_threads();

  /* Test repeated in order to cover chMtxUnlockS().*/
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, p1, thread13, "D");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, p2, thread14, "C");
  chMtxLock(&m2);
  test_assert(chThdGetPriority() == p, "#8");
  chThdSleepMilliseconds(100);
  test_assert(chThdGetPriority() == p1, "#9");
  chMtxLock(&m1);
  test_assert(chThdGetPriority() == p1, "#10");
  chThdSleepMilliseconds(100);
  test_assert(chThdGetPriority() == p2, "#11");
  chSysLock();
  chMtxUnlockS();
  chSysUnlock();
  test_assert(chThdGetPriority() == p1, "#12");
  chThdSleepMilliseconds(100);
  test_assert(chThdGetPriority() == p1, "#13");
  chMtxUnlockAll();
  test_assert(chThdGetPriority() == p, "#14");
  test_wait_threads();
}

const struct testcase testmtx4 = {
  mtx4_gettest,
  mtx4_setup,
  NULL,
  mtx4_execute
};

static char *mtx5_gettest(void) {

  return "Mutexes, coverage";
}

static void mtx5_setup(void) {

  chMtxInit(&m1);
}

static void mtx5_execute(void) {
  bool_t b;
  tprio_t prio;

  prio = chThdGetPriority();

  b = chMtxTryLock(&m1);
  test_assert(b, "#1");

  b = chMtxTryLock(&m1);
  test_assert(!b, "#2");

  chSysLock();
  chMtxUnlockS();
  chSysUnlock();

  test_assert(isempty(&m1.m_queue), "#3");            /* Queue not empty */
  test_assert(m1.m_owner == NULL, "#4");              /* Owned */
  test_assert(chThdGetPriority() == prio, "#5");
}

const struct testcase testmtx5 = {
  mtx5_gettest,
  mtx5_setup,
  NULL,
  mtx5_execute
};

#if CH_USE_CONDVARS
static char *mtx6_gettest(void) {

  return "CondVar, signal test";
}

static void mtx6_setup(void) {

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

static void mtx6_execute(void) {

  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread10, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread10, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread10, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread10, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread10, "A");
  chSysLock();
  chCondSignalI(&c1);
  chCondSignalI(&c1);
  chCondSignalI(&c1);
  chCondSignalI(&c1);
  chCondSignalI(&c1);
  chSchRescheduleS();
  chSysUnlock();
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testmtx6 = {
  mtx6_gettest,
  mtx6_setup,
  NULL,
  mtx6_execute
};

static char *mtx7_gettest(void) {

  return "CondVar, broadcast test";
}

static void mtx7_setup(void) {

  chCondInit(&c1);
  chMtxInit(&m1);
}

static void mtx7_execute(void) {

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

const struct testcase testmtx7 = {
  mtx7_gettest,
  mtx7_setup,
  NULL,
  mtx7_execute
};

static char *mtx8_gettest(void) {

  return "CondVar, inheritance boost test";
}

static void mtx8_setup(void) {

  chCondInit(&c1);
  chMtxInit(&m1);
  chMtxInit(&m2);
}

static msg_t thread11(void *p) {

  chMtxLock(&m2);
  chMtxLock(&m1);
  chCondWaitTimeout(&c1, TIME_INFINITE);
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

static void mtx8_execute(void) {

  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread11, "A");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread10, "C");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread12, "B");
  chCondSignal(&c1);
  chCondSignal(&c1);
  test_wait_threads();
  test_assert_sequence("ABC");
}

const struct testcase testmtx8 = {
  mtx8_gettest,
  mtx8_setup,
  NULL,
  mtx8_execute
};
#endif /* CH_USE_CONDVARS */
#endif /* CH_USE_MUTEXES */

/*
 * Test sequence for mutexes pattern.
 */
const struct testcase * const patternmtx[] = {
#if CH_USE_MUTEXES
  &testmtx1,
  &testmtx2,
  &testmtx3,
  &testmtx4,
  &testmtx5,
#if CH_USE_CONDVARS
  &testmtx6,
  &testmtx7,
  &testmtx8,
#endif
#endif
  NULL
};
