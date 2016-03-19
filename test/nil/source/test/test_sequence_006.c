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

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/**
 * @page test_sequence_006 Memory Heaps
 *
 * File: @ref test_sequence_006.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/NIL functionalities related to
 * memory heaps.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_006_001
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/


/****************************************************************************
 * Test cases.
 ****************************************************************************/

#if CH_CFG_USE_HEAP || defined(__DOXYGEN__)
/**
 * @page test_006_001 Allocation and fragmentation
 *
 * <h2>Description</h2>
 * Series of allocations/deallocations are performed in carefully
 * designed sequences in order to stimulate all the possible code paths
 * inside the allocator. The test expects to find the heap back to the
 * initial status after each sequence.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_HEAP
 * .
 *
 * <h2>Test Steps</h2>
 */

static void test_006_001_setup(void) {
  chHeapObjectInit(&test_heap, test.buffer, sizeof(union test_buffers));
}

static void test_006_001_execute(void) {
}

static const testcase_t test_006_001 = {
  "Allocation and fragmentation",
  test_006_001_setup,
  NULL,
  test_006_001_execute
};
#endif /* CH_CFG_USE_HEAP */

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Memory Heaps.
 */
const testcase_t * const test_sequence_006[] = {
#if CH_CFG_USE_HEAP || defined(__DOXYGEN__)
  &test_006_001,
#endif
  NULL
};
