/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    hal_jesd216_flash.h
 * @brief   JESD216 compliant flash driver class header.
 *
 * @addtogroup HAL_JESD216_FLASH
 * @{
 */

#ifndef HAL_JESD216_FLASH_H
#define HAL_JESD216_FLASH_H

#include "hal_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common command codes
 * @{
 */
#define JESD216_CMD_READ_ID                 0x9FU
#define JESD216_CMD_READ                    0x03U
#define JESD216_CMD_WRITE_ENABLE            0x06U
#define JESD216_CMD_WRITE_DISABLE           0x04U
#define JESD216_CMD_READ_STATUS_REGISTER    0x05U
#define JESD216_CMD_WRITE_STATUS_REGISTER   0x01U
#define JESD216_CMD_PAGE_PROGRAM            0x02U
#define JESD216_CMD_ERASE_4K                0x20U
#define JESD216_CMD_ERASE_BULK              0xC7U
#define JESD216_CMD_PROGRAM_ERASE_RESUME    0x7AU
#define JESD216_CMD_PROGRAM_ERASE_SUSPEND   0x75U
#define JESD216_CMD_READ_OTP_ARRAY          0x4BU
#define JESD216_CMD_PROGRAM_OTP_ARRAY       0x42U
/** @} */

/**
 * @name    Command options
 * @{
 */
#define JESD216_CMD_EXTENDED_ADDRESSING     0x80000000U
/** @} */

/**
 * @name    Bus interface.
 * @{
 */
#define JESD216_BUS_MODE_SPI                0U
#define JESD216_BUS_MODE_QSPI1L             1U
#define JESD216_BUS_MODE_QSPI2L             2U
#define JESD216_BUS_MODE_QSPI4L             4U
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
#if !defined(JESD216_BUS_MODE) || defined(__DOXYGEN__)
#define JESD216_BUS_MODE                    JESD216_BUS_MODE_QSPI4L
#endif

/**
 * @brief   Shared bus switch.
 * @details If set to @p TRUE the device acquires bus ownership
 *          on each transaction.
 * @note    Requires @p SPI_USE_MUTUAL_EXCLUSION or
 *          @p SPI_USE_MUTUAL_EXCLUSION.
 */
#if !defined(JESD216_SHARED_BUS) || defined(__DOXYGEN__)
#define JESD216_SHARED_BUS                  TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (JESD216_BUS_MODE == JESD216_BUS_MODE_SPI) && (HAL_USE_SPI == FALSE)
#error "JESD216_BUS_MODE_SPI requires HAL_USE_SPI"
#endif

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) && (HAL_USE_QSPI == FALSE)
#error "JESD216_BUS_MODE_QSPIxL requires HAL_USE_QSPI"
#endif

#if (JESD216_BUS_MODE == JESD216_BUS_MODE_SPI) &&                           \
    (JESD216_SHARED_BUS == TRUE) &&                                         \
    (SPI_USE_MUTUAL_EXCLUSION == FALSE)
#error "JESD216_SHARED_SPI requires SPI_USE_MUTUAL_EXCLUSION"
#endif

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) &&                           \
    (JESD216_BUS_MODE != JESD216_BUS_MODE_QSPI1L) &&                        \
    (JESD216_BUS_MODE != JESD216_BUS_MODE_QSPI2L) &&                        \
    (JESD216_BUS_MODE != JESD216_BUS_MODE_QSPI4L)
#error "invalid JESD216_BUS_MODE selected"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
#define BUSConfig QSPIConfig
#define BUSDriver QSPIDriver
#else
#define BUSConfig SPIConfig
#define BUSDriver SPIDriver
#endif

#define _jesd216_config                                                     \
  BUSDriver                 *busp;                                          \
  const BUSConfig           *buscfg;

/**
 * @brief   @p JESD215Flash specific methods.
 * @note    No methods so far, just a common ancestor interface.
 */
#define _jesd216_flash_methods_alone                                        \
  /* Read SFDP.*/                                                           \
  flash_error_t (*read_sfdp)(void *instance,                                \
                 flash_offset_t offset,                                     \
                 size_t n,                                                  \
                 uint8_t *rp);

/**
 * @brief   @p JESD215Flash specific methods with inherited ones.
 */
#define _jesd216_flash_methods                                              \
  _base_flash_methods                                                       \
  _jesd216_flash_methods_alone

/**
 * @brief   @p JESD215Flash virtual methods table.
 */
struct JESD215FlashVMT {
  _jesd216_flash_methods
};

/**
 * @brief   @p JESD215Flash specific data.
 */
#define _jesd216_flash_data                                                 \
  _base_flash_data

/**
 * @brief   Base flash class.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct JESD215FlashVMT *vmt;
  _jesd216_flash_data
} JESD215Flash;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions (BaseFlash)
 * @{
 */
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void jesd216_start(BUSDriver *busp, const BUSConfig *config);
  void jesd216_stop(BUSDriver *busp);
  void jesd216_cmd(BUSDriver *busp, uint32_t cmd);
  void jesd216_cmd_receive(BUSDriver *busp, uint32_t cmd,
                           size_t n, uint8_t *p);
  void jesd216_cmd_send(BUSDriver *busp, uint32_t cmd,
                        size_t n, const uint8_t *p);
  void jesd216_cmd_addr(BUSDriver *busp, uint32_t cmd, flash_offset_t offset);
  void jesd216_cmd_addr_send(BUSDriver *busp, uint32_t cmd,
                             flash_offset_t offset, size_t n, const uint8_t *p);
  void jesd216_cmd_addr_receive(BUSDriver *busp, uint32_t cmd,
                                flash_offset_t offset, size_t n, uint8_t *p);
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  void jesd216_cmd_addr_dummy_receive(BUSDriver *busp, uint32_t cmd,
                                      flash_offset_t offset, uint8_t dummy,
                                      size_t n, uint8_t *p);
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */
#if JESD216_SHARED_BUS == TRUE
  void jesd216_bus_acquire(BUSDriver *busp, const BUSConfig *config);
  void jesd216_bus_release(BUSDriver *busp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_JESD216_FLASH_H */

/** @} */
