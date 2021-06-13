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
 * @file    SDMMCv2/hal_sdc_lld.c
 * @brief   STM32 SDC subsystem low level driver source.
 *
 * @addtogroup SDC
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define SDMMC_ICR_ALL_FLAGS     0xFFFFFFFFU

#define SDMMC_STA_ERROR_MASK                                                \
  (SDMMC_STA_CCRCFAIL | SDMMC_STA_DCRCFAIL |                                \
   SDMMC_STA_CTIMEOUT | SDMMC_STA_DTIMEOUT |                                \
   SDMMC_STA_TXUNDERR | SDMMC_STA_RXOVERR)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SDCD1 driver identifier.*/
#if STM32_SDC_USE_SDMMC1 || defined(__DOXYGEN__)
SDCDriver SDCD1;
#endif

/** @brief SDCD2 driver identifier.*/
#if STM32_SDC_USE_SDMMC2 || defined(__DOXYGEN__)
SDCDriver SDCD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   SDIO default configuration.
 */
static const SDCConfig sdc_default_cfg = {
  SDC_MODE_4BIT
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Calculates a clock divider for the specified frequency.
 * @note    The divider is calculated to not exceed the required frequency
 *          in case of non-integer division.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] f         required frequency
 */
static uint32_t sdc_lld_clkdiv(SDCDriver *sdcp, uint32_t f) {

  if (f >= sdcp->clkfreq) {
    return 0;
  }

  return (sdcp->clkfreq + (f * 2) - 1) / (f * 2);
}

/**
 * @brief   Prepares to handle read transaction.
 * @details Designed for read special registers from card.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] buf      pointer to the read buffer
 * @param[in] bytes     number of bytes to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_prepare_read_bytes(SDCDriver *sdcp,
                                       uint8_t *buf, uint32_t bytes) {
  osalDbgCheck(bytes < 0x1000000);

  sdcp->sdmmc->DTIMER = STM32_SDC_SDMMC_READ_TIMEOUT;

  /* Checks for errors and waits for the card to be ready for reading.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return HAL_FAILED;

  /* Setting up data transfer.*/
  sdcp->sdmmc->ICR   = SDMMC_ICR_ALL_FLAGS;
  sdcp->sdmmc->MASK  = SDMMC_MASK_DCRCFAILIE |
                       SDMMC_MASK_DTIMEOUTIE |
                       SDMMC_MASK_RXOVERRIE |
                       SDMMC_MASK_DATAENDIE;
  sdcp->sdmmc->DLEN  = bytes;

  /* Transfer modes.*/
  sdcp->sdmmc->DCTRL = SDMMC_DCTRL_DTDIR |
                       SDMMC_DCTRL_DTMODE_0;    /* Multibyte data transfer.*/

  /* Prepares IDMA.*/
  sdcp->sdmmc->IDMABASE0 = (uint32_t)buf;
  sdcp->sdmmc->IDMACTRL  = SDMMC_IDMA_IDMAEN;

  return HAL_SUCCESS;
}

/**
 * @brief   Prepares card to handle read transaction.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[in] n         number of blocks to read
 * @param[in] resp      pointer to the response buffer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_prepare_read(SDCDriver *sdcp, uint32_t startblk,
                                 uint32_t n, uint32_t *resp) {

  /* Driver handles data in 512 bytes blocks (just like HC cards). But if we
     have not HC card than we must convert address from blocks to bytes.*/
  if (!(sdcp->cardmode & SDC_MODE_HIGH_CAPACITY))
    startblk *= MMCSD_BLOCK_SIZE;

  if (n > 1) {
    /* Send read multiple blocks command to card.*/
    if (sdc_lld_send_cmd_short_crc(sdcp, SDMMC_CMD_CMDTRANS | MMCSD_CMD_READ_MULTIPLE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
  }
  else {
    /* Send read single block command.*/
    if (sdc_lld_send_cmd_short_crc(sdcp, SDMMC_CMD_CMDTRANS | MMCSD_CMD_READ_SINGLE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Prepares card to handle write transaction.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[in] n         number of blocks to write
 * @param[in] resp      pointer to the response buffer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_prepare_write(SDCDriver *sdcp, uint32_t startblk,
                                  uint32_t n, uint32_t *resp) {

  /* Driver handles data in 512 bytes blocks (just like HC cards). But if we
     have not HC card than we must convert address from blocks to bytes.*/
  if (!(sdcp->cardmode & SDC_MODE_HIGH_CAPACITY))
    startblk *= MMCSD_BLOCK_SIZE;

  if (n > 1) {
    /* Write multiple blocks command.*/
    if (sdc_lld_send_cmd_short_crc(sdcp, SDMMC_CMD_CMDTRANS | MMCSD_CMD_WRITE_MULTIPLE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
  }
  else {
    /* Write single block command.*/
    if (sdc_lld_send_cmd_short_crc(sdcp, SDMMC_CMD_CMDTRANS | MMCSD_CMD_WRITE_BLOCK,
                                   startblk, resp) || MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Wait end of data transaction and performs finalizations.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] n         number of blocks in transaction
 * @param[in] resp      pointer to the response buffer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 */
static bool sdc_lld_wait_transaction_end(SDCDriver *sdcp, uint32_t n,
                                         uint32_t *resp) {

  /* Note the mask is checked before going to sleep because the interrupt
     may have occurred before reaching the critical zone.*/
  osalSysLock();
  if (sdcp->sdmmc->MASK != 0)
    osalThreadSuspendS(&sdcp->thread);

  /* Stopping operations.*/
  sdcp->sdmmc->IDMACTRL = 0;
  sdcp->sdmmc->MASK     = 0;
  sdcp->sdmmc->DCTRL    = 0;

  if ((sdcp->sdmmc->STA & SDMMC_STA_DATAEND) == 0) {
    osalSysUnlock();
    return HAL_FAILED;
  }

  /* Clearing status.*/
  sdcp->sdmmc->ICR      = SDMMC_ICR_ALL_FLAGS;
  osalSysUnlock();

  /* Finalize transaction.*/
  if (n > 1)
    return sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0, resp);

  return HAL_SUCCESS;
}

/**
 * @brief   Gets SDC errors.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] sta       value of the STA register
 *
 * @notapi
 */
static void sdc_lld_collect_errors(SDCDriver *sdcp, uint32_t sta) {
  uint32_t errors = SDC_NO_ERROR;

  if (sta & SDMMC_STA_CCRCFAIL)
    errors |= SDC_CMD_CRC_ERROR;
  if (sta & SDMMC_STA_DCRCFAIL)
    errors |= SDC_DATA_CRC_ERROR;
  if (sta & SDMMC_STA_CTIMEOUT)
    errors |= SDC_COMMAND_TIMEOUT;
  if (sta & SDMMC_STA_DTIMEOUT)
    errors |= SDC_DATA_TIMEOUT;
  if (sta & SDMMC_STA_TXUNDERR)
    errors |= SDC_TX_UNDERRUN;
  if (sta & SDMMC_STA_RXOVERR)
    errors |= SDC_RX_OVERRUN;
/*  if (sta & SDMMC_STA_STBITERR)
    errors |= SDC_STARTBIT_ERROR;*/

  sdcp->errors |= errors;
}

/**
 * @brief   Performs clean transaction stopping in case of errors.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] n         number of blocks in transaction
 * @param[in] resp      pointer to the response buffer
 *
 * @notapi
 */
static void sdc_lld_error_cleanup(SDCDriver *sdcp,
                                  uint32_t n,
                                  uint32_t *resp) {
  uint32_t sta = sdcp->sdmmc->STA;

  /* Clearing status.*/
  sta = sdcp->sdmmc->STA;
  sdcp->sdmmc->ICR = sta;
  sdc_lld_collect_errors(sdcp, sta);

  if (n > 1)
    sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0, resp);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SDC driver initialization.
 *
 * @notapi
 */
void sdc_lld_init(void) {

#if STM32_SDC_USE_SDMMC1
  sdcObjectInit(&SDCD1);
  SDCD1.thread  = NULL;
  SDCD1.sdmmc   = SDMMC1;
  SDCD1.clkfreq = STM32_SDMMC1CLK;
#endif

#if STM32_SDC_USE_SDMMC2
  sdcObjectInit(&SDCD2);
  SDCD2.thread  = NULL;
  SDCD2.sdmmc   = SDMMC2;
  SDCD2.clkfreq = STM32_SDMMC2CLK;
#endif
}

/**
 * @brief   Configures and activates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start(SDCDriver *sdcp) {

  /* Checking configuration, using a default if NULL has been passed.*/
  if (sdcp->config == NULL) {
    sdcp->config = &sdc_default_cfg;
  }

  /* If in stopped state then clocks are enabled and DMA initialized.*/
  if (sdcp->state == BLK_STOP) {
#if STM32_SDC_USE_SDMMC1
    if (&SDCD1 == sdcp) {
      rccEnableSDMMC1(true);
    }
#endif /* STM32_SDC_USE_SDMMC1 */

#if STM32_SDC_USE_SDMMC2
    if (&SDCD2 == sdcp) {
      rccEnableSDMMC2(true);
    }
#endif /* STM32_SDC_USE_SDMMC2 */
  }

  /* Configuration, card clock is initially stopped.*/
  sdcp->sdmmc->IDMACTRL = 0;
  sdcp->sdmmc->DCTRL    = 0;
  sdcp->sdmmc->POWER    = 0;
  sdcp->sdmmc->CLKCR    = 0;
  sdcp->sdmmc->DTIMER   = 0;
  sdcp->sdmmc->ICR      = SDMMC_ICR_ALL_FLAGS;
}

/**
 * @brief   Deactivates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop(SDCDriver *sdcp) {

  if (sdcp->state != BLK_STOP) {

    /* SDIO deactivation.*/
    sdcp->sdmmc->IDMACTRL = 0;
    sdcp->sdmmc->DCTRL    = 0;
    sdcp->sdmmc->POWER    = 0;
    sdcp->sdmmc->CLKCR    = 0;
    sdcp->sdmmc->DTIMER   = 0;
    sdcp->sdmmc->ICR      = SDMMC_ICR_ALL_FLAGS;

    /* Clock deactivation.*/
#if STM32_SDC_USE_SDMMC1
    if (&SDCD1 == sdcp) {
      rccDisableSDMMC1();
    }
#endif

#if STM32_SDC_USE_SDMMC2
    if (&SDCD2 == sdcp) {
      rccDisableSDMMC2();
    }
#endif
  }
}

/**
 * @brief   Starts the SDIO clock and sets it to init mode (400kHz or less).
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start_clk(SDCDriver *sdcp) {

  /* Initial clock setting: 400kHz, 1bit mode.*/
  sdcp->sdmmc->CLKCR  = sdc_lld_clkdiv(sdcp, 400000);
  sdcp->sdmmc->POWER |= SDMMC_POWER_PWRCTRL_0 | SDMMC_POWER_PWRCTRL_1;
/* TODO sdcp->sdmmc->CLKCR |= SDMMC_CLKCR_CLKEN;*/

  /* Clock activation delay.*/
  osalThreadSleep(OSAL_MS2I(STM32_SDC_SDMMC_CLOCK_DELAY));
}

/**
 * @brief   Sets the SDIO clock to data mode (25/50 MHz or less).
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] clk       the clock mode
 *
 * @notapi
 */
void sdc_lld_set_data_clk(SDCDriver *sdcp, sdcbusclk_t clk) {

  if (SDC_CLK_50MHz == clk) {
    sdcp->sdmmc->CLKCR = (sdcp->sdmmc->CLKCR & 0xFFFFFF00U) |
#if STM32_SDC_SDMMC_PWRSAV
                         sdc_lld_clkdiv(sdcp, 50000000) | SDMMC_CLKCR_PWRSAV;
#else
                         sdc_lld_clkdiv(sdcp, 50000000);
#endif
  }
  else {
#if STM32_SDC_SDMMC_PWRSAV
    sdcp->sdmmc->CLKCR = (sdcp->sdmmc->CLKCR & 0xFFFFFF00U) |
                         sdc_lld_clkdiv(sdcp, 25000000) | SDMMC_CLKCR_PWRSAV;
#else
    sdcp->sdmmc->CLKCR = (sdcp->sdmmc->CLKCR & 0xFFFFFF00U) |
                         sdc_lld_clkdiv(sdcp, 25000000);
#endif
  }
}

/**
 * @brief   Stops the SDIO clock.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop_clk(SDCDriver *sdcp) {

  sdcp->sdmmc->CLKCR = 0;
  sdcp->sdmmc->POWER = 0;
}

/**
 * @brief   Switches the bus to 1, 4 or 8 bits mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] mode      bus mode
 *
 * @notapi
 */
void sdc_lld_set_bus_mode(SDCDriver *sdcp, sdcbusmode_t mode) {
  uint32_t clk = sdcp->sdmmc->CLKCR & ~SDMMC_CLKCR_WIDBUS;

  switch (mode) {
  case SDC_MODE_1BIT:
    sdcp->sdmmc->CLKCR = clk;
    break;
  case SDC_MODE_4BIT:
    sdcp->sdmmc->CLKCR = clk | SDMMC_CLKCR_WIDBUS_0;
    break;
  case SDC_MODE_8BIT:
    sdcp->sdmmc->CLKCR = clk | SDMMC_CLKCR_WIDBUS_1;
    break;
  }
}

/**
 * @brief   Sends an SDIO command with no response expected.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 *
 * @notapi
 */
void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg) {

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_CPSMEN;
  while ((sdcp->sdmmc->STA & SDMMC_STA_CMDSENT) == 0)
    ;
  sdcp->sdmmc->ICR = SDMMC_ICR_CMDSENTC;
}

/**
 * @brief   Sends an SDIO command with a short response expected.
 * @note    The CRC is not verified.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (one word)
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                            uint32_t *resp) {
  uint32_t sta;

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_WAITRESP_0 | SDMMC_CMD_CPSMEN;
  while (((sta = sdcp->sdmmc->STA) & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                                     SDMMC_STA_CCRCFAIL)) == 0)
    ;
  sdcp->sdmmc->ICR = sta & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                           SDMMC_STA_CCRCFAIL);
  if ((sta & (SDMMC_STA_CTIMEOUT)) != 0) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp = sdcp->sdmmc->RESP1;
  return HAL_SUCCESS;
}

/**
 * @brief   Sends an SDIO command with a short response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (one word)
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_send_cmd_short_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                uint32_t *resp) {
  uint32_t sta;

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_WAITRESP_0 | SDMMC_CMD_CPSMEN;
  while (((sta = sdcp->sdmmc->STA) & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                                     SDMMC_STA_CCRCFAIL)) == 0)
    ;
  sdcp->sdmmc->ICR = sta & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                            SDMMC_STA_CCRCFAIL);
  if ((sta & (SDMMC_STA_CTIMEOUT | SDMMC_STA_CCRCFAIL)) != 0) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp = sdcp->sdmmc->RESP1;
  return HAL_SUCCESS;
}

/**
 * @brief   Sends an SDIO command with a long response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (four words)
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_send_cmd_long_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                               uint32_t *resp) {
  uint32_t sta;

  (void)sdcp;

  sdcp->sdmmc->ARG = arg;
  sdcp->sdmmc->CMD = (uint32_t)cmd | SDMMC_CMD_WAITRESP_0 | SDMMC_CMD_WAITRESP_1 |
                                    SDMMC_CMD_CPSMEN;
  while (((sta = sdcp->sdmmc->STA) & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                                     SDMMC_STA_CCRCFAIL)) == 0)
    ;
  sdcp->sdmmc->ICR = sta & (SDMMC_STA_CMDREND | SDMMC_STA_CTIMEOUT |
                           SDMMC_STA_CCRCFAIL);
  if ((sta & (SDMMC_STA_ERROR_MASK)) != 0) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  /* Save bytes in reverse order because MSB in response comes first.*/
  *resp++ = sdcp->sdmmc->RESP4;
  *resp++ = sdcp->sdmmc->RESP3;
  *resp++ = sdcp->sdmmc->RESP2;
  *resp   = sdcp->sdmmc->RESP1;
  return HAL_SUCCESS;
}

/**
 * @brief   Reads special registers using data bus.
 * @details Needs only during card detection procedure.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] buf      pointer to the read buffer
 * @param[in] bytes     number of bytes to read
 * @param[in] cmd       card command
 * @param[in] arg       argument for command
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_read_special(SDCDriver *sdcp, uint8_t *buf, size_t bytes,
                          uint8_t cmd, uint32_t arg) {
  uint32_t resp[1];

  if (sdc_lld_prepare_read_bytes(sdcp, buf, bytes))
    goto error;

  if (sdc_lld_send_cmd_short_crc(sdcp, SDMMC_CMD_CMDTRANS | cmd, arg, resp) ||
      MMCSD_R1_ERROR(resp[0]))
    goto error;

  if (sdc_lld_wait_transaction_end(sdcp, 1, resp))
    goto error;

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, 1, resp);
  return HAL_FAILED;
}

/**
 * @brief   Reads one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] blocks    number of blocks to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_read_aligned(SDCDriver *sdcp, uint32_t startblk,
                          uint8_t *buf, uint32_t blocks) {
  uint32_t resp[1];

  osalDbgCheck(blocks < 0x1000000 / MMCSD_BLOCK_SIZE);

  sdcp->sdmmc->DTIMER = STM32_SDC_SDMMC_READ_TIMEOUT;

  /* Checks for errors and waits for the card to be ready for reading.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return HAL_FAILED;

  /* Setting up data transfer.*/
  sdcp->sdmmc->ICR   = SDMMC_ICR_ALL_FLAGS;
  sdcp->sdmmc->MASK  = SDMMC_MASK_DCRCFAILIE |
                       SDMMC_MASK_DTIMEOUTIE |
                       SDMMC_MASK_RXOVERRIE |
                       SDMMC_MASK_DATAENDIE;
  sdcp->sdmmc->DLEN  = blocks * MMCSD_BLOCK_SIZE;

  /* Transfer modes.*/
  sdcp->sdmmc->DCTRL = SDMMC_DCTRL_DTDIR |
                       SDMMC_DCTRL_DBLOCKSIZE_3 |
                       SDMMC_DCTRL_DBLOCKSIZE_0;

  /* Prepares IDMA.*/
  sdcp->sdmmc->IDMABASE0 = (uint32_t)buf;
  sdcp->sdmmc->IDMACTRL  = SDMMC_IDMA_IDMAEN;

  if (sdc_lld_prepare_read(sdcp, startblk, blocks, resp) == true)
    goto error;

  if (sdc_lld_wait_transaction_end(sdcp, blocks, resp) == true)
    goto error;

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, blocks, resp);
  return HAL_FAILED;
}

/**
 * @brief   Writes one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] n         number of blocks to write
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_write_aligned(SDCDriver *sdcp, uint32_t startblk,
                           const uint8_t *buf, uint32_t blocks) {
  uint32_t resp[1];

  osalDbgCheck(blocks < 0x1000000 / MMCSD_BLOCK_SIZE);

  sdcp->sdmmc->DTIMER = STM32_SDC_SDMMC_WRITE_TIMEOUT;

  /* Checks for errors and waits for the card to be ready for writing.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return HAL_FAILED;

  /* Setting up data transfer.*/
  sdcp->sdmmc->ICR   = SDMMC_ICR_ALL_FLAGS;
  sdcp->sdmmc->MASK  = SDMMC_MASK_DCRCFAILIE |
                       SDMMC_MASK_DTIMEOUTIE |
                       SDMMC_MASK_TXUNDERRIE |
                       SDMMC_MASK_DATAENDIE;
  sdcp->sdmmc->DLEN  = blocks * MMCSD_BLOCK_SIZE;

  /* Transfer modes.*/
  sdcp->sdmmc->DCTRL = SDMMC_DCTRL_DBLOCKSIZE_3 |
                       SDMMC_DCTRL_DBLOCKSIZE_0;

  /* Prepares IDMA.*/
  sdcp->sdmmc->IDMABASE0 = (uint32_t)buf;
  sdcp->sdmmc->IDMACTRL  = SDMMC_IDMA_IDMAEN;

  if (sdc_lld_prepare_write(sdcp, startblk, blocks, resp) == true)
    goto error;

  if (sdc_lld_wait_transaction_end(sdcp, blocks, resp) == true)
    goto error;

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, blocks, resp);
  return HAL_FAILED;
}

/**
 * @brief   Reads one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] blocks    number of blocks to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_read(SDCDriver *sdcp, uint32_t startblk,
                  uint8_t *buf, uint32_t blocks) {

#if STM32_SDC_SDMMC_UNALIGNED_SUPPORT
  if (((unsigned)buf & 3) != 0) {
    uint32_t i;
    for (i = 0; i < blocks; i++) {
      if (sdc_lld_read_aligned(sdcp, startblk, sdcp->buf, 1))
        return HAL_FAILED;
      memcpy(buf, sdcp->buf, MMCSD_BLOCK_SIZE);
      buf += MMCSD_BLOCK_SIZE;
      startblk++;
    }
    return HAL_SUCCESS;
  }
#else /* !STM32_SDC_SDIO_UNALIGNED_SUPPORT */
  osalDbgAssert((((unsigned)buf & 3) == 0), "unaligned buffer");
#endif /* !STM32_SDC_SDIO_UNALIGNED_SUPPORT */
  return sdc_lld_read_aligned(sdcp, startblk, buf, blocks);
}

/**
 * @brief   Writes one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] blocks    number of blocks to write
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_write(SDCDriver *sdcp, uint32_t startblk,
                   const uint8_t *buf, uint32_t blocks) {

#if STM32_SDC_SDMMC_UNALIGNED_SUPPORT
  if (((unsigned)buf & 3) != 0) {
    uint32_t i;
    for (i = 0; i < blocks; i++) {
      memcpy(sdcp->buf, buf, MMCSD_BLOCK_SIZE);
      buf += MMCSD_BLOCK_SIZE;
      if (sdc_lld_write_aligned(sdcp, startblk, sdcp->buf, 1))
        return HAL_FAILED;
      startblk++;
    }
    return HAL_SUCCESS;
  }
#else /* !STM32_SDC_SDIO_UNALIGNED_SUPPORT */
  osalDbgAssert((((unsigned)buf & 3) == 0), "unaligned buffer");
#endif /* !STM32_SDC_SDIO_UNALIGNED_SUPPORT */
  return sdc_lld_write_aligned(sdcp, startblk, buf, blocks);
}

/**
 * @brief   Waits for card idle condition.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  the operation succeeded.
 * @retval HAL_FAILED   the operation failed.
 *
 * @api
 */
bool sdc_lld_sync(SDCDriver *sdcp) {

  /* CHTODO: Implement.*/
  (void)sdcp;
  return HAL_SUCCESS;
}

/**
 * @brief   Shared service routine.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 */
void sdc_lld_serve_interrupt(SDCDriver *sdcp) {

  /* Disables the source but the status flags are not reset because the
     read/write functions needs to check them.*/
  sdcp->sdmmc->MASK = 0;

  osalSysLockFromISR();
  osalThreadResumeI(&sdcp->thread, MSG_OK);
  osalSysUnlockFromISR();
}

#endif /* HAL_USE_SDC */

/** @} */
