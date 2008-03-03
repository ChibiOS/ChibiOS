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

#if !defined(_STDINT_H) && !defined(__STDINT_H_)
#include <stdint.h>
#endif

typedef int8_t          t_bool;
typedef uint8_t         t_tmode;
typedef uint8_t         t_tstate;
typedef uint8_t         t_tid;
typedef uint8_t         t_prio;
typedef int16_t         t_msg;
typedef uint8_t         t_eventid;
typedef uint8_t         t_eventmask;
typedef uint16_t        t_time;
typedef int8_t          t_cnt;
typedef uint16_t        t_size;

#define INLINE      inline

#endif /* _CHTYPES_H_ */
