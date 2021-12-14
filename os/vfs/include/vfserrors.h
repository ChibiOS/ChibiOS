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

#include <errno.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Error codes compatible with HAL and Posix
 * @{
 */
#define VFS_RET_SUCCESS         0               /* Success */
#define VFS_RET_TIMEOUT         -1              /* Timeout */
#define VFS_RET_EOF             -2              /* End-of-file */
#define VFS_RET_NOT_IMPLEMENTED -3              /* Not implemented functionality */
#define VFS_RET_INNER_ERROR     -4              /* Unspecified error */
#define VFS_RET_ENOENT          (-32 - ENOENT)  /* No such file or directory */
#define VFS_RET_EIO             (-32 - EIO)     /* I/O error */
#define VFS_RET_EBADF           (-32 - EBADF)   /* Bad file number */
#define VFS_RET_ENOMEM          (-32 - ENOMEM)  /* Not enough space */
#define VFS_RET_EACCES          (-32 - EACCES)  /* Permission denied */
#define VFS_RET_EEXIST          (-32 - EEXIST)  /* File exists */
#define VFS_RET_ENOTDIR         (-32 - ENOTDIR) /* Not a directory */
#define VFS_RET_EISDIR          (-32 - EISDIR)  /* Is a directory */
#define VFS_RET_EINVAL          (-32 - EINVAL)  /* Invalid argument */
#define VFS_RET_EMFILE          (-32 - EMFILE)  /* Too many open files in process */
#define VFS_RET_ENFILE          (-32 - ENFILE)  /* Too many open files in system */
#define VFS_RET_EFBIG           (-32 - EFBIG)   /* File too large */
#define VFS_RET_ENOSPC          (-32 - ENOSPC)  /* No space left on device */
#define VFS_RET_ESPIPE          (-32 - ESPIPE)  /* Illegal seek */
#define VFS_RET_EROFS           (-32 - EROFS)   /* Read-only file system */
#define VFS_RET_ENAMETOOLONG    (-32 - ENAMETOOLONG)/* File or path name too long */
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

#define VFS_BREAK_ON_ERROR(err) do {                                        \
  if ((err) < VFS_RET_SUCCESS) {                                            \
    break;                                                                  \
  }                                                                         \
} while (false)

#define VFS_RETURN_ON_ERROR(err) do {                                       \
  msg_t ret = (err);                                                        \
  if (ret < VFS_RET_SUCCESS) {                                              \
    return ret;                                                             \
  }                                                                         \
} while (false)

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
