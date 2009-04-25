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

#ifndef _TEST_H_
#define _TEST_H_

#ifndef DELAY_BETWEEN_TESTS
#define DELAY_BETWEEN_TESTS     200
#endif

#ifndef TEST_NO_BENCHMARKS
#define TEST_NO_BENCHMARKS      FALSE
#endif

#define MAX_THREADS             5
#define MAX_TOKENS              16

#if defined(CH_ARCHITECTURE_AVR) || defined(CH_ARCHITECTURE_MSP430)
#define THREADS_STACK_SIZE      48
#elif defined(CH_ARCHITECTURE_WIN32SIM)
#define THREADS_STACK_SIZE      512
#else
#define THREADS_STACK_SIZE      128
#endif
#define WA_SIZE THD_WA_SIZE(THREADS_STACK_SIZE)

struct testcase {
  char *(*gettest)(void);
  void (*setup)(void);
  void (*teardown)(void);
  void (*execute)(void);
};

#ifdef __cplusplus
extern "C" {
#endif
  msg_t TestThread(void *p);
  void test_printn(uint32_t n);
  void test_print(char *msgp);
  void test_println(char *msgp);
  void test_emit_token(char token);
  bool_t _test_fail(char * msg);
  bool_t _test_assert(bool_t condition, char * msg);
  bool_t _test_assert_sequence(char *expected);
  bool_t _test_assert_time_window(systime_t start, systime_t end);
  void test_terminate_threads(void);
  void test_wait_threads(void);
  systime_t test_wait_tick(void);
  void test_cpu_pulse(unsigned ms);
  void test_start_timer(unsigned ms);
#if defined(WIN32)
  void ChkIntSources(void);
#endif
#ifdef __cplusplus
}
#endif

#define test_fail(msg) {                                                \
  _test_fail(msg);                                                      \
  return;                                                               \
}

#define test_assert(condition, msg) {                                   \
  if (_test_assert(condition, msg))                                     \
    return;                                                             \
}

#define test_assert_sequence(expected) {                                \
  if (_test_assert_sequence(expected))                                  \
    return;                                                             \
}

#define test_assert_time_window(start, end) {                           \
  if (_test_assert_time_window(start, end))                             \
    return;                                                             \
}

extern Thread *threads[MAX_THREADS];
extern void *wa[MAX_THREADS];
extern bool_t test_timer_done;

#endif /* _TEST_H_ */
