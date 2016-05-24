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

#include <string.h>

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

static const uint8_t manufacturer_ids[] = M25Q_SUPPORTED_MANUFACTURE_IDS;
static const uint8_t memory_type_ids[] = M25Q_SUPPORTED_MEMORY_TYPE_IDS;

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
  .sectors_count    = 0U,           /* It is overwritten.*/
  .sectors          = NULL,
  .sectors_size     = SECTOR_SIZE,
  .address          = 0U
};

#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
/* Initial M25Q_CMD_READ_ID command.*/
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

/* Initial M25Q_CMD_WRITE_ENHANCED_V_CONF_REGISTER command.*/
static const qspi_command_t cmd_write_evconf = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_WRITE_ENHANCED_V_CONF_REGISTER) |
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

/* Initial M25Q_CMD_WRITE_ENABLE command.*/
static const qspi_command_t cmd_write_enable = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_WRITE_ENABLE) |
#if M25Q_SWITCH_WIDTH == TRUE
                      QSPI_CFG_CMD_MODE_ONE_LINE,
#else
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
                      QSPI_CFG_CMD_MODE_ONE_LINE,
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
                      QSPI_CFG_CMD_MODE_TWO_LINES,
#else
                      QSPI_CFG_CMD_MODE_FOUR_LINES,
#endif
#endif
  .addr             = 0,
  .alt              = 0
};

/* 1x M25Q_CMD_RESET_ENABLE command.*/
static const qspi_command_t cmd_reset_enable_1 = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_ENABLE) |
                      QSPI_CFG_CMD_MODE_ONE_LINE,
  .addr             = 0,
  .alt              = 0
};

/* 2x M25Q_CMD_RESET_ENABLE command.*/
static const qspi_command_t cmd_reset_enable_2 = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_ENABLE) |
                      QSPI_CFG_CMD_MODE_TWO_LINES,
  .addr             = 0,
  .alt              = 0
};

/* 4x M25Q_CMD_RESET_ENABLE command.*/
static const qspi_command_t cmd_reset_enable_4 = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_ENABLE) |
                      QSPI_CFG_CMD_MODE_FOUR_LINES,
  .addr             = 0,
  .alt              = 0
};

/* 1x M25Q_CMD_RESET_MEMORY command.*/
static const qspi_command_t cmd_reset_memory_1 = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_MEMORY) |
                      QSPI_CFG_CMD_MODE_ONE_LINE,
  .addr             = 0,
  .alt              = 0
};

/* 2x M25Q_CMD_RESET_MEMORY command.*/
static const qspi_command_t cmd_reset_memory_2 = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_MEMORY) |
                      QSPI_CFG_CMD_MODE_TWO_LINES,
  .addr             = 0,
  .alt              = 0
};

/* 4x M25Q_CMD_RESET_MEMORY command.*/
static const qspi_command_t cmd_reset_memory_4 = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_MEMORY) |
                      QSPI_CFG_CMD_MODE_FOUR_LINES,
  .addr             = 0,
  .alt              = 0
};

/* Bus width initialization.*/
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
static const uint8_t evconf_value[1] = {0xCF};
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
static const uint8_t evconf_value[1] = {0x8F};
#else
static const uint8_t evconf_value[1] = {0x4F};
#endif

static const uint8_t flash_status[1] = {(M25Q_READ_DUMMY_CYCLES << 4U) | 0x0FU};

#endif /* M25Q_BUS_MODE != M25Q_BUS_MODE_SPI */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static bool find_id(const uint8_t *set, size_t size, uint8_t element) {
  size_t i;

  for (i = 0; i < size; i++) {
    if (set[i] == element) {
      return true;
    }
  }
  return false;
}

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

static void flash_cmd(M25QDriver *devp, uint8_t cmd) {
#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES;
#endif
  mode.addr = 0U;
  mode.alt  = 0U;
  qspiCommand(devp->config->qspip, &mode);
#else
  uint8_t buf[1];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  spiSend(devp->config->spip, 1, buf);
  spiUnselect(devp->config->spip);
#endif
}

static void flash_cmd_receive(M25QDriver *devp,
                              uint8_t cmd,
                              size_t n,
                              uint8_t *p) {
#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = 0U;
  mode.alt  = 0U;
  qspiReceive(devp->config->qspip, &mode, n, p);
#else
  uint8_t buf[1];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  spiSend(devp->config->spip, 1, buf);
  spiReceive(devp->config->spip, n, p);
  spiUnselect(devp->config->spip);
#endif
}

static void flash_cmd_send(M25QDriver *devp,
                           uint8_t cmd,
                           size_t n,
                           const uint8_t *p) {
#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = 0U;
  mode.alt  = 0U;
  qspiSend(devp->config->qspip, &mode, n, p);
#else
  uint8_t buf[1];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  spiSend(devp->config->spip, 1, buf);
  spiSend(devp->config->spip, n, p);
  spiUnselect(devp->config->spip);
#endif
}

static void flash_cmd_addr(M25QDriver *devp,
                           uint8_t cmd,
                           flash_address_t addr) {
#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_ADDR_SIZE_24;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_ADDR_SIZE_24;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_SIZE_24;

#endif
  mode.addr = addr;
  mode.alt  = 0U;
  qspiCommand(devp->config->qspip, &mode);
#else
  uint8_t buf[4];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >> 8);
  buf[3] = (uint8_t)(addr >> 0);
  spiSend(devp->config->spip, 4, buf);
  spiUnselect(devp->config->spip);
#endif
}

static void flash_cmd_addr_send(M25QDriver *devp,
                                uint8_t cmd,
                                flash_address_t addr,
                                size_t n,
                                const uint8_t *p) {
#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = addr;
  mode.alt  = 0U;
  qspiSend(devp->config->qspip, &mode, n, p);
#else
  uint8_t buf[4];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >> 8);
  buf[3] = (uint8_t)(addr >> 0);
  spiSend(devp->config->spip, 4, buf);
  spiSend(devp->config->spip, n, p);
  spiUnselect(devp->config->spip);
#endif
}

static void flash_cmd_addr_receive(M25QDriver *devp,
                                   uint8_t cmd,
                                   flash_address_t addr,
                                   size_t n,
                                   uint8_t *p) {
#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = addr;
  mode.alt  = 0U;
  qspiReceive(devp->config->qspip, &mode, n, p);
#else
  uint8_t buf[4];

  spiSelect(devp->config->spip);
  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >> 8);
  buf[3] = (uint8_t)(addr >> 0);
  spiSend(devp->config->spip, 4, buf);
  spiReceive(devp->config->spip, n, p);
  spiUnselect(devp->config->spip);
#endif
}

#if (M25Q_BUS_MODE != M25Q_BUS_MODE_SPI) || defined(__DOXYGEN__)
static void flash_cmd_addr_dummy_receive(M25QDriver *devp,
                                         uint8_t cmd,
                                         flash_address_t addr,
                                         uint8_t dummy,
                                         size_t n,
                                         uint8_t *p) {
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DUMMY_CYCLES(dummy) |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif M25Q_BUS_MODE == M25Q_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DUMMY_CYCLES(dummy) |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DUMMY_CYCLES(dummy) |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = addr;
  mode.alt  = 0U;
  qspiReceive(devp->config->qspip, &mode, n, p);
}
#endif /* M25Q_BUS_MODE != M25Q_BUS_MODE_SPI */

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
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)addr + n <= (size_t)descriptor.sectors_count *
                                   (size_t)descriptor.sectors_size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  flash_bus_acquire(devp);

  /* FLASH_READY state while the operation is performed.*/
  devp->state = FLASH_READ;

#if M25Q_BUS_MODE != M25Q_BUS_MODE_SPI
  /* Fast read command in QSPI mode.*/
  flash_cmd_addr_dummy_receive(devp, M25Q_CMD_FAST_READ,
                               addr, M25Q_READ_DUMMY_CYCLES,
                               n, rp);
#else
  /* Normal read command in SPI mode.*/
#endif

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  flash_bus_release(devp);

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

static const qspi_command_t cmd_test_reset_enable_4 = {
  .cfg              = QSPI_CFG_ALT_MODE_FOUR_LINES,
  .addr             = 0,
  .alt              = M25Q_CMD_RESET_ENABLE
};

static const qspi_command_t cmd_test_reset_memory_4 = {
  .cfg              = QSPI_CFG_ALT_MODE_FOUR_LINES,
  .addr             = 0,
  .alt              = M25Q_CMD_RESET_MEMORY
};

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

    /* Bus acquisition.*/
    flash_bus_acquire(devp);

#if M25Q_BUS_MODE == M25Q_BUS_MODE_SPI
    /* SPI initialization.*/
    spiStart(devp->config->spip, devp->config->spicfg);

    /* Reading device ID.*/
#else /* M25Q_BUS_MODE != M25Q_BUS_MODE_SPI */
    /* QSPI initialization.*/
    qspiStart(devp->config->qspip, devp->config->qspicfg);

    /* Reading device ID and unique ID.*/

#if M25Q_SWITCH_WIDTH == TRUE
    /* Attempting a device reset with different bus widths, commands
       shorter than 8 bits are ignored.*/
    qspiCommand(devp->config->qspip, &cmd_reset_enable_1);
    qspiCommand(devp->config->qspip, &cmd_reset_memory_1);
    qspiCommand(devp->config->qspip, &cmd_reset_enable_2);
    qspiCommand(devp->config->qspip, &cmd_reset_memory_2);
    qspiCommand(devp->config->qspip, &cmd_reset_enable_4);
    qspiCommand(devp->config->qspip, &cmd_reset_memory_4);
#endif

    /* Reading device ID and unique ID.*/
    qspiReceive(devp->config->qspip, &cmd_read_id,
                sizeof devp->device_id, devp->device_id);
#endif /* M25Q_BUS_MODE != M25Q_BUS_MODE_SPI */

    /* Checking if the device is white listed.*/
    osalDbgAssert(find_id(manufacturer_ids,
                          sizeof manufacturer_ids,
                          devp->device_id[0]),
                  "invalid manufacturer id");
    osalDbgAssert(find_id(memory_type_ids,
                          sizeof memory_type_ids,
                          devp->device_id[1]),
                  "invalid memory type id");

#if (M25Q_BUS_MODE != M25Q_BUS_MODE_SPI) && (M25Q_SWITCH_WIDTH == TRUE)
    /* Setting up final bus width.*/
    qspiCommand(devp->config->qspip, &cmd_write_enable);
    qspiSend(devp->config->qspip, &cmd_write_evconf, 1, evconf_value);

    {
      uint8_t id[3];

      /* Reading ID again for confirmation.*/
      flash_cmd_receive(devp, M25Q_CMD_MULTIPLE_IO_READ_ID, 3, id);

      /* Checking if the device is white listed.*/
      osalDbgAssert(memcmp(id, devp->device_id, 3) == 0,
                    "id confirmation failed");
    }
#endif

    /* Setting up the device size.*/
    descriptor.sectors_count = (1U << (size_t)devp->device_id[2]) / SECTOR_SIZE;

#if (M25Q_BUS_MODE != M25Q_BUS_MODE_SPI)
    /* Setting up the dummy cycles to be used for rast read operations.*/
    flash_cmd(devp, M25Q_CMD_WRITE_ENABLE);
    flash_cmd_send(devp, M25Q_CMD_WRITE_V_CONF_REGISTER,
                   1, flash_status);
#endif

    /* Driver in ready state.*/
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

    /* Driver stopped.*/
    devp->state = FLASH_STOP;

    /* Bus release.*/
    flash_bus_release(devp);
  }
}

/** @} */
