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
 * @file    hal_flash.h
 * @brief   Flash compatibility facade header.
 *
 * @addtogroup HAL_FLASH
 * @{
 */

#ifndef HAL_FLASH_H
#define HAL_FLASH_H

#include "hal_flash_base.h"

/*===========================================================================*/
/* Compatibility macros.                                                    */
/*===========================================================================*/

/**
 * @brief   Instance getter.
 *
 * @param[in] ip                    Pointer to a flash driver instance.
 * @return                          Pointer to the embedded @p BaseFlash view.
 */
#define getBaseFlash(ip) ((BaseFlash *)(ip))

/**
 * @brief   Gets the flash descriptor structure.
 *
 * @param[in] ip                    Pointer to a @p BaseFlash or derived class.
 * @return                          A flash device descriptor.
 */
#define flashGetDescriptor(ip)                                              \
  flsGetDescriptor(&((BaseFlash *)(ip))->fls)

/**
 * @brief   Read operation.
 */
#define flashRead(ip, offset, n, rp)                                        \
  flsRead(&((BaseFlash *)(ip))->fls, offset, n, rp)

/**
 * @brief   Program operation.
 */
#define flashProgram(ip, offset, n, pp)                                     \
  flsProgram(&((BaseFlash *)(ip))->fls, offset, n, pp)

/**
 * @brief   Starts a whole-device erase operation.
 */
#define flashStartEraseAll(ip)                                              \
  flsStartEraseAll(&((BaseFlash *)(ip))->fls)

/**
 * @brief   Starts a sector erase operation.
 */
#define flashStartEraseSector(ip, sector)                                   \
  flsStartEraseSector(&((BaseFlash *)(ip))->fls, sector)

/**
 * @brief   Queries the driver for erase operation progress.
 */
#define flashQueryErase(ip, msec)                                           \
  flsQueryErase(&((BaseFlash *)(ip))->fls, msec)

/**
 * @brief   Returns the erase state of a sector.
 */
#define flashVerifyErase(ip, sector)                                        \
  flsVerifyErase(&((BaseFlash *)(ip))->fls, sector)

/**
 * @brief   Acquires exclusive access to flash.
 */
#define flashAcquireExclusive(ip)                                           \
  flsAcquireExclusive(&((BaseFlash *)(ip))->fls)

/**
 * @brief   Releases exclusive access to flash.
 */
#define flashReleaseExclusive(ip)                                           \
  flsReleaseExclusive(&((BaseFlash *)(ip))->fls)

#endif /* HAL_FLASH_H */

/** @} */
