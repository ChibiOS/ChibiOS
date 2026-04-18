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

#include "packbits.h"

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static uint32_t get_file_type(const vfs_romfs_file_desc_t *filep) {

  return filep->flags & VFS_ROMFS_FILE_TYPE_MASK;
}

static bool is_raw_file(const vfs_romfs_file_desc_t *filep) {

  return get_file_type(filep) == VFS_ROMFS_FILE_TYPE_RAW;
}

static bool is_dynamic_file(const vfs_romfs_file_desc_t *filep) {

  return get_file_type(filep) == VFS_ROMFS_FILE_TYPE_DYNAMIC;
}

static bool is_compressed_file(const vfs_romfs_file_desc_t *filep) {

  return get_file_type(filep) == VFS_ROMFS_FILE_TYPE_COMPRESSED;
}

static msg_t validate_file_descriptor(const vfs_romfs_file_desc_t *filep) {

  switch (get_file_type(filep)) {
  case VFS_ROMFS_FILE_TYPE_RAW:
    if ((filep->size != (vfs_offset_t)0) && (filep->content.data == NULL)) {
      return CH_RET_EINVAL;
    }
    break;
  case VFS_ROMFS_FILE_TYPE_DYNAMIC:
    if (filep->content.dynamic.ops == NULL) {
      return CH_RET_EINVAL;
    }
    break;
  case VFS_ROMFS_FILE_TYPE_COMPRESSED:
    if (filep->content.compressed.ops == NULL) {
      return CH_RET_EINVAL;
    }
    break;
  default:
    return CH_RET_EINVAL;
  }

  return CH_RET_SUCCESS;
}

/* Compression backends are optional, keep the helper code out of the
   binary when the feature is disabled. */
#if DRV_CFG_ROM_ENABLE_COMPRESSION == TRUE
typedef msg_t (*romfs_chunked_validator_t)(
    const vfs_romfs_chunked_desc_t *descp);
typedef ssize_t (*romfs_chunked_chunk_reader_t)(
    const vfs_romfs_chunked_desc_t *descp,
    size_t chunk_index,
    size_t chunk_offset,
    uint8_t *buf,
    size_t n);

static size_t get_chunk_uncompressed_size(const vfs_romfs_chunked_desc_t *descp,
                                          size_t chunk_index) {
  size_t remaining_size;
  size_t chunk_size;

  remaining_size = (size_t)(descp->size -
                            (vfs_offset_t)(chunk_index * descp->chunk_size));
  chunk_size = descp->chunk_size;
  if (chunk_size > remaining_size) {
    chunk_size = remaining_size;
  }

  return chunk_size;
}

static msg_t validate_chunked_desc(const vfs_romfs_chunked_desc_t *descp) {
  size_t expected_chunks;
  size_t i;

  if (descp == NULL) {
    return CH_RET_EINVAL;
  }

  if (descp->size < (vfs_offset_t)0) {
    return CH_RET_EINVAL;
  }

  if (descp->size == (vfs_offset_t)0) {
    if (descp->chunks_num != 0U) {
      return CH_RET_EINVAL;
    }

    return CH_RET_SUCCESS;
  }

  if ((descp->chunk_size == 0U) ||
      (descp->offsets == NULL) ||
      (descp->data == NULL)) {
    return CH_RET_EINVAL;
  }

  expected_chunks = ((size_t)descp->size + descp->chunk_size - 1U) /
                    descp->chunk_size;
  if (descp->chunks_num != expected_chunks) {
    return CH_RET_EINVAL;
  }

  if (descp->offsets[0] != 0U) {
    return CH_RET_EINVAL;
  }

  i = 0U;
  while (i < descp->chunks_num) {
    if (descp->offsets[i + 1U] < descp->offsets[i]) {
      return CH_RET_EINVAL;
    }

    i++;
  }

  return CH_RET_SUCCESS;
}

static msg_t validate_chunked_stored_desc(const vfs_romfs_chunked_desc_t *descp) {
  size_t i;

  if (CH_RET_IS_ERROR(validate_chunked_desc(descp))) {
    return CH_RET_EINVAL;
  }

  i = 0U;
  while (i < descp->chunks_num) {
    uint32_t start;
    uint32_t end;
    size_t expected_size;

    start = descp->offsets[i];
    end = descp->offsets[i + 1U];
    expected_size = get_chunk_uncompressed_size(descp, i);
    if ((size_t)(end - start) != expected_size) {
      return CH_RET_EINVAL;
    }

    i++;
  }

  return CH_RET_SUCCESS;
}

static msg_t romfs_chunked_open(const void *arg, int flags,
                                void **sessionp,
                                vfs_offset_t *sizep,
                                romfs_chunked_validator_t validator) {
  const vfs_romfs_chunked_desc_t *descp =
    (const vfs_romfs_chunked_desc_t *)arg;
  msg_t ret;

  (void)flags;

  ret = validator(descp);
  if (CH_RET_IS_ERROR(ret)) {
    return ret;
  }

  *sessionp = (void *)descp;
  *sizep = descp->size;

  return CH_RET_SUCCESS;
}

static void romfs_chunked_close(void *session) {

  (void)session;
}

static ssize_t romfs_chunked_stored_read_chunk(
    const vfs_romfs_chunked_desc_t *descp,
    size_t chunk_index,
    size_t chunk_offset,
    uint8_t *buf,
    size_t n) {
  uint32_t chunk_start;
  uint32_t chunk_end;
  size_t chunk_available;

  chunk_start = descp->offsets[chunk_index];
  chunk_end = descp->offsets[chunk_index + 1U];
  chunk_available = (size_t)(chunk_end - chunk_start);
  if (chunk_offset > chunk_available) {
    return CH_RET_EINVAL;
  }

  chunk_available -= chunk_offset;
  if (chunk_available < n) {
    return CH_RET_EINVAL;
  }

  memcpy(buf, &descp->data[chunk_start + chunk_offset], n);

  return (ssize_t)n;
}

static ssize_t romfs_chunked_read(void *session, vfs_offset_t offset,
                                  uint8_t *buf, size_t n,
                                  romfs_chunked_chunk_reader_t reader) {
  const vfs_romfs_chunked_desc_t *descp =
    (const vfs_romfs_chunked_desc_t *)session;
  size_t transferred;

  if ((descp == NULL) || (offset < (vfs_offset_t)0) ||
      (offset > descp->size)) {
    return CH_RET_EINVAL;
  }

  transferred = 0U;
  while (n > 0U) {
    size_t chunk_index;
    size_t chunk_offset;
    size_t chunk_available;
    ssize_t copied;

    chunk_index = (size_t)offset / descp->chunk_size;
    chunk_offset = (size_t)offset % descp->chunk_size;
    chunk_available = get_chunk_uncompressed_size(descp, chunk_index);
    if (chunk_offset > chunk_available) {
      return CH_RET_EINVAL;
    }

    chunk_available -= chunk_offset;
    if (chunk_available > n) {
      chunk_available = n;
    }

    copied = reader(descp, chunk_index, chunk_offset, buf, chunk_available);
    if (CH_RET_IS_ERROR(copied)) {
      return copied;
    }

    transferred += (size_t)copied;
    buf += copied;
    n -= (size_t)copied;
    offset += (vfs_offset_t)copied;
  }

  return (ssize_t)transferred;
}

static msg_t romfs_chunked_stat(const void *arg, vfs_offset_t *sizep,
                                romfs_chunked_validator_t validator) {
  const vfs_romfs_chunked_desc_t *descp =
    (const vfs_romfs_chunked_desc_t *)arg;
  msg_t ret;

  ret = validator(descp);
  if (CH_RET_IS_ERROR(ret)) {
    return ret;
  }

  *sizep = descp->size;

  return CH_RET_SUCCESS;
}

static msg_t romfs_chunked_stored_open(const void *arg, int flags,
                                       void **sessionp,
                                       vfs_offset_t *sizep) {

  return romfs_chunked_open(arg, flags, sessionp, sizep,
                            validate_chunked_stored_desc);
}

static ssize_t romfs_chunked_packbits_read_chunk(
    const vfs_romfs_chunked_desc_t *descp,
    size_t chunk_index,
    size_t chunk_offset,
    uint8_t *buf,
    size_t n) {
  const uint8_t *chunk_data;
  size_t encoded_size;
  size_t chunk_size;

  chunk_data = &descp->data[descp->offsets[chunk_index]];
  encoded_size = (size_t)(descp->offsets[chunk_index + 1U] -
                          descp->offsets[chunk_index]);
  chunk_size = get_chunk_uncompressed_size(descp, chunk_index);

  if (!packbits_decode_slice(chunk_data, encoded_size, chunk_offset,
                             buf, n, chunk_size)) {
    return CH_RET_EINVAL;
  }

  return (ssize_t)n;
}

static ssize_t romfs_chunked_stored_read(void *session, vfs_offset_t offset,
                                         uint8_t *buf, size_t n) {

  return romfs_chunked_read(session, offset, buf, n,
                            romfs_chunked_stored_read_chunk);
}

static msg_t romfs_chunked_stored_stat(const void *arg, vfs_offset_t *sizep) {

  return romfs_chunked_stat(arg, sizep, validate_chunked_stored_desc);
}

static msg_t romfs_chunked_packbits_open(const void *arg, int flags,
                                         void **sessionp,
                                         vfs_offset_t *sizep) {

  return romfs_chunked_open(arg, flags, sessionp, sizep,
                            validate_chunked_desc);
}

static ssize_t romfs_chunked_packbits_read(void *session, vfs_offset_t offset,
                                           uint8_t *buf, size_t n) {

  return romfs_chunked_read(session, offset, buf, n,
                            romfs_chunked_packbits_read_chunk);
}

static msg_t romfs_chunked_packbits_stat(const void *arg, vfs_offset_t *sizep) {

  return romfs_chunked_stat(arg, sizep, validate_chunked_desc);
}
#endif /* DRV_CFG_ROM_ENABLE_COMPRESSION == TRUE */

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

  ret = validate_file_descriptor(filep);
  if (CH_RET_IS_ERROR(ret)) {
    return ret;
  }

  sp->mode = VFS_MODE_S_IFREG | filep->mode;
  sp->size = filep->size;

  if (is_dynamic_file(filep) && (filep->content.dynamic.ops->stat != NULL)) {
    ret = filep->content.dynamic.ops->stat(filep->content.dynamic.arg,
                                           &sp->size);
    if (CH_RET_IS_ERROR(ret)) {
      return ret;
    }
  }
#if DRV_CFG_ROM_ENABLE_COMPRESSION == TRUE
  else if (is_compressed_file(filep) &&
           (filep->content.compressed.ops->stat != NULL)) {
    ret = filep->content.compressed.ops->stat(filep->content.compressed.arg,
                                              &sp->size);
    if (CH_RET_IS_ERROR(ret)) {
      return ret;
    }
  }
#endif

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

const vfs_romfs_compressed_ops_t vfs_romfs_chunked_stored_ops = {
#if DRV_CFG_ROM_ENABLE_COMPRESSION == TRUE
  .open                     = romfs_chunked_stored_open,
  .close                    = romfs_chunked_close,
  .read                     = romfs_chunked_stored_read,
  .stat                     = romfs_chunked_stored_stat
#else
  .open                     = NULL,
  .close                    = NULL,
  .read                     = NULL,
  .stat                     = NULL
#endif
};

const vfs_romfs_compressed_ops_t vfs_romfs_chunked_packbits_ops = {
#if DRV_CFG_ROM_ENABLE_COMPRESSION == TRUE
  .open                     = romfs_chunked_packbits_open,
  .close                    = romfs_chunked_close,
  .read                     = romfs_chunked_packbits_read,
  .stat                     = romfs_chunked_packbits_stat
#else
  .open                     = NULL,
  .close                    = NULL,
  .read                     = NULL,
  .stat                     = NULL
#endif
};

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

  if (self->file != NULL) {
    if (is_dynamic_file(self->file) &&
        (self->file->content.dynamic.ops->close != NULL)) {
      self->file->content.dynamic.ops->close(self->session);
    }
    else if (is_compressed_file(self->file) &&
             (self->file->content.compressed.ops->close != NULL)) {
      self->file->content.compressed.ops->close(self->session);
    }
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

  if (CH_RET_IS_ERROR(validate_file_descriptor(self->file))) {
    return CH_RET_EINVAL;
  }

  if (self->position >= self->size) {
    return (ssize_t)0;
  }

  available = (size_t)(self->size - self->position);
  if (n > available) {
    n = available;
  }

  if (is_raw_file(self->file)) {
    memcpy(buf, &self->file->content.data[self->position], n);
    self->position += (vfs_offset_t)n;

    return (ssize_t)n;
  }

  if (is_dynamic_file(self->file)) {
    if (self->file->content.dynamic.ops->read == NULL) {
      return CH_RET_ENOSYS;
    }
    else {
      ssize_t ret;

      ret = self->file->content.dynamic.ops->read(self->session,
                                                  self->position,
                                                  buf, n);
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

  if (self->file->content.compressed.ops->read == NULL) {
    return CH_RET_ENOSYS;
  }
  else {
    ssize_t ret;

    ret = self->file->content.compressed.ops->read(self->session,
                                                   self->position,
                                                   buf, n);
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

      ret = validate_file_descriptor(filep);
      if (CH_RET_IS_ERROR(ret)) {
        goto romdrv_openfile_done;
      }

      size = filep->size;
      session = NULL;

      if (is_dynamic_file(filep)) {
        if (filep->content.dynamic.ops->open == NULL) {
          ret = CH_RET_ENOSYS;
          goto romdrv_openfile_done;
        }

        ret = filep->content.dynamic.ops->open(filep->content.dynamic.arg,
                                               flags, &session, &size);
        if (CH_RET_IS_ERROR(ret)) {
          goto romdrv_openfile_done;
        }
      }
      else if (is_compressed_file(filep)) {
#if DRV_CFG_ROM_ENABLE_COMPRESSION == FALSE
        ret = CH_RET_ENOSYS;
        goto romdrv_openfile_done;
#else
        if (filep->content.compressed.ops->open == NULL) {
          ret = CH_RET_ENOSYS;
          goto romdrv_openfile_done;
        }

        ret = filep->content.compressed.ops->open(
                filep->content.compressed.arg, flags, &session, &size);
        if (CH_RET_IS_ERROR(ret)) {
          goto romdrv_openfile_done;
        }
#endif
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
        if (is_dynamic_file(filep) &&
            (filep->content.dynamic.ops->close != NULL)) {
          filep->content.dynamic.ops->close(session);
        }
        else if (is_compressed_file(filep) &&
                 (filep->content.compressed.ops->close != NULL)) {
          filep->content.compressed.ops->close(session);
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
