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
 * @file    demos/STM32/RT-STM32-LWIP/cfg/vfsconf.h
 * @brief   VFS configuration for the lwIP ROMFS HTTP demo.
 */

#ifndef VFSCONF_H
#define VFSCONF_H

#define _CHIBIOS_VFS_CONF_
#define _CHIBIOS_VFS_CONF_VER_1_0_

#if !defined(VFS_CFG_NAMELEN_MAX) || defined(__DOXYGEN__)
#define VFS_CFG_NAMELEN_MAX                 31
#endif

#if !defined(VFS_CFG_PATHLEN_MAX) || defined(__DOXYGEN__)
#define VFS_CFG_PATHLEN_MAX                 255
#endif

#if !defined(VFS_CFG_PATHBUFS_NUM) || defined(__DOXYGEN__)
#define VFS_CFG_PATHBUFS_NUM                1
#endif

#if !defined(VFS_CFG_ENABLE_DRV_OVERLAY) || defined(__DOXYGEN__)
#define VFS_CFG_ENABLE_DRV_OVERLAY          FALSE
#endif

#if !defined(VFS_CFG_ENABLE_DRV_STREAMS) || defined(__DOXYGEN__)
#define VFS_CFG_ENABLE_DRV_STREAMS          FALSE
#endif

#if !defined(VFS_CFG_ENABLE_DRV_CHFS) || defined(__DOXYGEN__)
#define VFS_CFG_ENABLE_DRV_CHFS             FALSE
#endif

#if !defined(VFS_CFG_ENABLE_DRV_FATFS) || defined(__DOXYGEN__)
#define VFS_CFG_ENABLE_DRV_FATFS            FALSE
#endif

#if !defined(VFS_CFG_ENABLE_DRV_LITTLEFS) || defined(__DOXYGEN__)
#define VFS_CFG_ENABLE_DRV_LITTLEFS         FALSE
#endif

#if !defined(VFS_CFG_ENABLE_DRV_ROMFS) || defined(__DOXYGEN__)
#define VFS_CFG_ENABLE_DRV_ROMFS            TRUE
#endif

#if !defined(DRV_CFG_ROM_DIR_NODES_NUM) || defined(__DOXYGEN__)
#define DRV_CFG_ROM_DIR_NODES_NUM           1
#endif

#if !defined(DRV_CFG_ROM_FILE_NODES_NUM) || defined(__DOXYGEN__)
#define DRV_CFG_ROM_FILE_NODES_NUM          4
#endif

#endif /* VFSCONF_H */
