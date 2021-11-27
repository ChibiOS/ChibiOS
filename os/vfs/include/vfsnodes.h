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
 * @file    vfs/include/vfsnodes.h
 * @brief   VFS nodes header file.
 *
 * @addtogroup VFS_NODES
 * @{
 */

#ifndef VFSNODES_H
#define VFSNODES_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Node attributes.
 * @{
 */
#define VFS_NODE_ATTR_READONLY      1U
#define VFS_NODE_ATTR_RESERVED2     2U
#define VFS_NODE_ATTR_SYSTEM        4U
#define VFS_NODE_ATTR_RESERVED8     8U
#define VFS_NODE_ATTR_ISDIR         16U
#define VFS_NODE_ATTR_ISSTREAM      256U
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

/* Forward declaration because nodes holds references to drivers.*/
typedef struct vfs_driver vfs_driver_t;

/**
 * @brief   Type of a file offset.
 */
typedef uint32_t vfs_offset_t;

/**
 * @brief   Type of a node attributes.
 */
typedef uint32_t vfs_nodeattr_t;

/**
 * @brief   Type of a node information structure.
 * @todo    Add attributes, permissions etc.
 */
typedef struct vfs_node_info {
  /**
   * @brief   Size of the node.
   */
  vfs_nodeattr_t        attr;
  /**
   * @brief   Size of the node.
   */
  vfs_offset_t          size;
  /**
   * @brief   Name of the node.
   */
  char                  name[VFS_CFG_MAX_NAMELEN + 1];
} vfs_node_info_t;

/**
 * @brief   Type of a structure representing a generic VFS node.
 */
typedef struct vfs_node vfs_node_t;

/**
 * @brief   @p vfs_node_t specific methods.
 */
#define __vfs_node_methods                                                  \
  _base_object_methods                                                      \
  /* Node release, the object is disposed when the counter reaches zero.*/  \
  void (*release)(void *instance);

/**
 * @brief   @p vfs_node_t specific data.
 */
#define __vfs_node_data                                                     \
  _base_object_data                                                         \
  /* Number of references to this node.*/                                   \
  unsigned              refs;                                               \
  /* Driver handling this node.*/                                           \
  vfs_driver_t          *driver;

/**
 * @brief   @p vfs_node_t virtual methods table.
 */
struct vfs_node_vmt {
  __vfs_node_methods
};

/**
 * @brief   Structure representing a generic VFS node.
 */
struct vfs_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_node_vmt *vmt;
  __vfs_node_data
};

/**
 * @brief   Type of a structure representing a directory VFS node.
 */
typedef struct vfs_directory_node vfs_directory_node_t;

/**
 * @brief   @p vfs_directory_node_t specific methods.
 */
#define __vfs_directory_node_methods                                        \
  __vfs_node_methods                                                        \
  msg_t (*dir_first)(void *instance, vfs_node_info_t *nip);                 \
  msg_t (*dir_next)(void *instance, vfs_node_info_t *nip);

/**
 * @brief   @p vfs_directory_node_t specific data.
 */
#define __vfs_directory_node_data                                           \
  __vfs_node_data

/**
 * @brief   @p vfs_directory_node_t virtual methods table.
 */
struct vfs_directory_node_vmt {
  __vfs_directory_node_methods
};

/**
 * @brief   Structure representing a directory VFS node.
 */
struct vfs_directory_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_directory_node_vmt *vmt;
  __vfs_directory_node_data
};

/**
 * @brief   Type of a structure representing a file VFS node.
 */
typedef struct vfs_file_node vfs_file_node_t;

/**
 * @brief   @p vfs_file_node_t specific methods.
 */
#define __vfs_file_node_methods                                             \
  __vfs_node_methods                                                        \
  BaseSequentialStream *(*file_get_stream)(void *instance);                 \
  ssize_t (*file_read)(void *instance, uint8_t *buf, size_t n);             \
  ssize_t (*file_write)(void *instance, const uint8_t *buf, size_t n);      \
  msg_t (*file_setpos)(void *instance, vfs_offset_t offset);                \
  vfs_offset_t (*file_getpos)(void *instance);                              \
  vfs_offset_t (*file_getsize)(void *instance);

/**
 * @brief   @p vfs_file_node_t specific data.
 */
#define __vfs_file_node_data                                                \
  __vfs_node_data

/**
 * @brief   @p vfs_file_node_t virtual methods table.
 */
struct vfs_file_node_vmt {
  __vfs_file_node_methods
};

/**
 * @brief   Structure representing a file VFS node.
 */
struct vfs_file_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_file_node_vmt *vmt;
  __vfs_file_node_data
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

#endif /* VFSNODES_H */

/** @} */
