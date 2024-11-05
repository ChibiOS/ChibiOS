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

static const char *get_current_directory(vfs_littlefs_driver_c *drvp) {
  const char *cwd = drvp->path_cwd;

  /* In case it has not yet been defined using root.*/
  if (cwd == NULL) {
    return "/";
  }

  return cwd;
}

static msg_t build_absolute_path(vfs_littlefs_driver_c *drvp,
                                 char *buf,
                                 const char *path) {
  msg_t ret;

  do {

    /* Initial buffer state, empty string.*/
    *buf = '\0';

    /* Relative paths handling.*/
    if (!vfs_path_is_separator(*path)) {
      if (vfs_path_append(buf,
                      get_current_directory(drvp),
                      VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
        ret = CH_RET_ENAMETOOLONG;
        break;
      }
    }

    /* Adding the specified path.*/
    if (vfs_path_append(buf, path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    /* Normalization of the absolute path.*/
    if (vfs_path_normalize(buf, buf, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    ret = CH_RET_SUCCESS;

  } while (false);

  return ret;
}

static enum lfs_open_flags translate_oflag(int oflag) {

  switch (oflag & VO_SUPPORTED_FLAGS_MASK) {
  case VO_RDONLY:                                   /* r */
    return LFS_O_RDONLY;
  case VO_RDWR:                                     /* r+ */
    return LFS_O_RDWR;
  case VO_CREAT | VO_APPEND | VO_WRONLY:            /* a */
    return LFS_O_CREAT | LFS_O_APPEND | LFS_O_WRONLY;
  case VO_CREAT | VO_APPEND | VO_RDWR:              /* a+ */
    return LFS_O_CREAT | LFS_O_APPEND | LFS_O_RDWR;
  case VO_CREAT | VO_WRONLY:                        /* w */
    return LFS_O_CREAT | LFS_O_WRONLY;
  case VO_CREAT | VO_RDWR:                          /* w+ */
    return LFS_O_CREAT | LFS_O_RDWR;
  case VO_CREAT | VO_EXCL | VO_WRONLY:              /* wx */
    return LFS_O_CREAT | LFS_O_EXCL | LFS_O_WRONLY;
  case VO_CREAT | VO_EXCL | VO_RDWR:                /* w+x */
    return LFS_O_CREAT | LFS_O_EXCL | LFS_O_RDWR;
  default:
    break;
  }

  return 0;
}

static vfs_mode_t translate_mode(enum lfs_open_flags fattrib) {
  vfs_mode_t mode = VFS_MODE_S_IFREG;

  if (fattrib & LFS_O_RDONLY) {
    mode |= VFS_MODE_S_IRUSR;
    return mode;
  }

  if (fattrib & LFS_O_WRONLY) {
    mode |= VFS_MODE_S_IWUSR;
    return mode;
  }

  if (fattrib & LFS_O_RDWR) {
    mode |= VFS_MODE_S_IRUSR | VFS_MODE_S_IWUSR;
  }

  return mode;
}

static msg_t translate_error(enum lfs_error res) {
  msg_t msg;

  switch (res) {
  case LFS_ERR_OK:
    msg = CH_RET_SUCCESS;
    break;

  case LFS_ERR_NOMEM:
    msg = CH_RET_ENOMEM;
    break;

  case LFS_ERR_IO:
    msg = CH_RET_EIO;
    break;

  case LFS_ERR_BADF:
    msg = CH_RET_ENOENT;
    break;

  case LFS_ERR_NAMETOOLONG:
    msg = CH_RET_ENAMETOOLONG;
    break;

  case LFS_ERR_EXIST:
    msg = CH_RET_EEXIST;
    break;

  case LFS_ERR_ISDIR:
    msg = CH_RET_EISDIR;
    break;

  case LFS_ERR_NOTDIR:
    msg = CH_RET_ENOTDIR;
    break;

  default:
    msg = CH_RET_INNER_ERROR;
    break;
  }

  return msg;
}

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
                     sizeof (vfs_littlefs_driver_c),
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
    chPoolLoadArray(&vfs_littlefs_driver_static.info_nodes_pool,
                    &vfs_littlefs_driver_static.info_nodes[0],
                    DRV_CFG_LITTLEFS_INFO_NODES_NUM);

    /* Initialise driver objects.*/
    extern const struct vfs_littlefs_driver_vmt __vfs_littlefs_driver_vmt;
    int i;
    for (i = 0; i < DRV_CFG_LITTLEFS_FS_NUM; i++) {
      __lfsdrv_objinit_impl(&vfs_littlefs_driver_static.fs[i],
                                              &__vfs_littlefs_driver_vmt);
    }
}

/**
 * @brief       Mounts a LittleFS volume.
 *
 * @param[in]     name          Name to be assigned to the volume.
 * @param[in]     cfgp          Pointer to @p lfs_config configuration.
 * @param[in,out] obj           Pointer to @ vfs_littlefs_driver_c pointer.
 * @return                      The operation result.
 *
 * @api
 */
msg_t lfsdrvMount(const char *name, const struct lfs_config *cfgp,
                                            vfs_littlefs_driver_c **obj) {

  osalDbgAssert(cfgp != NULL, "null config");
  osalDbgAssert(name != NULL, "null volume name");
  osalDbgAssert(*name != '\0', "blank volume name");

  vfs_littlefs_driver_c *fs;
  int i;

  /* Check the volume name is not already in use.*/
  for (i = 0; i < DRV_CFG_LITTLEFS_FS_NUM; i++) {
    if (vfs_littlefs_driver_static.fs[i].name != NULL) {
      if (strcmp(vfs_littlefs_driver_static.fs[i].name, name) == 0) {
        return CH_RET_EEXIST;
      }
    }
  }

  /* Allocate a drive object.*/
  fs = (vfs_littlefs_driver_c *)chPoolAlloc(&vfs_littlefs_driver_static.fs_pool);
  if (fs == NULL) {
    return CH_RET_ENOMEM;
  }

  /* Initialise and mount the drive.*/
  if (lfsdrvObjectInit(fs, cfgp) != NULL) {

    /* Set the volume name and the returned object reference on success.*/
    fs->name = name;
    *obj = fs;
    return CH_RET_SUCCESS;
  }

  chPoolFree(&vfs_littlefs_driver_static.fs_pool, (void *)fs);
  return CH_RET_EIO;
}

/**
 * @brief       Unmounts a LittleFS volume.
 *
 * @param[in]     name          Name of volume to be unmounted.
 * @return                      The operation result.
 *
 * @api
 */
msg_t lfsdrvUnmount(const char *name) {

  osalDbgAssert(name != NULL, "null volume name");
  osalDbgAssert(*name != '\0', "blank volume name");

  int res;
  int i;

  for (i = 0; i < DRV_CFG_LITTLEFS_FS_NUM; i++) {
    if (strcmp(vfs_littlefs_driver_static.fs[i].name, name) == 0) {

      /* Unmount from LFS.*/
      res = lfs_unmount(&vfs_littlefs_driver_static.fs[i].lfs);

      /* Dispose the driver object.*/
      __lfsdrv_dispose_impl(&vfs_littlefs_driver_static.fs[i]);
      return translate_error(res);
    }
  }

  return CH_RET_EINVAL;
}

/**
 * @brief       Gets name of LittleFS volume.
 *
 * @param[in]     drvp          Pointer to driver object.
 * @return                      pointer to driver volume name.
 *
 * @api
 */
const char *lfsdrvGetName(vfs_littlefs_driver_c *drvp) {
  return drvp->name;
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
static void *__lfsdir_objinit_impl(void *ip, const void *vmt,
                                   vfs_driver_c *driver, vfs_mode_t mode) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;
  vfs_littlefs_driver_c *fs = (vfs_littlefs_driver_c *)driver;

  /* Initialization code.*/
  self = __vfsdir_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);

  /* Initialise object.*/
  self->lfs = &fs->lfs;
  self->path[0] = '\0';

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
static void __lfsdir_dispose_impl(void *ip) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;

  /* Closing directory.*/
  (void)lfs_dir_close(self->lfs, &self->dir);

  /* Finalization of the ancestors-defined parts.*/
  __vfsdir_dispose_impl(self);

  /* Free the object.*/
  chPoolFree(&vfs_littlefs_driver_static.dir_nodes_pool, ip);
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
static msg_t __lfsdir_stat_impl(void *ip, vfs_stat_t *sp) {

  return __vfsnode_stat_impl(ip, sp);
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
static msg_t __lfsdir_next_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;
  msg_t ret;

  do {
    int res;
    struct lfs_info *lfsip;

    lfsip = (struct lfs_info *)chPoolAlloc(&vfs_littlefs_driver_static.info_nodes_pool);
    if (lfsip != NULL) {
      do {
        res = lfs_dir_read(self->lfs, &self->dir, lfsip);
        if (res == 0) {

          /* End of directory.*/
          (void) lfs_dir_rewind(self->lfs, &self->dir);
          ret = (msg_t)0;
          break;
        }
        else if (res > 0) {

          /* Skip over self and parent entries.*/
          if (lfsip->type == LFS_TYPE_DIR &&
                      (strcmp(lfsip->name, ".") == 0 ||
                       strcmp(lfsip->name, "..") == 0)) {
            continue;
          }
          dip->mode = lfsip->type == LFS_TYPE_REG ? VFS_MODE_S_IFREG : VFS_MODE_S_IFDIR;
          dip->size = (vfs_offset_t)lfsip->size;
          strncpy(dip->name, lfsip->name, VFS_CFG_NAMELEN_MAX);
          dip->name[VFS_CFG_NAMELEN_MAX] = '\0';
          ret = (msg_t)1;
          break;
        }
        else {
          ret = translate_error(res);
          break;
        }
      } while (true);
      chPoolFree(&vfs_littlefs_driver_static.info_nodes_pool, (void *)lfsip);
    }
    else {
      ret = CH_RET_ENOMEM;
      break;
    }
  }
  while (false);

  return ret;
}
/** @} */

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
static msg_t __lfsdir_first_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_littlefs_dir_node_c *self = (vfs_littlefs_dir_node_c *)ip;

  msg_t ret;
  int res;

  do {
    res = lfs_dir_rewind(self->lfs, &self->dir);
    if (res == LFS_ERR_OK) {
      ret = __lfsdir_next_impl(ip, dip);
      break;
    }

    ret = translate_error(res);
  }
  while (false);

  return ret;
}

/**
 * @brief       VMT structure of VFS littlefs directory node class.
 */
static const struct vfs_littlefs_dir_node_vmt __vfs_littlefs_dir_node_vmt = {
  .dispose                  = __lfsdir_dispose_impl,
  .addref                   = __ro_addref_impl,
  .release                  = __ro_release_impl,
  .stat                     = __lfsdir_stat_impl,
  .first                    = __lfsdir_first_impl,
  .next                     = __lfsdir_next_impl
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
static vfs_littlefs_dir_node_c *lfsdirObjectInit(vfs_littlefs_dir_node_c *self,
                                                 vfs_driver_c *driver,
                                                 vfs_mode_t mode) {

  return __lfsdir_objinit_impl(self, &__vfs_littlefs_dir_node_vmt, driver,
                               mode);
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
static size_t __lfsfile_stm_write_impl(void *ip, const uint8_t *bp, size_t n) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileWrite((void *)self, bp, n);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
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
static size_t __lfsfile_stm_read_impl(void *ip, uint8_t *bp, size_t n) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileRead((void *)self, bp, n);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
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
static int __lfsfile_stm_put_impl(void *ip, uint8_t b) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileWrite((void *)self, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return msg;
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
static int __lfsfile_stm_get_impl(void *ip) {
  vfs_littlefs_file_node_c *self = oopIfGetOwner(vfs_littlefs_file_node_c, ip);
  msg_t msg;
  uint8_t b;

  msg = vfsFileRead((void *)self, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return (msg_t)b;
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
static void *__lfsfile_objinit_impl(void *ip, const void *vmt,
                                    vfs_driver_c *driver, vfs_mode_t mode) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
  vfs_littlefs_driver_c *fs = (vfs_littlefs_driver_c *)driver;

  /* Initialization of interface sequential_stream_i.*/
  {
    static const struct sequential_stream_vmt lfsfile_stm_vmt = {
      .instance_offset      = offsetof(vfs_littlefs_file_node_c, stm),
      .write                = __lfsfile_stm_write_impl,
      .read                 = __lfsfile_stm_read_impl,
      .put                  = __lfsfile_stm_put_impl,
      .get                  = __lfsfile_stm_get_impl,
      .unget                = NULL /* Missing implementation.*/
    };
    oopIfObjectInit(&self->stm, &lfsfile_stm_vmt);
  }

  /* Initialization code.*/
  self = __vfsfile_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);

  /* Initialise object.*/
  self->lfs = &fs->lfs;
  self->path[0] = '\0';

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
static void __lfsfile_dispose_impl(void *ip) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;

  /* Closing file.*/
  (void) lfs_file_close(self->lfs, &self->file);

  /* Finalization of the ancestors-defined parts.*/
  __vfsfile_dispose_impl(ip);

  /* Release object.*/
  chPoolFree(&vfs_littlefs_driver_static.file_nodes_pool, ip);
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
static msg_t __lfsfile_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
  lfs_soff_t offset;

  offset = lfs_file_size(self->lfs, &self->file);
  if (offset < 0) {
    return translate_error(offset);
  }
  sp->size = offset;
  sp->mode = self->mode;

  return CH_RET_SUCCESS;
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
static ssize_t __lfsfile_read_impl(void *ip, uint8_t *buf, size_t n) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
  lfs_ssize_t br;

  br = lfs_file_read(self->lfs, &self->file, (void *)buf, (lfs_ssize_t)n);
  if (br < 0) {

    return translate_error(br);
  }

  return (ssize_t)br;
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
static ssize_t __lfsfile_write_impl(void *ip, const uint8_t *buf, size_t n) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
  lfs_ssize_t bw;

  bw = lfs_file_write(self->lfs, &self->file, (const void *)buf, (lfs_ssize_t)n);
  if (bw < 0) {

    return (ssize_t)translate_error(bw);
  }

  return (ssize_t)bw;
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
static msg_t __lfsfile_setpos_impl(void *ip, vfs_offset_t offset,
                                   vfs_seekmode_t whence) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
  lfs_soff_t finaloff;

  osalDbgCheck((whence == SEEK_SET) ||
               (whence == SEEK_CUR) ||
               (whence == SEEK_END));

  finaloff = lfs_file_seek(self->lfs, &self->file, (lfs_soff_t)offset, (int)whence);

  if (finaloff < 0) {
    return translate_error(finaloff);
  }

  return CH_RET_SUCCESS;
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
static vfs_offset_t __lfsfile_getpos_impl(void *ip) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;
  lfs_soff_t offset;

  offset = lfs_file_tell(self->lfs, &self->file);
  if (offset < 0) {
    return translate_error(offset);
  }
  return (vfs_offset_t)offset;
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
static sequential_stream_i *__lfsfile_getstream_impl(void *ip) {
  vfs_littlefs_file_node_c *self = (vfs_littlefs_file_node_c *)ip;

  return &self->stm;
}
/** @} */

/**
 * @brief       VMT structure of VFS littlefs file node class.
 */
static const struct vfs_littlefs_file_node_vmt __vfs_littlefs_file_node_vmt = {
  .dispose                  = __lfsfile_dispose_impl,
  .addref                   = __ro_addref_impl,
  .release                  = __ro_release_impl,
  .stat                     = __lfsfile_stat_impl,
  .read                     = __lfsfile_read_impl,
  .write                    = __lfsfile_write_impl,
  .setpos                   = __lfsfile_setpos_impl,
  .getpos                   = __lfsfile_getpos_impl,
  .getstream                = __lfsfile_getstream_impl
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
static vfs_littlefs_file_node_c *lfsfileObjectInit(vfs_littlefs_file_node_c *self,
                                                   vfs_driver_c *driver,
                                                   vfs_mode_t mode) {

  return __lfsfile_objinit_impl(self, &__vfs_littlefs_file_node_vmt, driver,
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

  /* Set references to unallocated.*/
  self->name = NULL;
  self->cwd = NULL;
  self->path_cwd[0] = '\0';

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

  /* Finalization of the ancestors-defined parts.*/
  __vfsdrv_dispose_impl(self);

  /* Set volume name to empty (unused).*/
  self->name = NULL;
  self->cwd = NULL;
  self->path_cwd[0] = '\0';

  /* Release object.*/
  chPoolFree(&vfs_littlefs_driver_static.fs_pool, (void *)self);
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

  msg_t ret;

  /* Close the current directory. This will flush any pending data in LFS.*/
  if (self->cwd != NULL) {
    __lfsdir_dispose_impl(self->cwd);
    self->cwd = NULL;
  }

  /* Build the path for the directory.*/
  ret = build_absolute_path(self, self->path_cwd, path);
  CH_RETURN_ON_ERROR(ret);

  /* Open the new directory.*/
  vfs_directory_node_c *vdnp;
  ret = __lfsdrv_opendir_impl(self, self->path_cwd, &vdnp);
  if (ret == CH_RET_SUCCESS) {

    /* Set the current directory reference.*/
    self->cwd = (vfs_littlefs_dir_node_c *)vdnp;
  }

  return ret;
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

  msg_t ret;

  if (self->cwd == NULL) {
    ret = __lfsdrv_setcwd_impl(self, "/");
    CH_RETURN_ON_ERROR(ret);
  }

  /* Copy out the directory name.*/
  strncpy(buf, self->path_cwd, size);

  return CH_RET_SUCCESS;
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
  msg_t ret;

  do {
    int res;
    struct lfs_info *lfsip;

    /* Check for no path.*/
    if (vfs_parse_match_end(&path) == CH_RET_SUCCESS) {

      /* If no path then report on the file system usage.*/
      lfs_ssize_t blocks = lfs_fs_size(&self->lfs);
      if (blocks >= 0) {
        sp->size = (vfs_offset_t)blocks;
        sp->mode = VFS_MODE_S_IFBLK;
        ret = CH_RET_SUCCESS;
        break;
      }
      ret = translate_error((int)blocks);
      break;
    }

    lfsip = (struct lfs_info *)chPoolAlloc(&vfs_littlefs_driver_static.info_nodes_pool);
    if (lfsip != NULL) {

      /* Build the path for the directory or file.*/
      ret = build_absolute_path(self, self->scratch, path);
      if (ret == CH_RET_SUCCESS) {

        res = lfs_stat(&self->lfs, self->scratch, lfsip);
        if (res == LFS_ERR_OK) {

          sp->size = (vfs_offset_t)lfsip->size;
          if (lfsip->type == LFS_TYPE_REG) {
            sp->mode = VFS_MODE_S_IFREG;
          }
          else {
            sp->mode = VFS_MODE_S_IFDIR;
          }
          ret = CH_RET_SUCCESS;
        }
        else {
          ret = translate_error(res);
        }
      }
      chPoolFree(&vfs_littlefs_driver_static.info_nodes_pool, (void *)lfsip);
      break;
    }
    ret = CH_RET_ENOMEM;
  }
  while (false);

  return ret;
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

  msg_t ret;

  do {
    vfs_littlefs_dir_node_c *lfsdnp;
    int res;

    lfsdnp = chPoolAlloc(&vfs_littlefs_driver_static.dir_nodes_pool);
    if (lfsdnp != NULL) {

      /* Node object initialization.*/
      (void) lfsdirObjectInit(lfsdnp, (vfs_driver_c *)self,
          (VFS_MODE_S_IRWXU | VFS_MODE_S_IFDIR));

      /* Build the path for the directory.*/
      ret = build_absolute_path(self, lfsdnp->path, path);
      if (ret == CH_RET_SUCCESS) {

        /* Open the directory.*/
        res = lfs_dir_open(&self->lfs, &lfsdnp->dir, lfsdnp->path);
        if (res == LFS_ERR_OK) {

          *vdnpp = (vfs_directory_node_c *)lfsdnp;
          return CH_RET_SUCCESS;
        }
      }
      ret = translate_error(res);
      chPoolFree(&vfs_littlefs_driver_static.dir_nodes_pool, (void *)lfsdnp);
      break;
    }
    ret = CH_RET_ENOMEM;
  } while (false);

  return ret;
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

  msg_t ret;

  do {
    vfs_littlefs_file_node_c *lfsfnp;
    int res;
    enum lfs_open_flags mode;

    mode = translate_oflag(flags);
    if (mode == 0) {
      ret = CH_RET_EINVAL;
      break;
    }

    lfsfnp = chPoolAlloc(&vfs_littlefs_driver_static.file_nodes_pool);
    if (lfsfnp != NULL) {

      /* Node object initialization. Note: reference to lfs is set in object.*/
      (void) lfsfileObjectInit(lfsfnp,
                              (vfs_driver_c *)self,
                              translate_mode(lfsfnp->file.flags));

      /* Build the path for the directory.*/
      ret = build_absolute_path(self, lfsfnp->path, path);
      if (ret == CH_RET_SUCCESS) {
        res = lfs_file_open(&self->lfs, &lfsfnp->file, lfsfnp->path, mode);
        if (res == LFS_ERR_OK) {
          *vfnpp = (vfs_file_node_c *)lfsfnp;
          return CH_RET_SUCCESS;
        }
      }
      ret = translate_error(res);
      chPoolFree(&vfs_littlefs_driver_static.file_nodes_pool, (void *)lfsfnp);
      break;
    }
    ret = CH_RET_ENOMEM;
  }
  while (false);

  return ret;
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
  int res;
  msg_t ret;

  /* Build the path for the directory.*/
  ret = build_absolute_path(self, self->scratch, path);
  CH_RETURN_ON_ERROR(ret);

  /* Check if it is the root.*/
  if (self->scratch[1] == '\0') {
    ret = CH_RET_EISDIR;
  }

  res = lfs_remove(&self->lfs, self->scratch);
  if (res != LFS_ERR_OK) {
    return translate_error(res);
  }

  /* Close to flush and re-open current directory.*/
  return __lfsdrv_setcwd_impl(ip, ".");
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
  int res;

  res = lfs_rename(&self->lfs, oldpath, newpath);
  if (res != LFS_ERR_OK) {
    return translate_error(res);
  }

  /* Close to flush and re-open current directory.*/
  return __lfsdrv_setcwd_impl(ip, ".");
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
  int res;
  msg_t ret;
  (void)mode; /* Not handled by LittleFS.*/

  /* Build the path for the directory.*/
  ret = build_absolute_path(self, self->scratch, path);
  if (ret != CH_RET_SUCCESS) {
    return ret;
  }

  res = lfs_mkdir(&self->lfs, self->scratch);
  if (res != LFS_ERR_OK) {
    return translate_error(res);
  }

  /* Close to flush and re-open current directory.*/
  return __lfsdrv_setcwd_impl(ip, ".");
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
  int res;
  msg_t ret;

  if (strcmp(path, ".") == 0) {
    /* Can't remove current.*/
    return CH_RET_EACCES;
  }

  /* Build the path for the directory.*/
  ret = build_absolute_path(self, self->scratch, path);
  if (ret != CH_RET_SUCCESS) {
    return ret;
  }

  if (strcmp(self->scratch, "/") == 0) {
    /* Can't remove root.*/
    return CH_RET_EACCES;
  }

  res = lfs_remove(&self->lfs, self->scratch);
  if (res != LFS_ERR_OK) {
    return translate_error(res);
  }

  /* Close to flush and re-open current directory.*/
  return __lfsdrv_setcwd_impl(ip, ".");
}
/** @} */

/**
 * @brief       VMT structure of VFS LittleFS driver class.
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

