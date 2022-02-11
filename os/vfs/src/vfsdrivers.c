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
 * @file    vfs/src/vfsdrivers.c
 * @brief   VFS drivers code.
 *
 * @addtogroup VFS_DRIVERS
 * @{
 */

#include "vfs.h"

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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Opens a VFS file or directory.
 *
 * @param[in] drvp      Pointer to the @p vfs_driver_c object.
 * @param[in] path      Absolute path of the node to be opened.
 * @param[in] flags     Open flags.
 * @param[out] vnpp     Pointer to the pointer to the instantiated
 *                      @p vfs_node_c object.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsDrvOpen(vfs_driver_c *drvp,
                 const char *path,
                 int flags,
                 vfs_node_c **vnpp) {
  msg_t ret;

  /* Attempting to open as file.*/
  ret = vfsDrvOpenFile(drvp, path, flags, (vfs_file_node_c **)vnpp);
  if (ret == CH_RET_EISDIR) {
    if ((flags & VO_ACCMODE) != VO_RDONLY) {
      ret = CH_RET_EISDIR;
    }
    else {
      ret = vfsDrvOpenDirectory(drvp, path, (vfs_directory_node_c **)vnpp);
    }
  }

  return ret;
}

msg_t drv_stat_unimpl(void *instance, vfs_stat_t *sp) {

  (void)instance;
  (void)sp;

  return CH_RET_ENOSYS;
}

msg_t drv_unlink_unimpl(void *instance, const char *path) {

  (void)instance;
  (void)path;

  return CH_RET_ENOSYS;
}

msg_t drv_rename_unimpl(void *instance,
                        const char *oldpath,
                        const char *newpath) {

  (void)instance;
  (void)oldpath;
  (void)newpath;

  return CH_RET_ENOSYS;
}

msg_t drv_mkdir_unimpl(void *instance,
                       const char *path,
                       vfs_mode_t mode) {

  (void)instance;
  (void)path;
  (void)mode;

  return CH_RET_ENOSYS;
}

msg_t drv_rmdir_unimpl(void *instance, const char *path) {

  (void)instance;
  (void)path;

  return CH_RET_ENOSYS;
}

/** @} */
