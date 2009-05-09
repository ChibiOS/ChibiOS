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

/**
 * @page test_queues I/O Queues test
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref IOQueues subsystem.
 * The tests are performed by inserting and removing data from queues and by
 * checking both the queues status and the correct sequence of the extracted
 * data.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref IOQueues code
 * as a necessary step in order to assess its maturity level.<br>
 * Note that the @ref IOQueues subsystem depends on the @ref Semaphores
 * subsystem that has to met its testing objectives as well.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_QUEUES (and dependent options)
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_queues_001
 * - @subpage test_queues_002
 * .
 * @file testqueues.c
 * @brief I/O Queues test source file
 * @file testqueues.h
 * @brief I/O Queues test header file
 */

#if CH_USE_QUEUES

#define TEST_QUEUES_SIZE 4

/**
 * @page test_queues_001 Input Queues functionality and APIs
 *
 * <h2>Description</h2>
 * This test case tests sysnchronos and asynchronous operations on an
 * @p InputQueue object including timeouts. The queue state must remain
 * consistent through the whole test.
 */
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

/**
 * @page test_queues_002 Output Queues functionality and APIs
 *
 * <h2>Description</h2>
 * This test case tests sysnchronos and asynchronous operations on an
 * @p OutputQueue object including timeouts. The queue state must remain
 * consistent through the whole test.
 */
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
