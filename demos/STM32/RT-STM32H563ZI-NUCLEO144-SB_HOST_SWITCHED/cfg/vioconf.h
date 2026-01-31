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
 * @file    templates/vioconf.h
 * @brief   VIO configuration header.
 * @details VIO configuration file, this file allows to enable or disable the
 *          various virtual peripherals from your application.
 *
 * @addtogroup VIO_CONF
 * @{
 */

#ifndef VIOCONF_H
#define VIOCONF_H

#define __CHIBIOS_VIO_CONF__
#define __CHIBIOS_VIO_CONF_VER_1_0__

/**
 * @brief   Enables the Virtual GPIO support.
 */
#if !defined(VIO_CFG_ENABLE_GPIO) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_GPIO                 TRUE
#endif

/**
 * @brief   Enables the Virtual SPI support.
 */
#if !defined(VIO_CFG_ENABLE_SPI) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_SPI                  FALSE
#endif

/**
 * @brief   Enables the Virtual UART support.
 */
#if !defined(VIO_CFG_ENABLE_UART) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_UART                 TRUE
#endif

#endif /* VIOCONF_H */

/** @} */
