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
 * @details This module manages a flash partition as a generic storage where
 *          arbitrary data records can be created, updated, deleted and
 *          retrieved.<br>
 *          A managed partition is composed of two banks of equal size, a
 *          bank is composed of one or more erasable sectors, a sector is
 *          divided in writable pages.<br>
 *          The module handles flash wear leveling and recovery of damaged
 *          banks (where possible) caused by power loss during operations.
 *          Both operations are transparent to the user.
 *
 * @addtogroup mfs
 * @{
 */

#include "hal.h"

#include "mfs.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define PAIR(a, b) (((unsigned)(a) << 2U) | (unsigned)(b))

/**
 * @brief   Error check helper.
 */
#define RET_ON_ERROR(err) do {                                              \
  mfs_error_t e = (err);                                                    \
  if (e != MFS_NO_ERROR) {                                                  \
    return e;                                                               \
  }                                                                         \
} while (false)

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

/**
 * @brief   Erases and verifies all sectors belonging to a bank.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be erased
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_erase(MFSDriver *devp, mfs_bank_t bank) {
  flash_sector_t sector, end;

  if (bank == MFS_BANK_0) {
    sector = devp->config->bank0_start;
    end    = devp->config->bank0_start + devp->config->bank0_sectors;
  }
  else {
    sector = devp->config->bank1_start;
    end    = devp->config->bank1_start + devp->config->bank1_sectors;
  }

  while (sector < end) {
    flash_error_t ferr;

    ferr = flashStartEraseSector(devp->config->flashp, sector);
    if (ferr != FLASH_NO_ERROR) {
      return MFS_FLASH_FAILURE;
    }
    ferr = flashWaitErase(devp->config->flashp);
    if (ferr != FLASH_NO_ERROR) {
      return MFS_FLASH_FAILURE;
    }
    ferr = flashVerifyErase(devp->config->flashp, sector);
    if (ferr != FLASH_NO_ERROR) {
      return MFS_FLASH_FAILURE;
    }

    sector++;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Writes the validation header in a bank.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be validated
 * @param[in] cnt       value for the flash usage counter
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @notapi
 */
static mfs_error_t mfs_validate(MFSDriver *devp,
                                mfs_bank_t bank,
                                uint32_t cnt) {

  (void)devp;
  (void)bank;
  (void)cnt;

  return MFS_NO_ERROR;
}

/**
 * @brief   Copies all records from a bank to another.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] sbank     source bank
 * @param[in] dbank     destination bank
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @notapi
 */
static mfs_error_t mfs_copy_bank(MFSDriver *devp,
                                mfs_bank_t sbank,
                                mfs_bank_t dbank) {

  (void)devp;
  (void)sbank;
  (void)dbank;

  return MFS_NO_ERROR;
}

/**
 * @brief   Selects a bank as current.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be erased
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 *
 * @notapi
 */
static mfs_error_t mfs_mount(MFSDriver *devp, mfs_bank_t bank) {

  (void)devp;
  (void)bank;

  return MFS_NO_ERROR;
}

/**
 * @brief   Determines the state of a flash bank.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] bank          the bank identifier
 * @param[out] cntp         bank counter value, only valid if the bank is not
 *                          in the @p MFS_BANK_GARBAGE or @p MFS_BANK_ERASED
 *                          states.
 *
 * @return                  The bank state.
 * @retval MFS_BANK_ERASED  if the bank is fully erased.
 * @retval MFS_BANK_OK      if the bank contains valid data.
 * @retval MFS_BANK_PARTIAL if the bank contains errors but the data is still
 *                          readable.
 * @retval MFS_BANK_GARBAGE if the bank contains unreadable garbage.
 */
static mfs_bank_state_t mfs_get_bank_state(MFSDriver *devp,
                                           mfs_bank_t bank,
                                           uint32_t *cntp) {

  (void)devp;
  (void)bank;
  (void)cntp;

  return MFS_BANK_OK;
}

/**
 * @brief   Performs a flash partition mount attempt.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_REPAIR_WARNING if the operation has been completed but a
 *                      repair has been performed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @api
 */
static mfs_error_t mfs_try_mount(MFSDriver *devp) {
  mfs_bank_state_t sts0, sts1;
  uint32_t cnt0 = 0, cnt1 = 0;

  /* Assessing the state of the two banks.*/
  sts0 = mfs_get_bank_state(devp, MFS_BANK_0, &cnt0);
  sts1 = mfs_get_bank_state(devp, MFS_BANK_1, &cnt1);

  /* Handling all possible scenarios, each one requires its own recovery
     strategy.*/
  switch (PAIR(sts0, sts1)) {

  case PAIR(MFS_BANK_ERASED, MFS_BANK_ERASED):
    /* Both banks erased, first initialization.*/
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, 1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_NO_ERROR;

  case PAIR(MFS_BANK_ERASED, MFS_BANK_OK):
    /* Normal situation, bank one is used.*/
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    return MFS_NO_ERROR;

  case PAIR(MFS_BANK_ERASED, MFS_BANK_PARTIAL):
    /* Bank zero is erased, bank one has problems.*/
    RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_1, MFS_BANK_0));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, cnt1 + 1));
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_ERASED, MFS_BANK_GARBAGE):
    /* Bank zero is erased, bank one is not readable.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, 1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_OK, MFS_BANK_ERASED):
    /* Normal situation, bank zero is used.*/
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_NO_ERROR;

  case PAIR(MFS_BANK_OK, MFS_BANK_OK):
    /* Both banks appear to be valid but one must be newer, erasing the
       older one.*/
    if (cnt0 > cnt1) {
      /* Bank 0 is newer.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    }
    else {
      /* Bank 1 is newer.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    }
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_OK, MFS_BANK_PARTIAL):
    /* Bank zero is normal, bank one has problems.*/
    if (cnt0 > cnt1) {
      /* Normal bank zero is more recent than the partial bank one, the
         partial bank needs to be erased.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    }
    else {
      /* Partial bank one is more recent than the normal bank zero.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
      RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_1, MFS_BANK_0));
      RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, cnt1 + 1));
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    }
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_OK, MFS_BANK_GARBAGE):
    /* Bank zero is normal, bank one is unreadable.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_PARTIAL, MFS_BANK_ERASED):
    /* Bank zero has problems, bank one is erased.*/
    RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_0, MFS_BANK_1));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_1, cnt0 + 1));
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_PARTIAL, MFS_BANK_OK):
    /* Bank zero has problems, bank one is normal.*/
    if (cnt1 > cnt0) {
      /* Normal bank one is more recent than the partial bank zero, the
         partial bank has to be erased.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    }
    else {
      /* Partial bank zero is more recent than the normal bank one.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
      RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_0, MFS_BANK_1));
      RET_ON_ERROR(mfs_validate(devp, MFS_BANK_1, cnt0 + 1));
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    }
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_PARTIAL, MFS_BANK_PARTIAL):
    /* Both banks have problems.*/
    if (cnt0 > cnt1) {
      /* Bank zero is newer, copying in bank one and using it.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
      RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_0, MFS_BANK_1));
      RET_ON_ERROR(mfs_validate(devp, MFS_BANK_1, cnt0 + 1));
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    }
    else {
      /* Bank one is newer, copying in bank zero and using it.*/
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
      RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_1, MFS_BANK_0));
      RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, cnt1 + 1));
      RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
      RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    }
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_PARTIAL, MFS_BANK_GARBAGE):
    /* Bank zero has problems, bank one is unreadable.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_0, MFS_BANK_1));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_1, cnt0 + 1));
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_ERASED):
    /* Bank zero is unreadable, bank one is erased.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, 1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_OK):
    /* Bank zero is unreadable, bank one is normal.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_1));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_PARTIAL):
    /* Bank zero is unreadable, bank one has problems.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
    RET_ON_ERROR(mfs_copy_bank(devp, MFS_BANK_1, MFS_BANK_0));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, cnt0 + 1));
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_REPAIR_WARNING;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_GARBAGE):
    /* Both banks are unreadable, reinitializing.*/
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_0));
    RET_ON_ERROR(mfs_bank_erase(devp, MFS_BANK_1));
    RET_ON_ERROR(mfs_validate(devp, MFS_BANK_0, 1));
    RET_ON_ERROR(mfs_mount(devp, MFS_BANK_0));
    return MFS_REPAIR_WARNING;

  default:
    osalSysHalt("internal error");
  }

  /* Never reached.*/
  return MFS_INTERNAL_ERROR;
}

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

    devp->config = config;
    devp->state  = MFS_READY;
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
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_REPAIR_WARNING if the operation has been completed but a
 *                      repair has been performed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @api
 */
mfs_error_t mfsMount(MFSDriver *devp) {
  unsigned i;

  /* Attempting to mount the managed partition.*/
  for (i = 0; i < MFS_MAX_REPAIR_ATTEMPTS; i++) {
    mfs_error_t err;

    err = mfs_try_mount(devp);
    if (!MFS_IS_ERROR(err))
      return err;
  }

  return MFS_FLASH_FAILURE;
}

/**
 * @brief   Unmounts a manage flash storage.
 */
mfs_error_t mfsUnmount(MFSDriver *devp) {

  (void)devp;

  return MFS_NO_ERROR;
}

/**
 * @brief   Retrieves and reads a data record.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier
 * @param[in,out] np    on input is the maximum buffer size, on return it is
 *                      the size of the data copied into the buffer
 * @param[in] buffer    pointer to a buffer for record data
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_ID_NOT_FOUND if the specified id does not exists.
 * @retval MFS_CRC_ERROR if retrieved data has a CRC error.
 *
 * @api
 */
mfs_error_t mfsReadRecord(MFSDriver *devp, uint32_t id,
                          uint32_t *np, uint8_t *buffer) {

  (void)devp;
  (void)id;
  (void)np;
  (void)buffer;

  return MFS_NO_ERROR;
}

/**
 * @brief   Creates or updates a data record.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier
 * @param[in] n         size of data to be written, it cannot be zero
 * @param[in] buffer    pointer to a buffer for record data
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @api
 */
mfs_error_t mfsUpdateRecord(MFSDriver *devp, uint32_t id,
                            uint32_t n, const uint8_t *buffer) {

  (void)devp;
  (void)id;
  (void)n;
  (void)buffer;

  return MFS_NO_ERROR;
}

/**
 * @brief   Erases a data record.
 *
 * @param[in] devp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures.
 *
 * @api
 */
mfs_error_t mfsEraseRecord(MFSDriver *devp, uint32_t id) {

  (void)devp;
  (void)id;

  return MFS_NO_ERROR;
}

/** @} */
