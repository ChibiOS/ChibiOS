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

static msg_t thread(void *p) {

  test_emit_token(*(char *)p);
  return 0;
}

static char *rdy1_gettest(void) {

  return "Ready List, priority enqueuing test #1";
}

static void rdy1_setup(void) {
}

static void rdy1_teardown(void) {
}

static void rdy1_execute(void) {

  threads[0] = chThdCreate(chThdGetPriority()-5, wa[0], STKSIZE, thread, "E");
  threads[1] = chThdCreate(chThdGetPriority()-4, wa[1], STKSIZE, thread, "D");
  threads[2] = chThdCreate(chThdGetPriority()-3, wa[2], STKSIZE, thread, "C");
  threads[3] = chThdCreate(chThdGetPriority()-2, wa[3], STKSIZE, thread, "B");
  threads[4] = chThdCreate(chThdGetPriority()-1, wa[4], STKSIZE, thread, "A");
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testrdy1 = {
  rdy1_gettest,
  rdy1_setup,
  rdy1_teardown,
  rdy1_execute
};

static char *rdy2_gettest(void) {

  return "Ready List, priority enqueuing test #2";
}

static void rdy2_setup(void) {
}

static void rdy2_teardown(void) {
}

static void rdy2_execute(void) {

  threads[1] = chThdCreate(chThdGetPriority()-4, wa[1], STKSIZE, thread, "D");
  threads[0] = chThdCreate(chThdGetPriority()-5, wa[0], STKSIZE, thread, "E");
  threads[4] = chThdCreate(chThdGetPriority()-1, wa[4], STKSIZE, thread, "A");
  threads[3] = chThdCreate(chThdGetPriority()-2, wa[3], STKSIZE, thread, "B");
  threads[2] = chThdCreate(chThdGetPriority()-3, wa[2], STKSIZE, thread, "C");
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testrdy2 = {
  rdy2_gettest,
  rdy2_setup,
  rdy2_teardown,
  rdy2_execute
};
