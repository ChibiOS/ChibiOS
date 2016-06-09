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

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Record identifiers cache size.
 * @details Cache trades RAM for a faster access to stored records. If zero
 *          then the cache is disabled.
 */
#if !defined(MFS_CFG_ID_CACHE_SIZE) || defined(__DOXIGEN__)
#define MFS_CFG_ID_CACHE_SIZE               16
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if MFS_CFG_ID_CACHE_SIZE < 0
#error "invalid MFS_CFG_ID_CACHE_SIZE value"
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
  MFS_NOERROR = 0,
  MFS_REPAIR_WARNING = 1,
  MFS_GC_WARNING = 2,
  MFS_ID_NOT_FOUND = -1,
  MFS_CRC_ERROR = -2,
  MFS_FLASH_FAILURE = -3
} mfs_error_t;

/**
 * @brief   Type of a bank state assessment.
 */
typedef enum {
  MFS_BANK_OK = 0,
  MFS_BANK_ERASED = 1,
  MFS_BANK_PROBLEMS = 2,
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
   */
  uint32_t                  counter;
  /**
   * @brief   First data element.
   */
  flash_address_t           next;
} mfs_bank_header_t;

/**
 * @brief   Type of a data block header.
 */
typedef union {
  struct {
    /**
     * @brief   Data header magic.
     */
    uint16_t                magic;
    /**
     * @brief   Data CRC.
     */
    uint16_t                crc;
    /**
     * @brief   Data identifier.
     */
    uint32_t                id;
    /**
     * @brief   Data size.
     */
    uint32_t                size;
  } fields;
  uint8_t                   h8[16];
  uint32_t                  h32[4];
} mfs_data_header_t;

#if (MFS_CFG_ID_CACHE_SIZE > 0) || defined(__DOXYGEN__)
/**
 * @brief   Type of an element of the record identifiers cache.
 */
typedef struct mfs_cached_id {
  /**
   * @brief   Pointer to the next element in the list.
   */
  struct mfs_cached_id      *lru_next;
  /**
   * @brief   Pointer to the previous element in the list.
   */
  struct mfs_cached_id      *lru_prev;
  /**
   * @brief   Identifier of the cached element.
   */
  uint32_t                  id;
  /**
   * @brief   Data address of the cached element.
   */
  flash_address_t           addr;
  /**
   * @brief   Data size of the cached element.
   */
  uint32_t                  size;
} mfs_cached_id_t;

/**
 * @brief   Type of an element of the record identifiers cache.
 */
typedef struct mfs_cache_header {
  /**
   * @brief   Pointer to the first element in the list.
   */
  struct mfs_cached_id      *lru_next;
  /**
   * @brief   Pointer to the last element in the list.
   */
  struct mfs_cached_id      *lru_prev;
} mfs_cache_header_t;
#endif /* MFS_CFG_ID_CACHE_SIZE > 0 */

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
  flash_address_t           next_position;
  /**
   * @brief   Used space in the current bank without considering erased records.
   */
  uint32_t                  used_space;
#if (MFS_CFG_ID_CACHE_SIZE > 0) || defined(__DOXYGEN__)
  /**
   * @brief   Header of the cache LRU list.
   */
  mfs_cache_header_t        cache_header;
  /**
   * @brief   Array of the cached identifiers.
   */
  mfs_cached_id_t           cache_buffer[MFS_CFG_ID_CACHE_SIZE];
#endif /* MFS_CFG_ID_CACHE_SIZE > 0 */
} MFSDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

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
  mfs_error_t mfsUpdateRecord(MFSDriver *devp, uint32_t id,
                              uint32_t n, const uint8_t *buffer);
  mfs_error_t mfsEraseRecord(MFSDriver *devp, uint32_t id);
#ifdef __cplusplus
}
#endif

#endif /* MFS_H */

/** @} */

