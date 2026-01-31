[#ftl]
[#--
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="sbconf.h" /]
/*
[@license.EmitLicenseAsText /]
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
 * @brief   Sandboxes privileged stack size.
 */
#if !defined(SB_CFG_PRIVILEGED_STACK_SIZE) || defined(__DOXYGEN__)
#define SB_CFG_PRIVILEGED_STACK_SIZE        ${doc.SB_CFG_PRIVILEGED_STACK_SIZE!"1024"}
#endif

/**
 * @brief   Number of memory regions for each sandbox.
 */
#if !defined(SB_CFG_NUM_REGIONS) || defined(__DOXYGEN__)
#define SB_CFG_NUM_REGIONS                  ${doc.SB_CFG_NUM_REGIONS!"2"}
#endif

/**
 * @brief   Enables support for sandbox Virtual IRQs.
 */
#if !defined(SB_CFG_ENABLE_VRQ) || defined(__DOXYGEN__)
#define SB_CFG_ENABLE_VRQ                   ${doc.SB_CFG_ENABLE_VRQ!"TRUE"}
#endif

/**
 * @brief   Virtual IRQ to be used for alarm.
 */
#if !defined(SB_CFG_ALARM_VRQ) || defined(__DOXYGEN__)
#define SB_CFG_ALARM_VRQ                    ${doc.SB_CFG_ALARM_VRQ!"0"}
#endif

/**
 * @brief   Enables support for sandbox virtualized I/O.
 */
#if !defined(SB_CFG_ENABLE_VIO) || defined(__DOXYGEN__)
#define SB_CFG_ENABLE_VIO                   ${doc.SB_CFG_ENABLE_VIO!"TRUE"}
#endif

/**
 * @brief   Enables Posix API in sandboxes using VFS.
 */
#if !defined(SB_CFG_ENABLE_VFS) || defined(__DOXYGEN__)
#define SB_CFG_ENABLE_VFS                   ${doc.SB_CFG_ENABLE_VFS!"TRUE"}
#endif

/**
 * @brief   Number of file descriptors for each sandbox.
 */
#if !defined(SB_CFG_FD_NUM) || defined(__DOXYGEN__)
#define SB_CFG_FD_NUM                       ${doc.SB_CFG_FD_NUM!"12"}
#endif

/**
 * @brief   Enables a breakpoint in RAM-executed programs.
 * @note    This option is only usable during development with an attached
 *          debug, it causes a crash in stand-alone systems.
 */
#if !defined(SB_CFG_EXEC_DEBUG) || defined(__DOXYGEN__)
#define SB_CFG_EXEC_DEBUG                   ${doc.SB_CFG_EXEC_DEBUG!"FALSE"}
#endif

#endif  /* SBCONF_H */

/** @} */
