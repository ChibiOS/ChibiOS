/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @page test_sequence_004 Mailboxes
 *
 * File: @ref test_sequence_004.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/NIL functionalities related to
 * mailboxes.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_004_001
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#define ALLOWED_DELAY MS2ST(5)
#define MB_SIZE 4

static msg_t mb_buffer[MB_SIZE];
static MAILBOX_DECL(mb1, mb_buffer, MB_SIZE);

/****************************************************************************
 * Test cases.
 ****************************************************************************/

#if CH_CFG_USE_MAILBOXES || defined(__DOXYGEN__)
/**
 * @page test_004_001 Mailbox non-blocking tests
 *
 * <h2>Description</h2>
 * The mailbox API is tested without triggering blocking conditions.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MAILBOXES
 * .
 *
 * <h2>Test Steps</h2>
 * - Testing the mailbox size.
 * - Testing forward and backward queuing, checking for no errors.
 * .
 */

static void test_004_001_setup(void) {
  chMBObjectInit(&mb1, mb_buffer, MB_SIZE);
}

static void test_004_001_teardown(void) {
  chMBReset(&mb1);
}

static void test_004_001_execute(void) {
  msg_t msg1;

  /* Testing the mailbox size.*/
  test_set_step(1);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "wrong size");
  }

  /* Testing forward and backward queuing, checking for no errors.*/
  test_set_step(2);
  {
    unsigned i;

    for (i = 0; i < MB_SIZE - 1; i++) {
      msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
      test_assert(msg1 == MSG_OK, "wrong wake-up message");
    }
    msg1 = chMBPostAhead(&mb1, 'A', TIME_INFINITE);
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
  }
}

static const testcase_t test_004_001 = {
  "Mailbox non-blocking tests",
  test_004_001_setup,
  test_004_001_teardown,
  test_004_001_execute
};
#endif /* CH_CFG_USE_MAILBOXES */

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Mailboxes.
 */
const testcase_t * const test_sequence_004[] = {
#if CH_CFG_USE_MAILBOXES || defined(__DOXYGEN__)
  &test_004_001,
#endif
  NULL
};
