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
 * @file    n25q128.c
 * @brief   N25Q128 flash interface module code.
 *
 * @addtogroup n25q128
 * @{
 */

#include "hal.h"

#include "n25q128.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

static const flash_descriptor_t *get_attributes(void *instance);
static flash_error_t erase_all(void *instance);
static flash_error_t erase_sectors(void *instance,
                                   flash_sector_t sector,
                                   flash_sector_t n);
static flash_error_t are_sectors_erased(void *instance,
                                        flash_sector_t sector,
                                        flash_sector_t n);
static flash_error_t program(void *instance, flash_address_t addr,
                             const uint8_t *pp, size_t n);
static flash_error_t read(void *instance, flash_address_t addr,
                          uint8_t *rp, size_t n);

/**
 * @brief   Virtual methods table.
 */
static const struct N25Q128DriverVMT n25q128_vmt = {
  get_attributes, erase_all, erase_sectors, are_sectors_erased,
  program, read
};

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static const flash_descriptor_t *get_attributes(void *instance) {

  (void)instance;

  return FLASH_NO_ERROR;
}

static flash_error_t erase_all(void *instance) {

  (void)instance;

  return FLASH_NO_ERROR;
}

static flash_error_t erase_sectors(void *instance,
                                   flash_sector_t sector,
                                   flash_sector_t n) {

  (void)instance;
  (void)sector;
  (void)n;

  return FLASH_NO_ERROR;
}

static flash_error_t are_sectors_erased(void *instance,
                                        flash_sector_t sector,
                                        flash_sector_t n) {

  (void)instance;
  (void)sector;
  (void)n;

  return FLASH_NO_ERROR;
}

static flash_error_t program(void *instance, flash_address_t addr,
                             const uint8_t *pp, size_t n) {

  (void)instance;
  (void)addr;
  (void)pp;
  (void)n;

  return FLASH_NO_ERROR;
}

static flash_error_t read(void *instance, flash_address_t addr,
                          uint8_t *rp, size_t n) {

  (void)instance;
  (void)addr;
  (void)rp;
  (void)n;

  return FLASH_NO_ERROR;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] devp     pointer to the @p N25Q128Driver object
 *
 * @init
 */
void n15q128ObjectInit(N25Q128Driver *devp) {

  devp->vmt_baseflash = &n25q128_vmt;
  devp->config = NULL;
}

/**
 * @brief   Configures and activates N25Q128 driver.
 *
 * @param[in] devp      pointer to the @p N25Q128Driver object
 * @param[in] config    pointer to the configuration
 *
 * @api
 */
void n15q128Start(N25Q128Driver *devp, const N25Q128Config *config) {

  (void)devp;
  (void)config;
} 

/**
 * @brief   Deactivates the N25Q128 driver.
 *
 * @param[in] devp       pointer to the @p N25Q128Driver object
 *
 * @api
 */
void n15q128Stop(N25Q128Driver *devp) {

  (void)devp;
}

/** @} */
