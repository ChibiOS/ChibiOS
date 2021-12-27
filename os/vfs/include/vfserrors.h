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
 * @name    VFS error codes
 * @{
 */
#define VFS_RET_SUCCESS         (msg_t)MSG_OK       /* Success */
#define VFS_RET_TIMEOUT         (msg_t)MSG_TIMEOUT  /* Timeout */
#define VFS_RET_EOF             (msg_t)-3           /* End-of-file */
#define VFS_RET_INNER_ERROR     (msg_t)-4           /* Unexpected condition */
/** @} */

/**
 * @name    Error codes compatible with Posix
 * @{
 */
#define VFS_RET_ENOENT          VFS_ERROR(ENOENT)   /* No such file or directory */
#define VFS_RET_EIO             VFS_ERROR(EIO)      /* I/O error */
#define VFS_RET_EBADF           VFS_ERROR(EBADF)    /* Bad file number */
#define VFS_RET_ENOMEM          VFS_ERROR(ENOMEM)   /* Not enough space */
#define VFS_RET_EACCES          VFS_ERROR(EACCES)   /* Permission denied */
#define VFS_RET_EEXIST          VFS_ERROR(EEXIST)   /* File exists */
#define VFS_RET_ENOTDIR         VFS_ERROR(ENOTDIR)  /* Not a directory */
#define VFS_RET_EISDIR          VFS_ERROR(EISDIR)   /* Is a directory */
#define VFS_RET_EINVAL          VFS_ERROR(EINVAL)   /* Invalid argument */
#define VFS_RET_EMFILE          VFS_ERROR(EMFILE)   /* Too many open files in process */
#define VFS_RET_ENFILE          VFS_ERROR(ENFILE)   /* Too many open files in system */
#define VFS_RET_EFBIG           VFS_ERROR(EFBIG)    /* File too large */
#define VFS_RET_ENOSPC          VFS_ERROR(ENOSPC)   /* No space left on device */
#define VFS_RET_ESPIPE          VFS_ERROR(ESPIPE)   /* Illegal seek */
#define VFS_RET_EROFS           VFS_ERROR(EROFS)    /* Read-only file system */
#define VFS_RET_ERANGE          VFS_ERROR(ERANGE)   /* Result too large */
#define VFS_RET_ENAMETOOLONG    VFS_ERROR(ENAMETOOLONG)/* File or path name too long */
#define VFS_RET_ENOSYS          VFS_ERROR(ENOSYS)
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

/**
 * @name    Errors handling macros
 * @{
 */
#define VFS_ERRORS_MASK         (msg_t)((msg_t)-1 & ~(msg_t)0xFF)
#define VFS_ERROR(posixerr)     (VFS_ERRORS_MASK | (msg_t)(posixerr))
#define VFS_IS_ERROR(x)         (((msg_t)(x) & VFS_ERRORS_MASK) == VFS_ERRORS_MASK)

#define VFS_BREAK_ON_ERROR(err)                                             \
  if (VFS_IS_ERROR(err)) break

#define VFS_RETURN_ON_ERROR(err) do {                                       \
  msg_t __ret = (err);                                                      \
  if (VFS_IS_ERROR(__ret)) {                                                \
    return __ret;                                                           \
  }                                                                         \
} while (false)
/** @} */

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
