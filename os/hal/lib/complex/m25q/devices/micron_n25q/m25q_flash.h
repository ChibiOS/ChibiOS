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
 * @file    n25q_flash.h
 * @brief   Micron N25Q serial flash driver header.
 *
 * @addtogroup MICRON_N25Q
 * @ingroup M25Q
 * @{
 */

#ifndef MICRON_N25Q_H
#define MICRON_N25Q_H

#include "hal_jesd216_flash.h"
#include "m25q_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Device identification
 * @{
 */
#define M25Q_SUPPORTED_MANUFACTURE_IDS      {0x20}
#define M25Q_SUPPORTED_MEMORY_TYPE_IDS      {0xBA, 0xBB}
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

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern flash_descriptor_t m25q_descriptor;
#endif

#ifdef __cplusplus
extern "C" {
#endif
void m25q_device_init(M25QDriver *devp);
const flash_descriptor_t *m25q_get_descriptor(void *instance);
flash_error_t m25q_device_read(M25QDriver *devp, flash_offset_t offset,
                               size_t n, uint8_t *rp);
flash_error_t m25q_device_program(M25QDriver *devp, flash_offset_t offset,
                                  size_t n, const uint8_t *pp);
flash_error_t m25q_device_start_erase_all(M25QDriver *devp);
flash_error_t m25q_device_start_erase_sector(M25QDriver *devp,
                                             flash_sector_t sector);
flash_error_t m25q_device_verify_erase(M25QDriver *devp,
                                       flash_sector_t sector);
flash_error_t m25q_device_query_erase(M25QDriver *devp, uint32_t *msec);
flash_error_t m25q_device_read_sfdp(M25QDriver *devp, flash_offset_t offset,
                                    size_t n, uint8_t *rp);
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  void m25q_activate_xip(M25QDriver *devp);
  void m25q_reset_xip(M25QDriver *devp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* MICRON_N25Q_H */

/** @} */

