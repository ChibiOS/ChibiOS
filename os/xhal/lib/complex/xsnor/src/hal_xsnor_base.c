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
 * @file        hal_xsnor_base.c
 * @brief       Generated SNOR Base Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_XSNOR_BASE
 * @{
 */

#include "hal.h"
#include "hal_xsnor_base.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       Checks if an identifier is in a supported identifiers table.
 *
 * @param[in]     set           Pointer to the identifiers table.
 * @param[in]     size          Number of elements in the identifiers table.
 * @param[in]     element       Identifier to be searched.
 * @return                      The search result.
 * @retval true                 If the identifier is present.
 * @retval false                If the identifier is not present.
 */
bool __xsnor_find_id(const uint8_t *set, size_t size, uint8_t element) {
  size_t i;

  for (i = 0U; i < size; i++) {
    if (set[i] == element) {
      return true;
    }
  }

  return false;
}

/*===========================================================================*/
/* Module class "hal_xsnor_base_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_xsnor_base_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_xsnor_base_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__xsnor_objinit_impl(void *ip, const void *vmt) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __flash_objinit_impl(self, vmt);

  /* Initialization code.*/
#if XSNOR_USE_WSPI == TRUE
  self->commands = NULL;
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance to be
 *                              disposed.
 */
void __xsnor_dispose_impl(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __flash_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __xsnor_start_impl(void *ip, const void *config) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  flash_error_t err;
  const xsnor_config_t *xcfg;

  if (config != NULL) {
    self->config = __drv_set_cfg(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  xcfg = self->config;
  if (xcfg == NULL) {
    return HAL_RET_CONFIG_ERROR;
  }

  /* Bus acquisition and selection.*/
  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    self->config = NULL;
    return HAL_RET_HW_FAILURE;
  }

#if XSNOR_SHARED_BUS != TRUE
#if XSNOR_USE_BOTH == TRUE
  if (xcfg->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    msg_t msg;

    msg = drvStart(xcfg->bus.wspi.drv, xcfg->bus.wspi.cfg);
    if (msg != HAL_RET_SUCCESS) {
      self->config = NULL;
      return msg;
    }
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
    msg_t msg;

    msg = drvStart(xcfg->bus.spi.drv, xcfg->bus.spi.cfg);
    if (msg != HAL_RET_SUCCESS) {
      self->config = NULL;
      return msg;
    }
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
#endif

  /* Device identification and initialization.*/
  err = xsnor_device_init(self);

  /* Bus release.*/
  __xsnor_bus_release(self);

  if (err != FLASH_NO_ERROR) {
#if XSNOR_SHARED_BUS != TRUE
#if XSNOR_USE_BOTH == TRUE
    if (xcfg->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
      drvStop(xcfg->bus.wspi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
    }
    else {
#endif
#if XSNOR_USE_SPI == TRUE
      drvStop(xcfg->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
    }
#endif
#endif
    self->config = NULL;
    return HAL_RET_HW_FAILURE;
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 */
void __xsnor_stop_impl(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

  if (config == NULL) {
    return;
  }

  /* Bus acquisition and selection.*/
  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return;
  }

#if XSNOR_SHARED_BUS != TRUE
  /* Stopping bus device.*/
#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    drvStop(config->bus.wspi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
    drvStop(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
#endif

  /* Bus release.*/
  __xsnor_bus_release(self);
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__xsnor_setcfg_impl(void *ip, const void *config) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *xcfg = (const xsnor_config_t *)config;

  (void)self;

  if ((xcfg == NULL) || (xcfg->buffers == NULL)) {
    return NULL;
  }

#if XSNOR_USE_BOTH == TRUE
  switch (xcfg->bus_type) {
  case XSNOR_BUS_MODE_SPI:
    if (xcfg->bus.spi.drv == NULL) {
      return NULL;
    }
    break;
  case XSNOR_BUS_MODE_WSPI_1LINE:
  case XSNOR_BUS_MODE_WSPI_2LINES:
  case XSNOR_BUS_MODE_WSPI_4LINES:
  case XSNOR_BUS_MODE_WSPI_8LINES:
    if (xcfg->bus.wspi.drv == NULL) {
      return NULL;
    }
    break;
  default:
    return NULL;
  }
#else
#if XSNOR_USE_SPI == TRUE
  if ((xcfg->bus_type != XSNOR_BUS_MODE_SPI) ||
      (xcfg->bus.spi.drv == NULL)) {
    return NULL;
  }
#endif
#if XSNOR_USE_WSPI == TRUE
  switch (xcfg->bus_type) {
  case XSNOR_BUS_MODE_WSPI_1LINE:
  case XSNOR_BUS_MODE_WSPI_2LINES:
  case XSNOR_BUS_MODE_WSPI_4LINES:
  case XSNOR_BUS_MODE_WSPI_8LINES:
    if (xcfg->bus.wspi.drv == NULL) {
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
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__xsnor_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  (void)self;
  (void)cfgnum;

  return NULL;
}
/** @} */

/**
 * @name        Regular methods of hal_xsnor_base_c
 * @{
 */
#if (XSNOR_USE_SPI == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Sends command and address over SPI.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     offset        Flash offset.
 */
void __xsnor_spi_cmd_addr(void *ip, uint32_t cmd, flash_offset_t offset) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

  config->buffers->spibuf[0] = cmd;
  if ((self->descriptor.attributes & FLASH_ATTR_SPI_4BYTES_ADDR_HINT) != 0U) {
    config->buffers->spibuf[1] = (uint8_t)(offset >> 24);
    config->buffers->spibuf[2] = (uint8_t)(offset >> 16);
    config->buffers->spibuf[3] = (uint8_t)(offset >> 8);
    config->buffers->spibuf[4] = (uint8_t)(offset >> 0);
    spiSend(config->bus.spi.drv, 5, config->buffers->spibuf);
  }
  else {
    config->buffers->spibuf[1] = (uint8_t)(offset >> 16);
    config->buffers->spibuf[2] = (uint8_t)(offset >> 8);
    config->buffers->spibuf[3] = (uint8_t)(offset >> 0);
    spiSend(config->bus.spi.drv, 4, config->buffers->spibuf);
  }
}
#endif /* XSNOR_USE_SPI == TRUE */

#if (XSNOR_SHARED_BUS == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Bus acquisition and lock.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 */
void __xsnor_bus_acquire(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if HAL_USE_MUTUAL_EXCLUSION != TRUE
  (void)self;
  (void)config;
#endif

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
#if HAL_USE_MUTUAL_EXCLUSION == TRUE
    drvLock(config->bus.wspi.drv);
#endif
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
#if HAL_USE_MUTUAL_EXCLUSION == TRUE
    drvLock(config->bus.spi.drv);
#endif
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Bus selection and configuration.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 */
msg_t __xsnor_bus_select(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    return drvStart(config->bus.wspi.drv, config->bus.wspi.cfg);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
    return drvStart(config->bus.spi.drv, config->bus.spi.cfg);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Bus release and unlock.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 */
void __xsnor_bus_release(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if HAL_USE_MUTUAL_EXCLUSION != TRUE
  (void)self;
  (void)config;
#endif

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
#if HAL_USE_MUTUAL_EXCLUSION == TRUE
    drvUnlock(config->bus.wspi.drv);
#endif
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
#if HAL_USE_MUTUAL_EXCLUSION == TRUE
    drvUnlock(config->bus.spi.drv);
#endif
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}
#endif /* XSNOR_SHARED_BUS == TRUE */

/**
 * @brief       Sends a naked command.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 */
void __xsnor_bus_cmd(void *ip, uint32_t cmd) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd;
    mode.addr  = 0U;
    mode.alt   = 0U;
    mode.dummy = 0U;
    wspiCommand(config->bus.wspi.drv, &mode);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE

    spiSelectX(config->bus.spi.drv);
    config->buffers->spibuf[0] = cmd;
    spiSend(config->bus.spi.drv, 1, config->buffers->spibuf);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a command followed by a data transmit phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     n             Number of bytes to transmit.
 * @param[in]     p             Data buffer.
 */
void __xsnor_bus_cmd_send(void *ip, uint32_t cmd, size_t n, const uint8_t *p) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_data;
    mode.addr  = 0U;
    mode.alt   = 0U;
    mode.dummy = 0U;
    wspiSend(config->bus.wspi.drv, &mode, n, p);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE

    spiSelectX(config->bus.spi.drv);
    config->buffers->spibuf[0] = cmd;
    spiSend(config->bus.spi.drv, 1, config->buffers->spibuf);
    spiSend(config->bus.spi.drv, n, p);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a command followed by a data receive phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     n             Number of bytes to receive.
 * @param[out]    p             Data buffer.
 */
void __xsnor_bus_cmd_receive(void *ip, uint32_t cmd, size_t n, uint8_t *p) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_data;
    mode.addr  = 0U;
    mode.alt   = 0U;
    mode.dummy = 0U;
    wspiReceive(config->bus.wspi.drv, &mode, n, p);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE

    spiSelectX(config->bus.spi.drv);
    config->buffers->spibuf[0] = cmd;
    spiSend(config->bus.spi.drv, 1, config->buffers->spibuf);
    spiReceive(config->bus.spi.drv, n, p);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a command followed by a flash address.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     offset        Flash offset.
 */
void __xsnor_bus_cmd_addr(void *ip, uint32_t cmd, flash_offset_t offset) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_addr;
    mode.addr  = offset;
    mode.alt   = 0U;
    mode.dummy = 0U;
    wspiCommand(config->bus.wspi.drv, &mode);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE

    spiSelectX(config->bus.spi.drv);
    __xsnor_spi_cmd_addr(self, cmd, offset);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a command followed by a flash address and a data transmit
 *              phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to transmit.
 * @param[in]     p             Data buffer.
 */
void __xsnor_bus_cmd_addr_send(void *ip, uint32_t cmd, flash_offset_t offset,
                               size_t n, const uint8_t *p) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_addr_data;
    mode.addr  = offset;
    mode.alt   = 0U;
    mode.dummy = 0U;
    wspiSend(config->bus.wspi.drv, &mode, n, p);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE

    spiSelectX(config->bus.spi.drv);
    __xsnor_spi_cmd_addr(self, cmd, offset);
    spiSend(config->bus.spi.drv, n, p);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a command followed by a flash address and a data receive
 *              phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     offset        Flash offset.
 * @param[in]     n             Number of bytes to receive.
 * @param[in]     p             Data buffer.
 */
void __xsnor_bus_cmd_addr_receive(void *ip, uint32_t cmd,
                                  flash_offset_t offset, size_t n, uint8_t *p) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_addr_data;
    mode.addr  = offset;
    mode.alt   = 0U;
    mode.dummy = 0U;
    wspiReceive(config->bus.wspi.drv, &mode, n, p);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE

    spiSelectX(config->bus.spi.drv);
    __xsnor_spi_cmd_addr(self, cmd, offset);
    spiReceive(config->bus.spi.drv, n, p);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a command followed by dummy cycles and a data receive
 *              phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     dummy         Number of dummy cycles.
 * @param[in]     n             Number of bytes to receive.
 * @param[out]    p             Data buffer.
 */
void __xsnor_bus_cmd_dummy_receive(void *ip, uint32_t cmd, uint32_t dummy,
                                   size_t n, uint8_t *p) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_data;
    mode.addr  = 0U;
    mode.alt   = 0U;
    mode.dummy = dummy;
    wspiReceive(config->bus.wspi.drv, &mode, n, p);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
    osalDbgAssert((dummy & 7) == 0U, "multiple of 8 dummy cycles");

    spiSelectX(config->bus.spi.drv);
    config->buffers->spibuf[0] = cmd;
    spiSend(config->bus.spi.drv, 1, config->buffers->spibuf);
    if (dummy != 0U) {
      spiIgnore(config->bus.spi.drv, dummy / 8U);
    }
    spiReceive(config->bus.spi.drv, n, p);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

/**
 * @brief       Sends a complete header followed by a data receive phase.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[in]     cmd           Instruction code.
 * @param[in]     offset        Flash offset.
 * @param[in]     dummy         Number of dummy cycles.
 * @param[in]     n             Number of bytes to receive.
 * @param[out]    p             Data buffer.
 */
void __xsnor_bus_cmd_addr_dummy_receive(void *ip, uint32_t cmd,
                                        flash_offset_t offset, uint32_t dummy,
                                        size_t n, uint8_t *p) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  const xsnor_config_t *config = self->config;

#if XSNOR_USE_BOTH == TRUE
  if (config->bus_type != XSNOR_BUS_MODE_SPI) {
#endif
#if XSNOR_USE_WSPI == TRUE
    wspi_command_t mode;

    mode.cmd   = cmd;
    mode.cfg   = self->commands->cmd_addr_data;
    mode.addr  = offset;
    mode.alt   = 0U;
    mode.dummy = dummy;
    wspiReceive(config->bus.wspi.drv, &mode, n, p);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
  else {
#endif
#if XSNOR_USE_SPI == TRUE
    osalDbgAssert((dummy & 7) == 0U, "multiple of 8 dummy cycles");

    spiSelectX(config->bus.spi.drv);
    __xsnor_spi_cmd_addr(self, cmd, offset);
    if (dummy != 0U) {
      spiIgnore(config->bus.spi.drv, dummy / 8U);
    }
    spiReceive(config->bus.spi.drv, n, p);
    spiUnselectX(config->bus.spi.drv);
#endif
#if XSNOR_USE_BOTH == TRUE
  }
#endif
}

#if ((XSNOR_USE_WSPI == TRUE) && defined(WSPI_SUPPORTS_MEMMAP) && (WSPI_SUPPORTS_MEMMAP == TRUE)) || defined (__DOXYGEN__)
/**
 * @brief       Enters the memory mapped mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 * @param[out]    addrp         Pointer to the memory mapped memory or @p NULL
 * @return                      An error code.
 * @retval FLASH_NO_ERROR       Operation successful.
 * @retval FLASH_ERROR_HW_FAILURE If memory mapped mode failed.
 *
 * @api
 */
flash_error_t xsnorMemoryMap(void *ip, uint8_t **addrp) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  flash_error_t err;

  osalDbgCheck((self != NULL) && (addrp != NULL));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  __xsnor_bus_acquire(self);
  if (__xsnor_bus_select(self) != HAL_RET_SUCCESS) {
    __xsnor_bus_release(self);
    return FLASH_ERROR_HW_FAILURE;
  }

  self->state = FLASH_MMAP;

  /* Activating XIP mode in the device.*/
  err = xsnor_device_mmap_on(self, addrp);
  if (err != FLASH_NO_ERROR) {
    self->state = HAL_DRV_STATE_READY;
    __xsnor_bus_release(self);
  }

  return err;
}

/**
 * @brief       Leaves the memory mapped mode.
 *
 * @param[in,out] ip            Pointer to a @p hal_xsnor_base_c instance.
 *
 * @api
 */
void xsnorMemoryUnmap(void *ip) {
  hal_xsnor_base_c *self = (hal_xsnor_base_c *)ip;
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == FLASH_MMAP, "invalid state");

  xsnor_device_mmap_off(self);

  self->state = HAL_DRV_STATE_READY;
  __xsnor_bus_release(self);
}
#endif /* (XSNOR_USE_WSPI == TRUE) && defined(WSPI_SUPPORTS_MEMMAP) && (WSPI_SUPPORTS_MEMMAP == TRUE) */
/** @} */

/** @} */
