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
 * @file    SPC5xx/DSPI_v1/spi_lld.c
 * @brief   SPC5xx SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/* Some forward declarations.*/
static void spi_serve_rx_irq(edma_channel_t channel, void *p);
static void spi_serve_tx_irq(edma_channel_t channel, void *p);
static void spi_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr);

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Excluded PUSHR bits.*/
#define DSPI_PUSHR_EXCLUDED_BITS    (SPC5_PUSHR_CTAS_MASK   |               \
                                     SPC5_PUSHR_EOQ         |               \
                                     SPC5_PUSHR_TXDATA_MASK)

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

/**
 * @brief   SPID5 driver identifier.
 */
#if SPC5_SPI_USE_DSPI4 || defined(__DOXYGEN__)
SPIDriver SPID5;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if SPC5_SPI_USE_DSPI0 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI0 TX1.
 */
static const edma_channel_config_t spi_dspi0_tx1_dma_config = {
  SPC5_SPI_DSPI0_TX1_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI0_TX1_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI0_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID1
};

/**
 * @brief   DMA configuration for DSPI0 TX2.
 */
static const edma_channel_config_t spi_dspi0_tx2_dma_config = {
  SPC5_SPI_DSPI0_TX2_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  0,
#endif
  SPC5_SPI_DSPI0_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID1
};

/**
 * @brief   DMA configuration for DSPI0 RX.
 */
static const edma_channel_config_t spi_dspi0_rx_dma_config = {
  SPC5_SPI_DSPI0_RX_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI0_RX_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI0_DMA_IRQ_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID1
};
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI1 TX1.
 */
static const edma_channel_config_t spi_dspi1_tx1_dma_config = {
  SPC5_SPI_DSPI1_TX1_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI1_TX1_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI1_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID2
};

/**
 * @brief   DMA configuration for DSPI1 TX2.
 */
static const edma_channel_config_t spi_dspi1_tx2_dma_config = {
  SPC5_SPI_DSPI1_TX2_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  0,
#endif
  SPC5_SPI_DSPI1_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID2
};

/**
 * @brief   DMA configuration for DSPI1 RX.
 */
static const edma_channel_config_t spi_dspi1_rx_dma_config = {
  SPC5_SPI_DSPI1_RX_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI1_RX_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI1_DMA_IRQ_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID2
};
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI2 TX1.
 */
static const edma_channel_config_t spi_dspi2_tx1_dma_config = {
  SPC5_SPI_DSPI2_TX1_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI2_TX1_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI2_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID3
};

/**
 * @brief   DMA configuration for DSPI2 TX2.
 */
static const edma_channel_config_t spi_dspi2_tx2_dma_config = {
  SPC5_SPI_DSPI2_TX2_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  0,
#endif
  SPC5_SPI_DSPI2_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID3
};

/**
 * @brief   DMA configuration for DSPI2 RX.
 */
static const edma_channel_config_t spi_dspi2_rx_dma_config = {
  SPC5_SPI_DSPI2_RX_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI2_RX_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI2_DMA_IRQ_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID3
};
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI3 TX1.
 */
static const edma_channel_config_t spi_dspi3_tx1_dma_config = {
  SPC5_SPI_DSPI3_TX1_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI3_TX1_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI3_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID4
};

/**
 * @brief   DMA configuration for DSPI3 TX2.
 */
static const edma_channel_config_t spi_dspi3_tx2_dma_config = {
  SPC5_SPI_DSPI3_TX2_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  0,
#endif
  SPC5_SPI_DSPI3_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID4
};

/**
 * @brief   DMA configuration for DSPI3 RX.
 */
static const edma_channel_config_t spi_dspi3_rx_dma_config = {
  SPC5_SPI_DSPI3_RX_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI3_RX_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI3_DMA_IRQ_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID4
};
#endif /* SPC5_SPI_USE_DSPI3 */

#if SPC5_SPI_USE_DSPI4 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for DSPI4 TX1.
 */
static const edma_channel_config_t spi_dspi4_tx1_dma_config = {
  SPC5_SPI_DSPI4_TX1_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI4_TX1_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI4_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID5
};

/**
 * @brief   DMA configuration for DSPI4 TX2.
 */
static const edma_channel_config_t spi_dspi4_tx2_dma_config = {
  SPC5_SPI_DSPI4_TX2_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  0,
#endif
  SPC5_SPI_DSPI4_DMA_IRQ_PRIO,
  spi_serve_tx_irq, spi_serve_dma_error_irq, &SPID5
};

/**
 * @brief   DMA configuration for DSPI4 RX.
 */
static const edma_channel_config_t spi_dspi4_rx_dma_config = {
  SPC5_SPI_DSPI4_RX_DMA_CH_ID,
#if SPC5_EDMA_HAS_MUX
  SPC5_DSPI4_RX_DMA_DEV_ID,
#endif
  SPC5_SPI_DSPI4_DMA_IRQ_PRIO,
  spi_serve_rx_irq, spi_serve_dma_error_irq, &SPID5
};
#endif /* SPC5_SPI_USE_DSPI4 */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Starts reception using DMA ignoring the received data.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 *
 * @notapi
 */
static void spi_start_dma_rx_ignore(SPIDriver *spip, size_t n) {
  static uint32_t datasink;

  edmaChannelSetup(spip->rx_channel,            /* channel.                 */
                   DSPI_POPR8_ADDRESS(spip),    /* src.                     */
                   &datasink,                   /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   0,                           /* ssize, 16 bits transfers.*/
                   0,                           /* dsize, 16 bits transfers.*/
                   1,                           /* nbytes, always one.      */
                   n,                           /* iter.                    */
                   0,                           /* slast.                   */
                   0,                           /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END);     /* mode.*/

  edmaChannelStart(spip->rx_channel);
}

/**
 * @brief   Starts reception using DMA for frames up to 8 bits.
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
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  edmaChannelStart(spip->rx_channel);
}

/**
 * @brief   Starts transmission using DMA for frames up to 8 bits.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 *
 * @notapi
 */
static void spi_start_dma_tx_ignore(SPIDriver *spip, size_t n) {

  /* Preparing the TX intermediate buffer with the fixed part.*/
  spip->tx_intbuf = spip->config->pushr | (uint32_t)0xFFFF;

  /* The first frame is pushed by the CPU, then the DMA is activated to
     send the following frames. This should reduce latency on the operation
     start.*/
  spip->dspi->PUSHR.R = spip->tx_last = spip->tx_intbuf;

  /* Setting up TX1 DMA TCD parameters for 32 bits transfers.*/
  edmaChannelSetup(spip->tx1_channel,           /* channel.                 */
                   &spip->tx_intbuf,            /* src.                     */
                   &spip->dspi->PUSHR.R,        /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   2,                           /* ssize, 32 bits transfers.*/
                   2,                           /* dsize, 32 bits transfers.*/
                   4,                           /* nbytes, always four.     */
                   n - 2,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* Starting TX1 DMA channel.*/
  edmaChannelStart(spip->tx1_channel);
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

  /* Preparing the TX intermediate buffer with the fixed part.*/
  spip->tx_intbuf = spip->config->pushr;

  /* The first frame is pushed by the CPU, then the DMA is activated to
     send the following frames. This should reduce latency on the operation
     start.*/
  spip->dspi->PUSHR.R = spip->tx_intbuf | (uint32_t)*txbuf;

  /* Setting up TX1 DMA TCD parameters for 8 bits transfers.*/
  edmaChannelSetupLinked(
                   spip->tx1_channel,           /* channel.                 */
                   spip->tx2_channel,           /* linkch.                  */
                   txbuf + 1,                   /* src.                     */
                   ((const uint8_t *)&spip->tx_intbuf) + 3,     /* dst.     */
                   1,                           /* soff, advance by 1.      */
                   0,                           /* doff, do not advance.    */
                   0,                           /* ssize, 8 bits transfers. */
                   0,                           /* dsize, 8 bits transfers. */
                   1,                           /* nbytes, always one.      */
                   n - 2,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ);         /* mode.                    */

  /* Setting up TX2 DMA TCD parameters for 32 bits transfers.*/
  edmaChannelSetup(spip->tx2_channel,           /* channel.                 */
                   &spip->tx_intbuf,            /* src.                     */
                   &spip->dspi->PUSHR.R,        /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   2,                           /* ssize, 32 bits transfers.*/
                   2,                           /* dsize, 32 bits transfers.*/
                   4,                           /* nbytes, always four.     */
                   n - 2,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* The last frame will be pushed by the TX DMA operation completion
     callback.*/
  spip->tx_last = txbuf[n - 1];

  /* Starting TX DMA channels.*/
  edmaChannelStart(spip->tx2_channel);
  edmaChannelStart(spip->tx1_channel);
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

  /* Preparing the TX intermediate buffer with the fixed part.*/
  spip->tx_intbuf = spip->config->pushr;

  /* The first frame is pushed by the CPU, then the DMA is activated to
     send the following frames. This should reduce latency on the operation
     start.*/
  spip->dspi->PUSHR.R = spip->tx_intbuf | (uint32_t)*txbuf;

  /* Setting up TX1 DMA TCD parameters for 8 bits transfers.*/
  edmaChannelSetupLinked(
                   spip->tx1_channel,           /* channel.                 */
                   spip->tx2_channel,           /* linkch.                  */
                   txbuf + 1,                   /* src.                     */
                   ((const uint8_t *)&spip->tx_intbuf) + 2,     /* dst.     */
                   1,                           /* soff, advance by 1.      */
                   0,                           /* doff, do not advance.    */
                   1,                           /* ssize, 16 bits transfers.*/
                   1,                           /* dsize, 16 bits transfers.*/
                   1,                           /* nbytes, always one.      */
                   n - 2,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ);         /* mode.                    */

  /* Setting up TX2 DMA TCD parameters for 32 bits transfers.*/
  edmaChannelSetup(spip->tx2_channel,           /* channel.                 */
                   &spip->tx_intbuf,            /* src.                     */
                   &spip->dspi->PUSHR.R,        /* dst.                     */
                   0,                           /* soff, do not advance.    */
                   0,                           /* doff, do not advance.    */
                   2,                           /* ssize, 32 bits transfers.*/
                   2,                           /* dsize, 32 bits transfers.*/
                   4,                           /* nbytes, always four.     */
                   n - 2,                       /* iter.                    */
                   0,                           /* slast, no source adjust. */
                   0,                           /* dlast, no dest.adjust.   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END); /* mode.    */

  /* The last frame will be pushed by the TX DMA operation completion
     callback.*/
  spip->tx_last = txbuf[n - 1];

  /* Starting TX DMA channels.*/
  edmaChannelStart(spip->tx2_channel);
  edmaChannelStart(spip->tx1_channel);
}

/**
 * @brief   Starts idle bits using FIFO pre-filling.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 *
 * @notapi
 */
static void spi_tx_prefill_ignore(SPIDriver *spip, size_t n) {
  uint32_t cmd = spip->config->pushr;

  do {
    if (--n == 0) {
      spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | cmd | (uint32_t)0xFFFF) &
                            ~SPC5_PUSHR_CONT;
      break;
    }
    spip->dspi->PUSHR.R = cmd | (uint32_t)0xFFFF;
  } while (TRUE);
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
  uint32_t cmd = spip->config->pushr;

  do {
    if (--n == 0) {
      spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | cmd | (uint32_t)*txbuf) &
                            ~SPC5_PUSHR_CONT;
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
  uint32_t cmd = spip->config->pushr;

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
 * @brief   Shared RX DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void spi_serve_rx_irq(edma_channel_t channel, void *p) {
  SPIDriver *spip = (SPIDriver *)p;

  /* Clearing RX channel state.*/
  edmaChannelStop(channel);

  /* Stops the DSPI and clears the queues.*/
  spip->dspi->MCR.R |= SPC5_MCR_HALT | SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;

  /* Portable SPI ISR code defined in the high level driver, note, it is
     a macro.*/
  _spi_isr_code(spip);
}

/**
 * @brief   Shared TX1/TX2 DMA events service routine.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void spi_serve_tx_irq(edma_channel_t channel, void *p) {
  SPIDriver *spip = (SPIDriver *)p;

  (void)channel;

  /* Clearing TX channels state.*/
  edmaChannelStop(spip->tx1_channel);
  edmaChannelStop(spip->tx2_channel);

  /* If the TX FIFO is full then the push of the last frame is delagated to
     an interrupt handler else it is performed immediately. Both conditions
     can be true depending on the SPI speed and ISR latency.*/
  if (spip->dspi->SR.B.TFFF) {
    spip->dspi->PUSHR.R = (spip->config->pushr | spip->tx_last | SPC5_PUSHR_EOQ) &
                          ~SPC5_PUSHR_CONT;
  }
  else {
    spip->dspi->RSER.B.TFFFDIRS = 0;
  }
}

/**
 * @brief   Shared ISR for DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 *
 * @notapi
 */
static void spi_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr) {
  SPIDriver *spip = (SPIDriver *)p;

  (void)channel;
  (void)esr;

  /* Stops the DSPI and clears the queues.*/
  spip->dspi->MCR.R |= SPC5_MCR_HALT | SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;

  edmaChannelStop(spip->tx1_channel);
  edmaChannelStop(spip->tx2_channel);
  edmaChannelStop(spip->rx_channel);

  SPC5_SPI_DMA_ERROR_HOOK(spip);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_SPI_USE_DSPI0 || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI0_TFFF_HANDLER)
#error "SPC5_DSPI0_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI0 TFFF interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_DSPI0_TFFF_HANDLER) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();

  /* Interrupt served and back to DMA mode.*/
  SPC5_DSPI0.RSER.B.TFFFDIRS = 1;
  SPC5_DSPI0.SR.B.TFFF = 1;

  /* Pushing last frame.*/
  SPC5_DSPI0.PUSHR.R = (SPID1.config->pushr | SPID1.tx_last | SPC5_PUSHR_EOQ) &
                        ~SPC5_PUSHR_CONT;

  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1 || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI1_TFFF_HANDLER)
#error "SPC5_DSPI1_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI1 TFFF interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_DSPI1_TFFF_HANDLER) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();

  /* Interrupt served and back to DMA mode.*/
  SPC5_DSPI1.RSER.B.TFFFDIRS = 1;
  SPC5_DSPI1.SR.B.TFFF = 1;

  /* Pushing last frame.*/
  SPC5_DSPI1.PUSHR.R = (SPID2.config->pushr | SPID2.tx_last | SPC5_PUSHR_EOQ) &
                        ~SPC5_PUSHR_CONT;

  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2 || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI2_TFFF_HANDLER)
#error "SPC5_DSPI2_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI2 TFFF interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_DSPI2_TFFF_HANDLER) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();

  /* Interrupt served and back to DMA mode.*/
  SPC5_DSPI2.RSER.B.TFFFDIRS = 1;
  SPC5_DSPI2.SR.B.TFFF = 1;

  /* Pushing last frame.*/
  SPC5_DSPI2.PUSHR.R = (SPID3.config->pushr | SPID3.tx_last | SPC5_PUSHR_EOQ) &
                        ~SPC5_PUSHR_CONT;

  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3 || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI3_TFFF_HANDLER)
#error "SPC5_DSPI3_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI3 TFFF interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_DSPI3_TFFF_HANDLER) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();

  /* Interrupt served and back to DMA mode.*/
  SPC5_DSPI3.RSER.B.TFFFDIRS = 1;
  SPC5_DSPI3.SR.B.TFFF = 1;

  /* Pushing last frame.*/
  SPC5_DSPI3.PUSHR.R = (SPID4.config->pushr | SPID4.tx_last | SPC5_PUSHR_EOQ) &
                        ~SPC5_PUSHR_CONT;

  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI3 */

#if SPC5_SPI_USE_DSPI4 || defined(__DOXYGEN__)
#if !defined(SPC5_DSPI4_TFFF_HANDLER)
#error "SPC5_DSPI4_TFFF_HANDLER not defined"
#endif
/**
 * @brief   DSPI4 TFFF interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_DSPI4_TFFF_HANDLER) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();

  /* Interrupt served and back to DMA mode.*/
  SPC5_DSPI4.RSER.B.TFFFDIRS = 1;
  SPC5_DSPI4.SR.B.TFFF = 1;

  /* Pushing last frame.*/
  SPC5_DSPI4.PUSHR.R = (SPID5.config->pushr | SPID5.tx_last | SPC5_PUSHR_EOQ) &
                        ~SPC5_PUSHR_CONT;

  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_SPI_USE_DSPI4 */

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
  SPC5_DSPI0_ENABLE_CLOCK();
  SPID1.dspi = &SPC5_DSPI0;
  SPID1.tx1_channel = EDMA_ERROR;
  SPID1.tx2_channel = EDMA_ERROR;
  SPID1.rx_channel  = EDMA_ERROR;
  SPC5_DSPI0.MCR.R  = SPC5_MCR_MSTR | SPC5_MCR_HALT | SPC5_MCR_MDIS |
                      SPC5_SPI_DSPI0_MCR;
  INTC.PSR[SPC5_DSPI0_TFFF_NUMBER].R = SPC5_SPI_DSPI0_IRQ_PRIO;
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1
  /* Driver initialization.*/
  SPC5_DSPI1_ENABLE_CLOCK();
  spiObjectInit(&SPID2);
  SPID2.dspi = &SPC5_DSPI1;
  SPID2.tx1_channel = EDMA_ERROR;
  SPID2.tx2_channel = EDMA_ERROR;
  SPID2.rx_channel  = EDMA_ERROR;
  SPC5_DSPI1.MCR.R  = SPC5_MCR_MSTR | SPC5_MCR_HALT | SPC5_MCR_MDIS |
                      SPC5_SPI_DSPI1_MCR;
  INTC.PSR[SPC5_DSPI1_TFFF_NUMBER].R = SPC5_SPI_DSPI1_IRQ_PRIO;
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2
  /* Driver initialization.*/
  spiObjectInit(&SPID3);
  SPC5_DSPI2_ENABLE_CLOCK();
  SPID3.dspi = &SPC5_DSPI2;
  SPID3.tx1_channel = EDMA_ERROR;
  SPID3.tx2_channel = EDMA_ERROR;
  SPID3.rx_channel  = EDMA_ERROR;
  SPC5_DSPI2.MCR.R  = SPC5_MCR_MSTR | SPC5_MCR_HALT | SPC5_MCR_MDIS |
                      SPC5_SPI_DSPI2_MCR;
  INTC.PSR[SPC5_DSPI2_TFFF_NUMBER].R = SPC5_SPI_DSPI2_IRQ_PRIO;
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3
  /* Driver initialization.*/
  spiObjectInit(&SPID4);
  SPC5_DSPI3_ENABLE_CLOCK();
  SPID4.dspi = &SPC5_DSPI3;
  SPID4.tx1_channel = EDMA_ERROR;
  SPID4.tx2_channel = EDMA_ERROR;
  SPID4.rx_channel  = EDMA_ERROR;
  SPC5_DSPI3.MCR.R  = SPC5_MCR_MSTR | SPC5_MCR_HALT | SPC5_MCR_MDIS |
                      SPC5_SPI_DSPI3_MCR;
  INTC.PSR[SPC5_DSPI3_TFFF_NUMBER].R = SPC5_SPI_DSPI3_IRQ_PRIO;
#endif /* SPC5_SPI_USE_DSPI3 */

#if SPC5_SPI_USE_DSPI4
  /* Driver initialization.*/
  spiObjectInit(&SPID5);
  SPC5_DSPI4_ENABLE_CLOCK();
  SPID5.dspi = &SPC5_DSPI4;
  SPID5.tx1_channel = EDMA_ERROR;
  SPID5.tx2_channel = EDMA_ERROR;
  SPID5.rx_channel  = EDMA_ERROR;
  SPC5_DSPI4.MCR.R  = SPC5_MCR_MSTR | SPC5_MCR_HALT | SPC5_MCR_MDIS |
                      SPC5_SPI_DSPI4_MCR;
  INTC.PSR[SPC5_DSPI4_TFFF_NUMBER].R = SPC5_SPI_DSPI4_IRQ_PRIO;
#endif /* SPC5_SPI_USE_DSPI4 */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  chDbgAssert((spip->config->pushr & DSPI_PUSHR_EXCLUDED_BITS) == 0,
              "spi_lld_start(), #1", "invalid PUSHR bits specified");

  if (spip->state == SPI_STOP) {
    /* Enables the peripheral.*/

#if SPC5_SPI_USE_DSPI0
    if (&SPID1 == spip) {
      spip->tx1_channel = edmaChannelAllocate(&spi_dspi0_tx1_dma_config);
      spip->tx2_channel = edmaChannelAllocate(&spi_dspi0_tx2_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi0_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI0 */

#if SPC5_SPI_USE_DSPI1
    if (&SPID2 == spip) {
      spip->tx1_channel = edmaChannelAllocate(&spi_dspi1_tx1_dma_config);
      spip->tx2_channel = edmaChannelAllocate(&spi_dspi1_tx2_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi1_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI1 */

#if SPC5_SPI_USE_DSPI2
    if (&SPID3 == spip) {
      spip->tx1_channel = edmaChannelAllocate(&spi_dspi2_tx1_dma_config);
      spip->tx2_channel = edmaChannelAllocate(&spi_dspi2_tx2_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi2_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI2 */

#if SPC5_SPI_USE_DSPI3
    if (&SPID4 == spip) {
      spip->tx1_channel = edmaChannelAllocate(&spi_dspi3_tx1_dma_config);
      spip->tx2_channel = edmaChannelAllocate(&spi_dspi3_tx2_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi3_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI3 */

#if SPC5_SPI_USE_DSPI4
    if (&SPID5 == spip) {
      spip->tx1_channel = edmaChannelAllocate(&spi_dspi4_tx1_dma_config);
      spip->tx2_channel = edmaChannelAllocate(&spi_dspi4_tx2_dma_config);
      spip->rx_channel = edmaChannelAllocate(&spi_dspi4_rx_dma_config);
    }
#endif /* SPC5_SPI_USE_DSPI5 */

    chDbgAssert((spip->tx1_channel != EDMA_ERROR) &&
                (spip->tx2_channel != EDMA_ERROR) &&
                (spip->rx_channel != EDMA_ERROR),
                "spi_lld_start(), #2", "channel cannot be allocated");
  }

  /* Configures the peripheral.*/
  spip->dspi->MCR.B.MDIS = 0;
  spip->dspi->CTAR[0].R  = spip->config->ctar0;
  spip->dspi->RSER.R     = SPC5_RSER_TFFF_RE | SPC5_RSER_TFFF_DIRS |
                           SPC5_RSER_RFDF_RE | SPC5_RSER_RFDF_DIRS;
  spip->dspi->SR.R       = spip->dspi->SR.R;
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
    edmaChannelRelease(spip->tx1_channel);
    edmaChannelRelease(spip->tx2_channel);
    edmaChannelRelease(spip->rx_channel);

    /* Resets the peripheral.*/
    spip->dspi->CTAR[0].R  = 0;
    spip->dspi->RSER.R     = 0;
    spip->dspi->SR.R       = spip->dspi->SR.R;
    spip->dspi->MCR.R     |= SPC5_MCR_HALT |
                             SPC5_MCR_CLR_TXF | SPC5_MCR_CLR_RXF;
    spip->dspi->MCR.B.MDIS = 1;
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

  palClearPad(spip->config->ssport, spip->config->sspad);
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

  palSetPad(spip->config->ssport, spip->config->sspad);
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

  /* Starting transfer.*/
  spip->dspi->SR.R  = spip->dspi->SR.R;
  spip->dspi->MCR.B.HALT = 0;

  /* Setting up the RX DMA channel.*/
  spi_start_dma_rx_ignore(spip, n);

  if (n <= SPC5_DSPI_FIFO_DEPTH) {
    /* If the total transfer size is smaller than the TX FIFO size then
       the whole transmitted data is pushed here and the TX DMA is not
       activated.*/
    spi_tx_prefill_ignore(spip, n);
  }
  else {
    spi_start_dma_tx_ignore(spip, n);
  }
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
  spip->dspi->SR.R  = spip->dspi->SR.R;
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

  /* Starting transfer.*/
  spip->dspi->SR.R  = spip->dspi->SR.R;
  spip->dspi->MCR.B.HALT = 0;

  /* Setting up the RX DMA channel.*/
  spi_start_dma_rx_ignore(spip, n);

  /* DMAs require a different setup depending on the frame size.*/
  if (spip->dspi->CTAR[0].B.FMSZ < 8) {
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

  /* Starting transfer.*/
  spip->dspi->SR.R  = spip->dspi->SR.R;
  spip->dspi->MCR.B.HALT = 0;

  /* DMAs require a different setup depending on the frame size.*/
  if (spip->dspi->CTAR[0].B.FMSZ < 8) {
    /* Setting up the RX DMA channel.*/
    spi_start_dma_rx8(spip, n, rxbuf);
  }
  else {
    /* Setting up the RX DMA channel.*/
    spi_start_dma_rx16(spip, n, rxbuf);
  }

  if (n <= SPC5_DSPI_FIFO_DEPTH) {
    /* If the total transfer size is smaller than the TX FIFO size then
       the whole transmitted data is pushed here and the TX DMA is not
       activated.*/
    spi_tx_prefill_ignore(spip, n);
  }
  else {
    spi_start_dma_tx_ignore(spip, n);
  }
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
  uint32_t popr;

  spip->dspi->MCR.B.HALT = 0;
  spip->dspi->PUSHR.R = (SPC5_PUSHR_EOQ | spip->config->pushr |
                         (uint32_t)frame) & ~SPC5_PUSHR_CONT;
  while (!spip->dspi->SR.B.RFDF)
    ;
  popr = spip->dspi->POPR.R;
  spip->dspi->MCR.B.HALT = 1;
  return (uint16_t)popr;
}

#endif /* HAL_USE_SPI */

/** @} */
