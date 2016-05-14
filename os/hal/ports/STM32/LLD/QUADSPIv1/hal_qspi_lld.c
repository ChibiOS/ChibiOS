/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    QUADSPIv1/hal_qspi_lld.c
 * @brief   STM32 QSPI subsystem low level driver source.
 *
 * @addtogroup QSPI
 * @{
 */

#include "hal.h"

#if HAL_USE_QSPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define QUADSPI1_DMA_CHANNEL                                                \
  STM32_DMA_GETCHANNEL(STM32_QSPI_QUADSPI1_DMA_STREAM,                      \
                       STM32_QUADSPI1_DMA_CHN)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief QUADSPI1 driver identifier.*/
#if STM32_QSPI_USE_QUADSPI1 || defined(__DOXYGEN__)
QSPIDriver QSPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared service routine.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void qspi_lld_serve_dma_interrupt(QSPIDriver *qspip, uint32_t flags) {

  (void)qspip;
  (void)flags;

  /* DMA errors handling.*/
#if defined(STM32_QSPI_DMA_ERROR_HOOK)
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    STM32_QSPI_DMA_ERROR_HOOK(qspip);
  }
#endif
}

/**
 * @brief   Shared service routine.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 */
static void qspi_lld_serve_interrupt(QSPIDriver *qspip) {

  /* Stop everything.*/
  dmaStreamDisable(qspip->dma);

  /* Portable QSPI ISR code defined in the high level driver, note, it is
     a macro.*/
  _qspi_isr_code(qspip);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_QSPI_USE_QUADSPI1 || defined(__DOXYGEN__)
#if !defined(STM32_QUADSPI1_SUPPRESS_ISR)
#if !defined(STM32_QUADSPI1_HANDLER)
#error "STM32_QUADSPI1_HANDLER not defined"
#endif
/**
 * @brief   STM32_QUADSPI1_HANDLER interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_QUADSPI1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  qspi_lld_serve_interrupt(&QSPID1);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(STM32_QUADSPI1_SUPPRESS_ISR) */
#endif /* STM32_QSPI_USE_QUADSPI1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level QSPI driver initialization.
 *
 * @notapi
 */
void qspi_lld_init(void) {

#if STM32_QSPI_USE_QUADSPI1
  qspiObjectInit(&QSPID1);
  QSPID1.qspi       = QUADSPI;
  QSPID1.dma        = STM32_DMA_STREAM(STM32_QSPI_QUADSPI1_DMA_STREAM);
  QSPID1.dmamode    = STM32_DMA_CR_CHSEL(QUADSPI1_DMA_CHANNEL) |
                      STM32_DMA_CR_PL(STM32_QSPI_QUADSPI1_DMA_PRIORITY) |
                      STM32_DMA_CR_PSIZE_BYTE |
                      STM32_DMA_CR_MSIZE_BYTE |
                      STM32_DMA_CR_MINC |
                      STM32_DMA_CR_DMEIE |
                      STM32_DMA_CR_TEIE;
  nvicEnableVector(STM32_QUADSPI1_NUMBER, STM32_QSPI_QUADSPI1_IRQ_PRIORITY);
#endif
}

/**
 * @brief   Configures and activates the QSPI peripheral.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
void qspi_lld_start(QSPIDriver *qspip) {

  /* If in stopped state then full initialization.*/
  if (qspip->state == QSPI_STOP) {
#if STM32_QSPI_USE_QUADSPI1
    if (&QSPID1 == qspip) {
      bool b = dmaStreamAllocate(qspip->dma,
                                 STM32_QSPI_QUADSPI1_DMA_IRQ_PRIORITY,
                                 (stm32_dmaisr_t)qspi_lld_serve_dma_interrupt,
                                 (void *)qspip);
      osalDbgAssert(!b, "stream already allocated");
      rccEnableQUADSPI1(false);
    }
#endif

    /* Common initializations.*/
    dmaStreamSetPeripheral(qspip->dma, &qspip->qspi->DR);
  }

  /* QSPI setup and enable.*/
  qspip->qspi->DCR = qspip->config->dcr;
  qspip->qspi->CR  = ((STM32_QSPI_QUADSPI1_PRESCALER_VALUE - 1U) << 24U) |
                      QUADSPI_CR_TCIE | QUADSPI_CR_TEIE | QUADSPI_CR_DMAEN |
                      QUADSPI_CR_EN;
  qspip->qspi->FCR = QUADSPI_FCR_CTEF | QUADSPI_FCR_CTCF |
                     QUADSPI_FCR_CSMF | QUADSPI_FCR_CTOF;
}

/**
 * @brief   Deactivates the QSPI peripheral.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
void qspi_lld_stop(QSPIDriver *qspip) {

  /* If in ready state then disables the QUADSPI clock.*/
  if (qspip->state == QSPI_READY) {

    /* QSPI disable.*/
    qspip->qspi->CR = 0U;

    /* Releasing the DMA.*/
    dmaStreamRelease(qspip->dma);

    /* Stopping involved clocks.*/
#if STM32_QSPI_USE_QUADSPI1
    if (&QSPID1 == qspip) {
      rccDisableQUADSPI1(FALSE);
    }
#endif
  }
}

/**
 * @brief   Sends a command without data phase.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 *
 * @notapi
 */
void qspi_lld_command(QSPIDriver *qspip, const qspi_command_t *cmdp) {

  qspip->qspi->ABR = cmdp->alt;
  qspip->qspi->CCR = cmdp->cfg;
  if ((cmdp->cfg & QSPI_CFG_ADDR_MODE_MASK) != QSPI_CFG_ADDR_MODE_NONE) {
    qspip->qspi->AR  = cmdp->addr;
  }
}

/**
 * @brief   Sends a command with data over the QSPI bus.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void qspi_lld_send(QSPIDriver *qspip, const qspi_command_t *cmdp,
                   size_t n, const uint8_t *txbuf) {

  dmaStreamSetMemory0(qspip->dma, txbuf);
  dmaStreamSetTransactionSize(qspip->dma, n);
  dmaStreamSetMode(qspip->dma, qspip->dmamode | STM32_DMA_CR_DIR_M2P);

  qspip->qspi->DLR = n - 1;
  qspip->qspi->ABR = cmdp->alt;
  qspip->qspi->CCR = cmdp->cfg;
  qspip->qspi->AR  = cmdp->addr;

  dmaStreamEnable(qspip->dma);
}

/**
 * @brief   Sends a command then receives data over the QSPI bus.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 * @param[in] n         number of bytes to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void qspi_lld_receive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                      size_t n, uint8_t *rxbuf) {

  dmaStreamSetMemory0(qspip->dma, rxbuf);
  dmaStreamSetTransactionSize(qspip->dma, n);
  dmaStreamSetMode(qspip->dma, qspip->dmamode | STM32_DMA_CR_DIR_P2M);

  qspip->qspi->DLR = n - 1;
  qspip->qspi->ABR = cmdp->alt;
  qspip->qspi->CCR = cmdp->cfg | QUADSPI_CCR_FMODE_0;
  qspip->qspi->AR  = cmdp->addr;

  dmaStreamEnable(qspip->dma);
}

#endif /* HAL_USE_QSPI */

/** @} */
