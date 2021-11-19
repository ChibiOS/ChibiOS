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
 * @addtogroup VFS_NODES
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
 * @brief   @p vfs_system_node_t specific methods.
 */
#define __vfs_system_node_methods                                           \
  __vfs_node_methods

/**
 * @brief   @p vfs_system_node_t specific data.
 */
#define __vfs_system_node_data                                              \
  __vfs_node_data

/**
 * @brief   @p vfs_system_node_t virtual methods table.
 */
struct vfs_system_node_vmt {
  __vfs_system_node_methods
};

/**
 * @brief   Type of a structure representing a VFS system node.
 */
typedef struct vfs_system_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_system_node_vmt *vmt;
  __vfs_system_node_data
} vfs_system_node_t;

/**
 * @brief   Type of a structure representing the VFS system.
 */
typedef struct vfs_system {
  /**
   * @brief   Absolute root node.
   */
  vfs_node_t                    *root_node;
} vfs_system_t;

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

#endif /* VFSSYSTEM_H */

/** @} */
