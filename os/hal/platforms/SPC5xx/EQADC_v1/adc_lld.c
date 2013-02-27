/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    SPC5xx/EQADC_v1/adc_lld.c
 * @brief   SPC5xx low level ADC driver code.
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
 * @brief   ADCD1 driver identifier.
 */
#if SPC5_ADC_USE_EQADC1_Q0 || defined(__DOXYGEN__)
ADCDriver ADCD10;
#endif

/**
 * @brief   ADCD20 driver identifier.
 */
#if SPC5_ADC_USE_EQADC2_Q0 || defined(__DOXYGEN__)
ADCDriver ADCD20;
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

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if SPC5_ADC_USE_EQADC1_Q0
  /* Driver initialization.*/
  adcObjectInit(&ADCD10);
#endif /* SPC5_ADC_USE_EQADC1_Q0 */

#if SPC5_ADC_USE_EQADC2_Q0
  /* Driver initialization.*/
  adcObjectInit(&ADCD20);
#endif /* SPC5_ADC_USE_EQADC2_Q0 */
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
#if SPC5_ADC_USE_EQADC1_Q0
    if (&ADCD10 == adcp) {

    }
#endif /* SPC5_ADC_USE_EQADC1_Q0 */
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
#if SPC5_ADC_USE_EQADC1_Q0
    if (&ADCD1 == adcp) {

    }
#endif /* SPC5_ADC_USE_EQADC1_Q0 */
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
