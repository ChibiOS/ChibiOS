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

#ifdef CH_USE_MUTEXES

#define ALLOWED_DELAY 5

static Mutex m1, m2;

static char *mtx1_gettest(void) {

  return "Mutexes, priority enqueuing test";
}

static void mtx1_setup(void) {

  chMtxInit(&m1);
}

static void mtx1_teardown(void) {
}

static msg_t thread1(void *p) {

  chMtxLock(&m1);
  test_emit_token(*(char *)p);
  chMtxUnlock();
  return 0;
}

static void mtx1_execute(void) {

  tprio_t prio = chThdGetPriority(); // Bacause priority inheritance.
  chMtxLock(&m1);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread1, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread1, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread1, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread1, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread1, "A");
  chMtxUnlock();
  test_wait_threads();
  test_assert(prio == chThdGetPriority(), "priority return failure");
  test_assert_sequence("ABCDE");
}

const struct testcase testmtx1 = {
  mtx1_gettest,
  mtx1_setup,
  mtx1_teardown,
  mtx1_execute
};

static char *mtx2_gettest(void) {

  return "Mutexes, priority inheritance, simple case";
}

static void mtx2_setup(void) {

  chMtxInit(&m1);
}

static void mtx2_teardown(void) {
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
  mtx2_teardown,
  mtx2_execute
};

static char *mtx3_gettest(void) {

  return "Mutexes, priority inheritance, complex case";
}

static void mtx3_setup(void) {

  chMtxInit(&m1);
  chMtxInit(&m2);
}

static void mtx3_teardown(void) {
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
  mtx3_teardown,
  mtx3_execute
};

#endif /* CH_USE_MUTEXES */
