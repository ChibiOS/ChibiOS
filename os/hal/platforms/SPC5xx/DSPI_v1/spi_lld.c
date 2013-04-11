/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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
 * @file    templates/spi_lld.c
 * @brief   SPI Driver subsystem low level driver source template.
 *
 * @addtogroup SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/* Some forward declarations.*/
static void spi_serve_rx_irq(edma_channel_t channel, void *p);
static void spi_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr);

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Enforced MCR bits.*/
#define DSPI_MCR_ENFORCED_BITS      (SPC5_MCR_MSTR)

/* Excluded MCR bits.*/
#define DSPI_MCR_EXCLUDED_BITS      (SPC5_MCR_CONT_SCKE     |               \
                                     SPC5_MCR_DCONF_MASK    |               \
                                     SPC5_MCR_ROOE          |               \
                                     SPC5_MCR_MDIS          |               \
                                     SPC5_MCR_DIS_TXF       |               \
                                     SPC5_MCR_DIS_RXF       |               \
                                     SPC5_MCR_CLR_TXF       |               \
                                     SPC5_MCR_CLR_RXF       |               \
                                     SPC5_MCR_HALT)

/* Excluded PUSHR bits.*/
#define DSPI_PUSHR_EXCLUDED_BITS    (SPC5_PUSHR_CTAS_MASK   |               \
                                     SPC5_PUSHR_EOQ         |               \
                                     SPC5_PUSHR_TXDATA_MASK)

#define DSPI_PUSHR8_ADDRESS(spip)   (((uint32_t)&(spip)->dspi->PUSHR.R) + 3)
#define DSPI_PUSHR16_ADDRESS(spip)  (((uint32_t)&(spip)->dspi->PUSHR.R) + 2)
#define DSPI_POPR8_ADDRESS(spip)    (((uint32_t)&(spip)->dspi->POPR.R) + 3)
#define DSPI_POPR16_ADDRESS(spip)   (((uint32_t)&(spip)->dspi->POPR.R) + 2)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SPID1 driver identifier.
 */
#if SPC5_SPI_USE_DSPI0 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/**
 * @brief   SPID2 driver identifier.
 */
#if SPC5_SPI_USE_DSPI1 || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif

/**
 * @brief   SPID3 driver identifier.
 */
#if SPC5_SPI_USE_DSPI2 || defined(__DOXYGEN__)
SPIDriver SPID3;
#endif

/**
 * @brief   SPID4 driver identifier.
 */
#if SPC5_SPI_USE_DSPI3 || defined(__DOXYGEN__)
SPIDriver SPID4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if SPC5_SPI_USE_DSPI0 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI0 TX.
 */
static const edma_channel_config_t spi_dspi0_tx_dma_config = {
  SPC5_DSPI0_TX_DMA_DEV_ID, SPC5_SPI_DSPI0_DMA_PRIO, SPC5_SPI_DSPI0_DMA_PRIO,
  NULL, spi_serve_dma_error_irq, &SPID1
};

/**
 * @brief   DMA configuration for DSPI0 RX.
 */
static const edma_channel_config_t spi_dspi0_rx_dma_config = {
  SPC5_DSPI0_RX_DMA_DEV_ID, SPC5_SPI_DSPI0_DMA_PRIO, SPC5_SPI_DSPI0_DMA_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID1
};
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI1 TX.
 */
static const edma_channel_config_t spi_dspi1_tx_dma_config = {
  SPC5_DSPI1_TX_DMA_DEV_ID, SPC5_SPI_DSPI1_DMA_PRIO, SPC5_SPI_DSPI1_DMA_PRIO,
  NULL, spi_serve_dma_error_irq, &SPID2
};

/**
 * @brief   DMA configuration for DSPI1 RX.
 */
static const edma_channel_config_t spi_dspi1_rx_dma_config = {
  SPC5_DSPI1_RX_DMA_DEV_ID, SPC5_SPI_DSPI1_DMA_PRIO, SPC5_SPI_DSPI1_DMA_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID2
};
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI2 TX.
 */
static const edma_channel_config_t spi_dspi2_tx_dma_config = {
  SPC5_DSPI2_TX_DMA_DEV_ID, SPC5_SPI_DSPI2_DMA_PRIO, SPC5_SPI_DSPI2_DMA_PRIO,
  NULL, spi_serve_dma_error_irq, &SPID3
};

/**
 * @brief   DMA configuration for DSPI2 RX.
 */
static const edma_channel_config_t spi_dspi2_rx_dma_config = {
  SPC5_DSPI2_RX_DMA_DEV_ID, SPC5_SPI_DSPI2_DMA_PRIO, SPC5_SPI_DSPI2_DMA_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID3
};
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI3 TX.
 */
static const edma_channel_config_t spi_dspi3_tx_dma_config = {
  SPC5_DSPI3_TX_DMA_DEV_ID, SPC5_SPI_DSPI3_DMA_PRIO, SPC5_SPI_DSPI3_DMA_PRIO,
  NULL, spi_serve_dma_error_irq, &SPID4
};

/**
 * @brief   DMA configuration for DSPI3 RX.
 */
static const edma_channel_config_t spi_dspi3_rx_dma_config = {
  SPC5_DSPI3_RX_DMA_DEV_ID, SPC5_SPI_DSPI3_DMA_PRIO, SPC5_SPI_DSPI3_DMA_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID4
};
#endif /* SPC5_SPI_USE_DSPI3 */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Starts eception using DMA for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
static void spi_start_dma_rx8(SPIDriver *spip,
                              size_t n,
                              uint8_t *rxbuf) {

  edmaChannelSetup(spip->rx_channel,            /* channel.                 */
                   DSPI_POPR8_ADDRESS(spip),    /* src.                     */
                   rxbuf,                       /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   1,                           /* doff, advance by one.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n,                           /* iter.                    */
                   0,                           /* slast.                   */
                   0,                           /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END);     /* mode.*/

  edmaChannelStart(spip->rx_channel);
}

/**
 * @brief   Starts reception using DMA for frames up to 16 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
static void spi_start_dma_rx16(SPIDriver *spip,
                               size_t n,
                               uint16_t *rxbuf) {

  edmaChannelSetup(spip->rx_channel,            /* channel.                 */
                   DSPI_POPR16_ADDRESS(spip),   /* src.                     */
                   rxbuf,                       /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   2,                           /* doff, advance by two.    */
                   1,                           /* ssize, 16 bits transfers.*/
                   1,                           /* dsize, 16 bits transfers.*/
                   2,                           /* nbytes, always two.      */
                   n,                           /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END);     /* mode.*/

  edmaChannelStart(spip->rx_channel);
}

/**
 * @brief   Starts transmission using DMA for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_start_dma_tx8(SPIDriver *spip,
                              size_t n,
                              const uint8_t *txbuf) {
  uint32_t cmd = spip->config->pushr & ~DSPI_PUSHR_EXCLUDED_BITS;
  uint8_t *cp = (uint8_t *)DSPI_PUSHR8_ADDRESS(spip);

  /* The first frame is pushed by the CPU, then the DMA is activated to
     send the following frames.*/
  spip->dspi->PUSHR.R = cmd | (uint32_t)*txbuf++;
  *cp = 0x55;

  /* Setting up TX DMA TCD parameters for 8 bits transfers.*/
  edmaChannelSetup(spip->tx_channel,            /* channel.                 */
                   txbuf,                       /* src.                     */
                   DSPI_PUSHR8_ADDRESS(spip),   /* dst.                     */
                   1,                           /* soff, advance by 1.      */
                   0,                           /* doff, do not advance.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n - 1,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ);         /* mode.                    */

  edmaChannelStart(spip->tx_channel);
}

/**
 * @brief   Starts transmission using DMA for frames up to 16 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_start_dma_tx16(SPIDriver *spip,
                               size_t n,
                               const uint16_t *txbuf) {
  uint32_t cmd = spip->config->pushr & ~DSPI_PUSHR_EXCLUDED_BITS;

  /* The first frame is pushed by the CPU, then the DMA is activated to
     send the following frames.*/
  spip->dspi->PUSHR.R = cmd | (uint32_t)*txbuf++;

  /* Setting up TX DMA TCD parameters for 16 bits transfers.*/
  edmaChannelSetup(spip->tx_channel,            /* channel.                 */
                   txbuf,                       /* src.                     */
                   DSPI_PUSHR16_ADDRESS(spip),  /* dst.                     */
                   2,                           /* soff, advance by 2.      */
                   0,                           /* doff, do not advance.    */
                   1,                           /* ssize, 16 bits transfers.*/
                   1,                           /* dsize, 16 bits transfers.*/
                   2,                           /* nbytes, always two.      */
                   n - 1,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ);         /* mode.                    */

  edmaChannelStart(spip->tx_channel);
}

/**
 * @brief   Starts transmission using FIFO pre-filling for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_tx_prefill8(SPIDriver *spip,
                            size_t n,
                            const uint8_t *txbuf) {
  uint32_t cmd = spip->config->pushr & ~DSPI_PUSHR_EXCLUDED_BITS;

  do {
    if (--n == 0) {
      spip->dspi->PUSHR.R = SPC5_PUSHR_EOQ | cmd | (uint32_t)*txbuf;
      break;
    }
    spip->dspi->PUSHR.R = cmd | (uint32_t)*txbuf;
    txbuf++;
  } while (TRUE);
}

/**
 * @brief   Starts transmission using FIFO pre-filling for frames up to 16 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
static void spi_tx_prefill16(SPIDriver *spip,
                             size_t n,
                             const uint16_t *txbuf) {
  uint32_t cmd = spip->config->pushr & ~DSPI_PUSHR_EXCLUDED_BITS;

  do {
    if (--n == 0) {
      spip->dspi->PUSHR.R = SPC5_PUSHR_EOQ | cmd | (uint32_t)*txbuf;
      break;
    }
    spip->dspi->PUSHR.R = cmd | (uint32_t)*txbuf;
    txbuf++;
  } while (TRUE);
}

static void spi_serve_rx_irq(edma_channel_t channel, void *p) {
  SPIDriver *spip = (SPIDriver *)p;

  (void)channel;

  /* Stops the DSPI and clears the queues.*/
  spip->dspi->MCR.R = SPC5_MCR_MSTR    | SPC5_MCR_HALT    |
                      SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;

  /* Portable SPI ISR code defined in the high level driver, note, it is
     a macro.*/
  _spi_isr_code(spip);
}

static void spi_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr) {
  SPIDriver *spip = (SPIDriver *)p;

  (void)channel;
  (void)esr;

  /* Stops the DSPI and clears the queues.*/
  spip->dspi->MCR.R = SPC5_MCR_MSTR    | SPC5_MCR_HALT    |
                      SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;

  SPC5_SPI_DMA_ERROR_HOOK(spip);
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

#if SPC5_SPI_USE_DSPI0
  /* Driver initialization.*/
  spiObjectInit(&SPID1);
  SPID1.dspi = &SPC5_DSPI0;
  SPID1.tx_channel = EDMA_ERROR;
  SPID1.rx_channel = EDMA_ERROR;
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1
  /* Driver initialization.*/
  spiObjectInit(&SPID2);
  SPID2.dspi = &SPC5_DSPI1;
  SPID2.tx_channel = EDMA_ERROR;
  SPID2.rx_channel = EDMA_ERROR;
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2
  /* Driver initialization.*/
  spiObjectInit(&SPID3);
  SPID3.dspi = &SPC5_DSPI2;
  SPID3.tx_channel = EDMA_ERROR;
  SPID3.rx_channel = EDMA_ERROR;
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI03
  /* Driver initialization.*/
  spiObjectInit(&SPID4);
  SPID4.dspi = &SPC5_DSPI3;
  SPID4.tx_channel = EDMA_ERROR;
  SPID4.rx_channel = EDMA_ERROR;
#endif /* SPC5_SPI_USE_DSPI3 */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  if (spip->state == SPI_STOP) {
    /* Enables the peripheral.*/

#if SPC5_SPI_USE_DSPI0
    if (&SPID1 == spip) {
      SPC5_DSPI0_ENABLE_CLOCK();
      spip->tx_channel = edmaChannelAllocate(&spi_dspi0_tx_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi0_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1
    if (&SPID2 == spip) {
      SPC5_DSPI1_ENABLE_CLOCK();
      spip->tx_channel = edmaChannelAllocate(&spi_dspi1_tx_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi1_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2
    if (&SPID3 == spip) {
      SPC5_DSPI2_ENABLE_CLOCK();
      spip->tx_channel = edmaChannelAllocate(&spi_dspi2_tx_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi2_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3
    if (&SPID4 == spip) {
      SPC5_DSPI3_ENABLE_CLOCK();
      spip->tx_channel = edmaChannelAllocate(&spi_dspi3_tx_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi3_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI3 */

    chDbgAssert((spip->tx_channel != EDMA_ERROR) &&
                (spip->rx_channel != EDMA_ERROR),
                "spi_lld_start(), #1", "channel cannot be allocated");
  }

  /* Configures the peripheral.*/
  spip->dspi->MCR.R     = SPC5_MCR_MSTR | SPC5_MCR_HALT | spip->config->mcr;
  spip->dspi->CTAR[0].R = spip->config->ctar0;
  spip->dspi->RSER.R    = SPC5_RSER_TFFF_RE | SPC5_RSER_TFFF_DIRS |
                          SPC5_RSER_RFDF_RE | SPC5_RSER_RFDF_DIRS;
  spip->dspi->SR.R      = spip->dspi->SR.R;
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
    /* Releases the allocated EDMA channels.*/
    edmaChannelRelease(spip->tx_channel);
    edmaChannelRelease(spip->rx_channel);

    /* Resets the peripheral.*/
    spip->dspi->CTAR[0].R = 0;
    spip->dspi->RSER.R    = 0;
    spip->dspi->SR.R      = spip->dspi->SR.R;
    spip->dspi->MCR.R     = SPC5_MCR_MSTR    | SPC5_MCR_MDIS    |
                            SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF |
                            SPC5_MCR_HALT;

#if SPC5_SPI_USE_DSPI0
    if (&SPID1 == spip) {
      SPC5_DSPI0_DISABLE_CLOCK();
    }
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1
    if (&SPID2 == spip) {
      SPC5_DSPI1_DISABLE_CLOCK();
    }
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2
    if (&SPID3 == spip) {
      SPC5_DSPI2_DISABLE_CLOCK();
    }
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3
    if (&SPID4 == spip) {
      SPC5_DSPI3_DISABLE_CLOCK();
    }
#endif /* SPC5_SPI_USE_DSPI3 */
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

  (void)spip;

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

  (void)spip;

}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  (void)spip;
  (void)n;

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
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  /* Starting transfer.*/
  spip->dspi->MCR.B.HALT = 0;

  /* DMAs require a different setup depending on the frame size.*/
  if (spip->dspi->CTAR[0].B.FMSZ < 8) {
    /* Setting up the RX DMA channel.*/
    spi_start_dma_rx8(spip, n, rxbuf);

    if (n <= SPC5_DSPI_FIFO_DEPTH) {
      /* If the total transfer size is smaller than the TX FIFO size then
         the whole transmitted data is pushed here and the TX DMA is not
         activated.*/
      spi_tx_prefill8(spip, n, txbuf);
    }
    else {
      spi_start_dma_tx8(spip, n, txbuf);
    }
  }
  else {
    /* Setting up the RX DMA channel.*/
    spi_start_dma_rx16(spip, n, rxbuf);

    if (n <= SPC5_DSPI_FIFO_DEPTH) {
      /* If the total transfer size is smaller than the TX FIFO size then
         the whole transmitted data is pushed here and the TX DMA is not
         activated.*/
      spi_tx_prefill16(spip, n, txbuf);
    }
    else {
      spi_start_dma_tx16(spip, n, txbuf);
    }
  }
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

  (void)spip;
  (void)n;
  (void)txbuf;

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
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  (void)spip;
  (void)n;
  (void)rxbuf;

}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {

  (void)spip;
  (void)frame;

  return 0;
}

#endif /* HAL_USE_SPI */

/** @} */
