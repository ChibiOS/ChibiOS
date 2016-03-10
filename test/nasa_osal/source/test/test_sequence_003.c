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
 * @page test_sequence_003 Timers Functionality
 *
 * File: @ref test_sequence_003.c
 *
 * <h2>Description</h2>
 * This sequence tests the NASA OSAL over ChibiOS/RT functionalities
 * related to timers.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_003_001
 * - @subpage test_003_002
 * - @subpage test_003_003
 * - @subpage test_003_004
 * - @subpage test_003_005
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include <string.h>

#include "osapi.h"

uint32 tmid;

static void tmr_callback(uint32 timer_id) {

  (void)timer_id;
}

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_003_001 OS_TimerCreate() and OS_TimerDelete() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_TimerCreate() and OS_TimerDelete() is
 * tested.
 *
 * <h2>Test Steps</h2>
 * - OS_TimerCreate() is invoked with timer_id set to NULL, an error is
 *   expected.
 * - OS_TimerCreate() is invoked with timer_name set to NULL, an error
 *   is expected.
 * - OS_TimerCreate() is invoked with accuracy set to NULL, an error is
 *   expected.
 * - OS_TimerCreate() is invoked with callback_ptr set to NULL, an
 *   error is expected.
 * - OS_TimerCreate() is invoked with a very long timer name, an error
 *   is expected.
 * - OS_TimerDelete() is invoked with timer_id set to -1, an error is
 *   expected.
 * - OS_TimerCreate() is invoked twice with duplicated name, an error
 *   is expected, then the queue is deleted using OS_TimerDelete().
 * .
 */

static void test_003_001_execute(void) {

  /* OS_TimerCreate() is invoked with timer_id set to NULL, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;
    uint32 accuracy;

    err = OS_TimerCreate(NULL,                      /* Error.*/
                         "failing timer",
                         &accuracy,
                         tmr_callback);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_TimerCreate() is invoked with timer_name set to NULL, an error
     is expected.*/
  test_set_step(2);
  {
    int32 err;
    uint32 tmid;
    uint32 accuracy;

    err = OS_TimerCreate(&tmid,
                         NULL,                      /* Error.*/
                         &accuracy,
                         tmr_callback);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_TimerCreate() is invoked with accuracy set to NULL, an error is
     expected.*/
  test_set_step(3);
  {
    int32 err;
    uint32 tmid;

    err = OS_TimerCreate(&tmid,
                         "failing timer",
                         NULL,                      /* Error.*/
                         tmr_callback);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_TimerCreate() is invoked with callback_ptr set to NULL, an
     error is expected.*/
  test_set_step(4);
  {
    int32 err;
    uint32 tmid;
    uint32 accuracy;

    err = OS_TimerCreate(&tmid,
                         "failing timer",
                         &accuracy,
                         NULL);                     /* Error.*/
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_TimerCreate() is invoked with a very long timer name, an error
     is expected.*/
  test_set_step(5);
  {
    int32 err;
    uint32 tmid;
    uint32 accuracy;

    err = OS_TimerCreate(&tmid,
                         "very very long timer name",   /* Error.*/
                         &accuracy,
                         tmr_callback);
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }

  /* OS_TimerDelete() is invoked with timer_id set to -1, an error is
     expected.*/
  test_set_step(6);
  {
    int32 err;

    err = OS_TimerDelete((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "wrong timer id not detected");
  }

  /* OS_TimerCreate() is invoked twice with duplicated name, an error
     is expected, then the queue is deleted using OS_TimerDelete().*/
  test_set_step(7);
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

static const testcase_t test_003_001 = {
  "OS_TimerCreate() and OS_TimerDelete() errors",
  NULL,
  NULL,
  test_003_001_execute
};

/**
 * @page test_003_002 OS_TimerSet() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_TimerSet() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_TimerSet() is invoked with timer_id set to -1, an error is
 *   expected.
 * .
 */

static void test_003_002_execute(void) {

  /* OS_TimerSet() is invoked with timer_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_TimerSet((uint32)-1, 10, 10);
    test_assert(err == OS_ERR_INVALID_ID, "invalid timer_id not detected");
  }
}

static const testcase_t test_003_002 = {
  "OS_TimerSet() errors",
  NULL,
  NULL,
  test_003_002_execute
};

/**
 * @page test_003_003 OS_TimerGetIdByName() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_TimerGetIdByName() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_TimerGetIdByName() is invoked with timer_id set to NULL, an
 *   error is expected.
 * - OS_TimerGetIdByName() is invoked with timer name set to NULL, an
 *   error is expected.
 * - OS_TimerGetIdByName() is invoked with a very long task name, an
 *   error is expected.
 * .
 */

static void test_003_003_execute(void) {

  /* OS_TimerGetIdByName() is invoked with timer_id set to NULL, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_TimerGetIdByName(NULL, "timer");
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_TimerGetIdByName() is invoked with timer name set to NULL, an
     error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_TimerGetIdByName(&tmid, NULL);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_TimerGetIdByName() is invoked with a very long task name, an
     error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_TimerGetIdByName(&tmid, "very very long timer name");
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }
}

static const testcase_t test_003_003 = {
  "OS_TimerGetIdByName() errors",
  NULL,
  NULL,
  test_003_003_execute
};

/**
 * @page test_003_004 OS_TimerSet() one-shot functionality
 *
 * <h2>Description</h2>
 * A timer is tested in one-shot mode.
 *
 * <h2>Test Steps</h2>
 */

static void test_003_004_setup(void) {
  tmid = 0;
}

static void test_003_004_teardown(void) {
  if (tmid != 0) {
    (void) OS_TimerDelete(tmid);
  }
}

static void test_003_004_execute(void) {
}

static const testcase_t test_003_004 = {
  "OS_TimerSet() one-shot functionality",
  test_003_004_setup,
  test_003_004_teardown,
  test_003_004_execute
};

/**
 * @page test_003_005 OS_TimerSet() periodic functionality
 *
 * <h2>Description</h2>
 * A timer is tested in periodic mode.
 *
 * <h2>Test Steps</h2>
 */

static void test_003_005_setup(void) {
  tmid = 0;
}

static void test_003_005_teardown(void) {
  if (tmid != 0) {
    (void) OS_TimerDelete(tmid);
  }
}

static void test_003_005_execute(void) {
}

static const testcase_t test_003_005 = {
  "OS_TimerSet() periodic functionality",
  test_003_005_setup,
  test_003_005_teardown,
  test_003_005_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Timers Functionality.
 */
const testcase_t * const test_sequence_003[] = {
  &test_003_001,
  &test_003_002,
  &test_003_003,
  &test_003_004,
  &test_003_005,
  NULL
};
