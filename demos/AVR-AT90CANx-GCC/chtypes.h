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

#ifndef _CHTYPES_H_
#define _CHTYPES_H_

/*
 * Generic types often dependant on the compiler.
 */
#define BOOL        char
#define BYTE8       unsigned char
#define SBYTE8      signed char
#define WORD16      int
#define UWORD16     unsigned int
#define LONG32      long
#define ULONG32     unsigned long
#define PTR_EQ      int

typedef BYTE8       t_tmode;
typedef BYTE8       t_tstate;
typedef BYTE8       t_tid;
typedef BYTE8       t_prio;
typedef WORD16      t_msg;
typedef BYTE8       t_eventid;
typedef BYTE8       t_eventmask;
typedef UWORD16     t_time;
typedef SBYTE8      t_cnt;
typedef UWORD16     t_size;

#define INLINE      inline

#endif /* _CHTYPES_H_ */
