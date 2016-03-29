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
 * @page test_sequence_005 [5] Mutexes, Condition Variables and Priority Inheritance
 *
 * File: @ref test_sequence_005.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/RT functionalities related to
 * mutexes, condition variables and priority inheritance algorithm.
 *
 * <h2>Conditions</h2>
 * This sequence is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MUTEXES
 * .
 *
 * <h2>Test Cases</h2>
 * No test cases defined in the test sequence.
 */

#if (CH_CFG_USE_MUTEXES) || defined(__DOXYGEN__)

/****************************************************************************
 * Shared code.
 ****************************************************************************/

static MUTEX_DECL(m1);
static MUTEX_DECL(m2);
#if CH_CFG_USE_CONDVARS || defined(__DOXYGEN__)
static CONDVAR_DECL(c1);
#endif

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Mutexes, Condition Variables and Priority Inheritance.
 */
const testcase_t * const test_sequence_005[] = {
  NULL
};

#endif /* CH_CFG_USE_MUTEXES */
