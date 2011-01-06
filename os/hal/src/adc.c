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
 * @file    adc.c
 * @brief   ADC Driver code.
 *
 * @addtogroup ADC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   ADC Driver initialization.
 */
void adcInit(void) {

  adc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p ADCDriver structure.
 *
 * @param[out] adcp     pointer to the @p ADCDriver object
 */
void adcObjectInit(ADCDriver *adcp) {

  adcp->ad_state    = ADC_STOP;
  adcp->ad_config   = NULL;
  adcp->ad_callback = NULL;
  adcp->ad_samples  = NULL;
  adcp->ad_depth    = 0;
  adcp->ad_grpp     = NULL;
  chSemInit(&adcp->ad_sem, 0);
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] config    pointer to the @p ADCConfig object
 */
void adcStart(ADCDriver *adcp, const ADCConfig *config) {

  chDbgCheck((adcp != NULL) && (config != NULL), "adcStart");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_STOP) || (adcp->ad_state == ADC_READY),
              "adcStart(), #1",
              "invalid state");
  adcp->ad_config = config;
  adc_lld_start(adcp);
  adcp->ad_state = ADC_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adcStop(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStop");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_STOP) ||
              (adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_COMPLETE),
              "adcStop(), #1",
              "invalid state");
  adc_lld_stop(adcp);
  adcp->ad_state = ADC_STOP;
  chSysUnlock();
}

/**
 * @brief   Starts an ADC conversion.
 * @details Starts a conversion operation, there are two kind of conversion
 *          modes:
 *          - <b>LINEAR</b>, in this mode the buffer is filled once and then
 *            the conversion stops automatically.
 *          - <b>CIRCULAR</b>, in this mode the conversion never stops and
 *            the buffer is filled circularly.<br>
 *            During the conversion the callback function is invoked when
 *            the buffer is 50% filled and when the buffer is 100% filled,
 *            this way is possible to process the conversion stream in real
 *            time. This kind of conversion can only be stopped by explicitly
 *            invoking @p adcStopConversion().
 *          .
 * @note    The buffer is organized as a matrix of M*N elements where M is the
 *          channels number configured into the conversion group and N is the
 *          buffer depth. The samples are sequentially written into the buffer
 *          with no gaps.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] grpp      pointer to a @p ADCConversionGroup object
 * @param[out] samples  pointer to the samples buffer
 * @param[in] depth     buffer depth (matrix rows number). The buffer depth
 *                      must be one or an even number.
 * @param[in] callback  pointer to the conversion callback function, this
 *                      parameter can be @p NULL if a callback is not required
 * @return              The operation status.
 * @retval FALSE        the conversion has been started.
 * @retval TRUE         the driver is busy, conversion not started.
 */
bool_t adcStartConversion(ADCDriver *adcp,
                          const ADCConversionGroup *grpp,
                          adcsample_t *samples,
                          size_t depth,
                          adccallback_t callback) {

  chDbgCheck((adcp != NULL) && (grpp != NULL) && (samples != NULL) &&
             ((depth == 1) || ((depth & 1) == 0)),
             "adcStartConversion");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_RUNNING) ||
              (adcp->ad_state == ADC_COMPLETE),
              "adcStartConversion(), #1",
              "invalid state");
  if (adcp->ad_state == ADC_RUNNING) {
    chSysUnlock();
    return TRUE;
  }
  adcp->ad_callback = callback;
  adcp->ad_samples  = samples;
  adcp->ad_depth    = depth;
  adcp->ad_grpp     = grpp;
  adc_lld_start_conversion(adcp);
  adcp->ad_state = ADC_RUNNING;
  chSysUnlock();
  return FALSE;
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
void adcStopConversion(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStopConversion");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_RUNNING) ||
              (adcp->ad_state == ADC_COMPLETE),
              "adcStopConversion(), #1",
              "invalid state");
  if (adcp->ad_state == ADC_RUNNING) {
    adc_lld_stop_conversion(adcp);
    adcp->ad_grpp  = NULL;
    adcp->ad_state = ADC_READY;
    chSemResetI(&adcp->ad_sem, 0);
    chSchRescheduleS();
  }
  else
    adcp->ad_state = ADC_READY;
  chSysUnlock();
}

/**
 * @brief   Waits for completion.
 * @details If the conversion is not completed or not yet started then the
 *          invoking thread waits for a conversion completion event.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation result.
 * @retval RDY_OK       conversion finished.
 * @retval RDY_TIMEOUT  conversion not finished within the specified time.
 */
msg_t adcWaitConversion(ADCDriver *adcp, systime_t timeout) {

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_RUNNING) ||
              (adcp->ad_state == ADC_COMPLETE),
              "adcWaitConversion(), #1",
              "invalid state");
  if (adcp->ad_state != ADC_COMPLETE) {
    if (chSemWaitTimeoutS(&adcp->ad_sem, timeout) == RDY_TIMEOUT) {
      chSysUnlock();
      return RDY_TIMEOUT;
    }
  }
  chSysUnlock();
  return RDY_OK;
}

#endif /* CH_HAL_USE_ADC */

/** @} */
