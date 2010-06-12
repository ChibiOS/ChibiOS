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
 * @file    templates/adc_lld.h
 * @brief   ADC Driver subsystem low level driver header template.
 *
 * @addtogroup ADC_LLD
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

#if CH_HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CH_USE_SEMAPHORES
#error "the ADC driver requires CH_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC sample data type.
 */
typedef uint16_t adcsample_t;

/**
 * @brief   Channels number in a conversion group.
 */
typedef uint16_t adc_channels_num_t;

/**
 * @brief   ADC notification callback type.
 *
 * @param[in] buffer    pointer to the most recent samples data
 * @param[in] n         number of buffer rows available starting from @p buffer
 */
typedef void (*adccallback_t)(adcsample_t *buffer, size_t n);

/**
 * @brief   Conversion group configuration structure.
 * @details This implementation-dependent structure describes a conversion
 *          operation.
 */
typedef struct {
  /**
   * @brief Enables the circular buffer mode for the group.
   */
  bool_t                    acg_circular;
  /**
   * @brief Number of the analog channels belonging to the conversion group.
   */
  adc_channels_num_t        acg_num_channels;
  /* End of the mandatory fields.*/
} ADCConversionGroup;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {

} ADCConfig;

/**
 * @brief   Structure representing an ADC driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  adcstate_t                ad_state;
  /**
   * @brief Current configuration data.
   */
  const ADCConfig           *ad_config;
  /**
   * @brief Synchronization semaphore.
   */
  Semaphore                 ad_sem;
  /**
   * @brief Current callback function or @p NULL.
   */
  adccallback_t             ad_callback;
  /**
   * @brief Current samples buffer pointer or @p NULL.
   */
  adcsample_t               *ad_samples;
  /**
   * @brief Current samples buffer depth or @p 0.
   */
  size_t                    ad_depth;
  /**
   * @brief Current conversion group pointer or @p NULL.
   */
  const ADCConversionGroup  *ad_grpp;
  /* End of the mandatory fields.*/
} ADCDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  void adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp);
  void adc_lld_stop_conversion(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_ADC */

#endif /* _ADC_LLD_H_ */

/** @} */
