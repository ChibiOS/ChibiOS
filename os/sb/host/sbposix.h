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
  int sb_posix_open(sb_class_t *sbp, const char *path, int flags);
  int sb_posix_close(sb_class_t *sbp, int fd);
  int sb_posix_dup(sb_class_t *sbp, int fd);
  int sb_posix_dup2(sb_class_t *sbp, int oldfd, int newfd);
  int sb_posix_fstat(sb_class_t *sbp, int fd, struct stat *statbuf);
  ssize_t sb_posix_read(sb_class_t *sbp, int fd, void *buf, size_t count);
  ssize_t sb_posix_write(sb_class_t *sbp, int fd, const void *buf, size_t count);
  off_t sb_posix_lseek(sb_class_t *sbp, int fd, off_t offset, int whence);
  ssize_t sb_posix_getdents(sb_class_t *sbp, int fd, void *buf, size_t count);
  int sb_posix_chdir(sb_class_t *sbp, const char *path);
  int sb_posix_getcwd(sb_class_t *sbp, char *buf, size_t size);
  int sb_posix_unlink(sb_class_t *sbp, const char *path);
  int sb_posix_rename(sb_class_t *sbp, const char *oldpath, const char *newpath);
  int sb_posix_mkdir(sb_class_t *sbp, const char *path, mode_t mode);
  int sb_posix_rmdir(sb_class_t *sbp, const char *path);
  int sb_posix_stat(sb_class_t *sbp, const char *path, struct stat *statbuf);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

static inline bool sb_is_valid_descriptor(int fd) {

  return (fd >= 0) && (fd < SB_CFG_FD_NUM);
}

static inline bool sb_is_available_descriptor(sb_ioblock_t *iop, int fd) {

  return (fd >= 0) && (fd < SB_CFG_FD_NUM) && (iop->vfs_nodes[fd] == NULL);
}

static inline bool sb_is_existing_descriptor(sb_ioblock_t *iop, int fd) {

  return (fd >= 0) && (fd < SB_CFG_FD_NUM) && (iop->vfs_nodes[fd] != NULL);
}

#endif /* SB_CFG_ENABLE_VFS == TRUE */

#endif /* SBPOSIX_H */

/** @} */
