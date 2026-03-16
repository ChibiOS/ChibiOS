/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RP/rp_pico_sdk_compat.h
 * @brief   Pico SDK compatible linker section macros.
 * @note    These macros provide compatibility with the Raspberry Pi Pico SDK
 *          section placement conventions. Functions are placed in RAM via the
 *          .ramtext linker section. Data is placed in RAM via .data.*
 *          subsections. Both are loaded from flash and copied to RAM at
 *          startup by the existing CRT0 initialization.
 * @note    Enabled by defining @p RP_PICO_SDK_COMPAT to @p TRUE.
 *
 * @addtogroup RP_PICO_SDK_COMPAT
 * @{
 */

#ifndef RP_PICO_SDK_COMPAT_H
#define RP_PICO_SDK_COMPAT_H

#if !defined(RP_PICO_SDK_COMPAT)
#define RP_PICO_SDK_COMPAT              FALSE
#endif

#if (RP_PICO_SDK_COMPAT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Section placement macros.                                                 */
/*===========================================================================*/

/**
 * @brief   Places data in RAM instead of flash.
 * @details Uses .data.* subsections which are collected by the existing
 *          linker rules and copied to RAM at startup.
 *
 * @param[in] group     a string suffix for the section name
 */
#ifndef __not_in_flash
#define __not_in_flash(group) __attribute__((section(".data.not_in_flash." group)))
#endif

/**
 * @brief   Places a function in RAM instead of flash.
 * @details Uses the .ramtext linker section which is collected by the
 *          existing linker rules and copied to RAM at startup.
 * @note    The function may still be inlined into a flash-resident caller
 *          by the compiler. Use @p __no_inline_not_in_flash_func to prevent
 *          this.
 *
 * @param[in] func_name the function name
 */
#ifndef __not_in_flash_func
#define __not_in_flash_func(func_name) \
    __attribute__((section(".ramtext"))) func_name
#endif

/**
 * @brief   Places a function in RAM for time-critical execution.
 * @details Currently identical to @p __not_in_flash_func.
 *
 * @param[in] func_name the function name
 */
#ifndef __time_critical_func
#define __time_critical_func(func_name) __not_in_flash_func(func_name)
#endif

/**
 * @brief   Places a function in RAM and prevents inlining.
 *
 * @param[in] func_name the function name
 */
#ifndef __no_inline_not_in_flash_func
#define __no_inline_not_in_flash_func(func_name) \
    __attribute__((noinline, section(".ramtext"))) func_name
#endif

/**
 * @brief   Places data explicitly in flash.
 * @note    Useful for const data that should remain in flash even in
 *          copy-to-RAM builds.
 *
 * @param[in] group     a string suffix for the section name
 */
#ifndef __in_flash
#define __in_flash(group) __attribute__((section(".flashdata." group)))
#endif

#endif /* RP_PICO_SDK_COMPAT == TRUE */

#endif /* RP_PICO_SDK_COMPAT_H */

/** @} */
