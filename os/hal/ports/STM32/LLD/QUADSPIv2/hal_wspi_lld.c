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
 * @file    QUADSPIv2/hal_wspi_lld.c
 * @brief   STM32 WSPI subsystem low level driver source.
 *
 * @addtogroup WSPI
 * @{
 */

#include "hal.h"

#if (HAL_USE_WSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* @brief MDMA HW request is QSPI FIFO threshold Flag */
#define MDMA_REQUEST_QUADSPI_FIFO_TH      ((uint32_t)0x00000016U)

/* @brief MDMA HW request is QSPI Transfer complete Flag */
#define MDMA_REQUEST_QUADSPI_TC           ((uint32_t)0x00000017U)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief QUADSPI1 driver identifier.*/
#if STM32_WSPI_USE_QUADSPI1 || defined(__DOXYGEN__)
WSPIDriver WSPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for completion of previous operation.
 */
static inline void wspi_lld_sync(WSPIDriver *wspip) {

  while ((wspip->qspi->SR & QUADSPI_SR_BUSY) != 0U) {
  }
}

/**
 * @brief   Shared service routine.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 * @param[in] flags     content of the CISR register
 */
static void wspi_lld_serve_mdma_interrupt(WSPIDriver *wspip, uint32_t flags) {

  (void)wspip;
  (void)flags;

  if (((flags & STM32_MDMA_CISR_CTCIF) != 0U) &&
      (wspip->state == WSPI_RECEIVE)) {
    /* Portable WSPI ISR code defined in the high level driver, note, it is
     a macro.*/
    _wspi_isr_code(wspip);

    mdmaChannelDisableX(wspip->mdma);
  }
  /* DMA errors handling.*/
#if defined(STM32_WSPI_MDMA_ERROR_HOOK)
  else if ((flags & STM32_MDMA_CISR_TEIF) != 0) {
    STM32_WSPI_MDMA_ERROR_HOOK(wspip);
  }
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level WSPI driver initialization.
 *
 * @notapi
 */
void wspi_lld_init(void) {

#if STM32_WSPI_USE_QUADSPI1
  wspiObjectInit(&WSPID1);
  WSPID1.qspi       = QUADSPI;
  WSPID1.mdma       = NULL;
#endif
}

/**
 * @brief   Configures and activates the WSPI peripheral.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 *
 * @notapi
 */
void wspi_lld_start(WSPIDriver *wspip) {

  /* If in stopped state then full initialization.*/
  if (wspip->state == WSPI_STOP) {
#if STM32_WSPI_USE_QUADSPI1
    if (&WSPID1 == wspip) {
      wspip->mdma = mdmaChannelAllocI(STM32_WSPI_QUADSPI1_MDMA_CHANNEL,
                                      (stm32_mdmaisr_t)wspi_lld_serve_mdma_interrupt,
                                      (void *)wspip);
      osalDbgAssert(wspip->mdma != NULL, "unable to allocate MDMA channel");
      rccEnableQUADSPI1(true);
    }
#endif
  }

  /* WSPI setup and enable.*/
  wspip->qspi->DCR = wspip->config->dcr;
#if STM32_WSPI_SET_CR_SSHIFT
  wspip->qspi->CR  = ((STM32_WSPI_QUADSPI1_PRESCALER_VALUE - 1U) << 24U) |
                      QUADSPI_CR_TCIE | QUADSPI_CR_DMAEN | QUADSPI_CR_SSHIFT |
                      QUADSPI_CR_EN;
#else
  wspip->qspi->CR  = ((STM32_WSPI_QUADSPI1_PRESCALER_VALUE - 1U) << 24U) |
                      QUADSPI_CR_TCIE | QUADSPI_CR_DMAEN | QUADSPI_CR_EN;
#endif
  wspip->qspi->FCR = QUADSPI_FCR_CTEF | QUADSPI_FCR_CTCF |
                     QUADSPI_FCR_CSMF | QUADSPI_FCR_CTOF;
}

/**
 * @brief   Deactivates the WSPI peripheral.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 *
 * @notapi
 */
void wspi_lld_stop(WSPIDriver *wspip) {

  /* If in ready state then disables the QUADSPI clock.*/
  if (wspip->state == WSPI_READY) {

    /* WSPI disable.*/
    wspip->qspi->CR = 0U;

    /* Releasing the DMA.*/
    mdmaChannelFreeI(wspip->mdma);
    wspip->mdma = NULL;

    /* Stopping involved clocks.*/
#if STM32_WSPI_USE_QUADSPI1
    if (&WSPID1 == wspip) {
      rccDisableQUADSPI1();
    }
#endif
  }
}

/**
 * @brief   Sends a command without data phase.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 *
 * @notapi
 */
void wspi_lld_command(WSPIDriver *wspip, const wspi_command_t *cmdp) {

#if STM32_USE_STM32_D1_WORKAROUND == TRUE
  /* If it is a command without address and alternate phases then the command
     is sent as an alternate byte, the command phase is suppressed.*/
  if ((cmdp->cfg & (WSPI_CFG_ADDR_MODE_MASK | WSPI_CFG_ALT_MODE_MASK)) == 0U) {
    /* The command mode field is copied in the alternate mode field. All
       other fields are not used in this scenario.*/
    wspip->qspi->DLR = 0U;
    wspip->qspi->ABR = cmdp->cmd;
    wspip->qspi->CCR = (cmdp->cfg  & WSPI_CFG_CMD_MODE_MASK) << 6U;
    return;
  }
#endif
  wspip->qspi->DLR = 0U;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->qspi->AR  = cmdp->addr;
  }
}

/**
 * @brief   Sends a command with data over the WSPI bus.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void wspi_lld_send(WSPIDriver *wspip, const wspi_command_t *cmdp,
                   size_t n, const uint8_t *txbuf) {
  uint32_t ctcr = STM32_MDMA_CTCR_BWM_NON_BUFF  |   /* Dest. non-cacheable. */
                  STM32_MDMA_CTCR_TRGM_BUFFER   |   /* Trigger on buffer.   */
                  STM32_MDMA_CTCR_TLEN(0U)      |   /* One byte buffer.     */
                  STM32_MDMA_CTCR_DBURST_1      |   /* Assuming AXI bus.    */
                  STM32_MDMA_CTCR_SBURST_1      |   /* Assuming AXI bus.    */
                  STM32_MDMA_CTCR_DINCOS_BYTE   |   /* Byte increment.      */
                  STM32_MDMA_CTCR_SINCOS_BYTE   |   /* Byte increment.      */
                  STM32_MDMA_CTCR_DSIZE_BYTE    |   /* Destination size.    */
                  STM32_MDMA_CTCR_SSIZE_BYTE    |   /* Source size.         */
                  STM32_MDMA_CTCR_DINC_FIXED    |   /* Destination fixed.   */
                  STM32_MDMA_CTCR_SINC_INC;         /* Source incremented.  */
  uint32_t ccr  = STM32_MDMA_CCR_PL(STM32_WSPI_QUADSPI1_MDMA_PRIORITY) |
                  STM32_MDMA_CCR_TEIE;              /* On transfer error.   */

  /* MDMA initializations.*/
  mdmaChannelSetSourceX(wspip->mdma, txbuf);
  mdmaChannelSetDestinationX(wspip->mdma, &wspip->qspi->DR);
  mdmaChannelSetTransactionSizeX(wspip->mdma, n, 0, 0);
  mdmaChannelSetModeX(wspip->mdma, ctcr, ccr);
  mdmaChannelSetTrigModeX(wspip->mdma, MDMA_REQUEST_QUADSPI_FIFO_TH);

  wspip->qspi->DLR = n - 1;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->qspi->AR  = cmdp->addr;
  }

  mdmaChannelEnableX(wspip->mdma);
}

/**
 * @brief   Sends a command then receives data over the WSPI bus.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void wspi_lld_receive(WSPIDriver *wspip, const wspi_command_t *cmdp,
                      size_t n, uint8_t *rxbuf) {
  uint32_t ctcr = STM32_MDMA_CTCR_BWM_NON_BUFF  |   /* Dest. non-cacheable. */
                  STM32_MDMA_CTCR_TRGM_BUFFER   |   /* Trigger on buffer.   */
                  STM32_MDMA_CTCR_TLEN(0)       |   /* One byte buffer.     */
                  STM32_MDMA_CTCR_DBURST_1      |   /* Assuming AXI bus.    */
                  STM32_MDMA_CTCR_SBURST_1      |   /* Assuming AXI bus.    */
                  STM32_MDMA_CTCR_DINCOS_BYTE   |   /* Byte increment.      */
                  STM32_MDMA_CTCR_SINCOS_BYTE   |   /* Byte increment.      */
                  STM32_MDMA_CTCR_DSIZE_BYTE    |   /* Destination size.    */
                  STM32_MDMA_CTCR_SSIZE_BYTE    |   /* Source size.         */
                  STM32_MDMA_CTCR_DINC_INC      |   /* Destination incr.    */
                  STM32_MDMA_CTCR_SINC_FIXED;       /* Source fixed.        */
  uint32_t ccr  = STM32_MDMA_CCR_PL(STM32_WSPI_QUADSPI1_MDMA_PRIORITY) |
                  STM32_MDMA_CCR_CTCIE          |   /* On transfer complete.*/
                  STM32_MDMA_CCR_TEIE;              /* On transfer error.   */

  /* MDMA initializations.*/
  mdmaChannelSetSourceX(wspip->mdma, &wspip->qspi->DR);
  mdmaChannelSetDestinationX(wspip->mdma, rxbuf);
  mdmaChannelSetTransactionSizeX(wspip->mdma, n, 0, 0);
  mdmaChannelSetModeX(wspip->mdma, ctcr, ccr);
  mdmaChannelSetTrigModeX(wspip->mdma, MDMA_REQUEST_QUADSPI_FIFO_TH);

  wspip->qspi->DLR = n - 1;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = cmdp->cmd | cmdp->cfg |
                     QUADSPI_CCR_DUMMY_CYCLES(cmdp->dummy) |
                     QUADSPI_CCR_FMODE_0;
  if ((cmdp->cfg & WSPI_CFG_ADDR_MODE_MASK) != WSPI_CFG_ADDR_MODE_NONE) {
    wspip->qspi->AR  = cmdp->addr;
  }

  mdmaChannelEnableX(wspip->mdma);
}

#if (WSPI_SUPPORTS_MEMMAP == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Maps in memory space a WSPI flash device.
 * @pre     The memory flash device must be initialized appropriately
 *          before mapping it in memory space.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 * @param[in] cmdp      pointer to the command descriptor
 * @param[out] addrp    pointer to the memory start address of the mapped
 *                      flash or @p NULL
 *
 * @notapi
 */
void wspi_lld_map_flash(WSPIDriver *wspip,
                        const wspi_command_t *cmdp,
                        uint8_t **addrp) {

  uint32_t ccr = cmdp->cmd | cmdp->cfg |
                 QUADSPI_CCR_DUMMY_CYCLES(cmdp->dummy) |
                 QUADSPI_CCR_FMODE_1 | QUADSPI_CCR_FMODE_0;

  /* Disabling the DMA request while in memory mapped mode.*/
  wspip->qspi->CR &= ~QUADSPI_CR_DMAEN;

  /* Starting memory mapped mode using the passed parameters.*/
  wspip->qspi->DLR = 0;
  wspip->qspi->ABR = cmdp->alt;
  wspip->qspi->CCR = ccr;

  /* Mapped flash absolute base address.*/
  if (addrp != NULL) {
    *addrp = (uint8_t *)0x90000000;
  }
}

/**
 * @brief   Unmaps from memory space a WSPI flash device.
 * @post    The memory flash device must be re-initialized for normal
 *          commands exchange.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 *
 * @notapi
 */
void wspi_lld_unmap_flash(WSPIDriver *wspip) {

  /* Aborting memory mapped mode.*/
  wspip->qspi->CR |= QUADSPI_CR_ABORT;
  while ((wspip->qspi->CR & QUADSPI_CR_ABORT) != 0U) {
  }

  /* Re-enabling DMA request, we are going back to indirect mode.*/
  wspip->qspi->CR |= QUADSPI_CR_DMAEN;
}
#endif /* WSPI_SUPPORTS_MEMMAP == TRUE */

/**
 * @brief   Shared service routine.
 *
 * @param[in] wspip     pointer to the @p WSPIDriver object
 */
void wspi_lld_serve_interrupt(WSPIDriver *wspip) {
  uint32_t sr;

  sr = wspip->qspi->SR;
  wspip->qspi->FCR = sr;

  if (((sr & QUADSPI_FCR_CTCF) != 0U) && (wspip->state == WSPI_SEND)) {
    /* Portable WSPI ISR code defined in the high level driver, note, it is
     a macro.*/
    _wspi_isr_code(wspip);

    mdmaChannelDisableX(wspip->mdma);
  }

  /* TODO errors handling.*/
}

#endif /* HAL_USE_WSPI */

/** @} */
