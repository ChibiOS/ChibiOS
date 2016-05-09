/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    hal_flash.h
 * @brief   Generic flash interface header.
 *
 * @addtogroup HAL_FLASH
 * @{
 */

#ifndef HAL_FLASH_H
#define HAL_FLASH_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Flash attributes
 * @{
 */
#define FLASH_ATTR_ERASED_IS_ONE        0x00000001
#define FLASH_ATTR_MEMORY_MAPPED        0x00000002
#define FLASH_ATTR_REWRITABLE           0x00000004
#define FLASH_ATTR_READ_ECC_CAPABLE     0x00000008
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  FLASH_UNINIT = 0,
  FLASH_STOP = 1,
  FLASH_READY = 2,
  FLASH_ACTIVE = 3
} flash_state_t;

/**
 * @brief   Type of a flash error code.
 */
typedef enum {
  FLASH_NO_ERROR = 0,           /* No error.                                */
  FLASH_ECC_ERROR = 1,          /* ECC error during read operation.         */
  FLASH_PROGRAM_FAILURE = 2,    /* Program operation failed.                */
  FLASH_ERASE_FAILURE = 3,      /* Erase operation failed.                  */
  FLASH_VERIFY_FAILURE = 4,     /* Verify operation failed.                 */
  FLASH_HW_FAILURE = 5          /* Controller or communication error.       */
} flash_error_t;

/**
 * @brief   Type of a flash address.
 */
typedef uint32_t flash_address_t;

/**
 * @brief   Type of a flash sector number.
 */
typedef uint32_t flash_sector_t;

/**
 * @brief   Flash sector descriptor.
 */
typedef struct {
  /**
   * @brief         Sector address.
   */
  flash_address_t       address;
  /**
   * @brief         Sector size.
   */
  size_t                size;
} flash_sector_descriptor_t;

/**
 * @brief   Type of a flash device descriptor.
 */
typedef struct {
  /**
   * @brief     Device_attributes.
   */
  uint32_t              attributes;
  /**
   * @brief     Size of write page.
   */
  size_t                page_size;
  /**
   * @brief     Number of sectors in the device.
   */
  flash_sector_t        sectors_count;
  /**
   * @brief     List of sectors for devices with non-uniform sector sizes.
   * @note      If @p NULL then the device has uniform sectors size equal
   *            to @p sector_size.
   */
  const flash_sector_descriptor_t *sectors;
  /**
   * @brief     Size of sectors for devices with uniform sector size.
   * @note      If zero then the device has non uniform sectos described
   *            by the @p sectors array.
   */
  size_t                sectors_size;
  /**
   * @brief     Flash address if memory mapped or zero.
   * @note      Conventionally, non memory mapped devices have address zero.
   */
  flash_address_t       address;
} flash_descriptor_t;

/**
 * @brief   @p BaseFlash specific methods.
 * @note    No methods so far, just a common ancestor interface.
 */
#define _base_flash_methods_alone                                           \
  /* Get flash device attributes.*/                                         \
  const flash_descriptor_t * (*get_descriptor)(void *instance);             \
  /* Erase whole flash device.*/                                            \
  flash_error_t (*erase_all)(void *instance);                               \
  /* Erase single sector.*/                                                 \
  flash_error_t (*erase_sectors)(void *instance,                            \
                                flash_sector_t sector,                      \
                                flash_sector_t n);                          \
  /* Erase single sector.*/                                                 \
  flash_error_t (*verify_erase)(void *instance,                             \
                                flash_sector_t sector,                      \
                                flash_sector_t n);                          \
  /* Write operation.*/                                                     \
  flash_error_t (*program)(void *instance, flash_address_t addr,            \
                           const uint8_t *pp, size_t n);                    \
  /* Read operation.*/                                                      \
  flash_error_t (*read)(void *instance, flash_address_t addr,               \
                        uint8_t *rp, size_t n);

/**
 * @brief   @p BaseFlash specific methods with inherited ones.
 */
#define _base_flash_methods                                                 \
  _base_flash_methods_alone

/**
 * @brief   @p BaseFlash virtual methods table.
 */
struct BaseFlashVMT {
  _base_flash_methods
};

/**
 * @brief   @p BaseFlash specific data.
 * @note    It is empty because @p BaseFlash is only an interface
 *          without implementation.
 */
#define _base_flash_data                                                    \
  /* Driver state.*/                                                        \
  flash_state_t         state;


/**
 * @brief   Base flash class.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseFlashVMT *vmt_baseflash;
  _base_flash_data
} BaseFlash;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions (BaseFlash)
 * @{
 */
/**
 * @brief   Sensors get axes number.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @return              An error code.
 *
 * @api
 */
#define flashGetDescriptor(ip)                                              \
  (ip)->vmt_baseflash->get_descriptor(ip)

/**
 * @brief   Whole device erase operation.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @return              An error code.
 *
 * @api
 */
#define flashEraseAll(ip)                                                   \
  (ip)->vmt_baseflash->erase_all(ip)

/**
 * @brief   Erase operation on a series of contiguous sectors.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @param[in] secotr    first sector to be erased
 * @param[in] n         number of sectors to be erased
 * @return              An error code.
 *
 * @api
 */
#define flashEraseSectors(ip, sector, n)                                    \
  (ip)->vmt_baseflash->erase_sectors(ip, sector, n)

/**
 * @brief   Returns the erase state of a series of contiguous sectors.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @param[in] secotr    first sector to be erased
 * @param[in] n         number of sectors to be erased
 * @return              An error code.
 *
 * @api
 */
#define flashVerifyErase(ip, sector, n)                                \
  (ip)->vmt_baseflash->verify_erase(ip, sector, n)

/**
 * @brief   Write operation.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @param[in] addr      flash address
 * @param[in] wp        pointer to the data buffer
 * @param[in] n         number of bytes to be programmed
 * @return              An error code.
 *
 * @api
 */
#define flashProgram(ip, addr, pp, n)                                       \
  (ip)->vmt_baseflash->program(ip, addr, pp, n)

/**
 * @brief   Read operation.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @param[in] addr      flash address
 * @param[out] rp       pointer to the data buffer
 * @param[in] n         number of bytes to be read
 * @return              An error code.
 *
 * @api
 */
#define flashRead(ip, addr, rp, n)                                          \
  (ip)->vmt_baseflash->read(ip, addr, rp, n)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* HAL_FLASH_H */

/** @} */
