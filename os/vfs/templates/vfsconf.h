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
 * @file    templates/vfsconf.h
 * @brief   VFS configuration header.
 *
 * @addtogroup VFS_CONF
 * @{
 */

#ifndef VFSCONF_H
#define VFSCONF_H

#define _CHIBIOS_VFS_CONF_
#define _CHIBIOS_VFS_CONF_VER_1_0_

/**
 * @brief   Maximum of drivers to be registered in VFS.
 */
#if !defined(VFS_CFG_MAX_DRIVERS) || defined(__DOXYGEN__)
#define VFS_CFG_MAX_DRIVERS                 2
#endif

#endif /* VFSCONF_H */

/** @} */
