/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @page test_sequence_001 Threads Functionality
 *
 * File: @ref test_sequence_001.c
 *
 * <h2>Description</h2>
 * This sequence tests the NASA OSAL over ChibiOS/RT functionalities
 * related to threading.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_001_001
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include "osapi.H"

static void test_thread1(void) {

  test_emit_token('A');
}

static void test_thread2(void) {

  test_emit_token('B');
}

static void test_thread3(void) {

  test_emit_token('C');
}

static void test_thread4(void) {

  test_emit_token('D');
}

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_001_001 OS_TaskCreate() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_TaskCreate() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_TaskCreate() is invoked with task_id set to NULL, an error is
 *   expected.
 * - OS_TaskCreate() is invoked with task_name set to NULL, an error is
 *   expected.
 * - OS_TaskCreate() is invoked with stack_pointer set to NULL, an
 *   error is expected.
 * - OS_TaskCreate() is invoked with a very long task name, an error is
 *   expected.
 * - OS_TaskCreate() is invoked with priority below and above allowed
 *   range, an error is expected.
 * - OS_TaskCreate() is invoked with a stack size below minimum, an
 *   error is expected.
 * - OS_TaskCreate() is invoked twice with duplicated name and then
 *   duplicated stack, an error is expected in both cases.
 * .
 */

static void test_001_001_execute(void) {

  /* OS_TaskCreate() is invoked with task_id set to NULL, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_TaskCreate(NULL,                   /* Error.*/
                        "failing thread",
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
    test_assert_sequence("", "task executed");
  }

  /* OS_TaskCreate() is invoked with task_name set to NULL, an error is
     expected.*/
  test_set_step(2);
  {
    int32 err;
    uint32 tid;

    err = OS_TaskCreate(&tid,
                        NULL,                   /* Error.*/
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
    test_assert_sequence("", "task executed");
  }

  /* OS_TaskCreate() is invoked with stack_pointer set to NULL, an
     error is expected.*/
  test_set_step(3);
  {
    int32 err;
    uint32 tid;

    err = OS_TaskCreate(&tid,
                        "failing thread",
                        test_thread1,
                        (uint32 *)NULL,         /* Error.*/
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
    test_assert_sequence("", "task executed");
  }

  /* OS_TaskCreate() is invoked with a very long task name, an error is
     expected.*/
  test_set_step(4);
  {
    int32 err;
    uint32 tid;

    err = OS_TaskCreate(&tid,
                        "this is a very very long task name", /* Error.*/
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
    test_assert_sequence("", "task executed");
  }

  /* OS_TaskCreate() is invoked with priority below and above allowed
     range, an error is expected.*/
  test_set_step(5);
  {
    int32 err;
    uint32 tid;

    err = OS_TaskCreate(&tid,
                        "failing thread",
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        0,                      /* Error.*/
                        0);
    test_assert(err == OS_ERR_INVALID_PRIORITY, "priority error not detected");
    test_assert_sequence("", "task executed");

    err = OS_TaskCreate(&tid,
                       "failing thread",
                       test_thread1,
                       (uint32 *)wa_test1,
                       sizeof wa_test1,
                       256,                     /* Error.*/
                       0);
    test_assert(err == OS_ERR_INVALID_PRIORITY, "priority error not detected");
    test_assert_sequence("", "task executed");
  }

  /* OS_TaskCreate() is invoked with a stack size below minimum, an
     error is expected.*/
  test_set_step(6);
  {
    int32 err;
    uint32 tid;

    err = OS_TaskCreate(&tid,
                        "failing thread",
                        test_thread1,
                        (uint32 *)wa_test1,
                        16,                     /* Error.*/
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_INVALID_INT_NUM, "stack insufficient size not detected");
    test_assert_sequence("", "task executed");
  }

  /* OS_TaskCreate() is invoked twice with duplicated name and then
     duplicated stack, an error is expected in both cases.*/
  test_set_step(7);
  {
    int32 err;
    uint32 tid;

    err = OS_TaskCreate(&tid,
                        "running thread",
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_SUCCESS, "task creation failed");

    err = OS_TaskCreate(&tid,
                        "running thread",
                        test_thread2,
                        (uint32 *)wa_test2,
                        sizeof wa_test2,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_ERR_NAME_TAKEN, "name conflict not detected");

    err = OS_TaskCreate(&tid,
                        "another running thread",
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_ERR_NO_FREE_IDS, "stack conflict not detected");

    OS_TaskDelay(5);
    test_assert_sequence("A", "task not executed");

    err = OS_TaskCreate(&tid,
                        "running thread",
                        test_thread1,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_SUCCESS, "task creation failed");

    OS_TaskDelay(5);
    test_assert_sequence("A", "task not executed");
  }
}

static const testcase_t test_001_001 = {
  "OS_TaskCreate() errors",
  NULL,
  NULL,
  test_001_001_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Threads Functionality.
 */
const testcase_t * const test_sequence_001[] = {
  &test_001_001,
  NULL
};
