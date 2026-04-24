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
 * @file        hal_flash_base.c
 * @brief       Generated Flash Base Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_FLASH_BASE
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
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

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "hal_flash_base_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of hal_flash_base_c
 * @{
 */
/**
 * @brief       Implementation of interface method @p flsGetDescriptor().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      A flash device descriptor.
 */
static const flash_descriptor_t *__flash_fls_get_descriptor_impl(void *ip) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);

  return &self->descriptor;
}

/**
 * @brief       Implementation of interface method @p flsRead().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_read_impl(void *ip, flash_offset_t offset,
                                           size_t n, uint8_t *rp) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);
  flash_error_t err;

  osalDbgCheck((self != NULL) && (rp != NULL) && (n > 0U));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  self->state = FLASH_READ;
  err = self->vmt->read(self, offset, n, rp);
  self->state = HAL_DRV_STATE_READY;

  return err;
}

/**
 * @brief       Implementation of interface method @p flsProgram().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_program_impl(void *ip, flash_offset_t offset,
                                              size_t n, const uint8_t *pp) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);
  flash_error_t err;

  osalDbgCheck((self != NULL) && (pp != NULL) && (n > 0U));
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  self->state = FLASH_PGM;
  err = self->vmt->program(self, offset, n, pp);
  self->state = HAL_DRV_STATE_READY;

  return err;
}

/**
 * @brief       Implementation of interface method @p flsStartEraseAll().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_start_erase_all_impl(void *ip) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);
  flash_error_t err;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  self->state = FLASH_ERASE;
  err = self->vmt->start_erase_all(self);
  if (err != FLASH_NO_ERROR) {
    self->state = HAL_DRV_STATE_READY;
  }

  return err;
}

/**
 * @brief       Implementation of interface method @p flsStartEraseSector().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_start_erase_sector_impl(void *ip,
                                                         flash_sector_t sector) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);
  flash_error_t err;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  self->state = FLASH_ERASE;
  err = self->vmt->start_erase_sector(self, sector);
  if (err != FLASH_NO_ERROR) {
    self->state = HAL_DRV_STATE_READY;
  }

  return err;
}

/**
 * @brief       Implementation of interface method @p flsQueryErase().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[out]    msec          Recommended time, in milliseconds, before
 *                              retrying, can be @p NULL.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_query_erase_impl(void *ip, unsigned *msec) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);
  flash_error_t err;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    err = self->vmt->query_erase(self, msec);
    if (err == FLASH_NO_ERROR) {
      self->state = HAL_DRV_STATE_READY;
    }
  }
  else {
    err = FLASH_NO_ERROR;
  }

  return err;
}

/**
 * @brief       Implementation of interface method @p flsVerifyErase().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_verify_erase_impl(void *ip,
                                                   flash_sector_t sector) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);
  flash_error_t err;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  self->state = FLASH_READ;
  err = self->vmt->verify_erase(self, sector);
  self->state = HAL_DRV_STATE_READY;

  return err;
}

/**
 * @brief       Implementation of interface method @p flsAcquireExclusive().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_acquire_exclusive_impl(void *ip) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);

  osalDbgCheck(self != NULL);
#if HAL_USE_MUTUAL_EXCLUSION == TRUE
  drvLock(self);
#endif

  return FLASH_NO_ERROR;
}

/**
 * @brief       Implementation of interface method @p flsReleaseExclusive().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      An error code.
 */
static flash_error_t __flash_fls_release_exclusive_impl(void *ip) {
  hal_flash_base_c *self = oopIfGetOwner(hal_flash_base_c, ip);

  osalDbgCheck(self != NULL);
#if HAL_USE_MUTUAL_EXCLUSION == TRUE
  drvUnlock(self);
#endif

  return FLASH_NO_ERROR;
}
/** @} */

/**
 * @name        Methods implementations of hal_flash_base_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_flash_base_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__flash_objinit_impl(void *ip, const void *vmt) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __drv_objinit_impl(self, vmt);

  /* Initialization of interface flash_interface_i.*/
  {
    static const struct flash_interface_vmt flash_fls_vmt = {
      .instance_offset      = offsetof(hal_flash_base_c, fls),
      .get_descriptor       = __flash_fls_get_descriptor_impl,
      .read                 = __flash_fls_read_impl,
      .program              = __flash_fls_program_impl,
      .start_erase_all      = __flash_fls_start_erase_all_impl,
      .start_erase_sector   = __flash_fls_start_erase_sector_impl,
      .query_erase          = __flash_fls_query_erase_impl,
      .verify_erase         = __flash_fls_verify_erase_impl,
      .acquire_exclusive    = __flash_fls_acquire_exclusive_impl,
      .release_exclusive    = __flash_fls_release_exclusive_impl
    };
    oopIfObjectInit(&self->fls, &flash_fls_vmt);
  }

  /* Initialization code.*/
  self->descriptor.attributes  = 0U;
  self->descriptor.page_size   = 0U;
  self->descriptor.sectors_count = 0U;
  self->descriptor.sectors     = NULL;
  self->descriptor.sectors_size = 0U;
  self->descriptor.address     = NULL;
  self->descriptor.size        = 0U;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance to be
 *                              disposed.
 */
void __flash_dispose_impl(void *ip) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;

  /* Finalization code.*/
  self->state = HAL_DRV_STATE_UNINIT;

  /* Finalization of the ancestors-defined parts.*/
  __drv_dispose_impl(self);
}
/** @} */

/**
 * @name        Regular methods of hal_flash_base_c
 * @{
 */
/**
 * @brief       Waits until the current erase operation is finished.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @return                      An error code.
 */
flash_error_t flashWaitErase(void *ip) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;
  while (true) {
    flash_error_t err;
    unsigned msec;

    err = flsQueryErase(&self->fls, &msec);
    if (err != FLASH_BUSY_ERASING) {
      return err;
    }

    osalThreadSleepMilliseconds(msec);
  }
}

/**
 * @brief       Returns the offset of a sector.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     sector        Flash sector number.
 * @return                      Sector offset.
 */
flash_offset_t flashGetSectorOffset(void *ip, flash_sector_t sector) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;
  flash_offset_t offset;
  const flash_descriptor_t *descriptor = &self->descriptor;

  osalDbgAssert(sector < descriptor->sectors_count, "invalid sector");

  if (descriptor->sectors != NULL) {
    offset = descriptor->sectors[sector].offset;
  }
  else {
    offset = (flash_offset_t)sector * (flash_offset_t)descriptor->sectors_size;
  }

  return offset;
}

/**
 * @brief       Returns the size of a sector.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     sector        Flash sector number.
 * @return                      Sector size.
 */
uint32_t flashGetSectorSize(void *ip, flash_sector_t sector) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;
  uint32_t size;
  const flash_descriptor_t *descriptor = &self->descriptor;

  osalDbgAssert(sector < descriptor->sectors_count, "invalid sector");

  if (descriptor->sectors != NULL) {
    size = descriptor->sectors[sector].size;
  }
  else {
    size = descriptor->sectors_size;
  }

  return size;
}

/**
 * @brief       Returns the sector containing an offset.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     offset        Flash offset.
 * @return                      Flash sector number.
 */
flash_sector_t flashGetOffsetSector(void *ip, flash_offset_t offset) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;
  flash_sector_t sector, i;
  const flash_descriptor_t *descriptor = &self->descriptor;

  osalDbgAssert(offset < descriptor->size, "invalid offset");

  if (descriptor->sectors != NULL) {
    flash_offset_t sector_start;
    flash_offset_t sector_end;

    for (i = 0; i < descriptor->sectors_count; i++) {
      sector_start = descriptor->sectors[i].offset;
      sector_end = sector_start + descriptor->sectors[i].size - 1U;
      if ((offset >= sector_start) && (offset <= sector_end)) {
        return i;
      }
    }
  }
  else {
    sector = offset / descriptor->sectors_size;
    return sector;
  }

  osalDbgAssert(false, "invalid offset");

  return 0U;
}

/**
 * @brief       Gets an absolute address from a flash offset.
 * @note        Meaningful only for memory mapped devices.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     offset        Flash offset.
 * @return                      A pointer to the offset address.
 */
void *flashGetOffsetAddress(void *ip, flash_offset_t offset) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;
  const flash_descriptor_t *descriptor = &self->descriptor;

  osalDbgAssert(offset < descriptor->size, "invalid offset");

  return (void *)(descriptor->address + offset);
}

/**
 * @brief       Gets a flash offset from an absolute address.
 * @note        Meaningful only for memory mapped devices.
 *
 * @param[in,out] ip            Pointer to a @p hal_flash_base_c instance.
 * @param[in]     addr          Absolute address.
 * @return                      Flash offset.
 */
flash_offset_t flashGetAddressOffset(void *ip, void *addr) {
  hal_flash_base_c *self = (hal_flash_base_c *)ip;
  const flash_descriptor_t *descriptor = &self->descriptor;
  uintptr_t addr_value = (uintptr_t)addr;
  uintptr_t base_value = (uintptr_t)descriptor->address;

  osalDbgAssert((addr_value >= base_value) &&
                (addr_value < (base_value + (uintptr_t)descriptor->size)),
                "invalid address");

  return (flash_offset_t)(addr_value - base_value);
}
/** @} */

/** @} */
