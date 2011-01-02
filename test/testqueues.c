/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

#include "ch.h"
#include "test.h"

/**
 * @page test_queues I/O Queues test
 *
 * File: @ref testqueues.c
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref io_queues subsystem.
 * The tests are performed by inserting and removing data from queues and by
 * checking both the queues status and the correct sequence of the extracted
 * data.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref io_queues code.<br>
 * Note that the @ref io_queues subsystem depends on the @ref semaphores
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

static void notify(GenericQueue *qp) {}

/*
 * Note, the static initializers are not really required because the
 * variables are explicitly initialized in each test case. It is done in order
 * to test the macros.
 */
static INPUTQUEUE_DECL(iq, test.wa.T0, TEST_QUEUES_SIZE, notify);
static OUTPUTQUEUE_DECL(oq, test.wa.T1, TEST_QUEUES_SIZE, notify);

/**
 * @page test_queues_001 Input Queues functionality and APIs
 *
 * <h2>Description</h2>
 * This test case tests sysnchronos and asynchronous operations on an
 * @p InputQueue object including timeouts. The queue state must remain
 * consistent through the whole test.
 */

static void queues1_setup(void) {

  chIQInit(&iq, wa[0], TEST_QUEUES_SIZE, notify);
}

static void queues1_execute(void) {
  unsigned i;
  size_t n;

  /* Initial empty state */
  test_assert(1, chIQIsEmptyI(&iq), "not empty");

  /* Queue filling */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chIQPutI(&iq, 'A' + i);
  test_assert(2, chIQIsFullI(&iq), "still has space");
  test_assert(3, chIQPutI(&iq, 0) == Q_FULL, "failed to report Q_FULL");

  /* Queue emptying */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    test_emit_token(chIQGet(&iq));
  test_assert(4, chIQIsEmptyI(&iq), "still full");
  test_assert_sequence(5, "ABCD");

  /* Queue filling again */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chIQPutI(&iq, 'A' + i);

  /* Reading the whole thing */
  n = chIQReadTimeout(&iq, wa[1], TEST_QUEUES_SIZE * 2, TIME_IMMEDIATE);
  test_assert(6, n == TEST_QUEUES_SIZE, "wrong returned size");
  test_assert(7, chIQIsEmptyI(&iq), "still full");

  /* Queue filling again */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chIQPutI(&iq, 'A' + i);

  /* Partial reads */
  n = chIQReadTimeout(&iq, wa[1], TEST_QUEUES_SIZE / 2, TIME_IMMEDIATE);
  test_assert(8, n == TEST_QUEUES_SIZE / 2, "wrong returned size");
  n = chIQReadTimeout(&iq, wa[1], TEST_QUEUES_SIZE / 2, TIME_IMMEDIATE);
  test_assert(9, n == TEST_QUEUES_SIZE / 2, "wrong returned size");
  test_assert(10, chIQIsEmptyI(&iq), "still full");

  /* Testing reset */
  chIQPutI(&iq, 0);
  chIQResetI(&iq);
  test_assert(11, chIQIsEmptyI(&iq), "still full");

  /* Timeout */
  test_assert(12, chIQGetTimeout(&iq, 10) == Q_TIMEOUT, "wrong timeout return");
}

ROMCONST struct testcase testqueues1 = {
  "Queues, input queues",
  queues1_setup,
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

static void queues2_setup(void) {

  chOQInit(&oq, wa[0], TEST_QUEUES_SIZE, notify);
}

static void queues2_execute(void) {
  unsigned i;
  size_t n;

  /* Initial empty state */
  test_assert(1, chOQIsEmptyI(&oq), "not empty");

  /* Queue filling */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    chOQPut(&oq, 'A' + i);
  test_assert(2, chOQIsFullI(&oq), "still has space");

  /* Queue emptying */
  for (i = 0; i < TEST_QUEUES_SIZE; i++)
    test_emit_token(chOQGetI(&oq));
  test_assert(3, chOQIsEmptyI(&oq), "still full");
  test_assert_sequence(4, "ABCD");
  test_assert(5, chOQGetI(&oq) == Q_EMPTY, "failed to report Q_EMPTY");

  /* Writing the whole thing */
  n = chOQWriteTimeout(&oq, wa[1], TEST_QUEUES_SIZE * 2, TIME_IMMEDIATE);
  test_assert(6, n == TEST_QUEUES_SIZE, "wrong returned size");
  test_assert(7, chOQIsFullI(&oq), "not full");

  /* Testing reset */
  chOQResetI(&oq);
  test_assert(8, chOQIsEmptyI(&oq), "still full");

  /* Partial writes */
  n = chOQWriteTimeout(&oq, wa[1], TEST_QUEUES_SIZE / 2, TIME_IMMEDIATE);
  test_assert(9, n == TEST_QUEUES_SIZE / 2, "wrong returned size");
  n = chOQWriteTimeout(&oq, wa[1], TEST_QUEUES_SIZE / 2, TIME_IMMEDIATE);
  test_assert(10, n == TEST_QUEUES_SIZE / 2, "wrong returned size");
  test_assert(11, chOQIsFullI(&oq), "not full");

  /* Timeout */
  test_assert(12, chOQPutTimeout(&oq, 0, 10) == Q_TIMEOUT, "wrong timeout return");
}

ROMCONST struct testcase testqueues2 = {
  "Queues, output queues",
  queues2_setup,
  NULL,
  queues2_execute
};
#endif /* CH_USE_QUEUES */

/**
 * @brief   Test sequence for queues.
 */
ROMCONST struct testcase * ROMCONST patternqueues[] = {
#if CH_USE_QUEUES
  &testqueues1,
  &testqueues2,
#endif
  NULL
};
