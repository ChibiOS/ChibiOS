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

  if (v <= 1U) {
    return 1U;
  }

  return 1U << (32U - __CLZ(v - 1U));
}

static size_t get_mpu_alignment(memory_area_t *map) {
  size_t basealign, sizealign;

  sizealign = get_next_po2(map->size) / 4U;
  map->size = MEM_ALIGN_NEXT(map->size, sizealign);
  basealign = get_next_po2(map->size);

  return basealign;
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
  mpur->rasr = (srd << 8) | ((__CLZ(__RBIT(region_size)) -1U) << 1) | MPU_RASR_ENABLE;

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
static size_t get_mpu_alignment(memory_area_t *map) {

  map->size = MEM_ALIGN_NEXT(map->size, 32U);

  return 32U;
}

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
  const sb_memory_region_t *rp = &sbp->regions[0];

  do {
    if (sb_reg_is_memory(rp) && sb_reg_is_writable(rp)) {
      return rp;
    }
    rp++;
  } while (rp < &sbp->regions[SB_CFG_NUM_REGIONS]);

  return NULL;
}

static size_t sb_init_environment(sb_class_t *sbp, const memory_area_t *up,
                                  const char *argv[], const char *envp[]) {
  void *usp, *uargv, *uenvp;
  size_t totsize, envsize, argsize, parsize;
  int uargc, uenvc;

  /* Setting up an initial stack for the sandbox.*/
  usp = up->base + up->size;

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

#if (CORTEX_USE_FPU == TRUE) && (PORT_USE_FPU_FAST_SWITCHING >= 2)
  /* Enforcing FPCA active for this thread by reading the FPSCR, this is
     actually simpler than trying to handle all possible cases in syscalls
     handling via SVC.*/
  (void) __get_FPSCR();
#endif

  asm volatile ("svc     #1");

  /* Jump with no return to the context saved at "u_psp". */
  chSysHalt("svc");
}

static void sb_release_memory(thread_t *tp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetObjectX(tp);

  (void)sbp;

#if (PORT_SWITCHED_REGIONS_NUMBER > 0) && (CH_CFG_USE_HEAP == TRUE)
  if (sbp->is_dynamic) {
    chHeapFree(sbp->thread.wabase);
    chHeapFree(sbp->regions[0].area.base);
  }
#endif
}

static thread_t *sb_start_unprivileged(sb_class_t *sbp,
                                       const char *name,
                                       tprio_t prio,
                                       stkline_t *stkbase,
                                       uint32_t u_pc) {
  thread_t *utp;

#if SB_CFG_ENABLE_VRQ == TRUE
  /* Clearing VRQ-related information that could be leftovers of previous
     sandbox executions.*/
  memset((void *)&sbp->vrq, 0, sizeof sbp->vrq);
  sbp->vrq.isr = SB_VRQ_ISR_DISABLED;
#endif

  /* Creating a thread on the unprivileged handler.*/
  thread_descriptor_t td = {
    .name       = name,
    .wbase      = stkbase,
    .wend       = stkbase + (SB_CFG_PRIVILEGED_STACK_SIZE / sizeof (stkline_t)),
    .prio       = prio,
    .funcp      = sb_unprivileged_trampoline,
    .arg        = NULL,
    .owner      = NULL
  };
  utp = chThdSpawnSuspended(&sbp->thread, &td);

#if PORT_SWITCHED_REGIONS_NUMBER > 0
  /* Switched-region MPU: regions are set up on switch-in.*/
  for (unsigned i = 0U; i < PORT_SWITCHED_REGIONS_NUMBER; i++) {
    port_mpureg_t mpureg;

    if (get_mpu_settings(&sbp->regions[i], &mpureg)) {
      utp->state = CH_STATE_FINAL;
      chThdRelease(utp);
      return NULL;
    }
    utp->ctx.regions[i] = mpureg;
  }
#endif

  /* The sandbox is the thread controller, note that it is done after
     calling chThdRelease() on error.*/
  chThdSetCallbackX(utp, sb_release_memory, sbp);

#if ((CORTEX_USE_FPU == TRUE) && (PORT_USE_FPU_FAST_SWITCHING <= 1))
  /* Starting with a long frame, FPCA is set on thread start.*/
  {
    struct port_extctx *ectxp;

    /* Creating entry frame.*/
    sbp->u_psp -= sizeof (struct port_extctx);
    ectxp = (struct port_extctx *)sbp->u_psp;

    /* Initializing the unprivileged mode entry context, clearing
       all registers.*/
    memset((void *)ectxp, 0, sizeof (struct port_extctx));
    ectxp->pc    = u_pc;
    ectxp->xpsr  = 0x01000000U;
    ectxp->fpscr = FPU->FPDSCR;
  }
#else
  /* Starting with a short frame, FPU disabled or FPCA cleared
     on thread start.*/
  {
    struct port_short_extctx *ectxp;

    /* Creating entry frame.*/
    sbp->u_psp -= sizeof (struct port_short_extctx);
    ectxp = (struct port_short_extctx *)sbp->u_psp;

    /* Initializing the unprivileged mode entry context, clearing
       all registers.*/
    memset((void *)ectxp, 0, sizeof (struct port_short_extctx));
    ectxp->pc    = u_pc;
    ectxp->xpsr  = 0x01000000U;
  }
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
  int n = 0;

  size = sizeof (const char *);
  if (v != NULL) {
    while ((s = *v) != NULL) {
      size += sizeof (const char *) + strlen(s) + (size_t)1;
      n++;
      v++;
    }
  }

  if (np != NULL) {
    *np = n;
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

/**
 * @brief   Sandbox object initialization.
 *
 * @param[out] sbp      pointer to the sandbox object
 *
 * @init
 */
void sbObjectInit(sb_class_t *sbp) {

  memset((void *)sbp, 0, sizeof (sb_class_t));

  /* Marking the thread as terminated in order to make sbIsThreadRunningX()
     behave correctly.*/
  sbp->thread.state = CH_STATE_FINAL;
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

/**
 * @brief   Starts a static sandbox.
 * @details Region zero is assumed to be the code region and contain a correct
 *          sandbox header. The data region is searched into the sandbox
 *          configuration regions list, the fist memory writable region is
 *          selected for stack initialization.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] prio      sandbox thread priority
 * @param[in] stkbase   base of the privileged stack area, the size is assumed
 *                      to be @p SB_CFG_PRIVILEGED_STACK_SIZE
 * @param[in] argv      array of parameters for the sandbox
 * @param[in] envp      array of environment variables for the sandbox
 * @return              The thread pointer.
 * @retval NULL         if the sandbox thread creation failed.
 */
thread_t *sbStart(sb_class_t *sbp, tprio_t prio, stkline_t *stkbase,
                  const char *argv[], const char *envp[]) {
  const sb_memory_region_t *codereg, *datareg;
  const sb_header_t *sbhp;

  /* Region zero is assumed to be executable and contain the start header.*/
  codereg = &sbp->regions[0];
  sbhp = (const sb_header_t *)(void *)codereg->area.base;

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
  if (!chMemIsSpaceWithinX(&codereg->area,
                           (const void *)sbhp->hdr_entry,
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

  /* No memory release.*/
  sbp->is_dynamic = false;

  /* Everything OK, starting the unprivileged thread inside the sandbox.*/
  return sb_start_unprivileged(sbp, argv[0], prio, stkbase, sbhp->hdr_entry);
}

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Execute an elf file within a sandbox.
 * @note    The file is loaded into region zero of the sandbox which is
 *          assumed to be used for both code and data, extra regions are
 *          not touched by this function.
 * @note    The size of the privileged stack is assumed to be always
 *          @p SB_CFG_PRIVILEGED_STACK_SIZE.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] prio      sandbox thread priority
 * @param[in] stkbase   base of the privileged stack for the sandbox
 * @param[in] path      file to be executed
 * @param[in] argv      arguments to be passed to the sandbox
 * @param[in] envp      environment variables to be passed to the sandbox
 * @return              The operation result.
 *
 * @api
 */
msg_t sbExecStatic(sb_class_t *sbp, tprio_t prio,
                   stkline_t *stkbase, const char *path,
                   const char *argv[], const char *envp[]) {
  memory_area_t ma = sbp->regions[0].area;
  const sb_header_t *sbhp;
  size_t totsize;
  msg_t ret;

  /* Pushing arguments, environment variables and other startup information
     at the top of the data memory area.*/
  totsize = sb_init_environment(sbp, &sbp->regions[0].area, argv, envp);
  if (totsize == (size_t)0) {
    return CH_RET_ENOMEM;
  }

  /* Adjusting the size of the memory area object, we don't want the loaded
     elf file to overwrite the environment data.*/
  ma.size -= totsize;

  /* Loading sandbox code into the specified memory area.*/
  ret = sbElfLoadFile(sbp->io.vfs_driver, path, &ma);
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

#if SB_CFG_EXEC_DEBUG == TRUE
  *((uint16_t *)(sbhp->hdr_entry & ~(uint32_t)1)) = 0xBE00U;
#endif

  /* No memory release.*/
  sbp->is_dynamic = false;

  /* Everything OK, starting the unprivileged thread inside the sandbox.*/
  if (sb_start_unprivileged(sbp, argv[0], prio, stkbase, sbhp->hdr_entry) == NULL) {
    return CH_RET_ENOMEM;
  }

  return CH_RET_SUCCESS;
}

#if (PORT_SWITCHED_REGIONS_NUMBER > 0) && (CH_CFG_USE_HEAP == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Execute an elf file within a dynamic sandbox.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] prio      sandbox thread priority
 * @param[in] heapsize  extra heap space for the launched application
 * @param[in] path      file to be executed
 * @param[in] argv      arguments to be passed to the sandbox
 * @param[in] envp      environment variables to be passed to the sandbox
 * @return              The operation result.
 *
 * @api
 */
msg_t sbExecDynamic(sb_class_t *sbp, tprio_t prio, size_t heapsize,
                    const char *path, const char *argv[], const char *envp[]) {
  memory_area_t *umap = &sbp->regions[0].area;
  memory_area_t elfma;
  const sb_header_t *sbhp;
  vfs_file_node_c *fnp;
  stkline_t *stkbase;
  size_t size, basealign;
  msg_t ret;

  ret = vfsDrvOpenFile(sbp->io.vfs_driver, path, VO_RDONLY, &fnp);
  CH_RETURN_ON_ERROR(ret);

  /* Calculating bare-minimum space required by the elf file.*/
  ret = sbElfGetAllocation(fnp, &umap->size);
  if (CH_RET_IS_ERROR(ret)) {
    goto skip1;
  }

  /* Adding the specified extra heap space.*/
  umap->size += heapsize;

  /* Adding space for arguments, environment variables and parameters.*/
  size = sb_strv_getsize(envp, NULL) +   /* Space for environment.   */
         sb_strv_getsize(argv, NULL) +   /* Space for arguments.     */
         (sizeof (uint32_t) * 4);        /* Space for parameters.    */
  umap->size += MEM_ALIGN_NEXT(size, PORT_STACK_ALIGN);

  /* Alignment of the memory area, it depends on the architecture because
     MPU-related restrictions. Note: there is the assumption that this
     alignment is greater than the alignment required for stacks.*/
  basealign = get_mpu_alignment(umap);

  /* Allocating the memory area required for this dynamic sandbox.*/
  umap->base = chHeapAllocAligned(NULL, umap->size, basealign);
  if (umap->base == NULL) {
    ret = CH_RET_ENOMEM;
    goto skip1;
  }

  /* Allocating an area for the privileged stack.*/
  stkbase = chHeapAllocAligned(NULL, SB_CFG_PRIVILEGED_STACK_SIZE, PORT_STACK_ALIGN);
  if (stkbase == NULL) {
    ret = CH_RET_ENOMEM;
    goto skip2;
  }

  /* Pushing arguments, environment variables and other startup information
     at the top of the data memory area.*/
  size = sb_init_environment(sbp, umap, argv, envp);
  if (size == (size_t)0) {
    goto skip3;
  }

  /* We don't want the loaded elf file to overwrite the environment data.*/
  elfma = *umap;
  elfma.size -= size;

  /* Loading sandbox code into the specified memory area.*/
  ret = sbElfLoad(fnp, &elfma);
  if (CH_RET_IS_ERROR(ret)) {
    goto skip3;
  }

  /* Header location.*/
  sbhp = (const sb_header_t *)(void *)umap->base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_HDR_MAGIC1) ||
      (sbhp->hdr_magic2 != SB_HDR_MAGIC2)) {
    ret = CH_RET_ENOEXEC;
    goto skip3;
  }

  /* Checking header size.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    ret = CH_RET_ENOEXEC;
    goto skip3;
  }

  /* Checking header entry point.*/
  if (!chMemIsSpaceWithinX(&elfma, (const void *)sbhp->hdr_entry, (size_t)2)) {
    ret = CH_RET_EFAULT;
    goto skip3;
  }

#if SB_CFG_EXEC_DEBUG == TRUE
  *((uint16_t *)(sbhp->hdr_entry & ~(uint32_t)1)) = 0xBE00U;
#endif

  /* Marks for memory release.*/
  sbp->is_dynamic = true;
  sbp->regions[0].attributes = SB_REG_IS_CODE_AND_DATA;

  /* Everything OK, starting the unprivileged thread inside the sandbox.*/
  if (sb_start_unprivileged(sbp, argv[0], prio, stkbase, sbhp->hdr_entry) == NULL) {
    ret = CH_RET_ENOMEM;
    goto skip3;
  }

  /* File closed.*/
  vfsClose((vfs_node_c *)fnp);

  return CH_RET_SUCCESS;

  /* Error exit points with resource freeing.*/
skip3:
  chHeapFree(stkbase);
skip2:
  chHeapFree(umap->base);
skip1:
  vfsClose((vfs_node_c *)fnp);
  return ret;
}
#endif /* CH_CFG_USE_HEAP == TRUE */
#endif /* SB_CFG_ENABLE_VFS == TRUE */

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
  if ((msg == MSG_TIMEOUT) && (sbp->base.msg_tp == ctp)) {
    sbp->base.msg_tp = NULL;
  }

  chSysUnlock();

  return msg;
}
#endif /* CH_CFG_USE_MESSAGES == TRUE */

/** @} */
