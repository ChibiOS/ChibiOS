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
#include "drvfatfs.h"

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
                          vfs_directory_node_t **vdnpp);
static msg_t drv_open_file(void *instance,
                           const char *path,
                           unsigned mode,
                           vfs_file_node_t **vfnpp);

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
  .release         = node_file_release,
  .file_get_stream = node_file_get_stream,
  .file_read       = node_file_read,
  .file_write      = node_file_write,
  .file_setpos     = node_file_setpos,
  .file_getpos     = node_file_getpos,
  .file_getsize    = node_file_getsize
};

static vfs_fatfs_dir_node_t drv_dir_nodes[DRV_CFG_FATFS_DIR_NODES_NUM];
static vfs_fatfs_file_node_t drv_file_nodes[DRV_CFG_FATFS_FILE_NODES_NUM];

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
  case FR_TOO_MANY_OPEN_FILES:
    msg = VFS_RET_NO_RESOURCE;
    break;
  case FR_DISK_ERR:
  case FR_NOT_READY:
  case FR_INVALID_DRIVE:
  case FR_NO_FILESYSTEM:
    msg = VFS_RET_MEDIA_ERROR;
    break;
  case FR_NO_FILE:
    msg = VFS_RET_NOT_FOUND;
    break;
  case FR_NO_PATH:
  case FR_INVALID_NAME:
    msg = VFS_RET_INVALID_PATH;
    break;
  case FR_DENIED:
  case FR_WRITE_PROTECTED:
    msg = VFS_RET_ACCESS_ERROR;
    break;
  case FR_EXIST:
    msg = VFS_RET_EXIST;
    break;
  default:
    msg = VFS_RET_INNER_ERROR;
    break;
  }

  return msg;
}

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp) {
  msg_t err;

  do {
    vfs_fatfs_driver_t *drvp = (vfs_fatfs_driver_t *)instance;
    vfs_fatfs_dir_node_t *ffdnp;
    FRESULT res;

    ffdnp = chPoolAlloc(&drvp->dir_nodes_pool);
    if (ffdnp != NULL) {

      /* Node object initialization.*/
      ffdnp->vmt    = &dir_node_vmt;
      ffdnp->refs   = 1U;
      ffdnp->driver = (vfs_driver_t *)drvp;

      res = f_opendir(&ffdnp->dir, (TCHAR *)path);
      if (res == FR_OK) {
        *vdnpp = (vfs_directory_node_t *)ffdnp;
        err = VFS_RET_SUCCESS;
        break;
      }

      chPoolFree(&drvp->dir_nodes_pool, (void *)ffdnp);
    }

    err = translate_error(res);
  }
  while (false);

  return err;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           unsigned mode,
                           vfs_file_node_t **vfnpp) {
  msg_t err;

  do {
    vfs_fatfs_driver_t *drvp = (vfs_fatfs_driver_t *)instance;
    vfs_fatfs_file_node_t *fffnp;
    FRESULT res;

    fffnp = chPoolAlloc(&drvp->file_nodes_pool);
    if (fffnp != NULL) {

      /* Node object initialization.*/
      fffnp->vmt    = &file_node_vmt;
      fffnp->refs   = 1U;
      fffnp->driver = (vfs_driver_t *)drvp;

      res = f_open(&fffnp->file, (TCHAR *)path, (BYTE)mode);
      if (res == FR_OK) {
        *vfnpp = (vfs_file_node_t *)fffnp;
        err = VFS_RET_SUCCESS;
        break;
      }

      chPoolFree(&drvp->file_nodes_pool, (void *)fffnp);
    }

    err = translate_error(res);
  }
  while (false);

  return err;
}

static void node_dir_release(void *instance) {
  vfs_fatfs_dir_node_t *ffdnp = (vfs_fatfs_dir_node_t *)instance;

  if (--ffdnp->refs == 0U) {

    chPoolFree(&((vfs_fatfs_driver_t *)ffdnp->driver)->dir_nodes_pool,
               (void *)ffdnp);
  }
}

static msg_t node_dir_first(void *instance, vfs_node_info_t *nip) {
  vfs_fatfs_dir_node_t *ffdnp = (vfs_fatfs_dir_node_t *)instance;
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
    vfs_fatfs_dir_node_t *ffdnp = (vfs_fatfs_dir_node_t *)instance;
    vfs_fatfs_driver_t *drvp = (vfs_fatfs_driver_t *)ffdnp->driver;
    FRESULT res;
    FILINFO *fip;

    fip = (FILINFO *)chPoolAlloc(&drvp->info_nodes_pool);
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

      chPoolFree(&drvp->info_nodes_pool, (void *)fip);
    }
  }
  while (false);

  return err;
}

static void node_file_release(void *instance) {
  vfs_fatfs_file_node_t *fffnp = (vfs_fatfs_file_node_t *)instance;

  if (--fffnp->refs == 0U) {

    chPoolFree(&((vfs_fatfs_driver_t *)fffnp->driver)->file_nodes_pool,
               (void *)fffnp);
  }
}

static BaseSequentialStream *node_file_get_stream(void *instance) {
  vfs_fatfs_file_node_t *fffnp = (vfs_fatfs_file_node_t *)instance;

  return &fffnp->stream;
}

static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n) {
  vfs_fatfs_file_node_t *fffnp = (vfs_fatfs_file_node_t *)instance;

  return streamRead(&fffnp->stream, buf, n);
}

static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n) {
  vfs_fatfs_file_node_t *fffnp = (vfs_fatfs_file_node_t *)instance;

  return streamWrite(&fffnp->stream, buf, n);
}

static msg_t node_file_setpos(void *instance, vfs_offset_t offset) {

  (void)instance;
  (void)offset;

  return VFS_RET_NOT_IMPLEMENTED;
}

static vfs_offset_t node_file_getpos(void *instance) {

  (void)instance;

  return 0U;
}

static vfs_offset_t node_file_getsize(void *instance) {

  (void)instance;

  return 0U;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   VFS FatFS object initialization.
 *
 * @param[out] ffdp     pointer to a @p vfs_fatfs_driver_t structure
 * @param[in] rootname  name to be attributed to this object
 * @return              A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_t *drvFatFSObjectInit(vfs_fatfs_driver_t *ffdp,
                                 const char *rootname) {

  ffdp->vmt      = &driver_vmt;
  ffdp->rootname = rootname;

  /* Initializing pools.*/
  chPoolObjectInit(&ffdp->dir_nodes_pool,
                   sizeof (vfs_fatfs_dir_node_t),
                   chCoreAllocAligned);
  chPoolObjectInit(&ffdp->file_nodes_pool,
                   sizeof (vfs_fatfs_file_node_t),
                   chCoreAllocAligned);
  chPoolObjectInit(&ffdp->info_nodes_pool,
                   sizeof (FILINFO),
                   chCoreAllocAligned);

  /* Preloading pools.*/
  chPoolLoadArray(&ffdp->dir_nodes_pool,
                  drv_dir_nodes,
                  DRV_CFG_FATFS_DIR_NODES_NUM);
  chPoolLoadArray(&ffdp->file_nodes_pool,
                  drv_file_nodes,
                  DRV_CFG_FATFS_FILE_NODES_NUM);

  return (vfs_driver_t *)ffdp;
}

/**
 * @brief   Mounts a FatFS volume.
 *
 * @param[in] ffdp      pointer to a @p vfs_fatfs_driver_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t drvFatFSMount(vfs_fatfs_driver_t *ffdp) {

  return translate_error(f_mount(&ffdp->fs,
                                 ffdp->rootname,
                                 DRV_CFG_FATFS_MOUNT_MODE));
}

/**
 * @brief   Unmounts a FatFS volume.
 *
 * @param[in] ffdp      pointer to a @p vfs_fatfs_driver_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t drvFatFSUnmount(vfs_fatfs_driver_t *ffdp) {

  return translate_error(f_unmount(ffdp->rootname));
}

/** @} */
