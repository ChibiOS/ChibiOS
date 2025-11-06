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

static int sb_io_stat(sb_class_t *sbp, const char *path, struct stat *statbuf) {
  msg_t ret;
  vfs_stat_t vstat;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  if (!sb_is_valid_write_range(sbp, (void *)statbuf, sizeof (struct stat))) {
    return CH_RET_EFAULT;
  }

  ret = (int)vfsDrvStat(sbp->io.vfs_driver, path, &vstat);
  if (!CH_RET_IS_ERROR(ret)) {
    memset((void *)statbuf, 0, sizeof (struct stat));
    statbuf->st_mode  = (mode_t)vstat.mode;
    statbuf->st_size  = (off_t)vstat.size;
    statbuf->st_nlink = 1;
    /* TODO st_blocks, st_blksize, st_ino, timespecs.*/
  }

  return ret;
}

static int sb_io_open(sb_class_t *sbp, const char *path, int flags) {
  vfs_node_c *np = NULL;
  msg_t ret;

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  do {
    ret = vfsDrvOpen(sbp->io.vfs_driver, path, (unsigned)flags, &np);
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

static int sb_io_close(sb_class_t *sbp, int fd) {

  if (!sb_is_existing_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  vfsClose(sbp->io.vfs_nodes[fd]);
  sbp->io.vfs_nodes[fd] = NULL;

  return CH_RET_SUCCESS;
}

static int sb_io_dup(sb_class_t *sbp, int fd) {
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

static int sb_io_dup2(sb_class_t *sbp, int oldfd, int newfd) {

  if (!sb_is_existing_descriptor(&sbp->io, oldfd)) {
    return CH_RET_EBADF;
  }

  if (!sb_is_valid_descriptor(oldfd)) {
    return CH_RET_EBADF;
  }

  if (!sb_is_valid_descriptor(newfd)) {
    return CH_RET_EBADF;
  }

  if (oldfd == newfd) {
    return (int)newfd;
  }

  if (sbp->io.vfs_nodes[newfd] != NULL) {
    vfsClose(sbp->io.vfs_nodes[newfd]);
  }

  sbp->io.vfs_nodes[newfd] = (vfs_node_c *)roAddRef(sbp->io.vfs_nodes[oldfd]);

  return (int)newfd;
}

static int sb_io_fstat(sb_class_t *sbp, int fd, struct stat *statbuf) {
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

static ssize_t sb_io_read(sb_class_t *sbp, int fd, void *buf, size_t count) {

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

static ssize_t sb_io_write(sb_class_t *sbp, int fd, const void *buf, size_t count) {

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

static off_t sb_io_lseek(sb_class_t *sbp, int fd, off_t offset, int whence) {

  if ((whence != SEEK_SET) && (whence != SEEK_CUR) && (whence != SEEK_END)) {
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
                            whence);
}

static ssize_t sb_io_getdents(sb_class_t *sbp, int fd, void *buf, size_t count) {
  vfs_shared_buffer_t *shbuf;
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

  shbuf = vfs_buffer_take_wait();
  dip = (vfs_direntry_info_t *)(void *)shbuf->buf;

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

  vfs_buffer_release(shbuf);

  return (ssize_t)ret;
}

static int sb_io_chdir(sb_class_t *sbp, const char *path) {

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvChangeCurrentDirectory(sbp->io.vfs_driver, path);
}

static int sb_io_getcwd(sb_class_t *sbp, char *buf, size_t size) {

  if (!sb_is_valid_write_range(sbp, buf, size)) {
    return CH_RET_EFAULT;
  }

  /* Note, it does not return a pointer to the buffer as required by Posix,
     this has to be handled on the user-side library.*/
  return vfsDrvGetCurrentDirectory(sbp->io.vfs_driver, buf, size);
}

static int sb_io_unlink(sb_class_t *sbp, const char *path) {

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvUnlink(sbp->io.vfs_driver, path);
}

static int sb_io_rename(sb_class_t *sbp, const char *oldpath, const char *newpath) {

  if (sb_check_string(sbp, (void *)oldpath, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  if (sb_check_string(sbp, (void *)newpath, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvRename(sbp->io.vfs_driver, oldpath, newpath);
}

static int sb_io_mkdir(sb_class_t *sbp, const char *path, mode_t mode) {

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvMkdir(sbp->io.vfs_driver, path, (vfs_mode_t)mode);
}

static int sb_io_rmdir(sb_class_t *sbp, const char *path) {

  if (sb_check_string(sbp, (void *)path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
    return CH_RET_EFAULT;
  }

  return (int)vfsDrvRmdir(sbp->io.vfs_driver, path);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void __sb_io_cleanup(sb_class_t *sbp) {
  unsigned fd;

  /* Closing all file descriptors.*/
  for (fd = 0U; fd < SB_CFG_FD_NUM; fd++) {
    if (sbp->io.vfs_nodes[fd] != NULL) {
      roRelease(sbp->io.vfs_nodes[fd]);
      sbp->io.vfs_nodes[fd] = NULL;
    }
  }
}

void sb_sysc_stdio(sb_class_t *sbp, struct port_extctx *ectxp) {

  /* VFS support could be enabled but this specific sandbox could not have
     one associated to it.*/
  if (sbp->io.vfs_driver == NULL) {
    ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
    return;
  }

  switch (ectxp->r0) {
  case SB_POSIX_OPEN:
    ectxp->r0 = (uint32_t)sb_io_open(sbp,
                                     (const char *)ectxp->r1,
                                     (int)ectxp->r2);
    break;
  case SB_POSIX_CLOSE:
    ectxp->r0 = (uint32_t)sb_io_close(sbp, (int)ectxp->r1);
    break;
  case SB_POSIX_DUP:
    ectxp->r0 = (uint32_t)sb_io_dup(sbp, (int)ectxp->r1);
    break;
  case SB_POSIX_DUP2:
    ectxp->r0 = (uint32_t)sb_io_dup2(sbp,
                                     (int)ectxp->r1,
                                     (int)ectxp->r2);
    break;
  case SB_POSIX_FSTAT:
    ectxp->r0 = (uint32_t)sb_io_fstat(sbp,
                                      (int)ectxp->r1,
                                      (struct stat *)ectxp->r2);
    break;
  case SB_POSIX_READ:
    ectxp->r0 = (uint32_t)sb_io_read(sbp,
                                     (int)ectxp->r1,
                                     (void *)ectxp->r2,
                                     (size_t)ectxp->r3);
    break;
  case SB_POSIX_WRITE:
    ectxp->r0 = (uint32_t)sb_io_write(sbp,
                                      (int)ectxp->r1,
                                      (const void *)ectxp->r2,
                                      (size_t)ectxp->r3);
    break;
  case SB_POSIX_LSEEK:
    ectxp->r0 = (uint32_t)sb_io_lseek(sbp,
                                      (int)ectxp->r1,
                                      (off_t)ectxp->r2,
                                      (int)ectxp->r3);
    break;
  case SB_POSIX_GETDENTS:
    ectxp->r0 = (uint32_t)sb_io_getdents(sbp,
                                         (int)ectxp->r1,
                                         (void *)ectxp->r2,
                                         (size_t)ectxp->r3);
    break;
  case SB_POSIX_CHDIR:
    ectxp->r0 = (uint32_t)sb_io_chdir(sbp, (const char *)ectxp->r1);
    break;
  case SB_POSIX_GETCWD:
    ectxp->r0 = (uint32_t)sb_io_getcwd(sbp,
                                       (char *)ectxp->r1,
                                       (size_t)ectxp->r2);
    break;
  case SB_POSIX_UNLINK:
    ectxp->r0 = (uint32_t)sb_io_unlink(sbp, (const char *)ectxp->r1);
    break;
  case SB_POSIX_RENAME:
    ectxp->r0 = (uint32_t)sb_io_rename(sbp,
                                       (const char *)ectxp->r1,
                                       (const char *)ectxp->r2);
    break;
  case SB_POSIX_MKDIR:
    ectxp->r0 = (uint32_t)sb_io_mkdir(sbp,
                                      (const char *)ectxp->r1,
                                      (mode_t)ectxp->r2);
    break;
  case SB_POSIX_RMDIR:
    ectxp->r0 = (uint32_t)sb_io_rmdir(sbp, (const char *)ectxp->r1);
    break;
  case SB_POSIX_STAT:
    ectxp->r0 = (uint32_t)sb_io_stat(sbp,
                                     (const char *)ectxp->r1,
                                     (struct stat *)ectxp->r2);
    break;
  default:
    ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
    break;
  }
}

#endif /* SB_CFG_ENABLE_VFS == TRUE */

/** @} */
