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
 * @brief   HAL FatFS VFS driver code.
 *
 * @addtogroup VFS_DRV_FATFS
 * @{
 */

#include <string.h>

#include "vfs.h"

#if (VFS_CFG_ENABLE_DRV_FATFS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
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

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp);
static msg_t drv_open_file(void *instance,
                           const char *path,
                           int oflag,
                           vfs_file_node_c **vfnpp);

static const struct vfs_fatfs_driver_vmt driver_vmt = {
  .open_dir     = drv_open_dir,
  .open_file    = drv_open_file
};

static void node_dir_release(void *instance);
static msg_t node_dir_first(void *instance, vfs_node_info_t *nip);
static msg_t node_dir_next(void *instance, vfs_node_info_t *nip);

static const struct vfs_fatfs_dir_node_vmt dir_node_vmt = {
  .release      = node_dir_release,
  .dir_first    = node_dir_first,
  .dir_next     = node_dir_next
};

static void node_file_release(void *instance);
static BaseSequentialStream *node_file_get_stream(void *instance);
static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n);
static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n);
static msg_t node_file_setpos(void *instance, vfs_offset_t offset);
static vfs_offset_t node_file_getpos(void *instance);
static vfs_offset_t node_file_getsize(void *instance);

static const struct vfs_fatfs_file_node_vmt file_node_vmt = {
  .release          = node_file_release,
  .file_get_stream  = node_file_get_stream,
  .file_read        = node_file_read,
  .file_write       = node_file_write,
  .file_setpos      = node_file_setpos,
  .file_getpos      = node_file_getpos,
  .file_getsize     = node_file_getsize
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
    msg = VFS_RET_SUCCESS;
    break;
  case FR_TIMEOUT:
    msg = VFS_RET_TIMEOUT;
    break;
  case FR_NOT_ENOUGH_CORE:
    msg = VFS_RET_ENOMEM;
    break;
  case FR_TOO_MANY_OPEN_FILES:
    msg = VFS_RET_ENFILE;
    break;
  case FR_DISK_ERR:
  case FR_NOT_READY:
  case FR_INVALID_DRIVE:
  case FR_NO_FILESYSTEM:
    msg = VFS_RET_EIO;
    break;
  case FR_NO_FILE:
  case FR_NO_PATH:
  case FR_INVALID_NAME:
    msg = VFS_RET_ENOENT;
    break;
  case FR_DENIED:
  case FR_WRITE_PROTECTED:
    msg = VFS_RET_EACCES;
    break;
  case FR_EXIST:
    msg = VFS_RET_EEXIST;
    break;
  default:
    msg = VFS_RET_INNER_ERROR;
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

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp) {
  msg_t err;

  do {
    vfs_fatfs_driver_c *drvp = (vfs_fatfs_driver_c *)instance;
    vfs_fatfs_dir_node_c *ffdnp;
    FRESULT res;

    ffdnp = chPoolAlloc(&vfs_fatfs_driver_static.dir_nodes_pool);
    if (ffdnp != NULL) {

      /* Node object initialization.*/
      __referenced_object_objinit_impl(ffdnp, &dir_node_vmt);
      ffdnp->driver     = (vfs_driver_c *)drvp;

      res = f_opendir(&ffdnp->dir, (TCHAR *)path);
      if (res == FR_OK) {
        *vdnpp = (vfs_directory_node_c *)ffdnp;
        err = VFS_RET_SUCCESS;
        break;
      }

      chPoolFree(&vfs_fatfs_driver_static.dir_nodes_pool, (void *)ffdnp);
    }

    err = translate_error(res);
  }
  while (false);

  return err;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           int oflag,
                           vfs_file_node_c **vfnpp) {
  msg_t err;

  do {
    vfs_fatfs_driver_c *drvp = (vfs_fatfs_driver_c *)instance;
    vfs_fatfs_file_node_c *fffnp;
    FRESULT res;
    BYTE mode;

    mode = translate_oflag(oflag);
    if (mode == (BYTE)0) {
      err = VFS_RET_EINVAL;
      break;
    }

    fffnp = chPoolAlloc(&vfs_fatfs_driver_static.file_nodes_pool);
    if (fffnp != NULL) {

      /* Node object initialization.*/
      __referenced_object_objinit_impl(fffnp, &file_node_vmt);
      fffnp->driver     = (vfs_driver_c *)drvp;
      fffnp->stream.vmt = &file_stream_vmt;

      res = f_open(&fffnp->file, (TCHAR *)path, mode);
      if (res == FR_OK) {
        *vfnpp = (vfs_file_node_c *)fffnp;
        err = VFS_RET_SUCCESS;
        break;
      }

      chPoolFree(&vfs_fatfs_driver_static.file_nodes_pool, (void *)fffnp);
    }

    err = translate_error(res);
  }
  while (false);

  return err;
}

static void node_dir_release(void *instance) {
  vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    chPoolFree(&vfs_fatfs_driver_static.dir_nodes_pool, (void *)ffdnp);
  }
}

static msg_t node_dir_first(void *instance, vfs_node_info_t *nip) {
  vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;
  msg_t err;
  FRESULT res;

  res = f_rewinddir(&ffdnp->dir);
  if (res == FR_OK) {
    err = node_dir_next(instance, nip);
  }
  else {
    err = translate_error(res);
  }

  return err;
}

static msg_t node_dir_next(void *instance, vfs_node_info_t *nip) {
  msg_t err;

  do {
    vfs_fatfs_dir_node_c *ffdnp = (vfs_fatfs_dir_node_c *)instance;
    FRESULT res;
    FILINFO *fip;

    fip = (FILINFO *)chPoolAlloc(&vfs_fatfs_driver_static.info_nodes_pool);
    if (fip != NULL) {

      res = f_readdir(&ffdnp->dir, fip);
      if (res == FR_OK) {
        if (fip->fname[0] == '\0') {
          err = VFS_RET_EOF;
        }
        else {
          nip->attr = (vfs_nodeattr_t)fip->fattrib;
          nip->size = (vfs_offset_t)fip->fsize;
          strncpy(nip->name, fip->fname, VFS_CFG_MAX_NAMELEN);
          nip->name[VFS_CFG_MAX_NAMELEN] = '\0';
          err = VFS_RET_SUCCESS;
        }
      }
      else {
        err = translate_error(res);
      }

      chPoolFree(&vfs_fatfs_driver_static.info_nodes_pool, (void *)fip);
    }
  }
  while (false);

  return err;
}

static void node_file_release(void *instance) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    chPoolFree(&vfs_fatfs_driver_static.file_nodes_pool, (void *)fffnp);
  }
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

static msg_t node_file_setpos(void *instance, vfs_offset_t offset) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  return translate_error(f_lseek(&fffnp->file, (FSIZE_t)offset));
}

static vfs_offset_t node_file_getpos(void *instance) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  return (vfs_offset_t)f_tell(&fffnp->file);
}

static vfs_offset_t node_file_getsize(void *instance) {
  vfs_fatfs_file_node_c *fffnp = (vfs_fatfs_file_node_c *)instance;

  return (vfs_offset_t)f_size(&fffnp->file);
}

static size_t file_stream_write(void *instance, const uint8_t *bp, size_t n) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;

  msg = fffnp->vmt->file_write((void *)fffnp, bp, n);
  if (msg < VFS_RET_SUCCESS) {

    return (size_t)0;
  }

  return (size_t)msg;
}

static size_t file_stream_read(void *instance, uint8_t *bp, size_t n) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;

  msg = fffnp->vmt->file_read((void *)fffnp, bp, n);
  if (msg < VFS_RET_SUCCESS) {

    return (size_t)0;
  }

  return (size_t)msg;
}

static msg_t file_stream_put(void *instance, uint8_t b) {
  vfs_fatfs_file_node_c *fffnp = objGetInstance(vfs_fatfs_file_node_c *,
                                                (BaseSequentialStream *)instance);
  msg_t msg;

  msg = fffnp->vmt->file_write((void *)fffnp, &b, (size_t)1);
  if (msg < VFS_RET_SUCCESS) {

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
  if (msg < VFS_RET_SUCCESS) {

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
void __vfs_fatfs_driver_init(void) {

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
 * @param[out] ffdp     pointer to a @p vfs_fatfs_driver_c structure
 * @param[in] rootname  name to be attributed to this object
 * @return              A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_c *drvFatFSObjectInit(vfs_fatfs_driver_c *vffdp,
                                 const char *rootname) {

  __base_object_objinit_impl(vffdp, &driver_vmt);
  vffdp->rootname = rootname;

  return (vfs_driver_c *)vffdp;
}

/**
 * @brief   Mounts a FatFS volume.
 *
 * @param[in] name      name to be assigned to the volume, see FatFS
 *                      @p f_mount() documentation because there are several
 *                      options
 * @param[in] mountnow  immediate mount option
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
      return VFS_RET_ENOMEM;
    }
  }

  return translate_error(f_mount(fs, name, (BYTE)(mountnow ? 1 : 0)));
}

/**
 * @brief   Unmounts a FatFS volume.
 *
 * @param[in] ffdp      pointer to a @p vfs_fatfs_driver_c structure
 * @return              The operation result.
 *
 * @api
 */
msg_t drvFatFSUnmount(const char *name) {
  FATFS *fs;
  FRESULT res;

  fs = f_getfs(name);
  if (fs == NULL) {
    return VFS_RET_EINVAL;
  }

  res = f_unmount(name);

  chPoolFree(&vfs_fatfs_driver_static.fs_nodes_pool, (void *)fs);

  return translate_error(res);
}

#endif /* VFS_CFG_ENABLE_DRV_FATFS == TRUE */

/** @} */
