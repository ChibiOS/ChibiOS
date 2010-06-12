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
 * @page test_events Events test
 *
 * File: @ref testevt.c
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref events subsystem.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref events subsystem.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_EVENTS
 * - @p CH_USE_EVENTS_TIMEOUT
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_events_001
 * - @subpage test_events_002
 * - @subpage test_events_003
 * .
 * @file testevt.c
 * @brief Events test source file
 * @file testevt.h
 * @brief Events test header file
 */

#if CH_USE_EVENTS

#define ALLOWED_DELAY MS2ST(5)

/*
 * Note, the static initializers are not really required because the
 * variables are explicitly initialized in each test case. It is done in order
 * to test the macros.
 */
static EVENTSOURCE_DECL(es1);
static EVENTSOURCE_DECL(es2);

/**
 * @page test_events_001 Events registration and dispatch
 *
 * <h2>Description</h2>
 * Two event listeners are registered on an event source and then unregistered
 * in the same order.<br>
 * The test expects that the even source has listeners after the registrations
 * and after the first unregistration, then, after the second unegistration,
 * the test expects no more listeners.<br>
 * In the second part the test dispatches three event flags and verifies that
 * the associated event handlers are invoked in LSb-first order.
 */

static char *evt1_gettest(void) {

  return "Events, registration and dispatch";
}

static void evt1_setup(void) {

  chEvtClear(ALL_EVENTS);
}

static void h1(eventid_t id) {(void)id;test_emit_token('A');}
static void h2(eventid_t id) {(void)id;test_emit_token('B');}
static void h3(eventid_t id) {(void)id;test_emit_token('C');}
static const evhandler_t evhndl[] = {h1, h2, h3};

static void evt1_execute(void) {
  EventListener el1, el2;

  /*
   * Testing chEvtRegisterMask() and chEvtUnregister().
   */
  chEvtInit(&es1);
  chEvtRegisterMask(&es1, &el1, 1);
  chEvtRegisterMask(&es1, &el2, 2);
  test_assert(1, chEvtIsListening(&es1), "no listener");
  chEvtUnregister(&es1, &el1);
  test_assert(2, chEvtIsListening(&es1), "no listener");
  chEvtUnregister(&es1, &el2);
  test_assert(3, !chEvtIsListening(&es1), "stuck listener");

  /*
   * Testing chEvtDispatch().
   */
  chEvtDispatch(evhndl, 7);
  test_assert_sequence(4, "ABC");
}

const struct testcase testevt1 = {
  evt1_gettest,
  evt1_setup,
  NULL,
  evt1_execute
};

/**
 * @page test_events_002 Events wait and broadcast
 *
 * <h2>Description</h2>
 * In this test the following APIs are indipently tested by starting threads
 * that signal/broadcast events after fixed delays:
 * - @p chEvtWaitOne()
 * - @p chEvtWaitAny()
 * - @p chEvtWaitAll()
 * .
 * After each test phase the test verifies that the events have been served at
 * the expected time and that there are no stuck event flags.
 */

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

  (void)p;
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
  test_assert(1, m == 1, "single event error");
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert(2, m == 4, "single event error");
  m = chEvtClear(ALL_EVENTS);
  test_assert(3, m == 0, "stuck event");

  /*
   * Test on chEvtWaitOne() with wait.
   */
  test_wait_tick();
  target_time = chTimeNow() + MS2ST(50);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() - 1,
                                 thread1, chThdSelf());
  m = chEvtWaitOne(ALL_EVENTS);
  test_assert_time_window(4, target_time, target_time + ALLOWED_DELAY);
  test_assert(5, m == 1, "single event error");
  m = chEvtClear(ALL_EVENTS);
  test_assert(6, m == 0, "stuck event");
  test_wait_threads();

  /*
   * Test on chEvtWaitAny() without wait.
   */
  chEvtPend(5);
  m = chEvtWaitAny(ALL_EVENTS);
  test_assert(7, m == 5, "unexpected pending bit");
  m = chEvtClear(ALL_EVENTS);
  test_assert(8, m == 0, "stuck event");

  /*
   * Test on chEvtWaitAny() with wait.
   */
  test_wait_tick();
  target_time = chTimeNow() + MS2ST(50);
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() - 1,
                                 thread1, chThdSelf());
  m = chEvtWaitAny(ALL_EVENTS);
  test_assert_time_window(9, target_time, target_time + ALLOWED_DELAY);
  test_assert(10, m == 1, "single event error");
  m = chEvtClear(ALL_EVENTS);
  test_assert(11, m == 0, "stuck event");
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
  test_assert_time_window(12, target_time, target_time + ALLOWED_DELAY);
  m = chEvtClear(ALL_EVENTS);
  test_assert(13, m == 0, "stuck event");
  test_wait_threads();
  chEvtUnregister(&es1, &el1);
  chEvtUnregister(&es2, &el2);
  test_assert(14, !chEvtIsListening(&es1), "stuck listener");
  test_assert(15, !chEvtIsListening(&es2), "stuck listener");
}

const struct testcase testevt2 = {
  evt2_gettest,
  evt2_setup,
  NULL,
  evt2_execute
};

#if CH_USE_EVENTS_TIMEOUT
/**
 * @page test_events_003 Events timeout
 *
 * <h2>Description</h2>
 * In this test the following APIs are let to timeout twice: immediatly and
 * after 10ms:
 * In this test the following APIs are indipently tested by starting threads
 * that broadcast events after fixed delays:
 * - @p chEvtWaitOneTimeout()
 * - @p chEvtWaitAnyTimeout()
 * - @p chEvtWaitAllTimeout()
 * .
 * After each test phase the test verifies that there are no stuck event flags.
 */

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
  test_assert(1, m == 0, "spurious event");
  m = chEvtWaitAnyTimeout(ALL_EVENTS, TIME_IMMEDIATE);
  test_assert(2, m == 0, "spurious event");
  m = chEvtWaitAllTimeout(ALL_EVENTS, TIME_IMMEDIATE);
  test_assert(3, m == 0, "spurious event");
  m = chEvtWaitOneTimeout(ALL_EVENTS, 10);
  test_assert(4, m == 0, "spurious event");
  m = chEvtWaitAnyTimeout(ALL_EVENTS, 10);
  test_assert(5, m == 0, "spurious event");
  m = chEvtWaitAllTimeout(ALL_EVENTS, 10);
  test_assert(6, m == 0, "spurious event");
}

const struct testcase testevt3 = {
  evt3_gettest,
  evt3_setup,
  NULL,
  evt3_execute
};
#endif /* CH_USE_EVENTS_TIMEOUT */

/**
 * @brief   Test sequence for events.
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

#endif /* CH_USE_EVENTS */
