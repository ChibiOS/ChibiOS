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
 * @file    hal_jesd216_flash.c
 * @brief   JESD216 compliant flash driver class code.
 *
 * @addtogroup HAL_JESD216_FLASH
 * @{
 */

#include "hal.h"

#include "hal_jesd216_flash.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void jesd216_start(BUSDriver *busp, const BUSConfig *config) {

#if JESD216_BUS_MODE == JESD216_BUS_MODE_SPI
  spiStart(busp, config);
#else
  qspiStart(busp, config);
#endif
}

void jesd216_stop(BUSDriver *busp) {

#if JESD216_BUS_MODE == JESD216_BUS_MODE_SPI
  spiStop(busp);
#else
  qspiStop(busp);
#endif
}

void jesd216_cmd(BUSDriver *busp, uint32_t cmd) {
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES;
#endif
  mode.addr = 0U;
  mode.alt  = 0U;
  qspiCommand(busp, &mode);
#else
  uint8_t buf[1];

  spiSelect(busp);
  buf[0] = cmd;
  spiSend(busp, 1, buf);
  spiUnselect(busp);
#endif
}

void jesd216_cmd_receive(BUSDriver *busp,
                         uint32_t cmd,
                         size_t n,
                         uint8_t *p) {
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = 0U;
  mode.alt  = 0U;
  qspiReceive(busp, &mode, n, p);
#else
  uint8_t buf[1];

  spiSelect(busp);
  buf[0] = cmd;
  spiSend(busp, 1, buf);
  spiReceive(busp, n, p);
  spiUnselect(busp);
#endif
}

void jesd216_cmd_send(BUSDriver *busp,
                      uint32_t cmd,
                      size_t n,
                      const uint8_t *p) {
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES;

#endif
  mode.addr = 0U;
  mode.alt  = 0U;
  qspiSend(busp, &mode, n, p);
#else
  uint8_t buf[1];

  spiSelect(busp);
  buf[0] = cmd;
  spiSend(busp, 1, buf);
  spiSend(busp, n, p);
  spiUnselect(busp);
#endif
}

void jesd216_cmd_addr(BUSDriver *busp,
                      uint32_t cmd,
                      flash_offset_t offset) {
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_ADDR_SIZE_24;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_ADDR_SIZE_24;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_SIZE_24;

#endif
  mode.addr = offset;
  mode.alt  = 0U;
  qspiCommand(busp, &mode);
#else
  uint8_t buf[4];

  spiSelect(busp);
  buf[0] = cmd;
  buf[1] = (uint8_t)(offset >> 16);
  buf[2] = (uint8_t)(offset >> 8);
  buf[3] = (uint8_t)(offset >> 0);
  spiSend(busp, 4, buf);
  spiUnselect(busp);
#endif
}

void jesd216_cmd_addr_send(BUSDriver *busp,
                           uint32_t cmd,
                           flash_offset_t offset,
                           size_t n,
                           const uint8_t *p) {
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd & 0xFFU) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES;
#endif

  /* Handling 32 bits addressing.*/
  if ((cmd & JESD216_CMD_EXTENDED_ADDRESSING) == 0) {
    mode .cfg |= QSPI_CFG_ADDR_SIZE_24;
  }
  else {
    mode .cfg |= QSPI_CFG_ADDR_SIZE_32;
  }

  mode.addr = offset;
  mode.alt  = 0U;
  qspiSend(busp, &mode, n, p);
#else
  uint8_t buf[4];

  spiSelect(busp);
  buf[0] = cmd;
  buf[1] = (uint8_t)(offset >> 16);
  buf[2] = (uint8_t)(offset >> 8);
  buf[3] = (uint8_t)(offset >> 0);
  spiSend(busp, 4, buf);
  spiSend(busp, n, p);
  spiUnselect(busp);
#endif
}

void jesd216_cmd_addr_receive(BUSDriver *busp,
                              uint32_t cmd,
                              flash_offset_t offset,
                              size_t n,
                              uint8_t *p) {
#if JESD216_BUS_MODE != JESD216_BUS_MODE_SPI
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd & 0xFFU) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_ADDR_SIZE_24 |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_DATA_MODE_FOUR_LINES;
#endif

  /* Handling 32 bits addressing.*/
  if ((cmd & JESD216_CMD_EXTENDED_ADDRESSING) == 0) {
    mode .cfg |= QSPI_CFG_ADDR_SIZE_24;
  }
  else {
    mode .cfg |= QSPI_CFG_ADDR_SIZE_32;
  }

  mode.addr = offset;
  mode.alt  = 0U;
  qspiReceive(busp, &mode, n, p);
#else
  uint8_t buf[4];

  spiSelect(busp);
  buf[0] = cmd;
  buf[1] = (uint8_t)(offset >> 16);
  buf[2] = (uint8_t)(offset >> 8);
  buf[3] = (uint8_t)(offset >> 0);
  spiSend(busp, 4, buf);
  spiReceive(busp, n, p);
  spiUnselect(busp);
#endif
}

#if (JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) || defined(__DOXYGEN__)
void jesd216_cmd_addr_dummy_receive(BUSDriver *busp,
                                    uint32_t cmd,
                                    flash_offset_t offset,
                                    uint8_t dummy,
                                    size_t n,
                                    uint8_t *p) {
  qspi_command_t mode;

  mode.cfg = QSPI_CFG_CMD(cmd & 0xFFU) |
#if JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI1L
             QSPI_CFG_CMD_MODE_ONE_LINE |
             QSPI_CFG_ADDR_MODE_ONE_LINE |
             QSPI_CFG_DUMMY_CYCLES(dummy) |
             QSPI_CFG_DATA_MODE_ONE_LINE;
#elif JESD216_BUS_MODE == JESD216_BUS_MODE_QSPI2L
             QSPI_CFG_CMD_MODE_TWO_LINES |
             QSPI_CFG_ADDR_MODE_TWO_LINES |
             QSPI_CFG_DUMMY_CYCLES(dummy) |
             QSPI_CFG_DATA_MODE_TWO_LINES;
#else
             QSPI_CFG_CMD_MODE_FOUR_LINES |
             QSPI_CFG_ADDR_MODE_FOUR_LINES |
             QSPI_CFG_DUMMY_CYCLES(dummy) |
             QSPI_CFG_DATA_MODE_FOUR_LINES;
#endif

  /* Handling 32 bits addressing.*/
  if ((cmd & JESD216_CMD_EXTENDED_ADDRESSING) == 0) {
    mode .cfg |= QSPI_CFG_ADDR_SIZE_24;
  }
  else {
    mode .cfg |= QSPI_CFG_ADDR_SIZE_32;
  }

  mode.addr = offset;
  mode.alt  = 0U;
  qspiReceive(busp, &mode, n, p);
}
#endif /* JESD216_BUS_MODE != JESD216_BUS_MODE_SPI */

#if ((JESD216_BUS_MODE != JESD216_BUS_MODE_SPI) &&                          \
     (JESD216_SHARED_BUS == TRUE)) || defined(__DOXYGEN__)
void jesd216_bus_acquire(BUSDriver *busp, const BUSConfig *config) {

  (void)config;

  qspiAcquireBus(busp);
  if (busp->config != config) {
    qspiStart(busp, config);
  }
}

void jesd216_bus_release(BUSDriver *busp) {

  qspiReleaseBus(busp);
}
#elif (JESD216_BUS_MODE == JESD216_BUS_MODE_SPI) &&                         \
      (JESD216_SHARED_BUS == TRUE)
void jesd216_bus_acquire(BUSDriver *busp, const BUSConfig *config) {

  spiAcquireBus(busp);
  if (busp->config != config) {
    spiStart(busp, config);
  }
}

void jesd216_bus_release(BUSDriver *busp) {

  spiReleaseBus(busp);
}
#else
#define jesd216_bus_acquire(busp)
#define jesd216_bus_release(busp)
#endif

/** @} */
