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

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

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
  void sb_strv_copy(const char *sp[], void *dp, int n);
  bool sb_is_valid_read_range(sb_class_t *sbp, const void *start, size_t size);
  bool sb_is_valid_write_range(sb_class_t *sbp, void *start, size_t size);
  size_t sb_check_string(sb_class_t *sbp, const char *s, size_t max);
  size_t sb_check_pointers_array(sb_class_t *sbp, const void *pp[], size_t max);
  size_t sb_check_strings_array(sb_class_t *sbp, const char *pp[], size_t max);
  void sbObjectInit(sb_class_t *sbp, const sb_config_t *config);
  thread_t *sbStartThread(sb_class_t *sbp, const char *name,
                          void *wsp, size_t size, tprio_t prio,
                          const char *argv[], const char *envp[]);
  bool sbIsThreadRunningX(sb_class_t *sbp);
#if SB_CFG_ENABLE_VFS == TRUE
  msg_t sbExec(sb_class_t *sbp, const char *pathname,
               void *wsp, size_t size, tprio_t prio,
               const char *argv[], const char *envp[]);
  void sbRegisterDescriptor(sb_class_t *sbp, int fd, vfs_node_c *np);
#endif
#if CH_CFG_USE_WAITEXIT == TRUE
  msg_t sbWaitThread(sb_class_t *sbp);
#endif
#if CH_CFG_USE_MESSAGES == TRUE
  msg_t sbSendMessageTimeout(sb_class_t *sbp,
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
 * @param[in] sbp      pointer to the sandbox object
 * @param[in] msg       message to be sent
 * @return              The returned message.
 * @retval MSG_RESET    if the exchange aborted, sandboxed thread API usage
 *                      error.
 *
 * @api
 */
static inline msg_t sbSendMessage(sb_class_t *sbp, msg_t msg) {

  return sbSendMessageTimeout(sbp, msg, TIME_INFINITE);
}
#endif /* CH_CFG_USE_MESSAGES == TRUE */

#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Adds a set of event flags directly to the specified sandbox.
 *
 * @param[in] sbp      pointer to the sandbox object
 * @param[in] events    the events set to be ORed
 *
 * @iclass
 */
static inline void sbEvtSignalI(sb_class_t *sbp, eventmask_t events) {

  chEvtSignalI(sbp->tp, events);
}

/**
 * @brief   Adds a set of event flags directly to the specified sandbox.
 *
 * @param[in] sbp      pointer to the sandbox object
 * @param[in] events    the events set to be ORed
 *
 * @api
 */
static inline void sbEvtSignal(sb_class_t *sbp, eventmask_t events) {

  chEvtSignal(sbp->tp, events);
}

/**
 * @brief   Returns the sandbox event source object.
 *
 * @param[in] sbp      pointer to the sandbox object
 * @return              The pointer to the event source object.
 *
 * @xclass
 */
static inline event_source_t *sbGetEventSourceX(sb_class_t *sbp) {

  return &sbp->es;
}
#endif /* CH_CFG_USE_EVENTS == TRUE */

#endif /* SBHOST_H */

/** @} */
