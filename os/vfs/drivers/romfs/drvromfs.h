/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file        drvromfs.h
 * @brief       Generated VFS ROMFS Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  DRVROMFS
 * @{
 */

#ifndef DRVROMFS_H
#define DRVROMFS_H

#if (defined(VFS_CFG_ENABLE_DRV_ROMFS) && (VFS_CFG_ENABLE_DRV_ROMFS == TRUE)) || defined(__DOXYGEN__)

#include "oop_random_stream.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Number of directory nodes pre-allocated in the pool.
 */
#if !defined(DRV_CFG_ROM_DIR_NODES_NUM) || defined(__DOXYGEN__)
#define DRV_CFG_ROM_DIR_NODES_NUM           1
#endif

/**
 * @brief       Number of file nodes pre-allocated in the pool.
 */
#if !defined(DRV_CFG_ROM_FILE_NODES_NUM) || defined(__DOXYGEN__)
#define DRV_CFG_ROM_FILE_NODES_NUM          1
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on DRV_CFG_ROM_DIR_NODES_NUM configuration.*/
#if DRV_CFG_ROM_DIR_NODES_NUM < 1
#error "invalid DRV_CFG_ROM_DIR_NODES_NUM value"
#endif

/* Checks on DRV_CFG_ROM_FILE_NODES_NUM configuration.*/
#if DRV_CFG_ROM_FILE_NODES_NUM < 1
#error "invalid DRV_CFG_ROM_FILE_NODES_NUM value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef struct vfs_romfs_dynamic_ops vfs_romfs_dynamic_ops_t;
typedef struct vfs_romfs_file_desc vfs_romfs_file_desc_t;
typedef struct vfs_romfs_dir_desc vfs_romfs_dir_desc_t;
typedef struct vfs_romfs_tree vfs_romfs_tree_t;

/**
 * @brief       ROMFS dynamic file callbacks.
 */
struct vfs_romfs_dynamic_ops {
  msg_t (*open)(const void *arg, int flags, void **sessionp,
                vfs_offset_t *sizep);
  void (*close)(void *session);
  ssize_t (*read)(void *session, vfs_offset_t offset, uint8_t *buf, size_t n);
  msg_t (*stat)(const void *arg, vfs_offset_t *sizep);
};

/**
 * @brief       ROMFS file descriptor.
 */
struct vfs_romfs_file_desc {
  const char                    *name;
  vfs_mode_t                    mode;
  uint32_t                      flags;
  vfs_offset_t                  size;
  const uint8_t                 *data;
  const vfs_romfs_dynamic_ops_t *ops;
  const void                    *arg;
};

/**
 * @brief       ROMFS directory descriptor.
 */
struct vfs_romfs_dir_desc {
  const char                    *path;
  const vfs_romfs_file_desc_t   *files;
  size_t                        files_num;
};

/**
 * @brief       ROMFS image descriptor.
 */
struct vfs_romfs_tree {
  const vfs_romfs_dir_desc_t    *dirs;
  size_t                        dirs_num;
};

/**
 * @class       vfs_rom_driver_c
 * @extends     vfs_driver_c
 *
 *
 * @name        Class @p vfs_rom_driver_c structures
 * @{
 */

/**
 * @brief       Type of a VFS ROMFS driver class.
 */
typedef struct vfs_rom_driver vfs_rom_driver_c;

/**
 * @brief       Class @p vfs_rom_driver_c virtual methods table.
 */
struct vfs_rom_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From vfs_driver_c.*/
  msg_t (*setcwd)(void *ip, const char *path);
  msg_t (*getcwd)(void *ip, char *buf, size_t size);
  msg_t (*stat)(void *ip, const char *path, vfs_stat_t *sp);
  msg_t (*opendir)(void *ip, const char *path, vfs_directory_node_c **vdnpp);
  msg_t (*openfile)(void *ip, const char *path, int flags, vfs_file_node_c **vfnpp);
  msg_t (*unlink)(void *ip, const char *path);
  msg_t (*rename)(void *ip, const char *oldpath, const char *newpath);
  msg_t (*mkdir)(void *ip, const char *path, vfs_mode_t mode);
  msg_t (*rmdir)(void *ip, const char *path);
  /* From vfs_rom_driver_c.*/
};

/**
 * @brief       Structure representing a VFS ROMFS driver class.
 */
struct vfs_rom_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct vfs_rom_driver_vmt *vmt;
  /**
   * @brief       Pointer to the ROMFS image.
   */
  const vfs_romfs_tree_t          *tree;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern struct vfs_rom_driver_static_struct vfs_rom_driver_static;

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of vfs_rom_driver_c.*/
  void *__romdrv_objinit_impl(void *ip, const void *vmt,
                              const vfs_romfs_tree_t *tree);
  void __romdrv_dispose_impl(void *ip);
  msg_t __romdrv_setcwd_impl(void *ip, const char *path);
  msg_t __romdrv_getcwd_impl(void *ip, char *buf, size_t size);
  msg_t __romdrv_stat_impl(void *ip, const char *path, vfs_stat_t *sp);
  msg_t __romdrv_opendir_impl(void *ip, const char *path,
                              vfs_directory_node_c **vdnpp);
  msg_t __romdrv_openfile_impl(void *ip, const char *path, int flags,
                               vfs_file_node_c **vfnpp);
  msg_t __romdrv_unlink_impl(void *ip, const char *path);
  msg_t __romdrv_rename_impl(void *ip, const char *oldpath,
                             const char *newpath);
  msg_t __romdrv_mkdir_impl(void *ip, const char *path, vfs_mode_t mode);
  msg_t __romdrv_rmdir_impl(void *ip, const char *path);
  /* Regular functions.*/
  void __drv_rom_init(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of vfs_rom_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p vfs_rom_driver_c.
 *
 * @param[out]    self          Pointer to a @p vfs_rom_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline vfs_rom_driver_c *romdrvObjectInit(vfs_rom_driver_c *self,
                                                 const vfs_romfs_tree_t *tree) {
  extern const struct vfs_rom_driver_vmt __vfs_rom_driver_vmt;

  return __romdrv_objinit_impl(self, &__vfs_rom_driver_vmt, tree);
}
/** @} */

#endif /* defined(VFS_CFG_ENABLE_DRV_ROMFS) && (VFS_CFG_ENABLE_DRV_ROMFS == TRUE) */

#endif /* DRVROMFS_H */

/** @} */
