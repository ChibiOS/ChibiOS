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
 * @page test_sequence_005 [5] Mutexes, Condition Variables and Priority Inheritance
 *
 * File: @ref test_sequence_005.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS/RT functionalities related to
 * mutexes, condition variables and priority inheritance algorithm.
 *
 * <h2>Conditions</h2>
 * This sequence is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_MUTEXES
 * .
 *
 * <h2>Test Cases</h2>
 * - @subpage test_005_001
 * - @subpage test_005_002
 * - @subpage test_005_003
 * - @subpage test_005_004
 * .
 */

#if (CH_CFG_USE_MUTEXES) || defined(__DOXYGEN__)

/****************************************************************************
 * Shared code.
 ****************************************************************************/

static MUTEX_DECL(m1);
static MUTEX_DECL(m2);
#if CH_CFG_USE_CONDVARS || defined(__DOXYGEN__)
static CONDVAR_DECL(c1);
#endif

#if CH_DBG_THREADS_PROFILING || defined(__DOXYGEN__)
/**
 * @brief   CPU pulse.
 * @note    The current implementation is not totally reliable.
 *
 * @param[in] duration      CPU pulse duration in milliseconds
 */
void test_cpu_pulse(unsigned duration) {
  systime_t start, end, now;

  start = chThdGetTicksX(chThdGetSelfX());
  end = start + MS2ST(duration);
  do {
    now = chThdGetTicksX(chThdGetSelfX());
#if defined(SIMULATOR)
    _sim_check_for_interrupts();
#endif
  }
  while (chVTIsTimeWithinX(now, start, end));
}
#endif /* CH_DBG_THREADS_PROFILING */

static THD_FUNCTION(thread1, p) {

  chMtxLock(&m1);
  test_emit_token(*(char *)p);
  chMtxUnlock(&m1);
}

/* Low priority thread */
static THD_FUNCTION(thread2L, p) {

  (void)p;
  chMtxLock(&m1);
  test_cpu_pulse(40);
  chMtxUnlock(&m1);
  test_cpu_pulse(10);
  test_emit_token('C');
}

/* Medium priority thread */
static THD_FUNCTION(thread2M, p) {

  (void)p;
  chThdSleepMilliseconds(20);
  test_cpu_pulse(40);
  test_emit_token('B');
}

/* High priority thread */
static THD_FUNCTION(thread2H, p) {

  (void)p;
  chThdSleepMilliseconds(40);
  chMtxLock(&m1);
  test_cpu_pulse(10);
  chMtxUnlock(&m1);
  test_emit_token('A');
}

/* Lowest priority thread */
static THD_FUNCTION(thread3LL, p) {

  (void)p;
  chMtxLock(&m1);
  test_cpu_pulse(30);
  chMtxUnlock(&m1);
  test_emit_token('E');
}

/* Low priority thread */
static THD_FUNCTION(thread3L, p) {

  (void)p;
  chThdSleepMilliseconds(10);
  chMtxLock(&m2);
  test_cpu_pulse(20);
  chMtxLock(&m1);
  test_cpu_pulse(10);
  chMtxUnlock(&m1);
  test_cpu_pulse(10);
  chMtxUnlock(&m2);
  test_emit_token('D');
}

/* Medium priority thread */
static THD_FUNCTION(thread3M, p) {

  (void)p;
  chThdSleepMilliseconds(20);
  chMtxLock(&m2);
  test_cpu_pulse(10);
  chMtxUnlock(&m2);
  test_emit_token('C');
}

/* High priority thread */
static THD_FUNCTION(thread3H, p) {

  (void)p;
  chThdSleepMilliseconds(40);
  test_cpu_pulse(20);
  test_emit_token('B');
}

/* Highest priority thread */
static THD_FUNCTION(thread3HH, p) {

  (void)p;
  chThdSleepMilliseconds(50);
  chMtxLock(&m2);
  test_cpu_pulse(10);
  chMtxUnlock(&m2);
  test_emit_token('A');
}

static THD_FUNCTION(thread4A, p) {

  (void)p;
  chThdSleepMilliseconds(50);
  chMtxLock(&m1);
  chMtxUnlock(&m1);
}

static THD_FUNCTION(thread4B, p) {

  (void)p;
  chThdSleepMilliseconds(150);
  chSysLock();
  chMtxLockS(&m2);   /* For coverage of the chMtxLockS() function variant.*/
  chMtxUnlockS(&m2); /* For coverage of the chMtxUnlockS() function variant.*/
  chSchRescheduleS();
  chSysUnlock();
}

#if CH_CFG_USE_CONDVARS || defined(__DOXYGEN__)
static THD_FUNCTION(thread6, p) {

  chMtxLock(&m1);
  chCondWait(&c1);
  test_emit_token(*(char *)p);
  chMtxUnlock(&m1);
}

static THD_FUNCTION(thread8, p) {

  chMtxLock(&m2);
  chMtxLock(&m1);
#if CH_CFG_USE_CONDVARS_TIMEOUT || defined(__DOXYGEN__)
  chCondWaitTimeout(&c1, TIME_INFINITE);
#else
  chCondWait(&c1);
#endif
  test_emit_token(*(char *)p);
  chMtxUnlock(&m1);
  chMtxUnlock(&m2);
}
#endif /* CH_CFG_USE_CONDVARS */

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_005_001 [5.1] Priority enqueuing test
 *
 * <h2>Description</h2>
 * Five threads, with increasing priority, are enqueued on a locked
 * mutex then the mutex is unlocked. The test expects the threads to
 * perform their operations in increasing priority order regardless of
 * the initial order.
 *
 * <h2>Test Steps</h2>
 * - [5.1.1] Getting the initial priority.
 * - [5.1.2] Locking the mutex.
 * - [5.1.3] Five threads are created that try to lock and unlock the
 *   mutex then terminate. The threads are created in ascending
 *   priority order.
 * - [5.1.4] Unlocking the mutex, the threads will wakeup in priority
 *   order because the mutext queue is an ordered one.
 * .
 */

static void test_005_001_setup(void) {
  chMtxObjectInit(&m1);
}

static void test_005_001_execute(void) {
  tprio_t prio;

  /* [5.1.1] Getting the initial priority.*/
  test_set_step(1);
  {
    prio = chThdGetPriorityX();
  }

  /* [5.1.2] Locking the mutex.*/
  test_set_step(2);
  {
    chMtxLock(&m1);
  }

  /* [5.1.3] Five threads are created that try to lock and unlock the
     mutex then terminate. The threads are created in ascending
     priority order.*/
  test_set_step(3);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, prio+1, thread1, "E");
    threads[1] = chThdCreateStatic(wa[1], WA_SIZE, prio+2, thread1, "D");
    threads[2] = chThdCreateStatic(wa[2], WA_SIZE, prio+3, thread1, "C");
    threads[3] = chThdCreateStatic(wa[3], WA_SIZE, prio+4, thread1, "B");
    threads[4] = chThdCreateStatic(wa[4], WA_SIZE, prio+5, thread1, "A");
  }

  /* [5.1.4] Unlocking the mutex, the threads will wakeup in priority
     order because the mutext queue is an ordered one.*/
  test_set_step(4);
  {
    chMtxUnlock(&m1);
    test_wait_threads();
    test_assert(prio == chThdGetPriorityX(), "wrong priority level");
    test_assert_sequence("ABCDE", "invalid sequence");
  }
}

static const testcase_t test_005_001 = {
  "Priority enqueuing test",
  test_005_001_setup,
  NULL,
  test_005_001_execute
};

/**
 * @page test_005_002 [5.2] Priority inheritance, simple case
 *
 * <h2>Description</h2>
 * Three threads are involved in the classic priority inversion
 * scenario, a medium priority thread tries to starve an high priority
 * thread by blocking a low priority thread into a mutex lock zone. The
 * test expects the threads to reach their goal in increasing priority
 * order by rearranging their priorities in order to avoid the priority
 * inversion trap.
 *
 * <h2>Test Steps</h2>
 * - [5.2.1] Getting the system time for test duration measurement.
 * - [5.2.2] The three contenders threads are created and let run
 *   atomically, the goals sequence is tested, the threads must
 *   complete in priority order.
 * - [5.2.3] Testing that all threads completed within the specified
 *   time windows (100mS...100mS+ALLOWED_DELAY).
 * .
 */

static void test_005_002_setup(void) {
  chMtxObjectInit(&m1);
}

static void test_005_002_execute(void) {
  systime_t time;

  /* [5.2.1] Getting the system time for test duration measurement.*/
  test_set_step(1);
  {
    time = test_wait_tick();
  }

  /* [5.2.2] The three contenders threads are created and let run
     atomically, the goals sequence is tested, the threads must
     complete in priority order.*/
  test_set_step(2);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()-1, thread2H, 0);
    threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriorityX()-2, thread2M, 0);
    threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriorityX()-3, thread2L, 0);
    test_wait_threads();
    test_assert_sequence("ABC", "invalid sequence");
  }

  /* [5.2.3] Testing that all threads completed within the specified
     time windows (100mS...100mS+ALLOWED_DELAY).*/
  test_set_step(3);
  {
    test_assert_time_window(time + MS2ST(100), time + MS2ST(100) + ALLOWED_DELAY,
                            "out of time window");
  }
}

static const testcase_t test_005_002 = {
  "Priority inheritance, simple case",
  test_005_002_setup,
  NULL,
  test_005_002_execute
};

/**
 * @page test_005_003 [5.3] Priority inheritance, complex case
 *
 * <h2>Description</h2>
 * Five threads are involved in the complex priority inversion
 * scenario, the priority inheritance algorithm is tested for depths
 * greater than one. The test expects the threads to perform their
 * operations in increasing priority order by rearranging their
 * priorities in order to avoid the priority inversion trap.
 *
 * <h2>Test Steps</h2>
 * - [5.3.1] Getting the system time for test duration measurement.
 * - [5.3.2] The five contenders threads are created and let run
 *   atomically, the goals sequence is tested, the threads must
 *   complete in priority order.
 * - [5.3.3] Testing that all threads completed within the specified
 *   time windows (110mS...110mS+ALLOWED_DELAY).
 * .
 */

static void test_005_003_setup(void) {
  chMtxObjectInit(&m1); /* Mutex B.*/
  chMtxObjectInit(&m2); /* Mutex A.*/
}

static void test_005_003_execute(void) {
  systime_t time;

  /* [5.3.1] Getting the system time for test duration measurement.*/
  test_set_step(1);
  {
    time = test_wait_tick();
  }

  /* [5.3.2] The five contenders threads are created and let run
     atomically, the goals sequence is tested, the threads must
     complete in priority order.*/
  test_set_step(2);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriorityX()-5, thread3LL, 0);
    threads[1] = chThdCreateStatic(wa[1], WA_SIZE, chThdGetPriorityX()-4, thread3L, 0);
    threads[2] = chThdCreateStatic(wa[2], WA_SIZE, chThdGetPriorityX()-3, thread3M, 0);
    threads[3] = chThdCreateStatic(wa[3], WA_SIZE, chThdGetPriorityX()-2, thread3H, 0);
    threads[4] = chThdCreateStatic(wa[4], WA_SIZE, chThdGetPriorityX()-1, thread3HH, 0);
    test_wait_threads();
    test_assert_sequence("ABCDE", "invalid sequence");
  }

  /* [5.3.3] Testing that all threads completed within the specified
     time windows (110mS...110mS+ALLOWED_DELAY).*/
  test_set_step(3);
  {
    test_assert_time_window(time + MS2ST(110), time + MS2ST(110) + ALLOWED_DELAY,
                            "out of time window");
  }
}

static const testcase_t test_005_003 = {
  "Priority inheritance, complex case",
  test_005_003_setup,
  NULL,
  test_005_003_execute
};

/**
 * @page test_005_004 [5.4] Priority return verification
 *
 * <h2>Description</h2>
 * Two threads are spawned that try to lock the mutexes already locked
 * by the tester thread with precise timing. The test expects that the
 * priority changes caused by the priority inheritance algorithm happen
 * at the right moment and with the right values.<br> Thread A performs
 * wait(50), lock(m1), unlock(m1), exit. Thread B performs wait(150),
 * lock(m2), unlock(m2), exit.
 *
 * <h2>Test Steps</h2>
 * - [5.4.1] Getting current thread priority P(0) and assigning to the
 *   threads A and B priorities +1 and +2.
 * - [5.4.2] Spawning threads A and B at priorities P(A) and P(B).
 * - [5.4.3] Locking the mutex M1 before thread A has a chance to lock
 *   it. The priority must not change because A has not yet reached
 *   chMtxLock(M1). the mutex is not locked.
 * - [5.4.4] Waiting 100mS, this makes thread A reach chMtxLock(M1) and
 *   get the mutex. This must boost the priority of the current thread
 *   at the same level of thread A.
 * - [5.4.5] Locking the mutex M2 before thread B has a chance to lock
 *   it. The priority must not change because B has not yet reached
 *   chMtxLock(M2). the mutex is not locked.
 * - [5.4.6] Waiting 100mS, this makes thread B reach chMtxLock(M2) and
 *   get the mutex. This must boost the priority of the current thread
 *   at the same level of thread B.
 * - [5.4.7] Unlocking M2, the priority should fall back to P(A).
 * - [5.4.8] Unlocking M1, the priority should fall back to P(0).
 * .
 */

static void test_005_004_setup(void) {
  chMtxObjectInit(&m1);
  chMtxObjectInit(&m2);
}

static void test_005_004_execute(void) {
  tprio_t p, pa, pb;

  /* [5.4.1] Getting current thread priority P(0) and assigning to the
     threads A and B priorities +1 and +2.*/
  test_set_step(1);
  {
    p = chThdGetPriorityX();
    pa = p + 1;
    pb = p + 2;
  }

  /* [5.4.2] Spawning threads A and B at priorities P(A) and P(B).*/
  test_set_step(2);
  {
    threads[0] = chThdCreateStatic(wa[0], WA_SIZE, pa, thread4A, "A");
    threads[1] = chThdCreateStatic(wa[1], WA_SIZE, pb, thread4B, "B");
  }

  /* [5.4.3] Locking the mutex M1 before thread A has a chance to lock
     it. The priority must not change because A has not yet reached
     chMtxLock(M1). the mutex is not locked.*/
  test_set_step(3);
  {
    chMtxLock(&m1);
    test_assert(chThdGetPriorityX() == p, "wrong priority level");
  }

  /* [5.4.4] Waiting 100mS, this makes thread A reach chMtxLock(M1) and
     get the mutex. This must boost the priority of the current thread
     at the same level of thread A.*/
  test_set_step(4);
  {
    chThdSleepMilliseconds(100);
    test_assert(chThdGetPriorityX() == pa, "wrong priority level");
  }

  /* [5.4.5] Locking the mutex M2 before thread B has a chance to lock
     it. The priority must not change because B has not yet reached
     chMtxLock(M2). the mutex is not locked.*/
  test_set_step(5);
  {
    chMtxLock(&m2);
    test_assert(chThdGetPriorityX() == pa, "wrong priority level");
  }

  /* [5.4.6] Waiting 100mS, this makes thread B reach chMtxLock(M2) and
     get the mutex. This must boost the priority of the current thread
     at the same level of thread B.*/
  test_set_step(6);
  {
    chThdSleepMilliseconds(100);
    test_assert(chThdGetPriorityX() == pb, "wrong priority level");
  }

  /* [5.4.7] Unlocking M2, the priority should fall back to P(A).*/
  test_set_step(7);
  {
    chMtxUnlock(&m2);
    test_assert(chThdGetPriorityX() == pa, "wrong priority level");
  }

  /* [5.4.8] Unlocking M1, the priority should fall back to P(0).*/
  test_set_step(8);
  {
    chMtxUnlock(&m1);
    test_assert(chThdGetPriorityX() == p, "wrong priority level");
  }
}

static const testcase_t test_005_004 = {
  "Priority return verification",
  test_005_004_setup,
  NULL,
  test_005_004_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Mutexes, Condition Variables and Priority Inheritance.
 */
const testcase_t * const test_sequence_005[] = {
  &test_005_001,
  &test_005_002,
  &test_005_003,
  &test_005_004,
  NULL
};

#endif /* CH_CFG_USE_MUTEXES */
