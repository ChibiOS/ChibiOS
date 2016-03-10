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
 * @page test_sequence_002 Queues Functionality
 *
 * File: @ref test_sequence_002.c
 *
 * <h2>Description</h2>
 * This sequence tests the NASA OSAL over ChibiOS/RT functionalities
 * related to queues.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_002_001
 * - @subpage test_002_002
 * - @subpage test_002_003
 * - @subpage test_002_004
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include <string.h>

#include "osapi.h"

uint32 qid, tid;

#define WRITER_NUM_MESSAGES 16
#define MESSAGE_SIZE        20

static void test_task_writer(void) {
  unsigned i;
  int32 err;

  for (i = 0; i < WRITER_NUM_MESSAGES; i++) {
    err = OS_QueuePut(qid, "Hello World", 12, 0);
    if (err != OS_SUCCESS) {
      test_emit_token('*');
    }
  }
}

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_002_001 OS_QueueCreate() and OS_QueueDelete() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_QueueCreate() and OS_QueueDelete() is
 * tested.
 *
 * <h2>Test Steps</h2>
 * - OS_QueueCreate() is invoked with queue_id set to NULL, an error is
 *   expected.
 * - OS_QueueCreate() is invoked with task_name set to NULL, an error
 *   is expected.
 * - OS_QueueCreate() is invoked with a very long task name, an error
 *   is expected.
 * - OS_QueueDelete() is invoked with queue_id set to -1, an error is
 *   expected.
 * - OS_QueueCreate() is invoked twice with duplicated name, an error
 *   is expected, then the queue is deleted using OS_DeleteQueue().
 * .
 */

static void test_002_001_execute(void) {

  /* OS_QueueCreate() is invoked with queue_id set to NULL, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_QueueCreate(NULL,                      /* Error.*/
                         "failing queue",
                         4,
                         128,
                         0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_QueueCreate() is invoked with task_name set to NULL, an error
     is expected.*/
  test_set_step(2);
  {
    int32 err;
    uint32 qid;

    err = OS_QueueCreate(&qid,
                         NULL,                      /* Error.*/
                         4,
                         128,
                         0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_QueueCreate() is invoked with a very long task name, an error
     is expected.*/
  test_set_step(3);
  {
    int32 err;
    uint32 qid;

    err = OS_QueueCreate(&qid,
                         "very very long queue name",   /* Error.*/
                         4,
                         128,
                         0);
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }

  /* OS_QueueDelete() is invoked with queue_id set to -1, an error is
     expected.*/
  test_set_step(4);
  {
    int32 err;

    err = OS_QueueDelete((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "wrong queue id not detected");
  }

  /* OS_QueueCreate() is invoked twice with duplicated name, an error
     is expected, then the queue is deleted using OS_DeleteQueue().*/
  test_set_step(5);
  {
    int32 err;
    uint32 qid1, qid2;

    err = OS_QueueCreate(&qid1, "my queue", 4, 128, 0);
    test_assert(err == OS_SUCCESS, "queue creation failed");

    err = OS_QueueCreate(&qid2, "my queue", 4, 128, 0);
    test_assert(err == OS_ERR_NAME_TAKEN, "name conflict not detected");

    err = OS_QueueDelete(qid1);
    test_assert(err == OS_SUCCESS, "queue deletion failed");
  }
}

static const testcase_t test_002_001 = {
  "OS_QueueCreate() and OS_QueueDelete() errors",
  NULL,
  NULL,
  test_002_001_execute
};

/**
 * @page test_002_002 OS_QueuePut() and OS_QueueGet() functionality
 *
 * <h2>Description</h2>
 * A task writes on a queue, the messages are retrieved on the other
 * side in blocking mode.
 *
 * <h2>Test Steps</h2>
 * - Creataing a queue with depth 4 and message size 20.
 * - Creating the writer task.
 * - Reading messages from the writer task.
 * - Waiting for task termination then checking for errors.
 * .
 */

static void test_002_002_setup(void) {
  qid = 0;
  tid = 0;
}

static void test_002_002_teardown(void) {
  if (qid != 0) {
    (void) OS_QueueDelete(qid);
  }

  if (tid != 0) {
    (void) OS_TaskWait(tid);
  }
}

static void test_002_002_execute(void) {
  uint32 tid;
  unsigned i;

  /* Creataing a queue with depth 4 and message size 20.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_QueueCreate(&qid, "test queue", 4, MESSAGE_SIZE, 0);
    test_assert(err == OS_SUCCESS, "queue creation failed");
  }

  /* Creating the writer task.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_TaskCreate(&tid,
                        "writer task",
                        test_task_writer,
                        (uint32 *)wa_test1,
                        sizeof wa_test1,
                        TASKS_BASE_PRIORITY,
                        0);
    test_assert(err == OS_SUCCESS, "writer task creation failed");
  }

  /* Reading messages from the writer task.*/
  test_set_step(3);
  {
    for (i = 0; i < WRITER_NUM_MESSAGES; i++) {
      int32 err;
      char data[MESSAGE_SIZE];
      uint32 copied;

      err = OS_QueueGet(qid, data, MESSAGE_SIZE, &copied, OS_Milli2Ticks(200));
      test_assert(err == OS_SUCCESS, "timed out");
      test_assert(strncmp(data, "Hello World", sizeof (data)) == 0,
                  "wrong message");
    }
  }

  /* Waiting for task termination then checking for errors.*/
  test_set_step(4);
  {
    (void) OS_TaskWait(tid);
    tid = 0;
    test_assert_sequence("", "queue write errors occurred");
  }
}

static const testcase_t test_002_002 = {
  "OS_QueuePut() and OS_QueueGet() functionality",
  test_002_002_setup,
  test_002_002_teardown,
  test_002_002_execute
};

/**
 * @page test_002_003 OS_QueueGetIdByName() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_QueueGetIdByName() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_QueueGetIdByName() is invoked with queue_id set to NULL, an
 *   error is expected.
 * - OS_QueueGetIdByName() is invoked with queue_name set to NULL, an
 *   error is expected.
 * - OS_QueueGetIdByName() is invoked with a very long task name, an
 *   error is expected.
 * .
 */

static void test_002_003_execute(void) {

  /* OS_QueueGetIdByName() is invoked with queue_id set to NULL, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_QueueGetIdByName(NULL, "queue");
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_QueueGetIdByName() is invoked with queue_name set to NULL, an
     error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_QueueGetIdByName(&qid, NULL);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_QueueGetIdByName() is invoked with a very long task name, an
     error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_QueueGetIdByName(&qid, "very very long queue name");
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }
}

static const testcase_t test_002_003 = {
  "OS_QueueGetIdByName() errors",
  NULL,
  NULL,
  test_002_003_execute
};

/**
 * @page test_002_004 OS_QueueGet() with timeout
 *
 * <h2>Description</h2>
 * OS_QueueGetIdByName is tested.
 *
 * <h2>Test Steps</h2>
 * - Retrieving the queue name.
 * - Get operation with a one second timeout, an error is expected.
 * - Get operation in non-blocking mode, an error is expected.
 * .
 */

static void test_002_004_setup(void) {
  qid = 0;
  (void) OS_QueueCreate(&qid, "test queue", 2, MESSAGE_SIZE, 0);
}

static void test_002_004_teardown(void) {
  if (qid != 0) {
    OS_QueueDelete(qid);
  }
}

static void test_002_004_execute(void) {
  uint32 local_qid;
  uint32 copied;
  char data[MESSAGE_SIZE];

  /* Retrieving the queue name.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_QueueGetIdByName(&local_qid, "test queue");
    test_assert(err == OS_SUCCESS, "queue not found");
  }

  /* Get operation with a one second timeout, an error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_QueueGet(qid, data, MESSAGE_SIZE, &copied, OS_Milli2Ticks(1000));
    test_assert(err == OS_QUEUE_TIMEOUT, "unexpected error code");
  }

  /* Get operation in non-blocking mode, an error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_QueueGet(qid, data, MESSAGE_SIZE, &copied, OS_CHECK);
    test_assert(err == OS_QUEUE_EMPTY, "unexpected error code");
  }
}

static const testcase_t test_002_004 = {
  "OS_QueueGet() with timeout",
  test_002_004_setup,
  test_002_004_teardown,
  test_002_004_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Queues Functionality.
 */
const testcase_t * const test_sequence_002[] = {
  &test_002_001,
  &test_002_002,
  &test_002_003,
  &test_002_004,
  NULL
};
