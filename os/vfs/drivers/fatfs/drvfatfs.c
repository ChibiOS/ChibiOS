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

#include "ff.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Number of directory nodes pre-allocated in the pool.
 */
#define DRV_DIR_NODES_NUM       1

/**
 * @brief   Number of file nodes pre-allocated in the pool.
 */
#define DRV_FILE_NODES_NUM      2

/**
 * @brief   @p vfs_fatfs_driver_t specific methods.
 */
#define __vfs_fatfs_driver_methods                                          \
  __vfs_driver_methods

/**
 * @brief   @p vfs_fatfs_driver_t specific data.
 */
#define __vfs_fatfs_driver_data                                             \
  __vfs_driver_data                                                         \
  memory_pool_t                 file_nodes_pool;                            \
  memory_pool_t                 dir_nodes_pool;

/**
 * @brief   @p vfs_fatfs_dir_node_t specific methods.
 */
#define __vfs_fatfs_dir_node_methods                                        \
  __vfs_directory_node_methods

/**
 * @brief   @p vfs_fatfs_dir_node_t specific data.
 */
#define __vfs_fatfs_dir_node_data                                           \
  __vfs_directory_node_data                                                 \
  DIR                           dir;

/**
 * @brief   @p vfs_fatfs_file_node_t specific methods.
 */
#define __vfs_fatfs_file_node_methods                                       \
  __vfs_file_node_methods                                                   \
  FIL                           file;

/**
 * @brief   @p vfs_fatfs_file_node_t specific data.
 */
#define __vfs_fatfs_file_node_data                                          \
  __vfs_file_node_data                                                      \
  BaseSequentialStream                  *stream;

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   @p vfs_fatfs_driver_t virtual methods table.
 */
struct vfs_fatfs_driver_vmt {
  __vfs_fatfs_driver_methods
};

/**
 * @brief   Type of a structure representing a VFS FatFS driver.
 */
typedef struct vfs_drv_streams {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_driver_vmt   *vmt;
  __vfs_fatfs_driver_data
} vfs_fatfs_driver_t;

/**
 * @brief   @p vfs_fatfs_dir_node_t virtual methods table.
 */
struct vfs_fatfs_dir_node_vmt {
  __vfs_fatfs_dir_node_methods
};

/**
 * @brief   Type of a structure representing a FatFS directory VFS node.
 */
typedef struct vfs_fatfs_dir_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_dir_node_vmt *vmt;
  __vfs_fatfs_dir_node_data
} vfs_fatfs_dir_node_t;

/**
 * @brief   @p vfs_fatfs_file_node_t virtual methods table.
 */
struct vfs_fatfs_file_node_vmt {
  __vfs_fatfs_file_node_methods
};

/**
 * @brief   Type of a structure representing a FatFS file VFS node.
 */
typedef struct vfs_fatfs_file_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_fatfs_file_node_vmt *vmt;
  __vfs_fatfs_file_node_data
} vfs_fatfs_file_node_t;

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

static vfs_fatfs_dir_node_t drv_dir_nodes[DRV_DIR_NODES_NUM];
static vfs_fatfs_file_node_t drv_file_nodes[DRV_FILE_NODES_NUM];

static vfs_fatfs_driver_t drv_fatfs;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp) {
  vfs_fatfs_driver_t *drvp = (vfs_fatfs_driver_t *)instance;
  msg_t err;

  do {

    err = VFS_RET_NO_RESOURCE;
  }
  while (false);

  return err;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           unsigned mode,
                           vfs_file_node_t **vfnpp) {
  vfs_fatfs_driver_t *drvp = (vfs_fatfs_driver_t *)instance;
  msg_t err;

  do {

    err = VFS_RET_NOT_FOUND;
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

  return VFS_RET_EOF;
}

static msg_t node_dir_next(void *instance, vfs_node_info_t *nip) {
  vfs_fatfs_dir_node_t *ffdnp = (vfs_fatfs_dir_node_t *)instance;

  return VFS_RET_EOF;
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

  return fffnp->stream;
}

static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n) {
  vfs_fatfs_file_node_t *fffnp = (vfs_fatfs_file_node_t *)instance;

  return streamRead(fffnp->stream, buf, n);
}

static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n) {
  vfs_fatfs_file_node_t *fffnp = (vfs_fatfs_file_node_t *)instance;

  return streamWrite(fffnp->stream, buf, n);
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

vfs_driver_t *drvFatfsInit(const char *rootname) {

  drv_fatfs.vmt      = &driver_vmt;
  drv_fatfs.rootname = rootname;
  chPoolObjectInit(&drv_fatfs.dir_nodes_pool,
                   sizeof (vfs_fatfs_dir_node_t),
                   chCoreAllocAligned);
  chPoolLoadArray(&drv_fatfs.dir_nodes_pool,
                  drv_dir_nodes,
                  DRV_DIR_NODES_NUM);
  chPoolObjectInit(&drv_fatfs.file_nodes_pool,
                   sizeof (vfs_fatfs_file_node_t),
                   chCoreAllocAligned);
  chPoolLoadArray(&drv_fatfs.file_nodes_pool,
                  drv_file_nodes,
                  DRV_FILE_NODES_NUM);

  return (vfs_driver_t *)&drv_fatfs;
}

/** @} */
