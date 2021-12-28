/*
    ChibiOS - Copyright (C) 2006..2020 Giovanni Di Sirio

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
 * @file    sb/templates/sbconf.h
 * @brief   Configuration file template.
 * @details A copy of this file must be placed in each project directory, it
 *          contains the application specific kernel settings.
 *
 * @addtogroup SB_CONFIG
 * @details Sandboxes-related settings and hooks.
 * @{
 */

#ifndef SBCONF_H
#define SBCONF_H

#define __CHIBIOS_SB_CONF__
#define __CHIBIOS_SB_CONF_VER_3_0__

/**
 * @brief   Number of memory regions for each sandbox.
 */
#if !defined(SB_CFG_NUM_REGIONS) || defined(__DOXYGEN__)
#define SB_CFG_NUM_REGIONS                  2
#endif

/**
 * @brief   Enables Posix API in sandboxes using VFS.
 */
#if !defined(SB_CFG_ENABLE_VFS) || defined(__DOXYGEN__)
#define SB_CFG_ENABLE_VFS                   TRUE
#endif

/**
 * @brief   Number of file descriptors for each sandbox.
 */
#if !defined(SB_CFG_FD_NUM) || defined(__DOXYGEN__)
#define SB_CFG_FD_NUM                       12
#endif

#endif  /* SBCONF_H */

/** @} */
