/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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
 * @file        hal_snor_base.c
 * @brief       Generated SNOR Base Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SNOR_BASE
 * @{
 */

#include "hal.h"
#include "hal_snor_base.h"

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
/* Module class "hal_snor_base_c" methods.                                   */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of hal_snor_base_c
 * @{
 */
/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashGetDescriptor().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      A flash device descriptor.
 */
static const flash_descriptor_t *__snorbase_flash_get_descriptor_impl(void *ip) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);

  return snor_get_descriptor(self);
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashRead().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_read_impl(void *ip,
                                                flash_offset_t offset,
                                                size_t n, uint8_t *rp) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);
  flash_error_t err;

  osalDbgCheck((self != NULL) && (rp != NULL) && (n > 0U));
  //osalDbgCheck((size_t)offset + n <= (size_t)snor_descriptor.sectors_count *
  //                                   (size_t)snor_descriptor.sectors_size);
  osalDbgAssert((self->state == FLASH_READY) || (self->state == FLASH_ERASE),
                "invalid state");

  if (self->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  wspiBusAcquire(self->wspi);

  /* FLASH_READY state while the operation is performed.*/
  self->state = FLASH_READ;

  /* Actual read implementation.*/
  //err = snor_device_read(devp, offset, n, rp);

  /* Ready state again.*/
  self->state = FLASH_READY;

  /* Bus released.*/
  wspiBusRelease(self->wspi);

  return err;
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashProgram().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_program_impl(void *ip,
                                                   flash_offset_t offset,
                                                   size_t n, const uint8_t *pp) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);
  ;
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashStartEraseAll().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_start_erase_all_impl(void *ip) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);
  ;
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashStartEraseSector().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_start_erase_sector_impl(void *ip,
                                                              const flash_sector_t *sector) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);
  ;
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashQueryErase().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @param[out]    msec          Recommended time, in milliseconds, that should
 *                              be spent before calling this function again,
 *                              can be @p NULL
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_query_erase_impl(void *ip,
                                                       unsigned *msec) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);
  ;
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashAcquireExclusive().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_acquire_exclusive_impl(void *ip) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);

  osalMutexLock(&self->mutex);
  return FLASH_NO_ERROR;
}

/**
 * @memberof    hal_snor_base_c
 * @private
 *
 * @brief       Implementation of interface method @p flashReleaseExclusive().
 *
 * @param[in,out] ip            Pointer to the @p flash_interface_i class
 *                              interface.
 * @return                      An error code.
 */
static flash_error_t __snorbase_flash_release_exclusive_impl(void *ip) {
  hal_snor_base_c *self = oopIfGetOwner(hal_snor_base_c, ip);

  osalMutexUnlock(&self->mutex);
  return FLASH_NO_ERROR;
}
/** @} */

/**
 * @name        Methods implementations of hal_snor_base_c
 * @{
 */
/**
 * @memberof    hal_snor_base_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_snor_base_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     nocache       Pointer to a non-cacheable buffer.
 * @return                      A new reference to the object.
 */
void *__snorbase_objinit_impl(void *ip, const void *vmt,
                              snor_nocache_t *nocache) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __bo_objinit_impl(self, vmt);

  /* Initialization of interface flash_interface_i.*/
  {
    static const struct flash_interface_vmt snorbase_flash_vmt = {
      .instance_offset      = offsetof(hal_snor_base_c, flash),
      .get_descriptor       = __snorbase_flash_get_descriptor_impl,
      .read                 = __snorbase_flash_read_impl,
      .program              = __snorbase_flash_program_impl,
      .start_erase_all      = __snorbase_flash_start_erase_all_impl,
      .start_erase_sector   = __snorbase_flash_start_erase_sector_impl,
      .query_erase          = __snorbase_flash_query_erase_impl,
      .acquire_exclusive    = __snorbase_flash_acquire_exclusive_impl,
      .release_exclusive    = __snorbase_flash_release_exclusive_impl
    };
    oopIfObjectInit(&self->flash, &snorbase_flash_vmt);
  }

  /* Initialization code.*/
  self->nocache = nocache;
  self->state = FLASH_UNINIT;
  osalMtxObjectInit(&self->mutex);

  return self;
}

/**
 * @memberof    hal_snor_base_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_snor_base_c instance to be
 *                              disposed.
 */
void __snorbase_dispose_impl(void *ip) {
  hal_snor_base_c *self = (hal_snor_base_c *)ip;

  /* Finalization code.*/
  ;

  /* Finalization of the ancestors-defined parts.*/
  __bo_dispose_impl(self);
}
/** @} */

/** @} */
