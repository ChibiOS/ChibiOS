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
 * @addtogroup Initialization
 * @{
 */

#ifndef _CH_H_
#define _CH_H_

#define _CHIBIOS_RT_

#ifndef __DOXIGEN__
#ifndef _CHCONF_H_
#include <chconf.h>
#endif

#ifndef _CHTYPES_H_
#include <chtypes.h>
#endif

#ifndef _CHCORE_H_
#include <chcore.h>
#endif
#endif /* __DOXIGEN__ */

#ifndef _LISTS_H_
#include "delta.h"
#endif

#ifndef _LISTS_H_
#include "lists.h"
#endif

#ifndef _SCHEDULER_H_
#include "scheduler.h"
#endif

#ifndef _SEMAPHORES_H_
#include "semaphores.h"
#endif

#ifndef _EVENTS_H_
#include "events.h"
#endif

#ifndef _MESSAGES_H_
#include "messages.h"
#endif

#ifndef _THREADS_H_
#include "threads.h"
#endif

#ifndef _INLINE_H_
#include "inline.h"
#endif

#ifndef _SLEEP_H_
#include "sleep.h"
#endif

#ifndef _QUEUES_H_
#include "queues.h"
#endif

#ifndef _SERIAL_H_
#include "serial.h"
#endif

#ifndef _DEBUG_H_
#include "debug.h"
#endif

/*
 * Common values.
 */
#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        1
#endif
#ifndef NULL
#define NULL        0
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void chSysInit(void);
#ifdef __cplusplus
}
#endif

#endif /* _CH_H_ */

/** @} */
