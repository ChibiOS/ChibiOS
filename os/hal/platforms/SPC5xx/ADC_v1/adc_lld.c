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
 * @file    SPC5xx/ADC_v1/adc_lld.c
 * @brief   ADC Driver subsystem low level driver source.
 *
 * @addtogroup ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/* Some forward declarations.*/
static void adc_serve_adc_irq(edma_channel_t channel, void *p);
static void adc_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr);

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ADC1 driver identifier.
 */
#if SPC5_ADC_USE_ADC0 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

#if SPC5_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if SPC5_ADC_USE_ADC0 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for ADC0.
 */
static const edma_channel_config_t adc_adc0_dma_config = {
  SPC5_ADC_ADC0_DMA_CH_ID,
  SPC5_ADC0_DMA_DEV_ID,
  SPC5_ADC_ADC0_DMA_IRQ_PRIO,
  adc_serve_adc_irq, adc_serve_dma_error_irq, &ADCD1
};
#endif /* SPC5_ADC_USE_ADC0 */

#if SPC5_ADC_USE_ADC1 || defined(__DOXYGEN__)
/**
 * @brief   DMA configuration for ADC1.
 */
static const edma_channel_config_t adc_adc1_dma_config = {
  SPC5_ADC_ADC1_DMA_CH_ID,
  SPC5_ADC1_DMA_DEV_ID,
  SPC5_ADC_ADC1_DMA_IRQ_PRIO,
  adc_serve_adc_irq, adc_serve_dma_error_irq, &ADCD2
};
#endif /* SPC5_ADC_USE_ADC1 */

/*===========================================================================*/
/* Driver local functions and macros.                                        */
/*===========================================================================*/

/**
 * @brief   Unsigned two's complement.
 *
 * @param[in] n         the value to be complemented
 *
 * @notapi
 */
#define CPL2(n) ((~(uint32_t)(n)) + 1)

/**
 * @brief   Shared ISR for ADC events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 *
 * @notapi
 */
static void adc_serve_adc_irq(edma_channel_t channel, void *p) {
  ADCDriver *adcp = (ADCDriver *)p;
  edma_tcd_t *tcdp = edmaGetTCD(channel);

  if (adcp->grpp != NULL) {
    if ((tcdp->word[5] >> 16) != (tcdp->word[7] >> 16)) {
      /* Half transfer processing.*/
      _adc_isr_half_code(adcp);
    }
    else {
      /* Re-starting DMA channel if in circular mode.*/
      if (adcp->grpp->circular) {
        edmaChannelStart(adcp->adc_dma_channel);
      }

      /* Transfer complete processing.*/
      _adc_isr_full_code(adcp);
    }
  }
}

/**
 * @brief   Shared ISR for DMA error events.
 *
 * @param[in] channel   the channel number
 * @param[in] p         parameter for the registered function
 * @param[in] esr       content of the ESR register
 *
 * @notapi
 */
static void adc_serve_dma_error_irq(edma_channel_t channel,
                                    void *p,
                                    uint32_t esr) {
  ADCDriver *adcp = (ADCDriver *)p;

  (void)channel;
  (void)esr;

  _adc_isr_error_code(adcp, ADC_ERR_DMAFAILURE);
}

/**
 * @brief   ADC ISR service routine.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] isr       content of the ISR register
 */
static void adc_lld_serve_interrupt(ADCDriver *adcp, uint32_t isr) {

  /* It could be a spurious interrupt caused by overflows after DMA disabling,
     just ignore it in this case.*/
  if (adcp->grpp != NULL) {
    if (isr & ADC_ISR_AWD0_LT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD0_LT);
    }
    if (isr & ADC_ISR_AWD1_LT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD1_LT);
    }
    if (isr & ADC_ISR_AWD2_LT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD2_LT);
    }
    if (isr & ADC_ISR_AWD3_LT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD3_LT);
    }
    if (isr & ADC_ISR_AWD0_HT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD0_HT);
    }
    if (isr & ADC_ISR_AWD1_HT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD1_HT);
    }
    if (isr & ADC_ISR_AWD2_HT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD2_HT);
    }
    if (isr & ADC_ISR_AWD3_HT) {
      /* Analog watchdog error.*/
      _adc_isr_error_code(adcp, ADC_ERR_AWD3_HT);
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_ADC_USE_ADC0
#if !defined(SPC5_ADC0_WD_HANDLER)
#error "SPC5_ADC0_WD_HANDLER not defined"
#endif
/**
 * @brief   ADC0 Watch Dog interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SPC5_ADC0_WD_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr = ADCD1.adc_tagp->WTISR.R;
  ADCD1.adc_tagp->WTISR.R = isr;
  adc_lld_serve_interrupt(&ADCD1, isr);

  OSAL_IRQ_EPILOGUE();
}
#endif /* SPC5_ADC_USE_ADC0 */

#if SPC5_ADC_USE_ADC1
#if !defined(SPC5_ADC1_WD_HANDLER)
#error "SPC5_ADC1_WD_HANDLER not defined"
#endif
/**
 * @brief   ADC1 Watch Dog interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SPC5_ADC1_WD_HANDLER) {
  uint32_t isr;

  OSAL_IRQ_PROLOGUE();

  isr = ADCD2.adc_tagp->WTISR.R;
  ADCD2.adc_tagp->WTISR.R = isr;
  adc_lld_serve_interrupt(&ADCD2, isr);

  OSAL_IRQ_EPILOGUE();
}
#endif /* SPC5_ADC_USE_ADC1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if SPC5_ADC_USE_ADC0
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.adc_dma_channel = EDMA_ERROR;
  ADCD1.adc_tagp = &SPC5_ADC_0;
#endif /* SPC5_ADC_USE_ADC0 */

#if SPC5_ADC_USE_ADC1
  /* Driver initialization.*/
  adcObjectInit(&ADCD2);
  ADCD2.adc_dma_channel = EDMA_ERROR;
  ADCD2.adc_tagp = &SPC5_ADC_1;
#endif /* SPC5_ADC_USE_ADC1 */

#if SPC5_ADC_USE_ADC0
  INTC.PSR[SPC5_ADC0_EOC_NUMBER].R = SPC5_ADC_ADC0_EOC_PRIORITY;
  INTC.PSR[SPC5_ADC0_ER_NUMBER].R = SPC5_ADC_ADC0_ER_PRIORITY;
  INTC.PSR[SPC5_ADC0_WD_NUMBER].R = SPC5_ADC_ADC0_WD_PRIORITY;

  /* Sets peripheral clock.*/
  halSPCSetPeripheralClockMode(SPC5_ADC0_PCTL, SPC5_ADC_ADC0_START_PCTL);
  ADCD1.adc_tagp->MCR.B.ADCLKSEL = SPC5_ADC_ADC0_CLK_FREQUENCY;
  halSPCSetPeripheralClockMode(SPC5_ADC0_PCTL, SPC5_ADC_ADC0_STOP_PCTL);
#endif

#if SPC5_ADC_USE_ADC1
  INTC.PSR[SPC5_ADC1_EOC_NUMBER].R = SPC5_ADC_ADC1_EOC_PRIORITY;
  INTC.PSR[SPC5_ADC1_ER_NUMBER].R = SPC5_ADC_ADC1_ER_PRIORITY;
  INTC.PSR[SPC5_ADC1_WD_NUMBER].R = SPC5_ADC_ADC1_WD_PRIORITY;

  /* Sets peripheral clock.*/
  halSPCSetPeripheralClockMode(SPC5_ADC1_PCTL, SPC5_ADC_ADC1_START_PCTL);
  ADCD2.adc_tagp->MCR.B.ADCLKSEL = SPC5_ADC_ADC1_CLK_FREQUENCY;
  halSPCSetPeripheralClockMode(SPC5_ADC1_PCTL, SPC5_ADC_ADC1_STOP_PCTL);
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

  if (adcp->state == ADC_STOP) {
    /* Enables the peripheral.*/
#if SPC5_ADC_USE_ADC0
    if (&ADCD1 == adcp) {
      adcp->adc_dma_channel = edmaChannelAllocate(&adc_adc0_dma_config);
      halSPCSetPeripheralClockMode(SPC5_ADC0_PCTL,
                                   SPC5_ADC_ADC0_START_PCTL);
    }
#endif /* SPC5_ADC_USE_ADC0 */

#if SPC5_ADC_USE_ADC1
    if (&ADCD2 == adcp) {
      adcp->adc_dma_channel = edmaChannelAllocate(&adc_adc1_dma_config);
      halSPCSetPeripheralClockMode(SPC5_ADC1_PCTL,
                                   SPC5_ADC_ADC1_START_PCTL);
    }
#endif /* SPC5_ADC_USE_ADC1 */

    osalDbgAssert((adcp->adc_dma_channel != EDMA_ERROR),
                  "adc_lld_start(), #1", "DMA channel cannot be allocated");

    /* Sets ADC Normal Mode.*/
    adcp->adc_tagp->MCR.B.PWDN = 0;

    /* Sets MCR Register.*/
    adcp->adc_tagp->MCR.R = ADC_MCR_OWREN | ADC_MCR_MODE;
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

  if (adcp->state == ADC_READY) {
    /* Resets the peripheral.*/

    /* Releases the allocated DMA channel.*/
    edmaChannelRelease(adcp->adc_dma_channel);

    /* Clears thresholds’ values and deactive watchdog threshold interrupts.*/
#if SPC5_ADC_NTRESHOLD == 4
    adcp->adc_tagp->TRC[0].R = 0;
    adcp->adc_tagp->TRC[1].R = 0;
    adcp->adc_tagp->TRC[2].R = 0;
    adcp->adc_tagp->TRC[3].R = 0;
    adcp->adc_tagp->THRHLR[0].R = 0;
    adcp->adc_tagp->THRHLR[1].R = 0;
    adcp->adc_tagp->THRHLR[2].R = 0;
    adcp->adc_tagp->THRHLR[3].R = 0;
    adcp->adc_tagp->WTIMR.R = 0;

    /* Disables the watchdog interrupts if any.*/
    adcp->adc_tagp->CIMR[0].R = 0;

    /* Clears watchdog interrupts if any.*/
    adcp->adc_tagp->WTISR.R = 0xFFFF;
#elif SPC5_ADC_NTRESHOLD == 16
    adcp->adc_tagp->THRHLR[0].R = 0;
    adcp->adc_tagp->THRHLR[1].R = 0;
    adcp->adc_tagp->THRHLR[2].R = 0;
    adcp->adc_tagp->THRHLR[3].R = 0;
    adcp->adc_tagp->THRHLR_2[0].R = 0;
    adcp->adc_tagp->THRHLR_2[1].R = 0;
    adcp->adc_tagp->THRHLR_2[2].R = 0;
    adcp->adc_tagp->THRHLR_2[3].R = 0;
    adcp->adc_tagp->THRHLR_2[4].R = 0;
    adcp->adc_tagp->THRHLR_2[5].R = 0;
    adcp->adc_tagp->THRHLR_2[6].R = 0;
    adcp->adc_tagp->THRHLR_2[7].R = 0;
    adcp->adc_tagp->THRHLR_2[8].R = 0;
    adcp->adc_tagp->THRHLR_2[9].R = 0;
    adcp->adc_tagp->THRHLR_2[10].R = 0;
    adcp->adc_tagp->THRHLR_2[11].R = 0;
    adcp->adc_tagp->CWSEL[0].R = 0;
    adcp->adc_tagp->CWSEL[1].R = 0;
    adcp->adc_tagp->CWENR[0].R = 0;
    adcp->adc_tagp->AWORR[0].R = 0;
    adcp->adc_tagp->WTIMR.R = 0;

    /* Disables the watchdog interrupts if any.*/
    adcp->adc_tagp->CIMR[0].R = 0;

    /* Clears watchdog interrupts if any.*/
    adcp->adc_tagp->WTISR.R = 0xFFFFFFFF;
#endif

    /* Deactive ADC channels and the ADC DMA channels.*/
    adcp->adc_tagp->NCMR[0].R = 0;
    adcp->adc_tagp->DMAR[0].R = 0;

    /* Puts the ADC Peripheral in Power-Down Mode.*/
    adcp->adc_tagp->MCR.B.PWDN = 1U;

    /* Disables the peripheral.*/
#if SPC5_ADC_USE_ADC0
    if (&ADCD1 == adcp) {
      halSPCSetPeripheralClockMode(SPC5_ADC0_PCTL,
                                   SPC5_ADC_ADC0_STOP_PCTL);
    }
#endif

#if SPC5_ADC_USE_ADC1
    if (&ADCD2 == adcp) {
      halSPCSetPeripheralClockMode(SPC5_ADC1_PCTL,
                                   SPC5_ADC_ADC1_STOP_PCTL);
    }
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
  uint32_t ch_mask;
  uint8_t i;

  /* Setting up DMA TCD parameters.*/
  edmaChannelSetup(adcp->adc_dma_channel,                                   /* channel.                 */
                   ((uint8_t *)&adcp->adc_tagp->CDR[adcp->grpp->init_channel].R) + 2,   /* src.                     */
                   adcp->samples,                                           /* dst.                     */
                   4,                                                       /* soff, advance by four.   */
                   2,                                                       /* doff, advance by two.    */
                   1,                                                       /* ssize, 16 bits transfers.*/
                   1,                                                       /* dsize, 16 bits transfers.*/
                   2,                                                       /* nbytes, always two.      */
                   (uint32_t)adcp->grpp->num_channels *
                   (uint32_t)adcp->depth,                                   /* iter.                    */
                   CPL2((uint32_t)adcp->grpp->num_channels * 4),            /* slast.  					*/
                   CPL2((uint32_t)adcp->grpp->num_channels *
                        (uint32_t)adcp->depth *
                        sizeof(adcsample_t)),                               /* dlast.                   */
                   EDMA_TCD_MODE_DREQ | EDMA_TCD_MODE_INT_END |
                   ((adcp->depth > 1) ? EDMA_TCD_MODE_INT_HALF: 0));        /* mode.					*/

  /* Active DMA.*/
  adcp->adc_tagp->DMAE.R = ADC_DMAE_DMAEN;

  /* TODO: make the number of WD registers a parameter in the registry, modify
     the configuration structure.*/
  /* Sets thresholds’ values and active watchdog threshold interrupts if any.*/
#if SPC5_ADC_NTRESHOLD == 4
  for (i = 0; i < SPC5_ADC_NTRESHOLD; i++) {
    switch (adcp->grpp->thresholds[i].threshold_mode){
    case ADC_THRHLR_DISABLED:
      break;
    case ADC_THRHLR_HIGHER:
      /* Sets threshold registers.*/
      adcp->adc_tagp->TRC[i].R = (1U << 15) | adcp->grpp->thresholds[i].adc_ch;
      adcp->adc_tagp->THRHLR[i].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;

      /* Active interrupts.*/
      adcp->adc_tagp->WTIMR.R = 1U << (4U + i);
      adcp->adc_tagp->CIMR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      break;
    case ADC_THRHLR_LOWER:
      /* Sets threshold registers.*/
      adcp->adc_tagp->TRC[i].R = (1U << 15) | adcp->grpp->thresholds[i].adc_ch;
      adcp->adc_tagp->THRHLR[i].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;

      /* Active interrupts.*/
      adcp->adc_tagp->WTIMR.R = 1U << i;
      adcp->adc_tagp->CIMR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      break;
    case ADC_THRHLR_BOTH_HL:
      /* Sets threshold registers.*/
      adcp->adc_tagp->TRC[i].R = (1U << 15) | adcp->grpp->thresholds[i].adc_ch;
      adcp->adc_tagp->THRHLR[i].B.THRL = adcp->grpp->thresholds[i].low_threshold_value;
      adcp->adc_tagp->THRHLR[i].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;

      /* Active interrupts.*/
      adcp->adc_tagp->WTIMR.R = (1U << i) | (1U << (4U + i));
      adcp->adc_tagp->CIMR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      break;
    }
  }
#elif SPC5_ADC_NTRESHOLD == 16
  for (i = 0; i < SPC5_ADC_NTRESHOLD; i++) {
    switch (adcp->grpp->thresholds[i].threshold_mode){
    case ADC_THRHLR_DISABLED:
      break;
    case ADC_THRHLR_HIGHER:
      /* Sets threshold registers.*/
      if (adcp->grpp->thresholds[i].adc_ch > 7U) {
        adcp->adc_tagp->CWSEL[1].R = i << ((adcp->grpp->thresholds[i].adc_ch - 8U) * 4U);
      } else {
        adcp->adc_tagp->CWSEL[0].R = i << (adcp->grpp->thresholds[i].adc_ch * 4U);
      }

      adcp->adc_tagp->CWENR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      if (i > 4U) {
        adcp->adc_tagp->THRHLR_2[i - 4U].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;
      } else {
        adcp->adc_tagp->THRHLR[i].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;
      }

      /* Active interrupts.*/
      adcp->adc_tagp->WTIMR.R = 1U << (1U + i * 2U);
      adcp->adc_tagp->CIMR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      break;
    case ADC_THRHLR_LOWER:
      /* Sets threshold registers.*/
      if (adcp->grpp->thresholds[i].adc_ch > 7U) {
        adcp->adc_tagp->CWSEL[1].R = i << ((adcp->grpp->thresholds[i].adc_ch - 8U) * 4U);
      } else {
        adcp->adc_tagp->CWSEL[0].R = i << (adcp->grpp->thresholds[i].adc_ch * 4U);
      }
      adcp->adc_tagp->CWENR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      if (i > 4U) {
        adcp->adc_tagp->THRHLR_2[i - 4U].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;
      } else {
        adcp->adc_tagp->THRHLR[i].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;
      }

      /* Active interrupts.*/
      adcp->adc_tagp->WTIMR.R = 1U << (i * 2U);
      adcp->adc_tagp->CIMR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      break;
    case ADC_THRHLR_BOTH_HL:
      /* Sets threshold registers.*/
      if (adcp->grpp->thresholds[i].adc_ch > 7U) {
        adcp->adc_tagp->CWSEL[1].R = i << ((adcp->grpp->thresholds[i].adc_ch - 8U) * 4U);
      } else {
        adcp->adc_tagp->CWSEL[0].R = i << (adcp->grpp->thresholds[i].adc_ch * 4U);
      }
      adcp->adc_tagp->CWENR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      if (i > 4U) {
        adcp->adc_tagp->THRHLR_2[i - 4U].B.THRL = adcp->grpp->thresholds[i].low_threshold_value;
        adcp->adc_tagp->THRHLR_2[i - 4U].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;
      } else {
        adcp->adc_tagp->THRHLR[i].B.THRL = adcp->grpp->thresholds[i].low_threshold_value;
        adcp->adc_tagp->THRHLR[i].B.THRH = adcp->grpp->thresholds[i].high_threshold_value;
      }

      /* Active interrupts.*/
      adcp->adc_tagp->WTIMR.R = (1U << (1U + i * 2U)) | (1U << (i * 2U));
      adcp->adc_tagp->CIMR[0].R = 1U << adcp->grpp->thresholds[i].adc_ch;
      break;
    }
  }
#endif

  /* Active ADC channels for the conversion and sets the ADC DMA channels.*/
  ch_mask = ((1 << adcp->grpp->num_channels) - 1) << adcp->grpp->init_channel;
  adcp->adc_tagp->NCMR[0].R = ch_mask;
  adcp->adc_tagp->DMAR[0].R = ch_mask;

  /* Sets ADC conversion timing register.*/
  adcp->adc_tagp->CTR[0].R = adcp->grpp->ctr;

  /* Starting DMA channels.*/
  edmaChannelStart(adcp->adc_dma_channel);

  /* Starts conversion.*/
  adcp->adc_tagp->MCR.B.NSTART = 1U;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  /* Stop DMA channel.*/
  edmaChannelStop(adcp->adc_dma_channel);

  /* Stops conversion.*/
  adcp->adc_tagp->MCR.B.NSTART = 0;

  /* Disables Interrupts and DMA.*/
  adcp->adc_tagp->WTIMR.R = 0;
  adcp->adc_tagp->DMAE.R = ADC_DMAE_DMAEN;

  /* Clears all Interrupts.*/
  adcp->adc_tagp->WTISR.R = 0;
}

#endif /* HAL_USE_ADC */

/** @} */
