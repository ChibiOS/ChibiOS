/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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

#if CH_CFG_USE_MEMCHECKS == FALSE
#error "SandBox requires CH_CFG_USE_MEMCHECKS == TRUE"
#endif

#if PORT_USE_SYSCALL == FALSE
#error "SandBox requires PORT_USE_SYSCALL == TRUE"
#endif

#if (SB_CFG_NUM_REGIONS < 1) || (SB_CFG_NUM_REGIONS > 4)
#error "invalid SB_CFG_NUM_REGIONS value"
#endif

#if (PORT_SWITCHED_REGIONS_NUMBER > 0) &&                                   \
    (PORT_SWITCHED_REGIONS_NUMBER != SB_CFG_NUM_REGIONS)
#error "SB_CFG_NUM_REGIONS not matching PORT_SWITCHED_REGIONS_NUMBER"
#endif

#if (SB_CFG_ALARM_VRQ < 0) || (SB_CFG_ALARM_VRQ > 31)
#error "invalid SB_CFG_ALARM_VRQ value"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/


/**
 * @brief   Type of a sandbox object.
 */
typedef struct sb_class sb_class_t;

/**
 * @brief   Type of a mask of Virtual IRQs.
 */
typedef uint32_t sb_vrqmask_t;

/**
 * @brief   Type of a Virtual IRQs.
 */
typedef uint32_t sb_vrqnum_t;

#if (SB_CFG_ENABLE_VRQ == TRUE) || defined (__DOXYGEN__)
#include "sbvrq.h"
#endif

#if (SB_CFG_ENABLE_VIO == TRUE) || defined (__DOXYGEN__)
#include "sbvio.h"
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
 * @brief   Type of a sandbox memory region.
 */
typedef struct {
  /**
   * @brief   Associated memory area.
   */
  memory_area_t                 area;
  /**
   * @brief   Memory region in use.
   */
  bool                          used;
  /**
   * @brief   Writable memory range.
   */
  bool                          writeable;
} sb_memory_region_t;

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Type of a sandbox I/O structure.
 */
typedef struct {
  /**
   * @brief   VFS nodes associated to file descriptors.
   */
  vfs_node_c                    *vfs_nodes[SB_CFG_FD_NUM];
} sb_ioblock_t;
#endif

/**
 * @brief   Type of a sandbox configuration structure.
 */
typedef struct {
  /**
   * @brief   Thread-related configurations.
   */
  struct {
    /**
     * @brief   Thread name.
     */
    const char                  *name;
    /**
     * @brief   Thread working area.
     */
    void                        *wsp;
    /**
     * @brief   Working area size.
     */
    size_t                      size;
    /**
     * @brief   Thread priority.
     */
    tprio_t                     prio;
    /**
     * @brief   Thread priority while serving a VRQ.
     */
    tprio_t                     vrq_prio;
  } thread;
  /**
   * @brief   Memory region for code.
   * @note    It is used to locate the startup header.
   */
  uint32_t                      code_region;
  /**
   * @brief   Memory region for data and stack.
   * @note    It is used for initial PSP placement.
   */
  uint32_t                      data_region;
  /**
   * @brief   SandBox regions.
   * @note    The following memory regions are used only for pointers
   *          validation, not for MPU setup.
   */
  sb_memory_region_t            regions[SB_CFG_NUM_REGIONS];
#if (PORT_SWITCHED_REGIONS_NUMBER == SB_CFG_NUM_REGIONS) || defined(__DOXYGEN__)
  /**
   * @brief   MPU regions initialization values.
   * @note    Regions initialization values must be chosen to be
   *          consistent with the values in the "regions" field.
   */
  mpureg_t                      mpuregs[SB_CFG_NUM_REGIONS];
#endif
#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VFS driver associated to the sandbox as root.
   */
  vfs_driver_c                  *vfs_driver;
#endif
#if (SB_CFG_ENABLE_VIO == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VIO configuration associated to this sandbox.
   */
  const vio_conf_t              *vioconf;
#endif
} sb_config_t;

/**
 * @brief   Structure representing a sandbox object.
 */
struct sb_class {
  /**
   * @brief   Pointer to the sandbox configuration data.
   */
  const sb_config_t             *config;
  /**
   * @brief   Thread running in the sandbox.
   */
  thread_t                      *tp;
  /**
   * @brief   Pointer to the image header.
   */
  const sb_header_t             *sbhp;
  /**
   * @brief   Virtual timer used for alarms.
   */
  virtual_timer_t               alarm_vt;
#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Thread sending a message to the sandbox.
   */
  thread_t                      *msg_tp;
#endif
#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Sandbox events source.
   */
  event_source_t                es;
#endif
#if (SB_CFG_ENABLE_VRQ == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Global virtual IRQ status register.
   */
  uint32_t                      vrq_isr;
  /**
   * @brief   Mask of enabled virtual IRQ flags.
   */
  sb_vrqmask_t                  vrq_enmask;
  /**
   * @brief   Mask of pending virtual IRQ flags.
   */
  sb_vrqmask_t                  vrq_wtmask;
  /**
   * @brief   Reference to sh SB thread while waiting for VRQs.
   */
  thread_reference_t            vrq_trp;
  /**
   * @brief   Status flags associated to each VRQ.
   */
  uint32_t                      vrq_flags[32];
#endif
#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VFS bindings for Posix API.
   */
  sb_ioblock_t                  io;
#endif
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

#include "sbelf.h"
#include "sbposix.h"
#include "sbapi.h"
#include "sbhost.h"

#endif /* SBHOST_H */

/** @} */
