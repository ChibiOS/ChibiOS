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
 * Test suite for NASA OSAL implementation over ChibiOS/RT. The purpose
 * of this suite is to perform unit tests on the OSAL module and to
 * converge to 100% code coverage through successive improvements.
 *
 * <h2>Test Sequences</h2>
 * - @subpage test_sequence_001
 * .
 */

/**
 * @file    test_root.c
 * @brief   Test Suite root structures code.
 *
 * @addtogroup SPC5_TEST_ROOT
 * @{
 */

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of all the test sequences.
 */
const testcase_t * const *test_suite[] = {
  test_sequence_001,
  NULL
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

THD_WORKING_AREA(wa_test1, TASKS_STACK_SIZE);
THD_WORKING_AREA(wa_test2, TASKS_STACK_SIZE);
THD_WORKING_AREA(wa_test3, TASKS_STACK_SIZE);
THD_WORKING_AREA(wa_test4, TASKS_STACK_SIZE);

/** @} */
