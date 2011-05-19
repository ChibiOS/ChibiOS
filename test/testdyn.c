/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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
 * @page test_dynamic Dynamic APIs test
 *
 * File: @ref testdyn.c
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the dynamic thread creation
 * APIs.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the dynamic APIs code.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_DYNAMIC
 * - @p CH_USE_HEAP
 * - @p CH_USE_MEMPOOLS
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_dynamic_001
 * - @subpage test_dynamic_002
 * - @subpage test_dynamic_003
 * .
 * @file testdyn.c
 * @brief Dynamic thread APIs test source file
 * @file testdyn.h
 * @brief Dynamic thread APIs test header file
 */

#if CH_USE_DYNAMIC || defined(__DOXYGEN__)
#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP) || defined(__DOXYGEN__)
static MemoryHeap heap1;
#endif
#if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
static MemoryPool mp1;
#endif

/**
 * @page test_dynamic_001 Threads creation from Memory Heap
 *
 * <h2>Description</h2>
 * Two threads are started by allocating the memory from the Memory Heap then
 * the remaining heap space is arbitrarily allocated and a third tread startup
 * is attempted.<br>
 * The test expects the first two threads to successfully start and the last
 * one to fail.
 */

static msg_t thread(void *p) {

  test_emit_token(*(char *)p);
  return 0;
}

#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP) || defined(__DOXYGEN__)
static void dyn1_setup(void) {

  chHeapInit(&heap1, test.buffer, sizeof(union test_buffers));
}

static void dyn1_execute(void) {
  size_t n, sz;
  void *p1;
  tprio_t prio = chThdGetPriority();

  (void)chHeapStatus(&heap1, &sz);
  /* Starting threads from the heap. */
  threads[0] = chThdCreateFromHeap(&heap1, THD_WA_SIZE(THREADS_STACK_SIZE),
                                   prio-1, thread, "A");
  threads[1] = chThdCreateFromHeap(&heap1, THD_WA_SIZE(THREADS_STACK_SIZE),
                                   prio-2, thread, "B");
  /* Allocating the whole heap in order to make the thread creation fail.*/
  (void)chHeapStatus(&heap1, &n);
  p1 = chHeapAlloc(&heap1, n);
  threads[2] = chThdCreateFromHeap(&heap1, THD_WA_SIZE(THREADS_STACK_SIZE),
                                   prio-3, thread, "C");
  chHeapFree(p1);

  test_assert(1, (threads[0] != NULL) &&
                 (threads[1] != NULL) &&
                 (threads[2] == NULL) &&
                 (threads[3] == NULL) &&
                 (threads[4] == NULL),
                 "thread creation failed");

  /* Claiming the memory from terminated threads. */
  test_wait_threads();
  test_assert_sequence(2, "AB");

  /* Heap status checked again.*/
  test_assert(3, chHeapStatus(&heap1, &n) == 1, "heap fragmented");
  test_assert(4, n == sz, "heap size changed");
}

ROMCONST struct testcase testdyn1 = {
  "Dynamic APIs, threads creation from heap",
  dyn1_setup,
  NULL,
  dyn1_execute
};
#endif /* (CH_USE_HEAP && !CH_USE_MALLOC_HEAP) */

#if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
/**
 * @page test_dynamic_002 Threads creation from Memory Pool
 *
 * <h2>Description</h2>
 * Five thread creation are attempted from a pool containing only four
 * elements.<br>
 * The test expects the first four threads to successfully start and the last
 * one to fail.
 */

static void dyn2_setup(void) {

  chPoolInit(&mp1, THD_WA_SIZE(THREADS_STACK_SIZE), NULL);
}

static void dyn2_execute(void) {
  int i;
  tprio_t prio = chThdGetPriority();

  /* Adding the WAs to the pool. */
  for (i = 0; i < 4; i++)
    chPoolFree(&mp1, wa[i]);

  /* Starting threads from the memory pool. */
  threads[0] = chThdCreateFromMemoryPool(&mp1, prio-1, thread, "A");
  threads[1] = chThdCreateFromMemoryPool(&mp1, prio-2, thread, "B");
  threads[2] = chThdCreateFromMemoryPool(&mp1, prio-3, thread, "C");
  threads[3] = chThdCreateFromMemoryPool(&mp1, prio-4, thread, "D");
  threads[4] = chThdCreateFromMemoryPool(&mp1, prio-5, thread, "E");

  test_assert(1, (threads[0] != NULL) &&
                 (threads[1] != NULL) &&
                 (threads[2] != NULL) &&
                 (threads[3] != NULL) &&
                 (threads[4] == NULL),
                 "thread creation failed");

  /* Claiming the memory from terminated threads. */
  test_wait_threads();
  test_assert_sequence(2, "ABCD");

  /* Now the pool must be full again. */
  for (i = 0; i < 4; i++)
    test_assert(3, chPoolAlloc(&mp1) != NULL, "pool list empty");
  test_assert(4, chPoolAlloc(&mp1) == NULL, "pool list not empty");
}

ROMCONST struct testcase testdyn2 = {
  "Dynamic APIs, threads creation from memory pool",
  dyn2_setup,
  NULL,
  dyn2_execute
};
#endif /* CH_USE_MEMPOOLS */

#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP && CH_USE_REGISTRY) ||              \
    defined(__DOXYGEN__)
/**
 * @page test_dynamic_003 Registry and References test
 *
 * <h2>Description</h2>
 * Registry and Thread References APIs are tested for functionality and
 * coverage.
 */

static bool_t regfind(Thread *tp) {
  Thread *ftp;
  bool_t found = FALSE;

  ftp = chRegFirstThread();
  do {
    found |= ftp == tp;
    ftp = chRegNextThread(ftp);
  } while (ftp != NULL);
  return found;
}

static void dyn3_setup(void) {

  chHeapInit(&heap1, test.buffer, sizeof(union test_buffers));
}

static void dyn3_execute(void) {
  Thread *tp;
  tprio_t prio = chThdGetPriority();

  /* Testing references increase/decrease and final detach.*/
  tp = chThdCreateFromHeap(&heap1, WA_SIZE, prio-1, thread, "A");
  test_assert(1, tp->p_refs == 1, "wrong initial reference counter");
  chThdAddRef(tp);
  test_assert(2, tp->p_refs == 2, "references increase failure");
  chThdRelease(tp);
  test_assert(3, tp->p_refs == 1, "references decrease failure");

  /* Verify the new threads count.*/
  test_assert(4, regfind(tp), "thread missing from registry");
  test_assert(5, regfind(tp), "thread disappeared");

  /* Detach and let the thread execute and terminate.*/
  chThdRelease(tp);
  test_assert(6, tp->p_refs == 0, "detach failure");
  test_assert(7, tp->p_state == THD_STATE_READY, "invalid state");
  test_assert(8, regfind(tp), "thread disappeared");
  test_assert(9, regfind(tp), "thread disappeared");
  chThdSleepMilliseconds(50);           /* The thread just terminates.      */
  test_assert(10, tp->p_state == THD_STATE_FINAL, "invalid state");

  /* Clearing the zombie by scanning the registry.*/
  test_assert(11, regfind(tp), "thread disappeared");
  test_assert(12, !regfind(tp), "thread still in registry");
}

ROMCONST struct testcase testdyn3 = {
  "Dynamic APIs, registry and references",
  dyn3_setup,
  NULL,
  dyn3_execute
};
#endif /* CH_USE_HEAP && CH_USE_REGISTRY */
#endif /* CH_USE_DYNAMIC */

/**
 * @brief   Test sequence for dynamic APIs.
 */
ROMCONST struct testcase * ROMCONST patterndyn[] = {
#if CH_USE_DYNAMIC || defined(__DOXYGEN__)
#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP) || defined(__DOXYGEN__)
  &testdyn1,
#endif
#if CH_USE_MEMPOOLS || defined(__DOXYGEN__)
  &testdyn2,
#endif
#if (CH_USE_HEAP && !CH_USE_MALLOC_HEAP && CH_USE_REGISTRY) ||              \
    defined(__DOXYGEN__)
  &testdyn3,
#endif
#endif
  NULL
};
