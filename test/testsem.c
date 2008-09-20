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

#define ALLOWED_DELAY 5

static Semaphore sem1;

static char *sem1_gettest(void) {

  return "Semaphores, FIFO enqueuing test";
}

static void sem1_setup(void) {

  chSemInit(&sem1, 0);
}

static void sem1_teardown(void) {
}

static msg_t thread(void *p) {

  chSemWait(&sem1);
  test_emit_token(*(char *)p);
  return 0;
}

static void sem1_execute(void) {

  threads[0] = chThdCreate(chThdGetPriority()+5, wa[0], STKSIZE, thread, "A");
  threads[1] = chThdCreate(chThdGetPriority()+1, wa[1], STKSIZE, thread, "B");
  threads[2] = chThdCreate(chThdGetPriority()+3, wa[2], STKSIZE, thread, "C");
  threads[3] = chThdCreate(chThdGetPriority()+4, wa[3], STKSIZE, thread, "D");
  threads[4] = chThdCreate(chThdGetPriority()+2, wa[4], STKSIZE, thread, "E");
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  chSemSignal(&sem1);
  test_wait_threads();
  test_assert_sequence("ABCDE");
}

const struct testcase testsem1 = {
  sem1_gettest,
  sem1_setup,
  sem1_teardown,
  sem1_execute
};

static char *sem2_gettest(void) {

  return "Semaphores, timeout test";
}

static void sem2_setup(void) {

  chSemInit(&sem1, 0);
}

static void sem2_teardown(void) {
}

static void sem2_execute(void) {
  int i;
  systime_t target_time;

  target_time = chSysGetTime() + 5 * 500;
  for (i = 0; i < 5; i++) {
    test_emit_token('A' + i);
    chSemWaitTimeout(&sem1, 500);
  }
  test_assert_sequence("ABCDE");
  test_assert_time_window(target_time, target_time + ALLOWED_DELAY);
}

const struct testcase testsem2 = {
  sem2_gettest,
  sem2_setup,
  sem2_teardown,
  sem2_execute
};
