/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @addtogroup Core
 * @{
 */

#ifndef _CHTYPES_H_
#define _CHTYPES_H_

#if !defined(_STDINT_H) && !defined(__STDINT_H_)
#include <stdint.h>
#endif

typedef int8_t          bool_t;
typedef uint8_t         t_tmode;    /* Thread mode flags, BYTE8 is ok. */
typedef uint8_t         t_tstate;   /* Thread state, BYTE8 is ok. */
typedef uint16_t        t_tid;      /* Thread id. */
typedef uint32_t        t_prio;     /* Priority, use the fastest unsigned type. */
typedef int32_t         t_msg;      /* Message, use signed pointer equivalent.*/
typedef int32_t         t_eventid;  /* Event Id, use fastest signed.*/
typedef uint32_t        t_eventmask;/* Event Mask, recommended fastest unsigned.*/
typedef uint32_t        t_time;     /* Time, recommended fastest unsigned.*/
typedef int32_t         t_cnt;      /* Counter, recommended fastest signed.*/
typedef uint32_t        t_size;     /* Size, use unsigned pointer equivalent.*/

#define INLINE      inline

#endif /* _CHTYPES_H_ */

/** @} */
