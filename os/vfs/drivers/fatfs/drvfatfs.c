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
 * @file    vfs/drivers/drvfatfs.c
 * @brief   FatFS VFS driver code.
 *
 * @addtogroup VFS_DRV_FATFS
 * @{
 */

#include "vfs.h"

#if (VFS_CFG_ENABLE_DRV_FATFS == TRUE) || defined(__DOXYGEN__)

#include "ff.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   @p vfs_fatfs_dir_node_c specific methods.
 */
#define __vfs_fatfs_dir_node_methods                                        \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_fatfs_dir_node_c specific data.
 */
#define __vfs_fatfs_dir_node_data                                           \
  __vfs_directory_node_data                                                 \
  DIR                           dir;

/**
 * @brief   @p vfs_fatfs_dir_node_c virtual methods table.
 */
struct vfs_fatfs_dir_node_vmt {
  __vfs_fatfs_dir_node_methods
};

/**
 * @brief   Type of a FatFS directory VFS node class.
 */
typedef struct vfs_fatfs_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_dir_node_vmt *vmt;
  __vfs_fatfs_dir_node_data
} vfs_fatfs_dir_node_c;

/**
 * @brief   @p vfs_fatfs_file_node_c specific methods.
 */
#define __vfs_fatfs_file_node_methods                                       \
  __vfs_file_node_methods

/**
 * @brief   @p vfs_fatfs_file_node_c specific data.
 */
#define __vfs_fatfs_file_node_data                                          \
  __vfs_file_node_data                                                      \
  FIL                           file;                                       \
  BaseSequentialStream          stream;

/**
 * @brief   @p vfs_fatfs_file_node_c virtual methods table.
 */
struct vfs_fatfs_file_node_vmt {
  __vfs_fatfs_file_node_methods
};

/**
 * @brief   Type of a FatFS file VFS node class.
 */
typedef struct vfs_fatfs_file_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_file_node_vmt *vmt;
  __vfs_fatfs_file_node_data
} vfs_fatfs_file_node_c;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static msg_t drv_set_cwd(void *instance, const char *path);
static msg_t drv_get_cwd(void *instance, char *buf, size_t size);
static msg_t drv_stat(void *instance, const char *path, vfs_stat_t *sp);
static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp);
static msg_t drv_open_file(void *instance,
                           const char *path,
                           int flags,
                           vfs_file_node_c **vfnpp);
msg_t drv_unlink(void *instance, const char *path);
msg_t drv_rename(void *instance, const char *oldpath, const char *newpath);
msg_t drv_mkdir(void *instance, const char *path, vfs_mode_t mode);
msg_t drv_rmdir(void *instance, const char *path);

static const struct vfs_fatfs_driver_vmt driver_vmt = {
  .set_cwd          = drv_set_cwd,
  .get_cwd          = drv_get_cwd,
  .stat             = drv_stat,
  .open_dir         = drv_open_dir,
  .open_file        = drv_open_file,
  .unlink           = drv_unlink,
  .rename           = drv_rename,
  .mkdir            = drv_mkdir,
  .rmdir            = drv_rmdir
};

static void *node_dir_addref(void *instance);
static void node_dir_release(void *instance);
static msg_t node_dir_stat(void *instance, vfs_stat_t *sp);
static msg_t node_dir_first(void *instance, vfs_direntry_info_t *dip);
static msg_t node_dir_next(void *instance, vfs_direntry_info_t *dip);

static const struct vfs_fatfs_dir_node_vmt dir_node_vmt = {
  .addref           = node_dir_addref,
  .release          = node_dir_release,
  .node_stat        = node_dir_stat,
  .dir_first        = node_dir_first,
  .dir_next         = node_dir_next
};

static void *node_file_addref(void *instance);
static void node_file_release(void *instance);
static msg_t node_file_stat(void *instance, vfs_stat_t *sp);
static BaseSequentialStream *node_file_get_stream(void *instance);
static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n);
static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n);
static msg_t node_file_setpos(void *instance,
                              vfs_offset_t offset,
                              vfs_seekmode_t whence);
static vfs_offset_t node_file_getpos(void *instance);

static const struct vfs_fatfs_file_node_vmt file_node_vmt = {
  .addref           = node_file_addref,
  .release          = node_file_release,
  .node_stat        = node_file_stat,
  .file_get_stream  = node_file_get_stream,
  .file_read        = node_file_read,
  .file_write       = node_file_write,
  .file_setpos      = node_file_setpos,
  .file_getpos      = node_file_getpos
};

static size_t file_stream_write(void *instance, const uint8_t *bp, size_t n);
static size_t file_stream_read(void *instance, uint8_t *bp, size_t n);
static msg_t file_stream_put(void *instance, uint8_t b);
static msg_t file_stream_get(void *instance);

static const struct BaseSequentialStreamVMT file_stream_vmt = {
  .instance_offset  = __CH_OFFSETOF(vfs_fatfs_file_node_c, stream.vmt),
  .write            = file_stream_write,
  .read             = file_stream_read,
  .put              = file_stream_put,
  .get              = file_stream_get
};

/**
 * @brief   Static members of @p vfs_fatfs_driver_c.
 */
static struct {
  /**
   * @brief   Pool of file system objects.
   */
  memory_pool_t                     fs_nodes_pool;
  /**
   * @brief   Pool of file info objects.
   */
  memory_pool_t                     info_nodes_pool;
  /**
   * @brief   Pool of directory nodes.
   */
  memory_pool_t                     dir_nodes_pool;
  /**
   * @brief   Pool of file nodes.
   */
  memory_pool_t                     file_nodes_pool;
  /**
   * @brief   Static storage of directory nodes.
   */
  vfs_fatfs_dir_node_c              dir_nodes[DRV_CFG_FATFS_DIR_NODES_NUM];
  /**
   * @brief   Static storage of file nodes.
   */
  vfs_fatfs_file_node_c             file_nodes[DRV_CFG_FATFS_FILE_NODES_NUM];
} vfs_fatfs_driver_static;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

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

static msg_t drv_set_cwd(void *instance, const char *path) {
#if FF_FS_RPATH >= 1

  (void)instance;

  return translate_error(f_chdir((const TCHAR *)path));
#else

  (void)instance;

  if (strcmp(path, "/") != 0) {
    return CH_RET_ENOENT;
  }

  return CH_RET_SUCCESS;
#endif
}

static msg_t drv_get_cwd(void *instance, char *buf, size_t size) {
#if FF_FS_RPATH >= 2

  (void)instance;

  return translate_error(f_getcwd((TCHAR *)buf, (UINT)size));
#else

  (void)instance;

  if (size < 2) {
    return CH_RET_ERANGE;
  }

  buf[0] = '/';
  buf[1] = '\0';

  return CH_RET_SUCCESS;
#endif
}

static msg_t drv_stat(void *instance, const char *path, vfs_stat_t *sp) {
  msg_t ret;

  (void)instance;

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

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp) {
  msg_t ret;

  do {
    vfs_fatfs_driver_c *drvp = (vfs_fatfs_driver_c *)instance;
    vfs_fatfs_dir_node_c *ffdnp;
    FRESULT res;

    ffdnp = chPoolAlloc(&vfs_fatfs_driver_static.dir_nodes_pool);
    if (ffdnp != NULL) {

      res = f_opendir(&ffdnp->dir, (const TCHAR *)path);
      if (res == FR_OK) {

        /* Node object initialization.*/
        __referenced_object_objinit_impl(ffdnp, &dir_node_vmt);
        ffdnp->driver   = (vfs_driver_c *)drvp;
        ffdnp->mode     = translate_mode(ffdnp->dir.obj.attr);

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

static msg_t drv_open_file(void *instance,
                           const char *path,
                           int flags,
                           vfs_file_node_c **vfnpp) {
  msg_t ret;

  do {
    vfs_fatfs_driver_c *drvp = (vfs_fatfs_driver_c *)instance;
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

      res = f_open(&fffnp->file, (const TCHAR *)path, mode);
      if (res == FR_OK) {

        /* Node object initialization.*/
        __referenced_object_objinit_impl(fffnp, &file_node_vmt);
        fffnp->driver     = (vfs_driver_c *)drvp;
        fffnp->mode       = translate_mode(fffnp->file.obj.attr);
        fffnp->stream.vmt = &file_stream_vmt;

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

msg_t drv_unlink(void *instance, const char *path) {

  (void)instance;

  return translate_error(f_unlink((const TCHAR *)path));
}

msg_t drv_rename(void *instance, const char *oldpath, const char *newpath) {

  (void)instance;

  return translate_error(f_rename((const TCHAR *)oldpath,
                                  (const TCHAR *)newpath));
}

msg_t drv_mkdir(void *instance, const char *path, vfs_mode_t mode) {

  (void)instance;
  (void)mode; /* Not handled by FatFS.*/

  return translate_error(f_mkdir((const TCHAR *)path));
}

msg_t drv_rmdir(void *instance, const char *path) {

  (void)instance;

  return translate_error(f_rmdir((const TCHAR *)path));
}

static void *node_dir_addref(void *instance) {

  return __referenced_object_addref_impl(instance);
}

static void node_dir_release(void *instance) {
  vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    chPoolFree(&vfs_fatfs_driver_static.dir_nodes_pool, (void *)ffdnp);
  }
}

static msg_t node_dir_stat(void *instance, vfs_stat_t *sp) {
  vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;

  sp->mode = ffdnp->mode;
  sp->size = (vfs_offset_t)0;

  return CH_RET_SUCCESS;
}

static msg_t node_dir_first(void *instance, vfs_direntry_info_t *dip) {
  vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;
  msg_t ret;
  FRESULT res;

  res = f_rewinddir(&ffdnp->dir);
  if (res == FR_OK) {
    ret = node_dir_next(instance, dip);
  }
  else {
    ret = translate_error(res);
  }

  return ret;
}

static msg_t node_dir_next(void *instance, vfs_direntry_info_t *dip) {
  msg_t ret;

  do {
    vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;
    FRESULT res;
    FILINFO *fip;

    fip = (FILINFO *)chPoolAlloc(&vfs_fatfs_driver_static.info_nodes_pool);
    if (fip != NULL) {

      res = f_readdir(&ffdnp->dir, fip);
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

static void *node_file_addref(void *instance) {

  return __referenced_object_addref_impl(instance);
}

static void node_file_release(void *instance) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    chPoolFree(&vfs_fatfs_driver_static.file_nodes_pool, (void *)fffnp);
  }
}

static msg_t node_file_stat(void *instance, vfs_stat_t *sp) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  sp->mode = fffnp->mode;
  sp->size = (vfs_offset_t)fffnp->file.obj.objsize;

  return CH_RET_SUCCESS;
}

static BaseSequentialStream *node_file_get_stream(void *instance) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  return &fffnp->stream;
}

static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;
  FRESULT res;
  UINT br;

  res = f_read(&fffnp->file, (void *)buf, (UINT)n, &br);
  if (res != FR_OK) {

    return translate_error(res);
  }

  return (ssize_t)br;
}

static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;
  FRESULT res;
  UINT bw;

  res = f_write(&fffnp->file, (const void *)buf, (UINT)n, &bw);
  if (res != FR_OK) {

    return translate_error(res);
  }

  return (ssize_t)bw;
}

#if 0
off_t sb_posix_lseek(int fd, off_t offset, int whence) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  msg_t ret;
  vfs_file_stat_t stat;
  off_t finaloff;

  if ((whence != SEEK_SET) || (whence == SEEK_CUR) || (whence != SEEK_END)) {
    return CH_RET_EINVAL;
  }

  if (!is_valid_descriptor(&sbp->io, fd)) {
    return CH_RET_EBADF;
  }

  if (sbp->io.attributes[fd] != 0) {
    return CH_RET_EISDIR;
  }

  ret = vfsGetFileStat((struct vfs_file_node *)sbp->io.vfs_nodes[fd], &stat);
  CH_RETURN_ON_ERROR(ret);

  if ((stat.attr & VFS_NODE_ATTR_ISSTREAM) != 0U) {
    return CH_RET_ESPIPE;
  }

  switch (whence) {
  case SEEK_SET:
    finaloff = offset;
    break;
  case SEEK_CUR:
    {
      off_t oldoff = vfsGetFilePosition((struct vfs_file_node *)sbp->io.vfs_nodes[fd]);
      CH_RETURN_ON_ERROR(oldoff);

      finaloff = oldoff + offset;
    }
    break;
  case SEEK_END:
    finaloff = stat.size + offset;
    break;
  }

  if (finaloff < 0) {
    return CH_RET_EOVERFLOW;
  }

  ret = vfsSetFilePosition((struct vfs_file_node *)sbp->io.vfs_nodes[fd],
                           finaloff);
  CH_RETURN_ON_ERROR(ret);

  return finaloff;
}
#endif

static msg_t node_file_setpos(void *instance,
                              vfs_offset_t offset,
                              vfs_seekmode_t whence) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;
  vfs_offset_t finaloff;

  chDbgCheck((whence == SEEK_SET) ||
             (whence == SEEK_CUR) ||
             (whence == SEEK_END));

  switch (whence) {
  case VFS_SEEK_CUR:
    finaloff = offset + (vfs_offset_t)fffnp->file.fptr;
    break;
  case VFS_SEEK_END:
    finaloff = offset + (vfs_offset_t)fffnp->file.obj.objsize;
    break;
  case VFS_SEEK_SET:
  default:
    finaloff = offset;
    break;
  }

  if (finaloff < 0) {
    return CH_RET_EOVERFLOW;
  }

  return translate_error(f_lseek(&fffnp->file, (FSIZE_t)finaloff));
}

static vfs_offset_t node_file_getpos(void *instance) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  return (vfs_offset_t)f_tell(&fffnp->file);
}

static size_t file_stream_write(void *instance, const uint8_t *bp, size_t n) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;

  msg = fffnp->vmt->file_write((void *)fffnp, bp, n);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
}

static size_t file_stream_read(void *instance, uint8_t *bp, size_t n) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;

  msg = fffnp->vmt->file_read((void *)fffnp, bp, n);
  if (CH_RET_IS_ERROR(msg)) {

    return (size_t)0;
  }

  return (size_t)msg;
}

static msg_t file_stream_put(void *instance, uint8_t b) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;

  msg = fffnp->vmt->file_write((void *)fffnp, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return msg;
}

static msg_t file_stream_get(void *instance) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;
  uint8_t b;

  msg = fffnp->vmt->file_read((void *)fffnp, &b, (size_t)1);
  if (CH_RET_IS_ERROR(msg)) {

    return STM_TIMEOUT;
  }

  return (msg_t)b;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Module initialization.
 *
 * @notapi
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
                   chCoreAllocAlignedI);

  /* Preloading pools.*/
  chPoolLoadArray(&vfs_fatfs_driver_static.dir_nodes_pool,
                  &vfs_fatfs_driver_static.dir_nodes[0],
                  DRV_CFG_FATFS_DIR_NODES_NUM);
  chPoolLoadArray(&vfs_fatfs_driver_static.file_nodes_pool,
                  &vfs_fatfs_driver_static.file_nodes[0],
                  DRV_CFG_FATFS_FILE_NODES_NUM);
}

/**
 * @brief   VFS FatFS object initialization.
 *
 * @param[out] vffdp    Pointer to a @p vfs_fatfs_driver_c structure.
 * @return              A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_c *drvFatFSObjectInit(vfs_fatfs_driver_c *vffdp) {

  __base_object_objinit_impl(vffdp, &driver_vmt);

  return (vfs_driver_c *)vffdp;
}

/**
 * @brief   Mounts a FatFS volume.
 *
 * @param[in] name      Name to be assigned to the volume, see FatFS
 *                      @p f_mount() documentation because there are several
 *                      options.
 * @param[in] mountnow  Immediate mount option.
 * @return              The operation result.
 *
 * @api
 */
msg_t drvFatFSMount(const char *name, bool mountnow) {
  FATFS *fs;

  fs = f_getfs(name);
  if (fs == NULL) {
    fs = chPoolAlloc(&vfs_fatfs_driver_static.fs_nodes_pool);
    if (fs == NULL) {
      return CH_RET_ENOMEM;
    }
  }

  return translate_error(f_mount(fs, name, (BYTE)(mountnow ? 1 : 0)));
}

/**
 * @brief   Unmounts a FatFS volume.
 *
 * @param[in] name      Name of the volume to be unmounted.
 * @return              The operation result.
 *
 * @api
 */
msg_t drvFatFSUnmount(const char *name) {
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

#endif /* VFS_CFG_ENABLE_DRV_FATFS == TRUE */

/** @} */
