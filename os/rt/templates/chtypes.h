/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    templates/chtypes.h
 * @brief   System types template.
 * @details The types defined in this file may change depending on the target
 *          architecture. You may also try to optimize the size of the various
 *          types in order to privilege size or performance, be careful in
 *          doing so.
 *
 * @addtogroup types
 * @details System types and macros.
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
typedef uint32_t        tprio_t;

/**
 * @brief   Message, use signed pointer equivalent.
 */
typedef int32_t         msg_t;

/**
 * @brief   Numeric event identifier, use fastest signed.
 */
typedef int32_t         eventid_t;

/**
 * @brief   Mask of event identifiers, recommended fastest unsigned.
 */
typedef uint32_t        eventmask_t;

/**
 * @brief   Mask of event flags, recommended fastest unsigned.
 */
typedef uint32_t        eventflags_t;

/**
 * @brief   System Time, recommended fastest unsigned.
 */
typedef uint32_t        systime_t;

/**
 * @brief   Counter, recommended fastest signed.
 */
typedef int32_t         cnt_t;

#endif /* _CHTYPES_H_ */

/** @} */
