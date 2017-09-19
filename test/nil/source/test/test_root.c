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
 * Test suite for ChibiOS/NIL. The purpose of this suite is to perform
 * unit tests on the NIL modules and to converge to 100% code coverage
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
#if (CH_CFG_USE_SEMAPHORES) || defined(__DOXYGEN__)
  test_sequence_003,
#endif
  test_sequence_004,
#if (CH_CFG_USE_MAILBOXES) || defined(__DOXYGEN__)
  test_sequence_005,
#endif
#if (CH_CFG_USE_MEMPOOLS) || defined(__DOXYGEN__)
  test_sequence_006,
#endif
#if (CH_CFG_USE_HEAP) || defined(__DOXYGEN__)
  test_sequence_007,
#endif
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

semaphore_t gsem1, gsem2;
thread_reference_t gtr1;

/*
 * Support thread.
 */
THD_WORKING_AREA(wa_test_support, 128);
THD_FUNCTION(test_support, arg) {
#if CH_CFG_USE_EVENTS == TRUE
  thread_t *tp = (thread_t *)arg;
#else
  (void)arg;
#endif

  /* Initializing global resources.*/
  chSemObjectInit(&gsem1, 0);
  chSemObjectInit(&gsem2, 0);

  while (true) {
    chSysLock();
    if (chSemGetCounterI(&gsem1) < 0)
      chSemSignalI(&gsem1);
    chSemResetI(&gsem2, 0);
    chThdResumeI(&gtr1, MSG_OK);
#if CH_CFG_USE_EVENTS == TRUE
    chEvtSignalI(tp, 0x55);
#endif
    chSchRescheduleS();
    chSysUnlock();

    chThdSleepMilliseconds(250);
  }
}

#endif /* !defined(__DOXYGEN__) */
