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

#define TEST_SUITE_NAME                     "ChibiOS/NIL Test Suite"

#define TEST_REPORT_HOOK_HEADER test_print_port_info();

extern semaphore_t gsem1, gsem2;
extern thread_reference_t gtr1;
extern THD_WORKING_AREA(wa_test_support, 128);

void test_print_port_info(void);
THD_FUNCTION(test_support, arg);

#endif /* !defined(__DOXYGEN__) */

#endif /* TEST_ROOT_H */
