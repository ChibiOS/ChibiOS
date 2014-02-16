/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    i2s_lld.c
 * @brief   I2S Driver subsystem low level driver source template.
 *
 * @addtogroup I2S
 * @{
 */

#include "hal.h"

#if HAL_USE_I2S || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief I2S2 driver identifier.*/
#if STM32_I2S_USE_SPI2 || defined(__DOXYGEN__)
I2SDriver I2SD2;
#endif

/** @brief I2S3 driver identifier.*/
#if STM32_I2S_USE_SPI3 || defined(__DOXYGEN__)
I2SDriver I2SD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared end-of-rx service routine.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void i2s_lld_serve_rx_interrupt(I2SDriver *i2sp, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(STM32_I2S_DMA_ERROR_HOOK)
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    STM32_I2S_DMA_ERROR_HOOK(i2sp);
  }
#else
  (void)flags;
#endif

  /* Stop everything.*/
  dmaStreamDisable(i2sp->dmatx);
  dmaStreamDisable(i2sp->dmarx);

  /* Portable I2S ISR code defined in the high level driver, note, it is
     a macro.*/
  _i2s_isr_code(i2sp);
}

/**
 * @brief   Shared end-of-tx service routine.
 *
 * @param[in] i2sp      pointer to the @p I2SDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void i2s_lld_serve_tx_interrupt(I2SDriver *i2sp, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(STM32_I2S_DMA_ERROR_HOOK)
  (void)i2sp;
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    STM32_I2S_DMA_ERROR_HOOK(i2sp);
  }
#else
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
 * @brief   Low level I2S driver initialization.
 *
 * @notapi
 */
void i2s_lld_init(void) {

#if STM32_I2S_USE_SPI2
  i2sObjectInit(&I2SD2);
  I2SD2.spi = SPI2;
#endif

#if STM32_I2S_USE_SPI3
  i2sObjectInit(&I2SD3);
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
#if STM32_I2S_USE_SPI2
    if (&I2SD2 == i2sp) {
      bool b;
      b = dmaStreamAllocate(i2sp->dmarx,
                            STM32_I2S_SPI2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");
      b = dmaStreamAllocate(i2sp->dmatx,
                            STM32_I2S_SPI2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");
      rccEnableSPI2(FALSE);
    }
#endif
#if STM32_I2S_USE_SPI3
    if (&I2SD3 == i2sp) {
      bool b;
      b = dmaStreamAllocate(i2sp->dmarx,
                            STM32_I2S_SPI3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");
      b = dmaStreamAllocate(i2sp->dmatx,
                            STM32_I2S_SPI3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");
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

  /* If in ready state then disables the SPI clock.*/
  if (i2sp->state == I2S_READY) {

    /* SPI disable.*/
    i2sp->spi->CR1 = 0;
    i2sp->spi->CR2 = 0;
    dmaStreamRelease(i2sp->dmarx);
    dmaStreamRelease(i2sp->dmatx);

#if STM32_I2S_USE_SPI2
    if (&I2SD2 == i2sp)
      rccDisableSPI2(FALSE);
#endif
#if STM32_I2S_USE_SPI3
    if (&I2SD3 == i2sp)
      rccDisableSPI3(FALSE);
#endif
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
