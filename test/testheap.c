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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "test.h"

/**
 * @page test_heap Memory Heap test
 *
 * File: @ref testheap.c
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref heaps subsystem.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref heaps subsystem.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_HEAP
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_heap_001
 * .
 * @file testheap.c
 * @brief Heap test source file
 * @file testheap.h
 * @brief Heap header file
 */

#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP) || defined(__DOXYGEN__)

#define SIZE 16

static MemoryHeap test_heap;

/**
 * @page test_heap_001 Allocation and fragmentation test
 *
 * <h2>Description</h2>
 * Series of allocations/deallocations are performed in carefully designed
 * sequences in order to stimulate all the possible code paths inside the
 * allocator.<br>
 * The test expects to find the heap back to the initial status after each
 * sequence.
 */

static void heap1_setup(void) {

  chHeapInit(&test_heap, test.buffer, sizeof(union test_buffers));
}

static void heap1_execute(void) {
  void *p1, *p2, *p3;
  size_t n, sz;

  /* Unrelated, for coverage only.*/
  (void)chCoreStatus();

  /*
   * Test on the default heap in order to cover the core allocator at
   * least one time.
   */
  (void)chHeapStatus(NULL, &sz);
  p1 = chHeapAlloc(NULL, SIZE);
  test_assert(1, p1 != NULL, "allocation failed");
  chHeapFree(p1);
  p1 = chHeapAlloc(NULL, (size_t)-256);
  test_assert(2, p1 == NULL, "allocation not failed");

  /* Initial local heap state.*/
  (void)chHeapStatus(&test_heap, &sz);

  /* Same order.*/
  p1 = chHeapAlloc(&test_heap, SIZE);
  p2 = chHeapAlloc(&test_heap, SIZE);
  p3 = chHeapAlloc(&test_heap, SIZE);
  chHeapFree(p1);                               /* Does not merge.*/
  chHeapFree(p2);                               /* Merges backward.*/
  chHeapFree(p3);                               /* Merges both sides.*/
  test_assert(3, chHeapStatus(&test_heap, &n) == 1, "heap fragmented");

  /* Reverse order.*/
  p1 = chHeapAlloc(&test_heap, SIZE);
  p2 = chHeapAlloc(&test_heap, SIZE);
  p3 = chHeapAlloc(&test_heap, SIZE);
  chHeapFree(p3);                               /* Merges forward.*/
  chHeapFree(p2);                               /* Merges forward.*/
  chHeapFree(p1);                               /* Merges forward.*/
  test_assert(4, chHeapStatus(&test_heap, &n) == 1, "heap fragmented");

  /* Small fragments handling.*/
  p1 = chHeapAlloc(&test_heap, SIZE + 1);
  p2 = chHeapAlloc(&test_heap, SIZE);
  chHeapFree(p1);
  test_assert(5, chHeapStatus(&test_heap, &n) == 2, "invalid state");
  p1 = chHeapAlloc(&test_heap, SIZE);
  /* Note, the first situation happens when the alignment size is smaller
     than the header size, the second in the other cases.*/
  test_assert(6, (chHeapStatus(&test_heap, &n) == 1) ||
                 (chHeapStatus(&test_heap, &n) == 2), "heap fragmented");
  chHeapFree(p2);
  chHeapFree(p1);
  test_assert(7, chHeapStatus(&test_heap, &n) == 1, "heap fragmented");

  /* Skip fragment handling.*/
  p1 = chHeapAlloc(&test_heap, SIZE);
  p2 = chHeapAlloc(&test_heap, SIZE);
  chHeapFree(p1);
  test_assert(8, chHeapStatus(&test_heap, &n) == 2, "invalid state");
  p1 = chHeapAlloc(&test_heap, SIZE * 2);       /* Skips first fragment.*/
  chHeapFree(p1);
  chHeapFree(p2);
  test_assert(9, chHeapStatus(&test_heap, &n) == 1, "heap fragmented");

  /* Allocate all handling.*/
  (void)chHeapStatus(&test_heap, &n);
  p1 = chHeapAlloc(&test_heap, n);
  test_assert(10, chHeapStatus(&test_heap, &n) == 0, "not empty");
  chHeapFree(p1);

  test_assert(11, chHeapStatus(&test_heap, &n) == 1, "heap fragmented");
  test_assert(12, n == sz, "size changed");
}

ROMCONST struct testcase testheap1 = {
  "Heap, allocation and fragmentation test",
  heap1_setup,
  NULL,
  heap1_execute
};

#endif /* CH_USE_HEAP.*/

/**
 * @brief   Test sequence for heap.
 */
ROMCONST struct testcase * ROMCONST patternheap[] = {
#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP) || defined(__DOXYGEN__)
  &testheap1,
#endif
  NULL
};
