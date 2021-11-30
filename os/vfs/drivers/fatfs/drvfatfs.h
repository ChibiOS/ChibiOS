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
 * @file    vfs/drivers/drvfatfs.h
 * @brief   HAL FatFS VFS driver header.
 *
 * @addtogroup VFS_DRV_FATFS
 * @details Exposes HAL FatFS as VFS files.
 * @{
 */

#ifndef DRVFATFS_H
#define DRVFATFS_H

#include "ff.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Configuration options checks.*/
#if !defined(DRV_CFG_FATFS_DIR_NODES_NUM)
#error "DRV_CFG_FATFS_DIR_NODES_NUM not defined in vfsconf.h"
#endif

#if !defined(DRV_CFG_FATFS_FILE_NODES_NUM)
#error "DRV_CFG_FATFS_FILE_NODES_NUM not defined in vfsconf.h"
#endif

#if DRV_CFG_FATFS_DIR_NODES_NUM < 1
#error "invalid value for DRV_CFG_FATFS_DIR_NODES_NUM"
#endif

#if DRV_CFG_FATFS_FILE_NODES_NUM < 1
#error "invalid value for DRV_CFG_FATFS_FILE_NODES_NUM"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   @p vfs_fatfs_dir_node_t specific methods.
 */
#define __vfs_fatfs_dir_node_methods                                        \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_fatfs_dir_node_t specific data.
 */
#define __vfs_fatfs_dir_node_data                                           \
  __vfs_directory_node_data                                                 \
  DIR                           dir;

/**
 * @brief   @p vfs_fatfs_dir_node_t virtual methods table.
 */
struct vfs_fatfs_dir_node_vmt {
  __vfs_fatfs_dir_node_methods
};

/**
 * @brief   Type of a structure representing a FatFS directory VFS node.
 */
typedef struct vfs_fatfs_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_dir_node_vmt *vmt;
  __vfs_fatfs_dir_node_data
} vfs_fatfs_dir_node_t;

/**
 * @brief   @p vfs_fatfs_file_node_t specific methods.
 */
#define __vfs_fatfs_file_node_methods                                       \
  __vfs_file_node_methods

/**
 * @brief   @p vfs_fatfs_file_node_t specific data.
 */
#define __vfs_fatfs_file_node_data                                          \
  __vfs_file_node_data                                                      \
  FIL                           file;                                       \
  BaseSequentialStream          stream;

/**
 * @brief   @p vfs_fatfs_file_node_t virtual methods table.
 */
struct vfs_fatfs_file_node_vmt {
  __vfs_fatfs_file_node_methods
};

/**
 * @brief   Type of a structure representing a FatFS file VFS node.
 */
typedef struct vfs_fatfs_file_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_file_node_vmt *vmt;
  __vfs_fatfs_file_node_data
} vfs_fatfs_file_node_t;

/**
 * @brief   @p vfs_fatfs_driver_t specific methods.
 */
#define __vfs_fatfs_driver_methods                                          \
  __vfs_driver_methods

/**
 * @brief   @p vfs_fatfs_driver_t specific data.
 */
#define __vfs_fatfs_driver_data                                             \
  __vfs_driver_data                                                         \
  memory_pool_t                 file_nodes_pool;                            \
  memory_pool_t                 dir_nodes_pool;                             \
  memory_pool_t                 info_nodes_pool;                            \
  memory_pool_t                 fs_nodes_pool;                              \
  vfs_fatfs_dir_node_t drv_dir_nodes[DRV_CFG_FATFS_DIR_NODES_NUM];          \
  vfs_fatfs_file_node_t drv_file_nodes[DRV_CFG_FATFS_FILE_NODES_NUM];

/**
 * @brief   @p vfs_fatfs_driver_t virtual methods table.
 */
struct vfs_fatfs_driver_vmt {
  __vfs_fatfs_driver_methods
};

/**
 * @brief   Type of a structure representing a VFS FatFS driver.
 */
typedef struct vfs_drv_streams {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_driver_vmt   *vmt;
  __vfs_fatfs_driver_data
} vfs_fatfs_driver_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern vfs_fatfs_driver_t vfs_fatfs;

#ifdef __cplusplus
extern "C" {
#endif
  vfs_driver_t *drvFatFSInit(const char *rootname);
  msg_t drvFatFSMount(const char *name, bool mountnow);
  msg_t drvFatFSUnmount(const char *name);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* DRVFATFS_H */

/** @} */
