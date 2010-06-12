/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file STM32/adc_lld.c
 * @brief STM32 ADC subsystem low level driver source.
 * @addtogroup STM32_ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if USE_STM32_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD1;
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

#if USE_STM32_ADC1 || defined(__DOXYGEN__)
/**
 * @brief ADC1 DMA interrupt handler (channel 1).
 */
CH_IRQ_HANDLER(Vector6C) {
  uint32_t isr;

  CH_IRQ_PROLOGUE();

  isr = DMA1->ISR;
  DMA1->IFCR |= DMA_IFCR_CGIF1  | DMA_IFCR_CTCIF1 |
                DMA_IFCR_CHTIF1 | DMA_IFCR_CTEIF1;
  if ((isr & DMA_ISR_HTIF1) != 0) {
    /* Half transfer processing.*/
    if (ADCD1.ad_callback != NULL) {
      /* Invokes the callback passing the 1st half of the buffer.*/
      ADCD1.ad_callback(ADCD1.ad_samples, ADCD1.ad_depth / 2);
    }
  }
  if ((isr & DMA_ISR_TCIF1) != 0) {
    /* Transfer complete processing.*/
    if (!ADCD1.ad_grpp->acg_circular) {
      /* End conversion.*/
      adc_lld_stop_conversion(&ADCD1);
      ADCD1.ad_grpp  = NULL;
      ADCD1.ad_state = ADC_COMPLETE;
      chSysLockFromIsr();
      chSemResetI(&ADCD1.ad_sem, 0);
      chSysUnlockFromIsr();
    }
    /* Callback handling.*/
    if (ADCD1.ad_callback != NULL) {
      if (ADCD1.ad_depth > 1) {
        /* Invokes the callback passing the 2nd half of the buffer.*/
        size_t half = ADCD1.ad_depth / 2;
        ADCD1.ad_callback(ADCD1.ad_samples + half, half);
      }
      else {
        /* Invokes the callback passing the whole buffer.*/
        ADCD1.ad_callback(ADCD1.ad_samples, ADCD1.ad_depth);
      }
    }
  }
  if ((isr & DMA_ISR_TEIF1) != 0) {
    /* DMA error processing.*/
    STM32_ADC1_DMA_ERROR_HOOK();
  }

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level ADC driver initialization.
 */
void adc_lld_init(void) {

#if USE_STM32_ADC1
  /* ADC reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB2RSTR = RCC_APB2RSTR_ADC1RST;
  RCC->APB2RSTR = 0;

  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.ad_adc = ADC1;
  ADCD1.ad_dma = DMA1_Channel1;
  ADCD1.ad_dmaprio = STM32_ADC1_DMA_PRIORITY << 12;

  /* Temporary activation.*/
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  ADC1->CR1 = 0;
  ADC1->CR2 = ADC_CR2_ADON;

  /* Reset calibration just to be safe.*/
  ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_RSTCAL;
  while ((ADC1->CR2 & ADC_CR2_RSTCAL) != 0)
    ;

  /* Calibration.*/
  ADC1->CR2 = ADC_CR2_ADON | ADC_CR2_CAL;
  while ((ADC1->CR2 & ADC_CR2_CAL) != 0)
    ;

  /* Return the ADC in low power mode.*/
  ADC1->CR2 = 0;
  RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;
#endif
}

/**
 * @brief Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_start(ADCDriver *adcp) {

  /* If in stopped state then enables the ADC and DMA clocks.*/
  if (adcp->ad_state == ADC_STOP) {
#if USE_STM32_ADC1
    if (&ADCD1 == adcp) {
      dmaEnable(DMA1_ID);   /* NOTE: Must be enabled before the IRQs.*/
      NVICEnableVector(DMA1_Channel1_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_ADC1_IRQ_PRIORITY));
      DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
      RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    }
#endif

    /* ADC setup, the calibration procedure has already been performed
       during initialization.*/
    adcp->ad_adc->CR1 = ADC_CR1_SCAN;
    adcp->ad_adc->CR2 = 0;
  }
}

/**
 * @brief Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop(ADCDriver *adcp) {

  /* If in ready state then disables the ADC clock.*/
  if (adcp->ad_state == ADC_READY) {
#if USE_STM32_ADC1
    if (&ADCD1 == adcp) {
      ADC1->CR1 = 0;
      ADC1->CR2 = 0;
      NVICDisableVector(DMA1_Channel1_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;
    }
#endif
  }
}

/**
 * @brief Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_start_conversion(ADCDriver *adcp) {
  uint32_t ccr, n;
  const ADCConversionGroup *grpp = adcp->ad_grpp;

  /* DMA setup.*/
  adcp->ad_dma->CMAR  = (uint32_t)adcp->ad_samples;
  ccr = adcp->ad_dmaprio | DMA_CCR1_EN   | DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0 |
                           DMA_CCR1_MINC | DMA_CCR1_TCIE    | DMA_CCR1_TEIE;
  if (grpp->acg_circular)
    ccr |= DMA_CCR1_CIRC;
  if (adcp->ad_depth > 1) {
    /* If the buffer depth is greater than one then the half transfer interrupt
       interrupt is enabled in order to allows streaming processing.*/
    ccr |= DMA_CCR1_HTIE;
    n = (uint32_t)grpp->acg_num_channels * (uint32_t)adcp->ad_depth;
  }
  else
    n = (uint32_t)grpp->acg_num_channels;
  adcp->ad_dma->CNDTR = n;
  adcp->ad_dma->CCR = ccr;

  /* ADC setup.*/
  adcp->ad_adc->SMPR1 = grpp->acg_smpr1;
  adcp->ad_adc->SMPR2 = grpp->acg_smpr2;
  adcp->ad_adc->SQR1  = grpp->acg_sqr1;
  adcp->ad_adc->SQR2  = grpp->acg_sqr2;
  adcp->ad_adc->SQR3  = grpp->acg_sqr3;
  adcp->ad_adc->CR1   = grpp->acg_cr1 | ADC_CR1_SCAN;
  adcp->ad_adc->CR2   = grpp->acg_cr2 | ADC_CR2_DMA | ADC_CR2_ADON;

  /* ADC start.*/
  adcp->ad_adc->CR2  |= ADC_CR2_SWSTART | ADC_CR2_EXTTRIG;
}

/**
 * @brief Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  adcp->ad_dma->CCR = 0;
  adcp->ad_adc->CR2 = 0;
}

#endif /* CH_HAL_USE_ADC */

/** @} */
