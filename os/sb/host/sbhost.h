/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

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
*/

/**
 * @file    sb/host/sbhost.h
 * @brief   ARMv7-M sandbox macros and structures.
 *
 * @addtogroup ARMV7M_SANDBOX
 * @{
 */

#ifndef SBHOST_H
#define SBHOST_H

#include "sberr.h"
#include "sbapi.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Magic numbers
 * @{
 */
#define SB_MAGIC1                           0xFE9154C0U
#define SB_MAGIC2                           0x0C4519EFU
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if PORT_USE_SYSCALL == FALSE
#error "sandbox requires PORT_USE_SYSCALL"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a syscall handler.
 */
typedef uint32_t (*port_syscall_t)(struct port_extctx *ctx);

/**
 * @brief   Type of a sandbox memory region.
 */
typedef struct {
  /**
   * @brief   Region 0 start address.
   */
  uint32_t                      r0_base;
  /**
   * @brief   Region 0 size.
   */
  uint32_t                      r0_end;
  /**
   * @brief   Region 2 start address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_base;
  /**
   * @brief   Region 2 end address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_end;
} sb_config_t;

/**
 * @brief   Type of a sandbox object.
 */
typedef struct {
  /**
   * @brief   Pointer to the sandbox configuration data.
   */
  const sb_config_t             *config;
  /**
   * @brief   Thread running in the sandbox.
   */
  thread_t                      *tp;
#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Thread sending a message to the sandbox.
   */
  thread_t                      *msg_tp;
#endif
#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  event_source_t                es;
#endif
} sb_class_t;

/**
 * @brief   Type of a sandbox applet headers.
 */
typedef struct {
  /**
   * @brief   Magic number 1.
   */
  uint32_t                      hdr_magic1;
  /**
   * @brief   Magic number 2.
   */
  uint32_t                      hdr_magic2;
  /**
   * @brief   Header size, inclusive of magic numbers.
   */
  uint32_t                      hdr_size;
  /**
   * @brief   Used-defined parameters, defaulted to zero.
   */
  uint32_t                      user;
  /**
   * @brief   Region 0 start address.
   */
  uint32_t                      r0_base;
  /**
   * @brief   Region 0 size.
   */
  uint32_t                      r0_end;
  /**
   * @brief   Region 2 start address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_base;
  /**
   * @brief   Region 2 end address.
   * @note    Zero if not used.
   */
  uint32_t                      r1_end;
} sb_header_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void port_syscall(struct port_extctx *ctxp, uint32_t n);
  void sbObjectInit(sb_class_t *sbcp);
  void sbStart(sb_class_t *sbcp, const sb_config_t *config);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#if (CH_CFG_USE_WAITEXIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Blocks the execution of the invoking thread until the sandbox
 *          thread terminates then the exit code is returned.
 * @pre     The configuration option @p CH_CFG_USE_WAITEXIT must be enabled in
 *          order to use this function.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @return              The exit code from the terminated thread.
 *
 * @api
 */
static inline msg_t sbWait(sb_class_t *sbcp) {

  return chThdWait(sbcp->tp);
}
#endif /* CH_CFG_USE_WAITEXIT == TRUE */

#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sends a message to a sandboxed thread.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @param[in] msg       message to be sent
 * @return              The returned message.
 * @retval MSG_RESET    Sandboxed thread API usage error, exchange aborted.
 *
 * @api
 */
static inline msg_t sbSendMessage(sb_class_t *sbcp, msg_t msg) {

  return chMsgSend(sbcp->tp, msg);
}
#endif /* CH_CFG_USE_MESSAGES == TRUE */

#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Adds a set of event flags directly to the specified sandbox.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @param[in] events    the events set to be ORed
 *
 * @iclass
 */
static inline void sbEvtSignalI(sb_class_t *sbcp, eventmask_t events) {

  chEvtSignalI(sbcp->tp, events);
}

/**
 * @brief   Adds a set of event flags directly to the specified sandbox.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @param[in] events    the events set to be ORed
 *
 * @api
 */
static inline void sbEvtSignal(sb_class_t *sbcp, eventmask_t events) {

  chEvtSignal(sbcp->tp, events);
}

/**
 * @brief   Returns the sandbox event source object.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @return              The pointer to the event source object.
 *
 * @xclass
 */
static inline event_source_t *sbGetEventSourceX(sb_class_t *sbcp) {

  return &sbcp->es;
}
#endif /* CH_CFG_USE_EVENTS == TRUE */

#endif /* SBHOST_H */

/** @} */
