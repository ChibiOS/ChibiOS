/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC17xx ADC driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC17xx/adc_lld.c
 * @brief   LPC17xx ADC subsystem low level driver source.
 * @note    Values in samples buffer are from DR register.
 *          To get ADC values make conversion (DR >> 6) & 0x03FF.
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
ADCDriver ADCD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if LPC17xx_ADC_USE_DMA
/**
 * @brief   Common IRQ handler.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
static void adc_serve_dma_interrupt(ADCDriver *adcp, uint32_t flags) {
  (void) flags;

  adcp->num++;
  if (adcp->num == 1) {
    dmaChannelSrcAddr(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.srcaddr);
    dmaChannelDstAddr(LPC17xx_ADC_DMA_CHANNEL, \
                      &adcp->samples[adcp->nsamples/2]);
    dmaChannelControl(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.control);
    dmaChannelConfig(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.config);
    _adc_isr_half_code(adcp);

  }
  else {
    adcp->num = 0;
    if (adcp->grpp->circular == TRUE) {
      dmaChannelSrcAddr(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.srcaddr);
      dmaChannelDstAddr(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.dstaddr);
      dmaChannelControl(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.control);
      dmaChannelConfig(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.config);
     }
    _adc_isr_full_code(adcp);
  }
}
#endif
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   ADC interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(Vector98) {
  uint32_t status;
  uint32_t n;
  uint8_t i;

  CH_IRQ_PROLOGUE();
  status = LPC_ADC->STAT;

  n = ADCD1.num;

  /* Note, an overrun may occur only in burst mode, if one or more
     conversions was (were) lost. */
  if ((status & ADC0STAT_OVERRUN_MASK)) {
    if (ADCD1.grpp != NULL)
      _adc_isr_error_code(&ADCD1, ADC_ERR_OVERRUN);
  }
  else {

    status = status & ADC0STAT_DONE_MASK;
    for (i = 0; i < ADC_MAX_CHANNELS; i++) {
      if (status & (0x01 << i)) {
        ADCD1.samples[n] = LPC_ADC->DR[i];
        n++;
      }
    }

    if (n == (ADCD1.nsamples / 2)) {
      _adc_isr_half_code(&ADCD1);
    }

    if (n == ADCD1.nsamples) {
      n = 0;
      _adc_isr_full_code(&ADCD1);
    }
  }
  ADCD1.num = n;
  CH_IRQ_EPILOGUE();
}


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.adc = LPC_ADC;

#if LPC17xx_ADC_USE_DMA
  nvicDisableVector(ADC_IRQn);
#else
  nvicEnableVector(ADC_IRQn, CORTEX_PRIORITY_MASK(LPC17xx_ADC_IRQ_PRIORITY));
#endif
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start(ADCDriver *adcp) {

  /* If in stopped state then enables the ADC. */
  if (adcp->state == ADC_STOP) {
    LPC_SC->PCONP |= (1UL << 12);            /* Enable ADC power */

#if LPC17xx_ADC_USE_DMA
  dmaChannelAllocate(LPC17xx_ADC_DMA_CHANNEL, \
                     (lpc17xx_dmaisr_t)adc_serve_dma_interrupt, \
                     (void *)adcp);
#endif
  }
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcp) {

  /* If in ready state then disables the ADC clock.*/
  if (adcp->state == ADC_READY) {
    adcp->adc->CR = 0;                              /* Clear PDN bit */
    LPC_SC->PCONP &= ~(1UL << 12);                  /* Disable ADC power */

#if LPC17xx_ADC_USE_DMA
    dmaChannelRelease(LPC17xx_ADC_DMA_CHANNEL);
#endif
  }
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start_conversion(ADCDriver *adcp) {

  uint32_t dummy;
  uint32_t cr;
  uint8_t i;
#if LPC17xx_ADC_USE_DMA
  uint8_t ch;
#endif

  cr = adcp->grpp->cr0;
  adcp->num = 0;
  adcp->nsamples = adcp->depth * adcp->grpp->num_channels;

  for (i = 0; i < ADC_MAX_CHANNELS; i++) {
    dummy = adcp->adc->DR[i];  /* Clear all DONE and OVERRUN flags. */
  }

#if LPC17xx_ADC_USE_DMA
  ch = 0;
  for (i = 0; i < 8; i++) {
    if (cr & (1UL << i)) {
      ch = i;                  /* Get enabled channel number */
      break;
    }
  }

  /* DMA configuration */
  adcp->adc_dma_cfg.srcaddr = (uint32_t)&adcp->adc->DR[ch];
  adcp->adc_dma_cfg.dstaddr = (uint32_t)&adcp->samples[0];
  adcp->adc_dma_cfg.lli = 0;
  adcp->adc_dma_cfg.control =
    DMA_CTRL_TRANSFER_SIZE(adcp->nsamples/2)    |
    DMA_CTRL_SRC_BSIZE_1                        |
    DMA_CTRL_DST_BSIZE_1                        |
    DMA_CTRL_SRC_WIDTH_WORD                     |
    DMA_CTRL_DST_WIDTH_WORD                     |
    DMA_CTRL_SRC_NOINC                          |
    DMA_CTRL_DST_INC                            |
    DMA_CTRL_PROT1_USER                         |
    DMA_CTRL_PROT2_NONBUFF                      |
    DMA_CTRL_PROT3_NONCACHE                     |
    DMA_CTRL_INT;

  adcp->adc_dma_cfg.config =
    DMA_CFG_CH_ENABLE           |
    DMA_CFG_SRC_PERIPH(DMA_ADC) |
    DMA_CFG_TTYPE_P2M           |
    DMA_CFG_IE                  |
    DMA_CFG_ITC;

  dmaChannelSrcAddr(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.srcaddr);
  dmaChannelDstAddr(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.dstaddr);
  dmaChannelControl(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.control);
  dmaChannelConfig(LPC17xx_ADC_DMA_CHANNEL, adcp->adc_dma_cfg.config);
#endif

  /* ADC configuration and conversion start. */
  adcp->adc->INTEN = adcp->grpp->inten;  /* Set ADC interrupt on selected channels */
  adcp->adc->CR = (cr & 0x0000FFFF) | ((LPC17xx_ADC_CLKDIV - 1) << 8) | AD0CR_PDN;
  adcp->adc->CR |= cr & 0xFFFF0000;

}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

#if LPC17xx_ADC_USE_DMA
  dmaChannelDisable(LPC17xx_ADC_DMA_CHANNEL);
#endif
  adcp->adc->CR &= ~(AD0CR_MODE_BURST | AD0CR_START_MASK);  /* Disable ADC conversion. */
  adcp->adc->INTEN = 0;                                     /* Mask ADC interrupts. */
}


#endif /* HAL_USE_ADC */

/** @} */
