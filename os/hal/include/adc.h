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
 * @file    adc.h
 * @brief   ADC Driver macros and structures.
 *
 * @addtogroup ADC
 * @{
 */

#ifndef _ADC_H_
#define _ADC_H_

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
#error "ADC driver requires CH_USE_SEMAPHORES"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  ADC_UNINIT = 0,                           /**< @brief Not initialized.    */
  ADC_STOP = 1,                             /**< @brief Stopped.            */
  ADC_READY = 2,                            /**< @brief Ready.              */
  ADC_RUNNING = 3,                          /**< @brief Conversion running. */
  ADC_COMPLETE = 4                          /**< @brief Conversion complete.*/
} adcstate_t;

#include "adc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void adcInit(void);
  void adcObjectInit(ADCDriver *adcp);
  void adcStart(ADCDriver *adcp, const ADCConfig *config);
  void adcStop(ADCDriver *adcp);
  bool_t adcStartConversion(ADCDriver *adcp,
                            const ADCConversionGroup *grpp,
                            adcsample_t *samples,
                            size_t depth,
                            adccallback_t callback);
  void adcStopConversion(ADCDriver *adcp);
  msg_t adcWaitConversion(ADCDriver *adcp, systime_t timeout);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_ADC */

#endif /* _ADC_H_ */

/** @} */
