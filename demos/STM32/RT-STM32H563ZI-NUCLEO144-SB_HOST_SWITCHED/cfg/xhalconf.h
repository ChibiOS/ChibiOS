/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_SIO                         TRUE
#define HAL_USE_SPI                         FALSE

/*===========================================================================*/
/* PAL driver settings.                                                      */
/*===========================================================================*/

#define PAL_USE_CALLBACKS                   TRUE
#define PAL_USE_WAIT                        TRUE

/*===========================================================================*/
/* SIO driver settings.                                                      */
/*===========================================================================*/

#define SIO_DEFAULT_BITRATE                 38400
#define SIO_USE_SYNCHRONIZATION             TRUE
#define SIO_USE_STREAMS_INTERFACE           SIO_USE_SYNCHRONIZATION
#define SIO_USE_BUFFERING                   TRUE
#define SIO_USE_CONFIGURATIONS              FALSE

/*===========================================================================*/
/* SPI driver settings.                                                      */
/*===========================================================================*/

#define SPI_USE_SYNCHRONIZATION             TRUE
#define SPI_USE_ASSERT_ON_ERROR             FALSE
#define SPI_USE_CONFIGURATIONS              FALSE

#endif /* XHALCONF_H */

/** @} */
