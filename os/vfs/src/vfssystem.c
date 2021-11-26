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
 * @file    vfs/src/chsystem.c
 * @brief   VFS system code.
 *
 * @addtogroup VFS_SYSTEM
 * @{
 */

#include <string.h>

#include "vfs.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   @p vfs_root_dir_node_t specific methods.
 */
#define __vfs_root_dir_node_methods                                         \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_root_dir_node_t specific data.
 */
#define __vfs_root_dir_node_data                                            \
  __vfs_directory_node_data                                                 \
  unsigned                              index;

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Global VFS state.
 */
vfs_system_t vfs;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   @p vfs_root_dir_node_t virtual methods table.
 */
struct vfs_root_dir_node_vmt {
  __vfs_root_dir_node_methods
};

/**
 * @brief   Type of a structure representing a root directory VFS node.
 */
typedef struct vfs_root_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_root_dir_node_vmt *vmt;
  __vfs_root_dir_node_data
} vfs_root_dir_node_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

msg_t match_driver(const char **pathp, vfs_driver_t **vdpp) {
  char fname[VFS_CFG_MAX_NAMELEN + 1];
  msg_t err;
  vfs_driver_t **pp;

  do {
    err = vfs_parse_match_separator(pathp);
    VFS_BREAK_ON_ERROR(err);

    err = vfs_parse_filename(pathp, fname);
    VFS_BREAK_ON_ERROR(err);

    /* Searching among registered drivers.*/
    pp = &vfs.drivers[0];
    while (pp < vfs.next_driver) {
      if (strncmp(fname, (*pp)->rootname, VFS_CFG_MAX_NAMELEN) == 0) {
        *vdpp = *pp;
        return VFS_RET_SUCCESS;
      }
    }

    err = VFS_RET_NO_DRIVER;
  }
  while (false);

  return err;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/


/**
 * @brief   VFS initialization.
 *
 * @special
 */
void vfsInit(void) {

  vfs.next_driver = &vfs.drivers[0];

#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  chMtxObjectInit(&vfs.mtx);
#else
  chSemObjectInit(&vfs.sem, (cnt_t)1);
#endif
}

/**
 * @brief   Registers a file system driver on VFS.
 *
 * @param[in] vdp       pointer to a @p vfs_driver_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsRegisterDriver(vfs_driver_t *vdp) {
  msg_t err;

  VFS_LOCK();

  if (vfs.next_driver >= &vfs.drivers[VFS_CFG_MAX_DRIVERS]) {
    err = VFS_RET_NO_RESOURCE;
  }
  else {
    *vfs.next_driver++ = vdp;
    err = VFS_RET_SUCCESS;
  }

  VFS_UNLOCK();

  return err;
}

/**
 * @brief   Opens a VFS directory.
 *
 * @param[in] path      absolute path of the directory to be opened
 * @param[out] vdnpp    pointer to the pointer to the instantiated
 *                      @p vfs_directory_node_t object
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsOpenDirectory(const char *path, vfs_directory_node_t **vdnpp) {
  msg_t err;
  vfs_driver_t *dp;

  do {
    err = match_driver(&path, &dp);
    VFS_BREAK_ON_ERROR(err);

    err = dp->vmt->open_dir((void *)dp, path, vdnpp);
  }
  while (false);

  return err;
}

/**
 * @brief   Releases a @p vfs_directory_node_t object.
 *
 * @param[in] vdnp      the pointer to the @p vfs_directory_node_t object
 *                      to be released
 *
 * @api
 */
void vfsCloseDirectory(vfs_directory_node_t *vdnp) {

  chDbgAssert(vdnp->refs > 0U, "zero count");

  vdnp->vmt->release((void *)vdnp);
}

/**
 * @brief   First directory entry.
 *
 * @param[in] vdnp      the pointer to the @p vfs_directory_node_t object
 * @param[out] nip      pointer to a @p vfs_node_info_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsReadDirectoryFirst(vfs_directory_node_t *vdnp,
                            vfs_node_info_t *nip) {

  chDbgAssert(vdnp->refs > 0U, "zero count");

  return vdnp->vmt->dir_first((void *)vdnp, nip);
}

/**
 * @brief   Next directory entry.
 *
 * @param[in] vdnp      the pointer to the @p vfs_directory_node_t object
 * @param[out] nip      pointer to a @p vfs_node_info_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsReadDirectoryNext(vfs_directory_node_t *vdnp,
                           vfs_node_info_t *nip) {

  chDbgAssert(vdnp->refs > 0U, "zero count");

  return vdnp->vmt->dir_next((void *)vdnp, nip);
}

/**
 * @brief   Opens a VFS file.
 *
 * @param[in] path      absolute path of the file to be opened
 * @param[out] vdnpp    pointer to the pointer to the instantiated
 *                      @p vfs_file_node_t object
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsOpenFile(const char *path, vfs_file_node_t **vfnpp) {
  msg_t err;
  vfs_driver_t *dp;

  do {
    err = match_driver(&path, &dp);
    VFS_BREAK_ON_ERROR(err);

    err = dp->vmt->open_file((void *)dp, path, vfnpp);
  }
  while (false);

  return err;
}

/**
 * @brief   Releases a @p vfs_file_node_t object.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 *                      to be released
 *
 * @api
 */
void vfsCloseFile(vfs_file_node_t *vfnp) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  vfnp->vmt->release((void *)vfnp);
}

/**
 * @brief   File node read.
 * @details The function reads data from a file node into a buffer.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @param[out] buf      pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The transferred number of bytes or an error.
 *
 * @api
 */
ssize_t vfsReadFile(vfs_file_node_t *vfnp, uint8_t *buf, size_t n) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  return vfnp->vmt->file_read((void *)vfnp, buf, n);
}

/**
 * @brief   File node write.
 * @details The function writes data from a buffer to a file node.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @param[out] buf      pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The transferred number of bytes or an error.
 *
 * @api
 */
ssize_t vfsWriteFile(vfs_file_node_t *vfnp, const uint8_t *buf, size_t n) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  return vfnp->vmt->file_write((void *)vfnp, buf, n);
}

/**
 * @brief   Changes the current file position.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @param[in] offset    new absolute position
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsSetFilePosition(vfs_file_node_t *vfnp, vfs_offset_t offset) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  return vfnp->vmt->file_setpos((void *)vfnp, offset);
}

/**
 * @brief   Returns the current file position.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @return              The current file position.
 *
 * @api
 */
vfs_offset_t vfsGetFilePosition(vfs_file_node_t *vfnp) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  return vfnp->vmt->file_getpos((void *)vfnp);
}

/**
 * @brief   Returns the current file size.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @return              The current file size.
 *
 * @api
 */
vfs_offset_t vfsGetFileSize(vfs_file_node_t *vfnp) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  return vfnp->vmt->file_getsize((void *)vfnp);
}

/** @} */
