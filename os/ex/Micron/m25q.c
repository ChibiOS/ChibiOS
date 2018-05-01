/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
 * @addtogroup M25Q
 * @ingroup EX_MICRON
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

static const flash_descriptor_t *m25q_get_descriptor(void *instance);
static flash_error_t m25q_read(void *instance, flash_offset_t offset,
                               size_t n, uint8_t *rp);
static flash_error_t m25q_program(void *instance, flash_offset_t offset,
                                  size_t n, const uint8_t *pp);
static flash_error_t m25q_start_erase_all(void *instance);
static flash_error_t m25q_start_erase_sector(void *instance,
                                             flash_sector_t sector);
static flash_error_t m25q_query_erase(void *instance, uint32_t *msec);
static flash_error_t m25q_verify_erase(void *instance, flash_sector_t sector);
static flash_error_t m25q_read_sfdp(void *instance, flash_offset_t offset,
                                    size_t n, uint8_t *rp);

/**
 * @brief   Virtual methods table.
 */
static const struct M25QDriverVMT m25q_vmt = {
  (size_t)0,
  m25q_get_descriptor, m25q_read, m25q_program,
  m25q_start_erase_all, m25q_start_erase_sector,
  m25q_query_erase, m25q_verify_erase,
  m25q_read_sfdp
};

/**
 * @brief   N25Q128 descriptor.
 */
static flash_descriptor_t m25q_descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE | FLASH_ATTR_REWRITABLE |
                      FLASH_ATTR_SUSPEND_ERASE_CAPABLE,
  .page_size        = 256U,
  .sectors_count    = 0U,           /* It is overwritten.*/
  .sectors          = NULL,
  .sectors_size     = SECTOR_SIZE,
  .address          = 0U
};

#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
/* Initial M25Q_CMD_READ_ID command.*/
static const qspi_command_t m25q_cmd_read_id = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_READ_ID) |
#if M25Q_SWITCH_WIDTH == TRUE
                      QSPI_CFG_CMD_MODE_ONE_LINE     |
                      QSPI_CFG_DATA_MODE_ONE_LINE,
#else
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
                      QSPI_CFG_CMD_MODE_ONE_LINE   |
                      QSPI_CFG_DATA_MODE_ONE_LINE,
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
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
static const qspi_command_t m25q_cmd_write_evconf = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_WRITE_ENHANCED_V_CONF_REGISTER) |
#if M25Q_SWITCH_WIDTH == TRUE
                      QSPI_CFG_CMD_MODE_ONE_LINE     |
                      QSPI_CFG_DATA_MODE_ONE_LINE,
#else
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
                      QSPI_CFG_CMD_MODE_ONE_LINE   |
                      QSPI_CFG_DATA_MODE_ONE_LINE,
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
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
static const qspi_command_t m25q_cmd_write_enable = {
  .cfg              = QSPI_CFG_CMD(M25Q_CMD_WRITE_ENABLE) |
#if M25Q_SWITCH_WIDTH == TRUE
                      QSPI_CFG_CMD_MODE_ONE_LINE,
#else
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
                      QSPI_CFG_CMD_MODE_ONE_LINE,
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
                      QSPI_CFG_CMD_MODE_TWO_LINES,
#else
                      QSPI_CFG_CMD_MODE_FOUR_LINES,
#endif
#endif
  .addr             = 0,
  .alt              = 0
};

/* Bus width initialization.*/
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
static const uint8_t m25q_evconf_value[1] = {0xCF};
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
static const uint8_t m25q_evconf_value[1] = {0x8F};
#else
static const uint8_t m25q_evconf_value[1] = {0x4F};
#endif
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

static const uint8_t m25q_manufacturer_ids[] = M25Q_SUPPORTED_MANUFACTURE_IDS;
static const uint8_t m25q_memory_type_ids[] = M25Q_SUPPORTED_MEMORY_TYPE_IDS;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static bool m25q_find_id(const uint8_t *set, size_t size, uint8_t element) {
  size_t i;

  for (i = 0; i < size; i++) {
    if (set[i] == element) {
      return true;
    }
  }
  return false;
}

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
void m25q_reset_xip(M25QDriver *devp) {
  static const uint8_t flash_conf[1] = {
    (M25Q_READ_DUMMY_CYCLES << 4U) | 0x0FU
  };
  qspi_command_t cmd;
  uint8_t buf[1];

  /* Resetting XIP mode by reading one byte without XIP confirmation bit.*/
  cmd.alt  = 0xFF;
  cmd.addr = 0;
  cmd.cfg  = QSPI_CFG_CMD_MODE_NONE |
             QSPI_CFG_ADDR_SIZE_24 |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE |
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_DATA_MODE_TWO_LINES |
#else
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES |
#endif
             QSPI_CFG_ALT_MODE_FOUR_LINES |  /* Always 4 lines, note.*/
             QSPI_CFG_ALT_SIZE_8 |
             QSPI_CFG_DUMMY_CYCLES(M25Q_READ_DUMMY_CYCLES - 2);
  qspiReceive(devp->config->busp, &cmd, 1, buf);

  /* Enabling write operation.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);

  /* Rewriting volatile configuration register.*/
  jesd216_cmd_send(devp->config->busp, M25Q_CMD_WRITE_V_CONF_REGISTER,
                   1, flash_conf);
}

void m25q_reset_memory(M25QDriver *devp) {

  /* 1x M25Q_CMD_RESET_ENABLE command.*/
  static const qspi_command_t cmd_reset_enable_1 = {
    .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_ENABLE) |
                        QSPI_CFG_CMD_MODE_ONE_LINE,
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

  /* If the device is in one bit mode then the following commands are
     rejected because shorter than 8 bits. If the device is in multiple
     bits mode then the commands are accepted and the device is reset to
     one bit mode.*/
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI4L
  /* 4x M25Q_CMD_RESET_ENABLE command.*/
  static const qspi_command_t cmd_reset_enable_4 = {
    .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_ENABLE) |
                        QSPI_CFG_CMD_MODE_FOUR_LINES,
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

  qspiCommand(devp->config->busp, &cmd_reset_enable_4);
  qspiCommand(devp->config->busp, &cmd_reset_memory_4);
#else
  /* 2x M25Q_CMD_RESET_ENABLE command.*/
  static const qspi_command_t cmd_reset_enable_2 = {
    .cfg              = QSPI_CFG_CMD(M25Q_CMD_RESET_ENABLE) |
                        QSPI_CFG_CMD_MODE_TWO_LINES,
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

  qspiCommand(devp->config->busp, &cmd_reset_enable_2);
  qspiCommand(devp->config->busp, &cmd_reset_memory_2);
#endif

  /* Now the device should be in one bit mode for sure and we perform a
     device reset.*/
  qspiCommand(devp->config->busp, &cmd_reset_enable_1);
  qspiCommand(devp->config->busp, &cmd_reset_memory_1);
}
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

static flash_error_t m25q_poll_status(M25QDriver *devp) {
  uint8_t sts;

  do {
#if M25Q_NICE_WAITING == TRUE
    osalThreadSleepMilliseconds(1);
#endif
    /* Read status command.*/
    jesd216_cmd_receive(devp->config->busp, M25Q_CMD_READ_FLAG_STATUS_REGISTER,
                        1, &sts);
  } while ((sts & M25Q_FLAGS_PROGRAM_ERASE) == 0U);

  /* Checking for errors.*/
  if ((sts & M25Q_FLAGS_ALL_ERRORS) != 0U) {
    /* Clearing status register.*/
    jesd216_cmd(devp->config->busp, M25Q_CMD_CLEAR_FLAG_STATUS_REGISTER);

    /* Program operation failed.*/
    return FLASH_ERROR_PROGRAM;
  }

  return FLASH_NO_ERROR;
}

static const flash_descriptor_t *m25q_get_descriptor(void *instance) {
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state != FLASH_UNINIT) && (devp->state != FLASH_STOP),
                "invalid state");

  return &m25q_descriptor;
}

static flash_error_t m25q_read(void *instance, flash_offset_t offset,
                               size_t n, uint8_t *rp) {
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)m25q_descriptor.sectors_count *
                                     (size_t)m25q_descriptor.sectors_size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* FLASH_READY state while the operation is performed.*/
  devp->state = FLASH_READ;

#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  /* Fast read command in QSPI mode.*/
  jesd216_cmd_addr_dummy_receive(devp->config->busp, M25Q_CMD_FAST_READ,
                                 offset, M25Q_READ_DUMMY_CYCLES, n, rp);
#else
  /* Normal read command in SPI mode.*/
  jesd216_cmd_addr_receive(devp->config->busp, M25Q_CMD_READ,
                           offset, n, rp);
#endif

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return FLASH_NO_ERROR;
}

static flash_error_t m25q_program(void *instance, flash_offset_t offset,
                                  size_t n, const uint8_t *pp) {
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck((instance != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)m25q_descriptor.sectors_count *
                                     (size_t)m25q_descriptor.sectors_size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* FLASH_PGM state while the operation is performed.*/
  devp->state = FLASH_PGM;

  /* Data is programmed page by page.*/
  while (n > 0U) {
    flash_error_t err;

    /* Data size that can be written in a single program page operation.*/
    size_t chunk = (size_t)(((offset | PAGE_MASK) + 1U) - offset);
    if (chunk > n) {
      chunk = n;
    }

    /* Enabling write operation.*/
    jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);

    /* Page program command.*/
    jesd216_cmd_addr_send(devp->config->busp, M25Q_CMD_PAGE_PROGRAM, offset,
                          chunk, pp);

    /* Wait for status and check errors.*/
    err = m25q_poll_status(devp);
    if (err != FLASH_NO_ERROR) {

      /* Bus released.*/
      jesd216_bus_release(devp->config->busp);

      return err;
    }

    /* Next page.*/
    offset += chunk;
    pp     += chunk;
    n      -= chunk;
  }

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return FLASH_NO_ERROR;
}

static flash_error_t m25q_start_erase_all(void *instance) {
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* FLASH_ERASE state while the operation is performed.*/
  devp->state = FLASH_ERASE;

  /* Enabling write operation.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);

  /* Bulk erase command.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_BULK_ERASE);

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return FLASH_NO_ERROR;
}

static flash_error_t m25q_start_erase_sector(void *instance,
                                             flash_sector_t sector) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_offset_t offset = (flash_offset_t)(sector * SECTOR_SIZE);

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < m25q_descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* FLASH_ERASE state while the operation is performed.*/
  devp->state = FLASH_ERASE;

  /* Enabling write operation.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);

  /* Sector erase command.*/
  jesd216_cmd_addr(devp->config->busp, M25Q_CMD_SECTOR_ERASE, offset);

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return FLASH_NO_ERROR;
}

static flash_error_t m25q_verify_erase(void *instance,
                                       flash_sector_t sector) {
  M25QDriver *devp = (M25QDriver *)instance;
  uint8_t cmpbuf[M25Q_COMPARE_BUFFER_SIZE];
  flash_offset_t offset;
  size_t n;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < m25q_descriptor.sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* FLASH_READY state while the operation is performed.*/
  devp->state = FLASH_READ;

  /* Read command.*/
  offset = (flash_offset_t)(sector * SECTOR_SIZE);
  n = SECTOR_SIZE;
  while (n > 0U) {
    uint8_t *p;

#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
   jesd216_cmd_addr_dummy_receive(devp->config->busp, M25Q_CMD_FAST_READ,
                                  offset, M25Q_READ_DUMMY_CYCLES,
                                  sizeof cmpbuf, cmpbuf);
#else
   /* Normal read command in SPI mode.*/
   jesd216_cmd_addr_receive(devp->config->busp, M25Q_CMD_READ,
                            offset, sizeof cmpbuf, cmpbuf);
#endif

    /* Checking for erased state of current buffer.*/
    for (p = cmpbuf; p < &cmpbuf[M25Q_COMPARE_BUFFER_SIZE]; p++) {
      if (*p != 0xFFU) {
        /* Ready state again.*/
        devp->state = FLASH_READY;

        /* Bus released.*/
        jesd216_bus_release(devp->config->busp);

        return FLASH_ERROR_VERIFY;
      }
    }

    offset += sizeof cmpbuf;
    n -= sizeof cmpbuf;
  }

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return FLASH_NO_ERROR;
}

static flash_error_t m25q_query_erase(void *instance, uint32_t *msec) {
  M25QDriver *devp = (M25QDriver *)instance;
  uint8_t sts;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* If there is an erase in progress then the device must be checked.*/
  if (devp->state == FLASH_ERASE) {

    /* Bus acquired.*/
    jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

    /* Read status command.*/
    jesd216_cmd_receive(devp->config->busp, M25Q_CMD_READ_FLAG_STATUS_REGISTER,
                        1, &sts);

    /* If the P/E bit is zero (busy) or the flash in a suspended state then
       report that the operation is still in progress.*/
    if (((sts & M25Q_FLAGS_PROGRAM_ERASE) == 0U) ||
        ((sts & M25Q_FLAGS_ERASE_SUSPEND) != 0U)) {

      /* Bus released.*/
      jesd216_bus_release(devp->config->busp);

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
    if ((sts & M25Q_FLAGS_ALL_ERRORS) != 0U) {

      /* Clearing status register.*/
      jesd216_cmd(devp->config->busp, M25Q_CMD_CLEAR_FLAG_STATUS_REGISTER);

      /* Erase operation failed.*/
      return FLASH_ERROR_ERASE;
    }

    /* Bus released.*/
    jesd216_bus_release(devp->config->busp);
  }

  return FLASH_NO_ERROR;
}

static flash_error_t m25q_read_sfdp(void *instance, flash_offset_t offset,
                                    size_t n, uint8_t *rp) {

  (void)instance;
  (void)rp;
  (void)offset;
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

    /* Bus acquisition.*/
    jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

#if JESD216_BUS_MODE == JESD216_BUS_MODE_SPI
    /* Reading device ID.*/
    jesd216_cmd_receive(devp->config->busp, M25Q_CMD_READ_ID,
                        sizeof devp->device_id, devp->device_id);

#else /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */
    /* Attempting a reset of the XIP mode, it could be in an unexpected state
       because a CPU reset does not reset the memory too.*/
    m25q_reset_xip(devp);

    /* Attempting a eeset of the device, it could be in an unexpected state
       because a CPU reset does not reset the memory too.*/
    m25q_reset_memory(devp);

    /* Reading device ID and unique ID.*/
    qspiReceive(devp->config->busp, &m25q_cmd_read_id,
                sizeof devp->device_id, devp->device_id);
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

    /* Checking if the device is white listed.*/
    osalDbgAssert(m25q_find_id(m25q_manufacturer_ids,
                          sizeof m25q_manufacturer_ids,
                          devp->device_id[0]),
                  "invalid manufacturer id");
    osalDbgAssert(m25q_find_id(m25q_memory_type_ids,
                          sizeof m25q_memory_type_ids,
                          devp->device_id[1]),
                  "invalid memory type id");

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) && (M25Q_SWITCH_WIDTH == TRUE)
    /* Setting up final bus width.*/
    qspiCommand(devp->config->busp, &m25q_cmd_write_enable);
    qspiSend(devp->config->busp, &m25q_cmd_write_evconf, 1, m25q_evconf_value);

    {
      uint8_t id[3];

      /* Reading ID again for confirmation.*/
      jesd216_cmd_receive(devp->config->busp, M25Q_CMD_MULTIPLE_IO_READ_ID,
                          3, id);

      /* Checking if the device is white listed.*/
      osalDbgAssert(memcmp(id, devp->device_id, 3) == 0,
                    "id confirmation failed");
    }
#endif

    /* Setting up the device size.*/
    m25q_descriptor.sectors_count = (1U << (size_t)devp->device_id[2]) /
                                    SECTOR_SIZE;

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI)
    {
      static const uint8_t flash_conf[1] = {
        (M25Q_READ_DUMMY_CYCLES << 4U) | 0x0FU
      };

      /* Setting up the dummy cycles to be used for fast read operations.*/
      jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);
      jesd216_cmd_send(devp->config->busp, M25Q_CMD_WRITE_V_CONF_REGISTER,
                       1, flash_conf);
    }
#endif

    /* Driver in ready state.*/
    devp->state = FLASH_READY;

    /* Bus release.*/
    jesd216_bus_release(devp->config->busp);
  }
} 

/**
 * @brief   Deactivates the N25Q128 driver.
 *
 * @param[in] devp      pointer to the @p M25QDriver object
 *
 * @api
 */
void m25qStop(M25QDriver *devp) {

  osalDbgCheck(devp != NULL);
  osalDbgAssert(devp->state != FLASH_UNINIT, "invalid state");

  if (devp->state != FLASH_STOP) {

    /* Bus acquisition.*/
    jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

    /* Stopping bus device.*/
    jesd216_stop(devp->config->busp);

    /* Deleting current configuration.*/
    devp->config = NULL;

    /* Driver stopped.*/
    devp->state = FLASH_STOP;

    /* Bus release.*/
    jesd216_bus_release(devp->config->busp);
  }
}

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
#if (QSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Enters the memory Mapping mode.
 * @details The memory mapping mode is only available when the QSPI mode
 *          is selected and the underlying QSPI controller supports the
 *          feature.
 *
 * @param[in] devp      pointer to the @p M25QDriver object
 * @param[out] addrp    pointer to the memory start address of the mapped
 *                      flash or @p NULL
 *
 * @api
 */
void m25qMemoryMap(M25QDriver *devp, uint8_t **addrp) {
  static const uint8_t flash_status_xip[1] = {
    (M25Q_READ_DUMMY_CYCLES << 4U) | 0x07U
  };
  qspi_command_t cmd;

  /* Bus acquisition.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* Activating XIP mode in the device.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);
  jesd216_cmd_send(devp->config->busp, M25Q_CMD_WRITE_V_CONF_REGISTER,
                   1, flash_status_xip);

  /* Putting the QSPI driver in memory mapped mode.*/
  cmd.cfg = QSPI_CFG_CMD(M25Q_CMD_FAST_READ) |
            QSPI_CFG_ADDR_SIZE_24 |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
            QSPI_CFG_CMD_MODE_ONE_LINE |
            QSPI_CFG_ADDR_MODE_ONE_LINE |
            QSPI_CFG_DATA_MODE_ONE_LINE |
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
            QSPI_CFG_CMD_MODE_TWO_LINES |
            QSPI_CFG_ADDR_MODE_TWO_LINES |
            QSPI_CFG_DATA_MODE_TWO_LINES |
#else
            QSPI_CFG_CMD_MODE_FOUR_LINES |
            QSPI_CFG_ADDR_MODE_FOUR_LINES |
            QSPI_CFG_DATA_MODE_FOUR_LINES |
#endif
            QSPI_CFG_ALT_MODE_FOUR_LINES |  /* Always 4 lines, note.*/
            QSPI_CFG_ALT_SIZE_8 |
            QSPI_CFG_SIOO |
            QSPI_CFG_DUMMY_CYCLES(M25Q_READ_DUMMY_CYCLES - 2);

  qspiMapFlash(devp->config->busp, &cmd, addrp);

  /* Bus release.*/
  jesd216_bus_release(devp->config->busp);
}

/**
 * @brief   Leaves the memory Mapping mode.
 *
 * @param[in] devp      pointer to the @p M25QDriver object
 *
 * @api
 */
void m25qMemoryUnmap(M25QDriver *devp) {

  /* Bus acquisition.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  qspiUnmapFlash(devp->config->busp);

  m25q_reset_xip(devp);

  /* Bus release.*/
  jesd216_bus_release(devp->config->busp);
}
#endif /* QSPI_SUPPORTS_MEMMAP == TRUE */
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

/** @} */
