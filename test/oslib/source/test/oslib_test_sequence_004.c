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

#include "hal.h"
#include "oslib_test_root.h"

/**
 * @file    oslib_test_sequence_004.c
 * @brief   Test Sequence 004 code.
 *
 * @page oslib_test_sequence_004 [4] Objects Factory
 *
 * File: @ref oslib_test_sequence_004.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS library functionalities related to
 * the object factory.
 *
 * <h2>Conditions</h2>
 * This sequence is only executed if the following preprocessor condition
 * evaluates to true:
 * - (CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE)
 * .
 *
 * <h2>Test Cases</h2>
 * No test cases defined in the test sequence.
 */

#if ((CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE)) || defined(__DOXYGEN__)

/****************************************************************************
 * Shared code.
 ****************************************************************************/


/****************************************************************************
 * Test cases.
 ****************************************************************************/

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t * const oslib_test_sequence_004_array[] = {
  NULL
};

/**
 * @brief   Objects Factory.
 */
const testsequence_t oslib_test_sequence_004 = {
  "Objects Factory",
  oslib_test_sequence_004_array
};

#endif /* (CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE) */
