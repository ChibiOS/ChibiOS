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
 * @file    hal_efl_lld.c
 * @brief   STM32G4xx Embedded Flash subsystem low level driver source.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#include <string.h>

#include "hal.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define STM32_FLASH_LINE_MASK               (STM32_FLASH_LINE_SIZE - 1U)

#define FLASH_KEY1                          0x45670123U
#define FLASH_KEY2                          0xCDEF89ABU

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EFL1 driver identifier.
 */
hal_efl_driver_c EFLD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const flash_descriptor_t efl_lld_desc = {
  .attributes    = FLASH_ATTR_ERASED_IS_ONE |
                   FLASH_ATTR_MEMORY_MAPPED |
                   FLASH_ATTR_ECC_CAPABLE   |
                   FLASH_ATTR_ECC_ZERO_LINE_CAPABLE,
  .page_size     = STM32_FLASH_LINE_SIZE,
  .sectors_count = STM32_FLASH_SECTORS_PER_BANK * STM32_FLASH_NUMBER_OF_BANKS,
  .sectors       = NULL,
  .sectors_size  = STM32_FLASH_SECTOR_SIZE,
  .address       = (uint8_t *)FLASH_BASE,
  .size          = STM32_FLASH_NUMBER_OF_BANKS *
                   STM32_FLASH_SECTORS_PER_BANK *
                   STM32_FLASH_SECTOR_SIZE
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void stm32_flash_lock(hal_efl_driver_c *self) {

  self->flash->CR |= FLASH_CR_LOCK;
}

static inline void stm32_flash_unlock(hal_efl_driver_c *self) {

  self->flash->KEYR = FLASH_KEY1;
  self->flash->KEYR = FLASH_KEY2;
}

static inline void stm32_flash_enable_pgm(hal_efl_driver_c *self) {

  self->flash->CR |= FLASH_CR_PG;
}

static inline void stm32_flash_disable_pgm(hal_efl_driver_c *self) {

  self->flash->CR &= ~FLASH_CR_PG;
}

static inline void stm32_flash_clear_status(hal_efl_driver_c *self) {

  self->flash->SR = 0x0000FFFFU;
}

static inline void stm32_flash_wait_busy(hal_efl_driver_c *self) {

  while ((self->flash->SR & FLASH_SR_BSY) != 0U) {
  }
}

static inline bool stm32_flash_dual_bank(hal_efl_driver_c *self) {

#if STM32_FLASH_NUMBER_OF_BANKS > 1
  return ((self->flash->OPTR & FLASH_OPTR_DBANK) != 0U);
#else
  (void)self;
  return false;
#endif
}

static inline flash_error_t stm32_flash_check_errors(hal_efl_driver_c *self) {
  uint32_t sr = self->flash->SR;

  self->flash->SR = sr & 0x0000FFFFU;

  osalDbgAssert((sr & (FLASH_SR_FASTERR |
                       FLASH_SR_MISERR |
                       FLASH_SR_SIZERR)) == 0U, "unexpected flash error");

  if ((sr & FLASH_SR_WRPERR) != 0U) {
    return FLASH_ERROR_HW_FAILURE;
  }

  if ((sr & (FLASH_SR_PGAERR | FLASH_SR_PROGERR | FLASH_SR_OPERR)) != 0U) {
    return self->state == FLASH_PGM ? FLASH_ERROR_PROGRAM : FLASH_ERROR_ERASE;
  }

  return FLASH_NO_ERROR;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level Embedded Flash driver initialization.
 *
 * @notapi
 */
void efl_lld_init(void) {

  eflObjectInit(&EFLD1);
  EFLD1.flash      = FLASH;
  EFLD1.descriptor = efl_lld_desc;
}

/**
 * @brief   Configures and activates the Embedded Flash peripheral.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @return                      The operation status.
 *
 * @notapi
 */
msg_t efl_lld_start(hal_efl_driver_c *self) {

  stm32_flash_unlock(self);
  self->flash->CR = 0x00000000U;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the Embedded Flash peripheral.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 *
 * @notapi
 */
void efl_lld_stop(hal_efl_driver_c *self) {

  stm32_flash_lock(self);
}

/**
 * @brief   Read operation.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @param[in]     offset        Offset within the flash address space.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_read(hal_efl_driver_c *self, flash_offset_t offset,
                           size_t n, uint8_t *rp) {
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((self != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)self->descriptor.size);
  osalDbgAssert(self->state == FLASH_READ, "invalid state");

  stm32_flash_clear_status(self);
  memcpy((void *)rp, (const void *)(self->descriptor.address + offset), n);

  if ((self->flash->SR & FLASH_SR_RDERR) != 0U) {
    err = FLASH_ERROR_READ;
  }

  return err;
}

/**
 * @brief   Program operation.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @param[in]     offset        Offset within the flash address space.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_program(hal_efl_driver_c *self, flash_offset_t offset,
                              size_t n, const uint8_t *pp) {
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((self != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)self->descriptor.size);
  osalDbgAssert(self->state == FLASH_PGM, "invalid state");

  stm32_flash_clear_status(self);
  stm32_flash_enable_pgm(self);

  while (n > 0U) {
    volatile uint32_t *address;
    uintptr_t address_value;
    union {
      uint32_t  w[STM32_FLASH_LINE_SIZE / sizeof(uint32_t)];
      uint8_t   b[STM32_FLASH_LINE_SIZE / sizeof(uint8_t)];
    } line;
    unsigned i;

    for (i = 0U; i < (unsigned)(STM32_FLASH_LINE_SIZE / sizeof(uint32_t)); i++) {
      line.w[i] = 0xFFFFFFFFU;
    }

    address_value = (uintptr_t)self->descriptor.address +
                    (uintptr_t)(offset & ~STM32_FLASH_LINE_MASK);
    osalDbgAssert((address_value & (uintptr_t)(sizeof(uint32_t) - 1U)) == 0U,
                  "unaligned flash line");
    address = (volatile uint32_t *)address_value;

    do {
      line.b[offset & STM32_FLASH_LINE_MASK] = *pp;
      offset++;
      n--;
      pp++;
    } while ((n > 0U) && ((offset & STM32_FLASH_LINE_MASK) != 0U));

    for (i = 0U; i < (unsigned)(STM32_FLASH_LINE_SIZE / sizeof(uint32_t)); i++) {
      address[i] = line.w[i];
    }

    stm32_flash_wait_busy(self);
    err = stm32_flash_check_errors(self);
    if (err != FLASH_NO_ERROR) {
      break;
    }
  }

  stm32_flash_disable_pgm(self);

  return err;
}

/**
 * @brief   Starts a whole-device erase operation.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @return                      An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_all(hal_efl_driver_c *self) {

  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == FLASH_ERASE, "invalid state");

#if defined(FLASH_CR_MER2)
  if (stm32_flash_dual_bank(self)) {
    stm32_flash_clear_status(self);
    self->flash->CR |= FLASH_CR_MER2;
    self->flash->CR |= FLASH_CR_STRT;
    return FLASH_NO_ERROR;
  }
#endif

  return FLASH_ERROR_UNIMPLEMENTED;
}

/**
 * @brief   Starts a sector erase operation.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_sector(hal_efl_driver_c *self,
                                         flash_sector_t sector) {

  osalDbgCheck(self != NULL);
  osalDbgCheck(sector < self->descriptor.sectors_count);
  osalDbgAssert(self->state == FLASH_ERASE, "invalid state");

  stm32_flash_clear_status(self);
  self->flash->CR |= FLASH_CR_PER;

#if defined(FLASH_CR_BKER)
  if (stm32_flash_dual_bank(self)) {
    if (sector < (self->descriptor.sectors_count / 2U)) {
      self->flash->CR &= ~FLASH_CR_BKER;
    }
    else {
      sector -= (self->descriptor.sectors_count / 2U);
      self->flash->CR |= FLASH_CR_BKER;
    }
  }
#endif

  self->flash->CR &= ~FLASH_CR_PNB;
  self->flash->CR |= sector << FLASH_CR_PNB_Pos;
  self->flash->CR |= FLASH_CR_STRT;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Queries the driver for erase operation progress.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @param[out]    msec          Recommended delay before polling again.
 * @return                      An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_query_erase(hal_efl_driver_c *self, unsigned *msec) {

  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == FLASH_ERASE, "invalid state");

  if ((self->flash->SR & FLASH_SR_BSY) == 0U) {
    self->flash->CR &= ~(FLASH_CR_MER1 |
#if defined(FLASH_CR_MER2)
                         FLASH_CR_MER2 |
#endif
                         FLASH_CR_PER);
    return stm32_flash_check_errors(self);
  }

  if (msec != NULL) {
    *msec = (unsigned)STM32_FLASH_WAIT_TIME_MS;
  }

  return FLASH_BUSY_ERASING;
}

/**
 * @brief   Returns the erase state of a sector.
 *
 * @param[in,out] self          Pointer to an EFL driver instance.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_verify_erase(hal_efl_driver_c *self,
                                   flash_sector_t sector) {
  uint32_t *address;
  uintptr_t address_value;
  flash_error_t err = FLASH_NO_ERROR;
  uint32_t sector_size;
  unsigned i;

  osalDbgCheck(self != NULL);
  osalDbgCheck(sector < self->descriptor.sectors_count);
  osalDbgAssert(self->state == FLASH_READ, "invalid state");

  address_value = (uintptr_t)self->descriptor.address +
                  (uintptr_t)flashGetSectorOffset(self, sector);
  osalDbgAssert((address_value & (uintptr_t)(sizeof(uint32_t) - 1U)) == 0U,
                "unaligned flash sector");
  address = (uint32_t *)address_value;
  sector_size = flashGetSectorSize(self, sector);

  for (i = 0U; i < (unsigned)(sector_size / sizeof(uint32_t)); i++) {
    if (*address != 0xFFFFFFFFU) {
      err = FLASH_ERROR_VERIFY;
      break;
    }
    address++;
  }

  return err;
}

#endif /* HAL_USE_EFL == TRUE */

/** @} */
