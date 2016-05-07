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

#define PAGE_SIZE                           256U
#define PAGE_MASK                           (PAGE_SIZE - 1U)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
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

/**
 * @brief   N25Q128 descriptor.
 */
static flash_descriptor_t descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE | FLASH_ATTR_REWRITABLE,
  .page_size        = 256,
  .sectors_count    = 4096,
  .sectors          = NULL,
  .sectors_size     = 4096,
  .address          = 0
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void spi_send_cmd_addr(N25Q128Driver *devp,
                              uint8_t cmd,
                              flash_address_t addr) {
  uint8_t buf[4];

  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >> 8);
  buf[3] = (uint8_t)(addr >> 0);
  spiSend(devp->config->spip, 4, buf);
}

static const flash_descriptor_t *get_attributes(void *instance) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

  (void)instance;

  return &descriptor;
}

static flash_error_t erase_all(void *instance) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

  (void)instance;

  return FLASH_NO_ERROR;
}

static flash_error_t erase_sectors(void *instance,
                                   flash_sector_t sector,
                                   flash_sector_t n) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

  (void)instance;
  (void)sector;
  (void)n;

  return FLASH_NO_ERROR;
}

static flash_error_t are_sectors_erased(void *instance,
                                        flash_sector_t sector,
                                        flash_sector_t n) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

  (void)instance;
  (void)sector;
  (void)n;

  return FLASH_NO_ERROR;
}

static flash_error_t program(void *instance, flash_address_t addr,
                             const uint8_t *pp, size_t n) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

#if N25Q128_SHARED_SPI == TRUE
  spiAcquireBus(spip);
  spiStart(spip, devp->config->spicfg);
#endif
  devp->state = FLASH_ACTIVE;

  while (n > 0) {
    size_t chunk = (size_t)(((addr | PAGE_MASK) + 1U) - addr);
    if (chunk > n) {
      chunk = n;
    }

    spiSelect(spip);

    spi_send_cmd_addr(devp, N25Q128_CMD_PAGE_PROGRAM, addr);
    spiSend(spip, chunk, pp);

    spiUnselect(spip);

    addr += chunk;
    pp   += chunk;
    n    -= chunk;
  }

  devp->state = FLASH_READY;

#if N25Q128_SHARED_SPI == TRUE
  spiReleaseBus(spip);
#endif

  return FLASH_NO_ERROR;
}

static flash_error_t read(void *instance, flash_address_t addr,
                          uint8_t *rp, size_t n) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

#if N25Q128_SHARED_SPI == TRUE
  spiAcquireBus(spip);
  spiStart(spip, devp->config->spicfg);
#endif
  devp->state = FLASH_ACTIVE;

  spiSelect(spip);
  spi_send_cmd_addr(devp, N25Q128_CMD_READ, addr);
  spiReceive(spip, n, rp);
  spiUnselect(spip);

  devp->state = FLASH_READY;

#if N25Q128_SHARED_SPI == TRUE
  spiReleaseBus(spip);
#endif

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
  devp->state = FLASH_STOP;
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

  (void)config;

  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  if (devp->state == FLASH_STOP) {
#if N25Q128_SHARED_SPI == TRUE
    spiStart(devp->config->spip, devp->config->spicfg);
#endif
    devp->state = FLASH_READY;
  }
} 

/**
 * @brief   Deactivates the N25Q128 driver.
 *
 * @param[in] devp       pointer to the @p N25Q128Driver object
 *
 * @api
 */
void n15q128Stop(N25Q128Driver *devp) {
  SPIDriver *spip = devp->config->spip;

  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  if (devp->state != FLASH_STOP) {
#if N25Q128_SHARED_SPI == TRUE
    spiAcquireBus(spip);
#endif

    spiStop(spip);

#if N25Q128_SHARED_SPI == TRUE
    spiReleaseBus(spip);
#endif

    devp->state = FLASH_STOP;
  }
}

/** @} */
