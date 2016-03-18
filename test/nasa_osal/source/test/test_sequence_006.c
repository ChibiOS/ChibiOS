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
 * @page test_sequence_006 Mutex Semaphores Functionality
 *
 * File: @ref test_sequence_006.c
 *
 * <h2>Description</h2>
 * This sequence tests the NASA OSAL over ChibiOS/RT functionalities
 * related to mutex semaphores.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_006_001
 * - @subpage test_006_002
 * - @subpage test_006_003
 * - @subpage test_006_004
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include "osapi.h"

uint32 msid;

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_006_001 OS_MutSemCreate() and OS_MutSemDelete() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_MutSemCreate() and OS_MutSemDelete() is
 * tested.
 *
 * <h2>Test Steps</h2>
 * - OS_MutSemCreate() is invoked with sem_id set to NULL, an error is
 *   expected.
 * - OS_MutSemCreate() is invoked with sem_name set to NULL, an error
 *   is expected.
 * - OS_MutSemCreate() is invoked with a very long timer name, an error
 *   is expected.
 * - OS_MutSemDelete() is invoked with timer_id set to -1, an error is
 *   expected.
 * - OS_MutSemCreate() is invoked twice with duplicated name, an error
 *   is expected, then the queue is deleted using OS_MutSemDelete().
 * .
 */

static void test_006_001_execute(void) {

  /* OS_MutSemCreate() is invoked with sem_id set to NULL, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_MutSemCreate(NULL,                     /* Error.*/
                         "failing semaphore",
                         0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_MutSemCreate() is invoked with sem_name set to NULL, an error
     is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_MutSemCreate(&msid,
                         NULL,                      /* Error.*/
                         0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_MutSemCreate() is invoked with a very long timer name, an error
     is expected.*/
  test_set_step(3);
  {
    #if 0 /* Semaphore name currently not implemented.*/
    int32 err;

    err = OS_MutSemCreate(&msid,
                         "very very long semaphore name",   /* Error.*/
                         0);
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
    #endif
  }

  /* OS_MutSemDelete() is invoked with timer_id set to -1, an error is
     expected.*/
  test_set_step(4);
  {
    int32 err;

    err = OS_MutSemDelete((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "wrong semaphore id not detected");
  }

  /* OS_MutSemCreate() is invoked twice with duplicated name, an error
     is expected, then the queue is deleted using OS_MutSemDelete().*/
  test_set_step(5);
  {
    int32 err;
    uint32 msid1; /*, msid2;*/

    err = OS_MutSemCreate(&msid1, "my semaphore", 0);
    test_assert(err == OS_SUCCESS, "semaphore creation failed");

    #if 0 /* Semaphore name currently not implemented.*/
    err = OS_MutSemCreate(&msid2, "my semaphore", 0);
    test_assert(err == OS_ERR_NAME_TAKEN, "name conflict not detected");
    #endif

    err = OS_MutSemDelete(msid1);
    test_assert(err == OS_SUCCESS, "semaphore deletion failed");
  }
}

static const testcase_t test_006_001 = {
  "OS_MutSemCreate() and OS_MutSemDelete() errors",
  NULL,
  NULL,
  test_006_001_execute
};

/**
 * @page test_006_002 OS_MutSemGive() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_MutSemGive() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_MutSemGive() is invoked with sem_id set to -1, an error is
 *   expected.
 * .
 */

static void test_006_002_execute(void) {

  /* OS_MutSemGive() is invoked with sem_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_MutSemGive((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_006_002 = {
  "OS_MutSemGive() errors",
  NULL,
  NULL,
  test_006_002_execute
};

/**
 * @page test_006_003 OS_MutSemTake() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_MutSemTake() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_MutSemTake() is invoked with sem_id set to -1, an error is
 *   expected.
 * .
 */

static void test_006_003_execute(void) {

  /* OS_MutSemTake() is invoked with sem_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_MutSemTake((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_006_003 = {
  "OS_MutSemTake() errors",
  NULL,
  NULL,
  test_006_003_execute
};

/**
 * @page test_006_004 OS_MutSemGetIdByName() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_MutSemGetIdByName() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_MutSemGetIdByName() is invoked with sem_id set to NULL, an
 *   error is expected.
 * - OS_MutSemGetIdByName() is invoked with semaphore name set to NULL,
 *   an error is expected.
 * - OS_MutSemGetIdByName() is invoked with a very long task name, an
 *   error is expected.
 * .
 */

static void test_006_004_execute(void) {

  /* OS_MutSemGetIdByName() is invoked with sem_id set to NULL, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_MutSemGetIdByName(NULL, "semaphore");
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_MutSemGetIdByName() is invoked with semaphore name set to NULL,
     an error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_MutSemGetIdByName(&msid, NULL);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_MutSemGetIdByName() is invoked with a very long task name, an
     error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_MutSemGetIdByName(&msid, "very very long semaphore name");
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }
}

static const testcase_t test_006_004 = {
  "OS_MutSemGetIdByName() errors",
  NULL,
  NULL,
  test_006_004_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Mutex Semaphores Functionality.
 */
const testcase_t * const test_sequence_006[] = {
  &test_006_001,
  &test_006_002,
  &test_006_003,
  &test_006_004,
  NULL
};
