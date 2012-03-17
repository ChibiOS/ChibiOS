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
 * @file    AT91SAM7/spi_lld.c
 * @brief   AT91SAM7 low level SPI driver code.
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

#if AT91SAM7_SPI_USE_SPI0 || defined(__DOXYGEN__)
/** @brief SPI1 driver identifier.*/
SPIDriver SPID1;
#endif

#if AT91SAM7_SPI_USE_SPI1 || defined(__DOXYGEN__)
/** @brief SPI2 driver identifier.*/
SPIDriver SPID2;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Idle line value.
 * @details This thing's DMA apparently does not allow to *not* increment
 *          the memory pointer so a buffer filled with ones is required
 *          somewhere.
 * @note    This buffer size also limits the maximum transfer size, 512B,
 *          for @p spiReceive() and @p spiIgnore(). @p spiSend() and
 *          @p spiExchange are not affected.
 */
static const uint16_t idle_buf[] = {
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Initializes a SPI device.
 */
static void spi_init(AT91PS_SPI spi) {

  /* Software reset must be written twice (errata for revision B parts).*/
  spi->SPI_CR   = AT91C_SPI_SWRST;
  spi->SPI_CR   = AT91C_SPI_SWRST;
  spi->SPI_RCR  = 0;
  spi->SPI_RNCR = 0;
  spi->SPI_TCR  = 0;
  spi->SPI_TNCR = 0;
  spi->SPI_PTCR = AT91C_PDC_RXTDIS | AT91C_PDC_TXTDIS;
  spi->SPI_MR   = AT91C_SPI_MSTR | AT91C_SPI_MODFDIS;
}

#if defined(__GNUC__)
__attribute__((noinline))
#endif
/**
 * @brief   Shared interrupt handling code.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
static void spi_lld_serve_interrupt(SPIDriver *spip) {
  uint32_t sr = spip->spd_spi->SPI_SR;

  if ((sr & AT91C_SPI_ENDRX) != 0) {
    (void)spip->spd_spi->SPI_RDR;               /* Clears eventual overflow.*/
    spip->spd_spi->SPI_PTCR = AT91C_PDC_RXTDIS |
                              AT91C_PDC_TXTDIS; /* PDC disabled.            */
    spip->spd_spi->SPI_IDR  = AT91C_SPI_ENDRX;  /* Interrupt disabled.      */
    spip->spd_spi->SPI_CR   = AT91C_SPI_SPIDIS; /* SPI disabled.            */
    /* Portable SPI ISR code defined in the high level driver, note, it is
       a macro.*/
    _spi_isr_code(spip);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if AT91SAM7_SPI_USE_SPI0 || defined(__DOXYGEN__)
/**
 * @brief   SPI0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPI0IrqHandler) {

  CH_IRQ_PROLOGUE();
  spi_lld_serve_interrupt(&SPID1);
  AT91C_BASE_AIC->AIC_EOICR = 0;
  CH_IRQ_EPILOGUE();
}
#endif

#if AT91SAM7_SPI_USE_SPI1 || defined(__DOXYGEN__)
/**
 * @brief   SPI1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPI1IrqHandler) {

  CH_IRQ_PROLOGUE();
  spi_lld_serve_interrupt(&SPID2);
  AT91C_BASE_AIC->AIC_EOICR = 0;
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

#if AT91SAM7_SPI_USE_SPI0
  spiObjectInit(&SPID1);
  SPID1.spd_spi = AT91C_BASE_SPI0;
  spi_init(AT91C_BASE_SPI0);
  AT91C_BASE_PIOA->PIO_PDR   = SPI0_MISO | SPI0_MOSI | SPI0_SCK;
  AT91C_BASE_PIOA->PIO_ASR   = SPI0_MISO | SPI0_MOSI | SPI0_SCK;
  AT91C_BASE_PIOA->PIO_PPUDR = SPI0_MISO | SPI0_MOSI | SPI0_SCK;
  AIC_ConfigureIT(AT91C_ID_SPI0,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | AT91SAM7_SPI0_PRIORITY,
                  SPI0IrqHandler);
#endif

#if AT91SAM7_SPI_USE_SPI1
  spiObjectInit(&SPID2);
  SPID2.spd_spi = AT91C_BASE_SPI1;
  spi_init(AT91C_BASE_SPI1);
  AT91C_BASE_PIOA->PIO_PDR   = SPI1_MISO | SPI1_MOSI | SPI1_SCK;
  AT91C_BASE_PIOA->PIO_BSR   = SPI1_MISO | SPI1_MOSI | SPI1_SCK;
  AT91C_BASE_PIOA->PIO_PPUDR = SPI1_MISO | SPI1_MOSI | SPI1_SCK;
  AIC_ConfigureIT(AT91C_ID_SPI1,
                  AT91C_AIC_SRCTYPE_HIGH_LEVEL | AT91SAM7_SPI1_PRIORITY,
                  SPI1IrqHandler);
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

  if (spip->spd_state == SPI_STOP) {
#if AT91SAM7_SPI_USE_SPI0
    if (&SPID1 == spip) {
      /* Clock activation.*/
      AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_SPI0);
      /* Enables associated interrupt vector.*/
      AIC_EnableIT(AT91C_ID_SPI0);
    }
#endif
#if AT91SAM7_SPI_USE_SPI1
    if (&SPID2 == spip) {
      /* Clock activation.*/
      AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_SPI1);
      /* Enables associated interrupt vector.*/
      AIC_EnableIT(AT91C_ID_SPI1);
    }
#endif
  }
  /* Configuration.*/
  spip->spd_spi->SPI_CSR[0] = spip->spd_config->spc_csr;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  if (spip->spd_state != SPI_STOP) {
#if AT91SAM7_SPI_USE_SPI0
    if (&SPID1 == spip) {
      AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_SPI0);
      AIC_DisableIT(AT91C_ID_SPI0);
    }
#endif
#if AT91SAM7_SPI_USE_SPI1
    if (&SPID1 == spip) {
      AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_SPI1);
      AIC_DisableIT(AT91C_ID_SPI0);
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

  spip->spd_spi->SPI_TCR  = n;
  spip->spd_spi->SPI_RCR  = n;
  spip->spd_spi->SPI_TPR  = (AT91_REG)idle_buf;
  spip->spd_spi->SPI_RPR  = (AT91_REG)idle_buf;
  spip->spd_spi->SPI_IER  = AT91C_SPI_ENDRX;
  spip->spd_spi->SPI_CR   = AT91C_SPI_SPIEN;
  spip->spd_spi->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This function performs a simultaneous transmit/receive operation.
 * @note    The buffers are organized as uint8_t arrays.
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

  spip->spd_spi->SPI_TCR  = n;
  spip->spd_spi->SPI_RCR  = n;
  spip->spd_spi->SPI_TPR  = (AT91_REG)txbuf;
  spip->spd_spi->SPI_RPR  = (AT91_REG)rxbuf;
  spip->spd_spi->SPI_IER  = AT91C_SPI_ENDRX;
  spip->spd_spi->SPI_CR   = AT91C_SPI_SPIEN;
  spip->spd_spi->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
}

/**
 * @brief   Sends data over the SPI bus.
 * @note    The buffers are organized as uint8_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  spip->spd_spi->SPI_TCR  = n;
  spip->spd_spi->SPI_RCR  = n;
  spip->spd_spi->SPI_TPR  = (AT91_REG)txbuf;
  spip->spd_spi->SPI_RPR  = (AT91_REG)idle_buf;
  spip->spd_spi->SPI_IER  = AT91C_SPI_ENDRX;
  spip->spd_spi->SPI_CR   = AT91C_SPI_SPIEN;
  spip->spd_spi->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
}

/**
 * @brief   Receives data from the SPI bus.
 * @note    The buffers are organized as uint8_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  spip->spd_spi->SPI_TCR  = n;
  spip->spd_spi->SPI_RCR  = n;
  spip->spd_spi->SPI_TPR  = (AT91_REG)idle_buf;
  spip->spd_spi->SPI_RPR  = (AT91_REG)rxbuf;
  spip->spd_spi->SPI_IER  = AT91C_SPI_ENDRX;
  spip->spd_spi->SPI_CR   = AT91C_SPI_SPIEN;
  spip->spd_spi->SPI_PTCR = AT91C_PDC_RXTEN | AT91C_PDC_TXTEN;
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

  spip->spd_spi->SPI_CR   = AT91C_SPI_SPIEN;
  spip->spd_spi->SPI_TDR = frame;
  while ((spip->spd_spi->SPI_SR & AT91C_SPI_RDRF) == 0)
    ;
  return spip->spd_spi->SPI_RDR;
}

#endif /* HAL_USE_SPI */

/** @} */
