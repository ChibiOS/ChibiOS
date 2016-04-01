/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/**
 * @page test_sequence_012 [12] Benchmarks
 *
 * File: @ref test_sequence_012.c
 *
 * <h2>Description</h2>
 * This module implements a series of system benchmarks. The benchmarks
 * are useful as a stress test and as a reference when comparing
 * ChibiOS/RT with similar systems.<br> Objective of the test sequence
 * is to provide a performance index for the most critical system
 * subsystems. The performance numbers allow to discover performance
 * regressions between successive ChibiOS/RT releases.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_012_001
 * - @subpage test_012_002
 * - @subpage test_012_003
 * - @subpage test_012_004
 * - @subpage test_012_005
 * - @subpage test_012_006
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#if CH_CFG_USE_SEMAPHORES || defined(__DOXYGEN__)
static semaphore_t sem1;
#endif
#if CH_CFG_USE_MUTEXES || defined(__DOXYGEN__)
static mutex_t mtx1;
#endif

#if CH_CFG_USE_MESSAGES
static THD_FUNCTION(bmk_thread1, p) {
  thread_t *tp;
  msg_t msg;

  (void)p;
  do {
    tp = chMsgWait();
    msg = chMsgGet(tp);
    chMsgRelease(tp, msg);
  } while (msg);
}

NOINLINE static unsigned int msg_loop_test(thread_t *tp) {
  systime_t start, end;
  
  uint32_t n = 0;
  start = test_wait_tick();
  end = start + MS2ST(1000);
  do {
    (void)chMsgSend(tp, 1);
    n++;
#if defined(SIMULATOR)
    _sim_check_for_interrupts();
#endif
  } while (chVTIsSystemTimeWithinX(start, end));
  (void)chMsgSend(tp, 0);
  return n;
}
#endif

static THD_FUNCTION(bmk_thread3, p) {

  chThdExit((msg_t)p);
}

static THD_FUNCTION(bmk_thread4, p) {
  msg_t msg;
  thread_t *self = chThdGetSelfX();

  (void)p;
  chSysLock();
  do {
    chSchGoSleepS(CH_STATE_SUSPENDED);
    msg = self->u.rdymsg;
  } while (msg == MSG_OK);
  chSysUnlock();
}

/****************************************************************************
 * Test cases.
 ****************************************************************************/

#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
/**
 * @page test_012_001 [12.1] Messages performance #1
 *
 * <h2>Description</h2>
 * A message server thread is created with a lower priority than the
 * client thread, the messages throughput per second is measured and
 * the result printed on the output log.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MESSAGES
 * .
 *
 * <h2>Test Steps</h2>
 * - [12.1.1] The messenger thread is started at a lower priority than
 *   the current thread.
 * - [12.1.2] The number of messages exchanged is counted in a one
 *   second time window.
 * - [12.1.3] Score is printed.
 * .
 */

static void test_012_001_execute(void) {
  uint32_t n;

  /* [12.1.1] The messenger thread is started at a lower priority than
     the current thread.*/
  test_set_step(1);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()-1, bmk_thread1, NULL);
  }

  /* [12.1.2] The number of messages exchanged is counted in a one
     second time window.*/
  test_set_step(2);
  {
    n = msg_loop_test(threads[0]);
    test_wait_threads();
  }

  /* [12.1.3] Score is printed.*/
  test_set_step(3);
  {
    test_print("--- Score : ");
    test_printn(n);
    test_print(" msgs/S, ");
    test_printn(n << 1);
    test_println(" ctxswc/S");
  }
}

static const testcase_t test_012_001 = {
  "Messages performance #1",
  NULL,
  NULL,
  test_012_001_execute
};
#endif /* CH_CFG_USE_MESSAGES */

#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
/**
 * @page test_012_002 [12.2] Messages performance #2
 *
 * <h2>Description</h2>
 * A message server thread is created with an higher priority than the
 * client thread, the messages throughput per second is measured and
 * the result printed on the output log.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MESSAGES
 * .
 *
 * <h2>Test Steps</h2>
 * - [12.2.1] The messenger thread is started at an higher priority
 *   than the current thread.
 * - [12.2.2] The number of messages exchanged is counted in a one
 *   second time window.
 * - [12.2.3] Score is printed.
 * .
 */

static void test_012_002_execute(void) {
  uint32_t n;

  /* [12.2.1] The messenger thread is started at an higher priority
     than the current thread.*/
  test_set_step(1);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()+1, bmk_thread1, NULL);
  }

  /* [12.2.2] The number of messages exchanged is counted in a one
     second time window.*/
  test_set_step(2);
  {
    n = msg_loop_test(threads[0]);
    test_wait_threads();
  }

  /* [12.2.3] Score is printed.*/
  test_set_step(3);
  {
    test_print("--- Score : ");
    test_printn(n);
    test_print(" msgs/S, ");
    test_printn(n << 1);
    test_println(" ctxswc/S");
  }
}

static const testcase_t test_012_002 = {
  "Messages performance #2",
  NULL,
  NULL,
  test_012_002_execute
};
#endif /* CH_CFG_USE_MESSAGES */

#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
/**
 * @page test_012_003 [12.3] Messages performance #3
 *
 * <h2>Description</h2>
 * A message server thread is created with an higher priority than the
 * client thread, four lower priority threads crowd the ready list, the
 * messages throughput per second is measured while the ready list and
 * the result printed on the output log.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MESSAGES
 * .
 *
 * <h2>Test Steps</h2>
 * - [12.3.1] The messenger thread is started at an higher priority
 *   than the current thread.
 * - [12.3.2] Four threads are started at a lower priority than the
 *   current thread.
 * - [12.3.3] The number of messages exchanged is counted in a one
 *   second time window.
 * - [12.3.4] Score is printed.
 * .
 */

static void test_012_003_execute(void) {
  uint32_t n;

  /* [12.3.1] The messenger thread is started at an higher priority
     than the current thread.*/
  test_set_step(1);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()+1, bmk_thread1, NULL);
  }

  /* [12.3.2] Four threads are started at a lower priority than the
     current thread.*/
  test_set_step(2);
  {
    threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriorityX()-2, bmk_thread3, NULL);
    threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriorityX()-3, bmk_thread3, NULL);
    threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriorityX()-4, bmk_thread3, NULL);
    threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriorityX()-5, bmk_thread3, NULL);
  }

  /* [12.3.3] The number of messages exchanged is counted in a one
     second time window.*/
  test_set_step(3);
  {
    n = msg_loop_test(threads[0]);
    test_wait_threads();
  }

  /* [12.3.4] Score is printed.*/
  test_set_step(4);
  {
    test_print("--- Score : ");
    test_printn(n);
    test_print(" msgs/S, ");
    test_printn(n << 1);
    test_println(" ctxswc/S");
  }
}

static const testcase_t test_012_003 = {
  "Messages performance #3",
  NULL,
  NULL,
  test_012_003_execute
};
#endif /* CH_CFG_USE_MESSAGES */

/**
 * @page test_012_004 [12.4] Context Switch performance
 *
 * <h2>Description</h2>
 * A thread is created that just performs a @p chSchGoSleepS() into a
 * loop, the thread is awakened as fast is possible by the tester
 * thread.<br> The Context Switch performance is calculated by
 * measuring the number of iterations after a second of continuous
 * operations.
 *
 * <h2>Test Steps</h2>
 * - [12.4.1] Starting the target thread at an higher priority level.
 * - [12.4.2] Waking up the thread as fast as possible in a one second
 *   time window.
 * - [12.4.3] Stopping the target thread.
 * - [12.4.4] Score is printed.
 * .
 */

static void test_012_004_execute(void) {
  thread_t *tp;
  uint32_t n;

  /* [12.4.1] Starting the target thread at an higher priority level.*/
  test_set_step(1);
  {
    tp = threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()+1,
                                        bmk_thread4, NULL);
  }

  /* [12.4.2] Waking up the thread as fast as possible in a one second
     time window.*/
  test_set_step(2);
  {
    systime_t start, end;

    n = 0;
    start = test_wait_tick();
    end = start + MS2ST(1000);
    do {
      chSysLock();
      chSchWakeupS(tp, MSG_OK);
      chSchWakeupS(tp, MSG_OK);
      chSchWakeupS(tp, MSG_OK);
      chSchWakeupS(tp, MSG_OK);
      chSysUnlock();
      n += 4;
    #if defined(SIMULATOR)
      _sim_check_for_interrupts();
    #endif
    } while (chVTIsSystemTimeWithinX(start, end));
  }

  /* [12.4.3] Stopping the target thread.*/
  test_set_step(3);
  {
    chSysLock();
    chSchWakeupS(tp, MSG_TIMEOUT);
    chSysUnlock();
    test_wait_threads();
  }

  /* [12.4.4] Score is printed.*/
  test_set_step(4);
  {
    test_print("--- Score : ");
    test_printn(n * 2);
    test_println(" ctxswc/S");
  }
}

static const testcase_t test_012_004 = {
  "Context Switch performance",
  NULL,
  NULL,
  test_012_004_execute
};

/**
 * @page test_012_005 [12.5] Threads performance, full cycle
 *
 * <h2>Description</h2>
 * Threads are continuously created and terminated into a loop. A full
 * chThdCreateStatic() / @p chThdExit() / @p chThdWait() cycle is
 * performed in each iteration.<br> The performance is calculated by
 * measuring the number of iterations after a second of continuous
 * operations.
 *
 * <h2>Test Steps</h2>
 * - [12.5.1] A thread is created at a lower priority level and its
 *   termination detected using @p chThdWait(). The operation is
 *   repeated continuously in a one-second time window.
 * - [12.5.2] Score is printed.
 * .
 */

static void test_012_005_execute(void) {
  uint32_t n;
  tprio_t prio = chThdGetPriorityX() - 1;
  systime_t start, end;

  /* [12.5.1] A thread is created at a lower priority level and its
     termination detected using @p chThdWait(). The operation is
     repeated continuously in a one-second time window.*/
  test_set_step(1);
  {
    n = 0;
    start = test_wait_tick();
    end = start + MS2ST(1000);
    do {
      chThdWait(chThdCreateStatic(wa[0], WA_SIZE, prio, bmk_thread3, NULL));
      n++;
    #if defined(SIMULATOR)
      _sim_check_for_interrupts();
    #endif
    } while (chVTIsSystemTimeWithinX(start, end));
  }

  /* [12.5.2] Score is printed.*/
  test_set_step(2);
  {
    test_print("--- Score : ");
    test_printn(n);
    test_println(" threads/S");
  }
}

static const testcase_t test_012_005 = {
  "Threads performance, full cycle",
  NULL,
  NULL,
  test_012_005_execute
};

/**
 * @page test_012_006 [12.6] Threads performance, create/exit only
 *
 * <h2>Description</h2>
 * Threads are continuously created and terminated into a loop. A
 * partial @p chThdCreateStatic() / @p chThdExit() cycle is performed
 * in each iteration, the @p chThdWait() is not necessary because the
 * thread is created at an higher priority so there is no need to wait
 * for it to terminate.<br> The performance is calculated by measuring
 * the number of iterations after a second of continuous operations.
 *
 * <h2>Test Steps</h2>
 * - [12.6.1] A thread is created at an higher priority level and let
 *   terminate immediately. The operation is repeated continuously in a
 *   one-second time window.
 * - [12.6.2] Score is printed.
 * .
 */

static void test_012_006_execute(void) {
  uint32_t n;
  tprio_t prio = chThdGetPriorityX() + 1;
  systime_t start, end;

  /* [12.6.1] A thread is created at an higher priority level and let
     terminate immediately. The operation is repeated continuously in a
     one-second time window.*/
  test_set_step(1);
  {
    n = 0;
    start = test_wait_tick();
    end = start + MS2ST(1000);
    do {
      chThdRelease(chThdCreateStatic(wa[0], WA_SIZE, prio, bmk_thread3, NULL));
      n++;
    #if defined(SIMULATOR)
      _sim_check_for_interrupts();
    #endif
    } while (chVTIsSystemTimeWithinX(start, end));
  }

  /* [12.6.2] Score is printed.*/
  test_set_step(2);
  {
    test_print("--- Score : ");
    test_printn(n);
    test_println(" threads/S");
  }
}

static const testcase_t test_012_006 = {
  "Threads performance, create/exit only",
  NULL,
  NULL,
  test_012_006_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Benchmarks.
 */
const testcase_t * const test_sequence_012[] = {
#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
  &test_012_001,
#endif
#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
  &test_012_002,
#endif
#if (CH_CFG_USE_MESSAGES) || defined(__DOXYGEN__)
  &test_012_003,
#endif
  &test_012_004,
  &test_012_005,
  &test_012_006,
  NULL
};
