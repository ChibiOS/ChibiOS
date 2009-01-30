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

#if defined(CH_USE_CONDVARS) && defined(CH_USE_MUTEXES)

static Mutex m1;
static CondVar c1;

static char *cond1_gettest(void) {

  return "CondVar, signal test";
}

static void cond1_setup(void) {

  chCondInit(&c1);
  chMtxInit(&m1);
}

static void cond1_teardown(void) {
}

static msg_t thread1(void *p) {

  chMtxLock(&m1);
  chCondWait(&c1);
  test_emit_token(*(char *)p);
  chMtxUnlock();
  return 0;
}

static void cond1_execute(void) {

  // Bacause priority inheritance.
  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread1, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread1, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread1, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread1, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread1, "A");
  test_assert(prio == chThdGetPriority(), "priority return failure");
  chCondSignal(&c1);
  chCondSignal(&c1);
  chCondSignal(&c1);
  chCondSignal(&c1);
  chCondSignal(&c1);
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testcond1 = {
  cond1_gettest,
  cond1_setup,
  cond1_teardown,
  cond1_execute
};

static char *cond2_gettest(void) {

  return "CondVar, broadcast test";
}

static void cond2_execute(void) {

  // Bacause priority inheritance.
  tprio_t prio = chThdGetPriority();
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread1, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread1, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread1, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread1, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread1, "A");
  test_assert(prio == chThdGetPriority(), "priority return failure");
  chCondBroadcast(&c1);
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testcond2 = {
  cond2_gettest,
  cond1_setup,
  cond1_teardown,
  cond2_execute
};

#endif /* defined(CH_USE_CONDVARS) && defined(CH_USE_MUTEXES) */
