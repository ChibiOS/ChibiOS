/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

#include <ch.h>

#include "test.h"

#ifdef CH_USE_DYNAMIC

static msg_t thread(void *p) {

  test_emit_token(*(char *)p);
  return 0;
}

#ifdef CH_USE_HEAP
static char *dyn1_gettest(void) {

  return "Dynamic APIs, threads creation from heap";
}

static void dyn1_setup(void) {
}

static void dyn1_teardown(void) {
}

static void dyn1_execute(void) {
  size_t n, sz;
  tprio_t prio = chThdGetPriority();

  /* Test skipped if the heap is already fragmented. */
  if ((n = chHeapStatus(&sz)) == 1) {
    /* Starting threads from the heap. */
    threads[0] = chThdCreateFromHeap(THD_WA_SIZE(THREADS_STACK_SIZE),
                                     prio-1, thread, "A");
    threads[1] = chThdCreateFromHeap(THD_WA_SIZE(THREADS_STACK_SIZE),
                                     prio-2, thread, "B");

    test_assert((threads[0] != NULL) &&
                (threads[1] != NULL) &&
                (threads[2] == NULL) &&
                (threads[3] == NULL) &&
                (threads[4] == NULL),
                "thread creation failed");

    /* Claiming the memory from terminated threads. */
    test_wait_threads();
    test_assert_sequence("AB");

    /* Heap status checked again.*/
    test_assert(chHeapStatus(&n) == 1, "heap fragmented");
    test_assert(n == sz, "heap size changed");
  }
}

const struct testcase testdyn1 = {
  dyn1_gettest,
  dyn1_setup,
  dyn1_teardown,
  dyn1_execute
};
#endif /* CH_USE_HEAP */

#ifdef CH_USE_MEMPOOLS
static MemoryPool mp1;

static char *dyn2_gettest(void) {

  return "Dynamic APIs, threads creation from memory pool";
}

static void dyn2_setup(void) {

  chPoolInit(&mp1, THD_WA_SIZE(THREADS_STACK_SIZE));
}

static void dyn2_teardown(void) {
}

static void dyn2_execute(void) {
  int i;
  tprio_t prio = chThdGetPriority();

  /* Adding the WAs to the pool. */
  for (i = 0; i < 5; i++)
    chPoolFree(&mp1, wa[i]);

  /* Starting threads from the memory pool. */
  threads[0] = chThdCreateFromMemoryPool(&mp1, prio-1, thread, "A");
  threads[1] = chThdCreateFromMemoryPool(&mp1, prio-2, thread, "B");
  threads[2] = chThdCreateFromMemoryPool(&mp1, prio-3, thread, "C");
  threads[3] = chThdCreateFromMemoryPool(&mp1, prio-4, thread, "D");
  threads[4] = chThdCreateFromMemoryPool(&mp1, prio-5, thread, "E");

  test_assert((threads[0] != NULL) &&
              (threads[1] != NULL) &&
              (threads[2] != NULL) &&
              (threads[3] != NULL) &&
              (threads[4] != NULL),
              "thread creation failed");

  /* Claiming the memory from terminated threads. */
  test_wait_threads();
  test_assert_sequence("ABCDE");

  /* Now the pool must be full again. */
  for (i = 0; i < 5; i++)
    test_assert(chPoolAlloc(&mp1) != NULL, "pool list empty");
  test_assert(chPoolAlloc(&mp1) == NULL, "pool list not empty");
}

const struct testcase testdyn2 = {
  dyn2_gettest,
  dyn2_setup,
  dyn2_teardown,
  dyn2_execute
};
#endif /* CH_USE_MEMPOOLS */

#endif /* CH_USE_DYNAMIC */
