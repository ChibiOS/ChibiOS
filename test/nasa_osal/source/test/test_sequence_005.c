/* Copyright statement.*/

#include "hal.h"
#include "ch_test.h"
#include "test_root.h"

/**
 * @file    test_sequence_005.c
 * @brief   Test Sequence 005 code.
 *
 * @page test_sequence_005 [5] Counter Semaphores Functionality
 *
 * File: @ref test_sequence_005.c
 *
 * <h2>Description</h2>
 * This sequence tests the NASA OSAL over ChibiOS/RT functionalities
 * related to counter semaphores.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_005_001
 * - @subpage test_005_002
 * - @subpage test_005_003
 * - @subpage test_005_004
 * - @subpage test_005_005
 * - @subpage test_005_006
 * .
 */

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#include "osapi.h"

uint32 csid;

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page test_005_001 [5.1] OS_CountSemCreate() and OS_CountSemDelete() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_CountSemCreate() and OS_CountSemDelete()
 * is tested.
 *
 * <h2>Test Steps</h2>
 * - [5.1.1] OS_CountSemCreate() is invoked with sem_id set to NULL, an
 *   error is expected.
 * - [5.1.2] OS_CountSemCreate() is invoked with sem_name set to NULL,
 *   an error is expected.
 * - [5.1.3] OS_CountSemCreate() is invoked with an invalid
 *   sem_initial_value, an error is expected.
 * - [5.1.4] OS_CountSemCreate() is invoked with a very long timer
 *   name, an error is expected.
 * - [5.1.5] OS_CountSemDelete() is invoked with timer_id set to -1, an
 *   error is expected.
 * - [5.1.6] OS_CountSemCreate() is invoked twice with duplicated name,
 *   an error is expected, then the queue is deleted using
 *   OS_CountSemDelete().
 * .
 */

static void test_005_001_execute(void) {

  /* [5.1.1] OS_CountSemCreate() is invoked with sem_id set to NULL, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_CountSemCreate(NULL,                   /* Error.*/
                            "failing semaphore",
                            0,
                            0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* [5.1.2] OS_CountSemCreate() is invoked with sem_name set to NULL,
     an error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_CountSemCreate(&csid,
                            NULL,                   /* Error.*/
                            0,
                            0);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* [5.1.3] OS_CountSemCreate() is invoked with an invalid
     sem_initial_value, an error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_CountSemCreate(&csid,
                            "failing semaphore",
                            (uint32)-1,              /* Error.*/
                            0);
    test_assert(err == OS_INVALID_INT_NUM, "counter error not detected");
  }

  /* [5.1.4] OS_CountSemCreate() is invoked with a very long timer
     name, an error is expected.*/
  test_set_step(4);
  {
#if 0 /* Semaphore name currently not implemented.*/
    int32 err;

    err = OS_CountSemCreate(&csid,
                            "very very long semaphore name",/* Error.*/
                            0,
                            0);
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
#endif
  }

  /* [5.1.5] OS_CountSemDelete() is invoked with timer_id set to -1, an
     error is expected.*/
  test_set_step(5);
  {
    int32 err;

    err = OS_CountSemDelete((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "wrong semaphore id not detected");
  }

  /* [5.1.6] OS_CountSemCreate() is invoked twice with duplicated name,
     an error is expected, then the queue is deleted using
     OS_CountSemDelete().*/
  test_set_step(6);
  {
    int32 err;
    uint32 csid1; /*, csid2;*/

    err = OS_CountSemCreate(&csid1, "my semaphore", 0, 0);
    test_assert(err == OS_SUCCESS, "semaphore creation failed");

#if 0 /* Semaphore name currently not implemented.*/
    err = OS_CountSemCreate(&csid2, "my semaphore", 0, 0);
    test_assert(err == OS_ERR_NAME_TAKEN, "name conflict not detected");
#endif

    err = OS_CountSemDelete(csid1);
    test_assert(err == OS_SUCCESS, "semaphore deletion failed");
  }
}

static const testcase_t test_005_001 = {
  "OS_CountSemCreate() and OS_CountSemDelete() errors",
  NULL,
  NULL,
  test_005_001_execute
};

/**
 * @page test_005_002 [5.2] OS_CountSemGive() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_CountSemGive() is tested.
 *
 * <h2>Test Steps</h2>
 * - [5.2.1] OS_CountSemGive() is invoked with sem_id set to -1, an
 *   error is expected.
 * .
 */

static void test_005_002_execute(void) {

  /* [5.2.1] OS_CountSemGive() is invoked with sem_id set to -1, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_CountSemGive((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_005_002 = {
  "OS_CountSemGive() errors",
  NULL,
  NULL,
  test_005_002_execute
};

/**
 * @page test_005_003 [5.3] OS_CountSemTake() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_CountSemTake() is tested.
 *
 * <h2>Test Steps</h2>
 * - [5.3.1] OS_CountSemTake() is invoked with sem_id set to -1, an
 *   error is expected.
 * .
 */

static void test_005_003_execute(void) {

  /* [5.3.1] OS_CountSemTake() is invoked with sem_id set to -1, an
     error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_CountSemTake((uint32)-1);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }
}

static const testcase_t test_005_003 = {
  "OS_CountSemTake() errors",
  NULL,
  NULL,
  test_005_003_execute
};

/**
 * @page test_005_004 [5.4] OS_CountSemTimedWait() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_CountSemTimedWait() is tested.
 *
 * <h2>Test Steps</h2>
 * - [5.4.1] OS_CountSemTimedWait() is invoked with sem_id set to -1,
 *   an error is expected.
 * - [5.4.2] OS_CountSemTimedWait() is invoked with msecs set to 0, an
 *   error is expected.
 * .
 */

static void test_005_004_setup(void) {
  csid = 0;
  (void) OS_CountSemCreate(&csid, "test semaphore", 0, 0);
}

static void test_005_004_teardown(void) {
  if (csid > 0) {
    (void) OS_CountSemDelete(csid);
  }
}

static void test_005_004_execute(void) {

  /* [5.4.1] OS_CountSemTimedWait() is invoked with sem_id set to -1,
     an error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_CountSemTimedWait((uint32)-1, 1000);
    test_assert(err == OS_ERR_INVALID_ID, "invalid sem_id not detected");
  }

  /* [5.4.2] OS_CountSemTimedWait() is invoked with msecs set to 0, an
     error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_CountSemTimedWait(csid, 0);
    test_assert(err == OS_INVALID_INT_NUM, "invalid msec not detected");
  }
}

static const testcase_t test_005_004 = {
  "OS_CountSemTimedWait() errors",
  test_005_004_setup,
  test_005_004_teardown,
  test_005_004_execute
};

/**
 * @page test_005_005 [5.5] OS_CountSemGetIdByName() errors
 *
 * <h2>Description</h2>
 * Parameters checking in OS_CountSemGetIdByName() is tested.
 *
 * <h2>Test Steps</h2>
 * - [5.5.1] OS_CountSemGetIdByName() is invoked with sem_id set to
 *   NULL, an error is expected.
 * - [5.5.2] OS_CountSemGetIdByName() is invoked with semaphore name
 *   set to NULL, an error is expected.
 * - [5.5.3] OS_CountSemGetIdByName() is invoked with a very long task
 *   name, an error is expected.
 * .
 */

static void test_005_005_execute(void) {

  /* [5.5.1] OS_CountSemGetIdByName() is invoked with sem_id set to
     NULL, an error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_CountSemGetIdByName(NULL, "semaphore");
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* [5.5.2] OS_CountSemGetIdByName() is invoked with semaphore name
     set to NULL, an error is expected.*/
  test_set_step(2);
  {
    int32 err;

    err = OS_CountSemGetIdByName(&csid, NULL);
    test_assert(err == OS_INVALID_POINTER, "NULL not detected");
  }

  /* [5.5.3] OS_CountSemGetIdByName() is invoked with a very long task
     name, an error is expected.*/
  test_set_step(3);
  {
    int32 err;

    err = OS_CountSemGetIdByName(&csid, "very very long semaphore name");
    test_assert(err == OS_ERR_NAME_TOO_LONG, "name limit not detected");
  }
}

static const testcase_t test_005_005 = {
  "OS_CountSemGetIdByName() errors",
  NULL,
  NULL,
  test_005_005_execute
};

/**
 * @page test_005_006 [5.6] OS_CountSemTimedWait() timeout functionality
 *
 * <h2>Description</h2>
 * OS_CountSemCreate() timeout functionality is tested.
 *
 * <h2>Test Steps</h2>
 * - [5.6.1] OS_CountSemTimedWait() is invoked with timeout set to one
 *   second, an error is expected.
 * .
 */

static void test_005_006_setup(void) {
  csid = 0;
  (void) OS_CountSemCreate(&csid, "test semaphore", 0, 0);
}

static void test_005_006_teardown(void) {
  if (csid > 0) {
    (void) OS_CountSemDelete(csid);
  }
}

static void test_005_006_execute(void) {

  /* [5.6.1] OS_CountSemTimedWait() is invoked with timeout set to one
     second, an error is expected.*/
  test_set_step(1);
  {
    int32 err;

    err = OS_CountSemTimedWait(csid, 1000);
    test_assert(err == OS_SEM_TIMEOUT, "unexpected error code");
  }
}

static const testcase_t test_005_006 = {
  "OS_CountSemTimedWait() timeout functionality",
  test_005_006_setup,
  test_005_006_teardown,
  test_005_006_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Counter Semaphores Functionality.
 */
const testcase_t * const test_sequence_005[] = {
  &test_005_001,
  &test_005_002,
  &test_005_003,
  &test_005_004,
  &test_005_005,
  &test_005_006,
  NULL
};
