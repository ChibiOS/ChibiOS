/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "test.h"

/**
 * @page test_threads Threads and Scheduler test
 *
 * File: @ref testthd.c
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref scheduler,
 * @ref threads and @ref time subsystems.<br>
 * Note that the tests on those subsystems are formally required but most of
 * their functionality is already demonstrated because the test suite itself
 * depends on them, anyway double check is good.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the subsystems code.
 *
 * <h2>Preconditions</h2>
 * None.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_threads_001
 * - @subpage test_threads_002
 * - @subpage test_threads_003
 * - @subpage test_threads_004
 * .
 * @file testthd.c
 * @brief Threads and Scheduler test source file
 * @file testthd.h
 * @brief Threads and Scheduler test header file
 */

/**
 * @page test_threads_001 Ready List functionality #1
 *
 * <h2>Description</h2>
 * Five threads, with increasing priority, are enqueued in the ready list
 * and atomically executed.<br>
 * The test expects the threads to perform their operations in increasing
 * priority order regardless of the initial order.
 */

static msg_t thread(void *p) {

  test_emit_token(*(char *)p);
  return 0;
}

static char *thd1_gettest(void) {

  return "Threads, enqueuing test #1";
}

static void thd1_execute(void) {

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-5, thread, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-4, thread, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-2, thread, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread, "A");
  test_wait_threads();
  test_assert_sequence(1, "ABCDE");
}

const struct testcase testthd1 = {
  thd1_gettest,
  NULL,
  NULL,
  thd1_execute
};

/**
 * @page test_threads_002 Ready List functionality #2
 *
 * <h2>Description</h2>
 * Five threads, with pseudo-random priority, are enqueued in the ready list
 * and atomically executed.<br>
 * The test expects the threads to perform their operations in increasing
 * priority order regardless of the initial order.
 */

static char *thd2_gettest(void) {

  return "Threads, enqueuing test #2";
}

static void thd2_execute(void) {

  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-4, thread, "D");
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-5, thread, "E");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread, "A");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-2, thread, "B");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread, "C");
  test_wait_threads();
  test_assert_sequence(1, "ABCDE");
}

const struct testcase testthd2 = {
  thd2_gettest,
  NULL,
  NULL,
  thd2_execute
};

/**
 * @page test_threads_003 Threads priority change test
 *
 * <h2>Description</h2>
 * A series of priority changes are performed on the current thread in order
 * to verify that the priority change happens as expected.<br>
 * If the @p CH_USE_MUTEXES option is enabled then the priority changes are
 * also tested under priority inheritance boosted priority state.
 */

static char *thd3_gettest(void) {

  return "Threads, priority change";
}

static void thd3_execute(void) {
  tprio_t prio, p1;

  prio = chThdGetPriority();
  p1 = chThdSetPriority(prio + 1);
  test_assert(1, p1 == prio,
              "unexpected returned priority level");
  test_assert(2, chThdGetPriority() == prio + 1,
              "unexpected priority level");
  p1 = chThdSetPriority(p1);
  test_assert(3, p1 == prio + 1,
              "unexpected returned priority level");
  test_assert(4, chThdGetPriority() == prio,
              "unexpected priority level");

#if CH_USE_MUTEXES
  /* Simulates a priority boost situation (p_prio > p_realprio).*/
  chSysLock();
  chThdSelf()->p_prio += 2;
  chSysUnlock();
  test_assert(5, chThdGetPriority() == prio + 2,
              "unexpected priority level");

  /* Tries to raise but below the boost level. */
  p1 = chThdSetPriority(prio + 1);
  test_assert(6, p1 == prio,
              "unexpected returned priority level");
  test_assert(7, chThdSelf()->p_prio == prio + 2,
              "unexpected priority level");
  test_assert(8, chThdSelf()->p_realprio == prio + 1,
              "unexpected returned real priority level");

  /* Tries to raise above the boost level. */
  p1 = chThdSetPriority(prio + 3);
  test_assert(9, p1 == prio + 1,
              "unexpected returned priority level");
  test_assert(10, chThdSelf()->p_prio == prio + 3,
              "unexpected priority level");
  test_assert(11, chThdSelf()->p_realprio == prio + 3,
              "unexpected real priority level");

  chSysLock();
  chThdSelf()->p_prio = prio;
  chThdSelf()->p_realprio = prio;
  chSysUnlock();
#endif
}

const struct testcase testthd3 = {
  thd3_gettest,
  NULL,
  NULL,
  thd3_execute
};

/**
 * @page test_threads_004 Threads delays test
 *
 * <h2>Description</h2>
 * Delay APIs and associated macros are tested, the invoking thread is verified
 * to wake up at the exact expected time.
 */

static char *thd4_gettest(void) {

  return "Threads, delays";
}

static void thd4_execute(void) {
  systime_t time;

  test_wait_tick();

  /* Timeouts in microseconds.*/
  time = chTimeNow();
  chThdSleepMicroseconds(100000);
  test_assert_time_window(1, time + US2ST(100000), time + US2ST(100000) + 1);

  /* Timeouts in milliseconds.*/
  time = chTimeNow();
  chThdSleepMilliseconds(100);
  test_assert_time_window(2, time + MS2ST(100), time + MS2ST(100) + 1);

  /* Timeouts in seconds.*/
  time = chTimeNow();
  chThdSleepSeconds(1);
  test_assert_time_window(3, time + S2ST(1), time + S2ST(1) + 1);

  /* Absolute timelines.*/
  time = chTimeNow() + MS2ST(100);
  chThdSleepUntil(time);
  test_assert_time_window(4, time, time + 1);
}

const struct testcase testthd4 = {
  thd4_gettest,
  NULL,
  NULL,
  thd4_execute
};

/**
 * @brief   Test sequence for threads.
 */
const struct testcase * const patternthd[] = {
  &testthd1,
  &testthd2,
  &testthd3,
  &testthd4,
  NULL
};
