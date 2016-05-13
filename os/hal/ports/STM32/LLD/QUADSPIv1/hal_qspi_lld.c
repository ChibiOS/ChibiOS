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

#define SPI1_RX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_QSPI_QUADSPI1_DMA_STREAM,                      \
                       STM32_QUADSPI1_RX_DMA_CHN)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SPI1 driver identifier.*/
#if STM32_QSPI_USE_QUADSPI1 || defined(__DOXYGEN__)
SPIDriver QSPID1;
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

  /* DMA errors handling.*/
#if defined(STM32_QSPI_DMA_ERROR_HOOK)
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    STM32_QSPI_DMA_ERROR_HOOK(qspip);
  }
#else
  (void)flags;
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

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void qspi_lld_init(void) {

#if STM32_QSPI_USE_QUADSPI1
  qspiObjectInit(&QSPID1);
  QSPID1.spi        = SPI1;
  QSPID1.dma        = STM32_DMA_STREAM(STM32_QSPI_QUADSPI1_DMA_STREAM);
  QSPID1.dmamode    = STM32_DMA_CR_CHSEL(SPI1_RX_DMA_CHANNEL) |
                      STM32_DMA_CR_PL(STM32_QSPI_QUADSPI1_DMA_PRIORITY) |
                      STM32_DMA_CR_PSIZE_BYTE |
                      STM32_DMA_CR_MSIZE_BYTE |
                      STM32_DMA_CR_MINC |
                      STM32_DMA_CR_DMEIE |
                      STM32_DMA_CR_TEIE;
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
  uint32_t ds;

  /* If in stopped state then enables the SPI and DMA clocks.*/
  if (qspip->state == QSPI_STOP) {
#if STM32_SPI_USE_SPI1
    if (&SPID1 == qspip) {
      rccEnableQUADSPI1(FALSE);
    }
#endif

  /* QSPI setup and enable.*/
//  spip->spi->CR1  = 0;
//  spip->spi->CR1  = spip->config->cr1 | SPI_CR1_MSTR;
//  spip->spi->CR2  = spip->config->cr2 | SPI_CR2_FRXTH | SPI_CR2_SSOE |
//                    SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
//  spip->spi->CR1 |= SPI_CR1_SPE;
}

/**
 * @brief   Deactivates the QSPI peripheral.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @notapi
 */
void qspi_lld_stop(QSPIDriver *qspip) {

  /* If in ready state then disables the SPI clock.*/
  if (qspip->state == QSPI_READY) {

    /* QSPI disable.*/
//    spip->spi->CR1 = 0;
//    spip->spi->CR2 = 0;
    dmaStreamRelease(qspip->dma);

#if STM32_QSPI_USE_QUADSPI1
    if (&QSPID1 == qspip)
      rccDisableQUADSPI1(FALSE);
#endif
  }
}

/**
 * @brief   Sends data over the QSPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void qspi_lld_send(QSPIDriver *qspip, const qspi_command_t *cmdp,
                   size_t n, const uint8_t *txbuf) {

  dmaStreamSetMemory0(qspip->dma, txbuf);
  dmaStreamSetTransactionSize(qspip->dma, n);
  dmaStreamSetMode(qspip->dma, qspip->dmamode | STM32_DMA_CR_DIR_M2P);

  dmaStreamEnable(qspip->dma);
}

/**
 * @brief   Receives data from the QSPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] cmd       pointer to the command descriptor
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void qspi_lld_receive(QSPIDriver *qspip, const qspi_command_t *cmdp,
                      size_t n, uint8_t *rxbuf) {

  dmaStreamSetMemory0(qspip->dma, rxbuf);
  dmaStreamSetTransactionSize(qspip->dma, n);
  dmaStreamSetMode(qspip->dma, qspip->dmamode | STM32_DMA_CR_DIR_P2M);

  dmaStreamEnable(qspip->dmarx);
}

#endif /* HAL_USE_QSPI */

/** @} */
