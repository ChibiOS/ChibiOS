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
  test_assert_sequence("ABCDE");
}

const struct testcase testthd1 = {
  thd1_gettest,
  NULL,
  NULL,
  thd1_execute
};

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
  test_assert_sequence("ABCDE");
}

const struct testcase testthd2 = {
  thd2_gettest,
  NULL,
  NULL,
  thd2_execute
};

static char *thd3_gettest(void) {

  return "Threads, priority change";
}

static void thd3_execute(void) {
  tprio_t prio, p1;

  prio = chThdGetPriority();
  p1 = chThdSetPriority(prio + 1);
  test_assert(p1 == prio, "#1");
  test_assert(chThdGetPriority() == prio + 1, "#2");
  p1 = chThdSetPriority(p1);
  test_assert(p1 == prio + 1, "#3");
  test_assert(chThdGetPriority() == prio, "#4");

#if CH_USE_MUTEXES
  /* Simulates a priority boost situation (p_prio > p_realprio).*/
  chSysLock();
  chThdSelf()->p_prio += 2;
  chSysUnlock();
  test_assert(chThdGetPriority() == prio + 2, "#5");
  
  /* Tries to raise but below the boost level. */
  p1 = chThdSetPriority(prio + 1);
  test_assert(p1 == prio, "#6");
  test_assert(chThdSelf()->p_prio == prio + 2, "#7");
  test_assert(chThdSelf()->p_realprio == prio + 1, "#8");

  /* Tries to raise above the boost level. */
  p1 = chThdSetPriority(prio + 3);
  test_assert(p1 == prio + 1, "#9");
  test_assert(chThdSelf()->p_prio == prio + 3, "#10");
  test_assert(chThdSelf()->p_realprio == prio + 3, "#11");

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

static char *thd4_gettest(void) {

  return "Threads, delays";
}

static void thd4_execute(void) {
  systime_t time;

  test_wait_tick();

  /* Timeouts in microseconds.*/
  time = chTimeNow();
  chThdSleepMicroseconds(100000);
  test_assert_time_window(time + US2ST(100000), time + US2ST(100000) + 1);

  /* Timeouts in milliseconds.*/
  time = chTimeNow();
  chThdSleepMilliseconds(100);
  test_assert_time_window(time + MS2ST(100), time + MS2ST(100) + 1);

  /* Timeouts in seconds.*/
  time = chTimeNow();
  chThdSleepSeconds(1);
  test_assert_time_window(time + S2ST(1), time + S2ST(1) + 1);

  /* Absolute timelines.*/
  time = chTimeNow() + MS2ST(100);
  chThdSleepUntil(time);
  test_assert_time_window(time, time + 1);
}

const struct testcase testthd4 = {
  thd4_gettest,
  NULL,
  NULL,
  thd4_execute
};

/*
 * Test sequence for ready list pattern.
 */
const struct testcase * const patternthd[] = {
  &testthd1,
  &testthd2,
  &testthd3,
  &testthd4,
  NULL
};
