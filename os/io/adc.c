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
  chSemInit(&adcp->adc_sem, 0);
}

/**
 * @brief Configures and activates the ADC peripheral.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 * @param[in] config pointer to the @p ADCConfig object
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

/**
 * @brief Starts an ADC conversion.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 * @param[in] grpp pointer to a @p ADCConversionGroup object
 * @param[out] samples pointer to the samples buffer
 * @param[in] depth buffer depth (matrix rows number). The buffer depth must
 *                  be one or an even number.
 * @return The operation status.
 * @retval FALSE the conversion has been started.
 * @retval TRUE the driver is busy, conversion not started.
 *
 * @note The buffer is organized as a matrix of M*N elements where M is the
 *       channels number configured into the conversion group and N is the
 *       buffer depth. The samples are sequentially written into the buffer
 *       with no gaps.
 */
bool_t adcStartConversion(ADCDriver *adcp,
                          ADCConversionGroup *grpp,
                          void *samples,
                          size_t depth) {

  chDbgCheck((adcp != NULL) && (grpp != NULL) && (samples != NULL) &&
             ((depth == 1) || ((depth & 1) == 0)),
             "adcStartConversion");

  chSysLock();

  chDbgAssert((adcp->adc_state == ADC_READY) ||
              (adcp->adc_state == ADC_RUNNING),
              "adcStartConversion(), #1",
              "invalid state");

  if (adcp->adc_state == ADC_RUNNING) {
    chSysUnlock();
    return TRUE;
  }

  adc_lld_start_conversion(adcp, grpp, samples);
  adcp->adc_state = ADC_RUNNING;

  chSysUnlock();
  return FALSE;
}

/**
 * @brief Stops an ongoing conversion.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 */
void adcStopConversion(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStopConversion");

  chSysLock();

  chDbgAssert((adcp->adc_state == ADC_READY) ||
              (adcp->adc_state == ADC_RUNNING),
              "adcStopConversion(), #1",
              "invalid state");

  adc_lld_stop_conversion(adcp);
  adcp->adc_state = ADC_READY;

  chSysUnlock();
}

/**
 * @brief Waits for completion.
 *
 * @param[in] adcp pointer to the @p ADCDriver object
 * @param[in] timeout the number of ticks before the operation timeouts,
 *                    the following special values are allowed:
 *                    - @a TIME_IMMEDIATE immediate timeout.
 *                    - @a TIME_INFINITE no timeout.
 *                    .
 * @return The operation result.
 * @retval RDY_OK conversion finished (or not started).
 * @retval RDY_TIMEOUT conversion not finished within the specified time.
 */
msg_t adcWaitConversion(ADCDriver *adcp, systme_t timeout) {

  chSysLock();

  chDbgAssert((adcp->adc_state == ADC_READY) ||
              (adcp->adc_state == ADC_RUNNING),
              "adcWaitConversion(), #1",
              "invalid state");

  if (adcp->adc_state == ADC_RUNNING) {
    if (chSemWaitTimeoutS(&adcp->adc_sem, timeout) == RDY_TIMEOUT) {
      chSysUnlock();
      return RDY_TIMEOUT;
    }
  }
  chSysUnlock();
  return RDY_OK;
}

/** @} */
