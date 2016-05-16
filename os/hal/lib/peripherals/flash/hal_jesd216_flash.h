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

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common command codes
 * @{
 */
#define JESD216_CMD_READ_ID                         0x9F
#define JESD216_CMD_READ                            0x03
#define JESD216_CMD_WRITE_ENABLE                    0x06
#define JESD216_CMD_WRITE_DISABLE                   0x04
#define JESD216_CMD_READ_STATUS_REGISTER            0x05
#define JESD216_CMD_WRITE_STATUS_REGISTER           0x01
#define JESD216_CMD_PAGE_PROGRAM                    0x02
#define JESD216_CMD_ERASE_4K                        0x20
#define JESD216_CMD_ERASE_BULK                      0xC7
#define JESD216_CMD_PROGRAM_ERASE_RESUME            0x7A
#define JESD216_CMD_PROGRAM_ERASE_SUSPEND           0x75
#define JESD216_CMD_READ_OTP_ARRAY                  0x4B
#define JESD216_CMD_PROGRAM_OTP_ARRAY               0x42
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

/**
 * @brief   @p JESD215Flash specific methods.
 * @note    No methods so far, just a common ancestor interface.
 */
#define _jesd216_flash_methods_alone                                        \
  /* Read SFDP.*/                                                           \
  void (*read_id)(void *instance, uint8_t *rp, size_t max);

/**
 * @brief   @p JESD215Flash specific methods with inherited ones.
 */
#define _jesd216_flash_methods                                              \
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
#define _jesd216_flash_data

/**
 * @brief   Base flash class.
 */
typedef struct {
  /** @brief Virtual Methods Table.*/
  const struct JESD215FlashVMT *vmt_jesd216flash;
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

#ifdef __cplusplus
}
#endif

#endif /* HAL_JESD216_FLASH_H */

/** @} */
