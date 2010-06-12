/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM8/chtypes.h
 * @brief   STM8 port system types.
 *
 * @addtogroup STM8_CORE
 * @{
 */

#ifndef _CHTYPES_H_
#define _CHTYPES_H_

#define __need_NULL
#define __need_size_t
#include <stddef.h>

//#if !defined(_STDINT_H) && !defined(__STDINT_H_)
//#include <stdint.h>
//#endif

typedef unsigned char   uint8_t;        /**< C99-style 8 bits unsigned.     */
typedef signed char     int8_t;         /**< C99-style 8 bits signed.       */
typedef unsigned int    uint16_t;       /**< C99-style 16 bits unsigned.    */
typedef signed int      int16_t;        /**< C99-style 16 bits signed.      */
typedef unsigned long   uint32_t;       /**< C99-style 32 bits unsigned.    */
typedef signed long     int32_t;        /**< C99-style 32 bits signed.      */
typedef uint8_t         uint_fast8_t;   /**< C99-style 8 bits unsigned.     */
typedef uint16_t        uint_fast16_t;  /**< C99-style 16 bits unsigned.    */
typedef uint32_t        uint_fast32_t;  /**< C99-style 32 bits unsigned.    */

/**
 * @brief   Boolean, recommended the fastest signed.
 */
typedef int8_t          bool_t;

/**
 * @brief   Thread mode flags, uint8_t is ok.
 */
typedef uint8_t         tmode_t;

/**
 * @brief   Thread state, uint8_t is ok.
 */
typedef uint8_t         tstate_t;

/**
 * @brief   Thread references counter, uint8_t is ok.
 */
typedef uint8_t         trefs_t;

/**
 * @brief   Priority, use the fastest unsigned type.
 */
typedef uint8_t         tprio_t;

/**
 * @brief   Message, use signed pointer equivalent.
 */
typedef int16_t         msg_t;

/**
 * @brief   Event Id, use fastest signed.
 */
typedef int8_t          eventid_t;

/**
 * @brief   Event Mask, recommended fastest unsigned.
 */
typedef uint8_t         eventmask_t;

/**
 * @brief   System Time, recommended fastest unsigned.
 */
typedef uint16_t        systime_t;

/**
 * @brief   Counter, recommended fastest signed.
 */
typedef int16_t         cnt_t;

/**
 * @brief   Inline function modifier.
 */
#define INLINE inline

/**
 * @brief   ROM constant modifier.
 * @note    Uses the custom "code" keyword in this port.
 */
#define ROMCONST code

/**
 * @brief   Packed structure modifier (within).
 * @note    Empty in this port.
 */
#define PACK_STRUCT_STRUCT

/**
 * @brief   Packed structure modifier (before).
 * @note    Empty in this port.
 */
#define PACK_STRUCT_BEGIN

/**
 * @brief   Packed structure modifier (after).
 * @note    Empty in this port.
 */
#define PACK_STRUCT_END

#endif /* _CHTYPES_H_ */

/** @} */
