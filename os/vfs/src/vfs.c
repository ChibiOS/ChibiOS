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
 * @file    vfs/src/vfs.c
 * @brief   VFS API code.
 *
 * @addtogroup VFS
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
 * @brief   VFS initialization.
 *
 * @init
 */
void vfsInit(void) {

  /* Shared buffers manager initialization.*/
  __vfs_buffers_init();

#if VFS_CFG_ENABLE_DRV_OVERLAY == TRUE
  __drv_overlay_init();
#endif

#if VFS_CFG_ENABLE_DRV_STREAMS == TRUE
  __drv_streams_init();
#endif

#if VFS_CFG_ENABLE_DRV_FATFS == TRUE
  __drv_fatfs_init();
#endif
}

/**
 * @brief   Changes the current VFS directory.
 *
 * @param[in] path      Path of the new current directory.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsChangeCurrentDirectory(const char *path) {

  return vfsDrvChangeCurrentDirectory(vfs_root, path);
}

/**
 * @brief   Returns the current VFS directory.
 *
 * @param[out] buf      Buffer for the path string.
 * @param[in] size      Size of the buffer.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsGetCurrentDirectory(char *buf, size_t size) {

  return vfsDrvGetCurrentDirectory(vfs_root, buf, size);
}

/**
 * @brief   Opens a VFS directory.
 *
 * @param[in] path      Absolute path of the directory to be opened.
 * @param[out] vdnpp    Pointer to the pointer to the instantiated.
 *                      @p vfs_directory_node_c object
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsOpenDirectory(const char *path,
                       vfs_directory_node_c **vdnpp) {

  return vfsDrvOpenDirectory(vfs_root, path, vdnpp);
}

/**
 * @brief   Releases a @p vfs_directory_node_c object.
 *
 * @param[in] vdnp      Pointer to the @p vfs_directory_node_c object
 *                      to be released.
 *
 * @api
 */
void vfsCloseDirectory(vfs_directory_node_c *vdnp) {

  vdnp->vmt->release((void *)vdnp);
}

/**
 * @brief   First directory entry.
 *
 * @param[in] vdnp      Pointer to the @p vfs_directory_node_c object.
 * @param[out] dip      Pointer to a @p vfs_direntry_info_t structure.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsReadDirectoryFirst(vfs_directory_node_c *vdnp,
                            vfs_direntry_info_t *dip) {

  chDbgAssert(vdnp->references > 0U, "zero count");

  return vdnp->vmt->dir_first((void *)vdnp, dip);
}

/**
 * @brief   Next directory entry.
 *
 * @param[in] vdnp      Pointer to the @p vfs_directory_node_c object..
 * @param[out] dip      Pointer to a @p vfs_direntry_info_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsReadDirectoryNext(vfs_directory_node_c *vdnp,
                           vfs_direntry_info_t *dip) {

  chDbgAssert(vdnp->references > 0U, "zero count");

  return vdnp->vmt->dir_next((void *)vdnp, dip);
}

/**
 * @brief   Opens a VFS file.
 *
 * @param[in] path      Path of the file to be opened.
 * @param[in] mode      File open mode.
 * @param[out] vdnpp    Pointer to the pointer to the instantiated
 *                      @p vfs_file_node_c object.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsOpenFile(const char *path,
                  unsigned mode,
                  vfs_file_node_c **vfnpp) {

  return vfsDrvOpenFile(vfs_root, path, mode, vfnpp);
}

/**
 * @brief   Releases a @p vfs_file_node_c object.
 *
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object.
 *                      to be released
 *
 * @api
 */
void vfsCloseFile(vfs_file_node_c *vfnp) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  vfnp->vmt->release((void *)vfnp);
}

/**
 * @brief   File node read.
 * @details The function reads data from a file node into a buffer.
 *
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object.
 * @param[out] buf      Pointer to the data buffer.
 * @param[in] n         Maximum amount of data to be transferred.
 * @return              The transferred number of bytes or an error.
 *
 * @api
 */
ssize_t vfsReadFile(vfs_file_node_c *vfnp, uint8_t *buf, size_t n) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  return vfnp->vmt->file_read((void *)vfnp, buf, n);
}

/**
 * @brief   File node write.
 * @details The function writes data from a buffer to a file node.
 *
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object.
 * @param[out] buf      Pointer to the data buffer.
 * @param[in] n         Maximum amount of data to be transferred.
 * @return              The transferred number of bytes or an error.
 *
 * @api
 */
ssize_t vfsWriteFile(vfs_file_node_c *vfnp, const uint8_t *buf, size_t n) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  return vfnp->vmt->file_write((void *)vfnp, buf, n);
}

/**
 * @brief   Changes the current file position.
 *
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object.
 * @param[in] offset    New absolute position.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsSetFilePosition(vfs_file_node_c *vfnp, vfs_offset_t offset) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  return vfnp->vmt->file_setpos((void *)vfnp, offset);
}

/**
 * @brief   Returns the current file position.
 *
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object.
 * @return              The current file position.
 *
 * @api
 */
vfs_offset_t vfsGetFilePosition(vfs_file_node_c *vfnp) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  return vfnp->vmt->file_getpos((void *)vfnp);
}

/**
 * @brief   Returns the current file size.
 *.
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object
 * @param[out] nsp      Pointer to a @p vfs_node_stat_t structure.
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsGetFileStat(vfs_file_node_c *vfnp, vfs_node_stat_t *nsp) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  return vfnp->vmt->file_getstat((void *)vfnp, nsp);
}

/**
 * @brief   Returns the inner stream associated to the file.
 *
 * @param[in] vfnp      Pointer to the @p vfs_file_node_c object.
 * @return              The current file size.
 *
 * @api
 */
BaseSequentialStream *vfsGetFileStream(vfs_file_node_c *vfnp) {

  chDbgAssert(vfnp->references > 0U, "zero count");

  return vfnp->vmt->file_get_stream((void *)vfnp);
}

/** @} */
