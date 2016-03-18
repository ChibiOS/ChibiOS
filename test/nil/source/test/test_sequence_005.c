/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @page test_sequence_005 Memory Pools
 *
 * File: @ref test_sequence_005.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/NIL functionalities related to
 * memory pools.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_005_001
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#define MEMORY_POOL_SIZE 4

static uint32_t objects[MEMORY_POOL_SIZE];
static MEMORYPOOL_DECL(mp1, sizeof (uint32_t), NULL);

static void *null_provider(size_t size, unsigned align) {

  (void)size;
  (void)align;

  return NULL;
}

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_005_001 Loading and empting the memory pool
 *
 * <h2>Description</h2>
 * The memory pool functionality is tested by loading and empting it,
 * all conditions are tested.
 *
 * <h2>Test Steps</h2>
 * - Adding the objects to the pool using chPoolLoadArray().
 * - Emptying the pool using chPoolAlloc().
 * - Now must be empty.
 * - Adding the objects to the pool using chPoolFree().
 * - Emptying the pool using chPoolAlloc() again.
 * - Now must be empty again.
 * - Covering the case where a provider is unable to return more
 *   memory.
 * .
 */

static void test_005_001_setup(void) {
  chPoolObjectInit(&mp1, sizeof (uint32_t), NULL);
}

static void test_005_001_execute(void) {
  unsigned i;

  /* Adding the objects to the pool using chPoolLoadArray().*/
  test_set_step(1);
  {
    chPoolLoadArray(&mp1, objects, MEMORY_POOL_SIZE);
  }

  /* Emptying the pool using chPoolAlloc().*/
  test_set_step(2);
  {
    for (i = 0; i < MEMORY_POOL_SIZE; i++)
      test_assert(chPoolAlloc(&mp1) != NULL, "list empty");
  }

  /* Now must be empty.*/
  test_set_step(3);
  {
    test_assert(chPoolAlloc(&mp1) == NULL, "list not empty");
  }

  /* Adding the objects to the pool using chPoolFree().*/
  test_set_step(4);
  {
    for (i = 0; i < MEMORY_POOL_SIZE; i++)
      chPoolFree(&mp1, &objects[i]);
  }

  /* Emptying the pool using chPoolAlloc() again.*/
  test_set_step(5);
  {
    for (i = 0; i < MEMORY_POOL_SIZE; i++)
      test_assert(chPoolAlloc(&mp1) != NULL, "list empty");
  }

  /* Now must be empty again.*/
  test_set_step(6);
  {
    test_assert(chPoolAlloc(&mp1) == NULL, "list not empty");
  }

  /* Covering the case where a provider is unable to return more
     memory.*/
  test_set_step(7);
  {
    chPoolObjectInit(&mp1, sizeof (uint32_t), null_provider);
    test_assert(chPoolAlloc(&mp1) == NULL, "provider returned memory");
  }
}

static const testcase_t test_005_001 = {
  "Loading and empting the memory pool",
  test_005_001_setup,
  NULL,
  test_005_001_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Memory Pools.
 */
const testcase_t * const test_sequence_005[] = {
  &test_005_001,
  NULL
};
