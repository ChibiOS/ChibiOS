/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio

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
#include "oslib_test_root.h"

/**
 * @file    oslib_test_sequence_002.c
 * @brief   Test Sequence 002 code.
 *
 * @page oslib_test_sequence_002 [2] Pipes
 *
 * File: @ref oslib_test_sequence_002.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS library functionalities related to
 * pipes.
 *
 * <h2>Conditions</h2>
 * This sequence is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_USE_PIPES
 * .
 *
 * <h2>Test Cases</h2>
 * - @subpage oslib_test_002_001
 * .
 */

#if (CH_CFG_USE_PIPES) || defined(__DOXYGEN__)

/****************************************************************************
 * Shared code.
 ****************************************************************************/

#define PIPE_SIZE 16

static uint8_t buffer[PIPE_SIZE];
static PIPE_DECL(pipe1, buffer, PIPE_SIZE);

static const uint8_t pipe_pattern[] = "0123456789ABCDEF";

/****************************************************************************
 * Test cases.
 ****************************************************************************/

/**
 * @page oslib_test_002_001 [2.1] Loading and emptying a pipe, non blocking
 *
 * <h2>Description</h2>
 * The pipe functionality is tested by loading and emptying it, all
 * conditions are tested.
 *
 * <h2>Test Steps</h2>
 * - [2.1.1] Filling whole pipe.
 * - [2.1.2] Emptying pipe.
 * - [2.1.3].
 * - [2.1.4].
 * - [2.1.5].
 * - [2.1.6].
 * .
 */

static void oslib_test_002_001_setup(void) {
  chPipeObjectInit(&pipe1, buffer, PIPE_SIZE);
}

static void oslib_test_002_001_execute(void) {
  unsigned i;

  /* [2.1.1] Filling whole pipe.*/
  test_set_step(1);
  {
    msg_t msg;

    msg = chPipeWriteTimeout(&pipe1, pipe_pattern, PIPE_SIZE, TIME_IMMEDIATE);
    test_assert(msg == PIPE_SIZE, "wrong size");
    test_assert((pipe1.rdptr == pipe1.buffer) &&
                (pipe1.wrptr == pipe1.buffer) &&
                (pipe1.cnt == PIPE_SIZE),
                "invalid pipe state");
  }

  /* [2.1.2] Emptying pipe.*/
  test_set_step(2);
  {
    msg_t msg;
    uint8_t buf[PIPE_SIZE];

    msg = chPipeReadTimeout(&pipe1, buf, PIPE_SIZE, TIME_IMMEDIATE);
    test_assert(msg == PIPE_SIZE, "wrong size");
    test_assert((pipe1.rdptr == pipe1.buffer) &&
                (pipe1.wrptr == pipe1.buffer) &&
                (pipe1.cnt == 0),
                "invalid pipe state");
  }

  /* [2.1.3].*/
  test_set_step(3);
  {
  }

  /* [2.1.4].*/
  test_set_step(4);
  {
  }

  /* [2.1.5].*/
  test_set_step(5);
  {
  }

  /* [2.1.6].*/
  test_set_step(6);
  {
  }
}

static const testcase_t oslib_test_002_001 = {
  "Loading and emptying a pipe, non blocking",
  oslib_test_002_001_setup,
  NULL,
  oslib_test_002_001_execute
};

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t * const oslib_test_sequence_002_array[] = {
  &oslib_test_002_001,
  NULL
};

/**
 * @brief   Pipes.
 */
const testsequence_t oslib_test_sequence_002 = {
  "Pipes",
  oslib_test_sequence_002_array
};

#endif /* CH_CFG_USE_PIPES */
