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
 * @file        drvromfile.h
 * @brief       Generated VFS RomFile Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  DRVROMFILE
 * @{
 */

#ifndef DRVROMFILE_H
#define DRVROMFILE_H

#if (VFS_CFG_ENABLE_DRV_ROMFILES == TRUE) || defined(__DOXYGEN__)

#include "oop_sequential_stream.h"

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
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a structure representing a ROM file data.
 */
typedef struct vfs_romfile vfs_romfile_t;

/**
 * @brief       Structure representing a ROM file data.
 */
struct vfs_romfile {
  /**
   * @brief       File data attributes.
   */
  uint32_t                  flags;
  /**
   * @brief       Compressed data.
   */
  uint8_t                   data[];
};

/**
 * @class       vfs_romfiles_driver_c
 * @extends     vfs_file_node_c
 * @implements  sequential_stream_i
 *
 *
 * @name        Class @p vfs_romfiles_driver_c structures
 * @{
 */

/**
 * @brief       Type of a VFS ROMFiles driver class.
 */
typedef struct vfs_romfiles_driver vfs_romfiles_driver_c;

/**
 * @brief       Class @p vfs_romfiles_driver_c virtual methods table.
 */
struct vfs_romfiles_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From referenced_object_c.*/
  void * (*addref)(void *ip);
  object_references_t (*release)(void *ip);
  /* From vfs_node_c.*/
  msg_t (*stat)(void *ip, vfs_stat_t *sp);
  /* From vfs_file_node_c.*/
  ssize_t (*read)(void *ip, uint8_t *buf, size_t n);
  ssize_t (*write)(void *ip, const uint8_t *buf, size_t n);
  msg_t (*setpos)(void *ip, vfs_offset_t offset, vfs_seekmode_t whence);
  vfs_offset_t (*getpos)(void *ip);
  sequential_stream_i * (*getstream)(void *ip);
  /* From vfs_romfiles_driver_c.*/
};

/**
 * @brief       Structure representing a VFS ROMFiles driver class.
 */
struct vfs_romfiles_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct vfs_romfiles_driver_vmt *vmt;
  /**
   * @brief       Number of references to the object.
   */
  object_references_t       references;
  /**
   * @brief       Driver handling this node.
   */
  vfs_driver_c              *driver;
  /**
   * @brief       Node mode information.
   */
  vfs_mode_t                mode;
  /**
   * @brief       Implemented interface @p sequential_stream_i.
   */
  sequential_stream_i       stm;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of vfs_romfiles_driver_c.*/
  void *__romfile_objinit_impl(void *ip, const void *vmt,
                               const vfs_romfile_t *romdatap);
  void __romfile_dispose_impl(void *ip);
  msg_t __romfile_stat_impl(void *ip, vfs_stat_t *sp);
  ssize_t __romfile_read_impl(void *ip, uint8_t *buf, size_t n);
  ssize_t __romfile_write_impl(void *ip, const uint8_t *buf, size_t n);
  msg_t __romfile_setpos_impl(void *ip, vfs_offset_t offset,
                              vfs_seekmode_t whence);
  vfs_offset_t __romfile_getpos_impl(void *ip);
  sequential_stream_i *__romfile_getstream_impl(void *ip);
  /* Regular functions.*/
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of vfs_romfiles_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p vfs_romfiles_driver_c.
 *
 * @param[out]    self          Pointer to a @p vfs_romfiles_driver_c instance
 *                              to be initialized.
 * @param[in]     romdatap      Pointer to @p vfs_romfile_t data.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline vfs_romfiles_driver_c *romfileObjectInit(vfs_romfiles_driver_c *self,
                                                       const vfs_romfile_t *romdatap) {
  extern const struct vfs_romfiles_driver_vmt __vfs_romfiles_driver_vmt;

  return __romfile_objinit_impl(self, &__vfs_romfiles_driver_vmt, romdatap);
}
/** @} */

#endif /* VFS_CFG_ENABLE_DRV_ROMFILES == TRUE */

#endif /* DRVROMFILE_H */

/** @} */
