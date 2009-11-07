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
 * @file STM32/adc_lld.h
 * @brief STM32 ADC subsystem low level driver header
 * @addtogroup STM32_ADC
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief ADC sample data type.
 */
typedef uint16_t adcsample_t;

/**
 * @brief Channels number in a conversion group.
 */
typedef uint16_t adc_channels_num_t;

/**
 * @brief ADC notification callback type.
 * @param[in] buffer pointer to the most recent samples data
 * @param[in] n number of buffer rows available starting from @p buffer
 */
typedef void (*adccallback_t)(adcsample_t *buffer, size_t n);

/**
 * @brief Conversion group configuration structure.
 * @details This implementation-dependent structure describes a conversion
 *          operation.
 */
typedef struct {
  /**
   * @brief Number of the analog channels belonging to the conversion group.
   */
  adc_channels_num_t    acg_num_channels;
} ADCConversionGroup;

/**
 * @brief Driver configuration structure.
 */
typedef struct {

} ADCConfig;

/**
 * @brief Structure representing an ADC driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  adcstate_t            adc_state;
  /**
   * @brief Current configuration data.
   */
  const ADCConfig       *adc_config;
  /**
   * @brief Semaphore for completion synchronization.
   */
  Semaphore             adc_sem;
  /* End of the mandatory fields.*/
} ADCDriver;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  void adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp,
                                ADCConversionGroup *grpp,
                                void *samples,
                                size_t depth,
                                adccallback_t callback);
  void adc_lld_stop_conversion(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* _ADC_LLD_H_ */

/** @} */
