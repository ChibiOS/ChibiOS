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
 * @file    sb/host/sbposix.c
 * @brief   ARM SandBox host Posix API code.
 *
 * @addtogroup ARM_SANDBOX_HOSTAPI
 * @{
 */

#include "sb.h"

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)

#include <dirent.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t create_descriptor(sb_ioblock_t *iop,
                               vfs_node_c *np) {
  unsigned fd;

  for (fd = 0U; fd < SB_CFG_FD_NUM; fd++) {
    if (iop->vfs_nodes[fd] == NULL) {
      iop->vfs_nodes[fd]  = np;

      return (msg_t)fd;
    }
  }

  return CH_RET_EMFILE;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

int sb_posix_stat(const char *path, struct stat *statbuf) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  msg_t ret;
  vfs_stat_t vstat;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  if (!sb_is_valid_write_range(sbp, (void *)statbuf, sizeof (struct stat))) {
    return CH_RET_EFAULT;
  }

  ret = (int)vfsDrvStat(sbp->config->vfs_driver, path, &vstat);
  if (!CH_RET_IS_ERROR(ret)) {
    memset((void *)statbuf, 0, sizeof (struct stat));
    statbuf->st_mode  = (mode_t)vstat.mode;
    statbuf->st_size  = (off_t)vstat.size;
    statbuf->st_nlink = 1;
  }

  return ret;
}

int sb_posix_open(const char *path, int flags) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  vfs_node_c *np = NULL;
  msg_t ret;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  do {
    ret = vfsDrvOpen(sbp->config->vfs_driver, path, (unsigned)flags, &np);
    CH_BREAK_ON_ERROR(ret);

    ret = create_descriptor(&sbp->io, np);
    CH_BREAK_ON_ERROR(ret);

    return (int)ret;
  } while (false);

  if (np != NULL) {
    vfsClose(np);
  }

  return (int)ret;
}

int sb_posix_close(int fd) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  vfsClose(sbp->io.vfs_nodes[fd]);
  sbp->io.vfs_nodes[fd] = NULL;

  return CH_RET_SUCCESS;
}

int sb_posix_dup(int fd) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  vfs_node_c *np;
  msg_t ret;

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  /* Node associated to the existing file descriptor.*/
  np = sbp->io.vfs_nodes[fd];

  /* Adding the same node to the new descriptor with increased reference
     counter.*/
  ret = create_descriptor(&sbp->io, (vfs_node_c *)roAddRef(np));
  if (CH_RET_IS_ERROR(ret)) {
    /* In case of error removing the added reference.*/
    vfsClose(np);
  }

  return (int)ret;
}

int sb_posix_dup2(int oldfd, int newfd) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_existing_descriptor(&sbp->io, oldfd)) {
    return CH_RET_EBADF;
  }

  if (!sb_is_valid_descriptor(oldfd)) {
    return CH_RET_EBADF;
  }

  if (oldfd == newfd) {
    return CH_RET_EINVAL;
  }

  if (sbp->io.vfs_nodes[newfd] != NULL) {
    vfsClose(sbp->io.vfs_nodes[newfd]);
  }

  sbp->io.vfs_nodes[newfd] = (vfs_node_c *)roAddRef(sbp->io.vfs_nodes[oldfd]);

  return (int)newfd;
}

int sb_posix_fstat(int fd, struct stat *statbuf) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  msg_t ret;
  vfs_stat_t vstat;

  if (!sb_is_valid_write_range(sbp, (void *)statbuf, sizeof (struct stat))) {
    return CH_RET_EFAULT;
  }

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  ret = vfsGetNodeStat(sbp->io.vfs_nodes[fd], &vstat);
  if (!CH_RET_IS_ERROR(ret)) {
    memset((void *)statbuf, 0, sizeof (struct stat));
    statbuf->st_mode  = (mode_t)vstat.mode;
    statbuf->st_size  = (off_t)vstat.size;
    statbuf->st_nlink = 1;
  }

  return ret;
}

ssize_t sb_posix_read(int fd, void *buf, size_t count) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (VFS_MODE_S_ISDIR(sbp->io.vfs_nodes[fd]->mode)) {
    return CH_RET_EISDIR;
  }

  if (count == (size_t)0) {
    return 0;
  }

  if (!sb_is_valid_write_range(sbp, buf, count)) {
    return CH_RET_EFAULT;
  }

  return vfsReadFile((vfs_file_node_c *)sbp->io.vfs_nodes[fd], buf, count);
}

ssize_t sb_posix_write(int fd, const void *buf, size_t count) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (VFS_MODE_S_ISDIR(sbp->io.vfs_nodes[fd]->mode)) {
    return CH_RET_EISDIR;
  }

  if (count == (size_t)0) {
    return 0;
  }

  if (!sb_is_valid_read_range(sbp, buf, count)) {
    return CH_RET_EFAULT;
  }

  return vfsWriteFile((vfs_file_node_c *)sbp->io.vfs_nodes[fd], buf, count);
}

off_t sb_posix_lseek(int fd, off_t offset, int whence) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if ((whence != SEEK_SET) || (whence == SEEK_CUR) || (whence != SEEK_END)) {
    return CH_RET_EINVAL;
  }

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (VFS_MODE_S_ISDIR(sbp->io.vfs_nodes[fd]->mode)) {
    return CH_RET_EISDIR;
  }

  if (!VFS_MODE_S_ISREG(sbp->io.vfs_nodes[fd]->mode)) {
    return CH_RET_ESPIPE;
  }

  return vfsSetFilePosition((struct vfs_file_node *)sbp->io.vfs_nodes[fd],
                            offset,
                            whence);;
}

ssize_t sb_posix_getdents(int fd, void *buf, size_t count) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  vfs_direntry_info_t *dip;
  msg_t ret;

  if (!sb_is_valid_write_range(sbp, buf, count)) {
    return (ssize_t)CH_RET_EFAULT;
  }

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return (ssize_t)CH_RET_EBADF;
  }

  if (!VFS_MODE_S_ISDIR(sbp->io.vfs_nodes[fd]->mode)) {
    return (ssize_t)CH_RET_ENOTDIR;
  }

  dip = (vfs_direntry_info_t *)(void *)vfs_buffer_take();

  do {
    size_t n;
    struct dirent *dep = (struct dirent *)buf;

    ret = vfsReadDirectoryNext((vfs_directory_node_c *)sbp->io.vfs_nodes[fd], dip);
    if (ret <= 0) {
      /* Note, zero means no more directory entries available.*/
      break;
    }

    n = sizeof (struct dirent) + strlen(dip->name) + (size_t)1;
    if (count < n) {
      ret = CH_RET_EINVAL;
      break;
    }

    /* Copying data from VFS structure to the Posix one.*/
    dep->d_ino    = (ino_t)1; /* TODO */
    dep->d_reclen = n;
    dep->d_type   = IFTODT(sbp->io.vfs_nodes[fd]->mode);
    strcpy(dep->d_name, dip->name);

    ret = (msg_t)n;

  } while (false);

  vfs_buffer_release((char *)dip);

  return (ssize_t)ret;
}

int sb_posix_chdir(const char *path) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvChangeCurrentDirectory(sbp->config->vfs_driver, path);
}

int sb_posix_getcwd(char *buf, size_t size) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_valid_write_range(sbp, buf, size)) {
    return CH_RET_EFAULT;
  }

  /* Note, it does not return a pointer to the buffer as required by Posix,
     this has to be handled on the user-side library.*/
  return vfsDrvGetCurrentDirectory(sbp->config->vfs_driver, buf, size);
}

int sb_posix_unlink(const char *path) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvUnlink(sbp->config->vfs_driver, path);
}

int sb_posix_rename(const char *oldpath, const char *newpath) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (sb_check_string(sbp, (void *)oldpath, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  if (sb_check_string(sbp, (void *)newpath, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvRename(sbp->config->vfs_driver, oldpath, newpath);
}

int sb_posix_mkdir(const char *path, mode_t mode) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvMkdir(sbp->config->vfs_driver, path, (vfs_mode_t)mode);
}

int sb_posix_rmdir(const char *path) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvRmdir(sbp->config->vfs_driver, path);
}

#endif

/** @} */
