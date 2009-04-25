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

#if CH_USE_MAILBOXES && CH_USE_SEMAPHORES_TIMEOUT

#define ALLOWED_DELAY MS2ST(5)
#define MB_SIZE 5

static msg_t mb1_buf[MB_SIZE];
static Mailbox mb1;

static char *mbox1_gettest(void) {

  return "Mailboxes, queuing and timeouts";
}

static void mbox1_setup(void) {

  chMBInit(&mb1, mb1_buf, MB_SIZE);
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

#endif /* CH_USE_MAILBOXES && CH_USE_SEMAPHORES_TIMEOUT */

/*
 * Test sequence for mailboxes pattern.
 */
const struct testcase * const patternmbox[] = {
#if CH_USE_MAILBOXES && CH_USE_SEMAPHORES_TIMEOUT
  &testmbox1,
#endif
  NULL
};
