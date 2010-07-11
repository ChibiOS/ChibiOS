/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file STM32/spi_lld.c
 * @brief STM32 SPI subsystem low level driver source.
 * @addtogroup STM32_SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SPI1 driver identifier.*/
#if USE_STM32_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/** @brief SPI2 driver identifier.*/
#if USE_STM32_SPI2 || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static uint16_t dummyrx;
static uint16_t dummytx;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void spi_stop(SPIDriver *spip) {

  /* Stops RX and TX DMA channels.*/
  spip->spd_dmarx->CCR = 0;
  spip->spd_dmatx->CCR = 0;

  /* Stops SPI operations.*/
  spip->spd_spi->CR1 &= ~SPI_CR1_SPE;

  chSysLockFromIsr();
  chSchReadyI(spip->spd_thread);
  chSysUnlockFromIsr();
}

static void spi_start_wait(SPIDriver *spip, size_t n,
                           const void *txbuf, void *rxbuf) {
  uint32_t ccr;

  /* Common DMA setup.*/
  ccr = spip->spd_dmaprio;
  if ((spip->spd_config->spc_cr1 & SPI_CR1_DFF) != 0)
    ccr |= DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0; /* 16 bits transfer.*/

  /* RX DMA setup.*/
  spip->spd_dmarx->CMAR = (uint32_t)rxbuf;
  spip->spd_dmarx->CNDTR = (uint32_t)n;
  spip->spd_dmarx->CCR |= ccr;

  /* TX DMA setup.*/
  spip->spd_dmatx->CMAR = (uint32_t)txbuf;
  spip->spd_dmatx->CNDTR = (uint32_t)n;
  spip->spd_dmatx->CCR |= ccr;

  /* SPI enable.*/
  chSysLock();
  spip->spd_spi->CR1 |= SPI_CR1_SPE;

  /* DMAs start.*/
  spip->spd_dmarx->CCR |= DMA_CCR1_EN;
  spip->spd_dmatx->CCR |= DMA_CCR1_EN;

  /* Wait for completion event.*/
  spip->spd_thread = currp;
  chSchGoSleepS(THD_STATE_SUSPENDED);
  spip->spd_thread = NULL;
  chSysUnlock();
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if USE_STM32_SPI1 || defined(__DOXYGEN__)
/**
 * @brief SPI1 RX DMA interrupt handler (channel 2).
 */
CH_IRQ_HANDLER(Vector70) {

  CH_IRQ_PROLOGUE();

  spi_stop(&SPID1);
  if ((DMA1->ISR & DMA_ISR_TEIF2) != 0) {
    STM32_SPI1_DMA_ERROR_HOOK();
  }
  DMA1->IFCR |= DMA_IFCR_CGIF2  | DMA_IFCR_CTCIF2 |
                DMA_IFCR_CHTIF2 | DMA_IFCR_CTEIF2;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief SPI1 TX DMA interrupt handler (channel 3).
 */
CH_IRQ_HANDLER(Vector74) {

  CH_IRQ_PROLOGUE();

  STM32_SPI1_DMA_ERROR_HOOK();
  DMA1->IFCR |= DMA_IFCR_CGIF3  | DMA_IFCR_CTCIF3 |
                DMA_IFCR_CHTIF3 | DMA_IFCR_CTEIF3;

  CH_IRQ_EPILOGUE();
}
#endif

#if USE_STM32_SPI2 || defined(__DOXYGEN__)
/**
 * @brief SPI2 RX DMA interrupt handler (channel 4).
 */
CH_IRQ_HANDLER(Vector78) {

  CH_IRQ_PROLOGUE();

  spi_stop(&SPID2);
  if ((DMA1->ISR & DMA_ISR_TEIF4) != 0) {
    STM32_SPI2_DMA_ERROR_HOOK();
  }
  DMA1->IFCR |= DMA_IFCR_CGIF4  | DMA_IFCR_CTCIF4 |
                DMA_IFCR_CHTIF4 | DMA_IFCR_CTEIF4;

  CH_IRQ_EPILOGUE();
}

/**
 * @brief SPI2 TX DMA interrupt handler (channel 5).
 */
CH_IRQ_HANDLER(Vector7C) {

  CH_IRQ_PROLOGUE();

  STM32_SPI2_DMA_ERROR_HOOK();
  DMA1->IFCR |= DMA_IFCR_CGIF5  | DMA_IFCR_CTCIF5 |
                DMA_IFCR_CHTIF5 | DMA_IFCR_CTEIF5;

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level SPI driver initialization.
 */
void spi_lld_init(void) {

  dummytx = 0xFFFF;

#if USE_STM32_SPI1
  RCC->APB2RSTR     = RCC_APB2RSTR_SPI1RST;
  RCC->APB2RSTR     = 0;
  spiObjectInit(&SPID1);
  SPID1.spd_thread  = NULL;
  SPID1.spd_spi     = SPI1;
  SPID1.spd_dmarx   = DMA1_Channel2;
  SPID1.spd_dmatx   = DMA1_Channel3;
  SPID1.spd_dmaprio = STM32_SPI1_DMA_PRIORITY << 12;
#endif

#if USE_STM32_SPI2
  RCC->APB1RSTR     = RCC_APB1RSTR_SPI2RST;
  RCC->APB1RSTR     = 0;
  spiObjectInit(&SPID2);
  SPID2.spd_thread  = NULL;
  SPID2.spd_spi     = SPI2;
  SPID2.spd_dmarx   = DMA1_Channel4;
  SPID2.spd_dmatx   = DMA1_Channel5;
  SPID2.spd_dmaprio = STM32_SPI2_DMA_PRIORITY << 12;
#endif
}

/**
 * @brief Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_start(SPIDriver *spip) {

  /* If in stopped state then enables the SPI and DMA clocks.*/
  if (spip->spd_state == SPI_STOP) {
#if USE_STM32_SPI1
    if (&SPID1 == spip) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(DMA1_Channel2_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI1_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel3_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI1_IRQ_PRIORITY));
      RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    }
#endif
#if USE_STM32_SPI2
    if (&SPID2 == spip) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(DMA1_Channel4_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI2_IRQ_PRIORITY));
      NVICEnableVector(DMA1_Channel5_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_SPI2_IRQ_PRIORITY));
      RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    }
#endif
  }

  /* SPI setup.*/
  spip->spd_spi->CR1 = spip->spd_config->spc_cr1 | SPI_CR1_MSTR;
  spip->spd_spi->CR2 = SPI_CR2_SSOE | SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

  /* DMA setup.*/
  spip->spd_dmarx->CPAR = (uint32_t)&spip->spd_spi->DR;
  spip->spd_dmatx->CPAR = (uint32_t)&spip->spd_spi->DR;
}

/**
 * @brief Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_stop(SPIDriver *spip) {

  /* If in ready state then disables the SPI clock.*/
  if (spip->spd_state == SPI_READY) {
#if USE_STM32_SPI1
    if (&SPID1 == spip) {
      NVICDisableVector(DMA1_Channel2_IRQn);
      NVICDisableVector(DMA1_Channel3_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
    }
#endif
#if USE_STM32_SPI2
    if (&SPID2 == spip) {
      NVICDisableVector(DMA1_Channel4_IRQn);
      NVICDisableVector(DMA1_Channel5_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;
    }
#endif
  }
}

/**
 * @brief Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_select(SPIDriver *spip) {

  palClearPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
}

/**
 * @brief Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_unselect(SPIDriver *spip) {

  palSetPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
}

/**
 * @brief Ignores data on the SPI bus.
 * @details This function transmits a series of idle words on the SPI bus and
 *          ignores the received data. This function can be invoked even
 *          when a slave select signal has not been yet asserted.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  spip->spd_dmarx->CCR = DMA_CCR1_TCIE | DMA_CCR1_TEIE;
  spip->spd_dmatx->CCR = DMA_CCR1_DIR  | DMA_CCR1_TEIE;
  spi_start_wait(spip, n, &dummytx, &dummyrx);
}

/**
 * @brief Exchanges data on the SPI bus.
 * @details This function performs a simultaneous transmit/receive operation.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  spip->spd_dmarx->CCR = DMA_CCR1_TCIE | DMA_CCR1_MINC | DMA_CCR1_TEIE;
  spip->spd_dmatx->CCR = DMA_CCR1_DIR  | DMA_CCR1_MINC | DMA_CCR1_TEIE;
  spi_start_wait(spip, n, txbuf, rxbuf);
}

/**
 * @brief Sends data ever the SPI bus.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  spip->spd_dmarx->CCR = DMA_CCR1_TCIE | DMA_CCR1_TEIE;
  spip->spd_dmatx->CCR = DMA_CCR1_DIR  | DMA_CCR1_MINC | DMA_CCR1_TEIE;
  spi_start_wait(spip, n, txbuf, &dummyrx);
}

/**
 * @brief Receives data from the SPI bus.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  spip->spd_dmarx->CCR = DMA_CCR1_TCIE | DMA_CCR1_MINC | DMA_CCR1_TEIE;
  spip->spd_dmatx->CCR = DMA_CCR1_DIR  | DMA_CCR1_TEIE;
  spi_start_wait(spip, n, &dummytx, rxbuf);
}

#endif /* CH_HAL_USE_SPI */

/** @} */
