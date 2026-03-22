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
 * @file        drvromfs_impl.inc
 * @brief       Template of VFS ROMFS Driver source.
 * @note        This is a template file, can be edited directly.
 *
 * @addtogroup  DRVROMFS
 * @{
 */

/* This is an, automatically generated, implementation file that can be
   manually edited, it is not re-generated if already present.*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static const char *get_dir_path(const vfs_rom_dir_node_c *dp) {

  if (dp->dir == NULL) {
    return "/";
  }

  return dp->dir->path;
}

static msg_t build_absolute_path(char *buf, const char *path) {
  msg_t ret;

  do {
    *buf = '\0';

    if (!vfs_path_is_separator(*path)) {
      if (vfs_path_append(buf, "/", VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
        ret = CH_RET_ENAMETOOLONG;
        break;
      }
    }

    if (vfs_path_append(buf, path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    if (vfs_path_normalize(buf, buf, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    ret = CH_RET_SUCCESS;
  }
  while (false);

  return ret;
}

static const vfs_romfs_dir_desc_t *find_dir(const vfs_rom_driver_c *drvp,
                                            const char *path) {
  size_t i;

  i = 0U;
  while (i < drvp->tree->dirs_num) {
    if (strcmp(drvp->tree->dirs[i].path, path) == 0) {
      return &drvp->tree->dirs[i];
    }

    i++;
  }

  return NULL;
}

static bool get_child_dir_name(const char *parent,
                               const char *path,
                               char *name) {
  const char *scanp;
  size_t n;

  if (strcmp(parent, "/") == 0) {
    if (!vfs_path_is_separator(*path) || (path[1] == '\0')) {
      return false;
    }

    scanp = path + 1;
  }
  else {
    size_t parent_len;

    parent_len = strlen(parent);
    if ((strncmp(parent, path, parent_len) != 0) ||
        !vfs_path_is_separator(path[parent_len]) ||
        (path[parent_len + 1] == '\0')) {
      return false;
    }

    scanp = path + parent_len + 1U;
  }

  n = vfs_path_get_element(&scanp, name, VFS_CFG_NAMELEN_MAX + 1);
  if ((n == (size_t)0) || (n >= VFS_CFG_NAMELEN_MAX + 1U)) {
    return false;
  }

  return true;
}

static bool get_next_child_dir(const vfs_rom_dir_node_c *self,
                               size_t from,
                               size_t *nextp,
                               char *name) {
  const vfs_rom_driver_c *drvp = (const vfs_rom_driver_c *)self->driver;
  const char *parent = get_dir_path(self);
  size_t i;

  i = from;
  while (i < drvp->tree->dirs_num) {
    char child_name[VFS_CFG_NAMELEN_MAX + 1];

    if (get_child_dir_name(parent, drvp->tree->dirs[i].path, child_name)) {
      size_t j;

      strcpy(name, child_name);
      j = i + 1U;
      while (j < drvp->tree->dirs_num) {
        char next_name[VFS_CFG_NAMELEN_MAX + 1];

        if (!get_child_dir_name(parent, drvp->tree->dirs[j].path, next_name) ||
            (strcmp(child_name, next_name) != 0)) {
          break;
        }

        j++;
      }

      *nextp = j;
      return true;
    }

    i++;
  }

  return false;
}

static msg_t get_file_stat(const vfs_romfs_file_desc_t *filep,
                           vfs_stat_t *sp) {
  msg_t ret;

  sp->mode = VFS_MODE_S_IFREG | filep->mode;
  sp->size = filep->size;

  if ((filep->ops != NULL) && (filep->ops->stat != NULL)) {
    ret = filep->ops->stat(filep->arg, &sp->size);
    if (CH_RET_IS_ERROR(ret)) {
      return ret;
    }
  }

  return CH_RET_SUCCESS;
}

static const vfs_romfs_file_desc_t *find_file(vfs_rom_driver_c *drvp,
                                              char *path) {
  const vfs_romfs_dir_desc_t *dirp;
  char *sep;
  const char *name;
  size_t i;

  if ((path[0] == '/') && (path[1] == '\0')) {
    return NULL;
  }

  sep = strrchr(path, '/');
  chDbgAssert(sep != NULL, "invalid path");
  name = sep + 1;
  if (*name == '\0') {
    return NULL;
  }

  if (sep == path) {
    dirp = find_dir(drvp, "/");
  }
  else {
    *sep = '\0';
    dirp = find_dir(drvp, path);
    *sep = '/';
  }

  if (dirp == NULL) {
    return NULL;
  }

  i = 0U;
  while (i < dirp->files_num) {
    if (strcmp(dirp->files[i].name, name) == 0) {
      return &dirp->files[i];
    }

    i++;
  }

  return NULL;
}

static bool is_read_only_open(int flags) {

  if ((flags & VO_ACCMODE) != VO_RDONLY) {
    return false;
  }

  if ((flags & (VO_APPEND | VO_CREAT | VO_TRUNC | VO_EXCL)) != 0) {
    return false;
  }

  return true;
}

static msg_t __romdir_next_impl(void *ip, vfs_direntry_info_t *dip);

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       Module initialization.
 *
 * @init
 */
void __drv_rom_init(void) {

    /* Initializing pools.*/
    chPoolObjectInit(&vfs_rom_driver_static.dir_nodes_pool,
                     sizeof (vfs_rom_dir_node_c),
                     chCoreAllocAlignedI);
    chPoolObjectInit(&vfs_rom_driver_static.file_nodes_pool,
                     sizeof (vfs_rom_file_node_c),
                     chCoreAllocAlignedI);

    /* Preloading pools.*/
    chPoolLoadArray(&vfs_rom_driver_static.dir_nodes_pool,
                    &vfs_rom_driver_static.dir_nodes[0],
                    DRV_CFG_ROM_DIR_NODES_NUM);
    chPoolLoadArray(&vfs_rom_driver_static.file_nodes_pool,
                    &vfs_rom_driver_static.file_nodes[0],
                    DRV_CFG_ROM_FILE_NODES_NUM);
}

/*===========================================================================*/
/* Module class "vfs_rom_dir_node_c" methods.                                */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_rom_dir_node_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_rom_dir_node_c instance to
 *                              be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @param[in]     dir           Pointer to the exact directory descriptor.
 * @return                      A new reference to the object.
 */
static void *__romdir_objinit_impl(void *ip, const void *vmt,
                                   vfs_driver_c *driver, vfs_mode_t mode,
                                   const vfs_romfs_dir_desc_t *dir) {
  vfs_rom_dir_node_c *self = (vfs_rom_dir_node_c *)ip;

  self = __vfsdir_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);
  self->dir = dir;
  self->next_dir = 0U;
  self->next_file = 0U;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_dir_node_c instance to
 *                              be disposed.
 */
static void __romdir_dispose_impl(void *ip) {

  __vfsdir_dispose_impl(ip);
  chPoolFree(&vfs_rom_driver_static.dir_nodes_pool, ip);
}

/**
 * @brief       Override of method @p vfsNodeStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_dir_node_c instance.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
static msg_t __romdir_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_rom_dir_node_c *self = (vfs_rom_dir_node_c *)ip;

  (void)self;

  sp->mode = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR | VFS_MODE_S_IXUSR;
  sp->size = (vfs_offset_t)0;

  return CH_RET_SUCCESS;
}

/**
 * @brief       Override of method @p vfsDirReadFirst().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_dir_node_c instance.
 * @param[out]    dip           Pointer to a @p vfs_direntry_info_t structure.
 * @return                      The operation result.
 */
static msg_t __romdir_first_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_rom_dir_node_c *self = (vfs_rom_dir_node_c *)ip;

  self->next_dir = 0U;
  self->next_file = 0U;

  return __romdir_next_impl(ip, dip);
}

/**
 * @brief       Override of method @p vfsDirReadNext().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_dir_node_c instance.
 * @param[out]    dip           Pointer to a @p vfs_direntry_info_t structure.
 * @return                      The operation result.
 */
static msg_t __romdir_next_impl(void *ip, vfs_direntry_info_t *dip) {
  vfs_rom_dir_node_c *self = (vfs_rom_dir_node_c *)ip;
  const vfs_romfs_file_desc_t *filep;
  char child_name[VFS_CFG_NAMELEN_MAX + 1];
  size_t next_dir;
  bool have_dir;

  have_dir = get_next_child_dir(self, self->next_dir, &next_dir, child_name);
  if ((self->dir != NULL) && (self->next_file < self->dir->files_num)) {
    filep = &self->dir->files[self->next_file];
  }
  else {
    filep = NULL;
  }

  if (!have_dir && (filep == NULL)) {
    return (msg_t)0;
  }

  if (!have_dir || ((filep != NULL) && (strcmp(filep->name, child_name) < 0))) {
    msg_t ret;
    vfs_stat_t st;

    ret = get_file_stat(filep, &st);
    if (CH_RET_IS_ERROR(ret)) {
      return ret;
    }
    dip->mode = st.mode;
    dip->size = st.size;
    strcpy(dip->name, filep->name);
    self->next_file++;

    return (msg_t)1;
  }

  dip->mode = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR | VFS_MODE_S_IXUSR;
  dip->size = (vfs_offset_t)0;
  strcpy(dip->name, child_name);
  self->next_dir = next_dir;

  return (msg_t)1;
}
/** @} */

/**
 * @brief       VMT structure of VFS ROMFS directory node class.
 * @note        It is public because accessed by the inlined constructor.
 */
static const struct vfs_rom_dir_node_vmt __vfs_rom_dir_node_vmt = {
  .dispose                  = __romdir_dispose_impl,
  .addref                   = __ro_addref_impl,
  .release                  = __ro_release_impl,
  .stat                     = __romdir_stat_impl,
  .first                    = __romdir_first_impl,
  .next                     = __romdir_next_impl
};

/**
 * @name        Default constructor of vfs_rom_dir_node_c
 * @{
 */
/**
 * @brief       Default initialization function of @p vfs_rom_dir_node_c.
 *
 * @param[out]    self          Pointer to a @p vfs_rom_dir_node_c instance to
 *                              be initialized.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @param[in]     dir           Pointer to the exact directory descriptor.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
static vfs_rom_dir_node_c *romdirObjectInit(vfs_rom_dir_node_c *self,
                                            vfs_driver_c *driver,
                                            vfs_mode_t mode,
                                            const vfs_romfs_dir_desc_t *dir) {

  return __romdir_objinit_impl(self, &__vfs_rom_dir_node_vmt,
                               driver, mode, dir);
}
/** @} */

/*===========================================================================*/
/* Module class "vfs_rom_file_node_c" methods.                               */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_rom_file_node_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_rom_file_node_c instance to
 *                              be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @param[in]     file          Pointer to the file descriptor.
 * @param[in]     size          File size for the current open session.
 * @param[in]     session       Dynamic file session pointer.
 * @return                      A new reference to the object.
 */
static void *__romfile_objinit_impl(void *ip, const void *vmt,
                                    vfs_driver_c *driver, vfs_mode_t mode,
                                    const vfs_romfs_file_desc_t *file,
                                    vfs_offset_t size, void *session) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;

  self = __vfsfile_objinit_impl(ip, vmt, (vfs_driver_c *)driver, mode);
  self->file = file;
  self->position = (vfs_offset_t)0;
  self->size = size;
  self->session = session;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance to
 *                              be disposed.
 */
static void __romfile_dispose_impl(void *ip) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;

  if ((self->file != NULL) &&
      (self->file->ops != NULL) &&
      (self->file->ops->close != NULL)) {
    self->file->ops->close(self->session);
  }

  __vfsfile_dispose_impl(self);
  chPoolFree(&vfs_rom_driver_static.file_nodes_pool, ip);
}

/**
 * @brief       Override of method @p vfsNodeStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
static msg_t __romfile_stat_impl(void *ip, vfs_stat_t *sp) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;

  sp->mode = self->mode;
  sp->size = self->size;

  return CH_RET_SUCCESS;
}

/**
 * @brief       Override of method @p vfsFileRead().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance.
 * @param[out]    buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
static ssize_t __romfile_read_impl(void *ip, uint8_t *buf, size_t n) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;
  size_t available;

  if (self->position >= self->size) {
    return (ssize_t)0;
  }

  available = (size_t)(self->size - self->position);
  if (n > available) {
    n = available;
  }

  if (self->file->ops == NULL) {
    memcpy(buf, &self->file->data[self->position], n);
    self->position += (vfs_offset_t)n;

    return (ssize_t)n;
  }

  if (self->file->ops->read == NULL) {
    return CH_RET_ENOSYS;
  }
  else {
    ssize_t ret;

    ret = self->file->ops->read(self->session, self->position, buf, n);
    if (CH_RET_IS_ERROR(ret)) {
      return ret;
    }
    if ((size_t)ret > n) {
      ret = (ssize_t)n;
    }
    self->position += (vfs_offset_t)ret;

    return ret;
  }
}

/**
 * @brief       Override of method @p vfsFileWrite().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance.
 * @param[in]     buf           Pointer to the data buffer.
 * @param[in]     n             Maximum amount of data to be transferred.
 * @return                      The transferred number of bytes or an error.
 */
static ssize_t __romfile_write_impl(void *ip, const uint8_t *buf, size_t n) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;

  (void)self;
  (void)buf;
  (void)n;

  return CH_RET_EROFS;
}

/**
 * @brief       Override of method @p vfsFileSetPosition().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance.
 * @param[in]     offset        Offset to be applied.
 * @param[in]     whence        Seek mode to be used.
 * @return                      The operation result.
 */
static msg_t __romfile_setpos_impl(void *ip, vfs_offset_t offset,
                                   vfs_seekmode_t whence) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;
  vfs_offset_t newpos;

  switch (whence) {
  case VFS_SEEK_SET:
    newpos = offset;
    break;
  case VFS_SEEK_CUR:
    newpos = self->position + offset;
    break;
  case VFS_SEEK_END:
    newpos = self->size + offset;
    break;
  default:
    return CH_RET_EINVAL;
  }

  if ((newpos < (vfs_offset_t)0) || (newpos > self->size)) {
    return CH_RET_EINVAL;
  }

  self->position = newpos;

  return CH_RET_SUCCESS;
}

/**
 * @brief       Override of method @p vfsFileGetPosition().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance.
 * @return                      The current file position.
 */
static vfs_offset_t __romfile_getpos_impl(void *ip) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;

  return self->position;
}

/**
 * @brief       Override of method @p vfsFileGetStream().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_file_node_c instance.
 * @return                      Pointer to the random stream interface.
 */
static random_stream_i *__romfile_getstream_impl(void *ip) {
  vfs_rom_file_node_c *self = (vfs_rom_file_node_c *)ip;

  return &self->rstm;
}
/** @} */

/**
 * @brief       VMT structure of VFS ROMFS file node class.
 * @note        It is public because accessed by the inlined constructor.
 */
static const struct vfs_rom_file_node_vmt __vfs_rom_file_node_vmt = {
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

/**
 * @name        Default constructor of vfs_rom_file_node_c
 * @{
 */
/**
 * @brief       Default initialization function of @p vfs_rom_file_node_c.
 *
 * @param[out]    self          Pointer to a @p vfs_rom_file_node_c instance to
 *                              be initialized.
 * @param[in]     driver        Pointer to the controlling driver.
 * @param[in]     mode          Node mode flags.
 * @param[in]     file          Pointer to the file descriptor.
 * @param[in]     size          File size for the current open session.
 * @param[in]     session       Dynamic file session pointer.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
static vfs_rom_file_node_c *romfileObjectInit(vfs_rom_file_node_c *self,
                                              vfs_driver_c *driver,
                                              vfs_mode_t mode,
                                              const vfs_romfs_file_desc_t *file,
                                              vfs_offset_t size,
                                              void *session) {

  return __romfile_objinit_impl(self, &__vfs_rom_file_node_vmt,
                                driver, mode, file, size, session);
}
/** @} */

/*===========================================================================*/
/* Module class "vfs_rom_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of vfs_rom_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vfs_rom_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     tree          Pointer to the ROMFS image descriptor.
 * @return                      A new reference to the object.
 */
void *__romdrv_objinit_impl(void *ip, const void *vmt,
                            const vfs_romfs_tree_t *tree) {
  vfs_rom_driver_c *self = (vfs_rom_driver_c *)ip;

  __vfsdrv_objinit_impl(self, vmt);
  self->tree = tree;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance to be
 *                              disposed.
 */
void __romdrv_dispose_impl(void *ip) {

  __vfsdrv_dispose_impl(ip);
}

/**
 * @brief       Override of method @p vfsDrvChangeCurrentDirectory().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Path of the new current directory.
 * @return                      The operation result.
 */
msg_t __romdrv_setcwd_impl(void *ip, const char *path) {
  vfs_shared_buffer_t *shbuf;
  msg_t ret;

  (void)ip;

  shbuf = vfs_buffer_take_wait();
  ret = build_absolute_path(shbuf->buf, path);
  if (!CH_RET_IS_ERROR(ret)) {
    if (strcmp(shbuf->buf, "/") == 0) {
      ret = CH_RET_SUCCESS;
    }
    else {
      ret = CH_RET_ENOSYS;
    }
  }
  vfs_buffer_release(shbuf);

  return ret;
}

/**
 * @brief       Override of method @p vfsDrvGetCurrentDirectory().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[out]    buf           Buffer for the path string.
 * @param[in]     size          Size of the buffer.
 * @return                      The operation result.
 */
msg_t __romdrv_getcwd_impl(void *ip, char *buf, size_t size) {

  (void)ip;

  if (size == (size_t)0) {
    return CH_RET_ERANGE;
  }
  if (size < 2U) {
    buf[0] = '\0';
    return CH_RET_ERANGE;
  }

  buf[0] = '/';
  buf[1] = '\0';

  return CH_RET_SUCCESS;
}

/**
 * @brief       Override of method @p vfsDrvStat().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Absolute path of the node to be examined.
 * @param[out]    sp            Pointer to a @p vfs_stat_t structure.
 * @return                      The operation result.
 */
msg_t __romdrv_stat_impl(void *ip, const char *path, vfs_stat_t *sp) {
  vfs_rom_driver_c *self = (vfs_rom_driver_c *)ip;
  vfs_shared_buffer_t *shbuf;
  msg_t ret;

  shbuf = vfs_buffer_take_wait();

  ret = build_absolute_path(shbuf->buf, path);
  if (!CH_RET_IS_ERROR(ret)) {
    if (strcmp(shbuf->buf, "/") == 0) {
      sp->mode = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR | VFS_MODE_S_IXUSR;
      sp->size = (vfs_offset_t)0;
      ret = CH_RET_SUCCESS;
    }
    else {
      const vfs_romfs_dir_desc_t *dirp;
      const vfs_romfs_file_desc_t *filep;

      dirp = find_dir(self, shbuf->buf);
      if (dirp != NULL) {
        sp->mode = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR | VFS_MODE_S_IXUSR;
        sp->size = (vfs_offset_t)0;
        ret = CH_RET_SUCCESS;
      }
      else {
        filep = find_file(self, shbuf->buf);
        if (filep != NULL) {
          ret = get_file_stat(filep, sp);
        }
        else {
          ret = CH_RET_ENOENT;
        }
      }
    }
  }

  vfs_buffer_release(shbuf);

  return ret;
}

/**
 * @brief       Override of method @p vfsDrvOpenDirectory().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Absolute path of the directory to be opened.
 * @param[out]    vdnpp         Pointer to the pointer to the instantiated @p
 *                              vfs_directory_node_c object.
 * @return                      The operation result.
 */
msg_t __romdrv_opendir_impl(void *ip, const char *path,
                            vfs_directory_node_c **vdnpp) {
  vfs_rom_driver_c *self = (vfs_rom_driver_c *)ip;
  vfs_shared_buffer_t *shbuf;
  const vfs_romfs_dir_desc_t *dirp;
  msg_t ret;

  shbuf = vfs_buffer_take_wait();

  ret = build_absolute_path(shbuf->buf, path);
  if (!CH_RET_IS_ERROR(ret)) {
    if (strcmp(shbuf->buf, "/") == 0) {
      dirp = find_dir(self, "/");
    }
    else {
      dirp = find_dir(self, shbuf->buf);
      if (dirp == NULL) {
        if (find_file(self, shbuf->buf) != NULL) {
          ret = CH_RET_ENOTDIR;
        }
        else {
          ret = CH_RET_ENOENT;
        }
      }
    }

    if (!CH_RET_IS_ERROR(ret)) {
      vfs_rom_dir_node_c *rdnp;

      rdnp = chPoolAlloc(&vfs_rom_driver_static.dir_nodes_pool);
      if (rdnp != NULL) {
        (void)romdirObjectInit(rdnp, (vfs_driver_c *)self,
                               VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR |
                               VFS_MODE_S_IXUSR, dirp);
        *vdnpp = (vfs_directory_node_c *)rdnp;
        ret = CH_RET_SUCCESS;
      }
      else {
        ret = CH_RET_ENOMEM;
      }
    }
  }

  vfs_buffer_release(shbuf);

  return ret;
}

/**
 * @brief       Override of method @p vfsDrvOpenFile().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Absolute path of the directory to be opened.
 * @param[in]     flags         File open flags.
 * @param[out]    vfnpp         Pointer to the pointer to the instantiated @p
 *                              vfs_file_node_c object.
 * @return                      The operation result.
 */
msg_t __romdrv_openfile_impl(void *ip, const char *path, int flags,
                             vfs_file_node_c **vfnpp) {
  vfs_rom_driver_c *self = (vfs_rom_driver_c *)ip;
  vfs_shared_buffer_t *shbuf;
  msg_t ret;

  if (!is_read_only_open(flags)) {
    return CH_RET_EROFS;
  }

  shbuf = vfs_buffer_take_wait();

  ret = build_absolute_path(shbuf->buf, path);
  if (!CH_RET_IS_ERROR(ret)) {
    const vfs_romfs_file_desc_t *filep;

    filep = find_file(self, shbuf->buf);
    if (filep != NULL) {
      vfs_rom_file_node_c *rfnp;
      vfs_offset_t size;
      void *session;

      size = filep->size;
      session = NULL;

      if (filep->ops != NULL) {
        if (filep->ops->open == NULL) {
          ret = CH_RET_ENOSYS;
          goto romdrv_openfile_done;
        }

        ret = filep->ops->open(filep->arg, flags, &session, &size);
        if (CH_RET_IS_ERROR(ret)) {
          goto romdrv_openfile_done;
        }
      }

      rfnp = chPoolAlloc(&vfs_rom_driver_static.file_nodes_pool);
      if (rfnp != NULL) {
        (void)romfileObjectInit(rfnp, (vfs_driver_c *)self,
                                VFS_MODE_S_IFREG | filep->mode,
                                filep, size, session);
        *vfnpp = (vfs_file_node_c *)rfnp;
        ret = CH_RET_SUCCESS;
      }
      else {
        if ((filep->ops != NULL) && (filep->ops->close != NULL)) {
          filep->ops->close(session);
        }
        ret = CH_RET_ENOMEM;
      }
    }
    else {
      if ((strcmp(shbuf->buf, "/") == 0) || (find_dir(self, shbuf->buf) != NULL)) {
        ret = CH_RET_EISDIR;
      }
      else {
        ret = CH_RET_ENOENT;
      }
    }
  }

romdrv_openfile_done:
  vfs_buffer_release(shbuf);

  return ret;
}

/**
 * @brief       Override of method @p vfsDrvUnlink().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Path of the file to be unlinked.
 * @return                      The operation result.
 */
msg_t __romdrv_unlink_impl(void *ip, const char *path) {

  (void)ip;
  (void)path;

  return CH_RET_EROFS;
}

/**
 * @brief       Override of method @p vfsDrvRename().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     oldpath       Path of the node to be renamed.
 * @param[in]     newpath       New path of the renamed node.
 * @return                      The operation result.
 */
msg_t __romdrv_rename_impl(void *ip, const char *oldpath, const char *newpath) {

  (void)ip;
  (void)oldpath;
  (void)newpath;

  return CH_RET_EROFS;
}

/**
 * @brief       Override of method @p vfsDrvMkdir().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Path of the directory to be created.
 * @param[in]     mode          Mode flags for the directory.
 * @return                      The operation result.
 */
msg_t __romdrv_mkdir_impl(void *ip, const char *path, vfs_mode_t mode) {

  (void)ip;
  (void)path;
  (void)mode;

  return CH_RET_EROFS;
}

/**
 * @brief       Override of method @p vfsDrvRmdir().
 *
 * @param[in,out] ip            Pointer to a @p vfs_rom_driver_c instance.
 * @param[in]     path          Path of the directory to be removed.
 * @return                      The operation result.
 */
msg_t __romdrv_rmdir_impl(void *ip, const char *path) {

  (void)ip;
  (void)path;

  return CH_RET_EROFS;
}
/** @} */

/**
 * @brief       VMT structure of VFS ROMFS driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct vfs_rom_driver_vmt __vfs_rom_driver_vmt = {
  .dispose                  = __romdrv_dispose_impl,
  .setcwd                   = __romdrv_setcwd_impl,
  .getcwd                   = __romdrv_getcwd_impl,
  .stat                     = __romdrv_stat_impl,
  .opendir                  = __romdrv_opendir_impl,
  .openfile                 = __romdrv_openfile_impl,
  .unlink                   = __romdrv_unlink_impl,
  .rename                   = __romdrv_rename_impl,
  .mkdir                    = __romdrv_mkdir_impl,
  .rmdir                    = __romdrv_rmdir_impl
};

/** @} */

/** @} */
