/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    SPIv2/hal_i2s_lld.c
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

#define I2S1_RX_DMA_CHANNEL                                                   \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI1_RX_DMA_STREAM,                          \
                       STM32_SPI1_RX_DMA_CHN)

#define I2S1_TX_DMA_CHANNEL                                                   \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI1_TX_DMA_STREAM,                          \
                       STM32_SPI1_TX_DMA_CHN)

#define I2S2_RX_DMA_CHANNEL                                                   \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI2_RX_DMA_STREAM,                          \
                       STM32_SPI2_RX_DMA_CHN)

#define I2S2_TX_DMA_CHANNEL                                                   \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI2_TX_DMA_STREAM,                          \
                       STM32_SPI2_TX_DMA_CHN)

#define I2S3_RX_DMA_CHANNEL                                                   \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI3_RX_DMA_STREAM,                          \
                       STM32_SPI3_RX_DMA_CHN)

#define I2S3_TX_DMA_CHANNEL                                                   \
  STM32_DMA_GETCHANNEL(STM32_I2S_SPI3_TX_DMA_STREAM,                          \
                       STM32_SPI3_TX_DMA_CHN)

#if !STM32_I2S_IS_MASTER(STM32_I2S_SPI1_MODE)
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
#define STM32_I2S1_CFGR_CFG                 0U
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE)
#define STM32_I2S1_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_0
#endif
#else
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
#define STM32_I2S1_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_1
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE)
#define STM32_I2S1_CFGR_CFG                 (SPI_I2SCFGR_I2SCFG_1 |           \
                                             SPI_I2SCFGR_I2SCFG_0)
#endif
#endif

#if !STM32_I2S_IS_MASTER(STM32_I2S_SPI2_MODE)
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 0U
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_0
#endif
#else
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_1
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
#define STM32_I2S2_CFGR_CFG                 (SPI_I2SCFGR_I2SCFG_1 |           \
                                             SPI_I2SCFGR_I2SCFG_0)
#endif
#endif

#if !STM32_I2S_IS_MASTER(STM32_I2S_SPI3_MODE)
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 0U
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_0
#endif
#else
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 SPI_I2SCFGR_I2SCFG_1
#endif
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
#define STM32_I2S3_CFGR_CFG                 (SPI_I2SCFGR_I2SCFG_1 |           \
                                             SPI_I2SCFGR_I2SCFG_0)
#endif
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_I2S_USE_SPI1 || defined(__DOXYGEN__)
I2SDriver I2SD1;
#endif

#if STM32_I2S_USE_SPI2 || defined(__DOXYGEN__)
I2SDriver I2SD2;
#endif

#if STM32_I2S_USE_SPI3 || defined(__DOXYGEN__)
I2SDriver I2SD3;
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void i2s_lld_disable_exchange(I2SDriver *i2sp) {

  if (i2sp->dmatx != NULL) {
    dmaStreamDisable(i2sp->dmatx);
  }
  if (i2sp->dmarx != NULL) {
    dmaStreamDisable(i2sp->dmarx);
  }

  i2sp->spi->I2SCFGR &= (uint16_t)~SPI_I2SCFGR_I2SE;
}

static const I2SConfig *i2s_validate_config(I2SDriver *i2sp,
                                            const I2SConfig *config) {
  if ((config == NULL) || (config->size == 0U)) {
    return NULL;
  }

#if STM32_I2S_USE_SPI1
  if (&I2SD1 == i2sp) {
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE)
    if (config->rx_buffer == NULL) {
      return NULL;
    }
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
    if (config->tx_buffer == NULL) {
      return NULL;
    }
#endif
  }
#endif

#if STM32_I2S_USE_SPI2
  if (&I2SD2 == i2sp) {
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
    if (config->rx_buffer == NULL) {
      return NULL;
    }
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
    if (config->tx_buffer == NULL) {
      return NULL;
    }
#endif
  }
#endif

#if STM32_I2S_USE_SPI3
  if (&I2SD3 == i2sp) {
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
    if (config->rx_buffer == NULL) {
      return NULL;
    }
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
    if (config->tx_buffer == NULL) {
      return NULL;
    }
#endif
  }
#endif

  return config;
}

#if (STM32_I2S_USE_SPI1 && STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE)) ||     \
    (STM32_I2S_USE_SPI2 && STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)) ||     \
    (STM32_I2S_USE_SPI3 && STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE))
static void i2s_lld_serve_rx_interrupt(I2SDriver *i2sp, uint32_t flags) {

  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0U) {
#if defined(STM32_I2S_DMA_ERROR_HOOK)
    STM32_I2S_DMA_ERROR_HOOK(i2sp);
#endif
    _i2s_isr_error_code(i2sp, I2S_ERR_DMAFAILURE);
  }
  else {
    if ((flags & STM32_DMA_ISR_HTIF) != 0U) {
      _i2s_isr_half_code(i2sp);
    }
    if ((flags & STM32_DMA_ISR_TCIF) != 0U) {
      _i2s_isr_full_code(i2sp);
    }
  }
}
#endif

#if (STM32_I2S_USE_SPI1 && STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)) ||     \
    (STM32_I2S_USE_SPI2 && STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)) ||     \
    (STM32_I2S_USE_SPI3 && STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE))
static void i2s_lld_serve_tx_interrupt(I2SDriver *i2sp, uint32_t flags) {

  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0U) {
#if defined(STM32_I2S_DMA_ERROR_HOOK)
    STM32_I2S_DMA_ERROR_HOOK(i2sp);
#endif
    _i2s_isr_error_code(i2sp, I2S_ERR_DMAFAILURE);
  }
  else {
    if ((flags & STM32_DMA_ISR_HTIF) != 0U) {
      _i2s_isr_half_code(i2sp);
    }
    if ((flags & STM32_DMA_ISR_TCIF) != 0U) {
      _i2s_isr_full_code(i2sp);
    }
  }
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void i2s_lld_init(void) {

#if STM32_I2S_USE_SPI1
  i2sObjectInit(&I2SD1);
  I2SD1.spi       = SPI1;
  I2SD1.cfg       = STM32_I2S1_CFGR_CFG;
  I2SD1.dmarx     = NULL;
  I2SD1.dmatx     = NULL;
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE)
  I2SD1.rxdmamode = STM32_DMA_CR_CHSEL(I2S1_RX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI1_DMA_PRIORITY) |
                    STM32_DMA_CR_PSIZE_HWORD |
                    STM32_DMA_CR_MSIZE_HWORD |
                    STM32_DMA_CR_DIR_P2M |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD1.rxdmamode = 0U;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
  I2SD1.txdmamode = STM32_DMA_CR_CHSEL(I2S1_TX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI1_DMA_PRIORITY) |
                    STM32_DMA_CR_PSIZE_HWORD |
                    STM32_DMA_CR_MSIZE_HWORD |
                    STM32_DMA_CR_DIR_M2P |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD1.txdmamode = 0U;
#endif
#endif

#if STM32_I2S_USE_SPI2
  i2sObjectInit(&I2SD2);
  I2SD2.spi       = SPI2;
  I2SD2.cfg       = STM32_I2S2_CFGR_CFG;
  I2SD2.dmarx     = NULL;
  I2SD2.dmatx     = NULL;
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
  I2SD2.rxdmamode = STM32_DMA_CR_CHSEL(I2S2_RX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI2_DMA_PRIORITY) |
                    STM32_DMA_CR_PSIZE_HWORD |
                    STM32_DMA_CR_MSIZE_HWORD |
                    STM32_DMA_CR_DIR_P2M |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD2.rxdmamode = 0U;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
  I2SD2.txdmamode = STM32_DMA_CR_CHSEL(I2S2_TX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI2_DMA_PRIORITY) |
                    STM32_DMA_CR_PSIZE_HWORD |
                    STM32_DMA_CR_MSIZE_HWORD |
                    STM32_DMA_CR_DIR_M2P |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD2.txdmamode = 0U;
#endif
#endif

#if STM32_I2S_USE_SPI3
  i2sObjectInit(&I2SD3);
  I2SD3.spi       = SPI3;
  I2SD3.cfg       = STM32_I2S3_CFGR_CFG;
  I2SD3.dmarx     = NULL;
  I2SD3.dmatx     = NULL;
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
  I2SD3.rxdmamode = STM32_DMA_CR_CHSEL(I2S3_RX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI3_DMA_PRIORITY) |
                    STM32_DMA_CR_PSIZE_HWORD |
                    STM32_DMA_CR_MSIZE_HWORD |
                    STM32_DMA_CR_DIR_P2M |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD3.rxdmamode = 0U;
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
  I2SD3.txdmamode = STM32_DMA_CR_CHSEL(I2S3_TX_DMA_CHANNEL) |
                    STM32_DMA_CR_PL(STM32_I2S_SPI3_DMA_PRIORITY) |
                    STM32_DMA_CR_PSIZE_HWORD |
                    STM32_DMA_CR_MSIZE_HWORD |
                    STM32_DMA_CR_DIR_M2P |
                    STM32_DMA_CR_MINC |
                    STM32_DMA_CR_CIRC |
                    STM32_DMA_CR_HTIE |
                    STM32_DMA_CR_TCIE |
                    STM32_DMA_CR_DMEIE |
                    STM32_DMA_CR_TEIE;
#else
  I2SD3.txdmamode = 0U;
#endif
#endif
}

const I2SConfig *i2s_lld_setcfg(I2SDriver *i2sp, const I2SConfig *config) {

  return i2s_validate_config(i2sp, config);
}

const I2SConfig *i2s_lld_selcfg(I2SDriver *i2sp, unsigned cfgnum) {
  (void)i2sp;
  (void)cfgnum;

  return NULL;
}

msg_t i2s_lld_start(I2SDriver *i2sp) {
  const I2SConfig *config = (const I2SConfig *)i2sp->config;

  {
    if (false) {
    }
#if STM32_I2S_USE_SPI1
    else if (&I2SD1 == i2sp) {
      rccEnableSPI1(true);
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI1_MODE)
      i2sp->dmarx = dmaStreamAlloc(STM32_I2S_SPI1_RX_DMA_STREAM,
                                   STM32_I2S_SPI1_IRQ_PRIORITY,
                                   (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                                   (void *)i2sp);
      if (i2sp->dmarx == NULL) {
        rccDisableSPI1();
        return HAL_RET_NO_RESOURCE;
      }
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(i2sp->dmarx, STM32_DMAMUX1_SPI1_RX);
#endif
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI1_MODE)
      i2sp->dmatx = dmaStreamAlloc(STM32_I2S_SPI1_TX_DMA_STREAM,
                                   STM32_I2S_SPI1_IRQ_PRIORITY,
                                   (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                                   (void *)i2sp);
      if (i2sp->dmatx == NULL) {
        if (i2sp->dmarx != NULL) {
          dmaStreamFree(i2sp->dmarx);
          i2sp->dmarx = NULL;
        }
        rccDisableSPI1();
        return HAL_RET_NO_RESOURCE;
      }
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(i2sp->dmatx, STM32_DMAMUX1_SPI1_TX);
#endif
#endif
    }
#endif
#if STM32_I2S_USE_SPI2
    else if (&I2SD2 == i2sp) {
      rccEnableSPI2(true);
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI2_MODE)
      i2sp->dmarx = dmaStreamAlloc(STM32_I2S_SPI2_RX_DMA_STREAM,
                                   STM32_I2S_SPI2_IRQ_PRIORITY,
                                   (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                                   (void *)i2sp);
      if (i2sp->dmarx == NULL) {
        rccDisableSPI2();
        return HAL_RET_NO_RESOURCE;
      }
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(i2sp->dmarx, STM32_DMAMUX1_SPI2_RX);
#endif
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI2_MODE)
      i2sp->dmatx = dmaStreamAlloc(STM32_I2S_SPI2_TX_DMA_STREAM,
                                   STM32_I2S_SPI2_IRQ_PRIORITY,
                                   (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                                   (void *)i2sp);
      if (i2sp->dmatx == NULL) {
        if (i2sp->dmarx != NULL) {
          dmaStreamFree(i2sp->dmarx);
          i2sp->dmarx = NULL;
        }
        rccDisableSPI2();
        return HAL_RET_NO_RESOURCE;
      }
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(i2sp->dmatx, STM32_DMAMUX1_SPI2_TX);
#endif
#endif
    }
#endif
#if STM32_I2S_USE_SPI3
    else if (&I2SD3 == i2sp) {
      rccEnableSPI3(true);
#if STM32_I2S_RX_ENABLED(STM32_I2S_SPI3_MODE)
      i2sp->dmarx = dmaStreamAlloc(STM32_I2S_SPI3_RX_DMA_STREAM,
                                   STM32_I2S_SPI3_IRQ_PRIORITY,
                                   (stm32_dmaisr_t)i2s_lld_serve_rx_interrupt,
                                   (void *)i2sp);
      if (i2sp->dmarx == NULL) {
        rccDisableSPI3();
        return HAL_RET_NO_RESOURCE;
      }
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(i2sp->dmarx, STM32_DMAMUX1_SPI3_RX);
#endif
#endif
#if STM32_I2S_TX_ENABLED(STM32_I2S_SPI3_MODE)
      i2sp->dmatx = dmaStreamAlloc(STM32_I2S_SPI3_TX_DMA_STREAM,
                                   STM32_I2S_SPI3_IRQ_PRIORITY,
                                   (stm32_dmaisr_t)i2s_lld_serve_tx_interrupt,
                                   (void *)i2sp);
      if (i2sp->dmatx == NULL) {
        if (i2sp->dmarx != NULL) {
          dmaStreamFree(i2sp->dmarx);
          i2sp->dmarx = NULL;
        }
        rccDisableSPI3();
        return HAL_RET_NO_RESOURCE;
      }
#if STM32_DMA_SUPPORTS_DMAMUX
      dmaSetRequestSource(i2sp->dmatx, STM32_DMAMUX1_SPI3_TX);
#endif
#endif
    }
#endif
    else {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  i2sp->spi->CR1 = 0U;
  i2sp->spi->CR2 = 0U;
  if (i2sp->dmarx != NULL) {
    i2sp->spi->CR2 |= SPI_CR2_RXDMAEN;
  }
  if (i2sp->dmatx != NULL) {
    i2sp->spi->CR2 |= SPI_CR2_TXDMAEN;
  }

  i2sp->spi->I2SPR   = (uint16_t)config->i2spr;
  i2sp->spi->I2SCFGR = (uint16_t)(config->i2scfgr |
                                  i2sp->cfg |
                                  SPI_I2SCFGR_I2SMOD);

  return HAL_RET_SUCCESS;
}

void i2s_lld_stop(I2SDriver *i2sp) {

  if (i2sp->state == HAL_DRV_STATE_STOPPING) {
    i2sp->spi->CR2 = 0U;
    i2sp->spi->I2SCFGR &= (uint16_t)~SPI_I2SCFGR_I2SE;

    if (i2sp->dmarx != NULL) {
      dmaStreamFree(i2sp->dmarx);
      i2sp->dmarx = NULL;
    }
    if (i2sp->dmatx != NULL) {
      dmaStreamFree(i2sp->dmatx);
      i2sp->dmatx = NULL;
    }

    if (false) {
    }
#if STM32_I2S_USE_SPI1
    else if (&I2SD1 == i2sp) {
      rccDisableSPI1();
    }
#endif
#if STM32_I2S_USE_SPI2
    else if (&I2SD2 == i2sp) {
      rccDisableSPI2();
    }
#endif
#if STM32_I2S_USE_SPI3
    else if (&I2SD3 == i2sp) {
      rccDisableSPI3();
    }
#endif
    else {
      osalDbgAssert(false, "invalid I2S instance");
    }
  }
}

msg_t i2s_lld_start_exchange(I2SDriver *i2sp) {
  const I2SConfig *config = (const I2SConfig *)i2sp->config;
  size_t size = config->size;

  if ((config->i2scfgr & SPI_I2SCFGR_DATLEN) != 0U) {
    size *= 2U;
  }

  if (i2sp->dmarx != NULL) {
    dmaStreamSetMode(i2sp->dmarx, i2sp->rxdmamode);
    dmaStreamSetPeripheral(i2sp->dmarx, &i2sp->spi->DR);
    dmaStreamSetMemory0(i2sp->dmarx, config->rx_buffer);
    dmaStreamSetTransactionSize(i2sp->dmarx, size);
    dmaStreamEnable(i2sp->dmarx);
  }

  if (i2sp->dmatx != NULL) {
    dmaStreamSetMode(i2sp->dmatx, i2sp->txdmamode);
    dmaStreamSetPeripheral(i2sp->dmatx, &i2sp->spi->DR);
    dmaStreamSetMemory0(i2sp->dmatx, (void *)config->tx_buffer);
    dmaStreamSetTransactionSize(i2sp->dmatx, size);
    dmaStreamEnable(i2sp->dmatx);
  }

  i2sp->spi->I2SCFGR |= SPI_I2SCFGR_I2SE;

  return HAL_RET_SUCCESS;
}

void i2s_lld_stop_exchange(I2SDriver *i2sp) {

  i2s_lld_disable_exchange(i2sp);
}

#endif /* HAL_USE_I2S */

/** @} */
