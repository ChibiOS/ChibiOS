/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    m25q_flash.c
 * @brief   Micron N25Q serial flash driver code.
 *
 * @addtogroup MICRON_N25Q
 * @ingroup M25Q
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

/**
 * @brief   N25Q128 descriptor.
 */
flash_descriptor_t m25q_descriptor = {
  .attributes       = FLASH_ATTR_ERASED_IS_ONE | FLASH_ATTR_REWRITABLE |
                      FLASH_ATTR_SUSPEND_ERASE_CAPABLE,
  .page_size        = 256U,
  .sectors_count    = 0U,           /* It is overwritten.*/
  .sectors          = NULL,
  .sectors_size     = SECTOR_SIZE,
  .address          = 0U
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

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

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
static void m25q_reset_memory(M25QDriver *devp) {

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
#endif /* #if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

static const uint8_t m25q_manufacturer_ids[] = M25Q_SUPPORTED_MANUFACTURE_IDS;
static const uint8_t m25q_memory_type_ids[] = M25Q_SUPPORTED_MEMORY_TYPE_IDS;

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void m25q_device_init(M25QDriver *devp) {

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
}

const flash_descriptor_t *m25q_get_descriptor(void *instance) {
  M25QDriver *devp = (M25QDriver *)instance;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state != FLASH_UNINIT) && (devp->state != FLASH_STOP),
                "invalid state");

  return &m25q_descriptor;
}

flash_error_t m25q_device_read(M25QDriver *devp, flash_offset_t offset,
                               size_t n, uint8_t *rp) {

#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  /* Fast read command in QSPI mode.*/
  jesd216_cmd_addr_dummy_receive(devp->config->busp, M25Q_CMD_FAST_READ,
                                 offset, M25Q_READ_DUMMY_CYCLES, n, rp);
#else
  /* Normal read command in SPI mode.*/
  jesd216_cmd_addr_receive(devp->config->busp, M25Q_CMD_READ,
                           offset, n, rp);
#endif

  return FLASH_NO_ERROR;
}

flash_error_t m25q_device_program(M25QDriver *devp, flash_offset_t offset,
                                  size_t n, const uint8_t *pp) {

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

  return FLASH_NO_ERROR;
}

flash_error_t m25q_device_start_erase_all(M25QDriver *devp) {

  /* Enabling write operation.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);

  /* Bulk erase command.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_BULK_ERASE);

  return FLASH_NO_ERROR;
}

flash_error_t m25q_device_start_erase_sector(M25QDriver *devp,
                                             flash_sector_t sector) {
  flash_offset_t offset = (flash_offset_t)(sector * SECTOR_SIZE);

  /* Enabling write operation.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);

  /* Sector erase command.*/
  jesd216_cmd_addr(devp->config->busp, M25Q_CMD_SECTOR_ERASE, offset);

  return FLASH_NO_ERROR;
}

flash_error_t m25q_device_verify_erase(M25QDriver *devp,
                                       flash_sector_t sector) {
  uint8_t cmpbuf[M25Q_COMPARE_BUFFER_SIZE];
  flash_offset_t offset;
  size_t n;

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

        return FLASH_ERROR_VERIFY;
      }
    }

    offset += sizeof cmpbuf;
    n -= sizeof cmpbuf;
  }

  return FLASH_NO_ERROR;
}

flash_error_t m25q_device_query_erase(M25QDriver *devp, uint32_t *msec) {
  uint8_t sts;

  /* Read status command.*/
  jesd216_cmd_receive(devp->config->busp, M25Q_CMD_READ_FLAG_STATUS_REGISTER,
                      1, &sts);

  /* If the P/E bit is zero (busy) or the flash in a suspended state then
     report that the operation is still in progress.*/
  if (((sts & M25Q_FLAGS_PROGRAM_ERASE) == 0U) ||
      ((sts & M25Q_FLAGS_ERASE_SUSPEND) != 0U)) {

    /* Recommended time before polling again, this is a simplified
       implementation.*/
    if (msec != NULL) {
      *msec = 1U;
    }

    return FLASH_BUSY_ERASING;
  }

  /* Checking for errors.*/
  if ((sts & M25Q_FLAGS_ALL_ERRORS) != 0U) {

    /* Clearing status register.*/
    jesd216_cmd(devp->config->busp, M25Q_CMD_CLEAR_FLAG_STATUS_REGISTER);

    /* Erase operation failed.*/
    return FLASH_ERROR_ERASE;
  }

  return FLASH_NO_ERROR;
}

flash_error_t m25q_device_read_sfdp(M25QDriver *devp, flash_offset_t offset,
                                    size_t n, uint8_t *rp) {

  (void)devp;
  (void)rp;
  (void)offset;
  (void)n;

  return FLASH_NO_ERROR;
}

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
void m25q_activate_xip(M25QDriver *devp) {
  static const uint8_t flash_status_xip[1] = {
    (M25Q_READ_DUMMY_CYCLES << 4U) | 0x07U
  };

  /* Activating XIP mode in the device.*/
  jesd216_cmd(devp->config->busp, M25Q_CMD_WRITE_ENABLE);
  jesd216_cmd_send(devp->config->busp, M25Q_CMD_WRITE_V_CONF_REGISTER,
                   1, flash_status_xip);
}

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
#endif /* #if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

/** @} */
