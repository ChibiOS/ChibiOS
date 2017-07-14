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
 * @brief   AVR SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define DUMMY_SPI_SEND_VALUE    0xFF

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SPI1 driver identifier.
 */
#if AVR_SPI_USE_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if AVR_SPI_USE_SPI1 || defined(__DOXYGEN__)
/**
 * @brief   SPI event interrupt handler.
 *
 * @notapi
 */
OSAL_IRQ_HANDLER(SPI_STC_vect) {
  OSAL_IRQ_PROLOGUE();

  SPIDriver *spip = &SPID1;

  /* a new value has arrived, store it if we are interested in it */
  if (spip->rxbuf) spip->rxbuf[spip->exidx] = SPDR;

  /* check if we are done */
  if (++(spip->exidx) >= spip->exbytes) {
    _spi_isr_code(spip);
  } else { /* if not done send the next byte */
     if (spip->txbuf) { /* if there is a buffer with values to be send then use it*/
       SPDR = spip->txbuf[spip->exidx];
    } else {            /* if there isn't a buffer with values to be send then send a the dummy value*/
      SPDR = DUMMY_SPI_SEND_VALUE;
    }
  }
  OSAL_IRQ_EPILOGUE();
}
#endif /* AVR_SPI_USE_SPI1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {

#if AVR_SPI_USE_SPI1
  /* Driver initialization.*/
  spiObjectInit(&SPID1);
#endif /* AVR_SPI_USE_SPI1 */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  uint8_t dummy;

  /* Configures the peripheral.*/

  if (spip->state == SPI_STOP) {
    /* Enables the peripheral.*/
#if AVR_SPI_USE_SPI1
    if (&SPID1 == spip) {
    /* Enable SPI clock using Power Reduction Register */
#if defined(PRR0)
      PRR0 &= ~(1 << PRSPI);
#elif defined(PRR)
      PRR &= ~(1 << PRSPI);
#endif

      /* SPI enable, SPI interrupt enable */
      SPCR |= ((1 << SPE) | (1 << SPIE));

      SPCR |= (1 << MSTR);
      DDR_SPI1 |=  ((1 << SPI1_MOSI) | (1 << SPI1_SCK));
      DDR_SPI1 &= ~(1 << SPI1_MISO);
      spip->config->ssport->dir |= (1 << spip->config->sspad);

      switch (spip->config->bitorder) {
      case SPI_LSB_FIRST:
        SPCR |= (1 << DORD);
        break;
      case SPI_MSB_FIRST: /* fallthrough */
      default:
        SPCR &= ~(1 << DORD);
        break;
      }

      SPCR &= ~((1 << CPOL) | (1 << CPHA));
      switch (spip->config->mode) {
      case SPI_MODE_1:
        SPCR |= (1 << CPHA);
        break;
      case SPI_MODE_2:
        SPCR |= (1 << CPOL);
        break;
      case SPI_MODE_3:
        SPCR |= ((1 << CPOL) | (1 << CPHA));
        break;
      case SPI_MODE_0: /* fallthrough */
      default: break;
      }

      SPCR &= ~((1 << SPR1) | (1 << SPR0));
      SPSR &= ~(1 << SPI2X);
      switch (spip->config->clockrate) {
      case SPI_SCK_FOSC_2:
        SPSR |= (1 << SPI2X);
        break;
      case SPI_SCK_FOSC_8:
        SPSR |= (1 << SPI2X);
        SPCR |= (1 << SPR0);
        break;
      case SPI_SCK_FOSC_16:
        SPCR |= (1 << SPR0);
        break;
      case SPI_SCK_FOSC_32:
        SPSR |= (1 << SPI2X);
        SPCR |= (1 << SPR1);
        break;
      case SPI_SCK_FOSC_64:
        SPCR |= (1 << SPR1);
        break;
      case SPI_SCK_FOSC_128:
        SPCR |= ((1 << SPR1) | (1 << SPR0));
        break;
      case SPI_SCK_FOSC_4: /* fallthrough */
      default: break;
      }

      /* dummy reads before enabling interrupt */
      dummy = SPSR;
      dummy = SPDR;
      (void) dummy; /* suppress warning about unused variable */
      SPCR |= (1 << SPIE);
    }
#endif /* AVR_SPI_USE_SPI1 */
  }
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
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if AVR_SPI_USE_SPI1
    if (&SPID1 == spip) {
      SPCR &= ((1 << SPIE) | (1 << SPE));
      spip->config->ssport->dir &= ~(1 << spip->config->sspad);
    }
/* Disable SPI clock using Power Reduction Register */
#if defined(PRR0)
      PRR0 |= (1 << PRSPI);
#elif defined(PRR)
      PRR |= (1 << PRSPI);
#endif
#endif /* AVR_SPI_USE_SPI1 */
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
  spip->config->ssport->out &= ~(1 << spip->config->sspad);

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
  spip->config->ssport->out |= (1 << spip->config->sspad);

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
void spi_lld_exchange(SPIDriver *spip, size_t n, const void *txbuf, void *rxbuf) {

  spip->txbuf = txbuf;
  spip->rxbuf = rxbuf;
  spip->exidx = 0;
  spip->exbytes = n;
  SPDR = (spip->txbuf ? spip->txbuf[0] : DUMMY_SPI_SEND_VALUE);
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
#if AVR_SPI_USE_16BIT_POLLED_EXCHANGE
uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame) {

  uint16_t spdr = 0;
  uint8_t dummy;

  /* disable interrupt */
  SPCR &= ~(1 << SPIE);

  SPDR = frame >> 8;
  while (!(SPSR & (1 << SPIF))) ;
  spdr = SPDR << 8;

  SPDR = frame & 0xFF;
  while (!(SPSR & (1 << SPIF))) ;
  spdr |= SPDR;

  dummy = SPSR;
  dummy = SPDR;
  (void) dummy; /* suppress warning about unused variable */
  SPCR |= (1 << SPIE);

  return spdr;
}
#else /* AVR_SPI_USE_16BIT_POLLED_EXCHANGE */
uint8_t spi_lld_polled_exchange(SPIDriver *spip, uint8_t frame) {

  uint8_t spdr = 0;
  uint8_t dummy;

  /* disable interrupt */
  SPCR &= ~(1 << SPIE);

  SPDR = frame;
  while (!(SPSR & (1 << SPIF))) ;
  spdr = SPDR;

  dummy = SPSR;
  dummy = SPDR;
  (void) dummy; /* suppress warning about unused variable */
  SPCR |= (1 << SPIE);

  return spdr;
}
#endif /* AVR_SPI_USE_16BIT_POLLED_EXCHANGE */

#endif /* HAL_USE_SPI */

/** @} */
