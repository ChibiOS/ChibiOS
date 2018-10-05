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
 * @file    serial_nor.h
 * @brief   Serial NOR driver header.
 *
 * @addtogroup SERIAL_NOR
 * @ingroup SERIAL_NOR
 * @{
 */

#ifndef SERIAL_NOR_H
#define SERIAL_NOR_H

#include "hal_flash.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Bus interface modes.
 * @{
 */
#define SNOR_BUS_MODE_SPI                   0U
#define SNOR_BUS_MODE_WSPI1L                1U
#define SNOR_BUS_MODE_WSPI2L                2U
#define SNOR_BUS_MODE_WSPI4L                4U
#define SNOR_BUS_MODE_WSPI8L                8U
/** @} */

#define SNOR_BUS_CMD_EXTENDED_ADDRESSING    0x80000000U

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
#if !defined(SNOR_BUS_MODE) || defined(__DOXYGEN__)
#define SNOR_BUS_MODE                       SNOR_BUS_MODE_WSPI4L
#endif

/**
 * @brief   Shared bus switch.
 * @details If set to @p TRUE the device acquires bus ownership
 *          on each transaction.
 * @note    Requires @p SPI_USE_MUTUAL_EXCLUSION or
 *          @p WSPI_USE_MUTUAL_EXCLUSION depending on mode selected
 *          with @p SNOR_BUS_MODE.
 */
#if !defined(SNOR_SHARED_BUS) || defined(__DOXYGEN__)
#define SNOR_SHARED_BUS                     TRUE
#endif

/**
 * @brief   Number of dummy cycles for fast read (1..15).
 * @details This is the number of dummy cycles to be used for fast read
 *          operations.
 * TODO: Should be handled in LLD.
 */
#if !defined(SNOR_READ_DUMMY_CYCLES) || defined(__DOXYGEN__)
#define SNOR_READ_DUMMY_CYCLES              8
#endif

/**
 * @brief   Switch QSPI bus width on initialization.
 * @details A bus width initialization is performed by writing the
 *          Enhanced Volatile Configuration Register. If the flash
 *          device is configured using the Non Volatile Configuration
 *          Register then this option is not required.
 * @note    This option is only valid in QSPI bus modes.
 * TODO: Should go in LLD.
 */
#if !defined(SNOR_SWITCH_WIDTH) || defined(__DOXYGEN__)
#define SNOR_SWITCH_WIDTH                   TRUE
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the flash waiting
 *          routines releasing some extra CPU time for threads with lower
 *          priority, this may slow down the driver a bit however.
 * TODO: Should go in LLD.
 */
#if !defined(SNOR_NICE_WAITING) || defined(__DOXYGEN__)
#define SNOR_NICE_WAITING                   TRUE
#endif

/**
 * @brief   Uses 4kB sub-sectors rather than 64kB sectors.
 * TODO: Should go in LLD.
 */
#if !defined(SNOR_USE_SUB_SECTORS) || defined(__DOXYGEN__)
#define SNOR_USE_SUB_SECTORS                FALSE
#endif

/**
 * @brief   Size of the compare buffer.
 * @details This buffer is allocated in the stack frame of the function
 *          @p flashVerifyErase() and its size must be a power of two.
 *          Larger buffers lead to better verify performance but increase
 *          stack usage for that function.
 * TODO: Should go in LLD.
 */
#if !defined(SNOR_COMPARE_BUFFER_SIZE) || defined(__DOXYGEN__)
#define SNOR_COMPARE_BUFFER_SIZE            32
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SNOR_BUS_MODE != SNOR_BUS_MODE_SPI) || defined(__DOXYGEN__)
#define BUSConfig WSPIConfig
#define BUSDriver WSPIDriver
#else
#define BUSConfig SPIConfig
#define BUSDriver SPIDriver
#endif

#if (SNOR_READ_DUMMY_CYCLES < 1) || (SNOR_READ_DUMMY_CYCLES > 15)
#error "invalid SNOR_READ_DUMMY_CYCLES value (1..15)"
#endif

#if (SNOR_COMPARE_BUFFER_SIZE & (SNOR_COMPARE_BUFFER_SIZE - 1)) != 0
#error "invalid SNOR_COMPARE_BUFFER_SIZE value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a SNOR configuration structure.
 */
typedef struct {
  BUSDriver                 *busp;
  const BUSConfig           *buscfg;
} SNORConfig;

/**
 * @brief   @p SNORDriver specific methods.
 */
#define _snor_flash_methods_alone                                           \
  /* Read SFDP.*/                                                           \
  flash_error_t (*read_sfdp)(void *instance,                                \
                 flash_offset_t offset,                                     \
                 size_t n,                                                  \
                 uint8_t *rp);

/**
 * @brief   @p SNORDriver specific methods with inherited ones.
 */
#define _snor_flash_methods                                                 \
  _base_flash_methods                                                       \
  _snor_flash_methods_alone

/**
 * @extends BaseFlashVMT
 *
 * @brief   @p SNOR virtual methods table.
 */
struct SNORDriverVMT {
  _snor_flash_methods
};
  
/**
 * @extends BaseFlash
 *
 * @brief   Type of SNOR flash class.
 */
typedef struct {
  /**
   * @brief   SNORDriver Virtual Methods Table.
   */
  const struct SNORDriverVMT    *vmt;
  _base_flash_data
  /**
   * @brief   Current configuration data.
   */
  const SNORConfig              *config;
  /**
   * @brief   Device ID and unique ID.
   */
  uint8_t                       device_id[20];
} SNORDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void bus_cmd(BUSDriver *busp, uint32_t cmd);
  void bus_cmd_send(BUSDriver *busp, uint32_t cmd, size_t n, const uint8_t *p);
  void bus_cmd_receive(BUSDriver *busp,
                       uint32_t cmd,
                       size_t n,
                       uint8_t *p);
  void bus_cmd_addr(BUSDriver *busp, uint32_t cmd, flash_offset_t offset);
  void bus_cmd_addr_send(BUSDriver *busp,
                         uint32_t cmd,
                         flash_offset_t offset,
                         size_t n,
                         const uint8_t *p);
  void bus_cmd_addr_receive(BUSDriver *busp,
                            uint32_t cmd,
                            flash_offset_t offset,
                            size_t n,
                            uint8_t *p);
  void bus_cmd_addr_dummy_receive(BUSDriver *busp,
                                  uint32_t cmd,
                                  flash_offset_t offset,
                                  uint32_t dummy,
                                  size_t n,
                                  uint8_t *p);
  void snorObjectInit(SNORDriver *devp);
  void snorStart(SNORDriver *devp, const SNORConfig *config);
  void snorStop(SNORDriver *devp);
#if (SNOR_BUS_MODE != SNOR_BUS_MODE_SPI) || defined(__DOXYGEN__)
#if (WSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
  void snorMemoryMap(SNORDriver *devp, uint8_t ** addrp);
  void snorMemoryUnmap(SNORDriver *devp);
#endif /* QSPI_SUPPORTS_MEMMAP == TRUE */
#endif /* SNOR_BUS_MODE != SNOR_BUS_MODE_SPI */
#ifdef __cplusplus
}
#endif

/* Device-specific implementations.*/
#include "flash_device.h"

#endif /* SERIAL_NOR_H */

/** @} */

