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
 * @name    Node modes and helpers
 * @{
 */
#define VFS_MODE_S_IFMT             S_IFMT
#define VFS_MODE_S_IFREG            S_IFREG
#define VFS_MODE_S_IFDIR            S_IFDIR
#define VFS_MODE_S_IFCHR            S_IFCHR
#define VFS_MODE_S_IFIFO            S_IFIFO

#define VFS_MODE_S_IRWXU            S_IRWXU
#define VFS_MODE_S_IRUSR            S_IRUSR
#define VFS_MODE_S_IWUSR            S_IWUSR
#define VFS_MODE_S_IXUSR            S_IXUSR

#define VFS_MODE_S_ISREG(mode)      S_ISREG(mode)
#define VFS_MODE_S_ISDIR(mode)      S_ISDIR(mode)
#define VFS_MODE_S_ISCHR(mode)      S_ISCHR(mode)
#define VFS_MODE_S_ISFIFO(mode)     S_ISFIFO(mode)
/** @} */

/**
 * @name    Seek modes compatible with Posix
 * @{
 */
#define VFS_SEEK_SET                SEEK_SET
#define VFS_SEEK_CUR                SEEK_CUR
#define VFS_SEEK_END                SEEK_END
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
typedef struct vfs_driver vfs_driver_c;

/**
 * @brief   Type of a file offset.
 */
typedef int32_t vfs_offset_t;

/**
 * @brief   Type of a node mode.
 */
typedef int32_t vfs_mode_t;

/**
 * @brief   Type of a seek mode.
 */
typedef int vfs_seekmode_t;

/**
 * @brief   Type of a directory entry structure.
 */
typedef struct vfs_direntry_info {
  /**
   * @brief   Node mode.
   */
  vfs_mode_t            mode;
  /**
   * @brief   Size of the node.
   */
  vfs_offset_t          size;
  /**
   * @brief   Name of the node.
   */
  char                  name[VFS_CFG_NAMELEN_MAX + 1];
} vfs_direntry_info_t;

/**
 * @brief   Type of a node information structure.
 * @todo    Add time, permissions etc.
 */
typedef struct vfs_stat {
  /**
   * @brief   Modes of the node.
   */
  vfs_mode_t            mode;
  /**
   * @brief   Size of the node.
   */
  vfs_offset_t          size;
} vfs_stat_t;

/**
 * @brief   Type of a generic VFS node class.
 */
typedef struct vfs_node vfs_node_c;

/**
 * @brief   @p vfs_node_c specific methods.
 */
#define __vfs_node_methods                                                  \
  __referenced_object_methods                                               \
  msg_t (*node_stat)(void *instance, vfs_stat_t *fsp);

/**
 * @brief   @p vfs_node_c specific data.
 */
#define __vfs_node_data                                                     \
  __referenced_object_data                                                  \
  /* Driver handling this node.*/                                           \
  vfs_driver_c          *driver;                                            \
  /* Node mode information.*/                                               \
  vfs_mode_t            mode;

/**
 * @brief   @p vfs_node_c virtual methods table.
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
 * @brief   Type of a directory VFS node class.
 */
typedef struct vfs_directory_node vfs_directory_node_c;

/**
 * @brief   @p vfs_directory_node_c specific methods.
 */
#define __vfs_directory_node_methods                                        \
  __vfs_node_methods                                                        \
  msg_t (*dir_first)(void *instance, vfs_direntry_info_t *dip);             \
  msg_t (*dir_next)(void *instance, vfs_direntry_info_t *dip);

/**
 * @brief   @p vfs_directory_node_c specific data.
 */
#define __vfs_directory_node_data                                           \
  __vfs_node_data

/**
 * @brief   @p vfs_directory_node_c virtual methods table.
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
 * @brief   Type of a a file VFS node class.
 */
typedef struct vfs_file_node vfs_file_node_c;

/**
 * @brief   @p vfs_file_node_c specific methods.
 */
#define __vfs_file_node_methods                                             \
  __vfs_node_methods                                                        \
  BaseSequentialStream *(*file_get_stream)(void *instance);                 \
  ssize_t (*file_read)(void *instance, uint8_t *buf, size_t n);             \
  ssize_t (*file_write)(void *instance, const uint8_t *buf, size_t n);      \
  msg_t (*file_setpos)(void *instance,                                      \
                       vfs_offset_t offset,                                 \
                       vfs_seekmode_t whence);                              \
  vfs_offset_t (*file_getpos)(void *instance);

/**
 * @brief   @p vfs_file_node_c specific data.
 */
#define __vfs_file_node_data                                                \
  __vfs_node_data

/**
 * @brief   @p vfs_file_node_c virtual methods table.
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
