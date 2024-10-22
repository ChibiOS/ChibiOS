/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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

/* This is an, automatically generated, implementation file that can be
   manually edited, it is not re-generated if already present.*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       Module initialization.
 *
 * @init
 */
void __drv_littlefs_init(void) {

    /* Initializing pools.*/
    chPoolObjectInit(&vfs_littlefs_driver_static.dir_nodes_pool,
                     sizeof (vfs_littlefs_dir_node_c),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_littlefs_driver_static.file_nodes_pool,
                     sizeof (vfs_littlefs_file_node_c),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_littlefs_driver_static.info_nodes_pool,
                     sizeof (struct lfs_info),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_littlefs_driver_static.fs_pool,
                     sizeof (lfs_t),
                     chCoreAllocAlignedI);

    /* Preloading pools.*/
    chPoolLoadArray(&vfs_littlefs_driver_static.dir_nodes_pool,
                    &vfs_littlefs_driver_static.dir_nodes[0],
                    DRV_CFG_LITTLEFS_DIR_NODES_NUM);
    chPoolLoadArray(&vfs_littlefs_driver_static.file_nodes_pool,
                    &vfs_littlefs_driver_static.file_nodes[0],
                    DRV_CFG_LITTLEFS_FILE_NODES_NUM);
    chPoolLoadArray(&vfs_littlefs_driver_static.fs_pool,
                    &vfs_littlefs_driver_static.fs[0],
                    DRV_CFG_LITTLEFS_FS_NUM);
}

/**
 * @brief       Mounts a LittleFS volume.
 *
 * @param[in]     name          Name to be assigned to the volume, see LittleFS
 *                              @p f_mount() documentation because there are
 *                              several options.
 * @param[in]     config        LFS configuration for the mounted file system.
 * @return                      The operation result.
 *
 * @api
 */
msg_t lfsdrvMount(const char *name, const struct lfsdrvMount *config) {
}

/**
 * @brief       Unmounts a LittleFS volume.
 *
 * @param[in]     name          Name of the volume to be unmounted.
 * @return                      The operation result.
 *
 * @api
 */
msg_t lfsdrvUnmount(const char *name) {
}

/*===========================================================================*/
/* Module class "vfs_littlefs_dir_node_c" methods.                           */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_littlefs_dir_node_c
 * @{
 */
/**
 * @memberof    vfs_littlefs_dir_node_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_littlefs_dir_node_c
 *                              instance to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
static void *__ffdir_objinit_impl(void *ip, const void *vmt,
                                  vfs_driver_c *driver, vfs_mode_t mode) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;

  /* Initialization code.*/
  self = __vfsdir_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);

  return self;
}

/**
 * @memberof    vfs_littlefs_dir_node_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_dir_node_c
 *                              instance to be disposed.
 */
static void __ffdir_dispose_impl(void *ip) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsdir_dispose_impl(self);
}

/**
 * @memberof    vfs_littlefs_dir_node_c
 * @protected
 *
 * @brief       Override of method @p vfsNodeStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_dir_node_c
 *                              instance.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
static msg_t __ffdir_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_dir_node_c
 * @protected
 *
 * @brief       Override of method @p vfsDirReadFirst().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_dir_node_c
 *                              instance.
 * @param[out]    dip           Pointer to a @p vfs_direntry_info_t structure.
 * @return                      The operation result.
 */
static msg_t __ffdir_first_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_dir_node_c
 * @protected
 *
 * @brief       Override of method @p vfsDirReadNext().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_dir_node_c
 *                              instance.
 * @param[out]    dip           Pointer to a @p vfs_direntry_info_t structure.
 * @return                      The operation result.
 */
static msg_t __ffdir_next_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;
}
/** @} */

/**
 * @brief       VMT structure of VFS littlefs directory node class.
 * @note        It is public because accessed by the inlined constructor.
 */
static const struct vfs_littlefs_dir_node_vmt __vfs_littlefs_dir_node_vmt = {
  .dispose                  = __ffdir_dispose_impl,
  .addref                   = __ro_addref_impl,
  .release                  = __ro_release_impl,
  .stat                     = __ffdir_stat_impl,
  .first                    = __ffdir_first_impl,
  .next                     = __ffdir_next_impl
};

/**
 * @name        Default constructor of vfs_littlefs_dir_node_c
 * @{
 */
/**
 * @memberof    vfs_littlefs_dir_node_c
 *
 * @brief       Default initialization function of @p vfs_littlefs_dir_node_c.
 *
 * @param[out]    self          Pointer to a @p vfs_littlefs_dir_node_c
 *                              instance to be initialized.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
static vfs_littlefs_dir_node_c *ffdirObjectInit(vfs_littlefs_dir_node_c *self,
                                                vfs_driver_c *driver,
                                                vfs_mode_t mode) {

  return __ffdir_objinit_impl(self, &__vfs_littlefs_dir_node_vmt, driver, mode);
}
/** @} */

/*===========================================================================*/
/* Module class "vfs_littlefs_file_node_c" methods.                          */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of vfs_littlefs_file_node_c
 * @{
 */
/**
 * @memberof    vfs_littlefs_file_node_c
 * @private
 *
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
static size_t __fffile_stm_write_impl(void *ip, const uint8_t *bp, size_t n) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @private
 *
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
static size_t __fffile_stm_read_impl(void *ip, uint8_t *bp, size_t n) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @private
 *
 * @brief       Implementation of interface method @p stmPut().
 *
 * @param[in,out] ip            Pointer to the @p sequential_stream_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
static int __fffile_stm_put_impl(void *ip, uint8_t b) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @private
 *
 * @brief       Implementation of interface method @p stmGet().
 *
 * @param[in,out] ip            Pointer to the @p sequential_stream_i class
 *                              interface.
 * @return                      A byte value from the stream.
 */
static int __fffile_stm_get_impl(void *ip) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
}
/** @} */

/**
 * @name        Methods implementations of vfs_littlefs_file_node_c
 * @{
 */
/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
static void *__fffile_objinit_impl(void *ip, const void *vmt,
                                   vfs_driver_c *driver, vfs_mode_t mode) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;

  /* Initialization of interface sequential_stream_i.*/
  {
    static const struct sequential_stream_vmt fffile_stm_vmt = {
      .instance_offset      = offsetof(vfs_littlefs_file_node_c, stm),
      .write                = __fffile_stm_write_impl,
      .read                 = __fffile_stm_read_impl,
      .put                  = __fffile_stm_put_impl,
      .get                  = __fffile_stm_get_impl,
      .unget                = NULL /* Missing implementation.*/
    };
    oopIfObjectInit(&self->stm, &fffile_stm_vmt);
  }

  /* Initialization code.*/
  self = __vfsfile_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);

  return self;
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance to be disposed.
 */
static void __fffile_dispose_impl(void *ip) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsfile_dispose_impl(self);
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Override of method @p vfsNodeStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
static msg_t __fffile_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Override of method @p vfsFileRead().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance.
 * @param[out]    buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
static ssize_t __fffile_read_impl(void *ip, uint8_t *buf, size_t n) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Override of method @p vfsFileWrite().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance.
 * @param[in]     buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
static ssize_t __fffile_write_impl(void *ip, const uint8_t *buf, size_t n) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Override of method @p vfsFileSetPosition().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance.
 * @param[in]     offset        Offset to be applied.
 * @param[in]     whence        Seek mode to be used.
 * @return                      The operation result.
 */
static msg_t __fffile_setpos_impl(void *ip, vfs_offset_t offset,
                                  vfs_seekmode_t whence) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Override of method @p vfsFileGetPosition().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance.
 * @return                      The current file position.
 */
static vfs_offset_t __fffile_getpos_impl(void *ip) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
}

/**
 * @memberof    vfs_littlefs_file_node_c
 * @protected
 *
 * @brief       Override of method @p vfsFileGetStream().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_file_node_c
 *                              instance.
 * @return                      Pointer to the HAL stream interface.
 */
static sequential_stream_i *__fffile_getstream_impl(void *ip) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
}
/** @} */

/**
 * @brief       VMT structure of VFS littlefs file node class.
 * @note        It is public because accessed by the inlined constructor.
 */
static const struct vfs_littlefs_file_node_vmt __vfs_littlefs_file_node_vmt = {
  .dispose                  = __fffile_dispose_impl,
  .addref                   = __ro_addref_impl,
  .release                  = __ro_release_impl,
  .stat                     = __fffile_stat_impl,
  .read                     = __fffile_read_impl,
  .write                    = __fffile_write_impl,
  .setpos                   = __fffile_setpos_impl,
  .getpos                   = __fffile_getpos_impl,
  .getstream                = __fffile_getstream_impl
};

/**
 * @name        Default constructor of vfs_littlefs_file_node_c
 * @{
 */
/**
 * @memberof    vfs_littlefs_file_node_c
 *
 * @brief       Default initialization function of @p vfs_littlefs_file_node_c.
 *
 * @param[out]    self          Pointer to a @p vfs_littlefs_file_node_c
 *                              instance to be initialized.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
static vfs_littlefs_file_node_c *fffileObjectInit(vfs_littlefs_file_node_c *self,
                                                  vfs_driver_c *driver,
                                                  vfs_mode_t mode) {

  return __fffile_objinit_impl(self, &__vfs_littlefs_file_node_vmt, driver,
                               mode);
}
/** @} */

/*===========================================================================*/
/* Module class "vfs_littlefs_driver_c" methods.                             */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_littlefs_driver_c
 * @{
 */
/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_littlefs_driver_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__lfsdrv_objinit_impl(void *ip, const void *vmt) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __vfsdrv_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance
 *                              to be disposed.
 */
void __lfsdrv_dispose_impl(void *ip) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsdrv_dispose_impl(self);
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvChangeCurrentDirectory().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Path of the new current directory.
 * @return                      The operation result.
 */
msg_t __lfsdrv_setcwd_impl(void *ip, const char *path) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvGetCurrentDirectory().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[out]    buf           Buffer for the path string.
 * @param[in]     size          Size of the buffer.
 * @return                      The operation result.
 */
msg_t __lfsdrv_getcwd_impl(void *ip, char *buf, size_t size) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Absolute path of the node to be examined.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
msg_t __lfsdrv_stat_impl(void *ip, const char *path, vfs_stat_t *sp) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvOpenDirectory().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Absolute path of the directory to be opened.
 * @param[out]    vdnpp         Pointer to the pointer to the instantiated @p
 *                              vfs_directory_node_c object.
 * @return                      The operation result.
 */
msg_t __lfsdrv_opendir_impl(void *ip, const char *path,
                            vfs_directory_node_c **vdnpp) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvOpenFile().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Absolute path of the directory to be opened.
 * @param[in]     flags         File open flags.
 * @param[out]    vfnpp         Pointer to the pointer to the instantiated @p
 *                              vfs_file_node_c object.
 * @return                      The operation result.
 */
msg_t __lfsdrv_openfile_impl(void *ip, const char *path, int flags,
                             vfs_file_node_c **vfnpp) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvUnlink().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Path of the file to be unlinked.
 * @return                      The operation result.
 */
msg_t __lfsdrv_unlink_impl(void *ip, const char *path) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvRename().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     oldpath       Path of the node to be renamed.
 * @param[in]     newpath       New path of the renamed node.
 * @return                      The operation result.
 */
msg_t __lfsdrv_rename_impl(void *ip, const char *oldpath, const char *newpath) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvMkdir().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Path of the directory to be created.
 * @param[in]     mode          Mode flags for the directory.
 * @return                      The operation result.
 */
msg_t __lfsdrv_mkdir_impl(void *ip, const char *path, vfs_mode_t mode) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}

/**
 * @memberof    vfs_littlefs_driver_c
 * @protected
 *
 * @brief       Override of method @p vfsDrvRmdir().
 *
 * @param[in,out] ip            Pointer to a @p vfs_littlefs_driver_c instance.
 * @param[in]     path          Path of the directory to be removed.
 * @return                      The operation result.
 */
msg_t __lfsdrv_rmdir_impl(void *ip, const char *path) {
  vfs_littlefs_driver_c *self = (vfs_littlefs_driver_c *)ip;
}
/** @} */

/**
 * @brief       VMT structure of VFS littlefs driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct vfs_littlefs_driver_vmt __vfs_littlefs_driver_vmt = {
  .dispose                  = __lfsdrv_dispose_impl,
  .setcwd                   = __lfsdrv_setcwd_impl,
  .getcwd                   = __lfsdrv_getcwd_impl,
  .stat                     = __lfsdrv_stat_impl,
  .opendir                  = __lfsdrv_opendir_impl,
  .openfile                 = __lfsdrv_openfile_impl,
  .unlink                   = __lfsdrv_unlink_impl,
  .rename                   = __lfsdrv_rename_impl,
  .mkdir                    = __lfsdrv_mkdir_impl,
  .rmdir                    = __lfsdrv_rmdir_impl
};

