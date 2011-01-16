/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM32/spi_lld.c
 * @brief   STM32 SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SPI1 driver identifier.*/
#if STM32_SPI_USE_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/** @brief SPI2 driver identifier.*/
#if STM32_SPI_USE_SPI2 || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif

/** @brief SPI3 driver identifier.*/
#if STM32_SPI_USE_SPI3 || defined(__DOXYGEN__)
SPIDriver SPID3;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static uint16_t dummytx;
static uint16_t dummyrx;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Stops the SPI DMA channels.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
#define dma_stop(spip) {                                                    \
  dmaChannelDisable(spip->spd_dmatx);                                       \
  dmaChannelDisable(spip->spd_dmarx);                                       \
}

/**
 * @brief   Starts the SPI DMA channels.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
#define dma_start(spip) {                                                   \
  dmaChannelEnable((spip)->spd_dmarx);                                      \
  dmaChannelEnable((spip)->spd_dmatx);                                      \
}

/**
 * @brief   Shared end-of-transfer service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
static void serve_interrupt(SPIDriver *spip) {

  /* Stop everything.*/
  dma_stop(spip);

  /* Portable SPI ISR code defined in the high level driver, note, it is
     a macro.*/
  _spi_isr_code(spip);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_SPI_USE_SPI1 || defined(__DOXYGEN__)
/**
 * @brief   SPI1 RX DMA interrupt handler (channel 2).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  if ((STM32_DMA1->ISR & DMA_ISR_TEIF2) != 0) {
    STM32_SPI_SPI1_DMA_ERROR_HOOK();
  }
  serve_interrupt(&SPID1);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_2);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   SPI1 TX DMA interrupt handler (channel 3).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch3_IRQHandler) {

  CH_IRQ_PROLOGUE();

  STM32_SPI_SPI1_DMA_ERROR_HOOK();
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_3);

  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_SPI_USE_SPI2 || defined(__DOXYGEN__)
/**
 * @brief   SPI2 RX DMA interrupt handler (channel 4).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch4_IRQHandler) {

  CH_IRQ_PROLOGUE();

  if ((STM32_DMA1->ISR & DMA_ISR_TEIF4) != 0) {
    STM32_SPI_SPI2_DMA_ERROR_HOOK();
  }
  serve_interrupt(&SPID2);
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_4);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   SPI2 TX DMA interrupt handler (channel 5).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA1_Ch5_IRQHandler) {

  CH_IRQ_PROLOGUE();

  STM32_SPI_SPI2_DMA_ERROR_HOOK();
  dmaClearChannel(STM32_DMA1, STM32_DMA_CHANNEL_5);

  CH_IRQ_EPILOGUE();
}
#endif

#if STM32_SPI_USE_SPI3 || defined(__DOXYGEN__)
/**
 * @brief   SPI3 RX DMA interrupt handler (DMA2, channel 1).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch1_IRQHandler) {

  CH_IRQ_PROLOGUE();

  if ((STM32_DMA2->ISR & DMA_ISR_TEIF1) != 0) {
    STM32_SPI_SPI3_DMA_ERROR_HOOK();
  }
  serve_interrupt(&SPID3);
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   SPI3 TX DMA2 interrupt handler (DMA2, channel 2).
 *
 * @isr
 */
CH_IRQ_HANDLER(DMA2_Ch2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  STM32_SPI_SPI3_DMA_ERROR_HOOK();
  dmaClearChannel(STM32_DMA2, STM32_DMA_CHANNEL_2);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {

  dummytx = 0xFFFF;

#if STM32_SPI_USE_SPI1
  RCC->APB2RSTR     = RCC_APB2RSTR_SPI1RST;
  RCC->APB2RSTR     = 0;
  spiObjectInit(&SPID1);
  SPID1.spd_thread  = NULL;
  SPID1.spd_spi     = SPI1;
  SPID1.spd_dmarx   = STM32_DMA1_CH2;
  SPID1.spd_dmatx   = STM32_DMA1_CH3;
#endif

#if STM32_SPI_USE_SPI2
  RCC->APB1RSTR     = RCC_APB1RSTR_SPI2RST;
  RCC->APB1RSTR     = 0;
  spiObjectInit(&SPID2);
  SPID2.spd_thread  = NULL;
  SPID2.spd_spi     = SPI2;
  SPID2.spd_dmarx   = STM32_DMA1_CH4;
  SPID2.spd_dmatx   = STM32_DMA1_CH5;
#endif

#if STM32_SPI_USE_SPI3
  RCC->APB1RSTR     = RCC_APB1RSTR_SPI3RST;
  RCC->APB1RSTR     = 0;
  spiObjectInit(&SPID3);
  SPID3.spd_thread  = NULL;
  SPID3.spd_spi     = SPI3;
  SPID3.spd_dmarx   = STM32_DMA2_CH1;
  SPID3.spd_dmatx   = STM32_DMA2_CH2;
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  /* If in stopped state then enables the SPI and DMA clocks.*/
  if (spip->spd_state == SPI_STOP) {
#if STM32_SPI_USE_SPI1
    if (&SPID1 == spip) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(DMA1_Channel2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI_SPI1_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI_SPI1_IRQ_PRIORITY));
      RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    }
#endif
#if STM32_SPI_USE_SPI2
    if (&SPID2 == spip) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(DMA1_Channel4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI_SPI2_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel5_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI_SPI2_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    }
#endif
#if STM32_SPI_USE_SPI3
    if (&SPID3 == spip) {
      dmaEnable(DMA2_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(DMA2_Channel1_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI_SPI3_IRQ_PRIORITY));
      NVICEnableVector(DMA2_Channel2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI_SPI3_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
    }
#endif

    /* DMA setup.*/
    dmaChannelSetPeripheral(spip->spd_dmarx, &spip->spd_spi->DR);
    dmaChannelSetPeripheral(spip->spd_dmatx, &spip->spd_spi->DR);
  }

  /* More DMA setup.*/
  if ((spip->spd_config->spc_cr1 & SPI_CR1_DFF) == 0)
    spip->spd_dmaccr = (STM32_SPI_SPI2_DMA_PRIORITY << 12) |
                       DMA_CCR1_TEIE;               /* 8 bits transfers.    */
  else
    spip->spd_dmaccr = (STM32_SPI_SPI2_DMA_PRIORITY << 12) |
                       DMA_CCR1_TEIE | DMA_CCR1_MSIZE_0 |
                       DMA_CCR1_PSIZE_0;            /* 16 bits transfers.   */

  /* SPI setup and enable.*/
  spip->spd_spi->CR1 = 0;
  spip->spd_spi->CR2 = SPI_CR2_SSOE | SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
  spip->spd_spi->CR1 = spip->spd_config->spc_cr1 | SPI_CR1_MSTR | SPI_CR1_SPE;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  /* If in ready state then disables the SPI clock.*/
  if (spip->spd_state == SPI_READY) {

    /* SPI disable.*/
    spip->spd_spi->CR1 = 0;

#if STM32_SPI_USE_SPI1
    if (&SPID1 == spip) {
      NVICDisableVector(DMA1_Channel2_IRQn);
      NVICDisableVector(DMA1_Channel3_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
    }
#endif
#if STM32_SPI_USE_SPI2
    if (&SPID2 == spip) {
      NVICDisableVector(DMA1_Channel4_IRQn);
      NVICDisableVector(DMA1_Channel5_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
    }
#endif
#if STM32_SPI_USE_SPI3
    if (&SPID3 == spip) {
      NVICDisableVector(DMA2_Channel1_IRQn);
      NVICDisableVector(DMA2_Channel2_IRQn);
      dmaDisable(DMA2_ID);
      RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;
    }
#endif
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

  palClearPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
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

  palSetPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
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

  dmaChannelSetup(spip->spd_dmarx, n, &dummyrx,
                  spip->spd_dmaccr | DMA_CCR1_TCIE | DMA_CCR1_EN);
  dmaChannelSetup(spip->spd_dmatx, n, &dummytx,
                  spip->spd_dmaccr | DMA_CCR1_DIR | DMA_CCR1_EN);
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

  dmaChannelSetup(spip->spd_dmarx, n, rxbuf,
                  spip->spd_dmaccr | DMA_CCR1_TCIE | DMA_CCR1_MINC |
                  DMA_CCR1_EN);
  dmaChannelSetup(spip->spd_dmatx, n, txbuf,
                  spip->spd_dmaccr | DMA_CCR1_DIR | DMA_CCR1_MINC |
                  DMA_CCR1_EN);
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

  dmaChannelSetup(spip->spd_dmarx, n, &dummyrx,
                  spip->spd_dmaccr | DMA_CCR1_TCIE | DMA_CCR1_EN);
  dmaChannelSetup(spip->spd_dmatx, n, txbuf,
                  spip->spd_dmaccr | DMA_CCR1_DIR | DMA_CCR1_MINC |
                  DMA_CCR1_EN);
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

  dmaChannelSetup(spip->spd_dmarx, n, rxbuf,
                  spip->spd_dmaccr | DMA_CCR1_TCIE | DMA_CCR1_MINC |
                  DMA_CCR1_EN);
  dmaChannelSetup(spip->spd_dmatx, n, &dummytx,
                  spip->spd_dmaccr | DMA_CCR1_DIR | DMA_CCR1_EN);
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

  spip->spd_spi->DR = frame;
  while ((spip->spd_spi->SR & SPI_SR_RXNE) == 0)
    ;
  return spip->spd_spi->DR;
}

#endif /* HAL_USE_SPI */

/** @} */
