/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "test.h"

/**
 * @page test_pools Memory Pools test
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref pools subsystem.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref pools code.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_MEMPOOLS
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_pools_001
 * .
 * @file testpools.c
 * @brief Memory Pools test source file
 * @file testpools.h
 * @brief Memory Pools test header file
 */

#if CH_USE_MEMPOOLS

static MEMORYPOOL_DECL(mp1, THD_WA_SIZE(THREADS_STACK_SIZE), NULL);

/**
 * @page test_pools_001 Allocation and enqueuing test
 *
 * <h2>Description</h2>
 * Five memory blocks are added to a memory pool then removed.<br>
 * The test expects to find the pool queue in the proper status after each
 * operation.
 */

static char *pools1_gettest(void) {

  return "Memory Pools, queue/dequeue";
}

static void pools1_setup(void) {

  chPoolInit(&mp1, THD_WA_SIZE(THREADS_STACK_SIZE), NULL);
}

static void pools1_execute(void) {
  int i;

  /* Adding the WAs to the pool. */
  for (i = 0; i < MAX_THREADS; i++)
    chPoolFree(&mp1, wa[i]);

  /* Empting the pool again. */
  for (i = 0; i < MAX_THREADS; i++)
    test_assert(1, chPoolAlloc(&mp1) != NULL, "list empty");

  /* Now must be empty. */
  test_assert(2, chPoolAlloc(&mp1) == NULL, "list not empty");
}

const struct testcase testpools1 = {
  pools1_gettest,
  pools1_setup,
  NULL,
  pools1_execute
};

#endif /* CH_USE_MEMPOOLS */

/*
 * Test sequence for pools pattern.
 */
const struct testcase * const patternpools[] = {
#if CH_USE_MEMPOOLS
  &testpools1,
#endif
  NULL
};
