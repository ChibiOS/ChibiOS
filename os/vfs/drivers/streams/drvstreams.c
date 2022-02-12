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

static const struct vfs_streams_driver_vmt driver_vmt = {
  .set_cwd          = drv_set_cwd,
  .get_cwd          = drv_get_cwd,
  .stat             = drv_stat,
  .open_dir         = drv_open_dir,
  .open_file        = drv_open_file,
  .unlink           = drv_unlink_unimpl,
  .rename           = drv_rename_unimpl,
  .mkdir            = drv_mkdir_unimpl,
  .rmdir            = drv_rmdir_unimpl
};

static void *node_dir_addref(void *instance);
static void node_dir_release(void *instance);
static msg_t node_dir_getstat(void *instance, vfs_stat_t *sp);
static msg_t node_dir_first(void *instance, vfs_direntry_info_t *dip);
static msg_t node_dir_next(void *instance, vfs_direntry_info_t *dip);

static const struct vfs_streams_dir_node_vmt dir_node_vmt = {
  .addref           = node_dir_addref,
  .release          = node_dir_release,
  .node_stat        = node_dir_getstat,
  .dir_first        = node_dir_first,
  .dir_next         = node_dir_next
};

static void *node_file_addref(void *instance);
static void node_file_release(void *instance);
static msg_t node_file_getstat(void *instance, vfs_stat_t *sp);
static BaseSequentialStream *node_file_get_stream(void *instance);
static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n);
static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n);
static msg_t node_file_setpos(void *instance,
                              vfs_offset_t offset,
                              vfs_seekmode_t whence);
static vfs_offset_t node_file_getpos(void *instance);

static const struct vfs_streams_file_node_vmt file_node_vmt = {
  .addref           = node_file_addref,
  .release          = node_file_release,
  .node_stat        = node_file_getstat,
  .file_get_stream  = node_file_get_stream,
  .file_read        = node_file_read,
  .file_write       = node_file_write,
  .file_setpos      = node_file_setpos,
  .file_getpos      = node_file_getpos
};

/**
 * @brief   Static members of @p vfs_streams_driver_c.
 */
static struct {
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
  vfs_streams_dir_node_c             dir_nodes[DRV_CFG_STREAMS_DIR_NODES_NUM];
  /**
   * @brief   Static storage of file nodes.
   */
  vfs_streams_file_node_c            file_nodes[DRV_CFG_STREAMS_FILE_NODES_NUM];
} vfs_streams_driver_static;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t drv_set_cwd(void *instance, const char *path) {

  (void)instance;

  if (strcmp(path, "/") != 0) {
    return CH_RET_ENOENT;
  }

  return CH_RET_SUCCESS;
}

static msg_t drv_get_cwd(void *instance, char *buf, size_t size) {

  (void)instance;

  if (size < 2) {
    return CH_RET_ERANGE;
  }

  buf[0] = '/';
  buf[1] = '\0';

  return CH_RET_SUCCESS;
}

static msg_t drv_stat(void *instance, const char *path, vfs_stat_t *sp) {
  msg_t ret;

  (void)instance;

  do {
    ret = vfs_parse_match_separator(&path);
    CH_BREAK_ON_ERROR(ret);

    ret = vfs_parse_match_end(&path);
    CH_BREAK_ON_ERROR(ret);

    sp->mode = VFS_MODE_S_IFDIR;
    sp->size = (vfs_offset_t)0;

    ret = CH_RET_SUCCESS;
  }
  while (false);

  return ret;
}

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp) {
  vfs_streams_driver_c *drvp = (vfs_streams_driver_c *)instance;
  msg_t ret;

  do {
    vfs_streams_dir_node_c *sdnp;

    ret = vfs_parse_match_separator(&path);
    CH_BREAK_ON_ERROR(ret);

    ret = vfs_parse_match_end(&path);
    CH_BREAK_ON_ERROR(ret);

    sdnp = chPoolAlloc(&vfs_streams_driver_static.dir_nodes_pool);
    if (sdnp != NULL) {

      /* Node object initialization.*/
      __referenced_object_objinit_impl(sdnp, &dir_node_vmt);
      sdnp->driver  = (vfs_driver_c *)drvp;
      sdnp->mode    = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR;
      sdnp->index   = 0U;

      *vdnpp = (vfs_directory_node_c *)sdnp;
      return CH_RET_SUCCESS;
    }

    ret = CH_RET_ENOMEM;
  }
  while (false);

  return ret;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           int flags,
                           vfs_file_node_c **vfnpp) {
  vfs_streams_driver_c *drvp = (vfs_streams_driver_c *)instance;
  const drv_streams_element_t *dsep;
  msg_t ret;

  (void)flags; /* TODO  handle invalid modes.*/

  do {
    char fname[VFS_CFG_NAMELEN_MAX + 1];

    ret = vfs_parse_match_separator(&path);
    CH_BREAK_ON_ERROR(ret);

    ret = (msg_t)path_get_element(&path, fname, VFS_CFG_NAMELEN_MAX + 1);
    CH_BREAK_ON_ERROR(ret);

    /* Null element.*/
    if (ret == (msg_t)0) {
      /* Trying to open the root as a file.*/
      ret = CH_RET_EISDIR;
      break;
    }

    ret = vfs_parse_match_end(&path);
    CH_BREAK_ON_ERROR(ret);

    dsep = &drvp->streams[0];
    while (dsep->name != NULL) {
      if (strncmp(fname, dsep->name, VFS_CFG_NAMELEN_MAX) == 0) {
        vfs_streams_file_node_c *sfnp;

        sfnp = chPoolAlloc(&vfs_streams_driver_static.file_nodes_pool);
        if (sfnp != NULL) {

          /* Node object initialization.*/
          __referenced_object_objinit_impl(sfnp, &file_node_vmt);
          sfnp->driver  = (vfs_driver_c *)drvp;
          sfnp->mode    = VFS_MODE_S_IFIFO | VFS_MODE_S_IRUSR | VFS_MODE_S_IWUSR;
          sfnp->stream  = dsep->stream;

          *vfnpp = (vfs_file_node_c *)sfnp;
          return CH_RET_SUCCESS;
        }

        return CH_RET_ENOMEM;
      }

      dsep++;
    }

    ret = CH_RET_ENOENT;
  }
  while (false);

  return ret;
}

static void *node_dir_addref(void *instance) {

  return __referenced_object_addref_impl(instance);
}

static void node_dir_release(void *instance) {
  vfs_streams_dir_node_c *sdnp = (vfs_streams_dir_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    chPoolFree(&vfs_streams_driver_static.dir_nodes_pool, (void *)sdnp);
  }
}

static msg_t node_dir_getstat(void *instance, vfs_stat_t *sp) {
  vfs_node_c *np = (vfs_node_c *)instance;

  sp->mode = np->mode;
  sp->size = (vfs_offset_t)0;

  return CH_RET_SUCCESS;
}

static msg_t node_dir_first(void *instance, vfs_direntry_info_t *dip) {
  vfs_streams_dir_node_c *sdnp = (vfs_streams_dir_node_c *)instance;

  sdnp->index = 0U;

  return node_dir_next(instance, dip);
}

static msg_t node_dir_next(void *instance, vfs_direntry_info_t *dip) {
  vfs_streams_dir_node_c *sdnp = (vfs_streams_dir_node_c *)instance;
  vfs_streams_driver_c *vsdp = (vfs_streams_driver_c *)sdnp->driver;

  if (vsdp->streams[sdnp->index].name != NULL) {

    dip->mode = VFS_MODE_S_IFIFO | VFS_MODE_S_IRUSR | VFS_MODE_S_IWUSR;
    dip->size = (vfs_offset_t)0;
    strcpy(dip->name, vsdp->streams[sdnp->index].name);

    sdnp->index++;

    return (msg_t)1;
  }

  return (msg_t)0;
}

static void *node_file_addref(void *instance) {

  return __referenced_object_addref_impl(instance);
}

static void node_file_release(void *instance) {
  vfs_streams_file_node_c *sfnp = (vfs_streams_file_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    chPoolFree(&vfs_streams_driver_static.file_nodes_pool, (void *)sfnp);
  }
}

static msg_t node_file_getstat(void *instance, vfs_stat_t *sp) {
  vfs_node_c *np = (vfs_node_c *)instance;

  sp->mode = np->mode;
  sp->size = (vfs_offset_t)0;

  return CH_RET_SUCCESS;
}

static BaseSequentialStream *node_file_get_stream(void *instance) {
  vfs_streams_file_node_c *sfnp = (vfs_streams_file_node_c *)instance;

  return sfnp->stream;
}

static ssize_t node_file_read(void *instance, uint8_t *buf, size_t n) {
  vfs_streams_file_node_c *sfnp = (vfs_streams_file_node_c *)instance;

  return streamRead(sfnp->stream, buf, n);
}

static ssize_t node_file_write(void *instance, const uint8_t *buf, size_t n) {
  vfs_streams_file_node_c *sfnp = (vfs_streams_file_node_c *)instance;

  return streamWrite(sfnp->stream, buf, n);
}

static msg_t node_file_setpos(void *instance,
                              vfs_offset_t offset,
                              vfs_seekmode_t whence) {

  (void)instance;
  (void)offset;
  (void)whence;

  return CH_RET_ENOSYS;
}

static vfs_offset_t node_file_getpos(void *instance) {

  (void)instance;

  return 0U;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Module initialization.
 *
 * @notapi
 */
void __drv_streams_init(void) {

  /* Initializing pools.*/
  chPoolObjectInit(&vfs_streams_driver_static.dir_nodes_pool,
                   sizeof (vfs_streams_dir_node_c),
                   chCoreAllocAlignedI);
  chPoolObjectInit(&vfs_streams_driver_static.file_nodes_pool,
                   sizeof (vfs_streams_file_node_c),
                   chCoreAllocAlignedI);

  /* Preloading pools.*/
  chPoolLoadArray(&vfs_streams_driver_static.dir_nodes_pool,
                  &vfs_streams_driver_static.dir_nodes[0],
                  DRV_CFG_STREAMS_DIR_NODES_NUM);
  chPoolLoadArray(&vfs_streams_driver_static.file_nodes_pool,
                  &vfs_streams_driver_static.file_nodes[0],
                  DRV_CFG_STREAMS_FILE_NODES_NUM);
}

/**
 * @brief   VFS streams object initialization.
 *
 * @param[out] vsdp     Pointer to a @p vfs_streams_driver_c structure.
 * @param[in] streams   Pointer to an array of @p drv_streams_element_t objects.
 * @return              A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_c *drvStreamsObjectInit(vfs_streams_driver_c *vsdp,
                                   const drv_streams_element_t *streams) {

  __base_object_objinit_impl(vsdp, &driver_vmt);
  vsdp->streams  = streams;

  return (vfs_driver_c *)vsdp;
}

#endif /* VFS_CFG_ENABLE_DRV_STREAMS == TRUE */

/** @} */
