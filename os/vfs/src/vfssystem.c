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
 * @file    vfs/src/chsystem.c
 * @brief   VFS system code.
 *
 * @addtogroup VFS_SYSTEM
 * @{
 */

#include <string.h>

#include "vfs.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define BREAK_ON_ERROR(err)                                                 \
  if ((err) < VFS_RET_SUCCESS) break

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Global VFS state.
 */
vfs_system_t vfs;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

vfserr_t vfs_match_separator(const char **pathp) {
  vfserr_t err;
  const char *p = *pathp;

  if (*p++ != '/') {
    err = VFS_RET_INVALID_PATH;
  }
  else {
    err = VFS_RET_SUCCESS;
    *pathp = p;
  }

  return err;
}


vfserr_t vfs_match_filename(const char **pathp, char *fname) {
  vfserr_t err;
  size_t n;
  const char *p = *pathp;

  n = 0U;
  while (true) {
    char c = *p++;
    if ((c == '\0') || (c == '/')) {
      *pathp = p;
      *fname = '\0';
      err = VFS_RET_SUCCESS;
      break;
    }

    if (n > VFS_CFG_MAX_NAMELEN) {
      err = VFS_RET_INVALID_PATH;
      break;
    }

    *fname++ = c;
    n++;
  }

  return err;
}

vfserr_t vfs_match_driver(const char **pathp, vfs_driver_t **vdpp) {
  char fname[VFS_CFG_MAX_NAMELEN + 1];
  vfserr_t err;
  vfs_driver_t **pp;

  do {
    err = vfs_match_separator(pathp);
    BREAK_ON_ERROR(err);

    err = vfs_match_filename(pathp, fname);
    BREAK_ON_ERROR(err);

    /* Searching among registered drivers.*/
    pp = &vfs.drivers[0];
    while (pp < vfs.next_driver) {
      if (strncmp(fname, (*pp)->vmt->get_name(), VFS_CFG_MAX_NAMELEN) == 0) {
        *vdpp = *pp;
        return VFS_RET_SUCCESS;
      }
    }

    err = VFS_RET_NO_DRIVER;
  }
  while (false);

  return err;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/


/**
 * @brief   VFS initialization.
 *
 * @special
 */
void vfsInit(void) {

  vfs.next_driver = &vfs.drivers[0];

#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  chMtxObjectInit(&vfs.mtx);
#else
  chSemObjectInit(&vfs.sem, (cnt_t)1);
#endif
}

/**
 * @brief   Registers a file system driver on VFS.
 *
 * @param[in] vdp       pointer to a @p vfs_driver_t structure
 * @return              The operation result.
 */
vfserr_t vfsRegisterDriver(vfs_driver_t *vdp) {
  vfserr_t err;

  VFS_LOCK();

  if (vfs.next_driver >= &vfs.drivers[VFS_CFG_MAX_DRIVERS]) {
    err = VFS_RET_NO_RESOURCE;
  }
  else {
    *vfs.next_driver++ = vdp;
    err = VFS_RET_SUCCESS;
  }

  VFS_UNLOCK();

  return err;
}

/**
 * @brief   Opens a VFS directory.
 *
 * @param[in] path      absolute path of the directory to be opened
 * @param[out] vdnpp    pointer to the pointer to the instantiated
 *                      @p vfs_directory_node_t object
 * @return              The operation result.
 */
vfserr_t vfsOpenDirectory(const char *path, vfs_directory_node_t **vdnpp) {
  vfserr_t err;
  vfs_driver_t *dp;

  do {
    err = vfs_match_driver(&path, &dp);
    BREAK_ON_ERROR(err);

    err = dp->vmt->open_dir(path, vdnpp);
  }
  while (false);

  return err;
}

/**
 * @brief   Releases a @p vfs_directory_node_t object.
 *
 * @param[in] vdnp      the pointer to the @p vfs_directory_node_t object
 *                      to be released
 */
void vfsCloseDirectory(vfs_directory_node_t *vdnp) {

  vdnp->vmt->release((void *)vdnp);
}

/**
 * @brief   Opens a VFS file.
 *
 * @param[in] path      absolute path of the file to be opened
 * @param[out] vdnpp    pointer to the pointer to the instantiated
 *                      @p vfs_file_node_t object
 * @return              The operation result.
 */
vfserr_t vfsOpenFile(const char *path, vfs_file_node_t **vfnpp) {
  vfserr_t err;
  vfs_driver_t *dp;

  do {
    err = vfs_match_driver(&path, &dp);
    BREAK_ON_ERROR(err);

    err = dp->vmt->open_file(path, vfnpp);
  }
  while (false);

  return err;
}

/**
 * @brief   Releases a @p vfs_file_node_t object.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 *                      to be released
 */
void vfsCloseFile(vfs_file_node_t *vfnp) {

  vfnp->vmt->release((void *)vfnp);
}

/**
 * @brief   File node read.
 * @details The function reads data from a file node into a buffer.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @param[out] buf      pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The transferred number of bytes or an error.
 *
 * @api
 */
vfserr_t vfsReadFile(vfs_file_node_t *vfnp, char *buf, size_t n) {
  vfserr_t err = VFS_RET_SUCCESS;

  (void)vfnp;
  (void)buf;
  (void)n;

  return err;
}

/**
 * @brief   File node write.
 * @details The function writes data from a buffer to a file node.
 *
 * @param[in] vfnp      the pointer to the @p vfs_file_node_t object
 * @param[out] buf      pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The transferred number of bytes or an error.
 *
 * @api
 */
vfserr_t vfsWriteFile(vfs_file_node_t *vfnp, const char *buf, size_t n) {
  vfserr_t err = VFS_RET_SUCCESS;

  (void)vfnp;
  (void)buf;
  (void)n;

  return err;
}

/** @} */
