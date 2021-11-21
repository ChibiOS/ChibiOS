/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    vfs/include/vfs.h
 * @brief   VFS main header file.
 * @details Main header of the ChibiOS Virtual File System.
 *
 * @addtogroup VFS
 * @{
 */

#ifndef VFS_H
#define VFS_H

/**
 * @brief   ChibiOS/VFS identification macro.
 */
#define __CHIBIOS_VFS__

/**
 * @brief   Stable release flag.
 */
#define CH_VFS_STABLE           0

/**
 * @name    ChibiOS/VFS version identification
 * @{
 */
/**
 * @brief   Kernel version string.
 */
#define CH_VFS_VERSION          "1.0.0"

/**
 * @brief   Kernel version major number.
 */
#define CH_VFS_MAJOR            1

/**
 * @brief   Kernel version minor number.
 */
#define CH_VFS_MINOR            0

/**
 * @brief   Kernel version patch number.
 */
#define CH_VFS_PATCH            0
/** @} */

#include <stddef.h>

/* Dependencies.*/
#include "osal.h"
#include "hal_objects.h"

/* License.*/
#include "chlicense.h"

/* Configuration headers, checks and licensing restrictions.*/
#include "vfsconf.h"
#include "vfschecks.h"

/* Base VFS headers.*/
#include "vfserrors.h"
#include "vfsnodes.h"
#include "vfsdrivers.h"
#include "vfssystem.h"

#endif /* VFS_H */

/** @} */
