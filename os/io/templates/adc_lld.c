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
 * @file templates/adc_lld.c
 * @brief ADC Driver subsystem low level driver source template
 * @addtogroup ADC_LLD
 * @{
 */

#include <ch.h>
#include <adc.h>

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

}

/**
 * @brief Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_start(ADCDriver *adcp) {

  if (adcp->adc_state == ADC_STOP) {
    /* Clock activation.*/
  }
  /* Configuration.*/
}

/**
 * @brief Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop(ADCDriver *adcp) {

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

}

/**
 * @brief Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

}

/** @} */
