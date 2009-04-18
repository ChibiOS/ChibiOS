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

  /* Testing initial space.*/
  test_assert(chMBGetEmpty(&mb1) == MB_SIZE, "#1");

  /* Testing enqueuing.*/
  for (i = 0; i < MB_SIZE - 1; i++) {
    msg1 = chMBPost(&mb1, 'B' + i, TIME_INFINITE);
    test_assert(msg1 == RDY_OK, "#2");
  }
  msg1 = chMBPostAhead(&mb1, 'A', TIME_INFINITE);
  test_assert(msg1 == RDY_OK, "#3");

  /* Testing post timeout.*/
  msg1 = chMBPost(&mb1, 'X', 1);
  test_assert(msg1 == RDY_TIMEOUT, "#4");

  /* Testing final conditions.*/
  test_assert(chMBGetEmpty(&mb1) == 0, "#5");
  test_assert(chMBGetFull(&mb1) == MB_SIZE, "#6");
  test_assert(mb1.mb_rdptr == mb1.mb_wrptr, "#7");

  /* Testing dequeuing.*/
  for (i = 0; i < MB_SIZE; i++) {
  msg1 = chMBFetch(&mb1, &msg2, TIME_INFINITE);
    test_assert(msg1 == RDY_OK, "#8");
    test_emit_token(msg2);
  }
  test_assert_sequence("ABCDE");

  /* Testing fetch timeout.*/
  msg1 = chMBFetch(&mb1, &msg2, 1);
  test_assert(msg1 == RDY_TIMEOUT, "#9");

  /* Testing final conditions.*/
  test_assert(chMBGetEmpty(&mb1) == MB_SIZE, "#10");
  test_assert(chMBGetFull(&mb1) == 0, "#11");
  test_assert(mb1.mb_rdptr == mb1.mb_wrptr, "#12");
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
