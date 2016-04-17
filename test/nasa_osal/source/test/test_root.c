/* Copyright statement.*/

/**
 * @mainpage Test Suite Specification
 * Test suite for NASA OSAL implementation over ChibiOS/RT. The purpose
 * of this suite is to perform unit tests on the OSAL module and to
 * converge to 100% code coverage through successive improvements.
 *
 * <h2>Test Sequences</h2>
 * - @subpage test_sequence_001
 * - @subpage test_sequence_002
 * - @subpage test_sequence_003
 * - @subpage test_sequence_004
 * - @subpage test_sequence_005
 * - @subpage test_sequence_006
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
  test_sequence_005,
  test_sequence_006,
  NULL
};

/*===========================================================================*/
/* Shared code.                                                              */
/*===========================================================================*/

THD_WORKING_AREA(wa_test1, TASKS_STACK_SIZE);
THD_WORKING_AREA(wa_test2, TASKS_STACK_SIZE);
THD_WORKING_AREA(wa_test3, TASKS_STACK_SIZE);
THD_WORKING_AREA(wa_test4, TASKS_STACK_SIZE);

#endif /* !defined(__DOXYGEN__) */
