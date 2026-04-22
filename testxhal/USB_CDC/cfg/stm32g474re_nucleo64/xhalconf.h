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

#ifndef XHALCONF_H
#define XHALCONF_H

#define __CHIBIOS_XHAL_CONF__
#define __CHIBIOS_XHAL_CONF_VER_1_0__

#include "xmcuconf.h"

/*===========================================================================*/
/* HAL general settings.                                                     */
/*===========================================================================*/

#define HAL_USE_PAL                         TRUE
#define HAL_USE_ADC                         FALSE
#define HAL_USE_CAN                         FALSE
#define HAL_USE_ETH                         FALSE
#define HAL_USE_GPT                         FALSE
#define HAL_USE_I2C                         FALSE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_PWM                         FALSE
#define HAL_USE_RTC                         FALSE
#define HAL_USE_SDC                         FALSE
#define HAL_USE_SIO                         FALSE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_USB                         TRUE
#define HAL_USE_WSPI                        FALSE

/*===========================================================================*/
/* PAL driver settings.                                                      */
/*===========================================================================*/

#define PAL_USE_CALLBACKS                   TRUE
#define PAL_USE_WAIT                        TRUE

/*===========================================================================*/
/* USB driver settings.                                                      */
/*===========================================================================*/

#define USB_USE_WAIT                        TRUE
#define USB_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* Serial USB settings.                                                      */
/*===========================================================================*/

#define SERIAL_USB_USE_MODULE               TRUE
#define SERIAL_USB_BUFFERS_SIZE             256U
#define SERIAL_USB_BUFFERS_NUMBER           2U
#define SERIAL_USB_SEND_ZLP                 TRUE
#define SERIAL_USB_RX_PACKET_MODE           FALSE

#endif /* XHALCONF_H */
