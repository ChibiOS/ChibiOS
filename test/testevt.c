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

#if CH_USE_EVENTS

#define ALLOWED_DELAY MS2ST(5)

static EventSource es1, es2;

static char *evt1_gettest(void) {

  return "Events, registration and dispatch";
}

static void evt1_setup(void) {

  chEvtClear(ALL_EVENTS);
}

static void h1(eventid_t id) {test_emit_token('A');}
static void h2(eventid_t id) {test_emit_token('B');}
static void h3(eventid_t id) {test_emit_token('C');}
static const evhandler_t evhndl[] = {h1, h2, h3};

static void evt1_execute(void) {
  EventListener el1, el2;

  /*
   * Testing chEvtRegisterMask() and chEvtUnregister().
   */
  chEvtInit(&es1);
  chEvtRegisterMask(&es1, &el1, 1);
  chEvtRegisterMask(&es1, &el2, 2);
  test_assert(chEvtIsListening(&es1), "#1"); /* Must not be empty */
  chEvtUnregister(&es1, &el1);
  test_assert(chEvtIsListening(&es1), "#2"); /* Must not be empty */
  chEvtUnregister(&es1, &el2);
  test_assert(!chEvtIsListening(&es1), "#3"); /* Stuck listener.*/

  /*
   * Testing chEvtDispatch().
   */
  chEvtDispatch(evhndl, 7);
  test_assert_sequence("ABC");
}

const struct testcase testevt1 = {
  evt1_gettest,
  evt1_setup,
  NULL,
  evt1_execute
};

static char *evt2_gettest(void) {

  return "Events, wait and broadcast";
}

static void evt2_setup(void) {

  chEvtClear(ALL_EVENTS);
}

static msg_t thread1(void *p) {

  chThdSleepMilliseconds(50);
  chEvtSignal((Thread *)p, 1);
  return 0;
}

static msg_t thread2(void *p) {

  chEvtBroadcast(&es1);
  chThdSleepMilliseconds(50);
  chEvtBroadcast(&es2);
  return 0;
}

static void evt2_execute(void) {
  eventmask_t m;
  EventListener el1, el2;
  systime_t target_time;

  /*
   * Test on chEvtWaitOne() without wait.
   */
  chEvtPend(5);
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert(m == 1, "#1"); /* Single bit error.*/
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert(m == 4, "#2"); /* Single bit error.*/
  m = chEvtClear(0);
  test_assert(m == 0, "#3"); /* Stuck bit.*/
  
  /*
   * Test on chEvtWaitOne() with wait.
   */
  test_wait_tick();
  target_time = chTimeNow() + MS2ST(50);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() - 1,
                                 thread1, chThdSelf());
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert_time_window(target_time, target_time + ALLOWED_DELAY);
  test_assert(m == 1, "#5"); /* Single bit error.*/
  m = chEvtClear(0);
  test_assert(m == 0, "#6"); /* Stuck bit.*/
  test_wait_threads();

  /*
   * Test on chEvtWaitAny() without wait.
   */
  chEvtPend(5);
  m = chEvtWaitAny(ALL_EVENTS);
  test_assert(m == 5, "#7"); /* Unexpected pending bit.*/
  m = chEvtClear(0);
  test_assert(m == 0, "#8"); /* Stuck bit.*/

  /*
   * Test on chEvtWaitAny() with wait.
   */
  test_wait_tick();
  target_time = chTimeNow() + MS2ST(50);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() - 1,
                                 thread1, chThdSelf());
  m = chEvtWaitAny(ALL_EVENTS);
  test_assert_time_window(target_time, target_time + ALLOWED_DELAY);
  test_assert(m == 1, "#9"); /* Single bit error.*/
  m = chEvtClear(0);
  test_assert(m == 0, "#10"); /* Stuck bit.*/
  test_wait_threads();

  /*
   * Test on chEvtWaitAll().
   */
  chEvtInit(&es1);
  chEvtInit(&es2);
  chEvtRegisterMask(&es1, &el1, 1);
  chEvtRegisterMask(&es2, &el2, 4);
  test_wait_tick();
  target_time = chTimeNow() + MS2ST(50);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() - 1,
                                 thread2, "A");
  m = chEvtWaitAll(5);
  test_assert_time_window(target_time, target_time + ALLOWED_DELAY);
  m = chEvtClear(0);
  test_assert(m == 0, "#11"); /* Stuck event.*/
  test_wait_threads();
  chEvtUnregister(&es1, &el1);
  chEvtUnregister(&es2, &el2);
  test_assert(!chEvtIsListening(&es1), "#12"); /* Stuck listener.*/
  test_assert(!chEvtIsListening(&es2), "#13"); /* Stuck listener.*/
}

const struct testcase testevt2 = {
  evt2_gettest,
  evt2_setup,
  NULL,
  evt2_execute
};

#if CH_USE_EVENTS_TIMEOUT
static char *evt3_gettest(void) {

  return "Events, timeouts";
}

static void evt3_setup(void) {

  chEvtClear(ALL_EVENTS);
}

static void evt3_execute(void) {
  eventmask_t m;

  /*
   * Tests various timeout situations.
   */
  m = chEvtWaitOneTimeout(ALL_EVENTS, TIME_IMMEDIATE);
  test_assert(m == 0, "#1");
  m = chEvtWaitAnyTimeout(ALL_EVENTS, TIME_IMMEDIATE);
  test_assert(m == 0, "#2");
  m = chEvtWaitAllTimeout(ALL_EVENTS, TIME_IMMEDIATE);
  test_assert(m == 0, "#3");
  m = chEvtWaitOneTimeout(ALL_EVENTS, 10);
  test_assert(m == 0, "#4");
  m = chEvtWaitAnyTimeout(ALL_EVENTS, 10);
  test_assert(m == 0, "#5");
  m = chEvtWaitAllTimeout(ALL_EVENTS, 10);
  test_assert(m == 0, "#6");
#endif
}

const struct testcase testevt3 = {
  evt3_gettest,
  evt3_setup,
  NULL,
  evt3_execute
};

#endif /* CH_USE_EVENTS */

/*
 * Test sequence for events pattern.
 */
const struct testcase * const patternevt[] = {
#if CH_USE_EVENTS
  &testevt1,
  &testevt2,
#if CH_USE_EVENTS_TIMEOUT
  &testevt3,
#endif
#endif
  NULL
};
