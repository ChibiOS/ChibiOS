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
 * @brief   Type of a VFS error code.
 */
typedef enum {
  VFS_RET_SUCCESS = 0,    /**< VFS_RET_SUCCESS */
  VFS_RET_NO_RESOURCE = -1/**< VFS_RET_NO_RESOURCE */
} vfserr_t;

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
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VFS access mutex.
   */
  mutex_t                       mtx;
#else
  /**
   * @brief   VFS access fallback semaphore.
   */
  semaphore_t                   sem;
#endif
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
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
#define VFS_LOCK()              chMtxLock(&vfs.mtx)
#define VFS_UNLOCK()            chMtxUnlock(&vfs.mtx)
#else
#define VFS_LOCK()              (void) chSemWait(&vfs.sem)
#define VFS_UNLOCK()            chSemSignal(&vfs.sem)
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern vfs_system_t vfs;

#ifdef __cplusplus
extern "C" {
#endif
  void vfsInit(void);
  vfserr_t vfsRegisterDriver(vfs_driver_t *vdp);
#ifdef __cplusplus
}
#endif

#endif /* VFSSYSTEM_H */

/** @} */
