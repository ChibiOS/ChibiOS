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
 * @file    hal_spi_lld.c
 * @brief   SAMA SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local macros.                                                      */
/*===========================================================================*/

/**
 * @brief   Enable write protection on SPI registers block.
 *
 * @param[in] spip    pointer to a SPI register block
 *
 * @notapi
 */
#define spiEnableWP(spip) {                                                  \
  spip->SPI_WPMR = SPI_WPMR_WPKEY_PASSWD | SPI_WPMR_WPEN;                    \
}

/**
 * @brief   Disable write protection on SPI registers block.
 *
 * @param[in] spip    pointer to a SPI register block
 *
 * @notapi
 */
#define spiDisableWP(spip) {                                                 \
  spip->SPI_WPMR = SPI_WPMR_WPKEY_PASSWD;                                    \
}


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SPI0 driver identifier.
 */
#if SAMA_SPI_USE_SPI0 || defined(__DOXYGEN__)
SPIDriver SPID0;
#endif

/**
 * @brief   SPI1 driver identifier.
 */
#if SAMA_SPI_USE_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint16_t dummytx = 0xFFFFU;
static uint16_t dummyrx;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared end-of-rx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void spi_lld_serve_rx_interrupt(SPIDriver *spip, uint32_t flags) {

  /* DMA errors handling.*/
  #if defined(SAMA_SPI_DMA_ERROR_HOOK)
  if ((flags & (XDMAC_CIS_RBEIS | XDMAC_CIS_ROIS)) != 0) {
    SAMA_SPI_DMA_ERROR_HOOK(spip);
  }
#else
  (void)flags;
#endif

  /* Stop everything.*/
  dmaChannelDisable(spip->dmatx);
  dmaChannelDisable(spip->dmarx);

  /* Portable SPI ISR code defined in the high level driver, note, it is
     a macro.*/
  _spi_isr_code(spip);
}

/**
 * @brief   Shared end-of-tx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void spi_lld_serve_tx_interrupt(SPIDriver *spip, uint32_t flags) {

  /* DMA errors handling.*/

#if defined(SAMA_SPI_DMA_ERROR_HOOK)
  (void)spip;
  if ((flags & (XDMAC_CIS_WBEIS | XDMAC_CIS_ROIS)) != 0) {
    SAMA_SPI_DMA_ERROR_HOOK(spip);
  }
#else
  (void)spip;
  (void)flags;
#endif
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
void spi_lld_init(void) {

#if SAMA_SPI_USE_SPI0
  /* Driver initialization.*/
  spiObjectInit(&SPID0);
  SPID0.spi       = SPI0;
  SPID0.dmarx     = NULL;
  SPID0.dmatx     = NULL;
  SPID0.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SINGLE |
                    XDMAC_CC_DSYNC_PER2MEM |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF1 |
                    XDMAC_CC_DIF_AHB_IF0 |
                    XDMAC_CC_SAM_FIXED_AM |
                    XDMAC_CC_DAM_INCREMENTED_AM |
                    XDMAC_CC_PERID(PERID_SPI0_RX);
  SPID0.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SINGLE |
                    XDMAC_CC_DSYNC_MEM2PER |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF0 |
                    XDMAC_CC_DIF_AHB_IF1 |
                    XDMAC_CC_SAM_INCREMENTED_AM |
                    XDMAC_CC_DAM_FIXED_AM |
                    XDMAC_CC_PERID(PERID_SPI0_TX);
#endif /* SAMA_SPI_USE_SPI0 */

#if SAMA_SPI_USE_SPI1
  /* Driver initialization.*/
  spiObjectInit(&SPID1);
  SPID1.spi       = SPI1;
  SPID1.dmarx     = NULL;
  SPID1.dmatx     = NULL;
  SPID1.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SINGLE |
                    XDMAC_CC_DSYNC_PER2MEM |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF1 |
                    XDMAC_CC_DIF_AHB_IF0 |
                    XDMAC_CC_SAM_FIXED_AM |
                    XDMAC_CC_DAM_INCREMENTED_AM |
                    XDMAC_CC_PERID(PERID_SPI1_RX);
  SPID1.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SINGLE |
                    XDMAC_CC_DSYNC_MEM2PER |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF0 |
                    XDMAC_CC_DIF_AHB_IF1 |
                    XDMAC_CC_SAM_INCREMENTED_AM |
                    XDMAC_CC_DAM_FIXED_AM |
                    XDMAC_CC_PERID(PERID_SPI1_TX);
#endif /* SAMA_SPI_USE_SPI1 */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  /* Configures the peripheral.*/

  if (spip->state == SPI_STOP) {
  /* Execute a software reset of the SPI twice */
    spip->spi->SPI_CR = SPI_CR_SWRST;
    spip->spi->SPI_CR = SPI_CR_SWRST;

#if SAMA_SPI_USE_SPI0
    if (&SPID0 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_SPI0_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_SPI0_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);

    /* Enable SPI0 clock */
      pmcEnableSPI0();

    }
#endif /* SAMA_SPI_USE_SPI0 */
#if SAMA_SPI_USE_SPI1
    if (&SPID1 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_SPI1_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_SPI1_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
    /* Enable SPI1 clock */
      pmcEnableSPI1();

    }
#endif /* SAMA_SPI_USE_SPI1 */
  }

  /* Disable write protection */
  spiDisableWP(spip->spi);
  /* SPI configuration */
  spip->spi->SPI_MR = spip->config->mr;
  spip->spi->SPI_CSR[spip->config->npcs] = spip->config->csr;
  /* Enable SPI */
  spip->spi->SPI_CR |= SPI_CR_SPIEN;
  /* Enable write protection.  */
  spiEnableWP(spip->spi);
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  if (spip->state == SPI_READY) {
  /* Disable write protection */
    spiDisableWP(spip->spi);
  /* Reset SPI */
    spip->spi->SPI_MR = 0;
    spip->spi->SPI_CSR[spip->config->npcs] = 0;
  /* Disable SPI */
    spip->spi->SPI_CR |= SPI_CR_SPIDIS;
  /* Enable write protection */
    spiEnableWP(spip->spi);
#if SAMA_SPI_USE_SPI0
    if (&SPID0 == spip)
    /* Disable SPI0 clock */
      pmcDisableSPI0();

#endif /* SAMA_SPI_USE_SPI1 */
#if SAMA_SPI_USE_SPI1
    if (&SPID1 == spip)
    /* Disable SPI1 clock */
      pmcDisableSPI1();

#endif /* SAMA_SPI_USE_SPI1 */
  }
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {

  /**
   * NOTE: This should only be called in master mode.
   */

  uint16_t pad =  spip->config->npcs;
  /* Disable write protection */
  spiDisableWP(spip->spi);

  spip->spi->SPI_MR &= ~SPI_MR_PCS_Msk;
  spip->spi->SPI_MR |=  SPI_PCS(pad);

  /* Enable write protection */
  spiEnableWP(spip->spi);

}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {

  /**
   * NOTE: This should only be called in master mode.
   */
  spip->spi->SPI_CR = SPI_CR_LASTXFER;

}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf    the pointer to the transmit buffer
 * @param[out]rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  osalDbgAssert(n > XDMAC_MAX_BT_SIZE, "unsupported DMA transfer size");
  /* Writing channel */
  dmaChannelSetSource(spip->dmatx, txbuf);
  dmaChannelSetDestination(spip->dmatx, &spip->spi->SPI_TDR);
  dmaChannelSetTransactionSize(spip->dmatx, n);
  dmaChannelSetMode(spip->dmatx, spip->txdmamode);
  /* Reading channel */
  dmaChannelSetSource(spip->dmarx, &spip->spi->SPI_RDR);
  dmaChannelSetDestination(spip->dmarx, rxbuf);
  dmaChannelSetTransactionSize(spip->dmarx, n);
  dmaChannelSetMode(spip->dmarx, spip->rxdmamode);

  dmaChannelEnable(spip->dmarx);
  dmaChannelEnable(spip->dmatx);

}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  osalDbgAssert(n > XDMAC_MAX_BT_SIZE, "unsupported DMA transfer size");
  /* Writing channel */
  dmaChannelSetSource(spip->dmatx, txbuf);
  dmaChannelSetDestination(spip->dmatx, &spip->spi->SPI_TDR);
  dmaChannelSetTransactionSize(spip->dmatx, n);
  dmaChannelSetMode(spip->dmatx, spip->txdmamode);
  /* Reading channel */
  dmaChannelSetSource(spip->dmarx, &spip->spi->SPI_RDR);
  dmaChannelSetDestination(spip->dmarx, &dummyrx);
  dmaChannelSetTransactionSize(spip->dmarx, n);
  dmaChannelSetMode(spip->dmarx, spip->rxdmamode);

  dmaChannelEnable(spip->dmarx);
  dmaChannelEnable(spip->dmatx);
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out]rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  osalDbgAssert(n > XDMAC_MAX_BT_SIZE, "unsupported DMA transfer size");
  /* Writing channel */
  dmaChannelSetSource(spip->dmatx, &dummytx);
  dmaChannelSetDestination(spip->dmatx, &spip->spi->SPI_TDR);
  dmaChannelSetTransactionSize(spip->dmatx, n);
  dmaChannelSetMode(spip->dmatx, spip->txdmamode);
  /* Reading channel */
  dmaChannelSetSource(spip->dmarx, &spip->spi->SPI_RDR);
  dmaChannelSetDestination(spip->dmarx, rxbuf);
  dmaChannelSetTransactionSize(spip->dmarx, n);
  dmaChannelSetMode(spip->dmarx, spip->rxdmamode);

  dmaChannelEnable(spip->dmarx);
  dmaChannelEnable(spip->dmatx);

}

#endif /* HAL_USE_SPI */

/** @} */
