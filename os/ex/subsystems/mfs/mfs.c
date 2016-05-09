/*
    Managed Flash Storage - Copyright (C) 2016 Giovanni Di Sirio

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    mfs.c
 * @brief   Managed Flash Storage module code.
 *
 * @addtogroup mfs
 * @{
 */

#include "hal.h"

#include "mfs.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p MFSDriver object
 *
 * @init
 */
void mfsObjectInit(MFSDriver *devp) {

  osalDbgCheck(devp != NULL);

  devp->state = MFS_STOP;
  devp->config = NULL;
}

/**
 * @brief   Configures and activates a MFS driver.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] config    pointer to the configuration
 *
 * @api
 */
void mfsStart(MFSDriver *devp, const MFSConfig *config) {

  osalDbgCheck((devp != NULL) && (config != NULL));
  osalDbgAssert(devp->state != MFS_UNINIT, "invalid state");

  if (devp->state == MFS_STOP) {

    devp->state = MFS_READY;
  }
} 

/**
 * @brief   Deactivates a MFS driver.
 *
 * @param[in] devp       pointer to the @p MFSDriver object
 *
 * @api
 */
void mfsStop(MFSDriver *devp) {

  osalDbgCheck(devp != NULL);
  osalDbgAssert(devp->state != MFS_UNINIT, "invalid state");

  if (devp->state != MFS_STOP) {

    devp->state = MFS_STOP;
  }
}

/** @} */
