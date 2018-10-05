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
 * @file    flash_device.h
 * @brief   Micron N25Q serial flash driver header.
 *
 * @addtogroup MICRON_N25Q
 * @ingroup MICRON_N25Q
 * @{
 */

#ifndef FLASH_DEVICE_H
#define FLASH_DEVICE_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Device identification
 * @{
 */
#define N25Q_SUPPORTED_MANUFACTURE_IDS      {0x20}
#define N25Q_SUPPORTED_MEMORY_TYPE_IDS      {0xBA, 0xBB}
/** @} */

/**
 * @name    Command codes
 * @{
 */
#define N25Q_CMD_RESET_ENABLE                       0x66
#define N25Q_CMD_RESET_MEMORY                       0x99
#define N25Q_CMD_READ_ID                            0x9F
#define N25Q_CMD_MULTIPLE_IO_READ_ID                0xAF
#define N25Q_CMD_READ_DISCOVERY_PARAMETER           0x5A
#define N25Q_CMD_READ                               0x03
#define N25Q_CMD_FAST_READ                          0x0B
#define N25Q_CMD_WRITE_ENABLE                       0x06
#define N25Q_CMD_WRITE_DISABLE                      0x04
#define N25Q_CMD_READ_STATUS_REGISTER               0x05
#define N25Q_CMD_WRITE_STATUS_REGISTER              0x01
#define N25Q_CMD_READ_LOCK_REGISTER                 0xE8
#define N25Q_CMD_WRITE_LOCK_REGISTER                0xE5
#define N25Q_CMD_READ_FLAG_STATUS_REGISTER          0x70
#define N25Q_CMD_CLEAR_FLAG_STATUS_REGISTER         0x50
#define N25Q_CMD_READ_NV_CONFIGURATION_REGISTER     0xB5
#define N25Q_CMD_WRITE_NV_CONFIGURATION_REGISTER    0xB1
#define N25Q_CMD_READ_V_CONF_REGISTER               0x85
#define N25Q_CMD_WRITE_V_CONF_REGISTER              0x81
#define N25Q_CMD_READ_ENHANCED_V_CONF_REGISTER      0x65
#define N25Q_CMD_WRITE_ENHANCED_V_CONF_REGISTER     0x61
#define N25Q_CMD_PAGE_PROGRAM                       0x02
#define N25Q_CMD_SUBSECTOR_ERASE                    0x20
#define N25Q_CMD_SECTOR_ERASE                       0xD8
#define N25Q_CMD_BULK_ERASE                         0xC7
#define N25Q_CMD_PROGRAM_ERASE_RESUME               0x7A
#define N25Q_CMD_PROGRAM_ERASE_SUSPEND              0x75
#define N25Q_CMD_READ_OTP_ARRAY                     0x4B
#define N25Q_CMD_PROGRAM_OTP_ARRAY                  0x42
/** @} */

/**
 * @name    Flags status register bits
 * @{
 */
#define N25Q_FLAGS_PROGRAM_ERASE                    0x80U
#define N25Q_FLAGS_ERASE_SUSPEND                    0x40U
#define N25Q_FLAGS_ERASE_ERROR                      0x20U
#define N25Q_FLAGS_PROGRAM_ERROR                    0x10U
#define N25Q_FLAGS_VPP_ERROR                        0x08U
#define N25Q_FLAGS_PROGRAM_SUSPEND                  0x04U
#define N25Q_FLAGS_PROTECTION_ERROR                 0x02U
#define N25Q_FLAGS_RESERVED                         0x01U
#define N25Q_FLAGS_ALL_ERRORS                   (N25Q_FLAGS_ERASE_ERROR |   \
                                                 N25Q_FLAGS_PROGRAM_ERROR | \
                                                 N25Q_FLAGS_VPP_ERROR |     \
                                                 N25Q_FLAGS_PROTECTION_ERROR)
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
extern flash_descriptor_t snor_descriptor;
#endif

#ifdef __cplusplus
extern "C" {
#endif
void snor_device_init(SNORDriver *devp);
const flash_descriptor_t *snor_get_descriptor(void *instance);
flash_error_t snor_device_read(SNORDriver *devp, flash_offset_t offset,
                               size_t n, uint8_t *rp);
flash_error_t snor_device_program(SNORDriver *devp, flash_offset_t offset,
                                  size_t n, const uint8_t *pp);
flash_error_t snor_device_start_erase_all(SNORDriver *devp);
flash_error_t snor_device_start_erase_sector(SNORDriver *devp,
                                             flash_sector_t sector);
flash_error_t snor_device_verify_erase(SNORDriver *devp,
                                       flash_sector_t sector);
flash_error_t snor_device_query_erase(SNORDriver *devp, uint32_t *msec);
flash_error_t snor_device_read_sfdp(SNORDriver *devp, flash_offset_t offset,
                                    size_t n, uint8_t *rp);
#if SNOR_BUS_MODE != SNOR_BUS_MODE_SPI
  void snor_activate_xip(SNORDriver *devp);
  void snor_reset_xip(SNORDriver *devp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* FLASH_DEVICE_H */

/** @} */

