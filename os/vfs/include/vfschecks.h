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
 * @file    vfs/include/vfschchecks.h
 * @brief   Configuration file checks header.
 *
 * @addtogroup VFS_CHECKS
 * @details This module performs a series of checks on configuration data,
 *          it is able to detect and reject obsolete or incomplete
 *          @p vfsconf.h files.
 * @{
 */

#ifndef VFSCHECKS_H
#define VFSCHECKS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/* Configuration file checks.*/
#if !defined(_CHIBIOS_VFS_CONF_)
#error "invalid configuration file"
#endif

#if !defined(_CHIBIOS_VFS_CONF_VER_1_0_)
#error "obsolete or unknown configuration file"
#endif

/* Configuration options checks.*/
#if !defined(VFS_CFG_MAX_DRIVERS)
#error "VFS_CFG_MAX_DRIVERS not defined in chconf.h"
#endif

#if (VFS_CFG_MAX_DRIVERS < 1) || (VFS_CFG_MAX_DRIVERS > 16)
#error "invalid value for VFS_CFG_MAX_DRIVERS"
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VFSCHECKS_H */

/** @} */
