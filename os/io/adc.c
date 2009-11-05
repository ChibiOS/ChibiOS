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
 * @file adc.c
 * @brief ADC Driver code.
 * @addtogroup ADC
 * @{
 */

#include <ch.h>
#include <adc.h>

/**
 * @brief ADC Driver initialization.
 */
void adcInit(void) {

  adc_lld_init();
}

/**
 * @brief Initializes the standard part of a @p ADCDriver structure.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 */
void adcObjectInit(ADCDriver *adcp) {

  adcp->adc_state = ADC_STOP;
  adcp->adc_config = NULL;
}

/**
 * @brief Configures and activates the ADC peripheral.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 * @param config pointer to the @p ADCConfig object
 */
void adcStart(ADCDriver *adcp, const ADCConfig *config) {

  chDbgCheck((adcp != NULL) && (config != NULL), "adcStart");
  chDbgAssert((adcp->adc_state == ADC_STOP) || (adcp->adc_state == ADC_READY),
              "adcStart(), #1",
              "invalid state");

  adcp->adc_config = config;
  adc_lld_start(adcp);
  adcp->adc_state = ADC_READY;
}

/**
 * @brief Deactivates the ADC peripheral.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 */
void adcStop(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStop");
  chDbgAssert((adcp->spd_state == ADC_STOP) || (adcp->spd_state == ADC_READY),
              "adcStop(), #1",
              "invalid state");

  adc_lld_stop(adcp);
  adcp->spd_state = ADC_STOP;
}

/** @} */
