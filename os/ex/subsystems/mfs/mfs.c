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

#define MFS_BANK_MAGIC1                 0x35A1EC13
#define MFS_BANK_MAGIC2                 0x0FE14991
#define MFS_HEADER_MAGIC                0x1AC7002E

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (MFS_CFG_ID_CACHE_SIZE > 0) || defined(__DOXYGEN__)
void mfs_cache_init(MFSDriver *devp) {

  (void)devp;
}

mfs_cached_id_t *mfs_cache_find_id(MFSDriver *devp, uint32_t id) {

  (void)devp;
  (void)id;

  return NULL;
}

void mfs_cache_update_id(MFSDriver *devp, uint32_t id) {

  (void)devp;
  (void)id;
}

void mfs_cache_erase_id(MFSDriver *devp, uint32_t id) {

  (void)devp;
  (void)id;
}
#endif /* MFS_CFG_ID_CACHE_SIZE > 0 */

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
 * @param[in] devp      pointer to the @p MFSDriver object
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

/**
 * @brief   Mounts a managed flash storage.
 * @details This functions checks the storage internal state and eventually
 *          performs the required initialization or repair operations.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 *
 * @api
 */
mfs_error_t mfsMount(MFSDriver *devp) {

  (void)devp;

  return MFS_NOERROR;
}

/**
 * @brief   Unmounts a manage flash storage.
 */
mfs_error_t mfsUnmount(MFSDriver *devp) {

  (void)devp;

  return MFS_NOERROR;
}

/**
 * @brief   Retrieves and reads a data record.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier
 * @param[in,out] np    on input is the maximum buffer size, on return it is
 *                      the size of the data copied into the buffer
 * @param[in] buffer    pointer to a buffer for record data
 *
 * @api
 */
mfs_error_t mfsReadRecord(MFSDriver *devp, uint32_t id,
                          uint32_t *np, uint8_t *buffer) {

  (void)devp;
  (void)id;
  (void)np;
  (void)buffer;

  return MFS_NOERROR;
}

/**
 * @brief   Creates or updates a data record.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier
 * @param[in] n         size of data to be written, it cannot be zero
 * @param[in] buffer    pointer to a buffer for record data
 *
 * @api
 */
mfs_error_t mfsUpdateRecord(MFSDriver *devp, uint32_t id,
                            uint32_t n, const uint8_t *buffer) {

  (void)devp;
  (void)id;
  (void)n;
  (void)buffer;

  return MFS_NOERROR;
}

/**
 * @brief   Erases a data record.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier
 *
 * @api
 */
mfs_error_t mfsEraseRecord(MFSDriver *devp, uint32_t id) {

  (void)devp;
  (void)id;

  return MFS_NOERROR;
}

/** @} */
