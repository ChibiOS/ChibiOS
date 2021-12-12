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
 * @file    vfs/drivers/drvoverlay.c
 * @brief   Overlays VFS driver code.
 *
 * @addtogroup VFS_DRV_OVERLAY
 * @{
 */

#include "vfs.h"

#if (VFS_CFG_ENABLE_DRV_OVERLAY == TRUE) || defined(__DOXYGEN__)

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

static const struct vfs_overlay_driver_vmt driver_vmt = {
  .open_dir     = drv_open_dir,
  .open_file    = drv_open_file
};

static void node_dir_release(void *instance);
static msg_t node_dir_first(void *instance, vfs_node_info_t *nip);
static msg_t node_dir_next(void *instance, vfs_node_info_t *nip);

static const struct vfs_overlay_dir_node_vmt dir_node_vmt = {
  .release      = node_dir_release,
  .dir_first    = node_dir_first,
  .dir_next     = node_dir_next
};

/**
 * @brief   Static members of @p vfs_overlay_driver_c.
 */
static struct {
  /**
   * @brief   Pool of directory nodes.
   */
  memory_pool_t                     dir_nodes_pool;
  /**
   * @brief   Static storage of directory nodes.
   */
  vfs_overlay_dir_node_c            dir_nodes[DRV_CFG_OVERLAY_DIR_NODES_NUM];
} vfs_overlay_driver_static;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t match_driver(vfs_overlay_driver_c *odp,
                          const char **pathp,
                          vfs_driver_c **vdpp) {
  char fname[VFS_CFG_NAMELEN_MAX + 1];
  msg_t err;
  vfs_driver_c **pp;

  do {
    err = vfs_parse_get_fname(pathp, fname);
    VFS_BREAK_ON_ERROR(err);

    /* Searching among registered drivers.*/
    pp = &odp->drivers[0];
    while (pp < &odp->drivers[odp->next_driver]) {
      if (strncmp(fname, (*pp)->rootname, VFS_CFG_NAMELEN_MAX) == 0) {
        *vdpp = *pp;
        return VFS_RET_SUCCESS;
      }

      pp++;
    }

    err = VFS_RET_ENOENT;
  }
  while (false);

  return err;
}

static msg_t build_path(vfs_overlay_driver_c *drvp,
                        const char *path,
                        char *buf) {

  *buf = '\0';

  /* Copying the prefix, if defined.*/
  if (drvp->path_prefix != NULL) {
    VFS_RETURN_ON_ERROR(vfs_path_append(buf, drvp->path_prefix));
  }

  VFS_RETURN_ON_ERROR(vfs_path_append(buf, path));

  return VFS_RET_SUCCESS;
}

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp) {
  vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
  const char *scanpath = path;
  msg_t err;

  do {
    /* Expecting an absolute path.*/
    err = vfs_parse_match_separator(&scanpath);
    VFS_BREAK_ON_ERROR(err);

    /* If it is the root.*/
    if (*scanpath == '\0') {

      /* Creating a root directory node.*/
      vfs_overlay_dir_node_c *odnp = chPoolAlloc(&vfs_overlay_driver_static.dir_nodes_pool);
      if (odnp != NULL) {

        /* Node object initialization.*/
        __referenced_object_objinit_impl(odnp, &dir_node_vmt);
        odnp->driver        = (vfs_driver_c *)instance;
        odnp->index         = 0U;
        odnp->overlaid_root = NULL;

        /* Trying to obtain a root node from the overlaid driver, it
           could fail, in that case the pointer stays at NULL.*/
        if (drvp->overlaid_drv != NULL) {
          (void) drvp->overlaid_drv->vmt->open_dir((void *)drvp->overlaid_drv,
                                                   drvp->path_prefix == NULL ? "/" : drvp->path_prefix,
                                                   &odnp->overlaid_root);
        }

        *vdnpp = (vfs_directory_node_c *)odnp;
        return VFS_RET_SUCCESS;
      }
    }
    else { /* Not the root.*/
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      err = match_driver(drvp, &scanpath, &dp);
      if (err == VFS_RET_SUCCESS) {
        /* Delegating node creation to a registered driver.*/
        err = dp->vmt->open_dir((void *)dp,
                                scanpath,
                                vdnpp);
      }
      else {
        /* Is there an overlaid driver? if so we need to pass request
           processing there.*/
        if (drvp->overlaid_drv != NULL) {
          char *buf;

          /* Taking a path buffer from the pool.*/
          buf = vfs_buffer_take();

          /* Building the final path for the overlaid driver.*/
          err = build_path(drvp, path, buf);

          /* Passing the combined path to the overlaid driver.*/
          if (err == VFS_RET_SUCCESS) {
            err = drvp->overlaid_drv->vmt->open_dir((void *)drvp->overlaid_drv,
                                                    buf,
                                                    vdnpp);
          }

          /* Buffer returned.*/
          vfs_buffer_release(buf);
        }
      }
    }
  }
  while (false);

  return err;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           int oflag,
                           vfs_file_node_c **vfnpp) {
  vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
  const char *scanpath = path;
  msg_t err;

  do {
    /* Expecting an absolute path.*/
    err = vfs_parse_match_separator(&scanpath);
    VFS_BREAK_ON_ERROR(err);

    if (*scanpath == '\0') {
      (void)instance;

      /* Always not found, root is not a file.*/
      err = VFS_RET_ENOENT;
    }
    else {
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      err = match_driver(drvp, &scanpath, &dp);
      if (err == VFS_RET_SUCCESS) {
        /* Delegating node creation to a registered driver.*/
        err = dp->vmt->open_file((void *)dp, scanpath, oflag, vfnpp);
      }
      else {
        /* Is there an overlaid driver? if so we need to pass request
           processing there.*/
        if (drvp->overlaid_drv != NULL) {
          char *buf;

          /* Taking a path buffer from the pool.*/
          buf = vfs_buffer_take();

          /* Building the final path for the overlaid driver.*/
          err = build_path(drvp, path, buf);

          /* Passing the combined path to the overlaid driver.*/
          if (err == VFS_RET_SUCCESS) {
            err = drvp->overlaid_drv->vmt->open_file((void *)drvp->overlaid_drv,
                                                     path,
                                                     oflag,
                                                     vfnpp);
          }

          /* Buffer returned.*/
          vfs_buffer_release(buf);
        }
      }
    }
  }
  while (false);

  return err;
}

static void node_dir_release(void *instance) {
  vfs_overlay_dir_node_c *odnp = (vfs_overlay_dir_node_c *)instance;

  __referenced_object_release_impl(instance);
  if (__referenced_object_getref_impl(instance) == 0U) {

    if (odnp->overlaid_root != NULL) {
      odnp->overlaid_root->vmt->release((void *)odnp->overlaid_root);
    }

    chPoolFree(&vfs_overlay_driver_static.dir_nodes_pool, (void *)odnp);
  }
}

static msg_t node_dir_first(void *instance, vfs_node_info_t *nip) {
  vfs_overlay_dir_node_c *odnp = (vfs_overlay_dir_node_c *)instance;

  odnp->index = 0U;

  return node_dir_next(instance, nip);
}

static msg_t node_dir_next(void *instance, vfs_node_info_t *nip) {
  vfs_overlay_dir_node_c *odnp = (vfs_overlay_dir_node_c *)instance;
  vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)odnp->driver;

  if (odnp->index < drvp->next_driver) {
    nip->attr   = VFS_NODE_ATTR_ISDIR | VFS_NODE_ATTR_READONLY;
    nip->size   = (vfs_offset_t)0;
    strcpy(nip->name, drvp->drivers[odnp->index]->rootname);

    odnp->index++;

    return VFS_RET_SUCCESS;
  }
  if (odnp->overlaid_root != NULL) {
    if (odnp->index == drvp->next_driver) {

      odnp->index++;

      return odnp->overlaid_root->vmt->dir_first((void *)odnp->overlaid_root,
                                                 nip);
    }
    if (odnp->index > drvp->next_driver) {

      return odnp->overlaid_root->vmt->dir_next((void *)odnp->overlaid_root,
                                                nip);
    }
  }

  return VFS_RET_EOF;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Module initialization.
 *
 * @notapi
 */
void __drv_overlay_init(void) {

  /* Initializing pools.*/
  chPoolObjectInit(&vfs_overlay_driver_static.dir_nodes_pool,
                   sizeof (vfs_overlay_dir_node_c),
                   chCoreAllocAlignedI);

  /* Preloading pools.*/
  chPoolLoadArray(&vfs_overlay_driver_static.dir_nodes_pool,
                  &vfs_overlay_driver_static.dir_nodes[0],
                  DRV_CFG_OVERLAY_DIR_NODES_NUM);
}

/**
 * @brief   VFS overlay object initialization.
 *
 * @param[out] vodp             pointer to a @p vfs_overlay_driver_c structure
 * @param[out] overlaid_drv     pointer to a driver to be overlaid
 * @param[out] path_prefix      prefix to be added to the paths or @p NULL
 * @param[in] rootname          name to be attributed to this object
 * @return                      A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_c *drvOverlayObjectInit(vfs_overlay_driver_c *vodp,
                                   vfs_driver_c *overlaid_drv,
                                   const char *path_prefix,
                                   const char *rootname) {

  __base_object_objinit_impl(vodp, &driver_vmt);
  vodp->rootname     = rootname;
  vodp->overlaid_drv = overlaid_drv;
  vodp->path_prefix  = path_prefix;
  vodp->next_driver  = 0U;

  return (vfs_driver_c *)vodp;
}

/**
 * @brief   Registers a VFS driver as an overlay.
 *
 * @param[in] vodp              pointer to a @p vfs_overlay_driver_c structure
 * @return                      The operation result.
 *
 * @api
 */
msg_t drvOverlayRegisterDriver(vfs_overlay_driver_c *vodp,
                               vfs_driver_c *vdp) {
  msg_t err;

  if (vodp->next_driver >= DRV_CFG_OVERLAY_DRV_MAX) {
    err = VFS_RET_ENOMEM;
  }
  else {
    vodp->drivers[vodp->next_driver++] = vdp;
    err = VFS_RET_SUCCESS;
  }

  return err;
}

#endif /* VFS_CFG_ENABLE_DRV_OVERLAY == TRUE */

/** @} */
