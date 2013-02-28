/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    templates/adc_lld.c
 * @brief   ADC Driver subsystem low level driver source template.
 *
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

/**
 * @brief   ADC1 driver identifier.
 */
#if PLATFORM_ADC_USE_ADC1 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

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

#if PLATFORM_ADC_USE_ADC1
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
#endif /* PLATFORM_ADC_USE_ADC1 */
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
#if PLATFORM_ADC_USE_ADC1
    if (&ADCD1 == adcp) {

    }
#endif /* PLATFORM_ADC_USE_ADC1 */
  }
  /* Configures the peripheral.*/

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

    /* Disables the peripheral.*/
#if PLATFORM_ADC_USE_ADC1
    if (&ADCD1 == adcp) {

    }
#endif /* PLATFORM_ADC_USE_ADC1 */
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

  (void)adcp;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  (void)adcp;
}

#endif /* HAL_USE_ADC */

/** @} */
