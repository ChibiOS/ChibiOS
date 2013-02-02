/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

/**
 * @file    STM32/i2s_lld.c
 * @brief   I2S Driver subsystem low level driver source template.
 *
 * @addtogroup I2S
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_I2S || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2S driver initialization.
 *
 * @notapi
 */
void i2s_lld_init(void) {

#if STM32_I2S_USE_I2S2
  spiObjectInit(&I2SD2);
  I2SD2.spi = SPI2;
#endif

#if STM32_I2S_USE_I2S3
  spiObjectInit(&I2SD3);
  I2SD3.spi = SPI3;
#endif
}

/**
 * @brief   Configures and activates the I2S peripheral.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @notapi
 */
void i2s_lld_start(I2SDriver *i2sp) {

  /* If in stopped state then enables the SPI and DMA clocks.*/
  if (i2sp->state == I2S_STOP) {
#if STM32_SPI_USE_SPI2
    if (&SPID2 == spip) {
      bool_t b;
      b = dmaStreamAllocate(spip->dma,
                            STM32_I2S_I2S2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                            (void *)spip);
      chDbgAssert(!b, "spi_lld_start(), #1", "stream already allocated");
      rccEnableSPI2(FALSE);
    }
#endif
#if STM32_SPI_USE_SPI3
    if (&SPID3 == spip) {
      bool_t b;
      b = dmaStreamAllocate(spip->dma,
                            STM32_I2S_I2S3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                            (void *)spip);
      chDbgAssert(!b, "spi_lld_start(), #2", "stream already allocated");
      rccEnableSPI3(FALSE);
    }
#endif
  }
  /* Configuration.*/
}

/**
 * @brief   Deactivates the I2S peripheral.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @notapi
 */
void i2s_lld_stop(I2SDriver *i2sp) {

  if (i2sp->state == I2S_READY) {
    /* Clock deactivation.*/

  }
}

/**
 * @brief   Starts a I2S data exchange.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @notapi
 */
void i2s_lld_start_exchange(I2SDriver *i2sp) {

}

/**
 * @brief   Starts a I2S data exchange in continuous mode.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @notapi
 */
void i2s_lld_start_exchange_continuous(I2SDriver *i2sp) {

}

/**
 * @brief   Stops the ongoing data exchange.
 * @details The ongoing data exchange, if any, is stopped, if the driver
 *          was not active the function does nothing.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 *
 * @notapi
 */
void i2s_lld_stop_exchange(I2SDriver *i2sp) {

}

#endif /* HAL_USE_I2S */

/** @} */
