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
 * @addtogroup m25q
 * @{
 */

#ifndef M25Q_H
#define M25Q_H

#include "hal_jesd216_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Command codes
 * @{
 */
#define M25Q_CMD_RESET_ENABLE                       0x66
#define M25Q_CMD_RESET_MEMORY                       0x99
#define M25Q_CMD_READ_ID                            0x9F
#define M25Q_CMD_READ_DISCOVERY_PARAMETER           0x5A
#define M25Q_CMD_READ                               0x03
#define M25Q_CMD_FAST_READ                          0x08
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

/**
 * @name    Bus interface.
 * @{
 */
#define M25Q_BUS_MODE_SPI                   0
#define M25Q_BUS_MODE_QSPI1L                1
#define M25Q_BUS_MODE_QSPI2L                2
#define M25Q_BUS_MODE_QSPI4L                4
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Physical transport interface.
 */
#if !defined(M25Q_USE_SPI) || defined(__DOXYGEN__)
#define M25Q_BUS_MODE                       M25Q_BUS_MODE_QSPI4L
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
 * @brief   Shared SPI switch.
 * @details If set to @p TRUE the device acquires SPI bus ownership
 *          on each transaction.
 * @note    This option is only valid in QSPI bus modes.
 * @note    Requires @p SPI_USE_MUTUAL_EXCLUSION.
 */
#if !defined(M25Q_SHARED_SPI) || defined(__DOXYGEN__)
#define M25Q_SHARED_SPI                     TRUE
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
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (M25Q_BUS_MODE == M25Q_BUS_MODE_SPI) && (HAL_USE_SPI == FALSE)
#error "M25Q_BUS_MODE_SPI requires HAL_USE_SPI"
#endif

#if (M25Q_BUS_MODE != M25Q_BUS_MODE_SPI) && (HAL_USE_QSPI == FALSE)
#error "M25Q_BUS_MODE_QSPIxL requires HAL_USE_QSPI"
#endif

#if (M25Q_BUS_MODE == M25Q_BUS_MODE_SPI) &&                                 \
    (M25Q_SHARED_SPI == TRUE) &&                                            \
    (SPI_USE_MUTUAL_EXCLUSION == FALSE)
#error "M25Q_SHARED_SPI requires SPI_USE_MUTUAL_EXCLUSION"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a M25Q configuration structure.
 */
typedef struct {
#if (M25Q_BUS_MODE != M25Q_BUS_MODE_SPI) || defined(__DOXYGEN__)
  /**
   * @brief   QSPI driver associated to this instance.
   */
  QSPIDriver                *qspip;
  /**
   * @brief   QSPI configuration associated to this instance.
   */
  const QSPIConfig          *qspicfg;
#else
  /**
   * @brief   SPI driver associated to this instance.
   */
  SPIDriver                 *spip;
  /**
   * @brief   SPI configuration associated to this instance.
   */
  const SPIConfig           *spicfg;
#endif
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
  const struct JESD216FlashVMT  *vmt_baseflash;
  _jesd216_flash_data
  /**
   * @brief   Current configuration data.
   */
  const M25QConfig              *config;
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
#ifdef __cplusplus
}
#endif

#endif /* M25Q_H */

/** @} */

