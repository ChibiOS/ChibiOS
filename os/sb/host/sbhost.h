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
 * @name    Memory regions types
 * @{
 */
#define SB_REG_TYPE_MASK        3U
#define SB_REG_TYPE_UNUSED      0U
#define SB_REG_TYPE_MEMORY      1U
#define SB_REG_TYPE_DEVICE      2U

/* Extra attributes regions.*/
#define SB_REG_ATTR_WRITABLE    (1U << 8)
#define SB_REG_ATTR_EXECUTABLE  (1U << 9)   /* Only for SB_REG_TYPE_MEMORY.*/
#define SB_REG_ATTR_CACHEABLE   (1U << 10)  /* Only for SB_REG_TYPE_MEMORY.*/

/* Derived region types.*/
#define SB_REG_IS_UNUSED        (SB_REG_TYPE_UNUSED)
#define SB_REG_IS_CODE          (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_CACHEABLE |                    \
                                 SB_REG_ATTR_EXECUTABLE)
#define SB_REG_IS_DATA          (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_CACHEABLE |                    \
                                 SB_REG_ATTR_WRITABLE)
#define SB_REG_IS_NOCACHE_DATA  (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_WRITABLE)
#define SB_REG_IS_CODE_AND_DATA (SB_REG_TYPE_MEMORY |                       \
                                 SB_REG_ATTR_CACHEABLE |                    \
                                 SB_REG_ATTR_EXECUTABLE |                   \
                                 SB_REG_ATTR_WRITABLE)
#define SB_REG_IS_DEVICE        (SB_REG_TYPE_DEVICE |                       \
                                 SB_REG_ATTR_WRITABLE)
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

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Sandbox stack area declaration.
 *
 * @param[in] name      name of the sandbox stack area
 */
#define SB_STACK(name) THD_STACK(name, SB_CFG_PRIVILEGED_STACK_SIZE);

/**
 * @name    Memory regions attributes checks
 * @{
 */
#define sb_reg_get_type(r)      ((r)->attributes & SB_REG_TYPE_MASK)
#define sb_reg_is_unused(r)     (bool)(sb_reg_get_type(r) == SB_REG_TYPE_UNUSED)
#define sb_reg_is_device(r)     (bool)(sb_reg_get_type(r) == SB_REG_TYPE_DEVICE)
#define sb_reg_is_memory(r)     (bool)(sb_reg_get_type(r) == SB_REG_TYPE_MEMORY)
#define sb_reg_is_writable(r)   (bool)(((r)->attributes & SB_REG_ATTR_WRITABLE) != 0U)
#define sb_reg_is_executable(r) (bool)(((r)->attributes & SB_REG_ATTR_EXECUTABLE) != 0U)
#define sb_reg_is_cacheable(r)     (bool)(((r)->attributes & SB_REG_ATTR_CACHEABLE) != 0U)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern sb_t sb;

#ifdef __cplusplus
extern "C" {
#endif
  size_t sb_strv_getsize(const char *v[], int *np);
  void sb_strv_copy(const char *sp[], void *dp, int n);
  bool sb_is_valid_read_range(sb_class_t *sbp, const void *start, size_t size);
  bool sb_is_valid_write_range(sb_class_t *sbp, void *start, size_t size);
  size_t sb_check_string(sb_class_t *sbp, const char *s, size_t max);
  size_t sb_check_pointers_array(sb_class_t *sbp, const void *pp[], size_t max);
  size_t sb_check_strings_array(sb_class_t *sbp, const char *pp[], size_t max);
  void sbObjectInit(sb_class_t *sbp, const sb_config_t *config);
  thread_t *sbStart(sb_class_t *sbp, stkline_t *stkbase,
                    const char *argv[], const char *envp[]);
  bool sbIsThreadRunningX(sb_class_t *sbp);
#if SB_CFG_ENABLE_VFS == TRUE
  msg_t sbExec(sb_class_t *sbp, stkline_t *stkbase, const char *pathname,
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

#if (CH_CFG_USE_WAITEXIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Blocks the execution of the invoking thread until the specified
 *          sandbox thread terminates then the exit code is returned.
 * @pre     The configuration option @p CH_CFG_USE_WAITEXIT must be enabled in
 *          order to use this function.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @return              The exit code from the terminated sandbox thread.
 * @retval MSG_RESET    Sandbox thread not started.
 *
 * @api
 */
static inline msg_t sbWait(sb_class_t *sbp) {
  msg_t msg;

  msg = chThdWait(&sbp->thread);

  return msg;
}
#endif

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

  chEvtSignalI(&sbp->thread, events);
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

  chEvtSignal(&sbp->thread, events);
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

  return &sbp->base.es;
}
#endif /* CH_CFG_USE_EVENTS == TRUE */

#endif /* SBHOST_H */

/** @} */
