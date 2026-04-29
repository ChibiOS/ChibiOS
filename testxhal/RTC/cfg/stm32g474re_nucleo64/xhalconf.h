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

#define HAL_USE_MUTUAL_EXCLUSION            TRUE
#define HAL_USE_REGISTRY                    TRUE

/*===========================================================================*/
/* HAL enabled drivers.                                                      */
/*===========================================================================*/

#define HAL_USE_PAL                         TRUE
#define HAL_USE_ADC                         FALSE
#define HAL_USE_DAC                         FALSE
#define HAL_USE_CAN                         FALSE
#define HAL_USE_EFL                         FALSE
#define HAL_USE_ETH                         FALSE
#define HAL_USE_GPT                         FALSE
#define HAL_USE_I2C                         FALSE
#define HAL_USE_I2S                         FALSE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_PWM                         FALSE
#define HAL_USE_RTC                         TRUE
#define HAL_USE_SDC                         FALSE
#define HAL_USE_SIO                         FALSE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_TRNG                        FALSE
#define HAL_USE_USB                         FALSE
#define HAL_USE_WDG                         FALSE
#define HAL_USE_WSPI                        FALSE

/*===========================================================================*/
/* ADC driver settings.                                                      */
/*===========================================================================*/

#define ADC_USE_SYNCHRONIZATION             TRUE
#define ADC_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* DAC driver settings.                                                      */
/*===========================================================================*/

#define DAC_USE_SYNCHRONIZATION             TRUE

/*===========================================================================*/
/* CAN driver settings.                                                      */
/*===========================================================================*/

#define CAN_USE_SYNCHRONIZATION             TRUE
#define CAN_USE_SLEEP_MODE                  TRUE
#define CAN_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* GPT driver settings.                                                      */
/*===========================================================================*/

#define GPT_DEFAULT_FREQUENCY               1000000U
#define GPT_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* I2C driver settings.                                                      */
/*===========================================================================*/

#define I2C_USE_SYNCHRONIZATION             TRUE
#define I2C_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* I2S driver settings.                                                      */
/*===========================================================================*/

#define I2S_USE_SYNCHRONIZATION             TRUE

/*===========================================================================*/
/* ICU driver settings.                                                      */
/*===========================================================================*/

#define ICU_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* PWM driver settings.                                                      */
/*===========================================================================*/

#define PWM_USE_CONFIGURATIONS              FALSE

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
/* SDC driver settings.                                                      */
/*===========================================================================*/

#define SDC_USE_SYNCHRONIZATION             TRUE
#define SDC_USE_CONFIGURATIONS              FALSE
#define SDC_INIT_RETRY                      100
#define SDC_MMC_SUPPORT                     FALSE
#define SDC_NICE_WAITING                    TRUE
#define SDC_INIT_OCR_V20                    0x50FF8000U
#define SDC_INIT_OCR                        0x80100000U

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
/* USB driver settings.                                                      */
/*===========================================================================*/

#define USB_USE_SYNCHRONIZATION             TRUE
#define USB_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* Serial USB settings.                                                      */
/*===========================================================================*/

#define SERIAL_USB_USE_MODULE               FALSE
#define SERIAL_USB_BUFFERS_SIZE             256U
#define SERIAL_USB_BUFFERS_NUMBER           2U
#define SERIAL_USB_SEND_ZLP                 TRUE
#define SERIAL_USB_RX_PACKET_MODE           FALSE

/*===========================================================================*/
/* WSPI driver settings.                                                     */
/*===========================================================================*/

#define WSPI_USE_SYNCHRONIZATION            TRUE

#endif /* XHALCONF_H */

/** @} */
