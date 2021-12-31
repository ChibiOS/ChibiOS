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
 * @file    vfs/include/vfsdrivers.h
 * @brief   VFS drivers header file.
 *
 * @addtogroup VFS_DRIVERS
 * @{
 */

#ifndef VFSDRIVERS_H
#define VFSDRIVERS_H

#include <fcntl.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    File open flags compatible with Posix
 * @{
 */
#define VO_SUPPORTED_FLAGS_MASK     (O_ACCMODE | O_APPEND | O_CREAT |       \
                                     O_TRUNC | O_EXCL)
#define VO_ACCMODE                  O_ACCMODE
#define VO_RDONLY                   O_RDONLY
#define VO_WRONLY                   O_WRONLY
#define VO_RDWR                     O_RDWR
#define VO_APPEND                   O_APPEND
#define VO_CREAT                    O_CREAT
#define VO_TRUNC                    O_TRUNC
#define VO_EXCL                     O_EXCL
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

/**
 * @brief   @p vfs_driver_c specific methods.
 */
#define __vfs_driver_methods                                                \
  __base_object_methods                                                     \
  msg_t (*set_cwd)(void *instance, const char *path);                       \
  msg_t (*get_cwd)(void *instance, char *buf, size_t size);                 \
  msg_t (*open_dir)(void *instance,                                         \
                    const char *path,                                       \
                    vfs_directory_node_c **vdnpp);                          \
  msg_t (*open_file)(void *instance,                                        \
                     const char *path,                                      \
                     int flags,                                             \
                     vfs_file_node_c **vfnpp);

/**
 * @brief   @p vfs_driver_c specific data.
 */
#define __vfs_driver_data                                                   \
  __base_object_data

/**
 * @brief   @p vfs_driver_c virtual methods table.
 */
struct vfs_driver_vmt {
  __vfs_driver_methods
};

/**
 * @brief   Type of a VFS driver class.
 */
typedef struct vfs_driver {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_driver_vmt   *vmt;
  __vfs_driver_data
} vfs_driver_c;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  msg_t vfsDrvOpen(vfs_driver_c *drvp,
                   const char *path,
                   int flags,
                   vfs_node_c **vnpp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Changes the current VFS directory.
 *
 * @param[in] drvp      Pointer to the @p vfs_driver_c object.
 * @param[in] path      Path of the new current directory.
 * @return              The operation result.
 *
 * @api
 */
static inline msg_t vfsDrvChangeCurrentDirectory(vfs_driver_c *drvp,
                                                 const char *path) {

  return drvp->vmt->set_cwd(drvp, path);
}

/**
 * @brief   Returns the current VFS directory.
 *
 * @param[in] drvp      Pointer to the @p vfs_driver_c object.
 * @param[out] buf      Buffer for the path string.
 * @param[in] size      Size of the buffer.
 * @return              The operation result.
 *
 * @api
 */
static inline msg_t vfsDrvGetCurrentDirectory(vfs_driver_c *drvp,
                                              char *buf, size_t size) {

  return drvp->vmt->get_cwd(drvp, buf, size);
}

/**
 * @brief   Opens a VFS directory.
 *
 * @param[in] drvp      Pointer to the @p vfs_driver_c object.
 * @param[in] path      Absolute path of the directory to be opened.
 * @param[out] vdnpp    Pointer to the pointer to the instantiated.
 *                      @p vfs_directory_node_c object
 * @return              The operation result.
 *
 * @api
 */
static inline msg_t vfsDrvOpenDirectory(vfs_driver_c *drvp,
                                        const char *path,
                                        vfs_directory_node_c **vdnpp) {

  return drvp->vmt->open_dir(drvp, path, vdnpp);
}

/**
 * @brief   Opens a VFS file.
 *
 * @param[in] drvp      Pointer to the @p vfs_driver_c object.
 * @param[in] path      Path of the file to be opened.
 * @param[in] flags     File open flags.
 * @param[out] vdnpp    Pointer to the pointer to the instantiated
 *                      @p vfs_file_node_c object.
 * @return              The operation result.
 *
 * @api
 */
static inline msg_t vfsDrvOpenFile(vfs_driver_c *drvp,
                                   const char *path,
                                   int flags,
                                   vfs_file_node_c **vfnpp) {

  return drvp->vmt->open_file(drvp, path, flags, vfnpp);
}

#endif /* VFSDRIVERS_H */

/** @} */
