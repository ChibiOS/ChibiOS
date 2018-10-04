/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    m25q.h
 * @brief   M25Q serial flash driver header.
 *
 * @addtogroup M25Q
 * @ingroup M25Q
 * @{
 */

#ifndef M25Q_H
#define M25Q_H

#include "hal_jesd216_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Number of dummy cycles for fast read (1..15).
 * @details This is the number of dummy cycles to be used for fast read
 *          operations.
 */
#if !defined(SNOR_READ_DUMMY_CYCLES) || defined(__DOXYGEN__)
#define SNOR_READ_DUMMY_CYCLES              8
#endif

/**
 * @brief   Switch QSPI bus width on initialization.
 * @details A bus width initialization is performed by writing the
 *          Enhanced Volatile Configuration Register. If the flash
 *          device is configured using the Non Volatile Configuration
 *          Register then this option is not required.
 * @note    This option is only valid in QSPI bus modes.
 */
#if !defined(SNOR_SWITCH_WIDTH) || defined(__DOXYGEN__)
#define SNOR_SWITCH_WIDTH                   TRUE
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the flash waiting
 *          routines releasing some extra CPU time for threads with lower
 *          priority, this may slow down the driver a bit however.
 */
#if !defined(SNOR_NICE_WAITING) || defined(__DOXYGEN__)
#define SNOR_NICE_WAITING                   TRUE
#endif

/**
 * @brief   Uses 4kB sub-sectors rather than 64kB sectors.
 */
#if !defined(SNOR_USE_SUB_SECTORS) || defined(__DOXYGEN__)
#define SNOR_USE_SUB_SECTORS                FALSE
#endif

/**
 * @brief   Size of the compare buffer.
 * @details This buffer is allocated in the stack frame of the function
 *          @p flashVerifyErase() and its size must be a power of two.
 *          Larger buffers lead to better verify performance but increase
 *          stack usage for that function.
 */
#if !defined(SNOR_COMPARE_BUFFER_SIZE) || defined(__DOXYGEN__)
#define SNOR_COMPARE_BUFFER_SIZE            32
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SNOR_READ_DUMMY_CYCLES < 1) || (SNOR_READ_DUMMY_CYCLES > 15)
#error "invalid SNOR_READ_DUMMY_CYCLES value (1..15)"
#endif

#if (SNOR_COMPARE_BUFFER_SIZE & (SNOR_COMPARE_BUFFER_SIZE - 1)) != 0
#error "invalid SNOR_COMPARE_BUFFER_SIZE value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a M25Q configuration structure.
 */
typedef struct {
  _jesd216_config
} SNORConfig;

/**
 * @brief   @p SNOR specific methods.
 */
#define _snor_methods                                                       \
  _jesd216_flash_methods

/**
 * @extends JESD216FlashVMT
 *
 * @brief   @p SNOR virtual methods table.
 */
struct SNORDriverVMT {
  _snor_methods
};
  
/**
 * @extends JESD216Flash
 *
 * @brief   Type of SNOR flash class.
 */
typedef struct {
  /**
   * @brief   SNORDriver Virtual Methods Table.
   */
  const struct SNORDriverVMT    *vmt;
  _jesd216_flash_data
  /**
   * @brief   Current configuration data.
   */
  const SNORConfig              *config;
  /**
   * @brief   Device ID and unique ID.
   */
  uint8_t                       device_id[20];
} SNORDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void m25qObjectInit(SNORDriver *devp);
  void m25qStart(SNORDriver *devp, const SNORConfig *config);
  void m25qStop(SNORDriver *devp);
#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
#if (WSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
  void m25qMemoryMap(SNORDriver *devp, uint8_t ** addrp);
  void m25qMemoryUnmap(SNORDriver *devp);
#endif /* QSPI_SUPPORTS_MEMMAP == TRUE */
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */
#ifdef __cplusplus
}
#endif

/* Device-specific implementations.*/
#include "flash_device.h"

#endif /* M25Q_H */

/** @} */

