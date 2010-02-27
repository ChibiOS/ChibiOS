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

#if defined(CH_ARCHITECTURE_AVR) ||                                         \
    defined(CH_ARCHITECTURE_MSP430) ||                                      \
    defined(CH_ARCHITECTURE_STM8)
#define THREADS_STACK_SIZE      48
#elif defined(CH_ARCHITECTURE_SIMIA32)
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

#ifndef __DOXYGEN__
union test_buffers {
  struct {
    WORKING_AREA(T0, THREADS_STACK_SIZE);
    WORKING_AREA(T1, THREADS_STACK_SIZE);
    WORKING_AREA(T2, THREADS_STACK_SIZE);
    WORKING_AREA(T3, THREADS_STACK_SIZE);
    WORKING_AREA(T4, THREADS_STACK_SIZE);
  } wa;
  uint8_t buffer[WA_SIZE * 5];
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
  msg_t TestThread(void *p);
  void test_printn(uint32_t n);
  void test_print(char *msgp);
  void test_println(char *msgp);
  void test_emit_token(char token);
  bool_t _test_fail(unsigned point);
  bool_t _test_assert(unsigned point, bool_t condition);
  bool_t _test_assert_sequence(unsigned point, char *expected);
  bool_t _test_assert_time_window(unsigned point, systime_t start, systime_t end);
  void test_terminate_threads(void);
  void test_wait_threads(void);
  systime_t test_wait_tick(void);
  void test_start_timer(unsigned ms);
#if CH_DBG_THREADS_PROFILING
  void test_cpu_pulse(unsigned duration);
#endif
#if defined(WIN32)
  void ChkIntSources(void);
#endif
#ifdef __cplusplus
}
#endif

#define test_fail(point) {                                              \
  _test_fail(point);                                                    \
  return;                                                               \
}

#define test_assert(point, condition, msg) {                            \
  if (_test_assert(point, condition))                                   \
    return;                                                             \
}

#define test_assert_sequence(point, expected) {                         \
  if (_test_assert_sequence(point, expected))                           \
    return;                                                             \
}

#define test_assert_time_window(point, start, end) {                    \
  if (_test_assert_time_window(point, start, end))                      \
    return;                                                             \
}

extern Thread *threads[MAX_THREADS];
extern union test_buffers test;
extern void * const wa[];
extern bool_t test_timer_done;

#endif /* _TEST_H_ */
