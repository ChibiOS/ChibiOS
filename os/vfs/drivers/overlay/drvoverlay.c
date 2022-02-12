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

static msg_t drv_set_cwd(void *instance, const char *path);
static msg_t drv_get_cwd(void *instance, char *buf, size_t size);
static msg_t drv_stat(void *instance, const char *path, vfs_stat_t *sp);
static msg_t drv_open_dir(void *instance, const char *path,
                          vfs_directory_node_c **vdnpp);
static msg_t drv_open_file(void *instance, const char *path,
                           int flags, vfs_file_node_c **vfnpp);
static msg_t drv_unlink(void *instance, const char *path);
static msg_t drv_rename(void *instance,const char *oldpath,const char *newpath);
static msg_t drv_mkdir(void *instance,const char *path, vfs_mode_t mode);
static msg_t drv_rmdir(void *instance, const char *path);

static const struct vfs_overlay_driver_vmt driver_vmt = {
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
static msg_t node_dir_first(void *instance, vfs_direntry_info_t *dip);
static msg_t node_dir_next(void *instance, vfs_direntry_info_t *dip);

static const struct vfs_overlay_dir_node_vmt dir_node_vmt = {
  .addref           = node_dir_addref,
  .release          = node_dir_release,
  .dir_first        = node_dir_first,
  .dir_next         = node_dir_next
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
  unsigned i;

  i = 0U;
  while (i < odp->next_driver) {
    size_t n;

    n = path_match_element(*pathp, odp->names[i], VFS_CFG_NAMELEN_MAX + 1);
    if (n < VFS_CFG_NAMELEN_MAX + 1) {
      *pathp += n;
      *vdpp = odp->drivers[i];
      return CH_RET_SUCCESS;
    }

    i++;
  }

  return CH_RET_ENOENT;
}

static const char *get_current_directory(vfs_overlay_driver_c *drvp) {
  const char *cwd = drvp->path_cwd;

  /* In case it has not yet been defined using root.*/
  if (cwd == NULL) {
    return "/";
  }

  return cwd;
}

static msg_t build_absolute_path(vfs_overlay_driver_c *drvp,
                                 char *buf,
                                 const char *path) {
  msg_t ret;

  do {

    /* Initial buffer state, empty string.*/
    *buf = '\0';

    /* Relative paths handling.*/
    if (!path_is_separator(*path)) {
      if (path_append(buf,
                      get_current_directory(drvp),
                      VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
        ret = CH_RET_ENAMETOOLONG;
        break;
      }
    }

    /* Adding the specified path.*/
    if (path_append(buf, path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    /* Normalization of the absolute path.*/
    if (path_normalize(buf, buf, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    ret = CH_RET_SUCCESS;

  } while (false);

  return ret;
}

static msg_t open_absolute_dir(vfs_overlay_driver_c *drvp,
                               char *path,
                               vfs_directory_node_c **vdnpp) {
  msg_t ret;

  do {
    const char *scanpath;

    /* Making sure there is a final separator.*/
    if (path_add_separator(path, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
      ret = CH_RET_ENAMETOOLONG;
      break;
    }

    /* Initial separator is expected, skipping it.*/
    scanpath = path + 1;

    /* If it is the root.*/
    if (*scanpath == '\0') {
      vfs_overlay_dir_node_c *odnp;

      /* Creating a root directory node.*/
      odnp = chPoolAlloc(&vfs_overlay_driver_static.dir_nodes_pool);
      if (odnp != NULL) {

        /* Node object initialization.*/
        __referenced_object_objinit_impl(odnp, &dir_node_vmt);
        odnp->driver        = (vfs_driver_c *)drvp;
        odnp->mode          = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR;
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

        ret = CH_RET_SUCCESS;
        break;
      }
      else {
        ret = CH_RET_ENOMEM;
        break;
      }
    }
    else { /* Not the root.*/
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      ret = match_driver(drvp, &scanpath, &dp);
      if (!CH_RET_IS_ERROR(ret)) {
        /* Delegating node creation to a registered driver.*/
        ret = dp->vmt->open_dir((void *)dp,
                                *scanpath == '\0' ? "/" : scanpath,
                                vdnpp);
      }
      else {
        size_t path_offset;

        /* Is there an overlaid driver? if so we need to pass request
           processing there.*/
        if (drvp->overlaid_drv != NULL) {

          /* Processing the prefix, if defined.*/
          if (drvp->path_prefix != NULL) {
            if ((path_offset = path_prepend(path,
                                            drvp->path_prefix,
                                            VFS_CFG_PATHLEN_MAX + 1)) == (size_t)0) {
              ret = CH_RET_ENAMETOOLONG;
              break;
            }
          }
          else {
            path_offset = 0U;
          }

          /* Passing the combined path to the overlaid driver.*/
          ret = drvp->overlaid_drv->vmt->open_dir((void *)drvp->overlaid_drv,
                                                  path,
                                                  vdnpp);
          CH_BREAK_ON_ERROR(ret);

          ret = (msg_t)path_offset;
        }
      }
    }
  }
  while (false);

  return ret;
}

static msg_t open_absolute_file(vfs_overlay_driver_c *drvp,
                                char *path,
                                int oflag,
                                vfs_file_node_c **vfnpp) {
  msg_t ret;

  do {
    const char *scanpath;

    /* Initial separator is expected, skipping it.*/
    scanpath = path + 1;

    /* If it is the root.*/
    if (*scanpath == '\0') {

      /* Always not found, root is not a file.*/
      ret = CH_RET_EISDIR;
    }
    else {
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      ret = match_driver(drvp, &scanpath, &dp);
      if (!CH_RET_IS_ERROR(ret)) {
        /* Delegating node creation to a registered driver, making sure it
           does not receive an empty path.*/
        ret = dp->vmt->open_file((void *)dp,
                                 *scanpath == '\0' ? "/" : scanpath,
                                 oflag, vfnpp);
      }
      else {
        /* Is there an overlaid driver? if so we need to pass request
           processing there.*/
        if (drvp->overlaid_drv != NULL) {

          /* Processing the prefix, if defined.*/
          if (drvp->path_prefix != NULL) {
            if (path_prepend(path,
                             drvp->path_prefix,
                             VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
              ret = CH_RET_ENAMETOOLONG;
              break;
            }
          }

          /* Passing the combined path to the overlaid driver.*/
          ret = drvp->overlaid_drv->vmt->open_file((void *)drvp->overlaid_drv,
                                                   path,
                                                   oflag,
                                                   vfnpp);
        }
      }
    }
  }
  while (false);

  return ret;
}

static msg_t drv_set_cwd(void *instance, const char *path) {
  char *buf;
  msg_t ret;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
    vfs_directory_node_c *vdnp;
    size_t path_offset;

    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    /* Trying to access the directory in order to validate the
       combined path. Note, it can modify the path in the buffer.*/
    ret = open_absolute_dir(drvp, buf, &vdnp);
    CH_BREAK_ON_ERROR(ret);
    vdnp->vmt->release((void *)vdnp);
    path_offset = (size_t)ret;

    /* One-time allocation of the CWD buffer, this memory is allocated, once,
       only if the application uses a CWD, it is never released.*/
    if (drvp->path_cwd == NULL) {
      drvp->path_cwd = chCoreAlloc(VFS_CFG_PATHLEN_MAX + 1);
      if (drvp->path_cwd == NULL) {
        ret = CH_RET_ENOMEM;
        break;
      }
    }

    /* Copying the validated path into the CWD buffer.*/
    strcpy(drvp->path_cwd, buf + path_offset);

  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

static msg_t drv_get_cwd(void *instance, char *buf, size_t size) {
  vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;

  *buf = '\0';
  if (path_append(buf, get_current_directory(drvp), size) == (size_t)0) {
    return CH_RET_ERANGE;
  }

  return CH_RET_SUCCESS;
}

static msg_t drv_stat(void *instance, const char *path, vfs_stat_t *sp) {
  msg_t ret;
  char *buf;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
    const char *scanpath;

    /* Building the absolute path based on current directory.*/
    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    /* Skipping the root separator.*/
    scanpath = buf + 1;

    /* If it is not root checking among mounted drivers.*/
    if (*scanpath != '\0') {
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      ret = match_driver(drvp, &scanpath, &dp);
      if (!CH_RET_IS_ERROR(ret)) {
        /* Delegating directory creation to a registered driver.*/
        ret = dp->vmt->stat((void *)dp, scanpath, sp);
        break;
      }
    }

    /* Is there an overlaid driver? if so we need to pass request
       processing there.*/
    if (drvp->overlaid_drv != NULL) {

      /* Processing the prefix, if defined.*/
      if (drvp->path_prefix != NULL) {
        if (path_prepend(buf,
                         drvp->path_prefix,
                         VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
          ret = CH_RET_ENAMETOOLONG;
          break;
        }
      }

      /* Passing the combined path to the overlaid driver.*/
      ret = drvp->overlaid_drv->vmt->stat((void *)drvp->overlaid_drv, buf, sp);
    }
    else {
      ret = CH_RET_ENOENT;
    }
  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

static msg_t drv_open_dir(void *instance,
                          const char *path,
                          vfs_directory_node_c **vdnpp) {
  msg_t ret;
  char *buf;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;

    /* Building the absolute path based on current directory.*/
    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    ret = open_absolute_dir(drvp, buf, vdnpp);
    CH_BREAK_ON_ERROR(ret);

    /* Required because the offset returned by open_absolute_dir().*/
    ret = CH_RET_SUCCESS;
  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

static msg_t drv_open_file(void *instance,
                           const char *path,
                           int flags,
                           vfs_file_node_c **vfnpp) {
  msg_t ret;
  char *buf;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;

    /* Building the absolute path based on current directory.*/
    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    ret = open_absolute_file(drvp, buf, flags, vfnpp);
  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

static msg_t drv_overlaid_path_call(vfs_overlay_driver_c *drvp,
                                    char *buf,
                                    msg_t (*fn)(void *ip, const char *path)) {
  msg_t ret;

  do {
    /* Is there an overlaid driver? if so we need to pass request
       processing there.*/
    if (drvp->overlaid_drv != NULL) {

      /* Processing the prefix, if defined.*/
      if (drvp->path_prefix != NULL) {
        if (path_prepend(buf,
                         drvp->path_prefix,
                         VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
          ret = CH_RET_ENAMETOOLONG;
          break;
        }
      }

      /* Passing the combined path to the overlaid driver.*/
      ret = fn((void *)drvp->overlaid_drv, buf);
    }
    else {
      ret = CH_RET_ENOENT;
    }
  } while (false);

  return ret;
}

msg_t drv_unlink(void *instance, const char *path) {
  msg_t ret;
  char *buf;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
    const char *scanpath;

    /* Building the absolute path based on current directory.*/
    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    /* Skipping the root separator.*/
    scanpath = buf + 1;

    /* If it is the root.*/
    if (*scanpath == '\0') {
      ret = CH_RET_EISDIR;
    }
    else { /* Not the root.*/
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      ret = match_driver(drvp, &scanpath, &dp);
      if (!CH_RET_IS_ERROR(ret)) {
        /* Delegating file deletion to a registered driver.*/
        ret = dp->vmt->unlink((void *)dp, scanpath);
      }
      else {
        /* Passing the request to the overlaid driver, if any.*/
        ret = drv_overlaid_path_call(drvp, buf, drvp->overlaid_drv->vmt->unlink);
      }
    }
  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

msg_t drv_rename(void *instance, const char *oldpath, const char *newpath) {
  msg_t ret;
  char *oldbuf, *newbuf;

  /* Taking a path buffer from the pool.*/
  oldbuf = vfs_buffer_take();
  newbuf = vfs_buffer_alloc();
  if (newbuf == NULL) {
    vfs_buffer_release(oldbuf);
    return CH_RET_ENOMEM;
  }

  do {
    msg_t oldret, newret;
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
    vfs_driver_c *olddp, *newdp;
    const char *op, *np;

    /* Building the absolute paths based on current directory.*/
    ret = build_absolute_path(drvp, oldbuf, oldpath);
    CH_BREAK_ON_ERROR(ret);
    ret = build_absolute_path(drvp, newbuf, newpath);
    CH_BREAK_ON_ERROR(ret);

    /* Skipping root separators.*/
    op = oldbuf + 1;
    np = newbuf + 1;

    /* Searching for a match among registered drivers.*/
    oldret = match_driver(drvp, &op, &olddp);
    newret = match_driver(drvp, &np, &newdp);

    /* There are various combinations to consider.*/
    if (!CH_RET_IS_ERROR(oldret) && !CH_RET_IS_ERROR(newret)) {
       /* If paths both refer to registered drivers then must refer to
          the same driver, we cannot do a rename across drivers.*/
      if (olddp == newdp) {
        /* Delegating node renaming to the registered driver.*/
        ret = olddp->vmt->rename((void *)olddp, op, np);
      }
      else {
        /* Mixed, not allowing it.*/
        ret = CH_RET_EXDEV;
      }
    }
    else if (CH_RET_IS_ERROR(oldret) && CH_RET_IS_ERROR(newret)) {
      /* If both paths refer to the overlaid driver then passing down the
         request.*/
      if (drvp->overlaid_drv != NULL) {

        /* Processing the prefix, if defined.*/
        if (drvp->path_prefix != NULL) {
          if (path_prepend(oldbuf,
                           drvp->path_prefix,
                           VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
            ret = CH_RET_ENAMETOOLONG;
            break;
          }
          if (path_prepend(newbuf,
                           drvp->path_prefix,
                           VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
            ret = CH_RET_ENAMETOOLONG;
            break;
          }
        }

        /* Passing the combined path to the overlaid driver.*/
        ret = drvp->overlaid_drv->vmt->rename((void *)drvp->overlaid_drv,
                                              oldbuf, newbuf);
      }
    }
    else {
      /* Mixed, not allowing it.*/
      ret = CH_RET_EXDEV;
    }
  } while (false);

  /* Buffers returned, note, in reverse order.*/
  vfs_buffer_free(newbuf);
  vfs_buffer_release(oldbuf);

  return ret;
}

msg_t drv_mkdir(void *instance, const char *path, vfs_mode_t mode) {
  msg_t ret;
  char *buf;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
    const char *scanpath;

    /* Building the absolute path based on current directory.*/
    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    /* Skipping the root separator.*/
    scanpath = buf + 1;

    /* If it is the root.*/
    if (*scanpath == '\0') {
      ret = CH_RET_EEXIST;
    }
    else { /* Not the root.*/
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      ret = match_driver(drvp, &scanpath, &dp);
      if (!CH_RET_IS_ERROR(ret)) {
        /* Delegating directory creation to a registered driver.*/
        ret = dp->vmt->mkdir((void *)dp, scanpath, mode);
      }
      else {
        /* Is there an overlaid driver? if so we need to pass request
           processing there.*/
        if (drvp->overlaid_drv != NULL) {

          /* Processing the prefix, if defined.*/
          if (drvp->path_prefix != NULL) {
            if (path_prepend(buf,
                             drvp->path_prefix,
                             VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) {
              ret = CH_RET_ENAMETOOLONG;
              break;
            }
          }

          /* Passing the combined path to the overlaid driver.*/
          ret = drvp->overlaid_drv->vmt->mkdir((void *)drvp->overlaid_drv, buf, mode);
        }
        else {
          ret = CH_RET_ENOENT;
        }
      }
    }
  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

msg_t drv_rmdir(void *instance, const char *path) {
  msg_t ret;
  char *buf;

  /* Taking a path buffer from the pool.*/
  buf = vfs_buffer_take();

  do {
    vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)instance;
    const char *scanpath;

    /* Building the absolute path based on current directory.*/
    ret = build_absolute_path(drvp, buf, path);
    CH_BREAK_ON_ERROR(ret);

    /* Skipping the root separator.*/
    scanpath = buf + 1;

    /* If it is the root.*/
    if (*scanpath == '\0') {
      ret = CH_RET_EACCES;
    }
    else { /* Not the root.*/
      vfs_driver_c *dp;

      /* Searching for a match among registered overlays.*/
      ret = match_driver(drvp, &scanpath, &dp);
      if (!CH_RET_IS_ERROR(ret)) {
        /* Delegating directory deletion to a registered driver.*/
        ret = dp->vmt->rmdir((void *)dp, scanpath);
      }
      else {
        /* Passing the request to the overlaid driver, if any.*/
        ret = drv_overlaid_path_call(drvp, buf, drvp->overlaid_drv->vmt->rmdir);
      }
    }
  } while (false);

  /* Buffer returned.*/
  vfs_buffer_release(buf);

  return ret;
}

static void *node_dir_addref(void *instance) {

  return __referenced_object_addref_impl(instance);
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

static msg_t node_dir_first(void *instance, vfs_direntry_info_t *dip) {
  vfs_overlay_dir_node_c *odnp = (vfs_overlay_dir_node_c *)instance;

  odnp->index = 0U;

  return node_dir_next(instance, dip);
}

static msg_t node_dir_next(void *instance, vfs_direntry_info_t *dip) {
  vfs_overlay_dir_node_c *odnp = (vfs_overlay_dir_node_c *)instance;
  vfs_overlay_driver_c *drvp = (vfs_overlay_driver_c *)odnp->driver;

  if (odnp->index < drvp->next_driver) {
    dip->mode   = VFS_MODE_S_IFDIR | VFS_MODE_S_IRUSR;
    dip->size   = (vfs_offset_t)0;
    strcpy(dip->name, drvp->names[odnp->index]);

    odnp->index++;

    return (msg_t)1;
  }
  if (odnp->overlaid_root != NULL) {
    if (odnp->index == drvp->next_driver) {

      odnp->index++;

      return odnp->overlaid_root->vmt->dir_first((void *)odnp->overlaid_root,
                                                 dip);
    }
    if (odnp->index > drvp->next_driver) {

      return odnp->overlaid_root->vmt->dir_next((void *)odnp->overlaid_root,
                                                dip);
    }
  }

  return (msg_t)0;
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
 * @param[out] vodp             Pointer to a @p vfs_overlay_driver_c structure.
 * @param[in] overlaid_drv      Pointer to a driver to be overlaid or @p NULL.
 * @param[in] path_prefix       Prefix to be added to the paths or @p NULL, it
 *                              must be a normalized absolute path.
 * @return                      A pointer to this initialized object.
 *
 * @api
 */
vfs_driver_c *drvOverlayObjectInit(vfs_overlay_driver_c *vodp,
                                   vfs_driver_c *overlaid_drv,
                                   const char *path_prefix) {

  __base_object_objinit_impl(vodp, &driver_vmt);
  vodp->overlaid_drv = overlaid_drv;
  vodp->path_prefix  = path_prefix;
  vodp->path_cwd     = NULL;
  vodp->next_driver  = 0U;

  return (vfs_driver_c *)vodp;
}

/**
 * @brief   Registers a VFS driver as an overlay.
 *
 * @param[in] vodp              Pointer to a @p vfs_overlay_driver_c structure.
 * @param[in] vdp               Pointer to a @p vfs_driver_c structure to
 *                              be registered.
 * @param[in] name              Name to be associated to the registered driver.
 * @return                      The operation result.
 *
 * @api
 */
msg_t drvOverlayRegisterDriver(vfs_overlay_driver_c *vodp,
                               vfs_driver_c *vdp,
                               const char *name) {
  msg_t ret;

  if (vodp->next_driver >= DRV_CFG_OVERLAY_DRV_MAX) {
    ret = CH_RET_ENOMEM;
  }
  else {
    vodp->names[vodp->next_driver]   = name;
    vodp->drivers[vodp->next_driver] = vdp;
    vodp->next_driver++;
    ret = CH_RET_SUCCESS;
  }

  return ret;
}

#endif /* VFS_CFG_ENABLE_DRV_OVERLAY == TRUE */

/** @} */
