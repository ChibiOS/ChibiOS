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
 * @file    vfs/include/vfssystem.h
 * @brief   VFS system header file.
 *
 * @addtogroup VFS_SYSTEM
 * @{
 */

#ifndef VFSSYSTEM_H
#define VFSSYSTEM_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  msg_t vfsOpenDirectory(vfs_driver_t *vdp,
                         const char *name,
                         vfs_directory_node_t **vdnpp);
  void vfsCloseDirectory(vfs_directory_node_t *vdnp);
  msg_t vfsReadDirectoryFirst(vfs_directory_node_t *vdnp,
                              vfs_node_info_t *nip);
  msg_t vfsReadDirectoryNext(vfs_directory_node_t *vdnp,
                             vfs_node_info_t *nip);
  msg_t vfsOpenFile(vfs_driver_t *vdp,
                    const char *name,
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

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VFSSYSTEM_H */

/** @} */
