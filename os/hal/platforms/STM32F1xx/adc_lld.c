/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM32F1xx/adc_lld.c
 * @brief   STM32F1xx ADC subsystem low level driver source.
 *
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief ADC1 driver identifier.*/
#if STM32_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared ADC DMA ISR service routine.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void adc_lld_serve_rx_interrupt(ADCDriver *adcp, uint32_t flags) {

  /* DMA errors handling.*/
  if ((flags & STM32_DMA_ISR_TEIF) != 0) {
    /* DMA, this could help only if the DMA tries to access an unmapped
       address space or violates alignment rules.*/
    _adc_isr_error_code(adcp, ADC_ERR_DMAFAILURE);
  }
  else {
    if ((flags & STM32_DMA_ISR_HTIF) != 0) {
      /* Half transfer processing.*/
      _adc_isr_half_code(adcp);
    }
    if ((flags & STM32_DMA_ISR_TCIF) != 0) {
      /* Transfer complete processing.*/
      _adc_isr_full_code(adcp);
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
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if STM32_ADC_USE_ADC1
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
  ADCD1.adc = ADC1;
  ADCD1.dmastp  = STM32_DMA1_STREAM1;
  ADCD1.dmamode = STM32_DMA_CR_PL(STM32_ADC_ADC1_DMA_PRIORITY) |
                  STM32_DMA_CR_MSIZE_HWORD | STM32_DMA_CR_PSIZE_HWORD |
                  STM32_DMA_CR_MINC        | STM32_DMA_CR_TCIE        |
                  STM32_DMA_CR_TEIE        | STM32_DMA_CR_EN;

  /* Temporary activation.*/
  rccEnableADC1(FALSE);
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
  rccDisableADC1(FALSE);
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

  /* If in stopped state then enables the ADC and DMA clocks.*/
  if (adcp->state == ADC_STOP) {
#if STM32_ADC_USE_ADC1
    if (&ADCD1 == adcp) {
      bool_t b;
      b = dmaStreamAllocate(adcp->dmastp,
                            STM32_ADC_ADC1_IRQ_PRIORITY,
                            (stm32_dmaisr_t)adc_lld_serve_rx_interrupt,
                            (void *)adcp);
      chDbgAssert(!b, "adc_lld_start(), #1", "stream already allocated");
      dmaStreamSetPeripheral(adcp->dmastp, &ADC1->DR);
      rccEnableADC1(FALSE);
    }
#endif

    /* ADC setup, the calibration procedure has already been performed
       during initialization.*/
    adcp->adc->CR1 = 0;
    adcp->adc->CR2 = 0;
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
#if STM32_ADC_USE_ADC1
    if (&ADCD1 == adcp) {
      ADC1->CR1 = 0;
      ADC1->CR2 = 0;
      dmaStreamRelease(adcp->dmastp);
      rccDisableADC1(FALSE);
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
  uint32_t mode, n;
  const ADCConversionGroup *grpp = adcp->grpp;

  /* DMA setup.*/
  mode = adcp->dmamode;
  if (grpp->circular)
    mode |= STM32_DMA_CR_CIRC;
  if (adcp->depth > 1) {
    /* If the buffer depth is greater than one then the half transfer interrupt
       interrupt is enabled in order to allows streaming processing.*/
    mode |= STM32_DMA_CR_HTIE;
    n = (uint32_t)grpp->num_channels * (uint32_t)adcp->depth;
  }
  else
    n = (uint32_t)grpp->num_channels;
  dmaStreamSetMemory0(adcp->dmastp, adcp->samples);
  dmaStreamSetTransactionSize(adcp->dmastp, n);
  dmaStreamSetMode(adcp->dmastp, mode);

  /* ADC setup.*/
  adcp->adc->CR1   = grpp->cr1 | ADC_CR1_SCAN;
  adcp->adc->CR2   = grpp->cr2 | ADC_CR2_DMA | ADC_CR2_CONT | ADC_CR2_ADON;
  adcp->adc->SMPR1 = grpp->smpr1;
  adcp->adc->SMPR2 = grpp->smpr2;
  adcp->adc->SQR1  = grpp->sqr1;
  adcp->adc->SQR2  = grpp->sqr2;
  adcp->adc->SQR3  = grpp->sqr3;

  /* ADC start by writing ADC_CR2_ADON a second time.*/
  adcp->adc->CR2   = grpp->cr2 | ADC_CR2_DMA | ADC_CR2_CONT | ADC_CR2_ADON;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  dmaStreamDisable(adcp->dmastp);
  adcp->adc->CR2 = 0;
}

#endif /* HAL_USE_ADC */

/** @} */
