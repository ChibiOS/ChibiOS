/*
    ChibiOS - Copyright (C) 2016 Giovanni Di Sirio

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
 * @file    m25q.h
 * @brief   Micron serial flash driver header.
 *
 * @addtogroup M25Q
 * @ingroup EX_MICRON
 * @{
 */

#ifndef M25Q_H
#define M25Q_H

#include "hal_jesd216_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Version identification
 * @{
 */
/**
 * @brief   M25Q driver version string.
 */
#define EX_M25Q_VERSION         "1.0.0"

/**
 * @brief   M25Q driver version major number.
 */
#define EX_M25Q_MAJOR           1

/**
 * @brief   M25Q driver version minor number.
 */
#define EX_M25Q_MINOR           0

/**
 * @brief   M25Q driver version patch number.
 */
#define EX_M25Q_PATCH           0
/** @} */

/**
 * @name    Command codes
 * @{
 */
#define M25Q_CMD_RESET_ENABLE                       0x66
#define M25Q_CMD_RESET_MEMORY                       0x99
#define M25Q_CMD_READ_ID                            0x9F
#define M25Q_CMD_MULTIPLE_IO_READ_ID                0xAF
#define M25Q_CMD_READ_DISCOVERY_PARAMETER           0x5A
#define M25Q_CMD_READ                               0x03
#define M25Q_CMD_FAST_READ                          0x0B
#define M25Q_CMD_WRITE_ENABLE                       0x06
#define M25Q_CMD_WRITE_DISABLE                      0x04
#define M25Q_CMD_READ_STATUS_REGISTER               0x05
#define M25Q_CMD_WRITE_STATUS_REGISTER              0x01
#define M25Q_CMD_READ_LOCK_REGISTER                 0xE8
#define M25Q_CMD_WRITE_LOCK_REGISTER                0xE5
#define M25Q_CMD_READ_FLAG_STATUS_REGISTER          0x70
#define M25Q_CMD_CLEAR_FLAG_STATUS_REGISTER         0x50
#define M25Q_CMD_READ_NV_CONFIGURATION_REGISTER     0xB5
#define M25Q_CMD_WRITE_NV_CONFIGURATION_REGISTER    0xB1
#define M25Q_CMD_READ_V_CONF_REGISTER               0x85
#define M25Q_CMD_WRITE_V_CONF_REGISTER              0x81
#define M25Q_CMD_READ_ENHANCED_V_CONF_REGISTER      0x65
#define M25Q_CMD_WRITE_ENHANCED_V_CONF_REGISTER     0x61
#define M25Q_CMD_PAGE_PROGRAM                       0x02
#define M25Q_CMD_SUBSECTOR_ERASE                    0x20
#define M25Q_CMD_SECTOR_ERASE                       0xD8
#define M25Q_CMD_BULK_ERASE                         0xC7
#define M25Q_CMD_PROGRAM_ERASE_RESUME               0x7A
#define M25Q_CMD_PROGRAM_ERASE_SUSPEND              0x75
#define M25Q_CMD_READ_OTP_ARRAY                     0x4B
#define M25Q_CMD_PROGRAM_OTP_ARRAY                  0x42
/** @} */

/**
 * @name    Flags status register bits
 * @{
 */
#define M25Q_FLAGS_PROGRAM_ERASE                    0x80U
#define M25Q_FLAGS_ERASE_SUSPEND                    0x40U
#define M25Q_FLAGS_ERASE_ERROR                      0x20U
#define M25Q_FLAGS_PROGRAM_ERROR                    0x10U
#define M25Q_FLAGS_VPP_ERROR                        0x08U
#define M25Q_FLAGS_PROGRAM_SUSPEND                  0x04U
#define M25Q_FLAGS_PROTECTION_ERROR                 0x02U
#define M25Q_FLAGS_RESERVED                         0x01U
#define M25Q_FLAGS_ALL_ERRORS                   (M25Q_FLAGS_ERASE_ERROR |   \
                                                 M25Q_FLAGS_PROGRAM_ERROR | \
                                                 M25Q_FLAGS_VPP_ERROR |     \
                                                 M25Q_FLAGS_PROTECTION_ERROR)
/** @} */

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
#if !defined(M25Q_READ_DUMMY_CYCLES) || defined(__DOXYGEN__)
#define M25Q_READ_DUMMY_CYCLES              8
#endif

/**
 * @brief   Switch QSPI bus width on initialization.
 * @details A bus width initialization is performed by writing the
 *          Enhanced Volatile Configuration Register. If the flash
 *          device is configured using the Non Volatile Configuration
 *          Register then this option is not required.
 * @note    This option is only valid in QSPI bus modes.
 */
#if !defined(M25Q_SWITCH_WIDTH) || defined(__DOXYGEN__)
#define M25Q_SWITCH_WIDTH                   TRUE
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the flash waiting
 *          routines releasing some extra CPU time for threads with lower
 *          priority, this may slow down the driver a bit however.
 */
#if !defined(M25Q_NICE_WAITING) || defined(__DOXYGEN__)
#define M25Q_NICE_WAITING                   TRUE
#endif

/**
 * @brief   Uses 4kB sub-sectors rather than 64kB sectors.
 */
#if !defined(M25Q_USE_SUB_SECTORS) || defined(__DOXYGEN__)
#define M25Q_USE_SUB_SECTORS                FALSE
#endif

/**
 * @brief   Supported JEDEC manufacturer identifiers.
 */
#if !defined(M25Q_SUPPORTED_MANUFACTURE_IDS) || defined(__DOXYGEN__)
#define M25Q_SUPPORTED_MANUFACTURE_IDS      {0x20}
#endif

/**
 * @brief   Supported memory type identifiers.
 */
#if !defined(M25Q_SUPPORTED_MEMORY_TYPE_IDS) || defined(__DOXYGEN__)
#define M25Q_SUPPORTED_MEMORY_TYPE_IDS      {0xBA, 0xBB}
#endif

/**
 * @brief   Size of the compare buffer.
 * @details This buffer is allocated in the stack frame of the function
 *          @p flashVerifyErase() and its size must be a power of two.
 *          Larger buffers lead to better verify performance but increase
 *          stack usage for that function.
 */
#if !defined(M25Q_COMPARE_BUFFER_SIZE) || defined(__DOXYGEN__)
#define M25Q_COMPARE_BUFFER_SIZE            32
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (M25Q_READ_DUMMY_CYCLES < 1) || (M25Q_READ_DUMMY_CYCLES > 15)
#error "invalid M25Q_READ_DUMMY_CYCLES value (1..15)"
#endif

#if (M25Q_COMPARE_BUFFER_SIZE & (M25Q_COMPARE_BUFFER_SIZE - 1)) != 0
#error "invalid M25Q_COMPARE_BUFFER_SIZE value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a M25Q configuration structure.
 */
typedef struct {
  _jesd216_config
} M25QConfig;

/**
 * @brief   @p M25Q specific methods.
 */
#define _m25q_methods                                                       \
  _jesd216_flash_methods

/**
 * @extends JESD216FlashVMT
 *
 * @brief   @p M25Q virtual methods table.
 */
struct M25QDriverVMT {
  _m25q_methods
};
  
/**
 * @extends JESD216Flash
 *
 * @brief   Type of M25Q flash class.
 */
typedef struct {
  /**
   * @brief   M25QDriver Virtual Methods Table.
   */
  const struct M25QDriverVMT    *vmt;
  _jesd216_flash_data
  /**
   * @brief   Current configuration data.
   */
  const M25QConfig              *config;
  /**
   * @brief   Device ID and unique ID.
   */
  uint8_t                       device_id[20];
} M25QDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void m25qObjectInit(M25QDriver *devp);
  void m25qStart(M25QDriver *devp, const M25QConfig *config);
  void m25qStop(M25QDriver *devp);
#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
#if (QSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
  void m25qMemoryMap(M25QDriver *devp, uint8_t ** addrp);
  void m25qMemoryUnmap(M25QDriver *devp);
#endif /* QSPI_SUPPORTS_MEMMAP == TRUE */
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */
#ifdef __cplusplus
}
#endif

#endif /* M25Q_H */

/** @} */

