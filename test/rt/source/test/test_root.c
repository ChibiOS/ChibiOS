/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @mainpage Test Suite Specification
 * Test suite for ChibiOS/RT. The purpose of this suite is to perform
 * unit tests on the RT modules and to converge to 100% code coverage
 * through successive improvements.
 *
 * <h2>Test Sequences</h2>
 * - @subpage test_sequence_001
 * - @subpage test_sequence_002
 * - @subpage test_sequence_003
 * - @subpage test_sequence_004
 * - @subpage test_sequence_005
 * - @subpage test_sequence_006
 * - @subpage test_sequence_007
 * - @subpage test_sequence_008
 * .
 */

/**
 * @file    test_root.c
 * @brief   Test Suite root structures code.
 */

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of all the test sequences.
 */
const testcase_t * const *test_suite[] = {
  test_sequence_001,
  test_sequence_002,
  test_sequence_003,
  test_sequence_004,
  test_sequence_005,
  test_sequence_006,
  test_sequence_007,
  test_sequence_008,
  NULL
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

/*
 * Static working areas, the following areas can be used for threads or
 * used as temporary buffers.
 */
union test_buffers test;

/*
 * Pointers to the spawned threads.
 */
thread_t *threads[MAX_THREADS];

/*
 * Pointers to the working areas.
 */
void * ROMCONST wa[5] = {test.wa.T0, test.wa.T1, test.wa.T2,
                         test.wa.T3, test.wa.T4};

/*
 * Sets a termination request in all the test-spawned threads.
 */
void test_terminate_threads(void) {
  int i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i])
      chThdTerminate(threads[i]);
}

/*
 * Waits for the completion of all the test-spawned threads.
 */
void test_wait_threads(void) {
  int i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i] != NULL) {
      chThdWait(threads[i]);
      threads[i] = NULL;
    }
}

systime_t test_wait_tick(void) {

  chThdSleep(1);
  return chVTGetSystemTime();
}

#endif /* !defined(__DOXYGEN__) */
