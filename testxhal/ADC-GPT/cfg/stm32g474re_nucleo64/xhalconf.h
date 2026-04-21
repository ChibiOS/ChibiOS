/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    templates/xhalconf.h
 * @brief   XHAL configuration header.
 * @details XHAL configuration file, this file allows to enable or disable the
 *          various device drivers from your application. You may also use
 *          this file in order to override the device drivers default settings.
 *
 * @addtogroup XHAL_CONF
 * @{
 */

#ifndef XHALCONF_H
#define XHALCONF_H

#define __CHIBIOS_XHAL_CONF__
#define __CHIBIOS_XHAL_CONF_VER_1_0__

#include "xmcuconf.h"

/*===========================================================================*/
/* HAL general settings.                                                     */
/*===========================================================================*/

#define HAL_USE_PAL                         TRUE
#define HAL_USE_ADC                         TRUE
#define HAL_USE_ETH                         FALSE
#define HAL_USE_GPT                         TRUE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_PWM                         FALSE
#define HAL_USE_RTC                         FALSE
#define HAL_USE_SIO                         FALSE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_WSPI                        FALSE

/*===========================================================================*/
/* ADC driver settings.                                                      */
/*===========================================================================*/

#define ADC_USE_WAIT                        TRUE

/*===========================================================================*/
/* GPT driver settings.                                                      */
/*===========================================================================*/

#define GPT_DEFAULT_FREQUENCY               1000000U

/*===========================================================================*/
/* ICU driver settings.                                                      */
/*===========================================================================*/

#define ICU_USE_OVERFLOW_SCALING            FALSE
#define ICU_USE_CONFIGURATIONS              FALSE
#define ICU_DEFAULT_FREQUENCY               1000000U
#define ICU_DEFAULT_ARR                     0xFFFFFFFFU

/*===========================================================================*/
/* PWM driver settings.                                                      */
/*===========================================================================*/

#define PWM_USE_CONFIGURATIONS              FALSE
#define PWM_DEFAULT_FREQUENCY               1000000U
#define PWM_DEFAULT_PERIOD                  1000U

/*===========================================================================*/
/* PAL driver settings.                                                      */
/*===========================================================================*/

#define PAL_USE_CALLBACKS                   TRUE
#define PAL_USE_WAIT                        TRUE

/*===========================================================================*/
/* ETH driver settings.                                                      */
/*===========================================================================*/

#define ETH_USE_SYNCHRONIZATION             TRUE
#define ETH_USE_EVENTS                      FALSE
#define ETH_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* SIO driver settings.                                                      */
/*===========================================================================*/

#define SIO_DEFAULT_BITRATE                 38400
#define SIO_USE_SYNCHRONIZATION             TRUE
#define SIO_USE_STREAMS_INTERFACE           SIO_USE_SYNCHRONIZATION
#define SIO_USE_BUFFERING                   FALSE
#define SIO_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* SPI driver settings.                                                      */
/*===========================================================================*/

#define SPI_USE_SYNCHRONIZATION             TRUE
#define SPI_USE_ASSERT_ON_ERROR             FALSE
#define SPI_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* WSPI driver settings.                                                     */
/*===========================================================================*/

#define WSPI_USE_SYNCHRONIZATION            TRUE
#define WSPI_USE_MUTUAL_EXCLUSION           TRUE

#endif /* XHALCONF_H */

/** @} */
