/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    test_sequence_003.c
 * @brief   Test Sequence 003 code.
 *
 * @page test_sequence_003 [3] Suspend/Resume
 *
 * File: @ref test_sequence_003.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/RT functionalities related to
 * threads suspend/resume.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_003_001
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

static thread_reference_t tr1;

static THD_FUNCTION(thread1, p) {

  chSysLock();
  chThdResumeI(&tr1, MSG_OK);
  chSchRescheduleS();
  chSysUnlock();
  test_emit_token(*(char *)p);
}

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_003_001 [3.1] Suspend and Resume functionality
 *
 * <h2>Description</h2>
 * The functionality of chThdSuspendTimeoutS() and chThdResumeI() is
 * tested.
 *
 * <h2>Test Steps</h2>
 * - [3.1.1] The function chThdSuspendTimeoutS() is invoked, the thread
 *   is remotely resumed with message @p MSG_OK. On return the message
 *   and the state of the reference are tested.
 * - [3.1.2] The function chThdSuspendTimeoutS() is invoked, the thread
 *   is not resumed so a timeout must occur. On return the message and
 *   the state of the reference are tested.
 * .
 */

static void test_003_001_setup(void) {
  tr1 = NULL;
}

static void test_003_001_execute(void) {
  systime_t time;
  msg_t msg;

  /* [3.1.1] The function chThdSuspendTimeoutS() is invoked, the thread
     is remotely resumed with message @p MSG_OK. On return the message
     and the state of the reference are tested.*/
  test_set_step(1);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()-1, thread1, "A");
    chSysLock();
    msg = chThdSuspendTimeoutS(&tr1, TIME_INFINITE);
    chSysUnlock();
    test_assert(NULL == tr1, "not NULL");
    test_assert(MSG_OK == msg,"wrong returned message");
    test_wait_threads();
  }

  /* [3.1.2] The function chThdSuspendTimeoutS() is invoked, the thread
     is not resumed so a timeout must occur. On return the message and
     the state of the reference are tested.*/
  test_set_step(2);
  {
    chSysLock();
    time = chVTGetSystemTimeX();
    msg = chThdSuspendTimeoutS(&tr1, MS2ST(1000));
    chSysUnlock();
    test_assert_time_window(time + MS2ST(1000),
                            time + MS2ST(1000) + CH_CFG_ST_TIMEDELTA + 1,
                            "out of time window");
    test_assert(NULL == tr1, "not NULL");
    test_assert(MSG_TIMEOUT == msg, "wrong returned message");
  }
}

static const testcase_t test_003_001 = {
  "Suspend and Resume functionality",
  test_003_001_setup,
  NULL,
  test_003_001_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Suspend/Resume.
 */
const testcase_t * const test_sequence_003[] = {
  &test_003_001,
  NULL
};
