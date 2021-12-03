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
 * @file    vfs/include/vfserrors.h
 * @brief   VFS erors header file.
 *
 * @addtogroup VFS_ERRORS
 * @{
 */

#ifndef VFSERRORS_H
#define VFSERRORS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Error codes compatible with HAL streams
 * @{
 */
#define VFS_RET_SUCCESS         0
#define VFS_RET_TIMEOUT         -1
#define VFS_RET_EOF             -2
#define VFS_RET_NOT_IMPLEMENTED -3
#define VFS_RET_NO_RESOURCE     -4
#define VFS_RET_NO_DRIVER       -5
#define VFS_RET_INVALID_PATH    -6
#define VFS_RET_NOT_FOUND       -7
#define VFS_RET_EXIST           -8
#define VFS_RET_ACCESS_DENIED   -9
#define VFS_RET_MEDIA_ERROR     -10
#define VFS_RET_INNER_ERROR     -11
#define VFS_RET_INVALID_MODE    -12
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#define VFS_BREAK_ON_ERROR(err)                                             \
  if ((err) < VFS_RET_SUCCESS) break

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

#endif /* VFSERRORS_H */

/** @} */
