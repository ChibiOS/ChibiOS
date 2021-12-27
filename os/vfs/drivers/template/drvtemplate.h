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
 * @file    vfs/drivers/drvtemplate.h
 * @brief  SimpleFS VFS driver header.
 *
 * @addtogroup VFS_DRV_TEMPLATE
 * @details Simple FS for VFS.
 * @{
 */

#ifndef DRVTEMPLATE_H
#define DRVTEMPLATE_H

#if (VFS_CFG_ENABLE_DRV_TEMPLATE == TRUE) || defined(__DOXYGEN__)

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
#if !defined(DRV_CFG_TEMPLATE_DIR_NODES_NUM)
#error "DRV_CFG_TEMPLATE_DIR_NODES_NUM not defined in vfsconf.h"
#endif

#if !defined(DRV_CFG_TEMPLATE_FILE_NODES_NUM)
#error "DRV_CFG_TEMPLATE_FILE_NODES_NUM not defined in vfsconf.h"
#endif

#if DRV_CFG_TEMPLATE_DIR_NODES_NUM < 1
#error "invalid value for DRV_CFG_TEMPLATE_DIR_NODES_NUM"
#endif

#if DRV_CFG_TEMPLATE_FILE_NODES_NUM < 1
#error "invalid value for DRV_CFG_TEMPLATE_FILE_NODES_NUM"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   @p vfs_template_dir_node_c specific methods.
 */
#define __vfs_template_dir_node_methods                                          \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_template_dir_node_c specific data.
 */
#define __vfs_template_dir_node_data                                             \
  __vfs_directory_node_data

/**
 * @brief   @p vfs_template_dir_node_c virtual methods table.
 */
struct vfs_template_dir_node_vmt {
  __vfs_template_dir_node_methods
};

/**
 * @brief   Type of a Template directory VFS node class.
 */
typedef struct vfs_template_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_template_dir_node_vmt     *vmt;
  __vfs_template_dir_node_data
} vfs_template_dir_node_c;

/**
 * @brief   @p vfs_template_file_node_c specific methods.
 */
#define __vfs_template_file_node_methods                                         \
  __vfs_file_node_methods

/**
 * @brief   @p vfs_template_file_node_c specific data.
 */
#define __vfs_template_file_node_data                                            \
  __vfs_file_node_data                                                      \
  BaseSequentialStream                  stream;

/**
 * @brief   @p vfs_template_file_node_c virtual methods table.
 */
struct vfs_template_file_node_vmt {
  __vfs_template_file_node_methods
};

/**
 * @brief   Type of a Template file VFS node class.
 */
typedef struct vfs_template_file_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_template_file_node_vmt    *vmt;
  __vfs_template_file_node_data
} vfs_template_file_node_c;

/**
 * @brief   @p vfs_template_driver_c specific methods.
 */
#define __vfs_template_driver_methods                                            \
  __vfs_driver_methods

/**
 * @brief   @p vfs_template_driver_c specific data.
 */
#define __vfs_template_driver_data                                               \
  __vfs_driver_data

/**
 * @brief   @p vfs_template_driver_c virtual methods table.
 */
struct vfs_template_driver_vmt {
  __vfs_template_driver_methods
};

/**
 * @brief   Type of a VFS Template driver class.
 */
typedef struct vfs_template_driver {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_template_driver_vmt       *vmt;
  __vfs_template_driver_data
} vfs_template_driver_c;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void __drv_template_init(void);
  vfs_driver_c *drvTEMPLATEObjectInit(vfs_template_driver_c *drvp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VFS_CFG_ENABLE_DRV_TEMPLATE == TRUE */

#endif /* DRVTEMPLATE_H */

/** @} */
