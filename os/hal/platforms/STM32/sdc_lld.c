/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32/sdc_lld.c
 * @brief   STM32 SDC subsystem low level driver source.
 *
 * @addtogroup SDC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SDCD1 driver identifier.*/
SDCDriver SDCD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   SDIO IRQ handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SDIO_IRQHandler) {

  CH_IRQ_PROLOGUE();


  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SDC driver initialization.
 *
 * @notapi
 */
void sdc_lld_init(void) {

  sdcObjectInit(&SDCD1);
}

/**
 * @brief   Configures and activates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start(SDCDriver *sdcp) {

  if (sdcp->state == SDC_STOP) {
    /* Note, the DMA must be enabled before the IRQs.*/
    dmaAllocate(STM32_DMA2_ID, STM32_DMA_CHANNEL_4, NULL, NULL);
    dmaChannelSetPeripheral(&STM32_DMA2->channels[STM32_DMA_CHANNEL_4], &SDIO->FIFO);
    NVICEnableVector(SDIO_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_SDC_SDIO_IRQ_PRIORITY));
    RCC->AHBENR |= RCC_AHBENR_SDIOEN;
  }
  /* Configuration, card clock is initially stopped.*/
  SDIO->POWER = 0;
  SDIO->CLKCR = 0;
  SDIO->DCTRL = 0;
}

/**
 * @brief   Deactivates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop(SDCDriver *sdcp) {

  if ((sdcp->state == SDC_READY) || (sdcp->state == SDC_ACTIVE)) {
    SDIO->POWER = 0;
    SDIO->CLKCR = 0;
    SDIO->DCTRL = 0;

    /* Clock deactivation.*/
    NVICDisableVector(SDIO_IRQn);
    dmaRelease(STM32_DMA2_ID, STM32_DMA_CHANNEL_4);
  }
}

/**
 * @brief   Starts the SDIO clock and sets it to init mode (400KHz or less).
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start_clk(SDCDriver *sdcp) {

  (void)sdcp;
  /* Initial clock setting: 400KHz, 1bit mode.*/
  SDIO->CLKCR  = STM32_SDIO_DIV_LS;
  SDIO->POWER |= SDIO_POWER_PWRCTRL_0 | SDIO_POWER_PWRCTRL_1;
  SDIO->CLKCR |= SDIO_CLKCR_CLKEN;
}

/**
 * @brief   Sets the SDIO clock to data mode (25MHz or less).
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_set_data_clk(SDCDriver *sdcp) {

  (void)sdcp;
  SDIO->CLKCR = (SDIO->CLKCR & 0xFFFFFF00) | STM32_SDIO_DIV_HS;
}

/**
 * @brief   Stops the SDIO clock.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop_clk(SDCDriver *sdcp) {

  (void)sdcp;
  SDIO->CLKCR = 0;
  SDIO->POWER = 0;
}

/**
 * @brief   Switches the bus to 4 bits mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_set_bus_mode(SDCDriver *sdcp, sdcbusmode_t mode) {
  uint32_t clk = SDIO->CLKCR & ~SDIO_CLKCR_WIDBUS;

  (void)sdcp;
  switch (mode) {
  case SDC_MODE_1BIT:
    SDIO->CLKCR = clk;
    break;
  case SDC_MODE_4BIT:
    SDIO->CLKCR = clk | SDIO_CLKCR_WIDBUS_0;
    break;
  case SDC_MODE_8BIT:
    SDIO->CLKCR = clk | SDIO_CLKCR_WIDBUS_1;
  }
}

/**
 * @brief   Sends an SDIO command with no response expected.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in[ cmd       card command
 * @param[in] arg       command argument
 *
 * @notapi
 */
void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg) {

  (void)sdcp;
  SDIO->ARG = arg;
  SDIO->CMD = (uint32_t)cmd | SDIO_CMD_CPSMEN;
  while ((SDIO->STA & SDIO_STA_CMDSENT) == 0)
    ;
  SDIO->ICR = 0xFFFFFFFF;
}

/**
 * @brief   Sends an SDIO command with a short response expected.
 * @note    The CRC is not verified.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in[ cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (one word)
 * @return              The operation status.
 * @retval FALSE        the operation succeeded.
 * @retval TRUE         the operation failed because timeout, CRC check or
 *                      other errors.
 *
 * @notapi
 */
bool_t sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                              uint32_t *resp) {
  uint32_t sta;

  (void)sdcp;
  SDIO->ARG = arg;
  SDIO->CMD = (uint32_t)cmd | SDIO_CMD_WAITRESP_0 | SDIO_CMD_CPSMEN;
  while (((sta = SDIO->STA) & (SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT |
                               SDIO_STA_CCRCFAIL)) == 0)
    ;
  SDIO->ICR = 0xFFFFFFFF;
  if ((sta & (SDIO_STA_CTIMEOUT)) != 0)
    return TRUE;
  *resp = SDIO->RESP1;
  return FALSE;
}

/**
 * @brief   Sends an SDIO command with a short response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in[ cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (one word)
 * @return              The operation status.
 * @retval FALSE        the operation succeeded.
 * @retval TRUE         the operation failed because timeout, CRC check or
 *                      other errors.
 *
 * @notapi
 */
bool_t sdc_lld_send_cmd_short_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                  uint32_t *resp) {
  uint32_t sta;

  (void)sdcp;
  SDIO->ARG = arg;
  SDIO->CMD = (uint32_t)cmd | SDIO_CMD_WAITRESP_0 | SDIO_CMD_CPSMEN;
  while (((sta = SDIO->STA) & (SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT |
                               SDIO_STA_CCRCFAIL)) == 0)
    ;
  SDIO->ICR = 0xFFFFFFFF;
  if ((sta & (SDIO_STA_CTIMEOUT | SDIO_STA_CCRCFAIL)) != 0)
    return TRUE;
  *resp = SDIO->RESP1;
  return FALSE;
}

/**
 * @brief   Sends an SDIO command with a long response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in[ cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (four words)
 * @return              The operation status.
 * @retval FALSE        the operation succeeded.
 * @retval TRUE         the operation failed because timeout, CRC check or
 *                      other errors.
 *
 * @notapi
 */
bool_t sdc_lld_send_cmd_long_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                 uint32_t *resp) {

  uint32_t sta;

  (void)sdcp;
  SDIO->ARG = arg;
  SDIO->CMD = (uint32_t)cmd | SDIO_CMD_WAITRESP_0 | SDIO_CMD_WAITRESP_1 |
                              SDIO_CMD_CPSMEN;
  while (((sta = SDIO->STA) & (SDIO_STA_CMDREND | SDIO_STA_CTIMEOUT |
                               SDIO_STA_CCRCFAIL)) == 0)
    ;
  SDIO->ICR = 0xFFFFFFFF;
  if ((sta & (SDIO_STA_CTIMEOUT | SDIO_STA_CCRCFAIL)) != 0)
    return TRUE;
  *resp = SDIO->RESP1;
  return FALSE;
}

/**
 * @brief   Data read through the SDIO bus.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] buf      pointer to the read buffer
 * @param[in] n         number of blocks to read
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      read.
 * @retval TRUE         operation failed, the state of the buffer is uncertain.
 *
 * @notapi
 */
bool_t sdc_lld_read_blocks(SDCDriver *sdcp, uint8_t *buf, uint32_t n) {

  return TRUE;
}

/**
 * @brief   Data write through the SDIO bus.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] buf      pointer to the write buffer
 * @param[in] n         number of blocks to write
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      written.
 * @retval TRUE         operation failed.
 *
 * @notapi
 */
bool_t sdc_lld_write_blocks(SDCDriver *sdcp, const uint8_t *buf, uint32_t n) {

  return TRUE;
}

#endif /* HAL_USE_SDC */

/** @} */
