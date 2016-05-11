/*
    N25Q128 Flash Driver - Copyright (C) 2016 Giovanni Di Sirio

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
 * @file    n25q128_spi.h
 * @brief   N25Q128 over SPI driver header.
 *
 * @addtogroup n25q128_spi
 * @{
 */

#ifndef N25Q128_H
#define N25Q128_H

#include "hal_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Command codes
 * @{
 */
#define N25Q128_CMD_RESET_ENABLE                    0x66
#define N25Q128_CMD_RESET_MEMORY                    0x99
#define N25Q128_CMD_READ_ID                         0x9E
#define N25Q128_CMD_READ_DISCOVERY_PARAMETER        0x5A
#define N25Q128_CMD_READ                            0x03
#define N25Q128_CMD_FAST_READ                       0x08
#define N25Q128_CMD_WRITE_ENABLE                    0x06
#define N25Q128_CMD_WRITE_DISABLE                   0x04
#define N25Q128_CMD_READ_STATUS_REGISTER            0x05
#define N25Q128_CMD_WRITE_STATUS_REGISTER           0x01
#define N25Q128_CMD_READ_LOCK_REGISTER              0xE8
#define N25Q128_CMD_WRITE_LOCK_REGISTER             0xE5
#define N25Q128_CMD_READ_FLAG_STATUS_REGISTER       0x70
#define N25Q128_CMD_CLEAR_FLAG_STATUS_REGISTER      0x50
#define N25Q128_CMD_READ_NV_CONFIGURATION_REGISTER  0xB5
#define N25Q128_CMD_WRITE_NV_CONFIGURATION_REGISTER 0xB1
#define N25Q128_CMD_READ_V_CONF_REGISTER            0x85
#define N25Q128_CMD_WRITE_V_CONF_REGISTER           0x81
#define N25Q128_CMD_READ_ENHANCED_V_CONF_REGISTER   0x65
#define N25Q128_CMD_WRITE_ENHANCED_V_CONF_REGISTER  0x61
#define N25Q128_CMD_PAGE_PROGRAM                    0x02
#define N25Q128_CMD_SUBSECTOR_ERASE                 0x20
#define N25Q128_CMD_SECTOR_ERASE                    0xD8
#define N25Q128_CMD_BULK_ERASE                      0xC7
#define N25Q128_CMD_PROGRAM_ERASE_RESUME            0x7A
#define N25Q128_CMD_PROGRAM_ERASE_SUSPEND           0x75
#define N25Q128_CMD_READ_OTP_ARRAY                  0x4B
#define N25Q128_CMD_PROGRAM_OTP_ARRAY               0x42
/** @} */

/**
 * @name    Status register bits
 * @{
 */
#define N25Q128_STS_PROGRAM_ERASE                   0x80U
#define N25Q128_STS_ERASE_SUSPEND                   0x40U
#define N25Q128_STS_ERASE_ERROR                     0x20U
#define N25Q128_STS_PROGRAM_ERROR                   0x10U
#define N25Q128_STS_VPP_ERROR                       0x08U
#define N25Q128_STS_PROGRAM_SUSPEND                 0x04U
#define N25Q128_STS_PROTECTION_ERROR                0x02U
#define N25Q128_STS_RESERVED                        0x01U
#define N25Q128_STS_ALL_ERRORS              (N25Q128_STS_ERASE_ERROR |      \
                                             N25Q128_STS_PROGRAM_ERROR |    \
                                             N25Q128_STS_VPP_ERROR |        \
                                             N25Q128_STS_PROTECTION_ERROR)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   N25Q128 shared SPI switch.
 * @details If set to @p TRUE the device acquires SPI bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires SPI_USE_MUTUAL_EXCLUSION
 */
#if !defined(N25Q128_SHARED_SPI) || defined(__DOXYGEN__)
#define N25Q128_SHARED_SPI                  TRUE
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the flash waiting
 *          routines releasing some extra CPU time for the threads with
 *          lower priority, this may slow down the driver a bit however.
 *          This option is recommended also when the SPI driver does not
 *          use a DMA channel and heavily loads the CPU.
 */
#if !defined(N25Q128_NICE_WAITING) || defined(__DOXYGEN__)
#define N25Q128_NICE_WAITING                TRUE
#endif

/**
 * @brief   Uses 4kB sub-sectors rather than 64kB sectors.
 */
#if !defined(N25Q128_USE_SUB_SECTORS) || defined(__DOXYGEN__)
#define N25Q128_USE_SUB_SECTORS             FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !HAL_USE_SPI
#error "this module requires HAL_USE_SPI"
#endif

#if N25Q128_SHARED_SPI && !SPI_USE_MUTUAL_EXCLUSION
#error "N25Q128_SHARED_SPI requires SPI_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a N25Q128 configuration structure.
 */
typedef struct {
  /**
   * @brief   SPI driver associated to this L3GD20.
   */
  SPIDriver                 *spip;
  /**
   * @brief   SPI configuration associated to this L3GD20.
   */
  const SPIConfig           *spicfg;
} N25Q128Config;

/**
 * @brief   @p N25Q128 specific methods.
 */
#define _n25q128_methods                                                    \
  _base_flash_methods

/**
 * @extends BaseGyroscopeVMT
 *
 * @brief   @p L3GD20 virtual methods table.
 */
struct N25Q128DriverVMT {
  _n25q128_methods
};
  
/**
 * @extends BaseFlash
 *
 * @brief   Type of N25Q128 flash class.
 */
typedef struct {
  /**
   * @brief   BaseFlash Virtual Methods Table.
   */
  const struct N25Q128DriverVMT *vmt_baseflash;
  _base_flash_data
  /**
   * @brief   Current configuration data.
   */
  const N25Q128Config       *config;
} N25Q128Driver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void n25q128ObjectInit(N25Q128Driver *devp);
  void n25q128Start(N25Q128Driver *devp, const N25Q128Config *config);
  void n25q128Stop(N25Q128Driver *devp);
  void n25q128ReadId(N25Q128Driver *devp, uint8_t *rp, size_t n);
#ifdef __cplusplus
}
#endif

#endif /* N25Q128_H */

/** @} */

