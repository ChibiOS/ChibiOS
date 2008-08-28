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

#ifdef CH_USE_HEAP

#define SIZE 16

static char *heap1_gettest(void) {

  return "Heap, allocation and fragmentation test";
}

static void heap1_setup(void) {
}

static void heap1_teardown(void) {
}

static void heap1_execute(void) {
  void *p1, *p2, *p3;

  /* Test skipped if the heap is already fragmented. */
  if (chHeapNotFragmented()) {
    /* Same order */
    p1 = chHeapAlloc(SIZE);
    p2 = chHeapAlloc(SIZE);
    p3 = chHeapAlloc(SIZE);
    chHeapFree(p1);               /* Does not merge */
    chHeapFree(p2);               /* Merges backward */
    chHeapFree(p3);               /* Merges both sides */
    test_assert(chHeapNotFragmented(), "heap fragmented #1");

    /* Reverse order */
    p1 = chHeapAlloc(SIZE);
    p2 = chHeapAlloc(SIZE);
    p3 = chHeapAlloc(SIZE);
    chHeapFree(p3);               /* Merges forward */
    chHeapFree(p2);               /* Merges forward */
    chHeapFree(p1);               /* Merges forward */
    test_assert(chHeapNotFragmented(), "heap fragmented #2");
  }
}

const struct testcase testheap1 = {
  heap1_gettest,
  heap1_setup,
  heap1_teardown,
  heap1_execute
};

#endif /* CH_USE_HEAP */
