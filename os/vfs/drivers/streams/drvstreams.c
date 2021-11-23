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
 * @file    vfs/drivers/drvstreams.c
 * @brief   HAL streams VFS driver code.
 *
 * @addtogroup VFS_DRV_STREAMS
 * @{
 */

#include <string.h>

#include "vfs.h"
#include "drvstreams.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Number of nodes pre-allocated in the pool.
 */
#define DRV_NODES_NUM           2

/**
 * @brief   @p vfs_streams_driver_t specific methods.
 */
#define __vfs_streams_driver_methods                                        \
  __vfs_driver_methods

/**
 * @brief   @p vfs_streams_driver_t specific data.
 */
#define __vfs_streams_driver_data                                           \
  __vfs_driver_data                                                         \
  const drv_stream_element_t    *streams;                                   \
  memory_pool_t                 nodes_pool;

/**
 * @brief   @p vfs_stream_file_node_t specific methods.
 */
#define __vfs_stream_file_node_methods                                      \
  __vfs_file_node_methods

/**
 * @brief   @p vfs_stream_file_node_t specific data.
 */
#define __vfs_stream_file_node_data                                         \
  __vfs_file_node_data                                                      \
  BaseSequentialStream                  *stream;

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   @p vfs_streams_driver_t virtual methods table.
 */
struct vfs_streams_driver_vmt {
  __vfs_streams_driver_methods
};

/**
 * @brief   Type of a structure representing a VFS streams driver.
 */
typedef struct vfs_drv_streams {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_streams_driver_vmt   *vmt;
  __vfs_streams_driver_data
} vfs_streams_driver_t;

/**
 * @brief   @p vfs_file_node_t virtual methods table.
 */
struct vfs_stream_file_node_vmt {
  __vfs_stream_file_node_methods
};

/**
 * @brief   Type of a structure representing a stream file VFS node.
 */
typedef struct vfs_stream_file_node {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_stream_file_node_vmt *vmt;
  __vfs_stream_file_node_data
} vfs_stream_file_node_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp);
static msg_t drv_open_file(void *instance,
                           const char *path,
                           vfs_file_node_t **vfnpp);

static const struct vfs_streams_driver_vmt vmt = {
  .open_dir     = drv_open_dir,
  .open_file    = drv_open_file
};

static vfs_stream_file_node_t drv_file_nodes[DRV_NODES_NUM];
static vfs_streams_driver_t drv_streams;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp) {

  (void)instance;
  (void)path;
  (void)vdnpp;

  return VFS_RET_NOT_FOUND;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           vfs_file_node_t **vfnpp) {
  vfs_streams_driver_t *drvp = (vfs_streams_driver_t *)instance;
  const drv_stream_element_t *dsep;
  msg_t err;

  do {
    char fname[VFS_CFG_MAX_NAMELEN + 1];

    err = vfs_parse_match_separator(&path);
    VFS_BREAK_ON_ERROR(err);

    err = vfs_parse_filename(&path, fname);
    VFS_BREAK_ON_ERROR(err);

    err = vfs_parse_match_end(&path);
    VFS_BREAK_ON_ERROR(err);

    dsep = &drvp->streams[0];
    while (dsep->name != NULL) {
      if (strncmp(fname, dsep->name, VFS_CFG_MAX_NAMELEN) == 0) {
        vfs_stream_file_node_t *sfnp;

        sfnp = chPoolAlloc(&drv_streams.nodes_pool);
        if (sfnp != NULL) {

          /* Node object initialization.*/
          sfnp->vmt    = 0;
          sfnp->refs   = 1U;
          sfnp->driver = (vfs_driver_t *)drvp;
          sfnp->stream = dsep->stream;

          *vfnpp = (vfs_file_node_t *)sfnp;
          return VFS_RET_SUCCESS;
        }

        return VFS_RET_NO_RESOURCE;
      }

      dsep++;
    }

    err = VFS_RET_NOT_FOUND;
  }
  while (false);

  return err;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

vfs_driver_t *drvStreamsInit(const char *rootname,
                             const drv_stream_element_t *streams) {

  drv_streams.vmt      = &vmt;
  drv_streams.rootname = rootname;
  drv_streams.streams  = streams;
  chPoolObjectInit(&drv_streams.nodes_pool,
                   sizeof (vfs_stream_file_node_t),
                   chCoreAllocAligned);
  chPoolLoadArray(&drv_streams.nodes_pool, drv_file_nodes, DRV_NODES_NUM);

  return (vfs_driver_t *)&drv_streams;
}

/** @} */
