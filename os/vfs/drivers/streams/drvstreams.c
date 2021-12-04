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

#if (VFS_CFG_ENABLE_DRV_STREAMS == TRUE) || defined(__DOXYGEN__)

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
                           int oflag,
                           vfs_file_node_t **vfnpp);

static const struct vfs_streams_driver_vmt driver_vmt = {
  .open_dir     = drv_open_dir,
  .open_file    = drv_open_file
};

static void node_dir_release(void *instance);
static msg_t node_dir_first(void *instance, vfs_node_info_t *nip);
static msg_t node_dir_next(void *instance, vfs_node_info_t *nip);

static const struct vfs_stream_dir_node_vmt dir_node_vmt = {
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

static const struct vfs_stream_file_node_vmt file_node_vmt = {
  .release         = node_file_release,
  .file_get_stream = node_file_get_stream,
  .file_read       = node_file_read,
  .file_write      = node_file_write,
  .file_setpos     = node_file_setpos,
  .file_getpos     = node_file_getpos,
  .file_getsize    = node_file_getsize
};

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp) {
  vfs_streams_driver_t *drvp = (vfs_streams_driver_t *)instance;
  msg_t err;

  do {
    vfs_stream_dir_node_t *sdnp;

    err = vfs_parse_match_separator(&path);
    VFS_BREAK_ON_ERROR(err);

    err = vfs_parse_match_end(&path);
    VFS_BREAK_ON_ERROR(err);

    sdnp = chPoolAlloc(&drvp->dir_nodes_pool);
    if (sdnp != NULL) {

      /* Node object initialization.*/
      sdnp->vmt    = &dir_node_vmt;
      sdnp->refs   = 1U;
      sdnp->driver = (vfs_driver_t *)drvp;
      sdnp->index  = 0U;

      *vdnpp = (vfs_directory_node_t *)sdnp;
      return VFS_RET_SUCCESS;
    }

    err = VFS_RET_ENOMEM;
  }
  while (false);

  return err;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           int oflag,
                           vfs_file_node_t **vfnpp) {
  vfs_streams_driver_t *drvp = (vfs_streams_driver_t *)instance;
  const drv_stream_element_t *dsep;
  msg_t err;

  (void)oflag;

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

        sfnp = chPoolAlloc(&drvp->file_nodes_pool);
        if (sfnp != NULL) {

          /* Node object initialization.*/
          sfnp->vmt    = &file_node_vmt;
          sfnp->refs   = 1U;
          sfnp->driver = (vfs_driver_t *)drvp;
          sfnp->stream = dsep->stream;

          *vfnpp = (vfs_file_node_t *)sfnp;
          return VFS_RET_SUCCESS;
        }

        return VFS_RET_ENOMEM;
      }

      dsep++;
    }

    err = VFS_RET_ENOENT;
  }
  while (false);

  return err;
}

static void node_dir_release(void *instance) {
  vfs_stream_dir_node_t *sdnp = (vfs_stream_dir_node_t *)instance;

  if (--sdnp->refs == 0U) {

    chPoolFree(&((vfs_streams_driver_t *)sdnp->driver)->dir_nodes_pool,
               (void *)sdnp);
  }
}

static msg_t node_dir_first(void *instance, vfs_node_info_t *nip) {
  vfs_stream_dir_node_t *sdnp = (vfs_stream_dir_node_t *)instance;

  sdnp->index = 0U;

  return node_dir_next(instance, nip);
}

static msg_t node_dir_next(void *instance, vfs_node_info_t *nip) {
  vfs_stream_dir_node_t *sdnp = (vfs_stream_dir_node_t *)instance;
  vfs_streams_driver_t *vsdp = (vfs_streams_driver_t *)sdnp->driver;

  if (vsdp->streams[sdnp->index].name != NULL) {

    nip->attr = VFS_NODE_ATTR_ISSTREAM;
    nip->size = (vfs_offset_t)0;
    strcpy(nip->name, vsdp->streams[sdnp->index].name);

    sdnp->index++;

    return VFS_RET_SUCCESS;
  }

  return VFS_RET_EOF;
}

static void node_file_release(void *instance) {
  vfs_stream_file_node_t *sfnp = (vfs_stream_file_node_t *)instance;

  if (--sfnp->refs == 0U) {

    chPoolFree(&((vfs_streams_driver_t *)sfnp->driver)->file_nodes_pool,
               (void *)sfnp);
  }
}

static BaseSequentialStream *node_file_get_stream(void *instance) {
  vfs_stream_file_node_t *sfnp = (vfs_stream_file_node_t *)instance;

  return sfnp->stream;
}

static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n) {
  vfs_stream_file_node_t *sfnp = (vfs_stream_file_node_t *)instance;

  return streamRead(sfnp->stream, buf, n);
}

static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n) {
  vfs_stream_file_node_t *sfnp = (vfs_stream_file_node_t *)instance;

  return streamWrite(sfnp->stream, buf, n);
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
 * @brief   VFS streams object initialization.
 *
 * @param[out] vsdp     pointer to a @p vfs_streams_driver_t structure
 * @param[in] rootname  name to be attributed to this object
 * @param[in] streams   pointer to an array of @p drv_stream_element_t objects
 * @return              A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_t *drvStreamsObjectInit(vfs_streams_driver_t *vsdp,
                                   const char *rootname,
                                   const drv_stream_element_t *streams) {

  vsdp->vmt      = &driver_vmt;
  vsdp->rootname = rootname;
  vsdp->streams  = streams;

  /* Initializing pools.*/
  chPoolObjectInit(&vsdp->dir_nodes_pool,
                   sizeof (vfs_stream_dir_node_t),
                   chCoreAllocAlignedI);
  chPoolObjectInit(&vsdp->file_nodes_pool,
                   sizeof (vfs_stream_file_node_t),
                   chCoreAllocAlignedI);

  /* Preloading pools.*/
  chPoolLoadArray(&vsdp->dir_nodes_pool,
                  &vsdp->dir_nodes[0],
                  DRV_CFG_STREAMS_DIR_NODES_NUM);
  chPoolLoadArray(&vsdp->file_nodes_pool,
                  &vsdp->file_nodes[0],
                  DRV_CFG_STREAMS_FILE_NODES_NUM);

  return (vfs_driver_t *)vsdp;
}

#endif /* VFS_CFG_ENABLE_DRV_STREAMS == TRUE */

/** @} */
