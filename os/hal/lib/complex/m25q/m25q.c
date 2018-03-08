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
 * @file    m25q.c
 * @brief   M25Q serial flash driver code.
 *
 * @addtogroup M25Q
 * @ingroup M25Q
 * @{
 */

#include "hal.h"
#include "m25q.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static flash_error_t m25q_read(void *instance, flash_offset_t offset,
                               size_t n, uint8_t *rp);
static flash_error_t m25q_program(void *instance, flash_offset_t offset,
                                  size_t n, const uint8_t *pp);
static flash_error_t m25q_start_erase_all(void *instance);
static flash_error_t m25q_start_erase_sector(void *instance,
                                             flash_sector_t sector);
static flash_error_t m25q_verify_erase(void *instance,
                                       flash_sector_t sector);
static flash_error_t m25q_query_erase(void *instance, uint32_t *msec);
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

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static flash_error_t m25q_read(void *instance, flash_offset_t offset,
                               size_t n, uint8_t *rp) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

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

  /* Actual read implementation.*/
  err = m25q_device_read(devp, offset, n, rp);

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return err;
}

static flash_error_t m25q_program(void *instance, flash_offset_t offset,
                                  size_t n, const uint8_t *pp) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

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

  /* Actual program implementation.*/
  err = m25q_device_program(devp, offset, n, pp);

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return err;
}

static flash_error_t m25q_start_erase_all(void *instance) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

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

  /* Actual erase implementation.*/
  err = m25q_device_start_erase_all(devp);

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return err;
}

static flash_error_t m25q_start_erase_sector(void *instance,
                                             flash_sector_t sector) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

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

  /* Actual erase implementation.*/
  err = m25q_device_start_erase_sector(devp, sector);

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return err;
}

static flash_error_t m25q_verify_erase(void *instance,
                                       flash_sector_t sector) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

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

  /* Actual verify erase implementation.*/
  err = m25q_device_verify_erase(devp, sector);

  /* Ready state again.*/
  devp->state = FLASH_READY;

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return err;
}

static flash_error_t m25q_query_erase(void *instance, uint32_t *msec) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* If there is an erase in progress then the device must be checked.*/
  if (devp->state == FLASH_ERASE) {

    /* Bus acquired.*/
    jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

    /* Actual query erase implementation.*/
    err = m25q_device_query_erase(devp, msec);

    /* The device is ready to accept commands.*/
    if (err == FLASH_NO_ERROR) {
      devp->state = FLASH_READY;
    }

    /* Bus released.*/
    jesd216_bus_release(devp->config->busp);
  }
  else {
    err = FLASH_NO_ERROR;
  }

  return err;
}

static flash_error_t m25q_read_sfdp(void *instance, flash_offset_t offset,
                                    size_t n, uint8_t *rp) {
  M25QDriver *devp = (M25QDriver *)instance;
  flash_error_t err;

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  /* Bus acquired.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* Actual read SFDP implementation.*/
  err = m25q_device_read_sfdp(devp, offset, n, rp);

  /* The device is ready to accept commands.*/
  if (err == FLASH_NO_ERROR) {
    devp->state = FLASH_READY;
  }

  /* Bus released.*/
  jesd216_bus_release(devp->config->busp);

  return err;
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

    /* Device identification and initialization.*/
    m25q_device_init(devp);

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
  qspi_command_t cmd;

  /* Bus acquisition.*/
  jesd216_bus_acquire(devp->config->busp, devp->config->buscfg);

  /* Activating XIP mode in the device.*/
  m25q_activate_xip(devp);

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

  /* Starting QSPI memory mapped mode.*/
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

  /* Stopping QSPI memory mapped mode.*/
  qspiUnmapFlash(devp->config->busp);

  m25q_reset_xip(devp);

  /* Bus release.*/
  jesd216_bus_release(devp->config->busp);
}
#endif /* QSPI_SUPPORTS_MEMMAP == TRUE */
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

/** @} */
