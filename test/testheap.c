/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#include <ch.h>

#include "test.h"

/**
 * @page test_heap Memory Heap test
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref Heap subsystem.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref Heap subsystem
 * code as a necessary step in order to assess its maturity level.
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

#if CH_USE_HEAP

#define SIZE 16

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

static char *heap1_gettest(void) {

  return "Heap, allocation and fragmentation test";
}

static void heap1_execute(void) {
  void *p1, *p2, *p3;
  size_t n, sz;

  /* Test skipped if the heap is already fragmented. */
  if ((n = chHeapStatus(&sz)) == 1) {
    test_print("--- Size  : ");
    test_printn(sz);
    test_println(" bytes, not fragmented");

    /* Same order */
    p1 = chHeapAlloc(SIZE);
    p2 = chHeapAlloc(SIZE);
    p3 = chHeapAlloc(SIZE);
    chHeapFree(p1);                           /* Does not merge */
    chHeapFree(p2);                           /* Merges backward */
    chHeapFree(p3);                           /* Merges both sides */
    test_assert(1, chHeapStatus(&n) == 1, "heap fragmented");

    /* Reverse order */
    p1 = chHeapAlloc(SIZE);
    p2 = chHeapAlloc(SIZE);
    p3 = chHeapAlloc(SIZE);
    chHeapFree(p3);                           /* Merges forward */
    chHeapFree(p2);                           /* Merges forward */
    chHeapFree(p1);                           /* Merges forward */
    test_assert(2, chHeapStatus(&n) == 1, "heap fragmented");

    /* Small fragments handling */
    p1 = chHeapAlloc(SIZE + 1);
    p2 = chHeapAlloc(SIZE);
    chHeapFree(p1);
    test_assert(3, chHeapStatus(&n) == 2, "invalid state");
    p1 = chHeapAlloc(SIZE);
    test_assert(4, chHeapStatus(&n) == 1, "heap fragmented");
    chHeapFree(p2);
    chHeapFree(p1);
    test_assert(5, chHeapStatus(&n) == 1, "heap fragmented");

    /* Skip fragment handling */
    p1 = chHeapAlloc(SIZE);
    p2 = chHeapAlloc(SIZE);
    chHeapFree(p1);
    test_assert(6, chHeapStatus(&n) == 2, "invalid state");
    p1 = chHeapAlloc(SIZE * 2);                 /* Skips first fragment */
    chHeapFree(p1);
    chHeapFree(p2);
    test_assert(7, chHeapStatus(&n) == 1, "heap fragmented");

    /* Allocate all handling */
    (void)chHeapStatus(&n);
    p1 = chHeapAlloc(n);
    test_assert(8, chHeapStatus(&n) == 0, "not empty");
    chHeapFree(p1);

    test_assert(9, chHeapStatus(&n) == 1, "heap fragmented");
    test_assert(10, n == sz, "size changed");
  }
  else {
    test_print("--- Size  : ");
    test_printn(sz);
    test_println(" bytes, fragmented, test skipped");
  }
}

const struct testcase testheap1 = {
  heap1_gettest,
  NULL,
  NULL,
  heap1_execute
};

#endif /* CH_USE_HEAP */

/*
 * Test sequence for heap pattern.
 */
const struct testcase * const patternheap[] = {
#if CH_USE_HEAP
  &testheap1,
#endif
  NULL
};
