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
#include "testrdy.h"
#include "testsem.h"
#include "testmtx.h"
#include "testcond.h"
#include "testmsg.h"
#include "testevt.h"
#include "testheap.h"
#include "testpools.h"
#include "testdyn.h"
#include "testbmk.h"

/*
 * Array of all the test cases.
 */
static const struct testcase *tests[] = {
  &testrdy1,
  &testrdy2,
#ifdef CH_USE_SEMAPHORES
  &testsem1,
  &testsem2,
#endif
#ifdef CH_USE_MUTEXES
  &testmtx1,
  &testmtx2,
  &testmtx3,
#ifdef CH_USE_CONDVARS
  &testcond1,
  &testcond2,
#endif
#endif
#ifdef CH_USE_MESSAGES
  &testmsg1,
#endif
#ifdef CH_USE_EVENTS
  &testevt1,
#endif
#ifdef CH_USE_HEAP
  &testheap1,
#endif
#ifdef CH_USE_MEMPOOLS
  &testpools1,
#endif
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_HEAP)
  &testdyn1,
#endif
#if defined(CH_USE_DYNAMIC) && defined(CH_USE_MEMPOOLS)
  &testdyn2,
#endif
  &testbmk1,
  &testbmk2,
  &testbmk3,
  &testbmk4,
  &testbmk5,
  &testbmk6,
  &testbmk7,
  &testbmk8,
  NULL
};

static bool_t local_fail, global_fail;
static char *failmsg;
static char tokens_buffer[MAX_TOKENS];
static char *tokp;
static WORKING_AREA(waT0, THREADS_STACK_SIZE);
static WORKING_AREA(waT1, THREADS_STACK_SIZE);
static WORKING_AREA(waT2, THREADS_STACK_SIZE);
static WORKING_AREA(waT3, THREADS_STACK_SIZE);
static WORKING_AREA(waT4, THREADS_STACK_SIZE);

void *wa[MAX_THREADS] = {waT0, waT1, waT2, waT3, waT4};
Thread *threads[MAX_THREADS];

/*
 * Console output.
 */
static FullDuplexDriver *comp;

void test_printn(uint32_t n) {
  char buf[16], *p;

  if (!n)
    chFDDPut(comp, '0');
  else {
    p = buf;
    while (n)
      *p++ = (n % 10) + '0', n /= 10;
    while (p > buf)
      chFDDPut(comp, *--p);
  }
}

void test_print(char *msgp) {

  while (*msgp)
    chFDDPut(comp, *msgp++);
}

void test_println(char *msgp) {

  test_print(msgp);
  chFDDPut(comp, '\r');
  chFDDPut(comp, '\n');
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
    chFDDPut(comp, *cp++);
}

void test_emit_token(char token) {

  chSysLock();
  *tokp++ = token;
  chSysUnlock();
}

/*
 * Assertions.
 */
bool_t _test_fail(char * msg) {

  local_fail = TRUE;
  global_fail = TRUE;
  failmsg = msg;
  return TRUE;
}

bool_t _test_assert(bool_t condition, char * msg) {

  if (!condition)
    return _test_fail(msg);
  return FALSE;
}

bool_t _test_assert_sequence(char *expected) {
  char *cp = tokens_buffer;
  while (cp < tokp) {
    if (*cp++ != *expected++)
     return _test_fail(NULL);
  }
  if (*expected)
    return _test_fail(NULL);
  return FALSE;
}

bool_t _test_assert_time_window(systime_t start, systime_t end) {

  return _test_assert(chSysInTimeWindow(start, end), "time window error");
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
    if (threads[i])
      chThdWait(threads[i]);
}

void test_cpu_pulse(unsigned ms) {

  systime_t duration = MS2ST(ms);
  systime_t start = chSysGetTime();
  while (chSysInTimeWindow(start, start + duration)) {
#if defined(WIN32)
    ChkIntSources();
#endif
  }
}

systime_t test_wait_tick(void) {

  chThdSleep(1);
  return chSysGetTime();
}

/*
 * Timer utils.
 */
static VirtualTimer vt;
bool_t test_timer_done;

static void tmr(void *p) {

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

  tcp->setup();
  tcp->execute();
  tcp->teardown();
}

msg_t TestThread(void *p) {
  int i;

  comp = p;
  test_println("");
  test_println("*****************************");
  test_println("*** ChibiOS/RT test suite ***");
  test_println("*****************************");
  test_println("");

  global_fail = FALSE;
  i = 0;
  while (tests[i]) {
#if DELAY_BETWEEN_TESTS > 0
    chThdSleepMilliseconds(DELAY_BETWEEN_TESTS);
#endif
    test_println("---------------------------------------------------------------------------");
    test_print("--- Test Case ");
    test_printn(i + 1);
    test_print(" (");
    test_print(tests[i]->gettest());
    test_println(")");
    execute_test(tests[i]);
    if (local_fail) {
      test_print("--- Result: FAIL (");
      if (failmsg)
        test_print(failmsg);
      else {
        test_print("sequence error: ");
        print_tokens();
      }
      test_println(")");
    }
    else
      test_println("--- Result: SUCCESS");
    i++;
  }
  test_println("---------------------------------------------------------------------------");
  test_println("");
  test_print("Final result: ");
  if (global_fail)
    test_println("FAIL");
  else
    test_println("SUCCESS");

  return 0;
}
