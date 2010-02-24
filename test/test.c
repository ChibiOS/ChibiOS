/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
#include "hal.h"

#include "test.h"
#include "testthd.h"
#include "testsem.h"
#include "testmtx.h"
#include "testmsg.h"
#include "testmbox.h"
#include "testevt.h"
#include "testheap.h"
#include "testpools.h"
#include "testdyn.h"
#include "testqueues.h"
#include "testbmk.h"

/*
 * Array of all the test patterns.
 */
static const struct testcase **patterns[] = {
  patternthd,
  patternsem,
  patternmtx,
  patternmsg,
  patternmbox,
  patternevt,
  patternheap,
  patternpools,
  patterndyn,
  patternqueues,
  patternbmk,
  NULL
};

static bool_t local_fail, global_fail;
static unsigned failpoint;
static char tokens_buffer[MAX_TOKENS];
static char *tokp;

/*
 * Static working areas, the following areas can be used for threads or
 * used as temporary buffers.
 */
union test_buffers test;

/*
 * Pointers to the spawned threads.
 */
Thread *threads[MAX_THREADS];

/*
 * Pointers to the working areas.
 */
void * const wa[5] = {test.wa.T0, test.wa.T1, test.wa.T2,
                      test.wa.T3, test.wa.T4};

/*
 * Console output.
 */
static BaseChannel *chp;

void test_printn(uint32_t n) {
  char buf[16], *p;

  if (!n)
    chIOPut(chp, '0');
  else {
    p = buf;
    while (n)
      *p++ = (n % 10) + '0', n /= 10;
    while (p > buf)
      chIOPut(chp, *--p);
  }
}

void test_print(char *msgp) {

  while (*msgp)
    chIOPut(chp, *msgp++);
}

void test_println(char *msgp) {

  test_print(msgp);
  chIOPut(chp, '\r');
  chIOPut(chp, '\n');
}

/*
 * Tokens.
 */
static void clear_tokens(void) {

  tokp = tokens_buffer;
}

static void print_tokens(void) {
  char *cp = tokens_buffer;

  while (cp < tokp)
    chIOPut(chp, *cp++);
}

void test_emit_token(char token) {

  chSysLock();
  *tokp++ = token;
  chSysUnlock();
}

/*
 * Assertions.
 */
bool_t _test_fail(unsigned point) {

  local_fail = TRUE;
  global_fail = TRUE;
  failpoint = point;
  return TRUE;
}

bool_t _test_assert(unsigned point, bool_t condition) {

  if (!condition)
    return _test_fail(point);
  return FALSE;
}

bool_t _test_assert_sequence(unsigned point, char *expected) {
  char *cp = tokens_buffer;
  while (cp < tokp) {
    if (*cp++ != *expected++)
     return _test_fail(point);
  }
  if (*expected)
    return _test_fail(point);
  clear_tokens();
  return FALSE;
}

bool_t _test_assert_time_window(unsigned point, systime_t start, systime_t end) {

  return _test_assert(point, chTimeIsWithin(start, end));
}

/*
 * Threads utils.
 */
void test_terminate_threads(void) {
  int i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i])
      chThdTerminate(threads[i]);
}

void test_wait_threads(void) {
  int i;

  for (i = 0; i < MAX_THREADS; i++)
    if (threads[i] != NULL) {
      chThdWait(threads[i]);
      threads[i] = NULL;
    }
}

#if CH_DBG_THREADS_PROFILING
void test_cpu_pulse(unsigned duration) {
  systime_t start, end, now;

  start = chThdSelf()->p_time;
  end = start + MS2ST(duration);
  do {
    now = chThdSelf()->p_time;
#if defined(SIMULATOR)
    ChkIntSources();
#endif
  }
  while (end > start ? (now >= start) && (now < end) :
                       (now >= start) || (now < end));
}
#endif

systime_t test_wait_tick(void) {

  chThdSleep(1);
  return chTimeNow();
}

/*
 * Timer utils.
 */
static VirtualTimer vt;
bool_t test_timer_done;

static void tmr(void *p) {
  (void)p;

  test_timer_done = TRUE;
}

void test_start_timer(unsigned ms) {

  systime_t duration = MS2ST(ms);
  test_timer_done = FALSE;
  chSysLock();
  chVTSetI(&vt, duration, tmr, NULL);
  chSysUnlock();
}

/*
 * Test suite execution.
 */
static void execute_test(const struct testcase *tcp) {
  int i;

  /* Initialization */
  clear_tokens();
  local_fail = FALSE;
  for (i = 0; i < MAX_THREADS; i++)
    threads[i] = NULL;

  if (tcp->setup != NULL)
    tcp->setup();
  tcp->execute();
  if (tcp->teardown != NULL)
    tcp->teardown();

  test_wait_threads();
}

static void print_line(void) {
  unsigned i;

  for (i = 0; i < 76; i++)
    chIOPut(chp, '-');
  chIOPut(chp, '\r');
  chIOPut(chp, '\n');
}

msg_t TestThread(void *p) {
  int i, j;

  chp = p;
  test_println("");
  test_println("*** ChibiOS/RT test suite");
  test_println("***");
  test_print("*** Kernel:       ");
  test_println(CH_KERNEL_VERSION);
#ifdef __GNUC__
  test_print("*** GCC Version:  ");
  test_println(__VERSION__);
#endif
  test_print("*** Architecture: ");
  test_println(CH_ARCHITECTURE_NAME);
#ifdef CH_CORE_VARIANT_NAME
  test_print("*** Core Variant: ");
  test_println(CH_CORE_VARIANT_NAME);
#endif
#ifdef PLATFORM_NAME
  test_print("*** Platform:     ");
  test_println(PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  test_print("*** Test Board:   ");
  test_println(BOARD_NAME);
#endif
  test_println("");

  global_fail = FALSE;
  i = 0;
  while (patterns[i]) {
    j = 0;
    while (patterns[i][j]) {
      print_line();
      test_print("--- Test Case ");
      test_printn(i + 1);
      test_print(".");
      test_printn(j + 1);
      test_print(" (");
      test_print(patterns[i][j]->gettest());
      test_println(")");
#if DELAY_BETWEEN_TESTS > 0
      chThdSleepMilliseconds(DELAY_BETWEEN_TESTS);
#endif
      execute_test(patterns[i][j]);
      if (local_fail) {
        test_print("--- Result: FAILURE (#");
        test_printn(failpoint);
        test_print(" [");
        print_tokens();
        test_println("])");
      }
      else
        test_println("--- Result: SUCCESS");
      j++;
    }
    i++;
  }
  print_line();
  test_println("");
  test_print("Final result: ");
  if (global_fail)
    test_println("FAILURE");
  else
    test_println("SUCCESS");

  return (msg_t)global_fail;
}
