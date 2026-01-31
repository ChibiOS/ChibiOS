/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sandbox/hal_lld.h
 * @brief   SandBox HAL subsystem low level driver header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

#include "sbuser.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Specifies implementation of dynamic clock management.
 */
/*#define HAL_LLD_USE_CLOCK_MANAGEMENT*/

/**
 * @name    Platform identification macros
 * @{
 */
#define PLATFORM_NAME           "SandBox"
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    PLATFORM configuration options
 * @{
 */
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*
 * Configuration-related checks.
 */
#if !defined(__VIO_XMCUCONF__)
#error "Using a wrong xmcuconf.h file, __VIO_XMCUCONF__ not defined"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a clock point identifier.
 */
typedef unsigned halclkpt_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#define __MK_VECTOR(n)      __sb_vector##n
#define MK_VECTOR(n)        __MK_VECTOR(n)

/**
 * @brief   Builds a VIO call 1st parameter.
 *
 * @param[in] c         the sub-code
 * @param[in] u         the unit identifier
 * @return              The formatted VIO call 1st parameter.
 */
#define VIO_CALL(c, u) (((uint32_t)(u) << 24) | (uint32_t)(c))

/**
 * @brief   Returns the frequency of a clock point in Hz.
 * @note    Static implementation.
 *
 * @param[in] clkpt     clock point to be returned
 * @return              The clock point frequency in Hz or zero if the
 *                      frequency is unknown.
 *
 * @notapi
 */
#define hal_lld_get_clock_point(clkpt) 0U

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
