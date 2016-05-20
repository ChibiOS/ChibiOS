/*
    ChibiOS - Copyright (C) 2016 Giovanni Di Sirio

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
 * @file    m25q.c
 * @brief   Micron serial flash driver code.
 *
 * @addtogroup m25q
 * @{
 */

#include "hal.h"

#include "m25q.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define PAGE_SIZE                           256U
#define PAGE_MASK                           (PAGE_SIZE - 1U)

#if M25Q_USE_SUB_SECTORS == TRUE
#define SECTOR_SIZE                         0x00001000U
#define CMD_SECTOR_ERASE                    M25Q_CMD_SUBSECTOR_ERASE
#else
#define SECTOR_SIZE                         0x00010000U
#define CMD_SECTOR_ERASE                    M25Q_CMD_SECTOR_ERASE
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
static flash_error_t read_id(void *instance, uint8_t *rp, size_t n);

/**
 * @brief   Virtual methods table.
 */
static const struct M25QDriverVMT m25q_vmt = {
  get_descriptor, read, program,
  start_erase_all, start_erase_sector, query_erase, verify_erase,
  read_id
};

/**
 * @brief   N25Q128 descriptor.
 */
static flash_descriptor_t descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE | FLASH_ATTR_REWRITABLE |
                      FLASH_ATTR_SUSPEND_ERASE_CAPABLE,
  .page_size        = 256U,
#if M25Q_USE_SUB_SECTORS == TRUE
  .sectors_count    = 4096U,
#else
  .sectors_count    = 256U,
#endif
  .sectors          = NULL,
  .sectors_size     = SECTOR_SIZE,
  .address          = 0U
};

#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
static const qspi_command_t cmd_read_id = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_READ_ID) |
#if M25Q_SWITCH_WIDTH == TRUE
                      QSPI_CFG_CMD_MODE_ONE_LINE     |
                      QSPI_CFG_DATA_MODE_ONE_LINE,
#else
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
                      QSPI_CFG_CMD_MODE_ONE_LINE   |
                      QSPI_CFG_DATA_MODE_ONE_LINE,
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
                      QSPI_CFG_CMD_MODE_TWO_LINES   |
                      QSPI_CFG_DATA_MODE_TWO_LINES,
#else
                      QSPI_CFG_CMD_MODE_FOUR_LINES   |
                      QSPI_CFG_DATA_MODE_FOUR_LINES,
#endif
#endif
  .addr             = 0,
  .alt              = 0
};
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if ((M25Q_BUS_MODE != M25Q_BUS_MODE_SPI) && (M25Q_SHARED_BUS == TRUE)) ||  \
    defined(__DOXYGEN__)
static void flash_bus_acquire(M25QDriver *devp) {

  qspiAcquireBus(devp->config->qspip);
}

static void flash_bus_release(M25QDriver *devp) {

  qspiReleaseBus(devp->config->qspip);
}
#elif (M25Q_BUS_MODE == M25Q_BUS_MODE_SPI) && (M25Q_SHARED_BUS == TRUE)
static void flash_bus_acquire(M25QDriver *devp) {

  spiAcquireBus(devp->config->spip);
  spiStart(devp->config->spip, devp->config->spicfg);
}

static void flash_bus_release(M25QDriver *devp) {

  spiReleaseBus(devp->config->spip);
}
#else
#define flash_bus_acquire(devp)
#define flash_bus_release(devp)
#endif

static void flash_short_cmd(M25QDriver *devp, uint8_t cmd) {

#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = devp->qspi_mode | QSPI_CFG_CMD(cmd);
#else
  uint8_t buf[1];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  spiSend(devp->config->spip, 1, buf);
  spiUnselect(devp->config->spip);
#endif
}

static void flash_send_cmd(M25QDriver *devp, uint8_t cmd) {
  uint8_t buf[1];

  buf[0] = cmd;
//  spiSend(devp->config->spip, 1, buf);
}

static void flash_send_cmd_addr(M25QDriver *devp,
                                uint8_t cmd,
                                flash_address_t addr) {
  uint8_t buf[4];

  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >> 8);
  buf[3] = (uint8_t)(addr >> 0);
//  spiSend(devp->config->spip, 4, buf);
}

static flash_error_t flash_poll_status(M25QDriver *devp) {

  return FLASH_NO_ERROR;
}

static const flash_descriptor_t *get_descriptor(void *instance) {
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state != FLASH_UNINIT) && (devp->state != FLASH_STOP),
                "invalid state");

  return &descriptor;
}

static flash_error_t read(void *instance, flash_address_t addr,
                          uint8_t *rp, size_t n) {

  return FLASH_NO_ERROR;
}

static flash_error_t program(void *instance, flash_address_t addr,
                             const uint8_t *pp, size_t n) {

  return FLASH_NO_ERROR;
}

static flash_error_t start_erase_all(void *instance) {

  return FLASH_NO_ERROR;
}

static flash_error_t start_erase_sector(void *instance, flash_sector_t sector) {

  return FLASH_NO_ERROR;
}

static flash_error_t verify_erase(void *instance, flash_sector_t sector) {

  return FLASH_NO_ERROR;
}

static flash_error_t query_erase(void *instance, uint32_t *msec) {

  return FLASH_NO_ERROR;
}

static flash_error_t read_id(void *instance, uint8_t *rp, size_t n) {

  (void)instance;
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
 * @param[out] devp     pointer to the @p M25QDriver object
 *
 * @init
 */
void m25qObjectInit(M25QDriver *devp) {

  osalDbgCheck(devp != NULL);

  devp->vmt         = &m25q_vmt;
  devp->state       = FLASH_STOP;
  devp->config      = NULL;
#if (M25Q_BUS_MODE != M25Q_BUS_MODE_SPI) || defined(__DOXYGEN__)
  devp->qspi_mode   = 0U;
#endif
}

/**
 * @brief   Configures and activates N25Q128 driver.
 *
 * @param[in] devp      pointer to the @p M25QDriver object
 * @param[in] config    pointer to the configuration
 *
 * @api
 */
void m25qStart(M25QDriver *devp, const M25QConfig *config) {

  osalDbgCheck((devp != NULL) && (config != NULL));
  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  devp->config = config;

  if (devp->state == FLASH_STOP) {
    uint8_t id[3];

    /* Bus acquisition.*/
    flash_bus_acquire(devp);

#if M25Q_BUS_MODE == M25Q_BUS_MODE_SPI
    spiStart(devp->config->spip, devp->config->spicfg);
#else
    qspiStart(devp->config->qspip, devp->config->qspicfg);
#endif

#if M25Q_BUS_MODE == M25Q_BUS_MODE_SPI
#else
    qspiSend(devp->config->qspip, &cmd_read_id, 3, id);
#endif

    /* Reset Enable command.*/
//    flash_short_cmd(devp, M25Q_CMD_RESET_ENABLE);

    /* Reset Memory command.*/
//    flash_short_cmd(devp, M25Q_CMD_RESET_MEMORY);

    devp->state = FLASH_READY;

    /* Bus release.*/
    flash_bus_release(devp);
  }
} 

/**
 * @brief   Deactivates the N25Q128 driver.
 *
 * @param[in] devp       pointer to the @p M25QDriver object
 *
 * @api
 */
void m25qStop(M25QDriver *devp) {

  osalDbgCheck(devp != NULL);
  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  if (devp->state != FLASH_STOP) {

    /* Bus acquisition.*/
    flash_bus_acquire(devp);

#if M25Q_BUS_MODE == M25Q_BUS_MODE_SPI
    spiStop(devp->config->spip);
#else
    qspiStop(devp->config->qspip);
#endif

    devp->config = NULL;
    devp->state = FLASH_STOP;

    /* Bus release.*/
    flash_bus_release(devp);
  }
}

/** @} */
