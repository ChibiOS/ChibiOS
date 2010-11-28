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
#define CH_KERNEL_VERSION       "2.0.8"

/**
 * @brief   Kernel version major number.
 */
#define CH_KERNEL_MAJOR         2

/**
 * @brief   Kernel version minor number.
 */
#define CH_KERNEL_MINOR         0

/**
 * @brief   Kernel version patch number.
 */
#define CH_KERNEL_PATCH         8

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
#include "chlists.h"
#include "chcore.h"
#include "chsys.h"
#include "chvt.h"
#include "chschd.h"
#include "chsem.h"
#include "chmtx.h"
#include "chcond.h"
#include "chevents.h"
#include "chmsg.h"
#include "chmboxes.h"
#include "chmemcore.h"
#include "chheap.h"
#include "chmempools.h"
#include "chthreads.h"
#include "chregistry.h"
#include "chinline.h"
#include "chqueues.h"
#include "chstreams.h"
#include "chioch.h"
#include "chdebug.h"

#endif /* _CH_H_ */

/** @} */
