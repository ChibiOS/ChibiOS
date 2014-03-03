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
 * @brief   STM32 I2S subsystem low level driver source.
 *
 * @addtogroup I2S
 * @{
 */

#include "hal.h"

#if HAL_USE_I2S || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define I2S2_RX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI2_RX_DMA_STREAM,                        \
                       STM32_SPI2_RX_DMA_CHN)

#define I2S2_TX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI2_TX_DMA_STREAM,                        \
                       STM32_SPI2_TX_DMA_CHN)

#define I2S3_RX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI3_RX_DMA_STREAM,                        \
                       STM32_SPI3_RX_DMA_CHN)

#define I2S3_TX_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI3_TX_DMA_STREAM,                        \
                       STM32_SPI3_TX_DMA_CHN)

/*
 * Static I2S settings for I2S2.
 */
#if !STM32_I2S_IS_MASTER(STM32_I2S_SPI2_MODE)
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 0
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_0
#endif
#else /* !STM32_I2S_IS_MASTER(STM32_I2S_SPI2_MODE) */
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_1
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 (SPI_I2SCFGR_I2SCFG_1 |         \
                                             SPI_I2SCFGR_I2SCFG_0)
#endif
#endif /* !STM32_I2S_IS_MASTER(STM32_I2S_SPI2_MODE) */

/*
 * Static I2S settings for I2S3.
 */
#if !STM32_I2S_IS_MASTER(STM32_I2S_SPI3_MODE)
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 0
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_0
#endif
#else /* !STM32_I2S_IS_MASTER(STM32_I2S_SPI3_MODE) */
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_1
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 (SPI_I2SCFGR_I2SCFG_1 |         \
                                             SPI_I2SCFGR_I2SCFG_0)
#endif
#endif /* !STM32_I2S_IS_MASTER(STM32_I2S_SP3_MODE) */

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

#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE) ||                            \
    STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE) || defined(__DOXYGEN__)
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
#endif

#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE) ||                            \
    STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE) || defined(__DOXYGEN__)
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
#endif

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
  I2SD2.spi       = SPI2;
  I2SD2.cfg       = STM32_I2S2_CFGR_CFG;
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
  I2SD2.dmarx     = STM32_DMA_STREAM(STM32_I2S_SPI2_RX_DMA_STREAM);
  I2SD2.rxdmamode = STM32_DMA_CR_CHSEL(I2S2_RX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI2_DMA_PRIORITY) |
                    STM32_DMA_CR_DIR_P2M |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD2.dmarx     = NULL;
  I2SD2.rxdmamode = 0;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
  I2SD2.dmatx     = STM32_DMA_STREAM(STM32_I2S_SPI2_TX_DMA_STREAM);
  I2SD2.txdmamode = STM32_DMA_CR_CHSEL(I2S2_TX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI2_DMA_PRIORITY) |
                    STM32_DMA_CR_DIR_M2P |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD2.dmatx     = NULL;
  I2SD2.txdmamode = 0;
#endif
#endif

#if STM32_I2S_USE_SPI3
  i2sObjectInit(&I2SD3);
  I2SD3.spi       = SPI3;
  I2SD3.cfg       = STM32_I2S3_CFGR_CFG;
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
  I2SD3.dmarx     = STM32_DMA_STREAM(STM32_I2S_SPI3_RX_DMA_STREAM);
  I2SD3.rxdmamode = STM32_DMA_CR_CHSEL(I2S3_RX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI3_DMA_PRIORITY) |
                    STM32_DMA_CR_DIR_P2M |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD3.dmarx     = NULL;
  I2SD3.rxdmamode = 0;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
  I2SD3.dmatx     = STM32_DMA_STREAM(STM32_I2S_SPI3_TX_DMA_STREAM);
  I2SD3.txdmamode = STM32_DMA_CR_CHSEL(I2S3_TX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI3_DMA_PRIORITY) |
                    STM32_DMA_CR_DIR_M2P |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD3.dmatx     = NULL;
  I2SD3.txdmamode = 0;
#endif
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
    uint32_t dmasize;

#if STM32_I2S_USE_SPI2
    if (&I2SD2 == i2sp) {
      bool b;

      /* Enabling I2S unit clock.*/
      rccEnableSPI2(FALSE);

#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
      b = dmaStreamAllocate(i2sp->dmarx,
                            STM32_I2S_SPI2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");

      /* CRs settings are done here because those never changes until
          the driver is stopped.*/
      i2sp->spi->CR1 = 0;
      i2sp->spi->CR2 = SPI_CR2_RXDMAEN;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
      b = dmaStreamAllocate(i2sp->dmatx,
                            STM32_I2S_SPI2_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");

      /* CRs settings are done here because those never changes until
          the driver is stopped.*/
      i2sp->spi->CR1 = 0;
      i2sp->spi->CR2 = SPI_CR2_TXDMAEN;
#endif
    }
#endif
#if STM32_I2S_USE_SPI3
    if (&I2SD3 == i2sp) {
      bool b;

      /* Enabling I2S unit clock.*/
      rccEnableSPI3(FALSE);

#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
      b = dmaStreamAllocate(i2sp->dmarx,
                            STM32_I2S_SPI3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");

      i2sp->spi->CR1 = 0;
      i2sp->spi->CR2 = SPI_CR2_RXDMAEN;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
      b = dmaStreamAllocate(i2sp->dmatx,
                            STM32_I2S_SPI3_IRQ_PRIORITY,
                            (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                            (void *)i2sp);
      osalDbgAssert(!b, "stream already allocated");

      i2sp->spi->CR1 = 0;
      i2sp->spi->CR2 = SPI_CR2_TXDMAEN;
#endif
    }
#endif

    /* DMA configuration.*/
    if ((i2sp->config->i2scfgr & (SPI_I2SCFGR_DATLEN |
                                  SPI_I2SCFGR_CHLEN)) == 0)
      dmasize = STM32_DMA_CR_PSIZE_HWORD | STM32_DMA_CR_MSIZE_HWORD;
    else
      dmasize = STM32_DMA_CR_PSIZE_WORD | STM32_DMA_CR_MSIZE_WORD;
    dmaStreamSetMode(i2sp->dmarx, i2sp->rxdmamode | dmasize);
    dmaStreamSetMode(i2sp->dmatx, i2sp->txdmamode | dmasize);
  }

  /* I2S configuration.*/
  i2sp->spi->I2SPR   = i2sp->config->i2spr;
  i2sp->spi->I2SCFGR = i2sp->config->i2scfgr | i2sp->cfg | SPI_I2SCFGR_I2SMOD;
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
    i2sp->spi->CR2 = 0;
    if (NULL != i2sp->dmarx)
      dmaStreamRelease(i2sp->dmarx);
    if (NULL != i2sp->dmatx)
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
