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

static msg_t thread(void *p) {

  test_emit_token(*(char *)p);
  return 0;
}

static char *rdy1_gettest(void) {

  return "Ready List, priority enqueuing test #1";
}

static void rdy1_execute(void) {

  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-5, thread, "E");
  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-4, thread, "D");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread, "C");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-2, thread, "B");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread, "A");
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testrdy1 = {
  rdy1_gettest,
  NULL,
  NULL,
  rdy1_execute
};

static char *rdy2_gettest(void) {

  return "Ready List, priority enqueuing test #2";
}

static void rdy2_execute(void) {

  threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriority()-4, thread, "D");
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority()-5, thread, "E");
  threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriority()-1, thread, "A");
  threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriority()-2, thread, "B");
  threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriority()-3, thread, "C");
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testrdy2 = {
  rdy2_gettest,
  NULL,
  NULL,
  rdy2_execute
};

/*
 * Test sequence for ready list pattern.
 */
const struct testcase * const patternrdy[] = {
  &testrdy1,
  &testrdy2,
  NULL
};
