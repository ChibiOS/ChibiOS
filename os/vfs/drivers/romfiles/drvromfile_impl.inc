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
 * @file        drvromfile_impl.inc
 * @brief       Template of VFS RomFile Driver source.
 * @note        This is a template file, can be edited directly.
 *
 * @addtogroup  DRVROMFILE
 * @{
 */

/* This is an, automatically generated, implementation file that can be
   manually edited, it is not re-generated if already present.*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "vfs_romfiles_driver_c" methods.                             */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of vfs_romfiles_driver_c
 * @{
 */
/**
 * @brief       Implementation of interface method @p stmWrite().
 *
 * @param[in,out] ip            Pointer to the @p sequential_stream_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __romfile_stm_write_impl(void *ip, const uint8_t *bp, size_t n) {
  vfs_romfiles_driver_c *self = oopIfGetOwner(vfs_romfiles_driver_c, ip);
}

/**
 * @brief       Implementation of interface method @p stmRead().
 *
 * @param[in,out] ip            Pointer to the @p sequential_stream_i class
 *                              interface.
 * @param[out]    bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __romfile_stm_read_impl(void *ip, uint8_t *bp, size_t n) {
  vfs_romfiles_driver_c *self = oopIfGetOwner(vfs_romfiles_driver_c, ip);
}

/**
 * @brief       Implementation of interface method @p stmPut().
 *
 * @param[in,out] ip            Pointer to the @p sequential_stream_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
static int __romfile_stm_put_impl(void *ip, uint8_t b) {
  vfs_romfiles_driver_c *self = oopIfGetOwner(vfs_romfiles_driver_c, ip);
}

/**
 * @brief       Implementation of interface method @p stmGet().
 *
 * @param[in,out] ip            Pointer to the @p sequential_stream_i class
 *                              interface.
 * @return                      A byte value from the stream.
 */
static int __romfile_stm_get_impl(void *ip) {
  vfs_romfiles_driver_c *self = oopIfGetOwner(vfs_romfiles_driver_c, ip);
}
/** @} */

/**
 * @name        Methods implementations of vfs_romfiles_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_romfiles_driver_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     romdatap      Pointer to @p vfs_romfile_t data.
 * @return                      A new reference to the object.
 */
void *__romfile_objinit_impl(void *ip, const void *vmt,
                             const vfs_romfile_t *romdatap) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __vfsfile_objinit_impl(self, vmt);

  /* Initialization of interface sequential_stream_i.*/
  {
    static const struct sequential_stream_vmt romfile_stm_vmt = {
      .instance_offset      = offsetof(vfs_romfiles_driver_c, stm),
      .write                = __romfile_stm_write_impl,
      .read                 = __romfile_stm_read_impl,
      .put                  = __romfile_stm_put_impl,
      .get                  = __romfile_stm_get_impl,
      .unget                = NULL /* Missing implementation.*/
    };
    oopIfObjectInit(&self->stm, &romfile_stm_vmt);
  }

  /* No initialization code.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance
 *                              to be disposed.
 */
void __romfile_dispose_impl(void *ip) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsfile_dispose_impl(self);
}

/**
 * @brief       Override of method @p vfsNodeStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
msg_t __romfile_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;
}

/**
 * @brief       Override of method @p vfsFileRead().
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance.
 * @param[out]    buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
ssize_t __romfile_read_impl(void *ip, uint8_t *buf, size_t n) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;
}

/**
 * @brief       Override of method @p vfsFileWrite().
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance.
 * @param[in]     buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
ssize_t __romfile_write_impl(void *ip, const uint8_t *buf, size_t n) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;
}

/**
 * @brief       Override of method @p vfsFileSetPosition().
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance.
 * @param[in]     offset        Offset to be applied.
 * @param[in]     whence        Seek mode to be used.
 * @return                      The operation result.
 */
msg_t __romfile_setpos_impl(void *ip, vfs_offset_t offset,
                            vfs_seekmode_t whence) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;
}

/**
 * @brief       Override of method @p vfsFileGetPosition().
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance.
 * @return                      The current file position.
 */
vfs_offset_t __romfile_getpos_impl(void *ip) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;
}

/**
 * @brief       Override of method @p vfsFileGetStream().
 *
 * @param[in,out] ip            Pointer to a @p vfs_romfiles_driver_c instance.
 * @return                      Pointer to the HAL stream interface.
 */
sequential_stream_i *__romfile_getstream_impl(void *ip) {
  vfs_romfiles_driver_c *self = (vfs_romfiles_driver_c *)ip;
}
/** @} */

/**
 * @brief       VMT structure of VFS ROMFiles driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct vfs_romfiles_driver_vmt __vfs_romfiles_driver_vmt = {
  .dispose                  = __romfile_dispose_impl,
  .addref                   = __ro_addref_impl,
  .release                  = __ro_release_impl,
  .stat                     = __romfile_stat_impl,
  .read                     = __romfile_read_impl,
  .write                    = __romfile_write_impl,
  .setpos                   = __romfile_setpos_impl,
  .getpos                   = __romfile_getpos_impl,
  .getstream                = __romfile_getstream_impl
};

/** @} */

/** @} */

/** @} */

/** @} */

/** @} */

/** @} */

/** @} */

