/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio

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
 * - @subpage test_sequence_009
 * - @subpage test_sequence_010
 * - @subpage test_sequence_011
 * - @subpage test_sequence_012
 * - @subpage test_sequence_013
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
#if (CH_CFG_USE_SEMAPHORES) || defined(__DOXYGEN__)
  test_sequence_005,
#endif
#if (CH_CFG_USE_MUTEXES) || defined(__DOXYGEN__)
  test_sequence_006,
#endif
#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
  test_sequence_007,
#endif
#if (CH_CFG_USE_EVENTS) || defined(__DOXYGEN__)
  test_sequence_008,
#endif
#if (CH_CFG_USE_MAILBOXES) || defined(__DOXYGEN__)
  test_sequence_009,
#endif
#if (CH_CFG_USE_MEMPOOLS) || defined(__DOXYGEN__)
  test_sequence_010,
#endif
#if (CH_CFG_USE_HEAP) || defined(__DOXYGEN__)
  test_sequence_011,
#endif
#if (CH_CFG_USE_DYNAMIC) || defined(__DOXYGEN__)
  test_sequence_012,
#endif
  test_sequence_013,
  NULL
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

void test_print_port_info(void) {

#ifdef PORT_COMPILER_NAME
  test_print("*** Compiler:     ");
  test_println(PORT_COMPILER_NAME);
#endif
  test_print("*** Architecture: ");
  test_println(PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  test_print("*** Core Variant: ");
  test_println(PORT_CORE_VARIANT_NAME);
#endif
#ifdef PORT_INFO
  test_print("*** Port Info:    ");
  test_println(PORT_INFO);
#endif
}

/*
 * Global test buffer holding 5 working areas.
 */
ALIGNED_VAR(PORT_WORKING_AREA_ALIGN) uint8_t test_buffer[WA_SIZE * 5];

/*
 * Pointers to the spawned threads.
 */
thread_t *threads[MAX_THREADS];

/*
 * Pointers to the working areas.
 */
void * ROMCONST wa[5] = {test_buffer + (WA_SIZE * 0),
                         test_buffer + (WA_SIZE * 1),
                         test_buffer + (WA_SIZE * 2),
                         test_buffer + (WA_SIZE * 3),
                         test_buffer + (WA_SIZE * 4)};

/*
 * Sets a termination request in all the test-spawned threads.
 */
void test_terminate_threads(void) {
  unsigned i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i])
      chThdTerminate(threads[i]);
}

/*
 * Waits for the completion of all the test-spawned threads.
 */
void test_wait_threads(void) {
  unsigned i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i] != NULL) {
      chThdWait(threads[i]);
      threads[i] = NULL;
    }
}

/*
 * Delays execution until next system time tick.
 */
systime_t test_wait_tick(void) {

  chThdSleep(1);
  return chVTGetSystemTime();
}

#endif /* !defined(__DOXYGEN__) */
