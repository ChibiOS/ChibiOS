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
 * @file    sb/host/sbposix.h
 * @brief   ARM SandBox host Posix API macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOSTAPI
 * @{
 */

#ifndef SBPOSIX_H
#define SBPOSIX_H

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
#include "vfs.h"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

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

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  int sb_posix_open(const char *path, int flags);
  int sb_posix_close(int fd);
  int sb_posix_dup(int fd);
  int sb_posix_dup2(int oldfd, int newfd);
  int sb_posix_fstat(int fd, struct stat *statbuf);
  ssize_t sb_posix_read(int fd, void *buf, size_t count);
  ssize_t sb_posix_write(int fd, const void *buf, size_t count);
  off_t sb_posix_lseek(int fd, off_t offset, int whence);
#if SB_CFG_ENABLE_VFS == TRUE
  void sbPosixRegisterDescriptor(sb_class_t *sbp, int fd, vfs_node_c *np);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBPOSIX_H */

/** @} */
