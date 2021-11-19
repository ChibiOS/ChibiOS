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

#include "vfs.h"

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

/** @} */
