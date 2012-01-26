/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

#include <string.h>

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

#if STM32_SDC_UNALIGNED_SUPPORT
/**
 * @brief   Buffer for temporary storage during unaligned transfers.
 */
static union {
  uint32_t  alignment;
  uint8_t   buf[SDC_BLOCK_SIZE];
} u;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Reads one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer, it must be aligned to
 *                      four bytes boundary
 * @param[in] n         number of blocks to read
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      read.
 * @retval TRUE         operation failed, the state of the buffer is uncertain.
 *
 * @notapi
 */
static bool_t sdc_lld_read_multiple(SDCDriver *sdcp, uint32_t startblk,
                                    uint8_t *buf, uint32_t n) {
  uint32_t resp[1];

  /* Checks for errors and waits for the card to be ready for reading.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return TRUE;

  /* Prepares the DMA channel for reading.*/
  dmaStreamSetMemory0(STM32_DMA2_STREAM4, buf);
  dmaStreamSetTransactionSize(STM32_DMA2_STREAM4,
                              (n * SDC_BLOCK_SIZE) / sizeof (uint32_t));
  dmaStreamSetMode(STM32_DMA2_STREAM4,
                   STM32_DMA_CR_PL(STM32_SDC_SDIO_DMA_PRIORITY) |
                   STM32_DMA_CR_DIR_P2M | STM32_DMA_CR_PSIZE_WORD |
                   STM32_DMA_CR_MSIZE_WORD | STM32_DMA_CR_MINC);

  /* Setting up data transfer.
     Options: Card to Controller, Block mode, DMA mode, 512 bytes blocks.*/
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = SDIO_MASK_DCRCFAILIE | SDIO_MASK_DTIMEOUTIE |
                SDIO_MASK_DATAENDIE | SDIO_MASK_STBITERRIE;
  SDIO->DLEN  = n * SDC_BLOCK_SIZE;
  SDIO->DCTRL = SDIO_DCTRL_DTDIR |
                SDIO_DCTRL_DBLOCKSIZE_3 | SDIO_DCTRL_DBLOCKSIZE_0 |
                SDIO_DCTRL_DMAEN |
                SDIO_DCTRL_DTEN;

  /* DMA channel activation.*/
  dmaStreamEnable(STM32_DMA2_STREAM4);

  /* Read multiple blocks command.*/
  if ((sdcp->cardmode & SDC_MODE_HIGH_CAPACITY) == 0)
    startblk *= SDC_BLOCK_SIZE;
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_READ_MULTIPLE_BLOCK,
                                 startblk, resp) ||
      SDC_R1_ERROR(resp[0]))
    goto error;

  chSysLock();
  if (SDIO->MASK != 0) {
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_read_multiple(), #1", "not NULL");
    sdcp->thread = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_read_multiple(), #2", "not NULL");
  }
  if ((SDIO->STA & SDIO_STA_DATAEND) == 0) {
    chSysUnlock();
    goto error;
  }
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->DCTRL = 0;
  chSysUnlock();

  return sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_STOP_TRANSMISSION, 0, resp);
error:
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = 0;
  SDIO->DCTRL = 0;
  return TRUE;
}

/**
 * @brief   Reads one block.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer, it must be aligned to
 *                      four bytes boundary
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      read.
 * @retval TRUE         operation failed, the state of the buffer is uncertain.
 *
 * @notapi
 */
static bool_t sdc_lld_read_single(SDCDriver *sdcp, uint32_t startblk,
                                  uint8_t *buf) {
  uint32_t resp[1];

  /* Checks for errors and waits for the card to be ready for reading.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return TRUE;

  /* Prepares the DMA channel for reading.*/
  dmaStreamSetMemory0(STM32_DMA2_STREAM4, buf);
  dmaStreamSetTransactionSize(STM32_DMA2_STREAM4,
                              SDC_BLOCK_SIZE / sizeof (uint32_t));
  dmaStreamSetMode(STM32_DMA2_STREAM4,
                   STM32_DMA_CR_PL(STM32_SDC_SDIO_DMA_PRIORITY) |
                   STM32_DMA_CR_DIR_P2M | STM32_DMA_CR_PSIZE_WORD |
                   STM32_DMA_CR_MSIZE_WORD | STM32_DMA_CR_MINC);

  /* Setting up data transfer.
     Options: Card to Controller, Block mode, DMA mode, 512 bytes blocks.*/
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = SDIO_MASK_DCRCFAILIE | SDIO_MASK_DTIMEOUTIE |
                SDIO_MASK_DATAENDIE | SDIO_MASK_STBITERRIE;
  SDIO->DLEN  = SDC_BLOCK_SIZE;
  SDIO->DCTRL = SDIO_DCTRL_DTDIR |
                SDIO_DCTRL_DBLOCKSIZE_3 | SDIO_DCTRL_DBLOCKSIZE_0 |
                SDIO_DCTRL_DMAEN |
                SDIO_DCTRL_DTEN;

  /* DMA channel activation.*/
  dmaStreamEnable(STM32_DMA2_STREAM4);

  /* Read single block command.*/
  if ((sdcp->cardmode & SDC_MODE_HIGH_CAPACITY) == 0)
    startblk *= SDC_BLOCK_SIZE;
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_READ_SINGLE_BLOCK,
                                 startblk, resp) ||
      SDC_R1_ERROR(resp[0]))
    goto error;

  chSysLock();
  if (SDIO->MASK != 0) {
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_read_single(), #1", "not NULL");
    sdcp->thread = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_read_single(), #2", "not NULL");
  }
  if ((SDIO->STA & SDIO_STA_DATAEND) == 0) {
    chSysUnlock();
    goto error;
  }
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->DCTRL = 0;
  chSysUnlock();

  return FALSE;
error:
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = 0;
  SDIO->DCTRL = 0;
  return TRUE;
}

/**
 * @brief   Writes one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer, it must be aligned to
 *                      four bytes boundary
 * @param[in] n         number of blocks to write
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      written.
 * @retval TRUE         operation failed.
 *
 * @notapi
 */
static bool_t sdc_lld_write_multiple(SDCDriver *sdcp, uint32_t startblk,
                              const uint8_t *buf, uint32_t n) {
  uint32_t resp[1];

  /* Checks for errors and waits for the card to be ready for writing.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return TRUE;

  /* Prepares the DMA channel for writing.*/
  dmaStreamSetMemory0(STM32_DMA2_STREAM4, buf);
  dmaStreamSetTransactionSize(STM32_DMA2_STREAM4,
                              (n * SDC_BLOCK_SIZE) / sizeof (uint32_t));
  dmaStreamSetMode(STM32_DMA2_STREAM4,
                   STM32_DMA_CR_PL(STM32_SDC_SDIO_DMA_PRIORITY) |
                   STM32_DMA_CR_DIR_M2P | STM32_DMA_CR_PSIZE_WORD |
                   STM32_DMA_CR_MSIZE_WORD | STM32_DMA_CR_MINC);

  /* Write multiple blocks command.*/
  if ((sdcp->cardmode & SDC_MODE_HIGH_CAPACITY) == 0)
    startblk *= SDC_BLOCK_SIZE;
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_WRITE_MULTIPLE_BLOCK,
                                 startblk, resp) ||
      SDC_R1_ERROR(resp[0]))
    return TRUE;

  /* Setting up data transfer.
     Options: Controller to Card, Block mode, DMA mode, 512 bytes blocks.*/
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = SDIO_MASK_DCRCFAILIE | SDIO_MASK_DTIMEOUTIE |
                SDIO_MASK_DATAENDIE | SDIO_MASK_TXUNDERRIE |
                SDIO_MASK_STBITERRIE;
  SDIO->DLEN  = n * SDC_BLOCK_SIZE;
  SDIO->DCTRL = SDIO_DCTRL_DBLOCKSIZE_3 | SDIO_DCTRL_DBLOCKSIZE_0 |
                SDIO_DCTRL_DMAEN |
                SDIO_DCTRL_DTEN;

  /* DMA channel activation.*/
  dmaStreamEnable(STM32_DMA2_STREAM4);

  /* Note the mask is checked before going to sleep because the interrupt
     may have occurred before reaching the critical zone.*/
  chSysLock();
  if (SDIO->MASK != 0) {
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_write_multiple(), #1", "not NULL");
    sdcp->thread = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_write_multiple(), #2", "not NULL");
  }
  if ((SDIO->STA & SDIO_STA_DATAEND) == 0) {
    chSysUnlock();
    goto error;
  }
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->DCTRL = 0;
  chSysUnlock();

  return sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_STOP_TRANSMISSION, 0, resp);
error:
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = 0;
  SDIO->DCTRL = 0;
  return TRUE;
}

/**
 * @brief   Writes one block.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer, it must be aligned to
 *                      four bytes boundary
 * @param[in] n         number of blocks to write
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      written.
 * @retval TRUE         operation failed.
 *
 * @notapi
 */
static bool_t sdc_lld_write_single(SDCDriver *sdcp, uint32_t startblk,
                            const uint8_t *buf) {
  uint32_t resp[1];

  /* Checks for errors and waits for the card to be ready for writing.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return TRUE;

  /* Prepares the DMA channel for writing.*/
  dmaStreamSetMemory0(STM32_DMA2_STREAM4, buf);
  dmaStreamSetTransactionSize(STM32_DMA2_STREAM4,
                              SDC_BLOCK_SIZE / sizeof (uint32_t));
  dmaStreamSetMode(STM32_DMA2_STREAM4,
                   STM32_DMA_CR_PL(STM32_SDC_SDIO_DMA_PRIORITY) |
                   STM32_DMA_CR_DIR_M2P | STM32_DMA_CR_PSIZE_WORD |
                   STM32_DMA_CR_MSIZE_WORD | STM32_DMA_CR_MINC);

  /* Write single block command.*/
  if ((sdcp->cardmode & SDC_MODE_HIGH_CAPACITY) == 0)
    startblk *= SDC_BLOCK_SIZE;
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_WRITE_BLOCK,
                                 startblk, resp) ||
      SDC_R1_ERROR(resp[0]))
    return TRUE;

  /* Setting up data transfer.
     Options: Controller to Card, Block mode, DMA mode, 512 bytes blocks.*/
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = SDIO_MASK_DCRCFAILIE | SDIO_MASK_DTIMEOUTIE |
                SDIO_MASK_DATAENDIE | SDIO_MASK_TXUNDERRIE |
                SDIO_MASK_STBITERRIE;
  SDIO->DLEN  = SDC_BLOCK_SIZE;
  SDIO->DCTRL = SDIO_DCTRL_DBLOCKSIZE_3 | SDIO_DCTRL_DBLOCKSIZE_0 |
                SDIO_DCTRL_DMAEN |
                SDIO_DCTRL_DTEN;

  /* DMA channel activation.*/
  dmaStreamEnable(STM32_DMA2_STREAM4);

  /* Note the mask is checked before going to sleep because the interrupt
     may have occurred before reaching the critical zone.*/
  chSysLock();
  if (SDIO->MASK != 0) {
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_write_single(), #1", "not NULL");
    sdcp->thread = chThdSelf();
    chSchGoSleepS(THD_STATE_SUSPENDED);
    chDbgAssert(sdcp->thread == NULL,
                "sdc_lld_write_single(), #2", "not NULL");
  }
  if ((SDIO->STA & SDIO_STA_DATAEND) == 0) {
    chSysUnlock();
    goto error;
  }
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->DCTRL = 0;
  chSysUnlock();

  return FALSE;
error:
  dmaStreamDisable(STM32_DMA2_STREAM4);
  SDIO->ICR   = 0xFFFFFFFF;
  SDIO->MASK  = 0;
  SDIO->DCTRL = 0;
  return TRUE;
}

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

  chSysLockFromIsr();
  if (SDCD1.thread != NULL) {
    chSchReadyI(SDCD1.thread);
    SDCD1.thread = NULL;
  }
  chSysUnlockFromIsr();

  /* Disables the source but the status flags are not reset because the
     read/write functions need to check them.*/
  SDIO->MASK = 0;

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
  SDCD1.thread = NULL;
}

/**
 * @brief   Configures and activates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object, must be @p NULL,
 *                      this driver does not require any configuration
 *
 * @notapi
 */
void sdc_lld_start(SDCDriver *sdcp) {

  if (sdcp->state == SDC_STOP) {
    /* Note, the DMA must be enabled before the IRQs.*/
    dmaStreamAllocate(STM32_DMA2_STREAM4, 0, NULL, NULL);
    dmaStreamSetPeripheral(STM32_DMA2_STREAM4, &SDIO->FIFO);
    nvicEnableVector(SDIO_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_SDC_SDIO_IRQ_PRIORITY));
    rccEnableSDIO(FALSE);
  }
  /* Configuration, card clock is initially stopped.*/
  SDIO->POWER  = 0;
  SDIO->CLKCR  = 0;
  SDIO->DCTRL  = 0;
  SDIO->DTIMER = STM32_SDC_DATATIMEOUT;
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
    SDIO->POWER  = 0;
    SDIO->CLKCR  = 0;
    SDIO->DCTRL  = 0;
    SDIO->DTIMER = 0;

    /* Clock deactivation.*/
    nvicDisableVector(SDIO_IRQn);
    dmaStreamRelease(STM32_DMA2_STREAM4);
    rccDisableSDIO(FALSE);
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
 * @param[in] mode      bus mode
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
 * @param[in] cmd       card command
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
  SDIO->ICR = SDIO_ICR_CMDSENTC;
}

/**
 * @brief   Sends an SDIO command with a short response expected.
 * @note    The CRC is not verified.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
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
  SDIO->ICR = SDIO_ICR_CMDRENDC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CCRCFAILC;
  if ((sta & (SDIO_STA_CTIMEOUT)) != 0)
    return TRUE;
  *resp = SDIO->RESP1;
  return FALSE;
}

/**
 * @brief   Sends an SDIO command with a short response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
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
  SDIO->ICR = SDIO_ICR_CMDRENDC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CCRCFAILC;
  if ((sta & (SDIO_STA_CTIMEOUT | SDIO_STA_CCRCFAIL)) != 0)
    return TRUE;
  *resp = SDIO->RESP1;
  return FALSE;
}

/**
 * @brief   Sends an SDIO command with a long response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
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
  SDIO->ICR = SDIO_ICR_CMDRENDC | SDIO_ICR_CTIMEOUTC | SDIO_ICR_CCRCFAILC;
  if ((sta & (SDIO_STA_CTIMEOUT | SDIO_STA_CCRCFAIL)) != 0)
    return TRUE;
  *resp = SDIO->RESP1;
  return FALSE;
}

/**
 * @brief   Reads one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] n         number of blocks to read
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      read.
 * @retval TRUE         operation failed, the state of the buffer is uncertain.
 *
 * @notapi
 */
bool_t sdc_lld_read(SDCDriver *sdcp, uint32_t startblk,
                    uint8_t *buf, uint32_t n) {

#if STM32_SDC_UNALIGNED_SUPPORT
  if (((unsigned)buf & 3) != 0) {
    uint32_t i;
    for (i = 0; i < n; i++) {
      if (sdc_lld_read_single(sdcp, startblk, u.buf))
        return TRUE;
      memcpy(buf, u.buf, SDC_BLOCK_SIZE);
      buf += SDC_BLOCK_SIZE;
      startblk++;
    }
    return FALSE;
  }
#endif
  if (n == 1)
    return sdc_lld_read_single(sdcp, startblk, buf);
  return sdc_lld_read_multiple(sdcp, startblk, buf, n);
}

/**
 * @brief   Writes one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] n         number of blocks to write
 * @return              The operation status.
 * @retval FALSE        operation succeeded, the requested blocks have been
 *                      written.
 * @retval TRUE         operation failed.
 *
 * @notapi
 */
bool_t sdc_lld_write(SDCDriver *sdcp, uint32_t startblk,
                     const uint8_t *buf, uint32_t n) {

#if STM32_SDC_UNALIGNED_SUPPORT
  if (((unsigned)buf & 3) != 0) {
    uint32_t i;
    for (i = 0; i < n; i++) {
      memcpy(u.buf, buf, SDC_BLOCK_SIZE);
      buf += SDC_BLOCK_SIZE;
      if (sdc_lld_write_single(sdcp, startblk, u.buf))
        return TRUE;
      startblk++;
    }
    return FALSE;
  }
#endif
  if (n == 1)
    return sdc_lld_write_single(sdcp, startblk, buf);
  return sdc_lld_write_multiple(sdcp, startblk, buf, n);
}

#endif /* HAL_USE_SDC */

/** @} */
