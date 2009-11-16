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

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level ADC driver initialization.
 */
void adc_lld_init(void) {

#if USE_STM32_ADC1
  adcObjectInit(&ADCD1);
  DMA1_Channel1->CPAR = (uint32_t)ADC1->DR;
  ADCD1.ad_adc = ADC1;
  ADCD1.ad_dma = DMA1_Channel1;
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
      RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    }
#endif
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
      NVICDisableVector(DMA1_Channel1_IRQn);
      dmaDisable(DMA1_ID);
      RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;
    }
#endif
  }
}

/**
 * @brief Starts an ADC conversion.
 * @details Starts a conversion operation, there are two kind of conversion
 *          modes:
 *          - <b>LINEAR</b>, this mode is activated when the @p callback
 *            parameter is set to @p NULL, in this mode the buffer is filled
 *            once and then the conversion stops automatically.
 *          - <b>CIRCULAR</b>, when a callback function is defined the
 *            conversion never stops and the buffer is filled circularly.
 *            During the conversion the callback function is invoked when
 *            the buffer is 50% filled and when the buffer is 100% filled,
 *            this way is possible to process the conversion stream in real
 *            time. This kind of conversion can only be stopped by explicitly
 *            invoking @p adcStopConversion().
 *          .
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] grpp      pointer to a @p ADCConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 * @param[in] callback  pointer to the conversion callback function
 * @return The operation status.
 * @retval FALSE        the conversion has been started.
 * @retval TRUE         the driver is busy, conversion not started.
 *
 * @note The buffer is organized as a matrix of M*N elements where M is the
 *       channels number configured into the conversion group and N is the
 *       buffer depth. The samples are sequentially written into the buffer
 *       with no gaps.
 */
void adc_lld_start_conversion(ADCDriver *adcp,
                              ADCConversionGroup *grpp,
                              void *samples,
                              size_t depth,
                              adccallback_t callback) {

}

/**
 * @brief Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

}

/** @} */
