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
 * @file    vfs/drivers/drvoverlay.h
 * @brief   HAL Overlay VFS driver header.
 *
 * @addtogroup VFS_DRV_OVERLAY
 * @details Exposes HAL Overlay as VFS files.
 * @{
 */

#ifndef DRVOVERLAY_H
#define DRVOVERLAY_H

#if (VFS_CFG_ENABLE_DRV_OVERLAY == TRUE) || defined(__DOXYGEN__)

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
#if !defined(DRV_CFG_OVERLAY_DIR_NODES_NUM)
#error "DRV_CFG_OVERLAY_DIR_NODES_NUM not defined in vfsconf.h"
#endif

#if !defined(DRV_CFG_OVERLAY_DRV_MAX)
#error "DRV_CFG_OVERLAY_DRV_MAX not defined in vfsconf.h"
#endif

#if DRV_CFG_OVERLAY_DIR_NODES_NUM < 1
#error "invalid value for DRV_CFG_OVERLAY_DIR_NODES_NUM"
#endif

#if (DRV_CFG_OVERLAY_DRV_MAX < 1) || (DRV_CFG_OVERLAY_DRV_MAX > 16)
#error "invalid value for DRV_CFG_OVERLAY_DRV_MAX"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   @p vfs_overlay_dir_node_t specific methods.
 */
#define __vfs_overlay_dir_node_methods                                      \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_overlay_dir_node_t specific data.
 */
#define __vfs_overlay_dir_node_data                                         \
  __vfs_directory_node_data                                                 \
  unsigned                          index;                                  \
  /* Root node of the overlaid driver or NULL.*/                            \
  vfs_directory_node_t              *overlaid_root;                         \

/**
 * @brief   @p vfs_overlay_dir_node_t virtual methods table.
 */
struct vfs_overlay_dir_node_vmt {
  __vfs_overlay_dir_node_methods
};

/**
 * @brief   Type of a structure representing a Overlay directory VFS node.
 */
typedef struct vfs_overlay_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_overlay_dir_node_vmt *vmt;
  __vfs_overlay_dir_node_data
} vfs_overlay_dir_node_t;

/**
 * @brief   @p vfs_overlay_driver_t specific methods.
 */
#define __vfs_overlay_driver_methods                                        \
  __vfs_driver_methods

/**
 * @brief   @p vfs_overlay_driver_t specific data.
 */
#define __vfs_overlay_driver_data                                           \
  __vfs_driver_data                                                         \
  /* Pool of directory nodes.*/                                             \
  memory_pool_t                     dir_nodes_pool;                         \
  /* Driver to be overlaid or NULL.*/                                       \
  vfs_driver_t                      *overlaid_drv;                          \
  /* Static storage of directory nodes.*/                                   \
  vfs_overlay_dir_node_t            dir_nodes[DRV_CFG_OVERLAY_DIR_NODES_NUM]; \
  /* Next registration slot.*/                                              \
  unsigned                          next_driver;                            \
  /* Registration slots.*/                                                  \
  vfs_driver_t                      *drivers[DRV_CFG_OVERLAY_DRV_MAX];

/**
 * @brief   @p vfs_overlay_driver_t virtual methods table.
 */
struct vfs_overlay_driver_vmt {
  __vfs_overlay_driver_methods
};

/**
 * @brief   Type of a structure representing a VFS Overlay driver.
 */
typedef struct vfs_overlay_driver {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_overlay_driver_vmt   *vmt;
  __vfs_overlay_driver_data
} vfs_overlay_driver_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  vfs_driver_t *drvOverlayObjectInit(vfs_overlay_driver_t *vodp,
                                     vfs_driver_t *overlaid_drv,
                                     const char *rootname);
  msg_t drvOverlayRegisterDriver(vfs_overlay_driver_t *vodp,
                                 vfs_driver_t *vdp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VFS_CFG_ENABLE_DRV_OVERLAY == TRUE */

#endif /* DRVOVERLAY_H */

/** @} */
