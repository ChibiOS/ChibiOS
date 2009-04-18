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

#if CH_USE_MESSAGES

static char *msg1_gettest(void) {

  return "Messages, dispatch test";
}

static msg_t thread(void *p) {
  msg_t msg;
  int i;

  for (i = 0; i < 5; i++) {
    msg = chMsgSend(p, 'A' + i);
    test_emit_token(msg);
  }
  chMsgSend(p, 0);
  return 0;
}

static void msg1_execute(void) {
  msg_t msg;

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-1, thread, chThdSelf());
  do {
    chMsgRelease(msg = chMsgWait());
    if (msg)
      test_emit_token(msg);
  } while (msg);
  test_wait_threads();
  test_assert_sequence("AABBCCDDEE");
}

const struct testcase testmsg1 = {
  msg1_gettest,
  NULL,
  NULL,
  msg1_execute
};

#endif /* CH_USE_MESSAGES */

/*
 * Test sequence for messages pattern.
 */
const struct testcase * const patternmsg[] = {
#if CH_USE_MESSAGES
  &testmsg1,
#endif
  NULL
};
