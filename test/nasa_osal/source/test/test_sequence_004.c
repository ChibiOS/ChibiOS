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
 * @page test_sequence_004 Binary Semaphores Functionality
 *
 * File: @ref test_sequence_004.c
 *
 * <h2>Description</h2>
 * This sequence tests the NASA OSAL over ChibiOS/RT functionalities
 * related to binary semaphores.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_004_001
 * - @subpage test_004_002
 * - @subpage test_004_003
 * - @subpage test_004_004
 * - @subpage test_004_005
 * - @subpage test_004_006
 * - @subpage test_004_007
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include "osapi.h"

uint32 bsid;

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_004_001 OS_BinSemCreate() and OS_BinSemDelete() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_BinSemCreate() and OS_BinSemDelete() is
 * tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemCreate() is invoked with sem_id set to NULL, an error is
 *   expected.
 * - OS_BinSemCreate() is invoked with sem_name set to NULL, an error
 *   is expected.
 * - OS_BinSemCreate() is invoked with an invalid sem_initial_value, an
 *   error is expected.
 * - OS_BinSemCreate() is invoked with a very long timer name, an error
 *   is expected.
 * - OS_BinSemDelete() is invoked with timer_id set to -1, an error is
 *   expected.
 * - OS_BinSemCreate() is invoked twice with duplicated name, an error
 *   is expected, then the queue is deleted using OS_BinSemDelete().
 * .
 */

static void test_004_001_execute(void) {

  /* OS_BinSemCreate() is invoked with sem_id set to NULL, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemCreate(NULL,                     /* Error.*/
                         "failing semaphore",
                         0,
                         0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_BinSemCreate() is invoked with sem_name set to NULL, an error
     is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_BinSemCreate(&bsid,
                         NULL,                      /* Error.*/
                         0,
                         0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_BinSemCreate() is invoked with an invalid sem_initial_value, an
     error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_BinSemCreate(&bsid,
                         "failing semaphore",
                         2,                         /* Error.*/
                         0);
    test_assert(err == OS_INVALID_INT_NUM, "counter error not detected");
  }

  /* OS_BinSemCreate() is invoked with a very long timer name, an error
     is expected.*/
  test_set_step(4);
  {
    #if 0 /* Semaphore name currently not implemented.*/
    int32 err;

    err = OS_BinSemCreate(&bsid,
                         "very very long semaphore name",   /* Error.*/
                         0,
                         0);
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
    #endif
  }

  /* OS_BinSemDelete() is invoked with timer_id set to -1, an error is
     expected.*/
  test_set_step(5);
  {
    int32 err;

    err = OS_BinSemDelete((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "wrong semaphore id not detected");
  }

  /* OS_BinSemCreate() is invoked twice with duplicated name, an error
     is expected, then the queue is deleted using OS_BinSemDelete().*/
  test_set_step(6);
  {
    int32 err;
    uint32 bsid1; /*, bsid2;*/

    err = OS_BinSemCreate(&bsid1, "my semaphore", 0, 0);
    test_assert(err == OS_SUCCESS, "semaphore creation failed");

    #if 0 /* Semaphore name currently not implemented.*/
    err = OS_BinSemCreate(&bsid2, "my semaphore", 0, 0);
    test_assert(err == OS_ERR_NAME_TAKEN, "name conflict not detected");
    #endif

    err = OS_BinSemDelete(bsid1);
    test_assert(err == OS_SUCCESS, "semaphore deletion failed");
  }
}

static const testcase_t test_004_001 = {
  "OS_BinSemCreate() and OS_BinSemDelete() errors",
  NULL,
  NULL,
  test_004_001_execute
};

/**
 * @page test_004_002 OS_BinSemFlush() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_BinSemFlush() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemFlush() is invoked with sem_id set to -1, an error is
 *   expected.
 * .
 */

static void test_004_002_execute(void) {

  /* OS_BinSemFlush() is invoked with sem_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemFlush((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_004_002 = {
  "OS_BinSemFlush() errors",
  NULL,
  NULL,
  test_004_002_execute
};

/**
 * @page test_004_003 OS_BinSemGive() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_BinSemGive() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemGive() is invoked with sem_id set to -1, an error is
 *   expected.
 * .
 */

static void test_004_003_execute(void) {

  /* OS_BinSemGive() is invoked with sem_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemGive((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_004_003 = {
  "OS_BinSemGive() errors",
  NULL,
  NULL,
  test_004_003_execute
};

/**
 * @page test_004_004 OS_BinSemTake() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_BinSemTake() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemTake() is invoked with sem_id set to -1, an error is
 *   expected.
 * .
 */

static void test_004_004_execute(void) {

  /* OS_BinSemTake() is invoked with sem_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemTake((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_004_004 = {
  "OS_BinSemTake() errors",
  NULL,
  NULL,
  test_004_004_execute
};

/**
 * @page test_004_005 OS_BinSemTimedWait() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_BinSemTimedWait() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemTimedWait() is invoked with sem_id set to -1, an error is
 *   expected.
 * - OS_BinSemTimedWait() is invoked with msecs set to 0, an error is
 *   expected.
 * .
 */

static void test_004_005_setup(void) {
  bsid = 0;
  (void) OS_BinSemCreate(&bsid, "test semaphore", 0, 0);
}

static void test_004_005_teardown(void) {
  if (bsid > 0) {
    (void) OS_BinSemDelete(bsid);
  }
}

static void test_004_005_execute(void) {

  /* OS_BinSemTimedWait() is invoked with sem_id set to -1, an error is
     expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemTimedWait((uint32)-1, 1000);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }

  /* OS_BinSemTimedWait() is invoked with msecs set to 0, an error is
     expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_BinSemTimedWait(bsid, 0);
    test_assert(err == OS_INVALID_INT_NUM, "invalid msec not detected");
  }
}

static const testcase_t test_004_005 = {
  "OS_BinSemTimedWait() errors",
  test_004_005_setup,
  test_004_005_teardown,
  test_004_005_execute
};

/**
 * @page test_004_006 OS_BinSemGetIdByName() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_BinSemGetIdByName() is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemGetIdByName() is invoked with sem_id set to NULL, an
 *   error is expected.
 * - OS_BinSemGetIdByName() is invoked with semaphore name set to NULL,
 *   an error is expected.
 * - OS_BinSemGetIdByName() is invoked with a very long task name, an
 *   error is expected.
 * .
 */

static void test_004_006_execute(void) {

  /* OS_BinSemGetIdByName() is invoked with sem_id set to NULL, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemGetIdByName(NULL, "semaphore");
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_BinSemGetIdByName() is invoked with semaphore name set to NULL,
     an error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_BinSemGetIdByName(&bsid, NULL);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* OS_BinSemGetIdByName() is invoked with a very long task name, an
     error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_BinSemGetIdByName(&bsid, "very very long semaphore name");
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }
}

static const testcase_t test_004_006 = {
  "OS_BinSemGetIdByName() errors",
  NULL,
  NULL,
  test_004_006_execute
};

/**
 * @page test_004_007 OS_BinSemTimedWait() timeout functionality
 *
 * <h2>Description</h2>
 * OS_BinSemCreate() timeout functionality is tested.
 *
 * <h2>Test Steps</h2>
 * - OS_BinSemTimedWait() is invoked with timeout set to one second, an
 *   error is expected.
 * .
 */

static void test_004_007_setup(void) {
  bsid = 0;
  (void) OS_BinSemCreate(&bsid, "test semaphore", 0, 0);
}

static void test_004_007_teardown(void) {
  if (bsid > 0) {
    (void) OS_BinSemDelete(bsid);
  }
}

static void test_004_007_execute(void) {

  /* OS_BinSemTimedWait() is invoked with timeout set to one second, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_BinSemTimedWait(bsid, 1000);
    test_assert(err == OS_SEM_TIMEOUT, "unexpected error code");
  }
}

static const testcase_t test_004_007 = {
  "OS_BinSemTimedWait() timeout functionality",
  test_004_007_setup,
  test_004_007_teardown,
  test_004_007_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Binary Semaphores Functionality.
 */
const testcase_t * const test_sequence_004[] = {
  &test_004_001,
  &test_004_002,
  &test_004_003,
  &test_004_004,
  &test_004_005,
  &test_004_006,
  &test_004_007,
  NULL
};
