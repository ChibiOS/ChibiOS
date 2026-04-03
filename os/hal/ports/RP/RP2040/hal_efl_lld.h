/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RP2040/hal_efl_lld.h
 * @brief   RP2040 Embedded Flash subsystem low level driver header.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#ifndef HAL_EFL_LLD_H
#define HAL_EFL_LLD_H

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Flash page size (minimum write unit).
 */
#define RP_FLASH_PAGE_SIZE                  256U

/**
 * @brief   Flash sector size (minimum erase unit).
 */
#define RP_FLASH_SECTOR_SIZE                4096U

/**
 * @brief   Flash sector erase command (JEDEC 0x20).
 */
#define RP_FLASH_CMD_SECTOR_ERASE          0x20U

/**
 * @brief   Flash 32KB block erase command (JEDEC 0x52).
 */
#define RP_FLASH_CMD_BLOCK_ERASE_32K        0x52U

/**
 * @brief   Flash 64KB block erase command (JEDEC 0xD8).
 */
#define RP_FLASH_CMD_BLOCK_ERASE_64K        0xD8U

/**
 * @brief   Flash 32KB block size.
 */
#define RP_FLASH_BLOCK_32K_SIZE             (32U * 1024U)

/**
 * @brief   Flash 64KB block size.
 */
#define RP_FLASH_BLOCK_64K_SIZE             65536U

/**
 * @brief   XIP base address.
 */
#define RP_FLASH_BASE                       0x10000000U

/**
 * @brief   SSI base address.
 */
#define RP_SSI_BASE                         0x18000000U

/**
 * @brief   XIP control base address.
 */
#define RP_XIP_CTRL_BASE                    0x14000000U

/**
 * @brief   Flash unique ID size in bytes.
 */
#define RP_FLASH_UNIQUE_ID_SIZE             8U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    RP2040 configuration options
 * @{
 */

/**
 * @brief   Total flash size in bytes.
 * @note    Default is 2MB for standard Raspberry Pi Pico.
 *          Override in mcuconf.h for boards with different flash sizes.
 */
#if !defined(RP_FLASH_SIZE) || defined(__DOXYGEN__)
#define RP_FLASH_SIZE                       (2U * 1024U * 1024U)
#endif

/**
 * @brief   Suggested wait time during erase operations polling.
 */
#if !defined(RP_FLASH_WAIT_TIME_MS) || defined(__DOXYGEN__)
#define RP_FLASH_WAIT_TIME_MS               1U
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness. */
#if !defined(RP_HAS_FLASH)
#error "RP_HAS_FLASH not defined in registry"
#endif

#if RP_HAS_FLASH != TRUE
#error "RP_HAS_FLASH is not TRUE"
#endif

/**
 * @brief   Number of sectors in flash.
 */
#define RP_FLASH_SECTORS_COUNT              (RP_FLASH_SIZE / RP_FLASH_SECTOR_SIZE)

/**
 * @brief   Number of 64KB blocks in flash.
 */
#define RP_FLASH_BLOCKS_COUNT               (RP_FLASH_SIZE / RP_FLASH_BLOCK_64K_SIZE)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

#include "rp_efl_lld.h"

/**
 * @brief   Low level fields of the embedded flash driver structure.
 */
#define efl_lld_driver_fields                                               \
  /* Pointer to SSI registers. */                                           \
  SSI_TypeDef                 *ssi;

/**
 * @brief   Low level fields of the embedded flash configuration structure.
 */
#define efl_lld_config_fields                                               \
  /* Dummy configuration, it is not needed.*/                               \
  uint32_t                    dummy;

/*===========================================================================*/
/* Application hooks.                                                        */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#if !defined(__DOXYGEN__)
extern EFlashDriver EFLD1;
#endif
#include "rp_efl_lld_api.inc"
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EFL == TRUE */

#endif /* HAL_EFL_LLD_H */

/** @} */
