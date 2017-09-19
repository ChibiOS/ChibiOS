/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio

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
 * @file    test_sequence_005.c
 * @brief   Test Sequence 005 code.
 *
 * @page test_sequence_005 [5] Mailboxes
 *
 * File: @ref test_sequence_005.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/NIL functionalities related to
 * mailboxes.
 *
 * <h2>Conditions</h2>
 * This sequence is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MAILBOXES
 * .
 *
 * <h2>Test Cases</h2>
 * - @subpage test_005_001
 * - @subpage test_005_002
 * - @subpage test_005_003
 * .
 */

#if (CH_CFG_USE_MAILBOXES) || defined(__DOXYGEN__)

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

/**
 * @page test_005_001 [5.1] Mailbox normal API, non-blocking tests
 *
 * <h2>Description</h2>
 * The mailbox normal API is tested without triggering blocking
 * conditions.
 *
 * <h2>Test Steps</h2>
 * - [5.1.1] Testing the mailbox size.
 * - [5.1.2] Resetting the mailbox, conditions are checked, no errors
 *   expected.
 * - [5.1.3] Testing the behavior of API when the mailbox is in reset
 *   state then return in active state.
 * - [5.1.4] Filling the mailbox using chMBPost() and chMBPostAhead()
 *   once, no errors expected.
 * - [5.1.5] Testing intermediate conditions. Data pointers must be
 *   aligned, semaphore counters are checked.
 * - [5.1.6] Emptying the mailbox using chMBFetch(), no errors
 *   expected.
 * - [5.1.7] Posting and then fetching one more message, no errors
 *   expected.
 * - [5.1.8] Testing final conditions. Data pointers must be aligned to
 *   buffer start, semaphore counters are checked.
 * .
 */

static void test_005_001_setup(void) {
  chMBObjectInit(&mb1, mb_buffer, MB_SIZE);
}

static void test_005_001_teardown(void) {
  chMBReset(&mb1);
}

static void test_005_001_execute(void) {
  msg_t msg1, msg2;
  unsigned i;

  /* [5.1.1] Testing the mailbox size.*/
  test_set_step(1);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "wrong size");
  }

  /* [5.1.2] Resetting the mailbox, conditions are checked, no errors
     expected.*/
  test_set_step(2);
  {
    chMBReset(&mb1);
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "not empty");
    test_assert_lock(chMBGetUsedCountI(&mb1) == 0, "still full");
    test_assert_lock(mb1.buffer == mb1.wrptr, "write pointer not aligned to base");
    test_assert_lock(mb1.buffer == mb1.rdptr, "read pointer not aligned to base");
  }

  /* [5.1.3] Testing the behavior of API when the mailbox is in reset
     state then return in active state.*/
  test_set_step(3);
  {
    msg1 = chMBPost(&mb1, (msg_t)0, TIME_INFINITE);
    test_assert(msg1 == MSG_RESET, "not in reset state");
    msg1 = chMBPostAhead(&mb1, (msg_t)0, TIME_INFINITE);
    test_assert(msg1 == MSG_RESET, "not in reset state");
    msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
    test_assert(msg1 == MSG_RESET, "not in reset state");
    chMBResumeX(&mb1);
  }

  /* [5.1.4] Filling the mailbox using chMBPost() and chMBPostAhead()
     once, no errors expected.*/
  test_set_step(4);
  {
    for (i = 0; i < MB_SIZE - 1; i++) {
      msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
      test_assert(msg1 == MSG_OK, "wrong wake-up message");
    }
    msg1 = chMBPostAhead(&mb1, 'A', TIME_INFINITE);
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
  }

  /* [5.1.5] Testing intermediate conditions. Data pointers must be
     aligned, semaphore counters are checked.*/
  test_set_step(5);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == 0, "still empty");
    test_assert_lock(chMBGetUsedCountI(&mb1) == MB_SIZE, "not full");
    test_assert_lock(mb1.rdptr == mb1.wrptr, "pointers not aligned");
  }

  /* [5.1.6] Emptying the mailbox using chMBFetch(), no errors
     expected.*/
  test_set_step(6);
  {
    for (i = 0; i < MB_SIZE; i++) {
      msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
      test_assert(msg1 == MSG_OK, "wrong wake-up message");
      test_emit_token(msg2);
    }
    test_assert_sequence("ABCD", "wrong get sequence");
  }

  /* [5.1.7] Posting and then fetching one more message, no errors
     expected.*/
  test_set_step(7);
  {
    msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
    msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
  }

  /* [5.1.8] Testing final conditions. Data pointers must be aligned to
     buffer start, semaphore counters are checked.*/
  test_set_step(8);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "not empty");
    test_assert_lock(chMBGetUsedCountI(&mb1) == 0, "still full");
    test_assert(mb1.buffer == mb1.wrptr, "write pointer not aligned to base");
    test_assert(mb1.buffer == mb1.rdptr, "read pointer not aligned to base");
  }
}

static const testcase_t test_005_001 = {
  "Mailbox normal API, non-blocking tests",
  test_005_001_setup,
  test_005_001_teardown,
  test_005_001_execute
};

/**
 * @page test_005_002 [5.2] Mailbox I-Class API, non-blocking tests
 *
 * <h2>Description</h2>
 * The mailbox I-Class API is tested without triggering blocking
 * conditions.
 *
 * <h2>Test Steps</h2>
 * - [5.2.1] Testing the mailbox size.
 * - [5.2.2] Resetting the mailbox, conditions are checked, no errors
 *   expected.
 * - [5.2.3] Filling the mailbox using chMBPostI() and chMBPostAheadI()
 *   once, no errors expected.
 * - [5.2.4] Testing intermediate conditions. Data pointers must be
 *   aligned, semaphore counters are checked.
 * - [5.2.5] Emptying the mailbox using chMBFetchI(), no errors
 *   expected.
 * - [5.2.6] Posting and then fetching one more message, no errors
 *   expected.
 * - [5.2.7] Testing final conditions. Data pointers must be aligned to
 *   buffer start, semaphore counters are checked.
 * .
 */

static void test_005_002_setup(void) {
  chMBObjectInit(&mb1, mb_buffer, MB_SIZE);
}

static void test_005_002_teardown(void) {
  chMBReset(&mb1);
}

static void test_005_002_execute(void) {
  msg_t msg1, msg2;
  unsigned i;

  /* [5.2.1] Testing the mailbox size.*/
  test_set_step(1);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "wrong size");
  }

  /* [5.2.2] Resetting the mailbox, conditions are checked, no errors
     expected.*/
  test_set_step(2);
  {
    chSysLock();
    chMBResetI(&mb1);
    chSysUnlock();
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "not empty");
    test_assert_lock(chMBGetUsedCountI(&mb1) == 0, "still full");
    test_assert_lock(mb1.buffer == mb1.wrptr, "write pointer not aligned to base");
    test_assert_lock(mb1.buffer == mb1.rdptr, "read pointer not aligned to base");
    chMBResumeX(&mb1);
  }

  /* [5.2.3] Filling the mailbox using chMBPostI() and chMBPostAheadI()
     once, no errors expected.*/
  test_set_step(3);
  {
    for (i = 0; i < MB_SIZE - 1; i++) {
      chSysLock();
      msg1 = chMBPostI(&mb1, 'B' + i);
      chSysUnlock();
      test_assert(msg1 == MSG_OK, "wrong wake-up message");
    }
    chSysLock();
    msg1 = chMBPostAheadI(&mb1, 'A');
    chSysUnlock();
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
  }

  /* [5.2.4] Testing intermediate conditions. Data pointers must be
     aligned, semaphore counters are checked.*/
  test_set_step(4);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == 0, "still empty");
    test_assert_lock(chMBGetUsedCountI(&mb1) == MB_SIZE, "not full");
    test_assert_lock(mb1.rdptr == mb1.wrptr, "pointers not aligned");
  }

  /* [5.2.5] Emptying the mailbox using chMBFetchI(), no errors
     expected.*/
  test_set_step(5);
  {
    for (i = 0; i < MB_SIZE; i++) {
      chSysLock();
      msg1 = chMBFetchI(&mb1, &msg2);
      chSysUnlock();
      test_assert(msg1 == MSG_OK, "wrong wake-up message");
      test_emit_token(msg2);
    }
    test_assert_sequence("ABCD", "wrong get sequence");
  }

  /* [5.2.6] Posting and then fetching one more message, no errors
     expected.*/
  test_set_step(6);
  {
    msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
    msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
    test_assert(msg1 == MSG_OK, "wrong wake-up message");
  }

  /* [5.2.7] Testing final conditions. Data pointers must be aligned to
     buffer start, semaphore counters are checked.*/
  test_set_step(7);
  {
    test_assert_lock(chMBGetFreeCountI(&mb1) == MB_SIZE, "not empty");
    test_assert_lock(chMBGetUsedCountI(&mb1) == 0, "still full");
    test_assert(mb1.buffer == mb1.wrptr, "write pointer not aligned to base");
    test_assert(mb1.buffer == mb1.rdptr, "read pointer not aligned to base");
  }
}

static const testcase_t test_005_002 = {
  "Mailbox I-Class API, non-blocking tests",
  test_005_002_setup,
  test_005_002_teardown,
  test_005_002_execute
};

/**
 * @page test_005_003 [5.3] Mailbox timeouts
 *
 * <h2>Description</h2>
 * The mailbox API is tested for timeouts.
 *
 * <h2>Test Steps</h2>
 * - [5.3.1] Filling the mailbox.
 * - [5.3.2] Testing chMBPost(), chMBPostI(), chMBPostAhead() and
 *   chMBPostAheadI() timeout.
 * - [5.3.3] Resetting the mailbox.
 * - [5.3.4] Testing chMBFetch() and chMBFetchI() timeout.
 * .
 */

static void test_005_003_setup(void) {
  chMBObjectInit(&mb1, mb_buffer, MB_SIZE);
}

static void test_005_003_teardown(void) {
  chMBReset(&mb1);
}

static void test_005_003_execute(void) {
  msg_t msg1, msg2;
  unsigned i;

  /* [5.3.1] Filling the mailbox.*/
  test_set_step(1);
  {
    for (i = 0; i < MB_SIZE; i++) {
      msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
      test_assert(msg1 == MSG_OK, "wrong wake-up message");
    }
  }

  /* [5.3.2] Testing chMBPost(), chMBPostI(), chMBPostAhead() and
     chMBPostAheadI() timeout.*/
  test_set_step(2);
  {
    msg1 = chMBPost(&mb1, 'X', 1);
    test_assert(msg1 == MSG_TIMEOUT, "wrong wake-up message");
    chSysLock();
    msg1 = chMBPostI(&mb1, 'X');
    chSysUnlock();
    test_assert(msg1 == MSG_TIMEOUT, "wrong wake-up message");
    msg1 = chMBPostAhead(&mb1, 'X', 1);
    test_assert(msg1 == MSG_TIMEOUT, "wrong wake-up message");
    chSysLock();
    msg1 = chMBPostAheadI(&mb1, 'X');
    chSysUnlock();
    test_assert(msg1 == MSG_TIMEOUT, "wrong wake-up message");
  }

  /* [5.3.3] Resetting the mailbox.*/
  test_set_step(3);
  {
    chMBReset(&mb1);;
    chMBResumeX(&mb1);
  }

  /* [5.3.4] Testing chMBFetch() and chMBFetchI() timeout.*/
  test_set_step(4);
  {
    msg1 = chMBFetch(&mb1, &msg2, 1);
    test_assert(msg1 == MSG_TIMEOUT, "wrong wake-up message");
    chSysLock();
    msg1 = chMBFetchI(&mb1, &msg2);
    chSysUnlock();
    test_assert(msg1 == MSG_TIMEOUT, "wrong wake-up message");
  }
}

static const testcase_t test_005_003 = {
  "Mailbox timeouts",
  test_005_003_setup,
  test_005_003_teardown,
  test_005_003_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Mailboxes.
 */
const testcase_t * const test_sequence_005[] = {
  &test_005_001,
  &test_005_002,
  &test_005_003,
  NULL
};

#endif /* CH_CFG_USE_MAILBOXES */
