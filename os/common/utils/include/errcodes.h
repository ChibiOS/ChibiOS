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
 * @file    errcodes.h
 * @brief   Errors handling header file.
 *
 * @addtogroup UTILS_ERRCODES
 * @{
 */

#ifndef ERRCODES_H
#define ERRCODES_H

#include <errno.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Error codes
 * @{
 */
#define CH_RET_SUCCESS          (msg_t)MSG_OK               /* Success */
#define CH_RET_TIMEOUT          (msg_t)MSG_TIMEOUT          /* Timeout */
#define CH_RET_INNER_ERROR      (msg_t)-3                   /* Unexpected condition */
/** @} */

/**
 * @name    Extra error codes mapped on Posix errors
 * @{
 */
#define CH_RET_ENOENT           CH_ENCODE_ERROR(ENOENT)     /* No such file or directory */
#define CH_RET_EIO              CH_ENCODE_ERROR(EIO)        /* I/O error */
#define CH_RET_EBADF            CH_ENCODE_ERROR(EBADF)      /* Bad file number */
#define CH_RET_ENOMEM           CH_ENCODE_ERROR(ENOMEM)     /* Not enough space */
#define CH_RET_EACCES           CH_ENCODE_ERROR(EACCES)     /* Permission denied */
#define CH_RET_EEXIST           CH_ENCODE_ERROR(EEXIST)     /* File exists */
#define CH_RET_ENOTDIR          CH_ENCODE_ERROR(ENOTDIR)    /* Not a directory */
#define CH_RET_EISDIR           CH_ENCODE_ERROR(EISDIR)     /* Is a directory */
#define CH_RET_EINVAL           CH_ENCODE_ERROR(EINVAL)     /* Invalid argument */
#define CH_RET_EMFILE           CH_ENCODE_ERROR(EMFILE)     /* Too many open files in process */
#define CH_RET_ENFILE           CH_ENCODE_ERROR(ENFILE)     /* Too many open files in system */
#define CH_RET_EFBIG            CH_ENCODE_ERROR(EFBIG)      /* File too large */
#define CH_RET_ENOSPC           CH_ENCODE_ERROR(ENOSPC)     /* No space left on device */
#define CH_RET_ESPIPE           CH_ENCODE_ERROR(ESPIPE)     /* Illegal seek */
#define CH_RET_EROFS            CH_ENCODE_ERROR(EROFS)      /* Read-only file system */
#define CH_RET_ERANGE           CH_ENCODE_ERROR(ERANGE)     /* Result too large */
#define CH_RET_ENAMETOOLONG     CH_ENCODE_ERROR(ENAMETOOLONG)/* File or path name too long */
#define CH_RET_ENOSYS           CH_ENCODE_ERROR(ENOSYS)     /* Syscall not implemented */
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
#define CH_ERRORS_MASK              (msg_t)0xFF
#define CH_ENCODE_ERROR(posixerr)   (~CH_ERRORS_MASK | (msg_t)(posixerr))
#define CH_DECODE_ERROR(err)        ((msg_t)(err) & CH_ERRORS_MASK)
#define CH_IS_ERROR(x)              (((msg_t)(x) & ~CH_ERRORS_MASK) == ~CH_ERRORS_MASK)

#define CH_BREAK_ON_ERROR(err)                                              \
  if (CH_IS_ERROR(err)) break

#define CH_RETURN_ON_ERROR(err) do {                                        \
  msg_t __ret = (err);                                                      \
  if (CH_IS_ERROR(__ret)) {                                                 \
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

#endif /* ERRCODES_H */

/** @} */
