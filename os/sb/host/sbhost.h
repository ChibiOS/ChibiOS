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
 * @file    sb/host/sbhost.h
 * @brief   ARM SandBox host macros and structures.
 *
 * @addtogroup ARM_SANDBOX
 * @{
 */

#ifndef SBHOST_H
#define SBHOST_H

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

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a sandbox binary image header.
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
   * @brief   Entry point address.
   */
  uint32_t                      hdr_entry;
  /**
   * @brief   Used-defined parameters, defaulted to zero.
   */
  uint32_t                      user[4];
} sb_header_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern sb_t sb;

#ifdef __cplusplus
extern "C" {
#endif
  void port_syscall(struct port_extctx *ctxp, uint32_t n);
  size_t sb_strv_getsize(const char *v[], int *np);
  bool sb_is_valid_read_range(sb_class_t *sbcp, const void *start, size_t size);
  bool sb_is_valid_write_range(sb_class_t *sbcp, void *start, size_t size);
  size_t sb_check_string(sb_class_t *sbcp, const char *s, size_t max);
  size_t sb_check_pointers_array(sb_class_t *sbcp, const void *pp[], size_t max);
  size_t sb_check_strings_array(sb_class_t *sbcp, const char *pp[], size_t max);
  void sbObjectInit(sb_class_t *sbcp, const sb_config_t *config);
  thread_t *sbStartThread(sb_class_t *sbcp, const char *name,
                          void *wsp, size_t size, tprio_t prio,
                          const char *argv[], const char *envp[]);
  bool sbIsThreadRunningX(sb_class_t *sbcp);
#if CH_CFG_USE_WAITEXIT == TRUE
  msg_t sbWaitThread(sb_class_t *sbcp);
#endif
#if CH_CFG_USE_MESSAGES == TRUE
  msg_t sbSendMessageTimeout(sb_class_t *sbcp,
                             msg_t msg,
                             sysinterval_t timeout);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Initialization of the sandbox host.
 *
 * @init
 */
static inline void sbHostInit(void) {

#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  chEvtObjectInit(&sb.termination_es);
#endif
}

#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sends a message to a sandboxed thread.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @param[in] msg       message to be sent
 * @return              The returned message.
 * @retval MSG_RESET    if the exchange aborted, sandboxed thread API usage
 *                      error.
 *
 * @api
 */
static inline msg_t sbSendMessage(sb_class_t *sbcp, msg_t msg) {

  return sbSendMessageTimeout(sbcp, msg, TIME_INFINITE);
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
