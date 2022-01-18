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
 * @file    sb/host/sbhost.c
 * @brief   ARM SandBox host code.
 *
 * @addtogroup ARM_SANDBOX
 * @{
 */

#include "ch.h"
#include "sb.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define PUSHSPACE(sp, n) do {                                               \
  (sp) = (void *)((uint8_t *)(sp) - (n));                                   \
} while (false)

#define PUSHTYPE(type, sp, p) do {                                          \
  PUSHSPACE(sp, sizeof (type));                                             \
  *(type *)(void *)(sp) = (type)(p);                                        \
} while (false)

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Global sandbox managed state variable.
 */
sb_t sb;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

size_t sb_strv_getsize(const char *v[], int *np) {
  const char* s;
  size_t size;
  int n;

  size = sizeof (const char *);
  if (v != NULL) {
    n = 0;
    while ((s = *v) != NULL) {
      size += sizeof (const char *) + strlen(s) + (size_t)1;
      n++;
      v++;
    }

    if (np != NULL) {
      *np = n;
    }
  }

  return MEM_ALIGN_NEXT(size, MEM_NATURAL_ALIGN);
}

void sb_strv_copy(const char *sp[], void *dp, int n) {
  char **vp;
  char *cp;
  int i;

  vp = (char **)dp;
  cp = (char *)dp + ((n + 1) * sizeof (char *));
  for (i = 0; i < n; i++) {
    const char *ss = sp[i];
    *(vp + i) = cp;
    while ((*cp++ = *ss++) != '\0') {
      /* Copy.*/
    }
  }
  *(vp + n) = NULL;
}

bool sb_is_valid_read_range(sb_class_t *sbp, const void *start, size_t size) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (rp->used && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return true;
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_write_range(sb_class_t *sbp, void *start, size_t size) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (rp->used && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return rp->writeable;
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

size_t sb_check_string(sb_class_t *sbp, const char *s, size_t max) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (rp->used) {
      size_t n = chMemIsStringWithinX(&rp->area, s, max);
      if (n > (size_t)0) {
        return n;
      }
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return (size_t)0;
}

size_t sb_check_pointers_array(sb_class_t *sbp, const void *pp[], size_t max) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (rp->used) {
      size_t an = chMemIsPointersArrayWithinX(&rp->area, pp, max);
      if (an > (size_t)0) {
        return an;
      }
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return (size_t)0;
}

size_t sb_check_strings_array(sb_class_t *sbp, const char *pp[], size_t max) {
  const char *s;
  size_t n;

  n = sb_check_pointers_array(sbp, (const void **)pp, max);
  if (n > (size_t)0) {
    while ((s = *pp++) != NULL) {
      size_t sn;

      sn = sb_check_string(sbp, s, max - n);
      if (sn == (size_t)0) {
        return (size_t)0;
      }

      n += sn;
    }
  }

  return n;
}

/**
 * @brief   Sandbox object initialization.
 *
 * @param[out] sbp      pointer to the sandbox object
 * @param[in] config    pointer to the sandbox configuration
 *
 * @init
 */
void sbObjectInit(sb_class_t *sbp, const sb_config_t *config) {

  sbp->config = config;
  sbp->tp     = NULL;
#if CH_CFG_USE_MESSAGES == TRUE
  sbp->msg_tp = NULL;
#endif
#if CH_CFG_USE_EVENTS == TRUE
  chEvtObjectInit(&sbp->es);
#endif
}

/**
 * @brief   Starts a sandboxed thread.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] name      name to be assigned to the thread
 * @param[out] wsp      pointer to a working area dedicated to the thread stack
 * @param[in] size      size of the working area
 * @param[in] prio      the priority level for the new thread
 * @param[in] argv      array of parameters for the sandbox
 * @param[in] envp      array of environment variables for the sandbox
 * @return              The thread pointer.
 * @retval NULL         if the sandbox thread creation failed.
 */
thread_t *sbStartThread(sb_class_t *sbp, const char *name,
                        void *wsp, size_t size, tprio_t prio,
                        const char *argv[], const char *envp[]) {
  thread_t *utp;
  const sb_header_t *sbhp;
  const sb_config_t *config = sbp->config;
  void *usp, *uargv, *uenvp;
  size_t envsize, argsize, parsize;
  int uargc, uenvc;

  /* Header location.*/
  sbhp = (const sb_header_t *)(void *)config->regions[config->code_region].area.base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_HDR_MAGIC1) ||
      (sbhp->hdr_magic2 != SB_HDR_MAGIC2)) {
    return NULL;
  }

  /* Checking header size and alignment.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    return NULL;
  }

  /* Checking header entry point.*/
  if (!chMemIsSpaceWithinX(&config->regions[config->code_region].area,
                           (const void *)sbhp->hdr_entry,
                           (size_t)2)) {
    return NULL;
  }

  /* Setting up an initial stack for the sandbox.*/
  usp = (config->regions[config->data_region].area.base +
         config->regions[config->data_region].area.size);

  /* Allocating space for environment variables.*/
  envsize = sb_strv_getsize(envp, &uenvc);
  PUSHSPACE(usp, envsize);
  uenvp = usp;

  /* Allocating space for arguments.*/
  argsize = sb_strv_getsize(argv, &uargc);
  PUSHSPACE(usp, argsize);
  uargv = usp;

  /* Allocating space for parameters.*/
  if (MEM_IS_ALIGNED(usp, PORT_STACK_ALIGN)) {
    parsize = sizeof (uint32_t) * 4;
  }
  else {
    parsize = sizeof (uint32_t) * 5;
  }
  PUSHSPACE(usp, parsize);

  /* Checking stack allocation.*/
  if (!chMemIsSpaceWithinX(&config->regions[config->data_region].area,
                           usp, envsize + argsize + parsize)) {
    return NULL;
  }

  /* Initializing stack.*/
  sb_strv_copy(envp, uenvp, uenvc);
  sb_strv_copy(argv, uargv, uargc);
  *((uint32_t *)usp + 4) = (uint32_t)0x55555555;
  *((uint32_t *)usp + 2) = (uint32_t)uenvp;
  *((uint32_t *)usp + 1) = (uint32_t)uargv;
  *((uint32_t *)usp + 0) = (uint32_t)uargc;

  unprivileged_thread_descriptor_t utd = {
    .name       = name,
    .wbase      = (stkalign_t *)wsp,
    .wend       = (stkalign_t *)wsp + (size / sizeof (stkalign_t)),
    .prio       = prio,
    .u_pc       = sbhp->hdr_entry,
    .u_psp      = (uint32_t)usp,
    .arg        = (void *)sbp
  };
#if PORT_SWITCHED_REGIONS_NUMBER > 0
  for (unsigned i = 0U; i < PORT_SWITCHED_REGIONS_NUMBER; i++) {
    utd.regions[i] = config->mpuregs[i];
  }
#endif

  utp = chThdCreateUnprivileged(&utd);

  /* For messages exchange.*/
  sbp->tp      = utp;

  return utp;
}

/**
 * @brief   Verifies if the sandbox thread is running.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @return              The thread status.
 *
 * @api
 */
bool sbIsThreadRunningX(sb_class_t *sbp) {

  if (sbp->tp == NULL) {
    return false;
  }

  return !chThdTerminatedX(sbp->tp);
}

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Execute an elf file within a sandbox.
 * @note    The file is loaded into region zero of the sandbox which is
 *          assumed to be used for both code and data, extra regions are
 *          not touched by this function.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] pathname  file to be executed
 * @param[in] argv      arguments to be passed to the sandbox
 * @param[in] envp      environment variables to be passed to the sandbox
 * @return              The operation result.
 *
 * @api
 */
msg_t sbExec(sb_class_t *sbp, const char *pathname,
             void *wsp, size_t size, tprio_t prio,
             const char *argv[], const char *envp[]) {
  const sb_config_t *config = sbp->config;
  memory_area_t ma = config->regions[0].area;
  const sb_header_t *sbhp;
  msg_t ret;
  void *usp, *uargv, *uenvp;
  size_t envsize, argsize, parsize;
  int uargc, uenvc;

  /* Setting up an initial stack for the sandbox.*/
  usp = (void *)(config->regions[0].area.base + config->regions[0].area.size);

  /* Allocating space for environment variables.*/
  envsize = sb_strv_getsize(envp, &uenvc);
  PUSHSPACE(usp, envsize);
  uenvp = usp;

  /* Allocating space for arguments.*/
  argsize = sb_strv_getsize(argv, &uargc);
  PUSHSPACE(usp, argsize);
  uargv = usp;

  /* Allocating space for parameters.*/
  if (MEM_IS_ALIGNED(usp, PORT_STACK_ALIGN)) {
    parsize = sizeof (uint32_t) * 4;
  }
  else {
    parsize = sizeof (uint32_t) * 5;
  }
  PUSHSPACE(usp, parsize);

  /* Checking stack allocation.*/
  if (!chMemIsSpaceWithinX(&config->regions[0].area,
                           usp, envsize + argsize + parsize)) {
    return CH_RET_ENOMEM;
  }

  /* Adjusting the size of the memory area object, we don't want the loaded
     elf file to overwrite the initialized stack.*/
  ma.size -= envsize + argsize + parsize;

  /* Initializing stack.*/
  sb_strv_copy(envp, uenvp, uenvc);
  sb_strv_copy(argv, uargv, uargc);
  *((uint32_t *)usp + 4) = (uint32_t)0x55555555;
  *((uint32_t *)usp + 2) = (uint32_t)uenvp;
  *((uint32_t *)usp + 1) = (uint32_t)uargv;
  *((uint32_t *)usp + 0) = (uint32_t)uargc;

  /* Loading sandbox code into the specified memory area.*/
  ret = sbElfLoadFile(config->vfs_driver, pathname, &ma);
  CH_RETURN_ON_ERROR(ret);

  /* Header location.*/
  sbhp = (const sb_header_t *)(void *)ma.base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_HDR_MAGIC1) ||
      (sbhp->hdr_magic2 != SB_HDR_MAGIC2)) {
    return CH_RET_ENOEXEC;
  }

  /* Checking header size.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    return CH_RET_ENOEXEC;
  }

  /* Checking header entry point.*/
  if (!chMemIsSpaceWithinX(&ma, (const void *)sbhp->hdr_entry, (size_t)2)) {
    return CH_RET_EFAULT;
  }

  /* Everything OK, starting the unprivileged thread inside the sandbox.*/
  unprivileged_thread_descriptor_t utd = {
    .name       = pathname,
    .wbase      = (stkalign_t *)wsp,
    .wend       = (stkalign_t *)wsp + (size / sizeof (stkalign_t)),
    .prio       = prio,
    .u_pc       = sbhp->hdr_entry,
    .u_psp      = (uint32_t)usp,
    .arg        = (void *)sbp
  };
#if PORT_SWITCHED_REGIONS_NUMBER > 0
  for (unsigned i = 0U; i < PORT_SWITCHED_REGIONS_NUMBER; i++) {
    utd.regions[i] = config->mpuregs[i];
  }
#endif

  sbp->tp = chThdCreateUnprivileged(&utd);

  if (sbp->tp == NULL) {
    return CH_RET_ENOMEM;
  }

  return CH_RET_SUCCESS;
}

/**
 * @brief   Registers a file descriptor on a sandbox.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] fd        file descriptor to be assigned
 * @param[in] np        VFS node to be registered on the file descriptor
 *
 * @api
 */
void sbRegisterDescriptor(sb_class_t *sbp, int fd, vfs_node_c *np) {

  chDbgAssert(sb_is_available_descriptor(&sbp->io, fd), "invalid file descriptor");

  sbp->io.vfs_nodes[fd]  = np;
}
#endif

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
msg_t sbWaitThread(sb_class_t *sbp) {
  msg_t msg;

  if (sbp->tp == NULL) {
    return MSG_RESET;
  }

  msg = chThdWait(sbp->tp);
  sbp->tp = NULL;

  return msg;
}
#endif

#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sends a message to a sandboxed thread.
 *
 * @param[in] sbp       pointer to the sandbox object
 * @param[in] msg       message to be sent
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The returned message.
 * @retval MSG_TIMEOUT  if a timeout occurred.
 * @retval MSG_RESET    if the exchange aborted, sandboxed thread API usage
 *                      error.
 *
 * @api
 */
msg_t sbSendMessageTimeout(sb_class_t *sbp,
                           msg_t msg,
                           sysinterval_t timeout) {
  thread_t *ctp = __sch_get_currthread();

  chDbgCheck(sbp != NULL);

  chSysLock();

  /* Sending the message.*/
  ctp->u.sentmsg = msg;
  __ch_msg_insert(&sbp->tp->msgqueue, ctp);
  if (sbp->tp->state == CH_STATE_WTMSG) {
    (void) chSchReadyI(sbp->tp);
  }
  msg = chSchGoSleepTimeoutS(CH_STATE_SNDMSGQ, timeout);

  /* If a timeout occurred while the boxed thread already received the message
     then this thread needs to "unregister" as sender, the boxed error will
     get SB_ERR_EBUSY when/if trying to reply.*/
  if ((msg == MSG_TIMEOUT) && (sbp->msg_tp == ctp)) {
    sbp->msg_tp = NULL;
  }

  chSysUnlock();

  return msg;
}
#endif /* CH_CFG_USE_MESSAGES == TRUE */

/** @} */
