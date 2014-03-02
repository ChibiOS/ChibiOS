/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx DAC driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/dac_lld.c
 * @brief   LPC17xx DAC subsystem low level driver source.
 *
 * @addtogroup DAC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_DAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief CHN1 driver identifier.*/
DACDriver DACD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static lpc17xx_dma_lli_config_t lpc_dac_lli[2] __attribute__((aligned(0x10)));

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/


/**
 * @brief   DAC DMA interrupt function.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] flags     dma error flags
 */
static void dac_serve_dma_interrupt(DACDriver *dacp, uint32_t flags) {

  if ((flags & (1 << LPC17xx_DAC_DMA_CHANNEL)) != 0) {
    _dac_isr_error_code(dacp, flags);  /* DMA errors handling.*/
  }
  else {
    if (dacp->half_buffer == false) {
      _dac_isr_half_code(dacp);
      dacp->half_buffer = true;
    }
    else {
      _dac_isr_full_code(dacp);
      dacp->half_buffer = false;
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level DAC driver initialization.
 *
 * @notapi
 */
void dac_lld_init(void) {
  dacObjectInit(&DACD1);
}

/**
 * @brief   Configures and activates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start(DACDriver *dacp) {

  if (dacp->state == DAC_STOP) {

    /* Enable DAC */
    LPC_PINCON->PINSEL1 |= (2UL << 20);   /* Set AOUT P0.26 pin.*/

    LPC_DAC->CR = 0;
    LPC_DAC->CTRL = 0;
    LPC_DAC->CNTVAL = LPC17xx_PCLK/dacp->config->frequency;

    dmaChannelAllocate(LPC17xx_DAC_DMA_CHANNEL, \
                     (lpc17xx_dmaisr_t)dac_serve_dma_interrupt, \
                     (void *)dacp);
  }
}

/**
 * @brief   Deactivates the DAC peripheral.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_stop(DACDriver *dacp) {

  /* If in ready state then disables the DAC clock.*/
  if (dacp->state == DAC_READY) {

    /* DMA disable.*/
    dmaChannelDisable(LPC17xx_DAC_DMA_CHANNEL);
    dmaChannelRelease(LPC17xx_DAC_DMA_CHANNEL);

    /* Disable DAC */
    LPC_PINCON->PINSEL1 &= ~(2UL << 20);   /* Disable AOUT P0.26 pin.*/
  }
}

/**
 * @brief   Sends data over the DAC bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void dac_lld_start_conversion(DACDriver *dacp) {

  dacp->half_buffer  = false;
  uint32_t dma_ch_config;

  /* DMA configuration */
  lpc_dac_lli[0].srcaddr = (uint32_t) &dacp->samples[0];
  lpc_dac_lli[0].dstaddr = (uint32_t)&LPC_DAC->CR;
  lpc_dac_lli[0].lli = (uint32_t) &lpc_dac_lli[1];
  lpc_dac_lli[0].control =
    DMA_CTRL_TRANSFER_SIZE(dacp->depth/2)                   |
    DMA_CTRL_SRC_BSIZE_1                                    |
    DMA_CTRL_DST_BSIZE_1                                    |
    DMA_CTRL_SRC_WIDTH_WORD                                 |
    DMA_CTRL_DST_WIDTH_WORD                                 |
    DMA_CTRL_SRC_INC                                        |
    DMA_CTRL_DST_NOINC                                      |
    DMA_CTRL_PROT1_USER                                     |
    DMA_CTRL_PROT2_NONBUFF                                  |
    DMA_CTRL_PROT3_NONCACHE                                 |
    DMA_CTRL_INT;

  lpc_dac_lli[1].srcaddr = (uint32_t) &dacp->samples[dacp->depth/2];
  lpc_dac_lli[1].dstaddr = lpc_dac_lli[0].dstaddr;
  lpc_dac_lli[1].control = lpc_dac_lli[0].control;

  if (dacp->grpp->circular == true) {
    lpc_dac_lli[1].lli = (uint32_t) &lpc_dac_lli[0];
  }
  else {
    lpc_dac_lli[1].lli = 0;
  }

  dma_ch_config =
    DMA_CFG_CH_ENABLE                |
    DMA_CFG_DST_PERIPH(DMA_DAC)      |
    DMA_CFG_TTYPE_M2P                |
    DMA_CFG_IE                       |
    DMA_CFG_ITC;

  dmaChannelSrcAddr(LPC17xx_DAC_DMA_CHANNEL, lpc_dac_lli[0].srcaddr);
  dmaChannelDstAddr(LPC17xx_DAC_DMA_CHANNEL, lpc_dac_lli[0].dstaddr);
  dmaChannelLinkedList(LPC17xx_DAC_DMA_CHANNEL, lpc_dac_lli[0].lli);
  dmaChannelControl(LPC17xx_DAC_DMA_CHANNEL, lpc_dac_lli[0].control);
  dmaChannelConfig(LPC17xx_DAC_DMA_CHANNEL, dma_ch_config);

  LPC_DAC->CTRL = DACCTRL_DMA_ENA | DACCTRL_CNT_ENA | DACCTRL_DBLBUF_ENA;
}

void dac_lld_stop_conversion(DACDriver *dacp) {

  /* If in active state then disables the DAC.*/
  if (dacp->state == DAC_ACTIVE) {

    /* DMA disable.*/
    dmaChannelDisable(LPC17xx_DAC_DMA_CHANNEL);
  }
};

#endif /* HAL_USE_DAC */

/** @} */
