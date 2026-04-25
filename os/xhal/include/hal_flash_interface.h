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
 * @file        hal_flash_interface.h
 * @brief       Generated Flash Interface header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_FLASH_INTERFACE
 * @brief       Flash Interface.
 * @details     Base interface for flash devices.
 * @{
 */

#ifndef HAL_FLASH_INTERFACE_H
#define HAL_FLASH_INTERFACE_H

#include "oop_base_interface.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Flash attributes
 * @{
 */
/**
 * @brief       Defines one as the erased bit state.
 */
#define FLASH_ATTR_ERASED_IS_ONE            0x00000001U

/**
 * @brief       The memory is accessible in a memory mapped mode.
 */
#define FLASH_ATTR_MEMORY_MAPPED            0x00000002U

/**
 * @brief       Programmed pages can be programmed again.
 * @note        This is incompatible and alternative to @p
 *              FLASH_ATTR_ECC_CAPABLE.
 */
#define FLASH_ATTR_REWRITABLE               0x00000004U

/**
 * @brief       The memory is protected by an ECC mechanism.
 * @note        This usually puts restrictions on the program operations. -
 *              Program operations can only happen at offsets aligned to write
 *              page boundaries. - The programmed data size must be a multiple
 *              of the write page size. - Programmed pages cannot be
 *              re-programmed.
 */
#define FLASH_ATTR_ECC_CAPABLE              0x00000008U

/**
 * @brief       The device is able to overwrite zero to a line.
 * @note        This attribute is only meaningful for ECC-capable devices.
 */
#define FLASH_ATTR_ECC_ZERO_LINE_CAPABLE    0x00000010U

/**
 * @brief       The device is able to suspend erase operations.
 */
#define FLASH_ATTR_SUSPEND_ERASE_CAPABLE    0x00000020U

/**
 * @brief       Hint to use 4 bytes addresses in SPI protocol.
 */
#define FLASH_ATTR_SPI_4BYTES_ADDR_HINT     0x00008000U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a flash sector descriptor.
 */
typedef struct flash_sector_descriptor flash_sector_descriptor_t;

/**
 * @brief       Type of a flash device descriptor.
 */
typedef struct flash_descriptor flash_descriptor_t;

/**
 * @brief       Type of a flash error code.
 */
typedef enum {
  FLASH_NO_ERROR = 0,
  FLASH_BUSY_ERASING = 1,
  FLASH_ERROR_READ = 2,
  FLASH_ERROR_PROGRAM = 3,
  FLASH_ERROR_ERASE = 4,
  FLASH_ERROR_VERIFY = 5,
  FLASH_ERROR_HW_FAILURE = 6,
  FLASH_ERROR_UNIMPLEMENTED = 7
} flash_error_t;

/**
 * @brief       Type of a flash offset.
 */
typedef uint32_t flash_offset_t;

/**
 * @brief       Type of a flash sector number.
 */
typedef uint32_t flash_sector_t;

/**
 * @brief       Flash sector descriptor.
 */
struct flash_sector_descriptor {
  /**
   * @brief       Sector offset.
   */
  flash_offset_t            offset;
  /**
   * @brief       Sector size.
   */
  uint32_t                  size;
};

/**
 * @brief       Flash device descriptor.
 */
struct flash_descriptor {
  /**
   * @brief       Device attributes.
   */
  uint32_t                  attributes;
  /**
   * @brief       Size of write page.
   */
  uint32_t                  page_size;
  /**
   * @brief       Number of sectors in the device.
   */
  flash_sector_t            sectors_count;
  /**
   * @brief       Sector descriptors for non-uniform devices or @p NULL.
   */
  const flash_sector_descriptor_t * sectors;
  /**
   * @brief       Uniform sector size or zero for non-uniform devices.
   */
  uint32_t                  sectors_size;
  /**
   * @brief       Mapped base address or @p NULL if not memory mapped.
   */
  uint8_t *                 address;
  /**
   * @brief       Flash size.
   */
  uint32_t                  size;
};

/**
 * @interface   flash_interface_i
 * @extends     base_interface_i
 *
 * @brief       Generic flash interface.
 *
 * @name        Interface @p flash_interface_i structures
 * @{
 */

/**
 * @brief       Type of a flash interface interface.
 */
typedef struct flash_interface flash_interface_i;

/**
 * @brief       Interface @p flash_interface_i virtual methods table.
 */
struct flash_interface_vmt {
  /* Memory offset between this interface structure and begin of
     the implementing class structure.*/
  size_t instance_offset;
  /* From base_interface_i.*/
  /* From flash_interface_i.*/
  const flash_descriptor_t * (*get_descriptor)(void *ip);
  flash_error_t (*read)(void *ip, flash_offset_t offset, size_t n, uint8_t *rp);
  flash_error_t (*program)(void *ip, flash_offset_t offset, size_t n, const uint8_t *pp);
  flash_error_t (*start_erase_all)(void *ip);
  flash_error_t (*start_erase_sector)(void *ip, flash_sector_t sector);
  flash_error_t (*query_erase)(void *ip, unsigned *msec);
  flash_error_t (*verify_erase)(void *ip, flash_sector_t sector);
  flash_error_t (*acquire_exclusive)(void *ip);
  flash_error_t (*release_exclusive)(void *ip);
};

/**
 * @brief       Structure representing a flash interface interface.
 */
struct flash_interface {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct flash_interface_vmt *vmt;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Regular functions.*/
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of flash_interface_i
 * @{
 */
/**
 * @brief       Gets the flash descriptor structure.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @return                      A flash device descriptor.
 *
 * @api
 */
CC_FORCE_INLINE
static inline const flash_descriptor_t *flsGetDescriptor(void *ip) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->get_descriptor(ip);
}

/**
 * @brief       Read operation.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING   If there is an erase operation in progress.
 * @retval FLASH_ERROR_READ     If the read operation failed.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsRead(void *ip, flash_offset_t offset, size_t n,
                                    uint8_t *rp) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->read(ip, offset, n, rp);
}

/**
 * @brief       Program operation.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING   If there is an erase operation in progress.
 * @retval FLASH_ERROR_PROGRAM  If the program operation failed.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsProgram(void *ip, flash_offset_t offset,
                                       size_t n, const uint8_t *pp) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->program(ip, offset, n, pp);
}

/**
 * @brief       Starts a whole-device erase operation.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING   If there is an erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsStartEraseAll(void *ip) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->start_erase_all(ip);
}

/**
 * @brief       Starts a sector erase operation.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING   If there is an erase operation in progress.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsStartEraseSector(void *ip,
                                                flash_sector_t sector) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->start_erase_sector(ip, sector);
}

/**
 * @brief       Queries the driver for erase operation progress.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @param[out]    msec          Recommended time, in milliseconds, before
 *                              retrying, can be @p NULL.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If there is no erase operation in progress.
 * @retval FLASH_BUSY_ERASING   If there is an erase operation in progress.
 * @retval FLASH_ERROR_ERASE    If the erase operation failed.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsQueryErase(void *ip, unsigned *msec) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->query_erase(ip, msec);
}

/**
 * @brief       Returns the erase state of a sector.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If the sector is erased.
 * @retval FLASH_BUSY_ERASING   If there is an erase operation in progress.
 * @retval FLASH_ERROR_VERIFY   If the verify operation failed.
 * @retval FLASH_ERROR_HW_FAILURE If access to the memory failed.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsVerifyErase(void *ip, flash_sector_t sector) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->verify_erase(ip, sector);
}

/**
 * @brief       Acquires exclusive access to flash.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If the access is obtained.
 * @retval FLASH_ERROR_UNIMPLEMENTED If exclusive access is not implemented.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsAcquireExclusive(void *ip) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->acquire_exclusive(ip);
}

/**
 * @brief       Releases exclusive access to flash.
 *
 * @param[in,out] ip            Pointer to a @p flash_interface_i instance.
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       If the access is released.
 * @retval FLASH_ERROR_UNIMPLEMENTED If exclusive access is not implemented.
 *
 * @api
 */
CC_FORCE_INLINE
static inline flash_error_t flsReleaseExclusive(void *ip) {
  flash_interface_i *self = (flash_interface_i *)ip;

  return self->vmt->release_exclusive(ip);
}
/** @} */

#endif /* HAL_FLASH_INTERFACE_H */

/** @} */
