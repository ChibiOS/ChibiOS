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
 * @file    vfs/include/vfs.h
 * @brief   VFS header file.
 * @details Main header of the ChibiOS Virtual File System.
 *
 * @addtogroup VFS
 * @{
 */

#ifndef VFS_H
#define VFS_H

/**
 * @brief   ChibiOS/VFS identification macro.
 */
#define __CHIBIOS_VFS__

/**
 * @brief   Stable release flag.
 */
#define CH_VFS_STABLE           0

/**
 * @name    ChibiOS/VFS version identification
 * @{
 */
/**
 * @brief   Kernel version string.
 */
#define CH_VFS_VERSION          "1.0.0"

/**
 * @brief   Kernel version major number.
 */
#define CH_VFS_MAJOR            1

/**
 * @brief   Kernel version minor number.
 */
#define CH_VFS_MINOR            0

/**
 * @brief   Kernel version patch number.
 */
#define CH_VFS_PATCH            0
/** @} */

#include <unistd.h>

/* Dependencies.*/
#include "osal.h"
#include "hal_objects.h"
#include "hal_streams.h"

/* License.*/
#include "chlicense.h"

/* Configuration headers, checks and licensing restrictions.*/
#include "vfsconf.h"
#include "vfschecks.h"

/* Base VFS headers.*/
#include "vfserrors.h"
#include "vfsparser.h"
#include "vfsnodes.h"
#include "vfsdrivers.h"

/* Application code is suppored to export this symbol, it is expected to
   exists.*/
extern vfs_driver_t *vfs_root;

#ifdef __cplusplus
extern "C" {
#endif
  msg_t vfsOpenDirectory(const char *name,
                         vfs_directory_node_t **vdnpp);
  void vfsCloseDirectory(vfs_directory_node_t *vdnp);
  msg_t vfsReadDirectoryFirst(vfs_directory_node_t *vdnp,
                              vfs_node_info_t *nip);
  msg_t vfsReadDirectoryNext(vfs_directory_node_t *vdnp,
                             vfs_node_info_t *nip);
  msg_t vfsOpenFile(const char *name,
                    unsigned mode,
                    vfs_file_node_t **vfnpp);
  void vfsCloseFile(vfs_file_node_t *vfnp);
  ssize_t vfsReadFile(vfs_file_node_t *vfnp, uint8_t *buf, size_t n);
  ssize_t vfsWriteFile(vfs_file_node_t *vfnp, const uint8_t *buf, size_t n);
  msg_t vfsSetFilePosition(vfs_file_node_t *vfnp, vfs_offset_t offset);
  vfs_offset_t vfsGetFilePosition(vfs_file_node_t *vfnp);
  vfs_offset_t vfsGetFileSize(vfs_file_node_t *vfnp);
  BaseSequentialStream *vfsGetFileStream(vfs_file_node_t *vfnp);
#ifdef __cplusplus
}
#endif

#endif /* VFS_H */

/** @} */
