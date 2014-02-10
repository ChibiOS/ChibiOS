/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/**
 * @page test_sequence_001 Semaphores functionality
 *
 * File: @ref test_sequence_001.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/NIL functionalities related to semaphores.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_001_000
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

static semaphore_t sem1;

/****************************************************************************
 * Test cases.
 ****************************************************************************/

#if TRUE || defined(__DOXYGEN__)
/**
 * @page test_001_000 Semaphores primitives, no state change
 *
 * <h2>Description</h2>
 * Wait, Signal and Reset primitives are tested. The testing thread does not
 * trigger a state change.
 *
 * <h2>Conditions</h2>
 * None.
 *
 * <h2>Test Steps</h2>
 * - The function chSemWait() is invoked, the Semaphore counter is tested
 *   for correct value after the call.
 * - The function chSemSignal() is invoked, the Semaphore counter is tested
 *   for correct value after the call.
 * - The function chSemReset() is invoked, the Semaphore counter is tested
 *   for correct value after the call.
 * .
 */

static void test_001_000_setup(void) {

  chSemObjectInit(&sem1, 1);
}

static void test_001_000_teardown(void) {

  chSemReset(&sem1, 0);
}

static void test_001_000_execute(void) {

  /* The function chSemWait() is invoked, the Semaphore counter is tested
     for correct value after the call.*/
  test_set_step(1);
  {
    chSemWait(&sem1);
    test_assert_lock(chSemGetCounterI(&sem1) == 0,
                     "wrong counter value");
  }

  /* The function chSemSignal() is invoked, the Semaphore counter is tested
     for correct value after the call.*/
  test_set_step(2);
  {
    chSemSignal(&sem1);
    test_assert_lock(chSemGetCounterI(&sem1) == 1,
                     "wrong counter value");
  }

  /* The function chSemReset() is invoked, the Semaphore counter is tested
     for correct value after the call.*/
  test_set_step(3);
  {
    chSemReset(&sem1, 2);
    test_assert_lock(chSemGetCounterI(&sem1) == 2,
                     "wrong counter value");
  }
}

static const testcase_t test_001_000 = {
  "Brief description",
  test_001_000_setup,
  test_001_000_teardown,
  test_001_000_execute
};
#endif /* TRUE */

#if TRUE || defined(__DOXYGEN__)
/**
 * @page test_001_000 Semaphores timeout
 *
 * <h2>Description</h2>
 * Timeouts on semaphores are tested.
 *
 * <h2>Conditions</h2>
 * None.
 *
 * <h2>Test Steps</h2>
 * - The function chSemWaitTimeout() is invoked, after return the system
 *   time, the counter and the returned message are tested.
 * .
 */

static void test_001_001_setup(void) {

  chSemObjectInit(&sem1, 0);
}

static void test_001_001_teardown(void) {

  chSemReset(&sem1, 0);
}

static void test_001_001_execute(void) {
  systime_t time;
  msg_t msg;

  /* The function chSemWaitTimeout() is invoked, after return the system
     time, the counter and the returned message are tested.*/
  test_set_step(1);
  {
    time = chVTGetSystemTimeX();
    msg = chSemWaitTimeout(&sem1, 100);
    test_assert_time_window(time + 100,
                            time + 100 + 1,
                            "out of time window");
    test_assert_lock(chSemGetCounterI(&sem1) == 0,
                     "wrong counter value");
    test_assert(MSG_TIMEOUT == msg,
                "wrong timeout message");
  }
}

static const testcase_t test_001_001 = {
  "Brief description",
  test_001_001_setup,
  test_001_001_teardown,
  test_001_001_execute
};
#endif /* TRUE */

 /****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Sequence brief description.
 */
const testcase_t * const test_sequence_001[] = {
#if TRUE || defined(__DOXYGEN__)
  &test_001_000,
#endif
#if TRUE || defined(__DOXYGEN__)
  &test_001_001,
#endif
  NULL
};
