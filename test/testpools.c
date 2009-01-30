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

#ifdef CH_USE_MEMPOOLS

static MemoryPool mp1;

static char *pools1_gettest(void) {

  return "Memory Pools, allocation and enqueuing test";
}

static void pools1_setup(void) {

  chPoolInit(&mp1, THD_WA_SIZE(THREADS_STACK_SIZE));
}

static void pools1_teardown(void) {
}

static void pools1_execute(void) {
  int i;

  /* Adding the WAs to the pool. */
  for (i = 0; i < MAX_THREADS; i++)
    chPoolFree(&mp1, wa[i]);

  /* Empting the pool again. */
  for (i = 0; i < MAX_THREADS; i++)
    test_assert(chPoolAlloc(&mp1) != NULL, "pool list empty");

  /* Now must be empty. */
  test_assert(chPoolAlloc(&mp1) == NULL, "pool list not empty");
}

const struct testcase testpools1 = {
  pools1_gettest,
  pools1_setup,
  pools1_teardown,
  pools1_execute
};

#endif /* CH_USE_MEMPOOLS */
