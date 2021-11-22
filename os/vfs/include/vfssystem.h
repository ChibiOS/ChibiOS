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
 * @file    vfs/include/vfssystem.h
 * @brief   VFS system header file.
 *
 * @addtogroup VFS_SYSTEM
 * @{
 */

#ifndef VFSSYSTEM_H
#define VFSSYSTEM_H

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

/**
 * @brief   @p vfs_system_directory_node_t specific methods.
 */
#define __vfs_system_directory_node_methods                                 \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_system_directory_node_t specific data.
 */
#define __vfs_system_directory_node_data                                    \
  __vfs_directory_node_data

/**
 * @brief   @p vfs_system_directory_node_t virtual methods table.
 */
struct vfs_system_directory_node_vmt {
  __vfs_system_directory_node_methods
};

/**
 * @brief   Type of a structure representing a VFS system directory node.
 */
typedef struct vfs_system_directory_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_system_directory_node_vmt *vmt;
  __vfs_system_directory_node_data
} vfs_system_directory_node_t;

/**
 * @brief   Type of a structure representing the VFS system.
 */
typedef struct vfs_system {
  /**
   * @brief   VFS access mutex.
   */
  mutex_t                       mtx;
  /**
   * @brief   Absolute root node.
   */
  vfs_system_directory_node_t   *root_node;
  /**
   * @brief   Next registration slot.
   */
  vfs_driver_t                  **next_driver;
  /**
   * @brief   Registration slots.
   */
  vfs_driver_t                  *drivers[VFS_CFG_MAX_DRIVERS];
} vfs_system_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*
 * Defaults on the best synchronization mechanism available.
 */
#define VFS_LOCK()              osalMutexLock(&vfs.mtx)
#define VFS_UNLOCK()            osalMutexUnlock(&vfs.mtx)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern vfs_system_t vfs;

#ifdef __cplusplus
extern "C" {
#endif
  void vfsInit(void);
  msg_t vfsRegisterDriver(vfs_driver_t *vdp);
  msg_t vfsOpenDirectory(const char *name, vfs_directory_node_t **vdnpp);
  void vfsCloseDirectory(vfs_directory_node_t *vdnp);
  msg_t vfsOpenFile(const char *name, vfs_file_node_t **vfnpp);
  void vfsCloseFile(vfs_file_node_t *vfnp);
  ssize_t vfsReadFile(vfs_file_node_t *vfnp, uint8_t *buf, size_t n);
  ssize_t vfsWriteFile(vfs_file_node_t *vfnp, const uint8_t *buf, size_t n);
  msg_t vfsSetFilePosition(vfs_file_node_t *vfnp, vfs_offset_t offset);
  vfs_offset_t vfsGetFilePosition(vfs_file_node_t *vfnp);
  vfs_offset_t vfsGetFileSize(vfs_file_node_t *vfnp);
#ifdef __cplusplus
}
#endif

#endif /* VFSSYSTEM_H */

/** @} */
