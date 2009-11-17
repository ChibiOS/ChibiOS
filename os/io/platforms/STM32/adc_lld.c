/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file STM32/adc_lld.c
 * @brief STM32 ADC subsystem low level driver source
 * @addtogroup STM32_ADC
 * @{
 */

#include <ch.h>
#include <adc.h>
#include <stm32_dma.h>
#include <nvic.h>

#if USE_STM32_ADC1 || defined(__DOXYGEN__)
/** @brief ADC1 driver identifier.*/
ADCDriver ADCD1;
#endif

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

#if USE_STM32_ADC1 || defined(__DOXYGEN__)
/**
 * @brief ADC1 DMA interrupt handler (channel 1).
 */
CH_IRQ_HANDLER(Vector6C) {

  CH_IRQ_PROLOGUE();

  if ((DMA1->ISR & DMA_ISR_TEIF1) != 0)
    chEvtBroadcastI(&ADCD1.ad_dmaerror);
  else {
    /* */
  }
  DMA1->IFCR |= DMA_IFCR_CGIF1  | DMA_IFCR_CTCIF1 |
                DMA_IFCR_CHTIF1 | DMA_IFCR_CTEIF1;

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level ADC driver initialization.
 */
void adc_lld_init(void) {

#if USE_STM32_ADC1
  adcObjectInit(&ADCD1);
  ADCD1.ad_adc = ADC1;
  ADCD1.ad_dma = DMA1_Channel1;
  ADCD1.ad_dmaprio = STM32_ADC1_DMA_PRIORITY << 12;
  chEvtInit(&ADCD1.ad_dmaerror);
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
      NVICEnableVector(DMA1_Channel1_IRQn, STM32_ADC1_IRQ_PRIORITY);
      dmaEnable(DMA1_ID);
      DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
/*      RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_ADCPRE) |
                  adcp->ad_config->ac_prescaler;*/
      RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    }
#endif

    /* ADC activation.*/
    adcp->ad_adc->CR1 = 0;
    adcp->ad_adc->CR2 = ADC_CR2_ADON;

    /* Reset calibration just to be safe.*/
    adcp->ad_adc->CR2 |= ADC_CR2_RSTCAL;
    while ((adcp->ad_adc->CR2 & ADC_CR2_RSTCAL) != 0)
      ;

    /* Calibration.*/
    adcp->ad_adc->CR2 |= ADC_CR2_CAL;
    while ((adcp->ad_adc->CR2 & ADC_CR2_CAL) != 0)
      ;
  }
}

/**
 * @brief Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop(ADCDriver *adcp) {

  /* If in ready state then disables the SPI clock.*/
  if (adcp->ad_state == ADC_READY) {
#if USE_STM32_ADC1
    if (&ADCD1 == adcp) {
      ADC1->CR1 = 0;
      ADC1->CR2 = 0;
      NVICDisableVector(DMA1_Channel1_IRQn);
      dmaDisable(DMA1_ID);
/*      RCC->CFGR &= ~RCC_CFGR_ADCPRE;*/
      RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;
    }
#endif
  }
}

/**
 * @brief Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] grpp      pointer to a @p ADCConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 *
 * @note The buffer is organized as a matrix of M*N elements where M is the
 *       channels number configured into the conversion group and N is the
 *       buffer depth. The samples are sequentially written into the buffer
 *       with no gaps.
 */
void adc_lld_start_conversion(ADCDriver *adcp,
                              ADCConversionGroup *grpp,
                              void *samples,
                              size_t depth) {

  /* DMA setup.*/
  adcp->ad_dma->CMAR  = (uint32_t)samples;
  if (depth > 1) {
    adcp->ad_dma->CNDTR = (uint32_t)grpp->acg_num_channels * (uint32_t)depth;
    adcp->ad_dma->CCR = adcp->ad_dmaprio |
                        DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0 | DMA_CCR1_MINC |
                        DMA_CCR1_TCIE | DMA_CCR1_TEIE | DMA_CCR1_HTIE;
  }
  else {
    adcp->ad_dma->CNDTR = (uint32_t)grpp->acg_num_channels;
    adcp->ad_dma->CCR = adcp->ad_dmaprio |
                        DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0 | DMA_CCR1_MINC |
                        DMA_CCR1_TCIE | DMA_CCR1_TEIE;
  }

  /* ADC setup.*/
  adcp->ad_adc->SMPR1 = grpp->acg_smpr1;
  adcp->ad_adc->SMPR2 = grpp->acg_smpr2;
  adcp->ad_adc->SQR1  = grpp->acg_sqr1;
  adcp->ad_adc->SQR2  = grpp->acg_sqr2;
  adcp->ad_adc->SQR3  = grpp->acg_sqr3;
  adcp->ad_adc->CR1   = grpp->acg_cr1 | ADC_CR1_SCAN;
  adcp->ad_adc->CR2   = grpp->acg_cr2 | ADC_CR2_DMA | ADC_CR2_ADON;
}

/**
 * @brief Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  adcp->ad_adc->CR2 &= ~(ADC_CR2_SWSTART | ADC_CR2_EXTTRIG);
  adcp->ad_dma->CCR = 0;
}

/** @} */
