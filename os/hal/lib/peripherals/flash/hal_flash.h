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
 * @brief   @p BaseFlash specific methods.
 * @note    No methods so far, just a common ancestor interface.
 */
#define _base_flash_methods_alone                                           \
  /* Get flash device attributes.*/                                         \
  const flash_descriptor_t * (*get_attributes)(void *instance);             \
  /* Erase whole flash device.*/                                            \
  flash_error_t erase_all(void *instance);                                  \
  /* Erase single sector.*/                                                 \
  flash_error_t erase_sector(void *instance, unsigned sector);              \
  /* Write operation.*/                                                     \
  flash_error_t write(void *instance, const uint8_t *wp, size_t n);         \
  /* Read operation.*/                                                      \
  flash_error_t read(void *instance, uint8_t *rp, size_t n);


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
#define _base_flash_data

/**
 * @brief   Base flash class.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct BaseFlashVMT *vmt_baseflash;
  _base_flash_data
} BaseFlash;

/**
 * @brief   Type of a flash error code.
 */
typedef enum {
  FLASH_NO_ERROR = 0,
  FLASH_PARAMETER_ERROR = 1,
  FLASH_VERIFY_FAILURE = 2,
  FLASH_HW_FAILURE = 3
} flash_error_t;

/**
 * @brief   Flash sector descriptor.
 */
typedef struct {
  /**
   * @brief         Sector address.
   */
  uint8_t               *address;
  /**
   * @brief         Sector size.
   */
  size_t                size;
} flash_sector_t;

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
  unsigned              sectors_count;
  /**
   * @brief     List of flash sectors for devices with non-uniform sector sizes.
   * @note      If @p NULL then the device has uniform sectors size equal
   *            to @p sector_size.
   */
  const flash_sector_t  *sectors;
  /**
   * @brief     Size of flash sectors for devices with uniform sector size.
   * @note      If zero then the device has non uniform sectos described by
   *            the @p sectors array.
   */
  size_t                sectors_size;
  /**
   * @brief     Flash address if memory mapped or zero.
   * @note      Conventionally, non memory mapped devices have address zero.
   */
  uint8_t               *address;
} flash_descriptor_t;

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
#define flashGetType(ip)                                                    \
  (ip)->vmt_baseflash->get_attributes(ip)

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
 * @brief   Single sector erase operation.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @return              An error code.
 *
 * @api
 */
#define flashEraseSector(ip)                                                \
  (ip)->vmt_baseflash->erase_sector(ip, sector)

/**
 * @brief   Write operation.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @param[in] wp        pointer to the data buffer
 * @param[in] n         number of bytes to be written
 * @return              An error code.
 *
 * @api
 */
#define flashWrite(ip)                                                      \
  (ip)->vmt_baseflash->write(ip, wp, n)

/**
 * @brief   Read operation.
 *
 * @param[in] ip        pointer to a @p BaseFlash or derived class
 * @param[out] rp       pointer to the data buffer
 * @param[in] n         number of bytes to be read
 * @return              An error code.
 *
 * @api
 */
#define flashRead(ip)                                                       \
  (ip)->vmt_baseflash->read(ip, rp, n)

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
