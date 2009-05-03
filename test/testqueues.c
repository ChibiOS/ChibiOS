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

#if CH_USE_QUEUES

#define TEST_QUEUES_SIZE 4

static void notify(void) {}

static char *queues1_gettest(void) {

  return "Queues, input queues";
}

static void queues1_execute(void) {
  InputQueue iq;
  unsigned i;

  chIQInit(&iq, wa[0], TEST_QUEUES_SIZE, notify);

  /* Initial empty state */
  test_assert(1, chIQIsEmpty(&iq), "not empty");

  /* Queue filling */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chIQPutI(&iq, 'A' + i);
  test_assert(2, chIQIsFull(&iq), "still has space");
  test_assert(3, chIQPutI(&iq, 0) == Q_FULL, "failed to report Q_FULL");

  /* Queue emptying */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    test_emit_token(chIQGet(&iq));
  test_assert(4, chIQIsEmpty(&iq), "still full");
  test_assert_sequence(5, "ABCD");

  /* Queue filling again */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chIQPutI(&iq, 'A' + i);

  /* Reading the whole thing */
  test_assert(6,
              chIQRead(&iq, wa[1], TEST_QUEUES_SIZE * 2) == TEST_QUEUES_SIZE,
              "wrong returned size");
  test_assert(7, chIQIsEmpty(&iq), "still full");

  /* Testing reset */
  chIQPutI(&iq, 0);
  chIQResetI(&iq);
  test_assert(8, chIQIsEmpty(&iq), "still full");

#if CH_USE_SEMAPHORES_TIMEOUT
  /* Timeout */
  test_assert(9, chIQGetTimeout(&iq, 10) == Q_TIMEOUT, "wrong timeout return");
#endif
}

const struct testcase testqueues1 = {
  queues1_gettest,
  NULL,
  NULL,
  queues1_execute
};

static char *queues2_gettest(void) {

  return "Queues, output queues";
}

static void queues2_execute(void) {
  OutputQueue oq;
  unsigned i;

  chOQInit(&oq, wa[0], TEST_QUEUES_SIZE, notify);

  /* Initial empty state */
  test_assert(1, chOQIsEmpty(&oq), "not empty");

  /* Queue filling */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chOQPut(&oq, 'A' + i);
  test_assert(2, chOQIsFull(&oq), "still has space");

  /* Queue emptying */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    test_emit_token(chOQGetI(&oq));
  test_assert(3, chOQIsEmpty(&oq), "still full");
  test_assert_sequence(4, "ABCD");
  test_assert(5, chOQGetI(&oq) == Q_EMPTY, "failed to report Q_EMPTY");

  /* Writing the whole thing */
  test_assert(6,
              chOQWrite(&oq, wa[1], TEST_QUEUES_SIZE * 2) == TEST_QUEUES_SIZE,
              "wrong returned size");
  test_assert(7, chOQIsFull(&oq), "not full");

  /* Testing reset */
  chOQResetI(&oq);
  test_assert(8, chOQIsEmpty(&oq), "still full");

#if CH_USE_SEMAPHORES_TIMEOUT
  /* Timeout */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chOQPut(&oq, 'A' + i);
  test_assert(9, chOQIsFull(&oq), "still has space");
  test_assert(10, chOQPutTimeout(&oq, 0, 10) == Q_TIMEOUT, "wrong timeout return");
#endif
}

const struct testcase testqueues2 = {
  queues2_gettest,
  NULL,
  NULL,
  queues2_execute
};
#endif /* CH_USE_QUEUES */

/*
 * Test sequence for queues pattern.
 */
const struct testcase * const patternqueues[] = {
#if CH_USE_QUEUES
  &testqueues1,
  &testqueues2,
#endif
  NULL
};
