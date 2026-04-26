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
 * @file        hal_xsnor_micron_n25q_impl.inc
 * @brief       Template of SNOR Micron N25Q source.
 * @note        This is a template file, can be edited directly.
 *
 * @addtogroup  HAL_XSNOR_MICRON_N25Q
 * @{
 */

/* This is an, automatically generated, implementation file that can be
   manually edited, it is not re-generated if already present.*/

#define PAGE_SIZE                                   256U
#define PAGE_MASK                                   (PAGE_SIZE - 1U)
#define xsnorcfg                                    ((const xsnor_config_t *)self->config)

/**
 * @name    Command codes
 * @{
 */
#define CMD_RESET_ENABLE                            0x66
#define CMD_RESET_MEMORY                            0x99
#define CMD_READ_ID                                 0x9F
#define CMD_MULTIPLE_IO_READ_ID                     0xAF
#define CMD_READ_DISCOVERY_PARAMETER                0x5A
#define CMD_READ                                    0x03
#define CMD_READ4B                                  0x13
#define CMD_FAST_READ                               0x0B
#define CMD_FAST_READ4B                             0x0C
#define CMD_WRITE_ENABLE                            0x06
#define CMD_WRITE_DISABLE                           0x04
#define CMD_READ_STATUS_REGISTER                    0x05
#define CMD_WRITE_STATUS_REGISTER                   0x01
#define CMD_READ_LOCK_REGISTER                      0xE8
#define CMD_WRITE_LOCK_REGISTER                     0xE5
#define CMD_READ_FLAG_STATUS_REGISTER               0x70
#define CMD_CLEAR_FLAG_STATUS_REGISTER              0x50
#define CMD_READ_NV_CONFIGURATION_REGISTER          0xB5
#define CMD_WRITE_NV_CONFIGURATION_REGISTER         0xB1
#define CMD_READ_V_CONF_REGISTER                    0x85
#define CMD_WRITE_V_CONF_REGISTER                   0x81
#define CMD_READ_ENHANCED_V_CONF_REGISTER           0x65
#define CMD_WRITE_ENHANCED_V_CONF_REGISTER          0x61
#define CMD_PAGE_PROGRAM                            0x02
#define CMD_PAGE_PROGRAM4B                          0x12
#define CMD_SUBSECTOR_ERASE                         0x20
#define CMD_SUBSECTOR_ERASE4B                       0x21
#define CMD_SECTOR_ERASE                            0xD8
#define CMD_SECTOR_ERASE4B                          0xDC
#define CMD_BULK_ERASE                              0xC7
#define CMD_PROGRAM_ERASE_RESUME                    0x7A
#define CMD_PROGRAM_ERASE_SUSPEND                   0x75
#define CMD_READ_OTP_ARRAY                          0x4B
#define CMD_PROGRAM_OTP_ARRAY                       0x42
/** @} */

/**
 * @name    Flags status register bits
 * @{
 */
#define FLAGS_PROGRAM_ERASE                         0x80U
#define FLAGS_ERASE_SUSPEND                         0x40U
#define FLAGS_ERASE_ERROR                           0x20U
#define FLAGS_PROGRAM_ERROR                         0x10U
#define FLAGS_VPP_ERROR                             0x08U
#define FLAGS_PROGRAM_SUSPEND                       0x04U
#define FLAGS_PROTECTION_ERROR                      0x02U
#define FLAGS_RESERVED                              0x01U
#define FLAGS_ALL_ERRORS                            (FLAGS_ERASE_ERROR |    \
                                                     FLAGS_PROGRAM_ERROR |  \
                                                     FLAGS_VPP_ERROR |      \
                                                     FLAGS_PROTECTION_ERROR)
/** @} */

#if (XSNOR_USE_WSPI == TRUE) || defined(__DOXYGEN__)
static const xsnor_commands_t cmd1l = {
  .cmd                  = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_addr             = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_ONE_LINE      |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_data             = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_ONE_LINE      |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_addr_data        = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_ONE_LINE      |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_ONE_LINE      |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24)
};

static const xsnor_commands_t cmd1l_4b = {
  .cmd                  = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_addr             = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_ONE_LINE      |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_data             = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_ONE_LINE      |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_addr_data        = (WSPI_CFG_CMD_MODE_ONE_LINE       |
                           WSPI_CFG_ADDR_MODE_ONE_LINE      |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_ONE_LINE      |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32)
};

static const xsnor_commands_t cmd2l = {
  .cmd                  = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_addr             = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_TWO_LINES     |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_data             = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_TWO_LINES     |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_addr_data        = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_TWO_LINES     |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_TWO_LINES     |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24)
};

static const xsnor_commands_t cmd2l_4b = {
  .cmd                  = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_addr             = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_TWO_LINES     |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_data             = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_TWO_LINES     |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_addr_data        = (WSPI_CFG_CMD_MODE_TWO_LINES      |
                           WSPI_CFG_ADDR_MODE_TWO_LINES     |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_TWO_LINES     |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32)
};

static const xsnor_commands_t cmd4l = {
  .cmd                  = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_addr             = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_FOUR_LINES    |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_data             = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_FOUR_LINES    |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24),
  .cmd_addr_data        = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_FOUR_LINES    |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_FOUR_LINES    |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_24)
};

static const xsnor_commands_t cmd4l_4b = {
  .cmd                  = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_addr             = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_FOUR_LINES    |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_NONE          |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_data             = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_NONE          |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_FOUR_LINES    |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32),
  .cmd_addr_data        = (WSPI_CFG_CMD_MODE_FOUR_LINES     |
                           WSPI_CFG_ADDR_MODE_FOUR_LINES    |
                           WSPI_CFG_ALT_MODE_NONE           |
                           WSPI_CFG_DATA_MODE_FOUR_LINES    |
                           WSPI_CFG_CMD_SIZE_8              |
                           WSPI_CFG_ADDR_SIZE_32)
};

/* 1x N25Q_CMD_RESET_ENABLE command.*/
static const wspi_command_t cmd_reset_enable_1 = {
  .cmd              = CMD_RESET_ENABLE,
  .cfg              = WSPI_CFG_CMD_MODE_ONE_LINE,
  .addr             = 0,
  .alt              = 0,
  .dummy            = 0
};

/* 1x N25Q_CMD_RESET_MEMORY command.*/
static const wspi_command_t cmd_reset_memory_1 = {
  .cmd              = CMD_RESET_MEMORY,
  .cfg              = WSPI_CFG_CMD_MODE_ONE_LINE,
  .addr             = 0,
  .alt              = 0,
  .dummy            = 0
};

/* 2x CMD_RESET_ENABLE command.*/
static const wspi_command_t cmd_reset_enable_2 = {
  .cmd              = CMD_RESET_ENABLE,
  .cfg              = WSPI_CFG_CMD_MODE_TWO_LINES,
  .addr             = 0,
  .alt              = 0,
  .dummy            = 0
};

/* 2x CMD_RESET_MEMORY command.*/
static const wspi_command_t cmd_reset_memory_2 = {
  .cmd              = CMD_RESET_MEMORY,
  .cfg              = WSPI_CFG_CMD_MODE_TWO_LINES,
  .addr             = 0,
  .alt              = 0,
  .dummy            = 0
};

/* 4x CMD_RESET_ENABLE command.*/
static const wspi_command_t cmd_reset_enable_4 = {
  .cmd              = CMD_RESET_ENABLE,
  .cfg              = WSPI_CFG_CMD_MODE_FOUR_LINES,
  .addr             = 0,
  .alt              = 0,
  .dummy            = 0
};

/* 4x CMD_RESET_MEMORY command.*/
static const wspi_command_t cmd_reset_memory_4 = {
  .cmd              = CMD_RESET_MEMORY,
  .cfg              = WSPI_CFG_CMD_MODE_FOUR_LINES,
  .addr             = 0,
  .alt              = 0,
  .dummy            = 0
};
#endif /* XSNOR_USE_WSPI == TRUE */

/* Device identifiers.*/
static const uint8_t n25q_manufacturer_ids[] = {0x20};
static const uint8_t n25q_memory_type_ids[] = {0xBA, 0xBB};

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static flash_error_t n25q_poll_status(hal_xsnor_micron_n25q_c *self) {

  do {
    if ((xsnorcfg->options & N25Q_OPT_NICE_WAITING) != 0U) {
      osalThreadSleepMilliseconds(1);
    }

    /* Read status command.*/
    __xsnor_bus_cmd_receive(self, CMD_READ_FLAG_STATUS_REGISTER,
                            1U, &xsnorcfg->buffers->databuf[0]);
  } while ((xsnorcfg->buffers->databuf[0] & FLAGS_PROGRAM_ERASE) == 0U);

  /* Checking for errors.*/
  if ((xsnorcfg->buffers->databuf[0] & FLAGS_ALL_ERRORS) != 0U) {
    /* Clearing status register.*/
    __xsnor_bus_cmd(self, CMD_CLEAR_FLAG_STATUS_REGISTER);

    /* Program operation failed.*/
    return FLASH_ERROR_PROGRAM;
  }

  return FLASH_NO_ERROR;
}

static inline bool n25q_spi_4bytes(const hal_xsnor_micron_n25q_c *self) {

  return (self->descriptor.attributes & FLASH_ATTR_SPI_4BYTES_ADDR_HINT) != 0U;
}

#if (XSNOR_USE_WSPI == TRUE) || defined(__DOXYGEN__)
static void n25q_activate_xip(hal_xsnor_micron_n25q_c *self) {

  /* Configuration to be written.*/
  xsnorcfg->buffers->databuf[0] = ((xsnorcfg->options & N25Q_OPT_DUMMY_CYCLES_MASK) << 4U) | 0x07U;

  /* Activating XIP mode in the device.*/
  __xsnor_bus_cmd(self, CMD_WRITE_ENABLE);
  __xsnor_bus_cmd_send(self, CMD_WRITE_V_CONF_REGISTER,
                       1, &xsnorcfg->buffers->databuf[0]);
}

static void n25q_reset_xip_mode(hal_xsnor_micron_n25q_c *self,
                                uint32_t iolines_cfg) {
  wspi_command_t cmd;

  /* See N25Q "Terminating XIP After a Controller and Memory Reset".
     Resetting XIP is done by forcing the XIP confirmation bit to one. The
     confirmation bit is sent by using one ALT byte over four lines, because of
     this two cycles are subtracted from the configured dummy cycles.*/
  cmd.cmd   = 0U;
  cmd.addr  = 0U;
  cmd.alt   = 0xFFU;
  cmd.dummy = (xsnorcfg->options & N25Q_OPT_DUMMY_CYCLES_MASK) - 2U;
  cmd.cfg   = WSPI_CFG_CMD_MODE_NONE |
              WSPI_CFG_ADDR_SIZE_24 |
              WSPI_CFG_ALT_MODE_FOUR_LINES |  /* Always 4 lines, note.*/
              WSPI_CFG_ALT_SIZE_8 |
              iolines_cfg;

  /* Resetting XIP.*/
  wspiReceive(xsnorcfg->bus.wspi.drv, &cmd,
              1, &xsnorcfg->buffers->databuf[0]);
}

static void n25q_reset_xip(hal_xsnor_micron_n25q_c *self) {

  /* The current bus width is not known at this point, try all XIP exit forms
     using explicit commands instead of the target-mode command table.*/
  n25q_reset_xip_mode(self, WSPI_CFG_ADDR_MODE_FOUR_LINES |
                            WSPI_CFG_DATA_MODE_FOUR_LINES);
  n25q_reset_xip_mode(self, WSPI_CFG_ADDR_MODE_TWO_LINES |
                            WSPI_CFG_DATA_MODE_TWO_LINES);
  n25q_reset_xip_mode(self, WSPI_CFG_ADDR_MODE_ONE_LINE |
                            WSPI_CFG_DATA_MODE_ONE_LINE);
}

static void n25q_reset_memory(hal_xsnor_micron_n25q_c *self) {

  /* See N25Q "RESET ENABLE and RESET MEMORY Command". The number of I/O lines
     varies with the active protocol, and the current bus width is not known.
     Try multi-line reset forms first, then finish with the one-line reset that
     should always be valid after XIP/protocol recovery.*/
  wspiCommand(xsnorcfg->bus.wspi.drv, &cmd_reset_enable_4);
  wspiCommand(xsnorcfg->bus.wspi.drv, &cmd_reset_memory_4);
  wspiCommand(xsnorcfg->bus.wspi.drv, &cmd_reset_enable_2);
  wspiCommand(xsnorcfg->bus.wspi.drv, &cmd_reset_memory_2);

  /* Now the device should be in one bit mode for sure and we perform a
     device reset.*/
  wspiCommand(xsnorcfg->bus.wspi.drv, &cmd_reset_enable_1);
  wspiCommand(xsnorcfg->bus.wspi.drv, &cmd_reset_memory_1);
}

static void n25q_read_id(hal_xsnor_micron_n25q_c *self, uint8_t *buf) {
  wspi_command_t cmd;

  /* This command is always one line.*/
  cmd.cfg   = WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_DATA_MODE_ONE_LINE;
  cmd.cmd   = CMD_READ_ID;
  cmd.addr  = 0U;
  cmd.alt   = 0U;
  cmd.dummy = 0U;
  wspiReceive(xsnorcfg->bus.wspi.drv, &cmd, 3U, buf);
}
#endif /* XSNOR_USE_WSPI == TRUE */

/* Caller must hold the bus and have selected the bus configuration. */
static flash_error_t n25q_read_unlocked(hal_xsnor_micron_n25q_c *self,
                                        flash_offset_t offset, size_t n,
                                        uint8_t *rp) {
  flash_error_t err = FLASH_NO_ERROR;

  switch (xsnorcfg->bus_type) {
  case XSNOR_BUS_MODE_SPI:
    /* Using normal read command because it does not require dummy cycles.*/
    __xsnor_bus_cmd_addr_receive(self,
                                 n25q_spi_4bytes(self) ? CMD_READ4B : CMD_READ,
                                 offset, n, rp);
    break;
  case XSNOR_BUS_MODE_WSPI_1LINE:
  case XSNOR_BUS_MODE_WSPI_2LINES:
  case XSNOR_BUS_MODE_WSPI_4LINES:
    /* Fast read in order to leverage multiple lines.*/
    __xsnor_bus_cmd_addr_dummy_receive(self,
                                       n25q_spi_4bytes(self) ?
                                       CMD_FAST_READ4B : CMD_FAST_READ,
                                       offset,
                                       xsnorcfg->options & N25Q_OPT_DUMMY_CYCLES_MASK,
                                       n, rp);
    break;
  default:
    osalDbgAssert(false, "invalid bus type");
    err = FLASH_ERROR_HW_FAILURE;
    break;
  }

  return err;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "hal_xsnor_micron_n25q_c" methods.                           */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_xsnor_micron_n25q_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__n25q_objinit_impl(void *ip, const void *vmt) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __xsnor_objinit_impl(self, vmt);

  /* Initialization code.*/
  self->descriptor.attributes    = FLASH_ATTR_ERASED_IS_ONE |
                                   FLASH_ATTR_REWRITABLE;
  self->descriptor.page_size     = 256U;
  self->descriptor.sectors_count = 0U; /* Overwritten.*/
  self->descriptor.sectors       = NULL;
  self->descriptor.sectors_size  = 0U;
  self->descriptor.address       = 0U;
  self->descriptor.size          = 0U; /* Overwritten.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance to be disposed.
 */
void __n25q_dispose_impl(void *ip) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;

  /* Finalization code.*/
  /* Implementation.*/

  /* Finalization of the ancestors-defined parts.*/
  __xsnor_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__n25q_setcfg_impl(void *ip, const void *config) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  const xsnor_config_t *xcfg;
  unsigned options;

  xcfg = (const xsnor_config_t *)__xsnor_setcfg_impl(self, config);
  if (xcfg == NULL) {
    return NULL;
  }

  options = (unsigned)xcfg->options;

  if ((options & ~(N25Q_OPT_DUMMY_CYCLES_MASK |
                   N25Q_OPT_NO_WIDTH_SWITCH |
                   N25Q_OPT_USE_SUBSECTORS |
                   N25Q_OPT_NICE_WAITING)) != 0U) {
    return NULL;
  }

#if XSNOR_USE_BOTH == TRUE
  switch (xcfg->bus_type) {
  case XSNOR_BUS_MODE_SPI:
    if ((options & N25Q_OPT_NO_WIDTH_SWITCH) != 0U) {
      return NULL;
    }
    break;
  case XSNOR_BUS_MODE_WSPI_1LINE:
  case XSNOR_BUS_MODE_WSPI_2LINES:
  case XSNOR_BUS_MODE_WSPI_4LINES:
    if ((options & N25Q_OPT_DUMMY_CYCLES_MASK) < 2U) {
      return NULL;
    }
    break;
  default:
    return NULL;
  }
#else
#if XSNOR_USE_SPI == TRUE
  if ((options & N25Q_OPT_NO_WIDTH_SWITCH) != 0U) {
    return NULL;
  }
#endif
#if XSNOR_USE_WSPI == TRUE
  switch (xcfg->bus_type) {
  case XSNOR_BUS_MODE_WSPI_1LINE:
  case XSNOR_BUS_MODE_WSPI_2LINES:
  case XSNOR_BUS_MODE_WSPI_4LINES:
    if ((options & N25Q_OPT_DUMMY_CYCLES_MASK) < 2U) {
      return NULL;
    }
    break;
  default:
    return NULL;
  }
#endif
#endif

  return xcfg;
}

/**
 * @brief       Override of method @p xsnor_device_init().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @return                      An error code.
 */
flash_error_t __n25q_init_impl(void *ip) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  const xsnor_config_t *config = self->config;

  /* Bus type and width setup, only necessary if WSPI is in use.*/
#if XSNOR_USE_WSPI == TRUE
  switch (config->bus_type) {
  case XSNOR_BUS_MODE_SPI:
    self->commands = NULL;
    break;
  case XSNOR_BUS_MODE_WSPI_1LINE:
    self->commands = &cmd1l;
    break;
  case XSNOR_BUS_MODE_WSPI_2LINES:
    self->commands = &cmd2l;
    break;
  case XSNOR_BUS_MODE_WSPI_4LINES:
    self->commands = &cmd4l;
    break;
  default:
    osalDbgAssert(false, "invalid bus type");
    self->commands = NULL;
    return FLASH_ERROR_HW_FAILURE;
  }
#endif

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type == XSNOR_BUS_MODE_SPI)
#endif
#if XSNOR_USE_SPI == TRUE
  {
    /* Initialization procedure in the SPI case.
       Reading device ID.*/
    __xsnor_bus_cmd_receive(self, CMD_READ_ID,
                            3U, &config->buffers->databuf[0]);

    /* Manufacturer identifier.*/
    if (!__xsnor_find_id(n25q_manufacturer_ids,
                         sizeof n25q_manufacturer_ids,
                         config->buffers->databuf[0])) {
      return FLASH_ERROR_HW_FAILURE;
    }

    /* Memory identifier.*/
    if (!__xsnor_find_id(n25q_memory_type_ids,
                         sizeof n25q_memory_type_ids,
                         config->buffers->databuf[1])) {
      return FLASH_ERROR_HW_FAILURE;
    }
  }
#endif /* XSNOR_USE_SPI == TRUE */
#if XSNOR_USE_BOTH == TRUE
  else
#endif
#if XSNOR_USE_WSPI == TRUE
  {
    /* Attempting a reset of the XIP mode, it could be in an unexpected state
       because a CPU reset does not reset the memory too.*/
    n25q_reset_xip(self);

    /* Attempting a reset of the device, it could be in an unexpected state
       because a CPU reset does not reset the memory too.*/
    n25q_reset_memory(self);

    /* Reading device ID and unique ID.*/
    n25q_read_id(self, &config->buffers->databuf[0]);

    /* Manufacturer identifier.*/
    if (!__xsnor_find_id(n25q_manufacturer_ids,
                         sizeof n25q_manufacturer_ids,
                         config->buffers->databuf[0])) {
      return FLASH_ERROR_HW_FAILURE;
    }

    /* Memory identifier.*/
    if (!__xsnor_find_id(n25q_memory_type_ids,
                         sizeof n25q_memory_type_ids,
                         config->buffers->databuf[1])) {
      return FLASH_ERROR_HW_FAILURE;
    }

    /* Setting up final bus width.*/
    if ((config->options & N25Q_OPT_NO_WIDTH_SWITCH) == 0U) {
      wspi_command_t cmd;

      /* We need to switch in the volatile register, write enable in one
         line mode.*/
      cmd.cfg   = WSPI_CFG_CMD_MODE_ONE_LINE;
      cmd.cmd   = CMD_WRITE_ENABLE;
      cmd.addr  = 0U;
      cmd.alt   = 0U;
      cmd.dummy = 0U;
      wspiCommand(config->bus.wspi.drv, &cmd);

      /* Bus width configuration.*/
      cmd.cfg   = WSPI_CFG_CMD_MODE_ONE_LINE | WSPI_CFG_DATA_MODE_ONE_LINE;
      cmd.cmd   = CMD_WRITE_ENHANCED_V_CONF_REGISTER;
      cmd.addr  = 0U;
      cmd.alt   = 0U;
      cmd.dummy = 0U;
      switch (config->bus_type) {
      case XSNOR_BUS_MODE_WSPI_1LINE:
        config->buffers->databuf[0] = 0xCFU;
        break;
      case XSNOR_BUS_MODE_WSPI_2LINES:
        config->buffers->databuf[0] = 0x8FU;
        break;
      case XSNOR_BUS_MODE_WSPI_4LINES:
        config->buffers->databuf[0] = 0x4FU;
        break;
      default:
        osalDbgAssert(false, "invalid bus type");
      }
      wspiSend(config->bus.wspi.drv, &cmd,
               1, &config->buffers->databuf[0]);

      /* Dummy cycles configuration, intentionally done only after reset/probe.
         The N25Q XIP termination sequence exits XIP but does not rewrite
         V_CONF; rewriting V_CONF earlier would use target-mode commands before
         the current device mode is known.*/
      __xsnor_bus_cmd(self, CMD_WRITE_ENABLE);
      config->buffers->databuf[0] =
        ((config->options & N25Q_OPT_DUMMY_CYCLES_MASK) << 4U) | 0x0FU;
      __xsnor_bus_cmd_send(self, CMD_WRITE_V_CONF_REGISTER,
                           1, &config->buffers->databuf[0]);

      /* Reading again the memory ID using final settings for confirmation that
         bus switch actually occurred.*/
      __xsnor_bus_cmd_receive(self, CMD_MULTIPLE_IO_READ_ID, 3U, &config->buffers->databuf[0]);

      /* Manufacturer identifier.*/
      if (!__xsnor_find_id(n25q_manufacturer_ids,
                           sizeof n25q_manufacturer_ids,
                           config->buffers->databuf[0])) {
        return FLASH_ERROR_HW_FAILURE;
      }

      /* Memory identifier.*/
      if (!__xsnor_find_id(n25q_memory_type_ids,
                           sizeof n25q_memory_type_ids,
                           config->buffers->databuf[1])) {
        return FLASH_ERROR_HW_FAILURE;
      }
    }
    else {
      /* No need to switch.*/
    }
  }
#endif /* XSNOR_USE_WSPI == TRUE */

  /* Variable parts of the descriptor.*/
  if ((xsnorcfg->options & N25Q_OPT_USE_SUBSECTORS) != 0U) {
    self->descriptor.sectors_size = 0x00001000U;
  }
  else {
    self->descriptor.sectors_size = 0x00010000U;
  }
  self->descriptor.size = (uint32_t)(1U << ((size_t)config->buffers->databuf[2] & 0x1FU));
  self->descriptor.sectors_count = self->descriptor.size / self->descriptor.sectors_size;

  if (self->descriptor.size > 0x01000000U) {
    self->descriptor.attributes |= FLASH_ATTR_SPI_4BYTES_ADDR_HINT;
  }
  else {
    self->descriptor.attributes &= ~FLASH_ATTR_SPI_4BYTES_ADDR_HINT;
  }

#if XSNOR_USE_WSPI == TRUE
  switch (config->bus_type) {
  case XSNOR_BUS_MODE_SPI:
    break;
  case XSNOR_BUS_MODE_WSPI_1LINE:
    self->commands = n25q_spi_4bytes(self) ? &cmd1l_4b : &cmd1l;
    break;
  case XSNOR_BUS_MODE_WSPI_2LINES:
    self->commands = n25q_spi_4bytes(self) ? &cmd2l_4b : &cmd2l;
    break;
  case XSNOR_BUS_MODE_WSPI_4LINES:
    self->commands = n25q_spi_4bytes(self) ? &cmd4l_4b : &cmd4l;
    break;
  default:
    break;
  }
#endif

  return FLASH_NO_ERROR;
}

/**
 * @brief       Override of method @p __flash_read().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be read.
 * @param[out]    rp            Pointer to the data buffer.
 * @return                      An error code.
 */
flash_error_t __n25q_read_impl(void *ip, flash_offset_t offset, size_t n,
                               uint8_t *rp) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  flash_error_t err;

  osalDbgAssert(self->state == FLASH_READ, "invalid state");

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  err = n25q_read_unlocked(self, offset, n, rp);

  __xsnor_bus_release(self);

  return err;
}

/**
 * @brief       Override of method @p __flash_program().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to be programmed.
 * @param[in]     pp            Pointer to the data buffer.
 * @return                      An error code.
 */
flash_error_t __n25q_program_impl(void *ip, flash_offset_t offset, size_t n,
                                  const uint8_t *pp) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgAssert(self->state == FLASH_PGM, "invalid state");

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  /* Data is programmed page by page.*/
  while ((n > 0U) && (err == FLASH_NO_ERROR)) {
    /* Data size that can be written in a single program page operation.*/
    size_t chunk = (size_t)(((offset | PAGE_MASK) + 1U) - offset);

    if (chunk > n) {
      chunk = n;
    }

    /* Enabling write operation.*/
    __xsnor_bus_cmd(self, CMD_WRITE_ENABLE);

    /* Page program command.*/
    __xsnor_bus_cmd_addr_send(self,
                              n25q_spi_4bytes(self) ?
                              CMD_PAGE_PROGRAM4B : CMD_PAGE_PROGRAM,
                              offset, chunk, pp);

    /* Wait for status and check errors.*/
    err = n25q_poll_status(self);

    if (err == FLASH_NO_ERROR) {
      /* Next page.*/
      offset += chunk;
      pp     += chunk;
      n      -= chunk;
    }
  }

  __xsnor_bus_release(self);

  return err;
}

/**
 * @brief       Override of method @p __flash_start_erase_all().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @return                      An error code.
 */
flash_error_t __n25q_start_erase_all_impl(void *ip) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;

  osalDbgAssert(self->state == FLASH_ERASE, "invalid state");

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  /* Enabling write operation.*/
  __xsnor_bus_cmd(self, CMD_WRITE_ENABLE);

  /* Bulk erase command.*/
  __xsnor_bus_cmd(self, CMD_BULK_ERASE);

  __xsnor_bus_release(self);

  return FLASH_NO_ERROR;
}

/**
 * @brief       Override of method @p __flash_start_erase_sector().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[in]     sector        Sector to be erased.
 * @return                      An error code.
 */
flash_error_t __n25q_start_erase_sector_impl(void *ip, flash_sector_t sector) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  flash_offset_t offset = (flash_offset_t)(sector * self->descriptor.sectors_size);

  osalDbgAssert(self->state == FLASH_ERASE, "invalid state");

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  /* Enabling write operation.*/
  __xsnor_bus_cmd(self, CMD_WRITE_ENABLE);

  /* Sector erase command.*/
  if ((xsnorcfg->options & N25Q_OPT_USE_SUBSECTORS) != 0U) {
    __xsnor_bus_cmd_addr(self,
                         n25q_spi_4bytes(self) ?
                         CMD_SUBSECTOR_ERASE4B : CMD_SUBSECTOR_ERASE,
                         offset);
  }
  else {
    __xsnor_bus_cmd_addr(self,
                         n25q_spi_4bytes(self) ?
                         CMD_SECTOR_ERASE4B : CMD_SECTOR_ERASE,
                         offset);
  }

  __xsnor_bus_release(self);

  return FLASH_NO_ERROR;
}

/**
 * @brief       Override of method @p __flash_query_erase().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[out]    msec          Recommended time, in milliseconds, that should
 *                              be spent before calling this function again,
 *                              can be @p NULL
 * @return                      An error code.
 */
flash_error_t __n25q_query_erase_impl(void *ip, unsigned *msec) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgAssert(self->state == FLASH_ERASE, "invalid state");

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  /* Read status command.*/
  __xsnor_bus_cmd_receive(self, CMD_READ_FLAG_STATUS_REGISTER,
                          1, &xsnorcfg->buffers->databuf[0]);

  /* If the P/E bit is zero (busy) or the flash in a suspended state then
     report that the operation is still in progress.*/
  if (((xsnorcfg->buffers->databuf[0] & FLAGS_PROGRAM_ERASE) == 0U) ||
      ((xsnorcfg->buffers->databuf[0] & FLAGS_ERASE_SUSPEND) != 0U)) {

    /* Recommended time before polling again, this is a simplified
       implementation.*/
    if (msec != NULL) {
      *msec = 1U;
    }

    err = FLASH_BUSY_ERASING;
  }

  /* Checking for errors.*/
  if ((err == FLASH_NO_ERROR) &&
      ((xsnorcfg->buffers->databuf[0] & FLAGS_ALL_ERRORS) != 0U)) {

    /* Clearing status register.*/
    __xsnor_bus_cmd(self, CMD_CLEAR_FLAG_STATUS_REGISTER);

    /* Erase operation failed.*/
    err = FLASH_ERROR_ERASE;
  }

  __xsnor_bus_release(self);

  return err;
}

/**
 * @brief       Override of method @p __flash_verify_erase().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[in]     sector        Sector to be verified.
 * @return                      An error code.
 */
flash_error_t __n25q_verify_erase_impl(void *ip, flash_sector_t sector) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;
  flash_offset_t offset;
  size_t n;
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgAssert(self->state == FLASH_READ, "invalid state");

  /* Read command.*/
  offset = (flash_offset_t)(sector * self->descriptor.sectors_size);
  n = self->descriptor.sectors_size;

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  while ((n > 0U) && (err == FLASH_NO_ERROR)) {
    uint8_t *p;
    size_t chunk;

    chunk = n;
    if (chunk > XSNOR_BUFFER_SIZE) {
      chunk = XSNOR_BUFFER_SIZE;
    }

    err = n25q_read_unlocked(self, offset, chunk, &xsnorcfg->buffers->databuf[0]);
    if (err != FLASH_NO_ERROR) {
      break;
    }

    /* Checking for erased state of current buffer.*/
    for (p = &xsnorcfg->buffers->databuf[0];
         p < &xsnorcfg->buffers->databuf[chunk];
         p++) {

      if (*p != 0xFFU) {
        err = FLASH_ERROR_VERIFY;
        break;
      }
    }

    offset += chunk;
    n -= chunk;
  }

  __xsnor_bus_release(self);

  return err;
}

/**
 * @brief       Override of method @p xsnor_device_mmap_on().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 * @param[out]    addrp         Pointer to the memory mapped memory or @p NULL
 * @return                      An error code.
 */
flash_error_t __n25q_mmap_on_impl(void *ip, uint8_t **addrp) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;

#if XSNOR_USE_BOTH == TRUE
  if (xsnorcfg->bus_type == XSNOR_BUS_MODE_SPI) {
    return FLASH_ERROR_UNIMPLEMENTED;
  }
#endif

#if XSNOR_USE_WSPI == FALSE
  (void)self;
  (void)addrp;
  return FLASH_ERROR_UNIMPLEMENTED;
#else /* XSNOR_USE_WSPI == TRUE */
  {
    wspi_command_t cmd;

    /* Activating XIP mode in the device.*/
    n25q_activate_xip(self);

    /* Starting WSPI memory mapped mode.*/
    cmd.cfg   = (n25q_spi_4bytes(self) ? WSPI_CFG_ADDR_SIZE_32 :
                 WSPI_CFG_ADDR_SIZE_24) |
                WSPI_CFG_ALT_MODE_FOUR_LINES |  /* Always 4 lines, note.*/
                WSPI_CFG_ALT_SIZE_8 |
                WSPI_CFG_SIOO;
    cmd.cmd   = n25q_spi_4bytes(self) ? CMD_FAST_READ4B : CMD_FAST_READ;
    cmd.addr  = 0U;
    cmd.alt   = 0U;
    cmd.dummy = (xsnorcfg->options & N25Q_OPT_DUMMY_CYCLES_MASK) - 2U;

    /* Variant part of the command configuration.*/
    switch (xsnorcfg->bus_type) {
    case XSNOR_BUS_MODE_WSPI_1LINE:
      cmd.cfg |= WSPI_CFG_CMD_MODE_ONE_LINE |
                 WSPI_CFG_ADDR_MODE_ONE_LINE |
                 WSPI_CFG_DATA_MODE_ONE_LINE;
      break;
    case XSNOR_BUS_MODE_WSPI_2LINES:
      cmd.cfg |= WSPI_CFG_CMD_MODE_TWO_LINES |
                 WSPI_CFG_ADDR_MODE_TWO_LINES |
                 WSPI_CFG_DATA_MODE_TWO_LINES;
      break;
    case XSNOR_BUS_MODE_WSPI_4LINES:
      cmd.cfg |= WSPI_CFG_CMD_MODE_FOUR_LINES |
                 WSPI_CFG_ADDR_MODE_FOUR_LINES |
                 WSPI_CFG_DATA_MODE_FOUR_LINES;
      break;
    default:
      osalDbgAssert(false, "invalid bus type");
      return FLASH_ERROR_HW_FAILURE;
    }
    wspiMapFlash(xsnorcfg->bus.wspi.drv, &cmd, addrp);

    return FLASH_NO_ERROR;
  }
#endif
}

/**
 * @brief       Override of method @p xsnor_device_mmap_off().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_micron_n25q_c
 *                              instance.
 */
void __n25q_mmap_off_impl(void *ip) {
  hal_xsnor_micron_n25q_c *self = (hal_xsnor_micron_n25q_c *)ip;

#if XSNOR_USE_WSPI == FALSE
  (void)self;
#endif

#if XSNOR_USE_WSPI == TRUE
  /* Stopping WSPI memory mapped mode.*/
  wspiUnmapFlash(xsnorcfg->bus.wspi.drv);

  n25q_reset_xip(self);
#endif
}
/** @} */

/**
 * @brief       VMT structure of SNOR Micron N25Q driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_xsnor_micron_n25q_vmt __hal_xsnor_micron_n25q_vmt = {
  .dispose                  = __n25q_dispose_impl,
  .start                    = __xsnor_start_impl,
  .stop                     = __xsnor_stop_impl,
  .setcfg                   = __n25q_setcfg_impl,
  .selcfg                   = __xsnor_selcfg_impl,
  .read                     = __n25q_read_impl,
  .program                  = __n25q_program_impl,
  .start_erase_all          = __n25q_start_erase_all_impl,
  .start_erase_sector       = __n25q_start_erase_sector_impl,
  .query_erase              = __n25q_query_erase_impl,
  .verify_erase             = __n25q_verify_erase_impl,
  .init                     = __n25q_init_impl,
  .mmap_on                  = __n25q_mmap_on_impl,
  .mmap_off                 = __n25q_mmap_off_impl
};

#undef PAGE_SIZE
#undef PAGE_MASK
#undef xsnorcfg

/** @} */
