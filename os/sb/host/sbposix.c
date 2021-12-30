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

#include "ch.h"
#include "sb.h"

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

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
static msg_t create_descriptor(sb_ioblock_t *iop,
                               vfs_node_c *np,
                               uint8_t attributes) {
  unsigned fd;

  for (fd = 0U; fd < SB_CFG_FD_NUM; fd++) {
    if (iop->vfs_nodes[fd] == NULL) {
      iop->vfs_nodes[fd]  = np;
      iop->attributes[fd] = attributes;

      return (msg_t)fd;
    }
  }

  return CH_RET_EMFILE;
}

static bool is_valid_descriptor(sb_ioblock_t *iop, int fd) {

  return (fd >= 0) && (fd < SB_CFG_FD_NUM) && (iop->vfs_nodes[fd] != NULL);
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
int sb_posix_open(const char *path, int flags) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  vfs_file_node_c *fnp = NULL;
  msg_t ret;

  if (!sb_is_valid_string_range(sbp, (void *)path, VFS_CFG_PATHLEN_MAX)) {
    return CH_RET_EFAULT;
  }

  do {
    ret = vfsDrvOpenFile(sbp->config->vfs_driver, path,
                         (unsigned)flags, &fnp);
    CH_BREAK_ON_ERROR(ret);

    ret = create_descriptor(&sbp->io, (vfs_node_c *)fnp, 0);
    CH_BREAK_ON_ERROR(ret);

    return (int)ret;
  } while (true);

  if (fnp != NULL) {
    vfsCloseFile(fnp);
  }

  return (int)ret;
}

int sb_posix_close(int fd) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!is_valid_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (sbp->io.attributes[fd] == 0) {
    vfsCloseFile((vfs_file_node_c *)sbp->io.vfs_nodes[fd]);
  }
  else {
    vfsCloseDirectory((vfs_directory_node_c *)sbp->io.vfs_nodes[fd]);
  }
  sbp->io.vfs_nodes[fd] = NULL;

  return CH_RET_SUCCESS;
}

ssize_t sb_posix_read(int fd, void *buf, size_t count) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_valid_write_range(sbp, (void *)buf, count)) {
    return CH_RET_EFAULT;
  }

  if (!is_valid_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (sbp->io.attributes[fd] != 0) {
    return CH_RET_EISDIR;
  }

  return vfsReadFile((vfs_file_node_c *)sbp->io.vfs_nodes[fd], buf, count);
}

ssize_t sb_posix_write(int fd, const void *buf, size_t count) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_valid_read_range(sbp, (void *)buf, count)) {
    return CH_RET_EFAULT;
  }

  if (!is_valid_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (sbp->io.attributes[fd] != 0) {
    return CH_RET_EISDIR;
  }

  return vfsWriteFile((vfs_file_node_c *)sbp->io.vfs_nodes[fd], buf, count);
}

off_t sb_posix_lseek(int fd, off_t offset, int whence) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  msg_t ret;
  vfs_file_stat_t stat;

  if ((whence != SEEK_SET) || (whence == SEEK_CUR) || (whence != SEEK_END)) {
    return CH_RET_EINVAL;
  }

  if (!is_valid_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (sbp->io.attributes[fd] != 0) {
    return CH_RET_EISDIR;
  }

  ret = vfsGetFileStat((struct vfs_file_node *)sbp->io.vfs_nodes[fd], &stat);
  CH_RETURN_ON_ERROR(ret);

  if ((stat.attr & VFS_NODE_ATTR_ISSTREAM) != 0U) {
    return CH_RET_ESPIPE;
  }

  return vfsSetFilePosition((struct vfs_file_node *)sbp->io.vfs_nodes[fd],
                            offset,
                            whence);;
}

void sbPosixRegisterFileDescriptor(sb_class_t *sbp,
                                   int fd,
                                   vfs_file_node_c *fnp) {

  chDbgAssert((fd >= 0) && (fd < SB_CFG_FD_NUM) &&
              sbp->io.vfs_nodes[fd] == NULL,
              "invalid file descriptor");

  sbp->io.vfs_nodes[fd]  = (vfs_node_c *)fnp;
  sbp->io.attributes[fd] = 0;
}

#else /* Fallbacks for when there is no VFS.*/
uint32_t sb_posix_open(const char *pathname, uint32_t flags) {

  (void)pathname;
  (void)flags;

  return SB_ERR_ENOENT;
}

uint32_t sb_posix_close(uint32_t fd) {

  if ((fd == 0U) || (fd == 1U) || (fd == 2U)) {

    return SB_ERR_NOERROR;
  }

  return SB_ERR_EBADFD;
}

uint32_t sb_posix_read(uint32_t fd, uint8_t *buf, size_t count) {
  sb_class_t *sbcp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_valid_write_range(sbcp, (void *)buf, count)) {
    return SB_ERR_EFAULT;
  }

  if (fd == 0U) {
    SandboxStream *ssp = sbcp->config->stdin_stream;

    if ((count == 0U) || (ssp == NULL)) {
      return 0U;
    }

    return (uint32_t)ssp->vmt->read((void *)ssp, buf, count);
  }

  return SB_ERR_EBADFD;
}

uint32_t sb_posix_write(uint32_t fd, const uint8_t *buf, size_t count) {
  sb_class_t *sbcp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (!sb_is_valid_read_range(sbcp, (const void *)buf, count)) {
    return SB_ERR_EFAULT;
  }

  if (fd == 1U) {
    SandboxStream *ssp = sbcp->config->stdout_stream;

    if ((count == 0U) || (ssp == NULL)) {
      return 0U;
    }

    return (uint32_t)ssp->vmt->write((void *)ssp, buf, count);
  }

  if (fd == 2U) {
    SandboxStream *ssp = sbcp->config->stderr_stream;

    if ((count == 0U) || (ssp == NULL)) {
      return 0U;
    }

    return (uint32_t)ssp->vmt->write((void *)ssp, buf, count);
  }

  return SB_ERR_EBADFD;
}

uint32_t sb_posix_lseek(uint32_t fd, uint32_t offset, uint32_t whence) {

  (void)offset;
  (void)whence;

  if ((fd == 0U) || (fd == 1U) || (fd == 2U)) {

    return SB_ERR_ESPIPE;
  }

  return SB_ERR_EBADFD;
}
#endif

/** @} */
