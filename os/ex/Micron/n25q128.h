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
 * @file    n25q128.h
 * @brief   N25Q128 flash interface module header.
 *
 * @{
 */

#ifndef N25Q128_H
#define N25Q128_H

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
/**
 * @brief   N25Q128 shared SPI switch.
 * @details If set to @p TRUE the device acquires SPI bus ownership
 *          on each transaction.
 * @note    The default is @p FALSE. Requires SPI_USE_MUTUAL_EXCLUSION
 */
#if !defined(N25Q128_SHARED_SPI) || defined(__DOXYGEN__)
#define N25Q128_SHARED_SPI                  FALSE
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
   * @brief SPI driver associated to this L3GD20.
   */
  SPIDriver                 *spip;
  /**
   * @brief SPI configuration associated to this L3GD20.
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
 * @brief   @p N25Q128Driver specific data.
 */
#define _n25q128_data                                                        \
  _base_flash_data
  
/**
 * @extends BaseFlash
 *
 * @brief   Type of N25Q128 flash class.
 */
typedef struct {
  /** @brief BaseSensor Virtual Methods Table. */
  const struct N25Q128DriverVMT *vmt_baseflash;
  _n25q128_data
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
  void n15q128ObjectInit(N25Q128Driver *devp);
  void n15q128Start(N25Q128Driver *devp, const N25Q128Config *config);
  void n15q128Stop(N25Q128Driver *devp);
#ifdef __cplusplus
}
#endif

#endif /* N25Q128_H */

/** @} */

