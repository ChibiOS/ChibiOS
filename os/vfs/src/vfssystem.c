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
 * @brief   Opens a VFS directory.
 *
 * @param[in] vdp       VFS instance where to open the directory
 * @param[in] path      absolute path of the directory to be opened
 * @param[out] vdnpp    pointer to the pointer to the instantiated
 *                      @p vfs_directory_node_t object
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsOpenDirectory(vfs_driver_t *vdp,
                       const char *path,
                       vfs_directory_node_t **vdnpp) {

  return vdp->vmt->open_dir(vdp, path, vdnpp);
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
 * @param[in] vdp       VFS instance where to open the file
 * @param[in] path      absolute path of the file to be opened
 * @param[in] mode      file open mode
 * @param[out] vdnpp    pointer to the pointer to the instantiated
 *                      @p vfs_file_node_t object
 * @return              The operation result.
 *
 * @api
 */
msg_t vfsOpenFile(vfs_driver_t *vdp,
                  const char *path,
                  unsigned mode,
                  vfs_file_node_t **vfnpp) {

  return vdp->vmt->open_file(vdp, path, mode, vfnpp);
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

/**
 * @brief   Returns the inner stream associated to the file.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @return              The current file size.
 *
 * @api
 */
BaseSequentialStream *vfsGetFileStream(vfs_file_node_t *vfnp) {

  chDbgAssert(vfnp->refs > 0U, "zero count");

  return vfnp->vmt->file_get_stream((void *)vfnp);
}

/** @} */
