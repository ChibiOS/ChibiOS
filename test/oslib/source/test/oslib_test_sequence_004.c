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
 * - @subpage oslib_test_004_001
 * .
 */

#if ((CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE)) || defined(__DOXYGEN__)

/****************************************************************************
 * Shared code.
 ****************************************************************************/


/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page oslib_test_004_001 [4.1] Objects Registry
 *
 * <h2>Description</h2>
 * This test case verifies the static objects registry.
 *
 * <h2>Test Steps</h2>
 * - [4.1.1] Retrieving a registered object by name, must not exist.
 * - [4.1.2] Registering an object, it must not exists, must succeed.
 * - [4.1.3] Registering an object with the same name, must fail.
 * - [4.1.4] Retrieving the registered object by name, must exist, then
 *   increasing the reference counter, finally releasing both
 *   references.
 * - [4.1.5] Releasing the first reference to the object, must not
 *   trigger an assertion.
 * - [4.1.6] Retrieving the registered object by name again, must not
 *   exist.
 * .
 */

static void oslib_test_004_001_execute(void) {
  registered_object_t *rop;

  /* [4.1.1] Retrieving a registered object by name, must not exist.*/
  test_set_step(1);
  {
    rop = chFactoryFindObject("myobj");
    test_assert(rop == NULL, "found");
  }

  /* [4.1.2] Registering an object, it must not exists, must succeed.*/
  test_set_step(2);
  {
    static uint32_t myobj = 0x55aa;

    rop = chFactoryRegisterObject("myobj", (void *)&myobj);
    test_assert(rop != NULL, "cannot register");
  }

  /* [4.1.3] Registering an object with the same name, must fail.*/
  test_set_step(3);
  {
    registered_object_t *rop1;
    static uint32_t myobj = 0x55aa;

    rop1 = chFactoryRegisterObject("myobj", (void *)&myobj);
    test_assert(rop1 == NULL, "can register");
  }

  /* [4.1.4] Retrieving the registered object by name, must exist, then
     increasing the reference counter, finally releasing both
     references.*/
  test_set_step(4);
  {
    registered_object_t *rop1, *rop2;

    rop1 = chFactoryFindObject("myobj");
    test_assert(rop1 != NULL, "not found");
    test_assert(*(uint32_t *)(rop1->objp) == 0x55aa, "object mismatch");
    test_assert(rop == rop1, "object reference mismatch");
    test_assert(rop1->element.refs == 2, "object reference mismatch");

    rop2 = (registered_object_t *)chFactoryDuplicateReference((dyn_element_t *)rop1);
    test_assert(rop1 == rop2, "object reference mismatch");
    test_assert(*(uint32_t *)(rop2->objp) == 0x55aa, "object mismatch");
    test_assert(rop2->element.refs == 3, "object reference mismatch");

    chFactoryReleaseObject(rop2);
    test_assert(rop1->element.refs == 2, "references mismatch");

    chFactoryReleaseObject(rop1);
    test_assert(rop->element.refs == 1, "references mismatch");
  }

  /* [4.1.5] Releasing the first reference to the object, must not
     trigger an assertion.*/
  test_set_step(5);
  {
    chFactoryReleaseObject(rop);
  }

  /* [4.1.6] Retrieving the registered object by name again, must not
     exist.*/
  test_set_step(6);
  {
    rop = chFactoryFindObject("myobj");
    test_assert(rop == NULL, "found");
  }
}

static const testcase_t oslib_test_004_001 = {
  "Objects Registry",
  NULL,
  NULL,
  oslib_test_004_001_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t * const oslib_test_sequence_004_array[] = {
  &oslib_test_004_001,
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
