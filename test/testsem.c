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

#if CH_USE_SEMAPHORES

#define ALLOWED_DELAY MS2ST(5)

static Semaphore sem1;

static char *sem1_gettest(void) {

  return "Semaphores, enqueuing test";
}

static void sem1_setup(void) {

  chSemInit(&sem1, 0);
}

static msg_t thread1(void *p) {

  chSemWait(&sem1);
  test_emit_token(*(char *)p);
  return 0;
}

static void sem1_execute(void) {

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+5, thread1, "A");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()+1, thread1, "B");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()+3, thread1, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()+4, thread1, "D");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()+2, thread1, "E");
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  test_wait_threads();
#if CH_USE_SEMAPHORES_PRIORITY
  test_assert_sequence(1, "ADCEB");
#else
  test_assert_sequence(1, "ABCDE");
#endif
}

const struct testcase testsem1 = {
  sem1_gettest,
  sem1_setup,
  NULL,
  sem1_execute
};

static char *sem2_gettest(void) {

  return "Semaphores, timeout test";
}

static void sem2_setup(void) {

  chSemInit(&sem1, 0);
}

static msg_t thread2(void *p) {

  chThdSleepMilliseconds(50);
  chSysLock();
  chSemSignalI(&sem1); /* For coverage reasons */
  chSchRescheduleS();
  chSysUnlock();
  return 0;
}

static void sem2_execute(void) {
  int i;
  systime_t target_time;
  msg_t msg;

  /*
   * Testing special case TIME_IMMEDIATE.
   */
  msg = chSemWaitTimeout(&sem1, TIME_IMMEDIATE);
  test_assert(1, msg == RDY_TIMEOUT, "wrong wake-up message");
  test_assert(2, isempty(&sem1.s_queue), "queue not empty");
  test_assert(3, sem1.s_cnt == 0, "counter not zero");

  /*
   * Testing not timeout condition.
   */
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() - 1,
                                 thread2, "A");
  msg = chSemWaitTimeout(&sem1, MS2ST(500));
  test_wait_threads();
  test_assert(4, msg == RDY_OK, "wrong wake-up message");
  test_assert(5, isempty(&sem1.s_queue), "queue not empty");
  test_assert(6, sem1.s_cnt == 0, "counter not zero");

  /*
   * Testing timeout condition.
   */
  test_wait_tick();
  target_time = chTimeNow() + MS2ST(5 * 500);
  for (i = 0; i < 5; i++) {
    test_emit_token('A' + i);
    msg = chSemWaitTimeout(&sem1, MS2ST(500));
    test_assert(7, msg == RDY_TIMEOUT, "wrong wake-up message");
    test_assert(8, isempty(&sem1.s_queue), "queue not empty");
    test_assert(9, sem1.s_cnt == 0, "counter not zero");
  }
  test_assert_sequence(10, "ABCDE");
  test_assert_time_window(11, target_time, target_time + ALLOWED_DELAY);
}

const struct testcase testsem2 = {
  sem2_gettest,
  sem2_setup,
  NULL,
  sem2_execute
};

#if CH_USE_SEMSW
static char *sem3_gettest(void) {

  return "Semaphores, atomic signal-wait";
}

static void sem3_setup(void) {

  chSemInit(&sem1, 0);
}

static msg_t thread3(void *p) {

  chSemWait(&sem1);
  chSemSignal(&sem1);
  return 0;
}

static void sem3_execute(void) {

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()+1, thread3, "A");
  chSemSignalWait(&sem1, &sem1);
  test_assert(1, isempty(&sem1.s_queue), "queue not empty");
  test_assert(2, sem1.s_cnt == 0, "counter not zero");

  chSemSignalWait(&sem1, &sem1);
  test_assert(3, isempty(&sem1.s_queue), "queue not empty");
  test_assert(4, sem1.s_cnt == 0, "counter not zero");
}

const struct testcase testsem3 = {
  sem3_gettest,
  sem3_setup,
  NULL,
  sem3_execute
};
#endif /* CH_USE_SEMSW */
#endif /* CH_USE_SEMAPHORES */

/*
 * Test sequence for semaphores pattern.
 */
const struct testcase * const patternsem[] = {
#if CH_USE_SEMAPHORES
  &testsem1,
  &testsem2,
#if CH_USE_SEMSW
  &testsem3,
#endif
#endif
  NULL
};
