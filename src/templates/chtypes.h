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

/*
 * Generic types often dependant on the compiler.
 */
#define BOOL        char
#define BYTE8       unsigned char
#define SBYTE8      char
#define WORD16      short
#define UWORD16     unsigned short
#define LONG32      int
#define ULONG32     unsigned int

typedef BYTE8       t_tmode;    /* Thread mode flags, BYTE8 is ok. */
typedef BYTE8       t_tstate;   /* Thread state, BYTE8 is ok. */
typedef ULONG32     t_prio;     /* Priority, use the fastest unsigned type. */
typedef LONG32      t_msg;      /* Message, use signed pointer equivalent.*/
typedef LONG32      t_eventid;  /* Event Id, use fastest signed.*/
typedef ULONG32     t_eventmask;/* Event Mask, recommended fastest unsigned.*/
typedef ULONG32     t_time;     /* Time, recommended fastest unsigned.*/
typedef LONG32      t_cnt;      /* Counter, recommended fastest signed.*/
typedef ULONG32     t_size;     /* Size, use unsigned pointer equivalent.*/

#define THREAD
#define INLINE      inline

#endif /* _CHTYPES_H_ */

/** @} */
