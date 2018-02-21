/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
#define MFS_HEADER_MAGIC                    0x5FAE45F0U

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

/**
 * @brief   Enables a stronger and slower check procedure on mount.
 * @details Strong checking requires reading of the whole written data and
 *          this can be slow, normal checking only checks integrity of
 *          metadata, data errors would be detected on read.
 */
#if !defined(MFS_CFG_STRONG_CHECKING) || defined(__DOXYGEN__)
#define MFS_CFG_STRONG_CHECKING             TRUE
#endif

/**
 * @brief   Size of the buffer used for data copying.
 * @note    The buffer size must be a power of two and not smaller than
 *          16 bytes.
 * @note    Larger buffers improve performance, buffers with size multiple
 *          of the flash program page size work better.
 */
#if !defined(MFS_CFG_BUFFER_SIZE) || defined(__DOXYGEN__)
#define MFS_CFG_BUFFER_SIZE                 32
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

#if MFS_CFG_BUFFER_SIZE <= 16
#error "invalid MFS_CFG_BUFFER_SIZE value"
#endif

#if (MFS_CFG_BUFFER_SIZE & (MFS_CFG_BUFFER_SIZE - 1)) != 0
#error "MFS_CFG_BUFFER_SIZE is not a power of two"
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
  MFS_ERROR = 3
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
  MFS_ERR_INV_STATE = -1,
  MFS_ERR_INV_SIZE = -2,
  MFS_ERR_NOT_FOUND = -3,
  MFS_ERR_OUT_OF_MEM = -4,
  MFS_ERR_NOT_ERASED = -5,
  MFS_ERR_FLASH_FAILURE = -6,
  MFS_ERR_INTERNAL = -7
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
 * @brief   Type of a record state assessment.
 */
typedef enum {
  MFS_RECORD_ERASED = 0,
  MFS_RECORD_OK = 1,
  MFS_RECORD_CRC = 2,
  MFS_RECORD_GARBAGE = 3
} mfs_record_state_t;

/**
 * @brief   Type of a record identifier.
 */
typedef uint32_t mfs_id_t;

/**
 * @brief   Type of a bank header.
 * @note    The header resides in the first 16 bytes of a bank.
 */
typedef union {
  struct {
    /**
     * @brief   Bank magic 1.
     */
    uint32_t                magic1;
    /**
     * @brief   Bank magic 2.
     */
    uint32_t                magic2;
    /**
     * @brief   Usage counter of the bank.
     * @details This value is increased each time a bank swap is performed. It
     *          indicates how much wearing the flash has already endured.
     */
    uint32_t                counter;
    /**
     * @brief   Reserved field.
     */
    uint16_t                reserved1;
    /**
     * @brief   Header CRC.
     */
    uint16_t                crc;
  } fields;
  uint8_t                   hdr8[16];
  uint32_t                  hdr32[4];
} mfs_bank_header_t;

/**
 * @brief   Type of a data block header.
 * @details This structure is placed before each written data block.
 */
typedef union {
  struct {
    /**
     * @brief   Data header magic.
     */
    uint32_t                magic;
    /**
     * @brief   Data identifier.
     */
    uint16_t                id;
    /**
     * @brief   Data CRC.
     */
    uint16_t                crc;
    /**
     * @brief   Data size.
     */
    uint32_t                size;
  } fields;
  uint8_t                   hdr8[12];
  uint32_t                  hdr32[3];
} mfs_data_header_t;

typedef struct {
  /**
   * @brief   Offset of the record header.
   */
  flash_offset_t            offset;
  /**
   * @brief   Record data size.
   */
  uint32_t                  size;
} mfs_record_descriptor_t;

/**
 * @brief   Type of a MFS configuration structure.
 */
typedef struct {
  /**
   * @brief   Flash driver associated to this MFS instance.
   */
  BaseFlash                 *flashp;
  /**
   * @brief   Erased value.
   */
  uint32_t                  erased;
  /**
   * @brief   Banks size.
   */
  flash_offset_t            bank_size;
  /**
   * @brief   Base sector index for bank 0.
   */
  flash_sector_t            bank0_start;
  /**
   * @brief   Number of sectors for bank 0.
   * @note    The total size of bank0 sectors must be greater or equal to
   *          @p bank_size.
   */
  flash_sector_t            bank0_sectors;
  /**
   * @brief   Base sector index for bank 1.
   */
  flash_sector_t            bank1_start;
  /**
   * @brief   Number of sectors for bank 1.
   * @note    The total size of bank1 sectors must be greater or equal to
   *          @p bank_size.
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
   * @brief   Usage counter of the current bank.
   */
  uint32_t                  current_counter;
  /**
   * @brief   Pointer to the next free position in the current bank.
   */
  flash_offset_t            next_offset;
  /**
   * @brief   Used space in the current bank without considering erased records.
   */
  flash_offset_t            used_space;
  /**
   * @brief   Offsets of the most recent instance of the records.
   * @note    Zero means that there is not a record with that id.
   */
  mfs_record_descriptor_t   descriptors[MFS_CFG_MAX_RECORDS];
  /**
   * @brief   Transient buffer.
   */
  union {
    mfs_data_header_t       dhdr;
    mfs_bank_header_t       bhdr;
    uint8_t                 data8[MFS_CFG_BUFFER_SIZE];
    uint16_t                data16[MFS_CFG_BUFFER_SIZE / sizeof (uint16_t)];
    uint32_t                data32[MFS_CFG_BUFFER_SIZE / sizeof (uint32_t)];
  } buffer;
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
  mfs_error_t mfsStart(MFSDriver *devp, const MFSConfig *config);
  void mfsStop(MFSDriver *devp);
  mfs_error_t mfsErase(MFSDriver *mfsp);
  mfs_error_t mfsReadRecord(MFSDriver *devp, mfs_id_t id,
                            size_t *np, uint8_t *buffer);
  mfs_error_t mfsWriteRecord(MFSDriver *devp, mfs_id_t id,
                             size_t n, const uint8_t *buffer);
  mfs_error_t mfsEraseRecord(MFSDriver *devp, mfs_id_t id);
  mfs_error_t mfsPerformGarbageCollection(MFSDriver *mfsp);
#ifdef __cplusplus
}
#endif

#endif /* MFS_H */

/** @} */

