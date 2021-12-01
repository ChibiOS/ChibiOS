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
 * @brief   HAL Overlay VFS driver code.
 *
 * @addtogroup VFS_DRV_OVERLAY
 * @{
 */

#include <string.h>

#include "vfs.h"
#include "drvoverlay.h"

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

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t match_driver(vfs_overlay_driver_t *odp,
                          const char **pathp,
                          vfs_driver_t **vdpp) {
  char fname[VFS_CFG_MAX_NAMELEN + 1];
  msg_t err;
  vfs_driver_t **pp;

  do {
    err = vfs_parse_filename(pathp, fname);
    VFS_BREAK_ON_ERROR(err);

    /* Searching among registered drivers.*/
    pp = &odp->drivers[0];
    while (pp < odp->next_driver) {
      if (strncmp(fname, (*pp)->rootname, VFS_CFG_MAX_NAMELEN) == 0) {
        *vdpp = *pp;
        return VFS_RET_SUCCESS;
      }

      pp++;
    }

    err = VFS_RET_NO_DRIVER;
  }
  while (false);

  return err;
}

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_t **vdnpp) {
  vfs_overlay_driver_t *drvp = (vfs_overlay_driver_t *)instance;
  const char *scanpath = path;
  msg_t err;

  do {
    err = vfs_parse_match_separator(&scanpath);
    VFS_BREAK_ON_ERROR(err);

    if (*scanpath == '\0') {

      /* Creating a root directory node.*/
      vfs_overlay_dir_node_t *onp = chPoolAlloc(&drvp->dir_nodes_pool);
      if (onp != NULL) {

        /* Node object initialization.*/
        onp->vmt    = &dir_node_vmt;
        onp->refs   = 1U;
        onp->driver = (vfs_driver_t *)instance;
        onp->index  = 0U;

        *vdnpp = (vfs_directory_node_t *)onp;
        return VFS_RET_SUCCESS;
      }
    }
    else {
      vfs_driver_t *dp;

      /* Searching for a match among registered overlays.*/
      err = match_driver(drvp, &scanpath, &dp);
      if (err == VFS_RET_SUCCESS) {
        /* Delegating node creation to a registered driver.*/
        err = dp->vmt->open_dir((void *)dp,
                                scanpath,
                                vdnpp);
      }
      else {
        /* No matching overlay, the whole path is passed to the overlaid
           driver, if defined, else returning the previous error.*/
        if (drvp->overlaid_drv != NULL) {
          err = drvp->overlaid_drv->vmt->open_dir((void *)drvp->overlaid_drv,
                                                  path,
                                                  vdnpp);
        }
      }
    }
  }
  while (false);

  return err;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           unsigned mode,
                           vfs_file_node_t **vfnpp) {
  vfs_overlay_driver_t *drvp = (vfs_overlay_driver_t *)instance;
  msg_t err;

  do {
    err = vfs_parse_match_separator(&path);
    VFS_BREAK_ON_ERROR(err);

    if (*path == '\0') {
      (void)instance;

      /* Always not found, there are no files in the root.*/
      err = VFS_RET_NOT_FOUND;
    }
    else {
      vfs_driver_t *dp;

      /* Delegating node creation to a registered driver.*/
      err = match_driver(drvp, &path, &dp);
      VFS_BREAK_ON_ERROR(err);

      err = dp->vmt->open_file((void *)dp, path, mode, vfnpp);
    }
  }
  while (false);

  return err;
}

static void node_dir_release(void *instance) {
  vfs_overlay_dir_node_t *odnp = (vfs_overlay_dir_node_t *)instance;
  vfs_overlay_driver_t *drvp = (vfs_overlay_driver_t *)odnp->driver;

  if (--odnp->refs == 0U) {

    chPoolFree(&drvp->dir_nodes_pool, (void *)odnp);
  }
}

static msg_t node_dir_first(void *instance, vfs_node_info_t *nip) {
  vfs_overlay_dir_node_t *odnp = (vfs_overlay_dir_node_t *)instance;

  odnp->index = 0U;

  return node_dir_next(instance, nip);
}

static msg_t node_dir_next(void *instance, vfs_node_info_t *nip) {
  vfs_overlay_dir_node_t *odnp = (vfs_overlay_dir_node_t *)instance;
  vfs_overlay_driver_t *drvp = (vfs_overlay_driver_t *)odnp->driver;

  if (odnp->index < DRV_CFG_OVERLAY_DRV_MAX) {
    nip->attr   = VFS_NODE_ATTR_ISDIR | VFS_NODE_ATTR_READONLY;
    nip->size   = (vfs_offset_t)0;
    strcpy(nip->name, drvp->drivers[odnp->index]->rootname);

    odnp->index++;

    return VFS_RET_SUCCESS;
  }

  return VFS_RET_EOF;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   VFS overlay object initialization.
 *
 * @param[out] vodp             pointer to a @p vfs_overlay_driver_t structure
 * @param[out] overlaid_drv     pointer to a driver to be overlaid
 * @param[in] rootname          name to be attributed to this object
 * @return                      A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_t *drvOverlayObjectInit(vfs_overlay_driver_t *vodp,
                                   vfs_driver_t *overlaid_drv,
                                   const char *rootname) {

  vodp->vmt          = &driver_vmt;
  vodp->rootname     = rootname;
  vodp->overlaid_drv = overlaid_drv;
  vodp->next_driver  = &vodp->drivers[0];

  /* Initializing pools.*/
  chPoolObjectInit(&vodp->dir_nodes_pool,
                   sizeof (vfs_overlay_dir_node_t),
                   chCoreAllocAlignedI);

  /* Preloading pools.*/
  chPoolLoadArray(&vodp->dir_nodes_pool,
                  &vodp->dir_nodes[0],
                  DRV_CFG_OVERLAY_DIR_NODES_NUM);

  return (vfs_driver_t *)vodp;
}

/**
 * @brief   Registers a VFS driver as an overlay.
 *
 * @param[in] vodp      pointer to a @p vfs_overlay_driver_t structure
 * @return              The operation result.
 *
 * @api
 */
msg_t drvOverlayRegisterDriver(vfs_overlay_driver_t *vodp,
                               vfs_driver_t *vdp) {
  msg_t err;

  if (vodp->next_driver >= &vodp->drivers[DRV_CFG_OVERLAY_DRV_MAX]) {
    err = VFS_RET_NO_RESOURCE;
  }
  else {
    *vodp->next_driver++ = vdp;
    err = VFS_RET_SUCCESS;
  }

  return err;
}

/** @} */
