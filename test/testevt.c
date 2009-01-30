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

#ifdef CH_USE_EVENTS

#define ALLOWED_DELAY MS2ST(5)

static EventSource es1, es2;

static char *evt1_gettest(void) {

  return "Events, wait and broadcast";
}

static void evt1_setup(void) {

  chEvtClear(ALL_EVENTS);
}

static void evt1_teardown(void) {
}

static msg_t thread(void *p) {

  chEvtBroadcast(&es1);
  chThdSleepMilliseconds(50);
  chEvtBroadcast(&es2);
  return 0;
}

static void evt1_execute(void) {
  eventmask_t m;
  EventListener el1, el2;
  systime_t target_time;

  /*
   * Test on chEvtWaitOne().
   */
  chEvtPend(5);
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert(m == 1, "chEvtWaitOne() error");
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert(m == 4, "chEvtWaitOne() error");
  m = chEvtClear(0);
  test_assert(m == 0, "stuck event");

  /*
   * Test on chEvtWaitAny().
   */
  chEvtPend(5);
  m = chEvtWaitAny(ALL_EVENTS);
  test_assert(m == 5, "chEvtWaitAny() error");
  m = chEvtClear(0);
  test_assert(m == 0, "stuck event");

  /*
   * Test on chEvtWaitAll(), chEvtRegisterMask() and chEvtUnregister().
   */
  chEvtInit(&es1);
  chEvtInit(&es2);
  chEvtRegisterMask(&es1, &el1, 1);
  chEvtRegisterMask(&es2, &el2, 4);
  target_time = chSysGetTime() + MS2ST(50);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread, "A");
  m = chEvtWaitAll(5);
  test_assert_time_window(target_time, target_time + ALLOWED_DELAY);
  m = chEvtClear(0);
  test_assert(m == 0, "stuck event");

  test_wait_threads();
  chEvtUnregister(&es1, &el1);
  chEvtUnregister(&es2, &el2);
  test_assert(!chEvtIsListening(&es1), "stuck listener");
  test_assert(!chEvtIsListening(&es2), "stuck listener");
}

const struct testcase testevt1 = {
  evt1_gettest,
  evt1_setup,
  evt1_teardown,
  evt1_execute
};

#endif /* CH_USE_EVENTS */
