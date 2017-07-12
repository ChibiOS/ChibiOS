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
 * @file    mfs.h
 * @brief   Managed Flash Storage module header.
 *
 * @{
 */

#ifndef MFS_H
#define MFS_H

#include "hal_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define MFS_BANK_MAGIC_1                    0xEC705ADEU
#define MFS_BANK_MAGIC_2                    0xF0339CC5U
#define MFS_HEADER_MAGIC                    0x5FAEU

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Maximum number of indexed records in the managed storage.
 * @note    Record indexes go from 0 to @p MFS_CFG_MAX_RECORDS - 1.
 */
#if !defined(MFS_CFG_MAX_RECORDS) || defined(__DOXYGEN__)
#define MFS_CFG_MAX_RECORDS                 32
#endif

/**
 * @brief   Maximum number of repair attempts on partition mount.
 */
#if !defined(MFS_CFG_MAX_REPAIR_ATTEMPTS) || defined(__DOXYGEN__)
#define MFS_CFG_MAX_REPAIR_ATTEMPTS         3
#endif

/**
 * @brief   Verify written data.
 */
#if !defined(MFS_CFG_WRITE_VERIFY) || defined(__DOXYGEN__)
#define MFS_CFG_WRITE_VERIFY                TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if MFS_CFG_MAX_RECORDS < 0
#error "invalid MFS_CFG_MAX_RECORDS value"
#endif

#if (MFS_CFG_MAX_REPAIR_ATTEMPTS < 1) || (MFS_CFG_MAX_REPAIR_ATTEMPTS > 10)
#error "invalid MFS_MAX_REPAIR_ATTEMPTS value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a flash bank.
 */
typedef enum {
  MFS_BANK_0 = 0,
  MFS_BANK_1 = 1
} mfs_bank_t;

/**
 * @brief   Type of driver state machine states.
 */
typedef enum {
  MFS_UNINIT = 0,
  MFS_STOP = 1,
  MFS_READY = 2,
  MFS_MOUNTED = 3,
  MFS_ACTIVE = 4
} mfs_state_t;

/**
 * @brief   Type of an MFS error code.
 * @note    Errors are negative integers, informative warnings are positive
 *          integers.
 */
typedef enum {
  MFS_NO_ERROR = 0,
  MFS_WARN_REPAIR = 1,
  MFS_WARN_GC = 2,
  MFS_ERR_NOT_FOUND = -1,
  MFS_ERR_CRC = -2,
  MFS_ERR_FLASH_FAILURE = -3,
  MFS_ERR_INTERNAL = -4
} mfs_error_t;

/**
 * @brief   Type of a bank state assessment.
 */
typedef enum {
  MFS_BANK_ERASED = 0,
  MFS_BANK_OK = 1,
  MFS_BANK_PARTIAL = 2,
  MFS_BANK_GARBAGE = 3
} mfs_bank_state_t;

/**
 * @brief   Type of a bank header.
 * @note    The header resides in the first 16 bytes of a bank extending
 *          to the next page boundary.
 */
typedef struct {
  /**
   * @brief   Bank magic 1.
   */
  uint32_t                  magic1;
  /**
   * @brief   Bank magic 2.
   */
  uint32_t                  magic2;
  /**
   * @brief   Usage counter of the bank.
   * @details This value is increased each time a bank swap is performed. It
   *          indicates how much wearing the flash has already endured.
   */
  uint32_t                  counter;
  /**
   * @brief   First data element.
   */
  flash_offset_t            next;
  /**
   * @brief   Header CRC.
   */
  uint16_t                  crc;
} mfs_bank_header_t;

/**
 * @brief   Type of a data block header.
 * @details This structure is placed before each written data block.
 */
typedef struct {
  /**
   * @brief   Data header magic.
   */
  uint16_t                  magic;
  /**
   * @brief   Data CRC.
   */
  uint16_t                  crc;
  /**
   * @brief   Data identifier.
   */
  uint16_t                  id;
  /**
   * @brief   Data attributes.
   */
  uint16_t                  flags;
  /**
   * @brief   Data size.
   */
  uint32_t                  size;
} mfs_data_header_t;

/**
 * @brief   Type of a MFS configuration structure.
 */
typedef struct {
  /**
   * @brief   Flash driver associated to this MFS instance.
   */
  BaseFlash                 *flashp;
  /**
   * @brief   Base sector index for bank 0.
   */
  flash_sector_t            bank0_start;
  /**
   * #brief   Number of sectors for bank 0.
   */
  flash_sector_t            bank0_sectors;
  /**
   * @brief   Base sector index for bank 1.
   */
  flash_sector_t            bank1_start;
  /**
   * #brief   Number of sectors for bank 1.
   */
  flash_sector_t            bank1_sectors;
} MFSConfig;

/**
 * @extends BaseFlash
 *
 * @brief   Type of an MFS instance.
 */
typedef struct {
  /**
   * @brief   Driver state.
   */
  mfs_state_t               state;
  /**
   * @brief   Current configuration data.
   */
  const MFSConfig           *config;
  /**
   * @brief   Bank currently in use.
   */
  mfs_bank_t                current_bank;
  /**
   * @brief   Size in bytes of banks.
   */
  uint32_t                  banks_size;
  /**
   * @brief   Pointer to the next free position in the current bank.
   */
  flash_offset_t            next_offset;
  /**
   * @brief   Used space in the current bank without considering erased records.
   */
  uint32_t                  used_space;
  /**
   * @brief   Offsets of the most recent instance of the records.
   * @note    Zero means that ther is not a record with that id.
   */
  flash_offset_t            instances[MFS_CFG_MAX_RECORDS];
} MFSDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name   Error codes handling macros
 * @{
 */
#define MFS_IS_ERROR(err) ((err) < MFS_NO_ERROR)
#define MFS_IS_WARNING(err) ((err) > MFS_NO_ERROR)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void mfsObjectInit(MFSDriver *devp);
  void mfsStart(MFSDriver *devp, const MFSConfig *config);
  void mfsStop(MFSDriver *devp);
  mfs_error_t mfsMount(MFSDriver *devp);
  mfs_error_t mfsUnmount(MFSDriver *devp);
  mfs_error_t mfsReadRecord(MFSDriver *devp, uint32_t id,
                            uint32_t *np, uint8_t *buffer);
  mfs_error_t mfsWriteRecord(MFSDriver *devp, uint32_t id,
                             uint32_t n, const uint8_t *buffer);
  mfs_error_t mfsEraseRecord(MFSDriver *devp, uint32_t id);
#ifdef __cplusplus
}
#endif

#endif /* MFS_H */

/** @} */

