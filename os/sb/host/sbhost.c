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

#include <string.h>

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

#if PORT_SWITCHED_REGIONS_NUMBER > 0
#if defined(PORT_ARCHITECTURE_ARM_V7M) || defined(PORT_ARCHITECTURE_ARM_V7ME)
static inline uint32_t get_alignment(uint32_t v) {

  return 1U << __CLZ(__RBIT(v));
}

static inline uint32_t get_next_po2(uint32_t v) {

  return 0x80000000U >> __CLZ(v);
}

/*
 * Translates generic SB regions in actual MPU settings, returns true if
 * the translation is not possible for some reason (usually required
 * alignments).
 */
static bool get_mpu_settings(const sb_memory_region_t *mrp,
                             port_mpureg_t *mpur) {
  uint32_t region_base, region_size, subregion_size;
  uint32_t area_base, area_size, area_end;
  uint32_t srd;

  /* Unused regions handling, MPU region is not enabled.*/
  if (sb_reg_is_unused(mrp)) {
    mpur->rbar = mpur->rasr = 0U;
    return false;
  }

  /* Area boundaries.*/
  area_base = (uint32_t)mrp->area.base;
  area_size = (uint32_t)mrp->area.size;
  area_end = area_base + area_size;

  /* Calculating the smallest region containing the requested area.
     The region size is the area size aligned to the next power of 2,
     region base is the area base aligned to the region size.*/
  region_size = get_next_po2(area_size);
  region_base = MEM_ALIGN_PREV(area_base, region_size);

  /* Checking if the area fits entirely in the calculated region, if not then
     region size is doubled.*/
  if (area_end <= region_base + region_size) {
    /* The area fits entirely in the region, calculating the sub-regions
       size.*/
    subregion_size = region_size / 8U;
  }
  else {
    /* It does not fit, doubling the region size, re-basing the region.*/
    region_size *= 2U;
    region_base = MEM_ALIGN_PREV(area_base, region_size);
    subregion_size = region_size / 8U;
  }

  /* Constraint, the area base address must be aligned to a sub-region
     boundary.*/
  if (!MEM_IS_ALIGNED(area_base, subregion_size)) {
    return true;
  }

  /* Constraint, the area size must also be aligned to a sub-region
     size.*/
  if (!MEM_IS_ALIGNED(area_size, subregion_size)) {
    return true;
  }

  /* Calculating the sub-regions disable mask.*/
  static const uint8_t srd_lower[] = {0x00U, 0x01U, 0x03U, 0x07U,
                                      0x0FU, 0x1FU, 0x3FU, 0x7FU};
  static const uint8_t srd_upper[] = {0x00U, 0x80U, 0xC0U, 0xE0U,
                                      0xF0U, 0xF8U, 0xFCU, 0xFEU};
  srd = (uint32_t)srd_lower[(area_base - region_base) / subregion_size] |
        (uint32_t)srd_upper[(region_base + region_size - area_end) / subregion_size];

  /* MPU registers settings.*/
  mpur->rbar = region_base;
  mpur->rasr = (srd << 8) | (__CLZ(__RBIT(region_size)) << 1) | MPU_RASR_ENABLE;

  /* Region attributes.*/
  if (sb_reg_is_writable(mrp)) {
    mpur->rasr |= MPU_RASR_ATTR_AP_RW_RW;
  }
  else {
    mpur->rasr |= MPU_RASR_ATTR_AP_RW_RO;
  }
  switch (sb_reg_get_type(mrp)) {
  case SB_REG_TYPE_DEVICE:
    /* Device type, execute and cached ignored.*/
    mpur->rasr |= MPU_RASR_ATTR_SHARED_DEVICE | MPU_RASR_ATTR_S;
    break;
  case SB_REG_TYPE_MEMORY:
    /* Memory type, there are various kinds.*/
    if (sb_reg_is_cacheable(mrp)) {
      if (sb_reg_is_writable(mrp)) {
        mpur->rasr |= MPU_RASR_ATTR_CACHEABLE_WB_WA | MPU_RASR_ATTR_S;
      }
      else {
        mpur->rasr |= MPU_RASR_ATTR_CACHEABLE_WT_NWA | MPU_RASR_ATTR_S;
      }
    }
    else {
      mpur->rasr |= MPU_RASR_ATTR_NON_CACHEABLE | MPU_RASR_ATTR_S;
    }
    if (!sb_reg_is_executable(mrp)) {
      mpur->rasr |= MPU_RASR_ATTR_XN;
    }
    break;
  default:
    return true;
  }

  return false;
}

#elif defined(PORT_ARCHITECTURE_ARM_V8M_MAINLINE)
static bool get_mpu_settings(const sb_memory_region_t *mrp,
                             port_mpureg_t *mpur) {
  uint32_t area_base, area_size, area_end;

  /* Unused regions handling, MPU region is not enabled.*/
  if (sb_reg_is_unused(mrp)) {
    mpur->rbar = mpur->rlar = 0U;
    return false;
  }

  /* Area boundaries.*/
  area_base = (uint32_t)mrp->area.base;
  area_size = (uint32_t)mrp->area.size;
  area_end = area_base + area_size;

  if (!MEM_IS_ALIGNED(area_base, 32U) || !MEM_IS_ALIGNED(area_end, 32U)) {
    return true;
  }


  /* MPU registers base settings.*/
  mpur->rbar = area_base | MPU_RBAR_SH_OUTER;
  mpur->rlar = area_end | MPU_RLAR_ENABLE;

  /* Region attributes.*/
  if (sb_reg_is_writable(mrp)) {
    mpur->rbar |= MPU_RBAR_AP_RW_RW;
  }
  else {
    mpur->rbar |= MPU_RBAR_AP_RW_RO;
  }
  switch (sb_reg_get_type(mrp)) {
  case SB_REG_TYPE_DEVICE:
    /* Device type, execute and cached ignored.*/
    mpur->rlar |= MPU_RLAR_ATTRINDX(3U);
    break;
  case SB_REG_TYPE_MEMORY:
    /* Memory type, there are various kinds.*/
    if (sb_reg_is_cacheable(mrp)) {
      if (sb_reg_is_writable(mrp)) {
        mpur->rlar |= MPU_RLAR_ATTRINDX(0U);
      }
      else {
        mpur->rlar |= MPU_RLAR_ATTRINDX(1U);
      }
    }
    else {
      mpur->rlar |= MPU_RLAR_ATTRINDX(2U);
    }
    if (!sb_reg_is_executable(mrp)) {
      mpur->rbar |= MPU_RBAR_XN;
    }
    break;
  default:
    return true;
  }

  return false;
}
#endif /* defined(PORT_ARCHITECTURE_ARM_V8M_MAINLINE) */
#endif /* PORT_SWITCHED_REGIONS_NUMBER > 0 */

static const sb_memory_region_t *sb_locate_data_region(sb_class_t *sbp) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (sb_reg_is_memory(rp) && sb_reg_is_writable(rp)) {
      return rp;
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return NULL;
}

static size_t sb_init_environment(sb_class_t *sbp, const memory_area_t *up,
                                  const char *argv[], const char *envp[]) {
  void *usp, *uargv, *uenvp;
  size_t totsize, envsize, argsize, parsize;
  int uargc, uenvc;

  /* Setting up an initial stack for the sandbox.*/
  usp = (up->base + up->size);

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

  /* Total pushed data size.*/
  totsize = envsize + argsize + parsize;

  /* Checking stack allocation.*/
  if (!chMemIsSpaceWithinX(up, usp, envsize + argsize + parsize)) {
    return (size_t)0;
  }

  /* Initializing stack.*/
  sb_strv_copy(envp, uenvp, uenvc);
  sb_strv_copy(argv, uargv, uargc);
  *((uint32_t *)usp + 3) = (uint32_t)0x55555555;
  *((uint32_t *)usp + 2) = (uint32_t)uenvp;
  *((uint32_t *)usp + 1) = (uint32_t)uargv;
  *((uint32_t *)usp + 0) = (uint32_t)uargc;

  /* Initial stack pointer is placed just below the environment data.*/
  sbp->u_psp    = (uint32_t)usp;
#if PORT_SAVE_PSPLIM == TRUE
  sbp->u_psplim = (uint32_t)up->base;
#endif

  return totsize;
}

static THD_FUNCTION(sb_unprivileged_trampoline, arg) {

  (void)arg;

  /* Jump with no return to the context saved at "u_psp". */
  asm volatile ("svc     #1");
  chSysHalt("svc");
}

static thread_t *sb_start_unprivileged(sb_class_t *sbp,
                                       stkline_t *stkbase,
                                       uint32_t u_pc) {
  thread_t *utp;
  struct port_extctx *ectxp;

#if SB_CFG_ENABLE_VRQ == TRUE
  /* Clearing VRQ-related information that could be leftovers of previous
     sandbox executions.*/
  memset((void *)&sbp->vrq, 0, sizeof sbp->vrq);
  sbp->vrq.isr = SB_VRQ_ISR_DISABLED;
#endif

  /* Creating a thread on the unprivileged handler.*/
  thread_descriptor_t td = {
    .name       = sbp->config->thread.name,
    .wbase      = stkbase,
    .wend       = stkbase + (SB_CFG_PRIVILEGED_STACK_SIZE / sizeof (stkline_t)),
    .prio       = sbp->config->thread.prio,
    .funcp      = sb_unprivileged_trampoline,
    .arg        = NULL,
    .owner      = NULL
  };
  utp = chThdSpawnSuspended(&sbp->thread, &td);

  /* The sandbox is the thread controller.*/
  utp->object = sbp;

#if PORT_SWITCHED_REGIONS_NUMBER > 0
  /* Regions for the unprivileged thread, will be set up on switch-in.*/
  for (unsigned i = 0U; i < PORT_SWITCHED_REGIONS_NUMBER; i++) {
    port_mpureg_t mpureg;

    if (get_mpu_settings(&sbp->config->regions[i], &mpureg)) {
      return NULL;
    }
    utp->ctx.regions[i] = mpureg;

  }
#endif

  /* Creating entry frame.*/
  sbp->u_psp -= sizeof (struct port_extctx); /* TODO */
  ectxp = (struct port_extctx *)sbp->u_psp;

  /* Initializing the unprivileged mode entry context, clearing
     all registers.*/
  memset((void *)ectxp, 0, sizeof (struct port_extctx));
  ectxp->pc    = u_pc;
  ectxp->xpsr  = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  ectxp->fpscr = FPU->FPDSCR;
#endif

  /* Starting the thread.*/
  return chThdStart(utp);
}

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
    if (sb_reg_is_memory(rp) && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return true;
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_write_range(sb_class_t *sbp, void *start, size_t size) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (sb_reg_is_memory(rp) && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return sb_reg_is_writable(rp);
    }
    rp++;
  } while (rp < &sbp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

size_t sb_check_string(sb_class_t *sbp, const char *s, size_t max) {
  const sb_memory_region_t *rp = &sbp->config->regions[0];

  do {
    if (sb_reg_is_memory(rp)) {
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
    if (sb_reg_is_memory(rp)) {
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

  memset((void *)sbp, 0, sizeof (sb_class_t));
  sbp->config = config;
  sbp->thread.state = CH_STATE_FINAL;
}

/**
 * @brief   Starts a static sandbox.
 * @details Region zero is assumed to be the code region and contain a correct
 *          sandbox header. The data region is searched into the sandbox
 *          configuration regions list, the fist memory writable region is
 *          selected for stack initialization.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] stkbase   base of the privileged stack area, the size is assumed
 *                      to be @p SB_CFG_PRIVILEGED_STACK_SIZE
 * @param[in] argv      array of parameters for the sandbox
 * @param[in] envp      array of environment variables for the sandbox
 * @return              The thread pointer.
 * @retval NULL         if the sandbox thread creation failed.
 */
thread_t *sbStart(sb_class_t *sbp, stkline_t *stkbase,
                  const char *argv[], const char *envp[]) {
  const sb_config_t *config = sbp->config;
  const sb_memory_region_t *codereg, *datareg;

  /* Region zero is assumed to be executable and contain the start header.*/
  codereg = &config->regions[0];
  sbp->sbhp = (const sb_header_t *)(void *)codereg->area.base;

  /* Checking header magic numbers.*/
  if ((sbp->sbhp->hdr_magic1 != SB_HDR_MAGIC1) ||
      (sbp->sbhp->hdr_magic2 != SB_HDR_MAGIC2)) {
    return NULL;
  }

  /* Checking header size and alignment.*/
  if (sbp->sbhp->hdr_size != sizeof (sb_header_t)) {
    return NULL;
  }

  /* Checking header entry point.*/
  if (!chMemIsSpaceWithinX(&codereg->area,
                           (const void *)sbp->sbhp->hdr_entry,
                           (size_t)2)) {
    return NULL;
  }

  /* Locating region for data, it could be also region zero.*/
  datareg = sb_locate_data_region(sbp);
  if (datareg == NULL) {
    return NULL;
  }

  /* Pushing arguments, environment variables and other startup information
     at the top of the data memory area.*/
  if (sb_init_environment(sbp, &datareg->area, argv, envp) == (size_t)0) {
    return NULL;
  }

  /* Everything OK, starting the unprivileged thread inside the sandbox.*/
  return sb_start_unprivileged(sbp, stkbase, sbp->sbhp->hdr_entry);
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

  return !chThdTerminatedX(&sbp->thread);
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
msg_t sbExec(sb_class_t *sbp, stkline_t *stkbase, const char *pathname,
             const char *argv[], const char *envp[]) {
  const sb_config_t *config = sbp->config;
  memory_area_t ma = config->regions[0].area;
  size_t totsize;
  msg_t ret;

  /* Pushing arguments, environment variables and other startup information
     at the top of the data memory area.*/
  totsize = sb_init_environment(sbp, &config->regions[0].area, argv, envp);
  if (totsize == (size_t)0) {
    return CH_RET_ENOMEM;
  }

  /* Adjusting the size of the memory area object, we don't want the loaded
     elf file to overwrite the environment data.*/
  ma.size -= totsize;

  /* Loading sandbox code into the specified memory area.*/
  ret = sbElfLoadFile(config->vfs_driver, pathname, &ma);
  CH_RETURN_ON_ERROR(ret);

  /* Header location.*/
  sbp->sbhp = (const sb_header_t *)(void *)ma.base;

  /* Checking header magic numbers.*/
  if ((sbp->sbhp->hdr_magic1 != SB_HDR_MAGIC1) ||
      (sbp->sbhp->hdr_magic2 != SB_HDR_MAGIC2)) {
    return CH_RET_ENOEXEC;
  }

  /* Checking header size.*/
  if (sbp->sbhp->hdr_size != sizeof (sb_header_t)) {
    return CH_RET_ENOEXEC;
  }

  /* Checking header entry point.*/
  if (!chMemIsSpaceWithinX(&ma, (const void *)sbp->sbhp->hdr_entry, (size_t)2)) {
    return CH_RET_EFAULT;
  }

  /* Everything OK, starting the unprivileged thread inside the sandbox.*/
  if (sb_start_unprivileged(sbp, stkbase, sbp->sbhp->hdr_entry) == NULL) {
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

  if (sbp->thread.state == CH_STATE_FINAL) {
    return MSG_RESET;
  }

  msg = chThdWait(&sbp->thread);

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
  __ch_msg_insert(&sbp->thread.msgqueue, ctp);
  if (sbp->thread.state == CH_STATE_WTMSG) {
    (void) chSchReadyI(&sbp->thread);
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
