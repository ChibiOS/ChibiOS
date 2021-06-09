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

#include "hal.h"
#include "corebmk_test_root.h"

/**
 * @file    corebmk_test_sequence_001.c
 * @brief   Test Sequence 001 code.
 *
 * @page corebmk_test_sequence_001 [1] Information
 *
 * File: @ref corebmk_test_sequence_001.c
 *
 * <h2>Description</h2>
 * This sequence reports configuration and version information about
 * execution environment.
 *
 * <h2>Test Cases</h2>
 * - @subpage corebmk_test_001_001
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include "ch.h"

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page corebmk_test_001_001 [1.1] Environment Info
 *
 * <h2>Description</h2>
 * Environment-related info are reported.
 *
 * <h2>Test Steps</h2>
 * - [1.1.1] Architecture and Compiler information.
 * .
 */

static void corebmk_test_001_001_execute(void) {

  /* [1.1.1] Architecture and Compiler information.*/
  test_set_step(1);
  {
#if defined(PORT_ARCHITECTURE_NAME)
    test_print("--- Architecture:                       ");
    test_println(PORT_ARCHITECTURE_NAME);
#endif
#if defined(PORT_CORE_VARIANT_NAME)
    test_print("--- Core Variant:                       ");
    test_println(PORT_CORE_VARIANT_NAME);
#endif
#if defined(PORT_COMPILER_NAME)
    test_print("--- Compiler:                           ");
    test_println(PORT_COMPILER_NAME);
#endif
#if defined(PORT_INFO)
    test_print("--- Port Info:                          ");
    test_println(PORT_INFO);
#endif
#if defined(PORT_NATURAL_ALIGN)
    test_print("--- Natural alignment:                  ");
    test_printn(PORT_NATURAL_ALIGN);
    test_println("");
#endif
#if defined(PORT_STACK_ALIGN)
    test_print("--- Stack alignment:                    ");
    test_printn(PORT_STACK_ALIGN);
    test_println("");
#endif
#if defined(PORT_WORKING_AREA_ALIGN)
    test_print("--- Working area alignment:             ");
    test_printn(PORT_WORKING_AREA_ALIGN);
    test_println("");
#endif
  }
  test_end_step(1);
}

static const testcase_t corebmk_test_001_001 = {
  "Environment Info",
  NULL,
  NULL,
  corebmk_test_001_001_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t * const corebmk_test_sequence_001_array[] = {
  &corebmk_test_001_001,
  NULL
};

/**
 * @brief   Information.
 */
const testsequence_t corebmk_test_sequence_001 = {
  "Information",
  corebmk_test_sequence_001_array
};
