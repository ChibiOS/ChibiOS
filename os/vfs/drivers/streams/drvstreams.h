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
 * @file    vfs/drivers/drvstreams.h
 * @brief   HAL streams VFS driver header.
 *
 * @addtogroup VFS_DRV_STREAMS
 * @details Exposes HAL streams as VFS files.
 * @{
 */

#ifndef DRVSTREAMS_H
#define DRVSTREAMS_H

#include "vfs.h"

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
 * @brief   Type of a structure representing a VFS driver.
 */
typedef struct vfs_drv_streams vfs_drv_streams_t;

/**
 * @brief   @p vfs_node_t specific methods.
 */
#define __vfs_drv_streams_methods                                           \
  __vfs_driver_methods

/**
 * @brief   @p vfs_node_t specific data.
 */
#define __vfs_drv_streams_data                                              \
  __vfs_driver_data

/**
 * @brief   @p vfs_node_t virtual methods table.
 */
struct vfs_drv_stream_vmt {
  __vfs_drv_streams_methods
};

/**
 * @brief   TStructure representing a VFS driver.
 */
struct vfs_drv_streams {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_drv_streams_vmt   *vmt;
  __vfs_drv_streams_data
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* DRVSTREAMS_H */

/** @} */
