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
#include "ch_test.h"
#include "test_root.h"

/**
 * @file    test_sequence_001.c
 * @brief   Test Sequence 001 code.
 *
 * @page test_sequence_001 [1] Information
 *
 * File: @ref test_sequence_001.c
 *
 * <h2>Description</h2>
 * This sequence reports configuration and version information about
 * the NIL kernel.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_001_001
 * - @subpage test_001_002
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
 * @page test_001_001 [1.1] Kernel Info
 *
 * <h2>Description</h2>
 * The version numbers are reported.
 *
 * <h2>Test Steps</h2>
 * - [1.1.1] Prints the version string.
 * .
 */

static void test_001_001_execute(void) {

  /* [1.1.1] Prints the version string.*/
  test_set_step(1);
  {
    test_println("--- Product:                   ChibiOS/NIL");
    test_print("--- Stable Flag:               ");
    test_printn(CH_KERNEL_STABLE);
    test_println("");
    test_print("--- Version String:            ");
    test_println(CH_KERNEL_VERSION);
    test_print("--- Major Number:              ");
    test_printn(CH_KERNEL_MAJOR);
    test_println("");
    test_print("--- Minor Number:              ");
    test_printn(CH_KERNEL_MINOR);
    test_println("");
    test_print("--- Patch Number:              ");
    test_printn(CH_KERNEL_PATCH);
    test_println("");
  }
}

static const testcase_t test_001_001 = {
  "Kernel Info",
  NULL,
  NULL,
  test_001_001_execute
};

/**
 * @page test_001_002 [1.2] Kernel Settings
 *
 * <h2>Description</h2>
 * The static kernel settings are reported.
 *
 * <h2>Test Steps</h2>
 * - [1.2.1] Prints the configuration options settings.
 * .
 */

static void test_001_002_execute(void) {

  /* [1.2.1] Prints the configuration options settings.*/
  test_set_step(1);
  {
    test_print("--- CH_CFG_NUM_THREADS:        ");
    test_printn(CH_CFG_NUM_THREADS);
    test_println("");
    test_print("--- CH_CFG_ST_RESOLUTION:      ");
    test_printn(CH_CFG_ST_RESOLUTION);
    test_println("");
    test_print("--- CH_CFG_ST_FREQUENCY:       ");
    test_printn(CH_CFG_ST_FREQUENCY);
    test_println("");
    test_print("--- CH_CFG_ST_TIMEDELTA:       ");
    test_printn(CH_CFG_ST_TIMEDELTA);
    test_println("");
    test_print("--- CH_CFG_USE_SEMAPHORES:     ");
    test_printn(CH_CFG_USE_SEMAPHORES);
    test_println("");
    test_print("--- CH_CFG_USE_MUTEXES:        ");
    test_printn(CH_CFG_USE_MUTEXES);
    test_println("");
    test_print("--- CH_CFG_USE_EVENTS:         ");
    test_printn(CH_CFG_USE_EVENTS);
    test_println("");
    test_print("--- CH_CFG_USE_MAILBOXES:      ");
    test_printn(CH_CFG_USE_MAILBOXES);
    test_println("");
    test_print("--- CH_CFG_USE_MEMCORE:        ");
    test_printn(CH_CFG_USE_MEMCORE);
    test_println("");
    test_print("--- CH_CFG_USE_HEAP:           ");
    test_printn(CH_CFG_USE_HEAP);
    test_println("");
    test_print("--- CH_CFG_USE_MEMPOOLS:       ");
    test_printn(CH_CFG_USE_MEMPOOLS);
    test_println("");
    test_print("--- CH_DBG_STATISTICS:         ");
    test_printn(CH_DBG_STATISTICS);
    test_println("");
    test_print("--- CH_DBG_SYSTEM_STATE_CHECK: ");
    test_printn(CH_DBG_SYSTEM_STATE_CHECK);
    test_println("");
    test_print("--- CH_DBG_ENABLE_CHECKS:      ");
    test_printn(CH_DBG_ENABLE_CHECKS);
    test_println("");
    test_print("--- CH_DBG_ENABLE_ASSERTS:     ");
    test_printn(CH_DBG_ENABLE_ASSERTS);
    test_println("");
    test_print("--- CH_DBG_ENABLE_STACK_CHECK: ");
    test_printn(CH_DBG_ENABLE_STACK_CHECK);
    test_println("");
  }
}

static const testcase_t test_001_002 = {
  "Kernel Settings",
  NULL,
  NULL,
  test_001_002_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Information.
 */
const testcase_t * const test_sequence_001[] = {
  &test_001_001,
  &test_001_002,
  NULL
};
