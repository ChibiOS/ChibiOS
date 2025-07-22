/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ARM-common/chtypes.h
 * @brief   ARM ports system types.
 *
 * @addtogroup ARM_COMMON_CORE
 * @{
 */

#ifndef CHTYPES_H
#define CHTYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "ccportab.h"

/**
 * @name    Architecture data constraints
 * @{
 */
#define PORT_ARCH_SIZEOF_DATA_PTR   4
#define PORT_ARCH_SIZEOF_CODE_PTR   4
#define PORT_ARCH_REGISTERS_WIDTH   32
#define PORT_ARCH_REVERSE_ORDER     1
/** @} */

/**
 * @name    Port types
 * @{
 */
/**
 * @brief   Realtime counter.
 */
typedef uint32_t            port_rtcnt_t;

/**
 * @brief   Realtime accumulator.
 */
typedef uint64_t            port_rttime_t;

/**
 * @brief   System status word.
 */
typedef uint32_t            port_syssts_t;

/**
 * @brief   Type of stack and memory alignment enforcement.
 * @note    In this architecture the stack alignment is enforced to 64 bits,
 *          32 bits alignment is supported by hardware but deprecated by ARM,
 *          the implementation choice is to not offer the option.
 */
typedef uint64_t            port_stkline_t; 
typedef uint64_t            port_stkalign_t; /* Alternative name.*/
/** @} */

/**
 * @brief   This port does not define OS-related types.
 */
#define PORT_DOES_NOT_PROVIDE_TYPES

/**
 * @brief   ROM constant modifier.
 * @note    It is set to use the "const" keyword in this port.
 */
#define ROMCONST            CC_ROMCONST

/**
 * @brief   Makes functions not inlineable.
 * @note    If the compiler does not support such attribute then some
 *          time-dependent services could be degraded.
 */
#define NOINLINE            CC_NO_INLINE

/**
 * @brief   Memory alignment enforcement for variables.
 */
#define ALIGNED_VAR(n)      CC_ALIGN_DATA(n)

/**
 * @brief   Size of a pointer.
 * @note    To be used where the sizeof operator cannot be used, preprocessor
 *          expressions for example.
 */
#define SIZEOF_PTR          PORT_ARCH_SIZEOF_DATA_PTR

/**
 * @brief   Marks a boolean expression as likely true.
 *
 * @param[in] x         a valid expression
 */
#if defined(CC_LIKELY) || defined(__DOXYGEN__)
#define PORT_LIKELY(x)      CC_LIKELY(x)
#else
#define PORT_LIKELY(x)      x
#endif

/**
 * @brief   Marks a boolean expression as likely false.
 *
 * @param[in] x         a valid expression
 */
#if defined(CC_UNLIKELY) || defined(__DOXYGEN__)
#define PORT_UNLIKELY(x)    CC_UNLIKELY(x)
#else
#define PORT_UNLIKELY(x)    x
#endif

#endif /* CHTYPES_H */

/** @} */
