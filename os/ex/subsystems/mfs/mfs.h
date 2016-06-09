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
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

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
    uint32_t                magic;
    /**
     * @brief   Data identifier.
     */
    uint32_t                id;
    /**
     * @brief   Data size.
     */
    uint32_t                size;
    /**
     * @brief   Data CRC.
     */
    uint32_t                crc;
  } fields;
  uint8_t                   h8[16];
  uint32_t                  h32[4];
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

