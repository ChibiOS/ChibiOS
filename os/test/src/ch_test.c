/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ch_test.c
 * @brief   Unit Tests Engine module code.
 *
 * @addtogroup CH_TEST
 * @{
 */

#include "ch_test.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Test engine context variable.
 */
ch_test_context_t chtest;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if (TEST_CFG_CHIBIOS_SUPPORT == TRUE) || defined(__DOXYGEN__)
static int test_stream_putchar(int c) {

  streamPut(chtest.stream, (uint8_t)c);

  return c;
}
#endif

static void test_putchar(char c) {

  if (chtest.putchar != NULL) {
    chtest.putchar(c);
  }
}

static void test_clear_tokens(void) {

  chtest.tokp = chtest.tokens_buffer;
}

static void test_print_tokens(void) {
  char *cp = chtest.tokens_buffer;

  while (cp < chtest.tokp) {
    test_putchar(*cp++);
  }
}

static void test_execute_case(const testcase_t *tcp) {

  /* Initialization */
  test_clear_tokens();
  chtest.local_fail = false;

  if (tcp->setup != NULL) {
    tcp->setup();
  }
  tcp->execute();
  if (tcp->teardown != NULL) {
    tcp->teardown();
  }
}

static void test_print_string(const char *s) {
  char c;

  while ((c = *s) != '\0') {
    test_putchar(c);
    s++;
  }
}

static void test_print_line(void) {
  unsigned i;

  for (i = 0; i < 76; i++) {
    test_putchar('-');
  }
  test_print_string(TEST_CFG_EOL_STRING);
}

static void test_print_fat_line(void) {
  unsigned i;

  for (i = 0; i < 76; i++) {
    test_putchar('=');
  }
  test_print_string(TEST_CFG_EOL_STRING);
}

/**
 * @brief   Test execution.
 *
 * @param[in] tsp       test suite to execute
 * @return              A failure boolean.
 * @retval false        if no errors occurred.
 * @retval true         if one or more tests failed.
 */
static bool test_execute_inner(const testsuite_t *tsp) {
  int tseq, tcase;

  /* Test execution.*/
  test_println("");
  if (tsp->name != NULL) {
    test_print("*** ");
    test_println(tsp->name);
  }
  else {
    test_println("*** Test Suite");
  }
  test_println("***");
  test_print("*** Compiled:     ");
  test_println(__DATE__ " - " __TIME__);
#if defined(PLATFORM_NAME)
  test_print("*** Platform:     ");
  test_println(PLATFORM_NAME);
#endif
#if defined(BOARD_NAME)
  test_print("*** Test Board:   ");
  test_println(BOARD_NAME);
#endif
#if TEST_CFG_SIZE_REPORT == TRUE
  {
    extern uint8_t __text_base__,   __text_end__,
                   __rodata_base__, __rodata_end__,
                   __data_base__,   __data_end__,
                   __bss_base__,    __bss_end__;
    test_println("***");
    test_print("*** Text size:    ");
    test_printn((uint32_t)(&__text_end__ - &__text_base__));
    test_println(" bytes");
    test_print("*** RO data size: ");
    test_printn((uint32_t)(&__rodata_end__ - &__rodata_base__));
    test_println(" bytes");
    test_print("*** Data size:    ");
    test_printn((uint32_t)(&__data_end__ - &__data_base__));
    test_println(" bytes");
    test_print("*** BSS size:     ");
    test_printn((uint32_t)(&__bss_end__ - &__bss_base__));
    test_println(" bytes");
  }
#endif
#if defined(TEST_REPORT_HOOK_HEADER)
  TEST_REPORT_HOOK_HEADER
#endif
  test_println("");

  chtest.global_fail = false;
  tseq = 0;
  while (tsp->sequences[tseq] != NULL) {
#if TEST_CFG_SHOW_SEQUENCES == TRUE
    test_print_fat_line();
    test_print("=== Test Sequence ");
    test_printn(tseq + 1);
    test_print(" (");
    test_print(tsp->sequences[tseq]->name);
    test_println(")");
#endif
    tcase = 0;
    while (tsp->sequences[tseq]->cases[tcase] != NULL) {
      test_print_line();
      test_print("--- Test Case ");
      test_printn(tseq + 1);
      test_print(".");
      test_printn(tcase + 1);
      test_print(" (");
      test_print(tsp->sequences[tseq]->cases[tcase]->name);
      test_println(")");
#if TEST_CFG_DELAY_BETWEEN_TESTS > 0
      osalThreadSleepMilliseconds(TEST_CFG_DELAY_BETWEEN_TESTS);
#endif
      test_execute_case(tsp->sequences[tseq]->cases[tcase]);
      if (chtest.local_fail) {
        test_print("--- Result: FAILURE (#");
        test_printn(chtest.current_step);
        test_print(" [");
        test_print_tokens();
        test_print("] \"");
        test_print(chtest.failure_message);
        test_println("\")");
      }
      else {
        test_println("--- Result: SUCCESS");
      }
      tcase++;
    }
    tseq++;
  }
  test_print_line();
  test_println("");
  test_print("Final result: ");
  if (chtest.global_fail)
    test_println("FAILURE");
  else
    test_println("SUCCESS");

#if defined(TEST_REPORT_HOOK_END)
  TEST_REPORT_HOOK_END
#endif

  return chtest.global_fail;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

bool __test_fail(const char *msg) {

  chtest.local_fail      = true;
  chtest.global_fail     = true;
  chtest.failure_message = msg;
  return true;
}

bool __test_assert(bool condition, const char *msg) {

  if (!condition) {
    return __test_fail(msg);
  }
  return false;
}

bool __test_assert_sequence(char *expected, const char *msg) {
  char *cp = chtest.tokens_buffer;

  while (cp < chtest.tokp) {
    if (*cp++ != *expected++)
     return __test_fail(msg);
  }

  if (*expected) {
    return __test_fail(msg);
  }

  test_clear_tokens();

  return false;
}

#if (TEST_CFG_CHIBIOS_SUPPORT == TRUE) || defined(__DOXYGEN__)
bool __test_assert_time_window(systime_t start,
                               systime_t end,
                               const char *msg) {

  return __test_assert(osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end),
                       msg);
}
#endif /* TEST_CFG_CHIBIOS_SUPPORT == TRUE */

/**
 * @brief   Prints a decimal unsigned number.
 *
 * @param[in] n         the number to be printed
 *
 * @api
 */
void test_printn(uint32_t n) {
  char buf[16], *p;

  if (!n) {
    test_putchar('0');
  }
  else {
    p = buf;
    while (n) {
      *p++ = (n % 10) + '0', n /= 10;
    }
    while (p > buf) {
      test_putchar(*--p);
    }
  }
}

/**
 * @brief   Prints a line without final end-of-line.
 *
 * @param[in] msgp      the message
 *
 * @api
 */
void test_print(const char *msgp) {

  test_print_string(msgp);
}

/**
 * @brief   Prints a line.
 *
 * @param[in] msgp      the message
 *
 * @api
 */
void test_println(const char *msgp) {

  test_print_string(msgp);
  test_print_string(TEST_CFG_EOL_STRING);
}

/**
 * @brief   Emits a token into the tokens buffer from a critical zone.
 *
 * @param[in] token     the token as a char
 *
 * @api
 */
void test_emit_token(char token) {

  if (chtest.tokp < &chtest.tokens_buffer[TEST_CFG_MAX_TOKENS]) {
    *chtest.tokp++ = token;
  }
}

/**
 * @brief   Test execution with char output.
 *
 * @param[in] putfunc   character output function to be used or @p NULL for
 *                      silent operation
 * @param[in] tsp       test suite to execute
 * @return              A failure boolean.
 * @retval false        if no errors occurred.
 * @retval true         if one or more tests failed.
 *
 * @api
 */
bool test_execute_putchar(test_putchar_t putfunc,
                          const testsuite_t *tsp) {

  /* Output initialization using streams.*/
  chtest.stream  = NULL;
  chtest.putchar = putfunc;

  /* Test execution.*/
  return test_execute_inner(tsp);
}

#if (TEST_CFG_CHIBIOS_SUPPORT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Test execution with stream output.
 *
 * @param[in] stream    pointer to a @p BaseSequentialStream object for test
 *                      output
 * @param[in] tsp       test suite to execute
 * @return              A failure boolean.
 * @retval false        if no errors occurred.
 * @retval true         if one or more tests failed.
 *
 * @api
 */
bool test_execute_stream(BaseSequentialStream *stream,
                         const testsuite_t *tsp) {

  /* Output initialization using streams.*/
  chtest.stream  = stream;
  chtest.putchar = test_stream_putchar;

  /* Test execution.*/
  return test_execute_inner(tsp);
}
#endif /* TEST_CFG_CHIBIOS_SUPPORT == TRUE */

/** @} */
