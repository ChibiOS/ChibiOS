/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    ch.h
 * @brief   ChibiOS/RT main include file.
 * @details This header includes all the required kernel headers so it is the
 *          only kernel header you usually want to include in your application.
 *
 * @addtogroup kernel_info
 * @details Kernel related info.
 * @{
 */

#ifndef _CH_H_
#define _CH_H_

/**
 * @brief   ChibiOS/RT identification macro.
 */
#define _CHIBIOS_RT_

/**
 * @brief   Kernel version string.
 */
#define CH_KERNEL_VERSION       "3.0.0dev"

/**
 * @name    Kernel version
 * @{
 */
/**
 * @brief   Kernel version major number.
 */
#define CH_KERNEL_MAJOR         3

/**
 * @brief   Kernel version minor number.
 */
#define CH_KERNEL_MINOR         0

/**
 * @brief   Kernel version patch number.
 */
#define CH_KERNEL_PATCH         0
/** @} */

/**
 * @name    Common constants
 */
/**
 * @brief   Generic 'false' boolean constant.
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                   0
#endif

/**
 * @brief   Generic 'true' boolean constant.
 */
#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                    (!FALSE)
#endif

/**
 * @brief   Generic success constant.
 * @details This constant is functionally equivalent to @p false but more
 *          readable, it can be used as return value of all those functions
 *          returning a @p bool as a status indicator.
 */
#if !defined(CH_SUCCESS) || defined(__DOXYGEN__)
#define CH_SUCCESS              false
#endif

/**
 * @brief   Generic failure constant.
 * @details This constant is functionally equivalent to @p true but more
 *          readable, it can be used as return value of all those functions
 *          returning a @p bool as a status indicator.
 */
#if !defined(CH_FAILED) || defined(__DOXYGEN__)
#define CH_FAILED               true
#endif
/** @} */

/* Forward declarations.*/
typedef struct thread thread_t;
typedef struct virtual_timer virtual_timer_t;

/* This function is special, it must be accessible from all modules.*/
#ifdef __cplusplus
extern "C" {
#endif
  void chSysHalt(const char *reason);
#ifdef __cplusplus
}
#endif

/* Inclusion of all the kernel sub-headers.*/
#include "chconf.h"
#include "chtypes.h"
#include "chdebug.h"
#include "chcore.h"
#include "chtm.h"
#include "chstats.h"
#include "chsys.h"
#include "chglobal.h"
#include "chvt.h"
#include "chlists.h"
#include "chschd.h"
#include "chthreads.h"
#include "chregistry.h"
#include "chsem.h"
#include "chbsem.h"
#include "chmtx.h"
#include "chcond.h"
#include "chevents.h"
#include "chmsg.h"
#include "chmboxes.h"
#include "chmemcore.h"
#include "chheap.h"
#include "chmempools.h"
#include "chdynamic.h"
#include "chqueues.h"
#include "chstreams.h"

#endif /* _CH_H_ */

/** @} */
