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
 * @brief   @p vfs_drv_streams_t specific methods.
 */
#define __vfs_drv_streams_methods                                           \
  __vfs_driver_methods

/**
 * @brief   @p vfs_drv_streams_t specific data.
 */
#define __vfs_drv_streams_data                                              \
  __vfs_driver_data                                                         \
  const drv_stream_element_t *streams;

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief   @p vfs_drv_streams_t virtual methods table.
 */
struct vfs_drv_streams_vmt {
  __vfs_drv_streams_methods
};

/**
 * @brief   Type of a structure representing a VFS streams driver.
 */
typedef struct vfs_drv_streams {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct vfs_drv_streams_vmt   *vmt;
  __vfs_drv_streams_data
} vfs_drv_streams_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp);
static msg_t drv_open_file(void *instance,
                           const char *path,
                           vfs_file_node_t **vfnpp);

static const struct vfs_drv_streams_vmt vmt = {
  .open_dir     = drv_open_dir,
  .open_file    = drv_open_file
};

static vfs_drv_streams_t drv_streams;

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
  vfs_drv_streams_t *drvp = (vfs_drv_streams_t *)instance;
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

        *vfnpp = NULL;
        return VFS_RET_SUCCESS;
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

  return (vfs_driver_t *)&drv_streams;
}

/** @} */
