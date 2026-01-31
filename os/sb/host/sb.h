/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/host/sb.h
 * @brief   ARM SandBox macros and structures.
 *
 * @addtogroup ARM_SANDBOX
 * @{
 */

#ifndef SB_H
#define SB_H

#include "hal.h"
#include "vfs.h"
#include "errcodes.h"

#include "sbhdr.h"
#include "sbsysc.h"
#include "sbconf.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   ChibiOS/SB identification macro.
 */
#define __CHIBIOS_SB__

/**
 * @brief   Stable release flag.
 */
#define CH_SB_STABLE            0

/**
 * @name    ChibiOS/SE version identification
 * @{
 */
/**
 * @brief   Safety Extensions version string.
 */
#define CH_SB_VERSION           "3.0.0"

/**
 * @brief   Safety Extensions version major number.
 */
#define CH_SB_MAJOR             3

/**
 * @brief   Safety Extensions version minor number.
 */
#define CH_SB_MINOR             0

/**
 * @brief   Safety Extensions version patch number.
 */
#define CH_SB_PATCH             0
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(__CHIBIOS_SB_CONF__)
#error "missing or wrong configuration file"
#endif

#if !defined(__CHIBIOS_SB_CONF_VER_3_0__)
#error "obsolete or unknown configuration file"
#endif

/* Checks on configuration options.*/

#if !defined(SB_CFG_PRIVILEGED_STACK_SIZE) || defined(__DOXYGEN__)
#error "SB_CFG_PRIVILEGED_STACK_SIZE not defined in sbconf.h"
#endif

#if !defined(SB_CFG_NUM_REGIONS) || defined(__DOXYGEN__)
#error "SB_CFG_NUM_REGIONS not defined in sbconf.h"
#endif

#if !defined(SB_CFG_ENABLE_VRQ) || defined(__DOXYGEN__)
#error "SB_CFG_ENABLE_VRQ not defined in sbconf.h"
#endif

#if !defined(SB_CFG_ALARM_VRQ) || defined(__DOXYGEN__)
#error "SB_CFG_ALARM_VRQ not defined in sbconf.h"
#endif

#if !defined(SB_CFG_ENABLE_VIO) || defined(__DOXYGEN__)
#error "SB_CFG_ENABLE_VIO not defined in sbconf.h"
#endif

#if !defined(SB_CFG_ENABLE_VFS) || defined(__DOXYGEN__)
#error "SB_CFG_ENABLE_VFS not defined in sbconf.h"
#endif

#if !defined(SB_CFG_FD_NUM) || defined(__DOXYGEN__)
#error "SB_CFG_FD_NUM not defined in sbconf.h"
#endif

/* License checks.*/
#if !defined(CH_CUSTOMER_LIC_SB) || !defined(CH_LICENSE_FEATURES)
#error "malformed chlicense.h"
#endif

#if CH_CUSTOMER_LIC_SB == FALSE
#error "ChibiOS/SB not licensed"
#endif

#if (CH_LICENSE_FEATURES != CH_FEATURES_FULL) &&                            \
    (CH_LICENSE_FEATURES != CH_FEATURES_INTERMEDIATE) &&                    \
    (CH_LICENSE_FEATURES != CH_FEATURES_BASIC)
#error "invalid CH_LICENSE_FEATURES setting"
#endif

#if CH_LICENSE_FEATURES != CH_FEATURES_FULL
#error "ChibiOS/SB insufficient features level"
#endif

#if CH_CFG_ST_RESOLUTION != 32
#error "SandBox requires CH_CFG_ST_RESOLUTION == 32"
#endif

#if CH_CFG_INTERVALS_SIZE != 32
#error "SandBox requires CH_CFG_INTERVALS_SIZE == 32"
#endif

#if CH_CFG_USE_WAITEXIT != TRUE
#error "SandBox requires CH_CFG_USE_WAITEXIT == TRUE"
#endif

#if CH_CFG_USE_DYNAMIC != TRUE
#error "SandBox requires CH_CFG_USE_DYNAMIC == TRUE"
#endif

#if CH_CFG_USE_MEMCHECKS == FALSE
#error "SandBox requires CH_CFG_USE_MEMCHECKS == TRUE"
#endif

#if (!defined(PORT_ARCHITECTURE_ARM_V7M) &&                                 \
     !defined(PORT_ARCHITECTURE_ARM_V7ME) &&                                \
     !defined(PORT_ARCHITECTURE_ARM_V8M_MAINLINE)) ||                       \
     !defined(PORT_USE_SYSCALL)
#error "unsupported port"
#endif

#if PORT_USE_SYSCALL == FALSE
#error "SandBox requires PORT_USE_SYSCALL == TRUE"
#endif

#if defined(PORT_ARCHITECTURE_ARM_V8M_MAINLINE) &&                          \
    (PORT_MPU_INITIALIZE == FALSE)
/* Because MPU_MAIRx registers are initialized by the port layer in this
   architecture.*/
#error "SandBox requires PORT_MPU_INITIALIZE == TRUE"
#endif

#if (SB_CFG_PRIVILEGED_STACK_SIZE < 64) ||                                  \
    ((SB_CFG_PRIVILEGED_STACK_SIZE % PORT_STACK_ALIGN) != 0)
#error "invalid SB_CFG_PRIVILEGED_STACK_SIZE value"
#endif

#if (SB_CFG_NUM_REGIONS < 1) || (SB_CFG_NUM_REGIONS > 4)
#error "invalid SB_CFG_NUM_REGIONS value"
#endif

#if (SB_CFG_ALARM_VRQ < 0) || (SB_CFG_ALARM_VRQ > 31)
#error "invalid SB_CFG_ALARM_VRQ value"
#endif

#if (PORT_SWITCHED_REGIONS_NUMBER > 0) &&                                  \
    (SB_CFG_NUM_REGIONS < PORT_SWITCHED_REGIONS_NUMBER)
#error "SB_CFG_NUM_REGIONS must be >= PORT_SWITCHED_REGIONS_NUMBER"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a sandbox object.
 */
typedef struct sb_class sb_class_t;

#include "sbregions.h"
#include "sbapi.h"

#if (SB_CFG_ENABLE_VRQ == TRUE) || defined (__DOXYGEN__)
#include "sbvrq.h"
#endif

#if (SB_CFG_ENABLE_VIO == TRUE) || defined (__DOXYGEN__)
#include "sbvio.h"
#endif

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
#include "sbposix.h"
#endif

/**
 * @brief   Type of a sandbox manager global structure.
 */
typedef struct {
#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Event source for sandbox termination.
   */
  event_source_t                termination_es;
#endif
} sb_t;

/**
 * @brief   Structure representing a sandbox object.
 */
struct sb_class {
  /**
   * @brief   Indicates that the sandbox is dynamically allocated from heap.
   */
  bool                          is_dynamic;
  /**
   * @brief   SandBox regions.
   * @note    Region zero is always used for code execution. The data
   *          region is assumed to be the first region in the list with
   *          attribute @p SB_REG_ATTR_WRITABLE.
   */
  sb_memory_region_t            regions[SB_CFG_NUM_REGIONS];
  /**
   * @brief   Pointer to the memory area used for data and stack.
   * @note    The user stack pointer is assumed to stay within this area,
   *          relocating it to other areas is forbidden, the base of this
   *          area is used for user-PSPLIM on ARMv8-M architecture.
   */
  const memory_area_t           *u_data;
  /**
   * @brief   Saved unprivileged PSP position.
   */
  uint32_t                      u_psp;
#if (SB_CFG_ENABLE_VRQ == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Cached VRQ handler entry address.
   */
  uint32_t                      vrq_entry;
#endif
#if (SB_CFG_ENABLE_VIO == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VIO configuration associated to this sandbox.
   */
  const vio_conf_t              *vioconf;
#endif
  /**
   * @brief   Base API-related fields.
   */
  sb_apiblock_t                 base;
#if (SB_CFG_ENABLE_VRQ == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VRQ-related fields.
   */
  sb_vrqblock_t                 vrq;
#endif
#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Posix IO-related fields.
   */
  sb_ioblock_t                  io;
#endif
  /**
   * @brief   Thread running in the sandbox.
   */
  thread_t                      thread;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#include "sbhost.h"
#include "sbsyscall.h"
#include "sbelf.h"

#endif /* SBHOST_H */

/** @} */
