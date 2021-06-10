/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * Test suite for core benchmarks. The purpose of this suite is to
 * perform general benchmarks in order to assess performance of cores
 * and/or compilers.
 *
 * <h2>Test Sequences</h2>
 * - @subpage corebmk_test_sequence_001
 * .
 */

/**
 * @file    corebmk_test_root.c
 * @brief   Test Suite root structures code.
 */

#include "hal.h"
#include "corebmk_test_root.h"

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Array of test sequences.
 */
const testsequence_t * const corebmk_test_suite_array[] = {
  &corebmk_test_sequence_001,
  NULL
};

/**
 * @brief   Test suite root structure.
 */
const testsuite_t corebmk_test_suite = {
  "ChibiOS/NIL Test Suite",
  corebmk_test_suite_array
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

/**/

#endif /* !defined(__DOXYGEN__) */
