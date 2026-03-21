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
 * @file        vfsnodes.c
 * @brief       Generated VFS Nodes source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  VFSNODES
 * @{
 */

#include "vfs.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "vfs_node_c" methods.                                        */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_node_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_node_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
void *__vfsnode_objinit_impl(void *ip, const void *vmt, vfs_driver_c *driver,
                             vfs_mode_t mode) {
  vfs_node_c *self = (vfs_node_c *)ip;

  /* Initialization code.*/
  self = __ro_objinit_impl(self, vmt);

  self->driver = driver;
  self->mode   = mode;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_node_c instance to be
 *                              disposed.
 */
void __vfsnode_dispose_impl(void *ip) {
  vfs_node_c *self = (vfs_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __ro_dispose_impl(self);
}

/**
 * @brief       Implementation of method @p vfsNodeStat().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_node_c instance.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
msg_t __vfsnode_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_node_c *self = (vfs_node_c *)ip;

  sp->mode = self->mode;
  sp->size = (vfs_offset_t)0;

  return CH_RET_SUCCESS;
}
/** @} */

/*===========================================================================*/
/* Module class "vfs_directory_node_c" methods.                              */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_directory_node_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_directory_node_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
void *__vfsdir_objinit_impl(void *ip, const void *vmt, vfs_driver_c *driver,
                            vfs_mode_t mode) {
  vfs_directory_node_c *self = (vfs_directory_node_c *)ip;

  /* Initialization code.*/
  self = __vfsnode_objinit_impl(ip, vmt, driver, mode);

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_directory_node_c instance
 *                              to be disposed.
 */
void __vfsdir_dispose_impl(void *ip) {
  vfs_directory_node_c *self = (vfs_directory_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsnode_dispose_impl(self);
}

/**
 * @brief       Implementation of method @p vfsDirReadFirst().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_directory_node_c instance.
 * @param[out]    dip           Pointer to a @p vfs_direntry_info_t structure.
 * @return                      The operation result.
 */
msg_t __vfsdir_first_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_directory_node_c *self = (vfs_directory_node_c *)ip;

  (void)self;
  (void)dip;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vfsDirReadNext().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_directory_node_c instance.
 * @param[out]    dip           Pointer to a @p vfs_direntry_info_t structure.
 * @return                      The operation result.
 */
msg_t __vfsdir_next_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_directory_node_c *self = (vfs_directory_node_c *)ip;

  (void)self;
  (void)dip;

  return CH_RET_ENOSYS;
}
/** @} */

/*===========================================================================*/
/* Module class "vfs_file_node_c" methods.                                   */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of vfs_file_node_c
 * @{
 */
/**
 * @brief       Implementation of interface method @p stmWrite().
 *
 * @param[in,out] ip            Pointer to the @p random_stream_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __vfsfile_rstm_write_impl(void *ip, const uint8_t *bp, size_t n) {
  vfs_file_node_c *self = oopIfGetOwner(vfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileWrite((void *)self, bp, n);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
}

/**
 * @brief       Implementation of interface method @p stmRead().
 *
 * @param[in,out] ip            Pointer to the @p random_stream_i class
 *                              interface.
 * @param[out]    bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __vfsfile_rstm_read_impl(void *ip, uint8_t *bp, size_t n) {
  vfs_file_node_c *self = oopIfGetOwner(vfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileRead((void *)self, bp, n);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
}

/**
 * @brief       Implementation of interface method @p stmPut().
 *
 * @param[in,out] ip            Pointer to the @p random_stream_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
static int __vfsfile_rstm_put_impl(void *ip, uint8_t b) {
  vfs_file_node_c *self = oopIfGetOwner(vfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileWrite((void *)self, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return msg;
}

/**
 * @brief       Implementation of interface method @p stmGet().
 *
 * @param[in,out] ip            Pointer to the @p random_stream_i class
 *                              interface.
 * @return                      A byte value from the stream.
 */
static int __vfsfile_rstm_get_impl(void *ip) {
  vfs_file_node_c *self = oopIfGetOwner(vfs_file_node_c, ip);
  msg_t msg;
  uint8_t b;

  msg = vfsFileRead((void *)self, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return (msg_t)b;
}

/**
 * @brief       Implementation of interface method @p stmUnget().
 *
 * @param[in,out] ip            Pointer to the @p random_stream_i class
 *                              interface.
 * @param[in]     b             The byte value to be pushed back to the stream.
 * @return                      The operation status.
 */
static int __vfsfile_rstm_unget_impl(void *ip, int b) {
  vfs_file_node_c *self = oopIfGetOwner(vfs_file_node_c, ip);
  msg_t msg;

  /* For seekable file nodes, unget is implemented as a seek-back by one
     byte. The parameter b is ignored because re-reading the position
     will return the original byte from the file.*/
  (void)b;

  msg = vfsFileSetPosition((void *)self, (vfs_offset_t)-1, VFS_SEEK_CUR);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_RESET;
  }

  return STM_OK;
}

/**
 * @brief       Implementation of interface method @p rstmSeek().
 *
 * @param[in,out] ip            Pointer to the @p random_stream_i class
 *                              interface.
 * @param[in]     offset        The file offset.
 * @param[in]     whence        Seek mode.
 * @return                      Upon successful completion, lseek() returns the
 *                              resulting offset location as measured in bytes
 *                              from the beginning of the file.
 */
static uint32_t __vfsfile_rstm_seek_impl(void *ip, uint32_t offset, int whence) {
  vfs_file_node_c *self = oopIfGetOwner(vfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileSetPosition((void *)self, (vfs_offset_t)offset,
                           (vfs_seekmode_t)whence);
  if (CH_RET_IS_ERROR(msg)) {

    return 0U;
  }

  return (uint32_t)vfsFileGetPosition((void *)self);
}
/** @} */

/**
 * @name        Methods implementations of vfs_file_node_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_file_node_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
void *__vfsfile_objinit_impl(void *ip, const void *vmt, vfs_driver_c *driver,
                             vfs_mode_t mode) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  /* Initialization of interface random_stream_i.*/
  {
    static const struct random_stream_vmt vfsfile_rstm_vmt = {
      .instance_offset      = offsetof(vfs_file_node_c, rstm),
      .write                = __vfsfile_rstm_write_impl,
      .read                 = __vfsfile_rstm_read_impl,
      .put                  = __vfsfile_rstm_put_impl,
      .get                  = __vfsfile_rstm_get_impl,
      .unget                = __vfsfile_rstm_unget_impl,
      .seek                 = __vfsfile_rstm_seek_impl
    };
    oopIfObjectInit(&self->rstm, &vfsfile_rstm_vmt);
  }

  /* Initialization code.*/
  self = __vfsnode_objinit_impl(ip, vmt, driver, mode);

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_file_node_c instance to be
 *                              disposed.
 */
void __vfsfile_dispose_impl(void *ip) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsnode_dispose_impl(self);
}

/**
 * @brief       Implementation of method @p vfsFileRead().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_file_node_c instance.
 * @param[out]    buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
ssize_t __vfsfile_read_impl(void *ip, uint8_t *buf, size_t n) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  (void)self;
  (void)buf;
  (void)n;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vfsFileWrite().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_file_node_c instance.
 * @param[in]     buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
ssize_t __vfsfile_write_impl(void *ip, const uint8_t *buf, size_t n) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  (void)self;
  (void)buf;
  (void)n;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vfsFileSetPosition().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_file_node_c instance.
 * @param[in]     offset        Offset to be applied.
 * @param[in]     whence        Seek mode to be used.
 * @return                      The operation result.
 */
msg_t __vfsfile_setpos_impl(void *ip, vfs_offset_t offset,
                            vfs_seekmode_t whence) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  (void)self;
  (void)offset;
  (void)whence;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vfsFileGetPosition().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_file_node_c instance.
 * @return                      The current file position.
 */
vfs_offset_t __vfsfile_getpos_impl(void *ip) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  (void)self;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vfsFileGetStream().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_file_node_c instance.
 * @return                      Pointer to the random stream interface.
 */
random_stream_i *__vfsfile_getstream_impl(void *ip) {
  vfs_file_node_c *self = (vfs_file_node_c *)ip;

  return &self->rstm;
}
/** @} */

/** @} */
