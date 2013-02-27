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
#if SPC5_ADC_USE_EQADC_Q0 || defined(__DOXYGEN__)
ADCDriver ADCD1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Enables a CFIFO.
 *
 * @param[in] fifo      the FIFO identifier
 * @param[in] cfcr      CFCR register value
 * @param[in] idcr      IDCR register value
 *
 * @notapi
 */
static void cfifo_enable(adcfifo_t fifo, uint16_t cfcr, uint16_t idcr) {

  EQADC.CFCR[fifo].R = cfcr;
  EQADC.IDCR[fifo].R = idcr;
}


/**
 * @brief   Disables a CFIFO and the associated resources.
 *
 * @param[in] fifo      the FIFO identifier
 *
 * @notapi
 */
static void cfifo_disable(adcfifo_t fifo) {

  /* Disables the CFIFO.*/
  EQADC.CFCR[fifo].R = EQADC_CFCR_MODE_DISABLED;

  /* Disables Interrupts and DMAs of the CFIFO.*/
  EQADC.IDCR[fifo].R = 0;

  /* Waits for the CFIFO to become idle.*/
  while ((EQADC.CFSR.R & (0xC0000000 >> (fifo * 2))) != 0)
    ;

  /* Invalidates the CFIFO.*/
  EQADC.CFCR[fifo].R = EQADC_CFCR_CFINV | EQADC_CFCR_MODE_DISABLED;

  /* Clears all Interrupts and eDMA flags for the CFIFO.*/
  EQADC.FISR[fifo].R = EQADC_FISR_CLEAR_MASK;

  /* Clears the Tx Count Registers for the CFIFO.*/
  EQADC.CFTCR[fifo].R = 0;
}

/**
 * @brief   Pushes a command into the CFIFO0.
 *
 * @param[in] cmd       the command
 *
 * @notapi
 */
static void cfifo0_push_command(adccommand_t cmd) {

  while (EQADC.FISR[0].B.CFCTR >= 4)
    ;
  EQADC.CFPR[0].R = cmd;
}

/**
 * @brief   Waits until the RFIFO0 contains the specified number of entries.
 *
 * @param[in] n     number of entries
 *
 * @notapi
 */
static void cfifo0_wait_rfifo(uint32_t n) {

  while (EQADC.FISR[0].B.RFCTR < n)
    ;
  EQADC.FISR[0].R = EQADC_FISR_CLEAR_MASK;
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

#if SPC5_ADC_USE_EQADC_Q0
  /* Driver initialization.*/
  adcObjectInit(&ADCD1);
#endif /* SPC5_ADC_USE_EQADC_Q0 */
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
#if SPC5_ADC_USE_EQADC_Q0
    if (&ADCD1 == adcp) {

    }
#endif /* SPC5_ADC_USE_EQADC_Q0 */
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
#if SPC5_ADC_USE_EQADC_Q0
    if (&ADCD1 == adcp) {

    }
#endif /* SPC5_ADC_USE_EQADC_Q0 */
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
