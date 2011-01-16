/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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

#if HAL_USE_ADC || defined(__DOXYGEN__)

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
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void adcInit(void) {

  adc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p ADCDriver structure.
 *
 * @param[out] adcp     pointer to the @p ADCDriver object
 *
 * @init
 */
void adcObjectInit(ADCDriver *adcp) {

  adcp->ad_state    = ADC_STOP;
  adcp->ad_config   = NULL;
  adcp->ad_samples  = NULL;
  adcp->ad_depth    = 0;
  adcp->ad_grpp     = NULL;
#if ADC_USE_WAIT
  adcp->ad_thread   = NULL;
#endif /* ADC_USE_WAIT */
#if ADC_USE_MUTUAL_EXCLUSION
#if CH_USE_MUTEXES
  chMtxInit(&adcp->ad_mutex);
#else
  chSemInit(&adcp->ad_semaphore, 1);
#endif
#endif /* ADC_USE_MUTUAL_EXCLUSION */
#if defined(ADC_DRIVER_EXT_INIT_HOOK)
  ADC_DRIVER_EXT_INIT_HOOK(adcp);
#endif
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] config    pointer to the @p ADCConfig object. Depending on
 *                      the implementation the value can be @p NULL.
 *
 * @api
 */
void adcStart(ADCDriver *adcp, const ADCConfig *config) {

  chDbgCheck(adcp != NULL, "adcStart");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_STOP) || (adcp->ad_state == ADC_READY),
              "adcStart(), #1", "invalid state");
  adcp->ad_config = config;
  adc_lld_start(adcp);
  adcp->ad_state = ADC_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcStop(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStop");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_STOP) || (adcp->ad_state == ADC_READY),
              "adcStop(), #1", "invalid state");
  adc_lld_stop(adcp);
  adcp->ad_state = ADC_STOP;
  chSysUnlock();
}

/**
 * @brief   Starts an ADC conversion.
 * @details Starts an asynchronous conversion operation.
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
 *
 * @api
 */
void adcStartConversion(ADCDriver *adcp,
                        const ADCConversionGroup *grpp,
                        adcsample_t *samples,
                        size_t depth) {

  chSysLock();
  adcStartConversionI(adcp, grpp, samples, depth);
  chSysUnlock();
}

/**
 * @brief   Starts an ADC conversion.
 * @details Starts an asynchronous conversion operation.
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
 *
 * @iclass
 */
void adcStartConversionI(ADCDriver *adcp,
                         const ADCConversionGroup *grpp,
                         adcsample_t *samples,
                         size_t depth) {

  chDbgCheck((adcp != NULL) && (grpp != NULL) && (samples != NULL) &&
             ((depth == 1) || ((depth & 1) == 0)),
             "adcStartConversionI");

  chDbgAssert((adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_COMPLETE),
              "adcStartConversionI(), #1", "not ready");
  adcp->ad_samples  = samples;
  adcp->ad_depth    = depth;
  adcp->ad_grpp     = grpp;
  adcp->ad_state    = ADC_ACTIVE;
  adc_lld_start_conversion(adcp);
}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p ADC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcStopConversion(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStopConversion");

  chSysLock();
  chDbgAssert((adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_ACTIVE),
              "adcStopConversion(), #1", "invalid state");
  if (adcp->ad_state != ADC_READY) {
    adc_lld_stop_conversion(adcp);
    adcp->ad_grpp  = NULL;
    adcp->ad_state = ADC_READY;
    _adc_reset_s(adcp);
  }
  chSysUnlock();
}

/**
 * @brief   Stops an ongoing conversion.
 * @details This function stops the currently ongoing conversion and returns
 *          the driver in the @p ADC_READY state. If there was no conversion
 *          being processed then the function does nothing.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @iclass
 */
void adcStopConversionI(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcStopConversionI");

  chDbgAssert((adcp->ad_state == ADC_READY) ||
              (adcp->ad_state == ADC_ACTIVE) ||
              (adcp->ad_state == ADC_COMPLETE),
              "adcStopConversionI(), #1", "invalid state");
  if (adcp->ad_state != ADC_READY) {
    adc_lld_stop_conversion(adcp);
    adcp->ad_grpp  = NULL;
    adcp->ad_state = ADC_READY;
    _adc_reset_i(adcp);
  }
}

#if ADC_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Performs an ADC conversion.
 * @details Performs a synchronous conversion operation.
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
 * @return              The operation result.
 * @retval RDY_OK       Conversion finished.
 * @retval RDY_RESET    The conversion has been stopped using
 *                      @p acdStopConversion() or @p acdStopConversionI(),
 *                      the result buffer may contain incorrect data.
 *
 * @api
 */
msg_t adcConvert(ADCDriver *adcp,
                 const ADCConversionGroup *grpp,
                 adcsample_t *samples,
                 size_t depth) {
  msg_t msg;

  chSysLock();
  chDbgAssert(grpp->acg_endcb == NULL, "adcConvert(), #1", "has callback");
  adcStartConversionI(adcp, grpp, samples, depth);
  (adcp)->ad_thread = chThdSelf();
  chSchGoSleepS(THD_STATE_SUSPENDED);
  msg = chThdSelf()->p_u.rdymsg;
  chSysUnlock();
  return msg;
}
#endif /* ADC_USE_WAIT */

#if ADC_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the ADC peripheral.
 * @details This function tries to gain ownership to the ADC bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p ADC_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcAcquireBus(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcAcquireBus");

#if CH_USE_MUTEXES
  chMtxLock(&adcp->ad_mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&adcp->ad_semaphore);
#endif
}

/**
 * @brief   Releases exclusive access to the ADC peripheral.
 * @pre     In order to use this function the option @p ADC_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @api
 */
void adcReleaseBus(ADCDriver *adcp) {

  chDbgCheck(adcp != NULL, "adcReleaseBus");

#if CH_USE_MUTEXES
  (void)adcp;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal(&adcp->ad_semaphore);
#endif
}
#endif /* ADC_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_ADC */

/** @} */
