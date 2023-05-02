/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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

static vfs_fatfs_dir_node_c *ffdir_object_init(vfs_fatfs_dir_node_c *self,
                                               vfs_driver_c *driver,
                                               vfs_mode_t mode);
static vfs_fatfs_file_node_c *fffile_object_init(vfs_fatfs_file_node_c *self,
                                                 vfs_driver_c *driver,
                                                 vfs_mode_t mode);

static BYTE translate_oflag(int oflag) {

  switch (oflag & VO_SUPPORTED_FLAGS_MASK) {
  case VO_RDONLY:                                   /* r */
    return FA_READ;
  case VO_RDWR:                                     /* r+ */
    return FA_READ | FA_WRITE;
  case VO_CREAT | VO_APPEND | VO_WRONLY:            /* a */
    return FA_OPEN_APPEND | FA_WRITE;
  case VO_CREAT | VO_APPEND | VO_RDWR:              /* a+ */
    return FA_OPEN_APPEND | FA_WRITE | FA_READ;
  case VO_CREAT | VO_WRONLY:                        /* w */
    return FA_CREATE_ALWAYS | FA_WRITE;
  case VO_CREAT | VO_RDWR:                          /* w+ */
    return FA_CREATE_ALWAYS | FA_WRITE | FA_READ;
  case VO_CREAT | VO_EXCL | VO_WRONLY:              /* wx */
    return  FA_CREATE_NEW | FA_WRITE;
  case VO_CREAT | VO_EXCL | VO_RDWR:                /* w+x */
    return  FA_CREATE_NEW | FA_WRITE | FA_READ;
  default:
    break;
  }

  return (BYTE)0;
}

static vfs_mode_t translate_mode(BYTE fattrib) {
  vfs_mode_t mode = VFS_MODE_S_IRUSR;

  if ((fattrib & AM_RDO) == 0) {
    mode |= VFS_MODE_S_IWUSR;
  }

  if ((fattrib & AM_DIR) == 0) {
    mode |= VFS_MODE_S_IFREG;
  }
  else {
    mode |= VFS_MODE_S_IFDIR;
  }

  return mode;
}

static msg_t translate_error(FRESULT res) {
  msg_t msg;

  switch (res) {
  case FR_OK:
    msg = CH_RET_SUCCESS;
    break;
  case FR_TIMEOUT:
    msg = CH_RET_TIMEOUT;
    break;
  case FR_NOT_ENOUGH_CORE:
    msg = CH_RET_ENOMEM;
    break;
  case FR_TOO_MANY_OPEN_FILES:
    msg = CH_RET_ENFILE;
    break;
  case FR_DISK_ERR:
  case FR_NOT_READY:
  case FR_INVALID_DRIVE:
  case FR_NO_FILESYSTEM:
    msg = CH_RET_EIO;
    break;
  case FR_NO_FILE:
  case FR_NO_PATH:
    msg = CH_RET_ENOENT;
    break;
  case FR_INVALID_NAME:
    msg = CH_RET_ENAMETOOLONG;
    break;
  case FR_DENIED:
  case FR_WRITE_PROTECTED:
    msg = CH_RET_EACCES;
    break;
  case FR_EXIST:
    msg = CH_RET_EEXIST;
    break;
  case FR_IS_DIRECTORY:
    msg = CH_RET_EISDIR;
    break;
  case FR_NOT_DIRECTORY:
    msg = CH_RET_ENOTDIR;
    break;
  default:
    msg = CH_RET_INNER_ERROR;
    break;
  }

  return msg;
}

static void __ffdir_dispose_impl(void *ip);
static void __ffdir_bo_dispose_impl(void *ip) {

  /* Disposing self.*/
  __ffdir_dispose_impl(ip);

  /* Last because it corrupts the object.*/
  chPoolFree(&vfs_fatfs_driver_static.dir_nodes_pool, ip);
}

static void *__ffdir_ro_addref_impl(void *ip) {

  chSysLock();
  ip = __ro_addref_impl(ip);
  chSysUnlock();

  return ip;
}

static object_references_t __ffdir_ro_release_impl(void *ip) {
  object_references_t references;

  chSysLock();
  references = __ro_release_impl(ip);
  chSysUnlock();

  return references;
}

static msg_t __ffdir_vfsnode_stat_impl(void *ip, vfs_stat_t *sp) {

  return __vfsnode_stat_impl(ip, sp);
}

static msg_t __ffdir_vfsdir_next_impl(void *ip, vfs_direntry_info_t *dip) {
  msg_t ret;

  do {
    vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)ip;
    FRESULT res;
    FILINFO *fip;

    fip = (FILINFO *)chPoolAlloc(&vfs_fatfs_driver_static.info_nodes_pool);
    if (fip != NULL) {

      res = f_readdir(&ffdnp->ffdir.dir, fip);
      if (res == FR_OK) {
        if (fip->fname[0] == '\0') {
          ret = (msg_t)0;
        }
        else {
          dip->mode = translate_mode(fip->fattrib);
          dip->size = (vfs_offset_t)fip->fsize;
          strncpy(dip->name, fip->fname, VFS_CFG_NAMELEN_MAX);
          dip->name[VFS_CFG_NAMELEN_MAX] = '\0';
          ret = (msg_t)1;
        }
      }
      else {
        ret = translate_error(res);
      }

      chPoolFree(&vfs_fatfs_driver_static.info_nodes_pool, (void *)fip);
    }
    else {
      ret = CH_RET_ENOMEM;
      break;
    }
  }
  while (false);

  return ret;
}

static msg_t __ffdir_vfsdir_first_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)ip;
  msg_t ret;
  FRESULT res;

  res = f_rewinddir(&ffdnp->ffdir.dir);
  if (res == FR_OK) {
    ret = __ffdir_vfsdir_next_impl(ip, dip);
  }
  else {
    ret = translate_error(res);
  }

  return ret;
}

static void __fffile_dispose_impl(void *ip);
static void __fffile_bo_dispose_impl(void *ip) {

  /* Disposing self.*/
  __fffile_dispose_impl(ip);

  /* Last because it corrupts the object.*/
  chPoolFree(&vfs_fatfs_driver_static.file_nodes_pool, ip);
}

static void *__fffile_ro_addref_impl(void *ip) {

  chSysLock();
  ip = __ro_addref_impl(ip);
  chSysUnlock();

  return ip;
}

static object_references_t __fffile_ro_release_impl(void *ip) {
  object_references_t references;

  chSysLock();
  references = __ro_release_impl(ip);
  chSysUnlock();

  return references;
}

static msg_t __fffile_vfsnode_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)ip;

  sp->mode = fffnp->vfsnode.mode;
  sp->size = (vfs_offset_t)fffnp->fffile.file.obj.objsize;

  return CH_RET_SUCCESS;
}

static ssize_t __fffile_vfsfile_read_impl(void *ip, uint8_t *buf, size_t size) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)ip;
  FRESULT res;
  UINT br;

  res = f_read(&fffnp->fffile.file, (void *)buf, (UINT)size, &br);
  if (res != FR_OK) {

    return translate_error(res);
  }

  return (ssize_t)br;
}

static ssize_t __fffile_vfsfile_write_impl(void *ip, const uint8_t *buf,
                                           size_t size) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)ip;
  FRESULT res;
  UINT bw;

  res = f_write(&fffnp->fffile.file, (const void *)buf, (UINT)size, &bw);
  if (res != FR_OK) {

    return translate_error(res);
  }

  return (ssize_t)bw;
}

static msg_t __fffile_vfsfile_setpos_impl(void *ip, vfs_offset_t offset,
                                          vfs_seekmode_t whence) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)ip;
  vfs_offset_t finaloff;

  chDbgCheck((whence == SEEK_SET) ||
             (whence == SEEK_CUR) ||
             (whence == SEEK_END));

  switch (whence) {
  case VFS_SEEK_CUR:
    finaloff = offset + (vfs_offset_t)fffnp->fffile.file.fptr;
    break;
  case VFS_SEEK_END:
    finaloff = offset + (vfs_offset_t)fffnp->fffile.file.obj.objsize;
    break;
  case VFS_SEEK_SET:
  default:
    finaloff = offset;
    break;
  }

  if (finaloff < 0) {
    return CH_RET_EOVERFLOW;
  }

  return translate_error(f_lseek(&fffnp->fffile.file, (FSIZE_t)finaloff));
}

static vfs_offset_t __fffile_vfsfile_getpos_impl(void *ip) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)ip;

  return (vfs_offset_t)f_tell(&fffnp->fffile.file);
}

static sequential_stream_i *__fffile_vfsfile_getstream_impl(void *ip) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)ip;

  return &fffnp->fffile.stm;
}

static void __ffdrv_bo_dispose_impl(void *ip) {

  /* Disposing self.*/
  __ffdrv_dispose_impl(ip);
}

static msg_t __ffdrv_vfsdrv_setcwd_impl(void *ip, const char *path) {
#if FF_FS_RPATH >= 1

  (void)ip;

  return translate_error(f_chdir((const TCHAR *)path));
#else

  (void)ip;

  if (strcmp(path, "/") != 0) {
    return CH_RET_ENOENT;
  }

  return CH_RET_SUCCESS;
#endif
}

static msg_t __ffdrv_vfsdrv_getcwd_impl(void *ip, char *buf, size_t size) {
#if FF_FS_RPATH >= 2

  (void)ip;

  return translate_error(f_getcwd((TCHAR *)buf, (UINT)size));
#else

  (void)ip;

  if (size < 2) {
    return CH_RET_ERANGE;
  }

  buf[0] = '/';
  buf[1] = '\0';

  return CH_RET_SUCCESS;
#endif
}

static msg_t __ffdrv_vfsdrv_stat_impl(void *ip, const char *path,
                                      vfs_stat_t *sp) {
  msg_t ret;

  (void)ip;

  do {
    FRESULT res;
    FILINFO *fip;

    fip = (FILINFO *)chPoolAlloc(&vfs_fatfs_driver_static.info_nodes_pool);
    if (fip != NULL) {

      res = f_stat((const TCHAR *)path, fip);
      if (res == FR_OK) {

        sp->mode = translate_mode(fip->fattrib);
        sp->size = (vfs_offset_t)fip->fsize;

        ret = CH_RET_SUCCESS;
      }
      else {
        ret = translate_error(res);
      }

      chPoolFree(&vfs_fatfs_driver_static.info_nodes_pool, (void *)fip);
    }
    else {
      ret = CH_RET_ENOMEM;
      break;
    }
  }
  while (false);

  return ret;
}

static msg_t __ffdrv_vfsdrv_opendir_impl(void *ip, const char *path,
                                         vfs_directory_node_c **vdnpp) {
  msg_t ret;

  do {
    vfs_fatfs_driver_c *drvp = (vfs_fatfs_driver_c *)ip;
    vfs_fatfs_dir_node_c *ffdnp;
    FRESULT res;

    ffdnp = chPoolAlloc(&vfs_fatfs_driver_static.dir_nodes_pool);
    if (ffdnp != NULL) {

      res = f_opendir(&ffdnp->ffdir.dir, (const TCHAR *)path);
      if (res == FR_OK) {

        /* Node object initialization.*/
        (void) ffdir_object_init(ffdnp,
                                 (vfs_driver_c *)drvp,
                                 translate_mode(ffdnp->ffdir.dir.obj.attr));
/*        __ro_objinit_impl(ffdnp, &dir_node_vmt);
        ffdnp->vfsnode.driver   = (vfs_driver_c *)drvp;
        ffdnp->vfsnode.mode     = translate_mode(ffdnp->dir.obj.attr);*/

        *vdnpp = (vfs_directory_node_c *)ffdnp;
        ret = CH_RET_SUCCESS;
        break;
      }

      chPoolFree(&vfs_fatfs_driver_static.dir_nodes_pool, (void *)ffdnp);
    }
    else {
      ret = CH_RET_ENOMEM;
      break;
    }

    ret = translate_error(res);
  }
  while (false);

  return ret;
}

static msg_t __ffdrv_vfsdrv_openfile_impl(void *ip, const char *path,
                                          int flags, vfs_file_node_c **vfnpp) {
  msg_t ret;

  do {
    vfs_fatfs_driver_c *drvp = (vfs_fatfs_driver_c *)ip;
    vfs_fatfs_file_node_c *fffnp;
    FRESULT res;
    BYTE mode;

    mode = translate_oflag(flags);
    if (mode == (BYTE)0) {
      ret = CH_RET_EINVAL;
      break;
    }

    fffnp = chPoolAlloc(&vfs_fatfs_driver_static.file_nodes_pool);
    if (fffnp != NULL) {

      res = f_open(&fffnp->fffile.file, (const TCHAR *)path, mode);
      if (res == FR_OK) {

        /* Node object initialization.*/
        (void) fffile_object_init(fffnp,
                                  (vfs_driver_c *)drvp,
                                  translate_mode(fffnp->fffile.file.obj.attr));
/*        __ro_objinit_impl(fffnp, &file_node_vmt);
        fffnp->vfsnode.driver     = (vfs_driver_c *)drvp;
        fffnp->vfsnode.mode       = translate_mode(fffnp->file.obj.attr);
        fffnp->stream.vmt = &file_stream_vmt;*/

        *vfnpp = (vfs_file_node_c *)fffnp;
        ret = CH_RET_SUCCESS;
        break;
      }

      chPoolFree(&vfs_fatfs_driver_static.file_nodes_pool, (void *)fffnp);
    }
    else {
      ret = CH_RET_ENOMEM;
      break;
    }

    ret = translate_error(res);
  }
  while (false);

  return ret;
}

static msg_t __ffdrv_vfsdrv_unlink_impl(void *ip, const char *path) {

  (void)ip;

  return translate_error(f_unlink((const TCHAR *)path));
}

static msg_t __ffdrv_vfsdrv_rename_impl(void *ip, const char *oldpath,
                                        const char *newpath) {

  (void)ip;

  return translate_error(f_rename((const TCHAR *)oldpath,
                                  (const TCHAR *)newpath));
}

static msg_t __ffdrv_vfsdrv_mkdir_impl(void *ip, const char *path,
                                       vfs_mode_t mode) {

  (void)ip;
  (void)mode; /* Not handled by FatFS.*/

  return translate_error(f_mkdir((const TCHAR *)path));
}

static msg_t __ffdrv_vfsdrv_rmdir_impl(void *ip, const char *path) {

  (void)ip;

  return translate_error(f_rmdir((const TCHAR *)path));
}

static size_t __fffile_stm_write_impl(void *ip, const uint8_t *buf,
                                      size_t size) {
  vfs_fatfs_file_node_c *self = oopIfGetOwner(vfs_fatfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileWrite((void *)self, buf, size);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
}

static size_t __fffile_stm_read_impl(void *ip, uint8_t *buf, size_t size) {
  vfs_fatfs_file_node_c *self = oopIfGetOwner(vfs_fatfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileRead((void *)self, buf, size);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
}

static msg_t __fffile_stm_put_impl(void *ip, uint8_t b) {
  vfs_fatfs_file_node_c *self = oopIfGetOwner(vfs_fatfs_file_node_c, ip);
  msg_t msg;

  msg = vfsFileWrite((void *)self, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return msg;
}

static msg_t __fffile_stm_get_impl(void *ip) {
  vfs_fatfs_file_node_c *self = oopIfGetOwner(vfs_fatfs_file_node_c, ip);
  msg_t msg;
  uint8_t b;

  msg = vfsFileRead((void *)self, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return (msg_t)b;
}

/**
 * @name        Methods implementations of vfs_fatfs_dir_node_c
 * @{
 */
/**
 * @memberof    vfs_fatfs_dir_node_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_fatfs_dir_node_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
static void *__ffdir_objinit_impl(void *ip, const void *vmt,
                                  vfs_driver_c *driver, vfs_mode_t mode) {
  vfs_fatfs_dir_node_c *self = (vfs_fatfs_dir_node_c *)ip;

  /* Initialization code.*/
  self = __vfsdir_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);

  return self;
}

/**
 * @memberof    vfs_fatfs_dir_node_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_fatfs_dir_node_c instance
 *                              to be disposed.
 */
static void __ffdir_dispose_impl(void *ip) {
  vfs_fatfs_dir_node_c *self = (vfs_fatfs_dir_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsdir_dispose_impl(self);
}
/** @} */

/**
 * @name        Default constructor of vfs_fatfs_dir_node_c
 * @{
 */
/**
 * @memberof    vfs_fatfs_dir_node_c
 *
 * @brief       Default initialization function of @p vfs_fatfs_dir_node_c.
 *
 * @param[out]    self          Pointer to a @p vfs_fatfs_dir_node_c instance
 *                              to be initialized.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline vfs_fatfs_dir_node_c *ffdir_object_init(vfs_fatfs_dir_node_c *self,
                                                      vfs_driver_c *driver,
                                                      vfs_mode_t mode) {
  static const struct vfs_fatfs_dir_node_vmt __ffdir_vmt = {
    __ffdir_vmt_init(ffdir)
  };

  return __ffdir_objinit_impl(self, &__ffdir_vmt, driver, mode);
}
/** @} */

/**
 * @name        Methods implementations of vfs_fatfs_file_node_c
 * @{
 */
/**
 * @memberof    vfs_fatfs_file_node_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_fatfs_file_node_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      A new reference to the object.
 */
static void *__fffile_objinit_impl(void *ip, const void *vmt,
                                   vfs_driver_c *driver, vfs_mode_t mode) {
  vfs_fatfs_file_node_c *self = (vfs_fatfs_file_node_c *)ip;

  /* Implementation of interface sequential_stream_i.*/
  {
    static const struct sequential_stream_vmt fffile_stm_vmt = {
      __stm_vmt_init(fffile, offsetof(vfs_fatfs_file_node_c, fffile.stm))
    };
    oopIfObjectInit(&self->fffile.stm, &fffile_stm_vmt);
  }

  /* Initialization code.*/
  self = __vfsfile_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);

  return self;
}

/**
 * @memberof    vfs_fatfs_file_node_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_fatfs_file_node_c instance
 *                              to be disposed.
 */
static void __fffile_dispose_impl(void *ip) {
  vfs_fatfs_file_node_c *self = (vfs_fatfs_file_node_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsfile_dispose_impl(self);
}
/** @} */

/**
 * @name        Default constructor of vfs_fatfs_file_node_c
 * @{
 */
/**
 * @memberof    vfs_fatfs_file_node_c
 *
 * @brief       Default initialization function of @p vfs_fatfs_file_node_c.
 *
 * @param[out]    self          Pointer to a @p vfs_fatfs_file_node_c instance
 *                              to be initialized.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline vfs_fatfs_file_node_c *fffile_object_init(vfs_fatfs_file_node_c *self,
                                                        vfs_driver_c *driver,
                                                        vfs_mode_t mode) {
  static const struct vfs_fatfs_file_node_vmt __fffile_vmt = {
    __fffile_vmt_init(fffile)
  };

  return __fffile_objinit_impl(self, &__fffile_vmt, driver, mode);
}
/** @} */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       Module initialization.
 *
 * @init
 */
void __drv_fatfs_init(void) {

    /* Initializing pools.*/
    chPoolObjectInit(&vfs_fatfs_driver_static.dir_nodes_pool,
                     sizeof (vfs_fatfs_dir_node_c),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_fatfs_driver_static.file_nodes_pool,
                     sizeof (vfs_fatfs_file_node_c),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_fatfs_driver_static.info_nodes_pool,
                     sizeof (FILINFO),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_fatfs_driver_static.fs_nodes_pool,
                     sizeof (FATFS),
                     NULL);

    /* Preloading pools.*/
    chPoolLoadArray(&vfs_fatfs_driver_static.dir_nodes_pool,
                    &vfs_fatfs_driver_static.dir_nodes[0],
                    DRV_CFG_FATFS_DIR_NODES_NUM);
    chPoolLoadArray(&vfs_fatfs_driver_static.file_nodes_pool,
                    &vfs_fatfs_driver_static.file_nodes[0],
                    DRV_CFG_FATFS_FILE_NODES_NUM);
    chPoolLoadArray(&vfs_fatfs_driver_static.fs_nodes_pool,
                    &__nocache_vfs_fatfs_driver_static.fs[0],
                    DRV_CFG_FATFS_FS_NUM);
}

/**
 * @brief       Mounts a FatFS volume.
 *
 * @param[in]     name          Name to be assigned to the volume, see FatFS @p
 *                              f_mount() documentation because there are
 *                              several options.
 * @param[in]     mountnow      Immediate mount option.
 * @return                      The operation result.
 *
 * @api
 */
msg_t ffdrvMount(const char *name, bool mountnow) {
  FATFS *fs;
  FRESULT res;

  fs = f_getfs(name);
  if (fs == NULL) {
    fs = chPoolAlloc(&vfs_fatfs_driver_static.fs_nodes_pool);
    if (fs == NULL) {
      return CH_RET_ENOMEM;
    }
  }

  res = f_mount(fs, name, (BYTE)(mountnow ? 1 : 0));
  if (res != FR_OK) {
    chPoolFree(&vfs_fatfs_driver_static.fs_nodes_pool, (void *)fs);
  }

  return translate_error(res);
}

/**
 * @brief       Unmounts a FatFS volume.
 *
 * @param[in]     name          Name of the volume to be unmounted.
 * @return                      The operation result.
 *
 * @api
 */
msg_t ffdrvUnmount(const char *name) {
  FATFS *fs;
  FRESULT res;

  fs = f_getfs(name);
  if (fs == NULL) {
    return CH_RET_EINVAL;
  }

  res = f_unmount(name);

  chPoolFree(&vfs_fatfs_driver_static.fs_nodes_pool, (void *)fs);

  return translate_error(res);
}

/*===========================================================================*/
/* Module class "vfs_fatfs_driver_c" methods.                                */
/*===========================================================================*/

/**
 * @brief       VMT structure of VFS fatfs driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct vfs_fatfs_driver_vmt __ffdrv_vmt = {
  __ffdrv_vmt_init(ffdrv)
};

/**
 * @name        Methods implementations of vfs_fatfs_driver_c
 * @{
 */
/**
 * @memberof    vfs_fatfs_driver_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_fatfs_driver_c instance to
 *                              be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__ffdrv_objinit_impl(void *ip, const void *vmt) {
  vfs_fatfs_driver_c *self = (vfs_fatfs_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __vfsdrv_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @memberof    vfs_fatfs_driver_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_fatfs_driver_c instance to
 *                              be disposed.
 */
void __ffdrv_dispose_impl(void *ip) {
  vfs_fatfs_driver_c *self = (vfs_fatfs_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __vfsdrv_dispose_impl(self);
}
/** @} */

