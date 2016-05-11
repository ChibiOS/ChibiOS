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
 * @file    n25q128_spi.c
 * @brief   N25Q128 over SPI driver code.
 *
 * @addtogroup n25q128_spi
 * @{
 */

#include "hal.h"

#include "n25q128_spi.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define PAGE_SIZE                           256U
#define PAGE_MASK                           (PAGE_SIZE - 1U)

#if N25Q128_USE_SUB_SECTORS == TRUE
#define SECTOR_SIZE                         0x00001000U
#define CMD_SECTOR_ERASE                    N25Q128_CMD_SUBSECTOR_ERASE
#else
#define SECTOR_SIZE                         0x00010000U
#define CMD_SECTOR_ERASE                    N25Q128_CMD_SECTOR_ERASE
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const flash_descriptor_t *get_descriptor(void *instance);
static flash_error_t read(void *instance, flash_address_t addr,
                          uint8_t *rp, size_t n);
static flash_error_t program(void *instance, flash_address_t addr,
                             const uint8_t *pp, size_t n);
static flash_error_t start_erase_all(void *instance);
static flash_error_t start_erase_sector(void *instance, flash_sector_t sector);
static flash_error_t query_erase(void *instance, uint32_t *msec);
static flash_error_t verify_erase(void *instance, flash_sector_t sector);

/**
 * @brief   Virtual methods table.
 */
static const struct N25Q128DriverVMT n25q128_vmt = {
  get_descriptor, read, program,
  start_erase_all, start_erase_sector, query_erase, verify_erase
};

/**
 * @brief   N25Q128 descriptor.
 */
static flash_descriptor_t descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE | FLASH_ATTR_REWRITABLE |
                      FLASH_ATTR_SUSPEND_ERASE_CAPABLE,
  .page_size        = 256U,
#if N25Q128_USE_SUB_SECTORS == TRUE
  .sectors_count    = 4096U,
#else
  .sectors_count    = 256U,
#endif
  .sectors          = NULL,
  .sectors_size     = SECTOR_SIZE,
  .address          = 0U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if N25Q128_SHARED_SPI == TRUE
void flash_bus_acquire(N25Q128Driver *devp) {

  spiAcquireBus(devp->config->spip);
  spiStart(devp->config->spip, devp->config->spicfg);
}

void flash_bus_release(N25Q128Driver *devp) {

  spiReleaseBus(devp->config->spip);
}
#else
#define flash_bus_acquire(devp)
#define flash_bus_release(devp)
#endif

static void flash_short_cmd(N25Q128Driver *devp, uint8_t cmd) {
  uint8_t buf[1];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  spiSend(devp->config->spip, 1, buf);
  spiUnselect(devp->config->spip);
}

static void flash_send_cmd(N25Q128Driver *devp, uint8_t cmd) {
  uint8_t buf[1];

  buf[0] = cmd;
  spiSend(devp->config->spip, 1, buf);
}

static void flash_send_cmd_addr(N25Q128Driver *devp,
                              uint8_t cmd,
                              flash_address_t addr) {
  uint8_t buf[4];

  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >> 8);
  buf[3] = (uint8_t)(addr >> 0);
  spiSend(devp->config->spip, 4, buf);
}

static flash_error_t flash_poll_status(N25Q128Driver *devp) {
  SPIDriver *spip = devp->config->spip;
  uint8_t sts;

  do {
#if N25Q128_NICE_WAITING == TRUE
    osalThreadSleepMilliseconds(1);
#endif
    /* Read status command.*/
    spiSelect(spip);
    flash_send_cmd(devp, N25Q128_CMD_READ_FLAG_STATUS_REGISTER);
    spiReceive(spip, 1, &sts);
    spiUnselect(spip);
  } while ((sts & N25Q128_STS_PROGRAM_ERASE) == 0U);

  /* Checking for errors.*/
  if ((sts & N25Q128_STS_ALL_ERRORS) != 0U) {
    /* Clearing status register.*/
    flash_send_cmd(devp, N25Q128_CMD_CLEAR_FLAG_STATUS_REGISTER);

    /* Program operation failed.*/
    return FLASH_ERROR_PROGRAM;
  }

  return FLASH_NO_ERROR;
}

static const flash_descriptor_t *get_descriptor(void *instance) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state != FLASH_UNINIT) && (devp->state != FLASH_STOP),
                "invalid state");

  return &descriptor;
}

static flash_error_t read(void *instance, flash_address_t addr,
                          uint8_t *rp, size_t n) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)addr + n <= (size_t)descriptor.sectors_count *
                                   (size_t)descriptor.sectors_size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  flash_bus_acquire(devp);
  devp->state = FLASH_READ;

  /* Read command.*/
  spiSelect(spip);
  flash_send_cmd_addr(devp, N25Q128_CMD_READ, addr);
  spiReceive(spip, n, rp);
  spiUnselect(spip);

  devp->state = FLASH_READY;
  flash_bus_release(devp);
  return FLASH_NO_ERROR;
}

static flash_error_t program(void *instance, flash_address_t addr,
                             const uint8_t *pp, size_t n) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgCheck((instance != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)addr + n <= (size_t)descriptor.sectors_count *
                                   (size_t)descriptor.sectors_size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  flash_bus_acquire(devp);
  devp->state = FLASH_PGM;

  /* Data is programmed page by page.*/
  while (n > 0U) {
    flash_error_t err;

    /* Data size that can be written in a single program page operation.*/
    size_t chunk = (size_t)(((addr | PAGE_MASK) + 1U) - addr);
    if (chunk > n) {
      chunk = n;
    }

    /* Enabling write operation.*/
    flash_short_cmd(devp, N25Q128_CMD_WRITE_ENABLE);
    (void) spiPolledExchange(spip, 0xFF);   /* One frame delay.*/

    /* Page program command.*/
    spiSelect(spip);
    flash_send_cmd_addr(devp, N25Q128_CMD_PAGE_PROGRAM, addr);
    spiSend(spip, chunk, pp);
    spiUnselect(spip);

    /* Wait for status and check errors.*/
    (void) spiPolledExchange(spip, 0xFF);   /* One frame delay.*/
    err = flash_poll_status(devp);
    if (err != FLASH_NO_ERROR) {
      flash_bus_release(devp);
      return err;
    }

    /* Next page.*/
    addr += chunk;
    pp   += chunk;
    n    -= chunk;
  }

  devp->state = FLASH_READY;
  flash_bus_release(devp);
  return FLASH_NO_ERROR;
}

static flash_error_t start_erase_all(void *instance) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  flash_bus_acquire(devp);
  devp->state = FLASH_ERASE;

  /* Enabling write operation.*/
  flash_short_cmd(devp, N25Q128_CMD_WRITE_ENABLE);
  (void) spiPolledExchange(spip, 0xFF);   /* One frame delay.*/

  /* Bulk erase command.*/
  flash_short_cmd(devp, N25Q128_CMD_BULK_ERASE);

  devp->state = FLASH_READY;
  flash_bus_release(devp);
  return FLASH_NO_ERROR;
}

static flash_error_t start_erase_sector(void *instance, flash_sector_t sector) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;
  flash_address_t addr = (flash_address_t)(sector * SECTOR_SIZE);

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  flash_bus_acquire(devp);
  devp->state = FLASH_ERASE;

  /* Enabling write operation.*/
  flash_short_cmd(devp, N25Q128_CMD_WRITE_ENABLE);
  (void) spiPolledExchange(spip, 0xFF);   /* One frame delay.*/

  /* Sector erase command.*/
  spiSelect(spip);
  flash_send_cmd_addr(devp, CMD_SECTOR_ERASE, addr);
  spiUnselect(spip);

  devp->state = FLASH_READY;
  flash_bus_release(devp);
  return FLASH_NO_ERROR;
}

static flash_error_t verify_erase(void *instance, flash_sector_t sector) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;
  unsigned i;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  flash_bus_acquire(devp);
  devp->state = FLASH_READ;

  /* Read command.*/
  spiSelect(spip);
  flash_send_cmd_addr(devp, N25Q128_CMD_READ, (size_t)(sector * SECTOR_SIZE));
  for (i = SECTOR_SIZE; i > 0U; i--) {
    if (spiPolledExchange(spip, 0xFF) != 0xFF) {
      flash_bus_release(devp);
      return FLASH_ERROR_VERIFY;
    }
  }
  spiUnselect(spip);

  devp->state = FLASH_READY;
  flash_bus_release(devp);
  return FLASH_NO_ERROR;
}

static flash_error_t query_erase(void *instance, uint32_t *msec) {
  N25Q128Driver *devp = (N25Q128Driver *)instance;
  SPIDriver *spip = devp->config->spip;
  uint8_t sts;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* If there is an erase in progress then the device must be checked.*/
  if (devp->state == FLASH_ERASE) {

    flash_bus_acquire(devp);

    /* Read status command.*/
    spiSelect(spip);
    flash_send_cmd(devp, N25Q128_CMD_READ_FLAG_STATUS_REGISTER);
    spiReceive(spip, 1, &sts);
    spiUnselect(spip);

    /* If the P/E bit is zero (busy) or the flash in a suspended state then
       report that the operation is still in progress.*/
    if (((sts & N25Q128_STS_PROGRAM_ERASE) == 0U) ||
        ((sts & N25Q128_STS_ERASE_SUSPEND) != 0U)) {
      flash_bus_release(devp);

      /* Recommended time before polling again, this is a simplified
         implementation.*/
      if (msec != NULL) {
        *msec = 1U;
      }

      return FLASH_BUSY_ERASING;
    }

    /* The device is ready to accept commands.*/
    devp->state = FLASH_READY;

    /* Checking for errors.*/
    if ((sts & N25Q128_STS_ALL_ERRORS) != 0U) {
      /* Clearing status register.*/
      flash_short_cmd(devp, N25Q128_CMD_CLEAR_FLAG_STATUS_REGISTER);

      /* Program operation failed.*/
      return FLASH_ERROR_ERASE;
    }

    flash_bus_release(devp);
  }

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
void n25q128ObjectInit(N25Q128Driver *devp) {

  osalDbgCheck(devp != NULL);

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
void n25q128Start(N25Q128Driver *devp, const N25Q128Config *config) {

  osalDbgCheck((devp != NULL) && (config != NULL));
  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  if (devp->state == FLASH_STOP) {
    SPIDriver *spip = config->spip;

    devp->config = config;
    flash_bus_acquire(devp);

    /* Reset Enable command.*/
    flash_short_cmd(devp, N25Q128_CMD_RESET_ENABLE);

    (void) spiPolledExchange(spip, 0xFF);   /* One frame delay.*/

    /* Reset Memory command.*/
    flash_short_cmd(devp, N25Q128_CMD_RESET_MEMORY);

    devp->state = FLASH_READY;
    flash_bus_release(devp);
  }
} 

/**
 * @brief   Deactivates the N25Q128 driver.
 *
 * @param[in] devp       pointer to the @p N25Q128Driver object
 *
 * @api
 */
void n25q128Stop(N25Q128Driver *devp) {
  SPIDriver *spip = devp->config->spip;

  osalDbgCheck(devp != NULL);
  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  if (devp->state != FLASH_STOP) {
    flash_bus_acquire(devp);

    spiStop(spip);

    devp->config = NULL;
    devp->state = FLASH_STOP;
    flash_bus_release(devp);
  }
}

/**
 * @brief   Reads the device identifier.
 *
 * @param[in] devp      pointer to the @p N25Q128Driver object
 * @param[in] rp        pointer to the read buffer
 * @param[in] n         number of bytes to read (1..17)
 *
 * @api
 */
void n25q128ReadId(N25Q128Driver *devp, uint8_t *rp, size_t n) {
  SPIDriver *spip = devp->config->spip;

  osalDbgCheck((devp != NULL) && (rp != NULL) && (n > 0U) && (n <= 17U));
  osalDbgAssert(devp->state == FLASH_READY, "invalid state");

  flash_bus_acquire(devp);
  devp->state = FLASH_READ;

  /* Read Id command.*/
  spiSelect(spip);
  flash_send_cmd(devp, N25Q128_CMD_READ_ID);
  spiReceive(spip, n, rp);
  spiUnselect(spip);

  devp->state = FLASH_READY;
  flash_bus_release(devp);
}

/** @} */
