/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "test.h"

/**
 * @page test_pools Memory Pools test
 *
 * File: @ref testpools.c
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

#if CH_USE_MEMPOOLS || defined(__DOXYGEN__)

static MEMORYPOOL_DECL(mp1, THD_WA_SIZE(THREADS_STACK_SIZE), NULL);

/**
 * @page test_pools_001 Allocation and enqueuing test
 *
 * <h2>Description</h2>
 * Five memory blocks are added to a memory pool then removed.<br>
 * The test expects to find the pool queue in the proper status after each
 * operation.
 */

static void *null_provider(size_t size) {

  (void)size;
  return NULL;
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

  /* Covering the case where a provider is unable to return more memory.*/
  chPoolInit(&mp1, 16, null_provider);
  test_assert(3, chPoolAlloc(&mp1) == NULL, "provider returned memory");
}

ROMCONST struct testcase testpools1 = {
  "Memory Pools, queue/dequeue",
  pools1_setup,
  NULL,
  pools1_execute
};

#endif /* CH_USE_MEMPOOLS */

/*
 * @brief   Test sequence for pools.
 */
ROMCONST struct testcase * ROMCONST patternpools[] = {
#if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
  &testpools1,
#endif
  NULL
};
