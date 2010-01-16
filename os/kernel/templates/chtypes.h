/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @file templates/chtypes.h
 * @brief System types template.
 * @addtogroup types
 * @{
 */

#ifndef _CHTYPES_H_
#define _CHTYPES_H_

#define __need_NULL
#define __need_size_t
#include <stddef.h>

#if !defined(_STDINT_H) && !defined(__STDINT_H_)
#include <stdint.h>
#endif

/** Signed boolean. */
typedef int32_t         bool_t;

/** Thread mode flags, uint8_t is ok. */
typedef uint8_t         tmode_t;

/** Thread state, uint8_t is ok. */
typedef uint8_t         tstate_t;

/** Priority, use the fastest unsigned type. */
typedef uint32_t        tprio_t;

/** Message, use signed pointer equivalent.*/
typedef int32_t         msg_t;

/** Event Id, use fastest signed.*/
typedef int32_t         eventid_t;

/** Event Mask, recommended fastest unsigned.*/
typedef uint32_t        eventmask_t;

/** System Time, recommended fastest unsigned.*/
typedef uint32_t        systime_t;

/** Counter, recommended fastest signed.*/
typedef int32_t         cnt_t;

/** Inline function modifier. */
#define INLINE      inline

/** Packed structure modifier (within). */
#define PACK_STRUCT_STRUCT __attribute__((packed))

/** Packed structure modifier (before). */
#define PACK_STRUCT_BEGIN

/** Packed structure modifier (after). */
#define PACK_STRUCT_END

#endif /* _CHTYPES_H_ */

/** @} */
