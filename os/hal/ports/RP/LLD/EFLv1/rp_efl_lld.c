/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    EFLv1/rp_efl_lld.c
 * @brief   RP shared Embedded Flash subsystem low level driver source.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#include <string.h>

#include "hal.h"
#include "rp_efl_lld.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const flash_descriptor_t efl_lld_descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE |
                      FLASH_ATTR_MEMORY_MAPPED |
                      FLASH_ATTR_REWRITABLE,
  .page_size        = RP_FLASH_PAGE_SIZE,
  .sectors_count    = RP_FLASH_SECTORS_COUNT,
  .sectors          = NULL,
  .sectors_size     = RP_FLASH_SECTOR_SIZE,
  .address          = (uint8_t *)RP_FLASH_BASE,
  .size             = RP_FLASH_SIZE
};

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level Embedded Flash driver initialization.
 *
 * @notapi
 */
void efl_lld_init(void) {

  /* Driver initialization. */
  eflObjectInit(&EFLD1);

  /* Platform-specific one-time initialization (e.g. RP2040 boot2 copy). */
  rp_efl_lld_init();
}

/**
 * @brief   Configures and activates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_start(EFlashDriver *eflp) {

  rp_efl_lld_start(eflp);
}

/**
 * @brief   Deactivates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_stop(EFlashDriver *eflp) {

  (void)eflp;

  /* Nothing to do. */
}

/**
 * @brief   Gets the flash descriptor structure.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @return              A flash device descriptor.
 *
 * @notapi
 */
const flash_descriptor_t *efl_lld_get_descriptor(void *instance) {

  (void)instance;

  return &efl_lld_descriptor;
}

/**
 * @brief   Read operation.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] offset    offset within full flash address space
 * @param[in] n         number of bytes to be read
 * @param[out] rp       pointer to the data buffer
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_READ         if the read operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                           size_t n, uint8_t *rp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)efl_lld_descriptor.size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No reading while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_READ state while the operation is performed. */
  devp->state = FLASH_READ;

  /* Read from memory-mapped XIP region. */
  memcpy((void *)rp, (const void *)(efl_lld_descriptor.address + offset), n);

  /* Ready state again. */
  devp->state = FLASH_READY;

  return err;
}

/**
 * @brief   Program operation.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] offset    offset within full flash address space
 * @param[in] n         number of bytes to be programmed
 * @param[in] pp        pointer to the data buffer
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_PROGRAM      if the program operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                              size_t n, const uint8_t *pp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  syssts_t sts;

  osalDbgCheck((instance != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)efl_lld_descriptor.size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No programming while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_PGM state while the operation is performed. */
  devp->state = FLASH_PGM;

  /* Allow the application to prepare for XIP becoming unavailable. */
  rpEflBeforeXipOff();

  /* Program in page-sized chunks.  The source data copy is intentionally
   * done outside the system lock while XIP is still enabled; only the
   * RAM-resident page transaction itself is bracketed by syslock. */
  while (n > 0U) {
    uint8_t page_buf[RP_FLASH_PAGE_SIZE];
    uint32_t page_base = offset & ~(uint32_t)(RP_FLASH_PAGE_SIZE - 1U);
    size_t page_offset = offset & (RP_FLASH_PAGE_SIZE - 1U);
    size_t page_remaining = RP_FLASH_PAGE_SIZE - page_offset;
    size_t chunk = (n < page_remaining) ? n : page_remaining;

    /*
     * Programming is done page-by-page. Fill the untouched bytes with
     * 0xFF (all ones) so a partial write still emits a full page.
     */
    memset(page_buf, 0xFF, sizeof(page_buf));
    memcpy(page_buf + page_offset, pp, chunk);

    sts = osalSysGetStatusAndLockX();

    /* Program the page. */
    rp_efl_lld_program_page_full(devp, page_base, page_buf,
                                 RP_FLASH_PAGE_SIZE);

    osalSysRestoreStatusX(sts);

    offset += chunk;
    pp += chunk;
    n -= chunk;
  }

  /* Notify the application that XIP is available again. */
  rpEflAfterXipOn();

  /* Ready state again. */
  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Starts a whole-device erase operation.
 * @note    This is not implemented for safety reasons - erasing the entire
 *          flash would destroy the running firmware.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @return              An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_all(void *instance) {

  (void)instance;

  return FLASH_ERROR_UNIMPLEMENTED;
}

/**
 * @brief   Starts a sector erase operation.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] sector    sector to be erased
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_sector(void *instance,
                                         flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_offset_t offset;
  syssts_t sts;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < efl_lld_descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No erasing while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* FLASH_ERASE state while the operation is performed. */
  devp->state = FLASH_ERASE;

  /* Calculate sector offset. */
  offset = sector * RP_FLASH_SECTOR_SIZE;

  /* Allow the application to prepare for XIP becoming unavailable. */
  rpEflBeforeXipOff();

  /* Lock the system around the single RAM-resident erase sequence. */
  sts = osalSysGetStatusAndLockX();

  /* Perform the entire erase sequence in RAM. */
  rp_efl_lld_erase_full(devp, RP_FLASH_CMD_SECTOR_ERASE, offset);

  /* Restore system state. */
  osalSysRestoreStatusX(sts);

  /* Notify the application that XIP is available again. */
  rpEflAfterXipOn();

  /* Back to ready state. */
  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Starts a block erase operation.
 *
 * @param[in] instance    pointer to a @p EFlashDriver instance
 * @param[in] cmd         JEDEC erase command byte
 * @param[in] erase_size  erase unit size in bytes
 * @param[in] block       block number to be erased
 * @return                An error code.
 * @retval FLASH_NO_ERROR           if the block erase completed.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_block(void *instance,
                                        uint8_t cmd,
                                        uint32_t erase_size,
                                        uint32_t block) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_offset_t offset;
  syssts_t sts;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(block < (RP_FLASH_SIZE / erase_size));
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  devp->state = FLASH_ERASE;

  offset = block * erase_size;

  /* Allow the application to prepare for XIP becoming unavailable. */
  rpEflBeforeXipOff();

  /* Erase is one uninterrupted RAM-resident XIP-off transaction, so the
   * whole helper runs under syslock. */
  sts = osalSysGetStatusAndLockX();
  rp_efl_lld_erase_full(devp, cmd, offset);
  osalSysRestoreStatusX(sts);

  /* Notify the application that XIP is available again. */
  rpEflAfterXipOn();

  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Queries the driver for erase operation progress.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[out] msec     recommended time, in milliseconds, that
 *                      should be spent before calling this
 *                      function again, can be @p NULL
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_ERASE        if the erase operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_query_erase(void *instance, uint32_t *msec) {
  EFlashDriver *devp = (EFlashDriver *)instance;

  /* If there is an erase in progress then the device must be checked. */
  if (devp->state == FLASH_ERASE) {
    /*
     * Note: Our implementation waits for erase completion in
     * efl_lld_start_erase_sector(), so we should never actually
     * be in FLASH_ERASE state when this is called. However, we
     * handle it properly for interface compliance.
     */
    if (msec != NULL) {
      *msec = RP_FLASH_WAIT_TIME_MS;
    }
    return FLASH_BUSY_ERASING;
  }

  return FLASH_NO_ERROR;
}

/**
 * @brief   Returns the erase state of a sector.
 *
 * @param[in] instance  pointer to a @p EFlashDriver instance
 * @param[in] sector    sector to be verified
 * @return              An error code.
 * @retval FLASH_NO_ERROR           if the sector is erased.
 * @retval FLASH_BUSY_ERASING       if there is an erase operation in progress.
 * @retval FLASH_ERROR_VERIFY       if the verify operation failed.
 * @retval FLASH_ERROR_HW_FAILURE   if access to the memory failed.
 *
 * @notapi
 */
flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const uint32_t *address;
  flash_error_t err = FLASH_NO_ERROR;
  unsigned i;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < efl_lld_descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No verifying while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Address of the sector in XIP space. */
  address = (const uint32_t *)(efl_lld_descriptor.address +
                               flashGetSectorOffset(getBaseFlash(devp), sector));

  /* FLASH_READ state while the operation is performed. */
  devp->state = FLASH_READ;

  /* Scanning the sector space. */
  for (i = 0U; i < RP_FLASH_SECTOR_SIZE / sizeof(uint32_t); i++) {
    if (address[i] != 0xFFFFFFFFU) {
      err = FLASH_ERROR_VERIFY;
      break;
    }
  }

  /* Ready state again. */
  devp->state = FLASH_READY;

  return err;
}

/**
 * @brief   Reads the flash chip's unique ID.
 * @note    The JEDEC 0x4B command is issued at runtime, which requires
 *          exiting and re-entering XIP mode.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 * @param[out] uid      pointer to an 8-byte buffer for the unique ID
 *
 * @api
 */
void efl_lld_read_unique_id(EFlashDriver *eflp, uint8_t *uid) {
  uint8_t rx[4U + RP_FLASH_UNIQUE_ID_SIZE];
  syssts_t sts;

  osalDbgCheck((eflp != NULL) && (uid != NULL));

  /* Allow the application to prepare for XIP becoming unavailable. */
  rpEflBeforeXipOff();

  sts = osalSysGetStatusAndLockX();
  rp_efl_lld_read_uid_full(eflp, rx, sizeof(rx));
  osalSysRestoreStatusX(sts);

  /* Notify the application that XIP is available again. */
  rpEflAfterXipOn();

  memcpy(uid, rx + 4U, RP_FLASH_UNIQUE_ID_SIZE);
}

/**
 * @brief   Default RP EFL pre-XIP-off hook.
 * @details Weak no-op implementation applications may override.
 */
CC_WEAK void rpEflBeforeXipOff(void) {
}

/**
 * @brief   Default RP EFL post-XIP-on hook.
 * @details Weak no-op implementation applications may override.
 */
CC_WEAK void rpEflAfterXipOn(void) {
}

#endif /* HAL_USE_EFL == TRUE */

/** @} */
