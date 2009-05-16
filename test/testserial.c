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
 * @page test_serial Serial Drivers test
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref Serial subsystem.
 * The tests are performed on a loopback software serial driver where a
 * dedicated thread echoes back in the input queue the data read from the
 * output queue at a fixed rate. The test module also tests implicitly the
 * channels code.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref Serial code
 * as a necessary step in order to assess its maturity level.<br>
 * Note that the @ref Serial subsystem depends on the @ref Semaphores and
 * @ref Events subsystems that have to met their testing objectives as well.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_SERIAL_FULLDUPLEX
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 *
 * <h2>Test Cases</h2>
 * - @subpage test_serial_001
 * - @subpage test_serial_002
 * .
 * @file testserial.c
 * @brief Serial Driver test source file
 * @file testserial.h
 * @brief Serial Driver test header file
 */

#if CH_USE_SERIAL_FULLDUPLEX

#define TEST_QUEUES_SIZE 8

static FullDuplexDriver fdd;

/* Loopback thread, it simulates a low level driver. The thread terminates by
   sending a zero through the loopback driver.*/
static msg_t thread1(void *p) {

  while (TRUE) {
    chEvtWaitAny(1);
    chSysLock();
    while (TRUE) {
      msg_t b = chFDDRequestDataI(&fdd);
      if (b < Q_OK)
        break;
      if (b == 0) {
        chSchRescheduleS();
        chSysUnlock();
        return 0;
      }
      chFDDIncomingDataI(&fdd, (uint8_t)b);
      chSchRescheduleS();
    }
    chSysUnlock();
  }
}

static void infy(void) {}

static void onfy(void) {

  chEvtSignalI(threads[0], 1);
  chSchRescheduleS();
}

/**
 * @page test_serial_001 Synchronous loopback test
 *
 * <h2>Description</h2>
 * A sequence of characters are sent to the loopback driver and read back. The
 * test is performed twice using both the direct APIs and the channels API
 * implementations.<br>
 * The test expects to read all the characters back and in the correct
 * sequence.
 */

static char *serial1_gettest(void) {

  return "Serial driver, synchronous";
}

static void serial1_setup(void) {

  /* Initializes the loopback driver.*/
  chFDDInit(&fdd, wa[3], 8, infy, wa[4], 8, onfy);
  /* Starts the loopback thread.*/
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() + 1,
                                 thread1, 0);
}

static void serial1_teardown(void) {

  /* Terminates the loopback thread.*/
  chFDDPut(&fdd, 0);
}

static void serial1_execute(void) {
  unsigned i;
  msg_t b;

  /* Loopback test using the direct APIs.*/
  for (i = 0; i < 4; i++) {
    chFDDPut(&fdd, 'A' + i);
    b = chFDDGetTimeout(&fdd, S2ST(1));
    if (b < Q_OK)
      break;
    test_emit_token(b);
  }
  test_assert_sequence(1, "ABCD");
  test_assert(2, chFDDPutWouldBlock(&fdd) == FALSE, "output would block");
  test_assert(3, chFDDGetWouldBlock(&fdd) == TRUE, "input would not block");

  /* Loopback test using the channel APIs.*/
  for (i = 0; i < 4; i++) {
    chIOPut(&fdd, 'A' + i);
    b = chIOGetTimeout(&fdd, S2ST(1));
    if (b < Q_OK)
      break;
    test_emit_token(b);
  }
  test_assert_sequence(4, "ABCD");
  test_assert(5, chIOPutWouldBlock(&fdd) == FALSE, "output would block");
  test_assert(6, chIOGetWouldBlock(&fdd) == TRUE, "input would not block");
}

const struct testcase testserial1 = {
  serial1_gettest,
  serial1_setup,
  serial1_teardown,
  serial1_execute
};

/**
 * @page test_serial_002 Asynchronous loopback test
 *
 * <h2>Description</h2>
 * A sequence of characters are sent to the loopback driver using the
 * asynchronous APIs and then read back. The test is performed twice using
 * both the direct APIs and the channels API. An input queue overflow test
 * is performed too.<br>
 * The test expects that the queues are filled and emptied as expected and that
 * the overflow error condition is reported when expected.
 */

static char *serial2_gettest(void) {

  return "Serial driver, asynchronous";
}

static void serial2_setup(void) {

  /* Initializes the loopback driver.*/
  chFDDInit(&fdd, wa[3], 8, infy, wa[4], 8, onfy);
  /* Starts the loopback thread.*/
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() + 1,
                                 thread1, 0);
}

static void serial2_teardown(void) {

  /* Terminates the loopback thread.*/
  chFDDPut(&fdd, 0);
}

static void serial2_execute(void) {
  size_t n;
  dflags_t flags;

  /* Asynchronous test using the direct APIs.*/
  n = chFDDWrite(&fdd, "ABCDEFGH", TEST_QUEUES_SIZE);
  test_assert(1, n == TEST_QUEUES_SIZE, "unexpected write condition");
  n = chFDDRead(&fdd, wa[1], TEST_QUEUES_SIZE);
  test_assert(2, n == TEST_QUEUES_SIZE, "unexpected read condition");
  test_assert(2, chFDDPutWouldBlock(&fdd) == FALSE, "output would block");
  test_assert(3, chFDDGetWouldBlock(&fdd) == TRUE, "input would not block");
  flags = chFDDGetAndClearFlags(&fdd);
  test_assert(4, flags == 0, "unexpected error condition");

  /* Input overflow testing.*/
  n = chFDDWrite(&fdd, "ABCDEFGH", TEST_QUEUES_SIZE);
  test_assert(5, n == TEST_QUEUES_SIZE, "unexpected write condition");
  /* The following operation will fail to loopback because the input queue
   * is full.*/
  chFDDPut(&fdd, 'Z');
  flags = chFDDGetAndClearFlags(&fdd);
  test_assert(6, flags == SD_OVERRUN_ERROR, "unexpected error condition");
  n = chFDDRead(&fdd, wa[1], TEST_QUEUES_SIZE);
  test_assert(7, n == TEST_QUEUES_SIZE, "unexpected read condition");

  /* Asynchronous test using the channel APIs.*/
  n = chIOWrite(&fdd, "ABCDEFGH", TEST_QUEUES_SIZE);
  test_assert(8, n == TEST_QUEUES_SIZE, "unexpected write condition");
  n = chIORead(&fdd, wa[1], TEST_QUEUES_SIZE);
  test_assert(9, n == TEST_QUEUES_SIZE, "unexpected read condition");
  test_assert(10, chIOPutWouldBlock(&fdd) == FALSE, "output would block");
  test_assert(11, chIOGetWouldBlock(&fdd) == TRUE, "input would not block");
}

const struct testcase testserial2 = {
  serial2_gettest,
  serial2_setup,
  serial2_teardown,
  serial2_execute
};
#endif /* CH_USE_SERIAL_FULLDUPLEX */

/*
 * Test sequence for queues pattern.
 */
const struct testcase * const patternserial[] = {
#if CH_USE_SERIAL_FULLDUPLEX
  &testserial1,
  &testserial2,
#endif
  NULL
};
