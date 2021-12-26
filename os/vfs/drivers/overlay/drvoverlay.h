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
 * @brief   Overlays VFS driver header.
 *
 * @addtogroup VFS_DRV_OVERLAY
 * @details Implements overlays on top of another VFS driver.
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
 * @brief   @p vfs_overlay_dir_node_c specific methods.
 */
#define __vfs_overlay_dir_node_methods                                      \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_overlay_dir_node_c specific data.
 */
#define __vfs_overlay_dir_node_data                                         \
  __vfs_directory_node_data                                                 \
  unsigned                          index;                                  \
  /* Root node of the overlaid driver or NULL.*/                            \
  vfs_directory_node_c              *overlaid_root;                         \

/**
 * @brief   @p vfs_overlay_dir_node_c virtual methods table.
 */
struct vfs_overlay_dir_node_vmt {
  __vfs_overlay_dir_node_methods
};

/**
 * @brief   Type of an overlay directory VFS node class.
 */
typedef struct vfs_overlay_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_overlay_dir_node_vmt *vmt;
  __vfs_overlay_dir_node_data
} vfs_overlay_dir_node_c;

/**
 * @brief   @p vfs_overlay_driver_c specific methods.
 */
#define __vfs_overlay_driver_methods                                        \
  __vfs_driver_methods

/**
 * @brief   @p vfs_overlay_driver_c specific data.
 */
#define __vfs_overlay_driver_data                                           \
  __vfs_driver_data                                                         \
  /* Driver to be overlaid or NULL.*/                                       \
  vfs_driver_c                      *overlaid_drv;                          \
  /* Path prefix for the overlaid driver or NULL.*/                         \
  const char                        *path_prefix ;                          \
  /* Current directory or NULL.*/                                           \
  char                              *path_cwd;                              \
  /* Next registration slot.*/                                              \
  unsigned                          next_driver;                            \
  /* Registration slots.*/                                                  \
  const char                        *names[DRV_CFG_OVERLAY_DRV_MAX];        \
  vfs_driver_c                      *drivers[DRV_CFG_OVERLAY_DRV_MAX];

/**
 * @brief   @p vfs_overlay_driver_c virtual methods table.
 */
struct vfs_overlay_driver_vmt {
  __vfs_overlay_driver_methods
};

/**
 * @brief   Type of a VFS overlay driver class.
 */
typedef struct vfs_overlay_driver {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_overlay_driver_vmt   *vmt;
  __vfs_overlay_driver_data
} vfs_overlay_driver_c;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void __drv_overlay_init(void);
  vfs_driver_c *drvOverlayObjectInit(vfs_overlay_driver_c *vodp,
                                     vfs_driver_c *overlaid_drv,
                                     const char *path_prefix);
  msg_t drvOverlayRegisterDriver(vfs_overlay_driver_c *vodp,
                                 vfs_driver_c *vdp,
                                 const char *name);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VFS_CFG_ENABLE_DRV_OVERLAY == TRUE */

#endif /* DRVOVERLAY_H */

/** @} */
