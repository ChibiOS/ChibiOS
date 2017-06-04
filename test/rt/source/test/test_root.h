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

/**
 * @file    test_root.h
 * @brief   Test Suite root structures header.
 */

#ifndef TEST_ROOT_H
#define TEST_ROOT_H

#include "test_sequence_001.h"
#include "test_sequence_002.h"
#include "test_sequence_003.h"
#include "test_sequence_004.h"
#include "test_sequence_005.h"
#include "test_sequence_006.h"
#include "test_sequence_007.h"
#include "test_sequence_008.h"
#include "test_sequence_009.h"
#include "test_sequence_010.h"
#include "test_sequence_011.h"
#include "test_sequence_012.h"

#if !defined(__DOXYGEN__)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern const testcase_t * const *test_suite[];

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Shared definitions.                                                       */
/*===========================================================================*/

#define TEST_SUITE_NAME                     "ChibiOS/RT Test Suite"

/*
 * Allowed delay in timeout checks.
 */
#define ALLOWED_DELAY MS2ST(2)

/*
 * Maximum number of test threads.
 */
#define MAX_THREADS             5

/*
 * Stack size of test threads.
 */
#if defined(CH_ARCHITECTURE_AVR) || defined(CH_ARCHITECTURE_MSP430)
#define THREADS_STACK_SIZE      48
#elif defined(CH_ARCHITECTURE_STM8)
#define THREADS_STACK_SIZE      64
#elif defined(CH_ARCHITECTURE_SIMIA32)
#define THREADS_STACK_SIZE      512
#else
#define THREADS_STACK_SIZE      128
#endif

/*
 * Working Area size of test threads.
 */
#define WA_SIZE MEM_ALIGN_NEXT(THD_WORKING_AREA_SIZE(THREADS_STACK_SIZE),	\
                               PORT_WORKING_AREA_ALIGN)

extern uint8_t test_buffer[WA_SIZE * 5];
extern thread_t *threads[MAX_THREADS];
extern void * ROMCONST wa[5];

void test_terminate_threads(void);
void test_wait_threads(void);
systime_t test_wait_tick(void);

#endif /* !defined(__DOXYGEN__) */

#endif /* TEST_ROOT_H */
