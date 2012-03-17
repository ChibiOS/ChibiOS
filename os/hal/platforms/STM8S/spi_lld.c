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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    STM8S/spi_lld.c
 * @brief   STM8S low level SPI driver code.
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

#if STM8S_SPI_USE_SPI || defined(__DOXYGEN__)
/** @brief SPI1 driver identifier.*/
SPIDriver SPID1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM8S_SPI_USE_SPI || defined(__DOXYGEN__)
/**
 * @brief   IRQ 10 service routine.
 *
 * @isr
 */
CH_IRQ_HANDLER(10) {

  CH_IRQ_PROLOGUE();

  if ((SPI->SR & SPI_SR_OVR) != 0) {
    /* The overflow condition should never happen because priority is given
       to receive but a hook macro is provided anyway...*/
    STM8S_SPI_ERROR_HOOK(&SPID1);
  }
  /* Handling the DR register like it is a FIFO with depth>1 in order to
     handle the case where a frame arrives immediately after reading the
     DR register.*/
  while ((SPI->SR & SPI_SR_RXNE) != 0) {
    if (SPID1.spd_rxptr != NULL)
       *SPID1.spd_rxptr++ = SPI->DR;
    else
      (void)SPI->DR;
    if (--SPID1.spd_rxcnt == 0) {
      chDbgAssert(SPID1.spd_txcnt == 0,
                  "IRQ10, #1", "counter out of synch");
      /* Stops all the IRQ sources.*/
      SPI->ICR = 0;
      /* Portable SPI ISR code defined in the high level driver, note, it
         is a macro.*/
      _spi_isr_code(&SPID1);
      /* Goto because it is mandatory to go through the epilogue, cannot
         just return.*/
      goto exit_isr;
    }
  }
  /* Loading the DR register.*/
  if ((SPI->SR & SPI_SR_TXE) != 0) {
    if (SPID1.spd_txptr != NULL)
      SPI->DR = *SPID1.spd_txptr++;
    else
      SPI->DR = 0xFF;
  }

exit_isr:
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

#if STM8S_SPI_USE_SPI
  spiObjectInit(&SPID1);
#endif /* STM8S_SPI_USE_SPI */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  /* Clock activation.*/
  CLK->PCKENR1 |= CLK_PCKENR1_SPI;              /* PCKEN11, clock source.   */

  /* Configuration.*/
  SPI->CR1  = 0;
  SPI->CR1  = spip->config->cr1 | SPI_CR1_MSTR;
  SPI->CR2  = SPI_CR2_SSI | SPI_CR2_SSM;
  SPI->CR1 |= SPI_CR1_SPE;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  (void)spip;

  /* Reset state.*/
  SPI->CR1 = 0;
  SPI->CR2 = 0;
  SPI->ICR = 0;

  /* Clock de-activation.*/
  CLK->PCKENR1 &= (uint8_t)~CLK_PCKENR1_SPI;    /* PCKEN11, clock source.   */
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
 * @details This function transmits a series of idle words on the SPI bus and
 *          ignores the received data. This function can be invoked even
 *          when a slave select signal has not been yet asserted.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  spip->spd_rxptr = NULL;
  spip->spd_txptr = NULL;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  SPI->ICR = SPI_ICR_TXEI | SPI_ICR_RXEI | SPI_ICR_ERRIE;
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

  spip->spd_rxptr = rxbuf;
  spip->spd_txptr = txbuf;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  SPI->ICR = SPI_ICR_TXEI | SPI_ICR_RXEI | SPI_ICR_ERRIE;
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

  spip->spd_rxptr = NULL;
  spip->spd_txptr = txbuf;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  SPI->ICR = SPI_ICR_TXEI | SPI_ICR_RXEI | SPI_ICR_ERRIE;
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

  spip->spd_rxptr = rxbuf;
  spip->spd_txptr = NULL;
  spip->spd_rxcnt = spip->spd_txcnt = n;
  SPI->ICR = SPI_ICR_TXEI | SPI_ICR_RXEI | SPI_ICR_ERRIE;
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
uint8_t spi_lld_polled_exchange(SPIDriver *spip, uint8_t frame) {

  (void)spip;

  SPI->DR = (uint32_t)frame;
  while ((SPI->SR & SPI_SR_RXNE) == 0)
    ;
  return (uint16_t)SPI->DR;
}

#endif /* HAL_USE_SPI */

/** @} */
