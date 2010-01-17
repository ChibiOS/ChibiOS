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
 * @file ch.h
 * @brief ChibiOS/RT main include file, it includes everything else.
 * @addtogroup kernel_info
 * @{
 */

#ifndef _CH_H_
#define _CH_H_

/**
 * ChibiOS/RT identification macro.
 */
#define _CHIBIOS_RT_

/**
 * Kernel version string.
 */
#define CH_KERNEL_VERSION       "1.4.0"

/**
 * Kernel version major number.
 */
#define CH_KERNEL_MAJOR         1

/**
 * Kernel version minor number.
 */
#define CH_KERNEL_MINOR         4

/**
 * Kernel version patch number.
 */
#define CH_KERNEL_PATCH         0

/*
 * Common values.
 */
#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        (!FALSE)
#endif

#include "chconf.h"
#include "chtypes.h"
#include "lists.h"
#include "chcore.h"
#include "sys.h"
#include "vt.h"
#include "scheduler.h"
#include "semaphores.h"
#include "mutexes.h"
#include "condvars.h"
#include "events.h"
#include "messages.h"
#include "mailboxes.h"
#include "memcore.h"
#include "heap.h"
#include "mempools.h"
#include "threads.h"
#include "inline.h"
#include "queues.h"
#include "streams.h"
#include "channels.h"
#include "debug.h"

#endif /* _CH_H_ */

/** @} */
