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
 * @file templates/adc_lld.h
 * @brief ADC Driver subsystem low level driver header template
 * @addtogroup ADC_LLD
 * @{
 */

#ifndef _ADC_LLD_H_
#define _ADC_LLD_H_

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Driver configuration structure.
 */
typedef struct {

} ADCConfig;

/**
 * @brief Structure representing a MAC driver.
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
                                void *samples);
  void adc_lld_stop_conversion(ADCDriver *adcp);
#ifdef __cplusplus
}
#endif

#endif /* _ADC_LLD_H_ */

/** @} */
