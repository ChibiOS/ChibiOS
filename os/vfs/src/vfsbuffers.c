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
 * @file    vfs/src/vfsbuffers.c
 * @brief   VFS shared path buffers code.
 *
 * @addtogroup VFS_BUFFERS
 * @{
 */

#include "vfs.h"

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

/**
 * @brief   VFS static data.
 */
static struct {
  /**
   * @brief   Guarded pool of path buffers.
   */
  guarded_memory_pool_t             path_buffers1_pool;
  /**
   * @brief   Normal pool of path buffers.
   */
  memory_pool_t                     path_buffers2_pool;
  /**
   * @brief   Shared path buffers.
   */
  char                              path_buffers1[VFS_CFG_PATHBUFS_NUM]
                                                 [VFS_BUFFERS_SIZE];
} vfs_buffers_static;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   VFS path buffers initialization.
 *
 * @init
 */
void __vfs_buffers_init(void) {

  chGuardedPoolObjectInit(&vfs_buffers_static.path_buffers1_pool,
                          VFS_BUFFERS_SIZE);
  chGuardedPoolLoadArray(&vfs_buffers_static.path_buffers1_pool,
                         &vfs_buffers_static.path_buffers1[0],
                         VFS_CFG_PATHBUFS_NUM);
  chPoolObjectInit(&vfs_buffers_static.path_buffers2_pool,
                   VFS_BUFFERS_SIZE,
                   chCoreAllocAlignedI);
}

/**
 * @brief   Claims a path buffer from the fixed pool, waiting if not available.
 *
 * @return                      Pointer to the taken buffer.
 */
char *vfs_buffer_take(void) {

  return (char *)chGuardedPoolAllocTimeout(&vfs_buffers_static.path_buffers1_pool,
                                           TIME_INFINITE);
}

/**
 * @brief   Releases a path buffer into the fixed pool.
 *
 * @param[in] buf               Buffer to be released.
 */
void vfs_buffer_release(char *buf) {

  chGuardedPoolFree(&vfs_buffers_static.path_buffers1_pool, (void *)buf);
}

/**
 * @brief   Allocates a path buffer from the dynamic pool.
 *
 * @return                      Pointer to the allocated buffer.
 * @retval NULL                 If the buffer allocation failed.
 */
char *vfs_buffer_alloc(void) {

  return (char *)chPoolAlloc(&vfs_buffers_static.path_buffers2_pool);
}

/**
 * @brief   Frees a path buffer into the dynamic pool.
 *
 * @param[in] buf               Buffer to be freed.
 */
void vfs_buffer_free(char *buf) {

  chPoolFree(&vfs_buffers_static.path_buffers2_pool, (void *)buf);
}

/** @} */
