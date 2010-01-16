/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @page test_mbox Mailboxes test
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref mailboxes subsystem.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref mailboxes
 * subsystem code.<br>
 * Note that the @ref mailboxes subsystem depends on the @ref semaphores
 * subsystem that has to met its testing objectives as well.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_MAILBOXES
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_mbox_001
 * .
 * @file testmbox.c
 * @brief Mailboxes test source file
 * @file testmbox.h
 * @brief Mailboxes header file
 */

#if CH_USE_MAILBOXES

#define ALLOWED_DELAY MS2ST(5)
#define MB_SIZE 5

/*
 * Note, the static initializers are not really required because the
 * variables are explicitly initialized in each test case. It is done in order
 * to test the macros.
 */
static MAILBOX_DECL(mb1, test.waT0, MB_SIZE);

/**
 * @page test_mbox_001 Queuing and timeouts
 *
 * <h2>Description</h2>
 * Messages are posted/fetched from a mailbox in carefully designed sequences
 * in order to stimulate all the possible code paths inside the mailbox.<br>
 * The test expects to find a consistent mailbox status after each operation.
 */

static char *mbox1_gettest(void) {

  return "Mailboxes, queuing and timeouts";
}

static void mbox1_setup(void) {

  chMBInit(&mb1, (msg_t *)test.waT0, MB_SIZE);
}

static void mbox1_execute(void) {
  msg_t msg1, msg2;
  unsigned i;

  /*
   * Testing initial space.
   */
  test_assert(1, chMBGetEmpty(&mb1) == MB_SIZE, "wrong size");

  /*
   * Testing enqueuing and backward circularity.
   */
  for (i = 0; i < MB_SIZE - 1; i++) {
    msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
    test_assert(2, msg1 == RDY_OK, "wrong wake-up message");
  }
  msg1 = chMBPostAhead(&mb1, 'A', TIME_INFINITE);
  test_assert(3, msg1 == RDY_OK, "wrong wake-up message");

  /*
   * Testing post timeout.
   */
  msg1 = chMBPost(&mb1, 'X', 1);
  test_assert(4, msg1 == RDY_TIMEOUT, "wrong wake-up message");

  /*
   * Testing final conditions.
   */
  test_assert(5, chMBGetEmpty(&mb1) == 0, "still empty");
  test_assert(6, chMBGetFull(&mb1) == MB_SIZE, "not full");
  test_assert(7, mb1.mb_rdptr == mb1.mb_wrptr, "pointers not aligned");

  /*
   * Testing dequeuing.
   */
  for (i = 0; i < MB_SIZE; i++) {
  msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
    test_assert(8, msg1 == RDY_OK, "wrong wake-up message");
    test_emit_token(msg2);
  }
  test_assert_sequence(9, "ABCDE");

  /*
   * Testing buffer circularity.
   */
  msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
  test_assert(10, msg1 == RDY_OK, "wrong wake-up message");
  msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
  test_assert(11, msg1 == RDY_OK, "wrong wake-up message");
  test_assert(12, mb1.mb_buffer == mb1.mb_wrptr, "write pointer not aligned to base");
  test_assert(13, mb1.mb_buffer == mb1.mb_rdptr, "read pointer not aligned to base");

  /*
   * Testing fetch timeout.
   */
  msg1 = chMBFetch(&mb1, &msg2, 1);
  test_assert(14, msg1 == RDY_TIMEOUT, "wrong wake-up message");

  /*
   * Testing final conditions.
   */
  test_assert(15, chMBGetEmpty(&mb1) == MB_SIZE, "not empty");
  test_assert(16, chMBGetFull(&mb1) == 0, "still full");
  test_assert(17, mb1.mb_rdptr == mb1.mb_wrptr, "pointers not aligned");

  /*
   * Testing reset.
   */
  chMBReset(&mb1);

  /*
   * Re-testing final conditions.
   */
  test_assert(18, chMBGetEmpty(&mb1) == MB_SIZE, "not empty");
  test_assert(19, chMBGetFull(&mb1) == 0, "still full");
  test_assert(20, mb1.mb_buffer == mb1.mb_wrptr, "write pointer not aligned to base");
  test_assert(21, mb1.mb_buffer == mb1.mb_rdptr, "read pointer not aligned to base");
}

const struct testcase testmbox1 = {
  mbox1_gettest,
  mbox1_setup,
  NULL,
  mbox1_execute
};

#endif /* CH_USE_MAILBOXES */

/*
 * Test sequence for mailboxes pattern.
 */
const struct testcase * const patternmbox[] = {
#if CH_USE_MAILBOXES
  &testmbox1,
#endif
  NULL
};
