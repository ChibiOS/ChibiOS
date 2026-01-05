/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio.

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
 * @file    ARMv8-M-ML-ALT/chcore.h
 * @brief   ARMv8-M MainLine (alternate) port macros and structures.
 *
 * @addtogroup ARMV8M_ML_ALT_CORE
 * @{
 */

#ifndef CHCORE_H
#define CHCORE_H

/* Inclusion of the Cortex-Mx implementation specific parameters.*/
#include "cmparams.h"
#include "mpu_v8m.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Compiler name and version.
 */
#if defined(__GNUC__) || defined(__DOXYGEN__)
#define PORT_COMPILER_NAME              "GCC " __VERSION__

#elif defined(__ICCARM__)
#define PORT_COMPILER_NAME              "IAR"

#elif defined(__CC_ARM)
#define PORT_COMPILER_NAME              "RVCT"

#else
#error "unsupported compiler"
#endif

/**
 * @name    Port Capabilities and Constants
 * @{
 */
/**
 * @brief   This port supports a realtime counter.
 */
#define PORT_SUPPORTS_RT                TRUE

/**
 * @brief   Natural alignment constant.
 * @note    It is the minimum alignment for pointer-size variables.
 */
#define PORT_NATURAL_ALIGN              sizeof (void *)

/**
 * @brief   Stack initial alignment constant.
 * @note    It is the alignment required for the initial stack pointer,
 *          must be a multiple of sizeof (port_stkline_t).
 * @note    It is set to 32 in this architecture in order to have stacks
 *          initially aligned with cache lines.
 */
#define PORT_STACK_ALIGN                32U

/**
 * @brief   Working Areas alignment constant.
 * @note    It is the alignment to be enforced for thread working areas,
 *          must be a multiple of sizeof (port_stkline_t).
 * @note    It is set to 32 in this architecture in order to have working
 *          areas aligned with cache lines and MPU guard pages.
 */
#define PORT_WORKING_AREA_ALIGN         32U
/** @} */

/**
 * @name    Kernel modes
 * @{
 */
/**
 * @brief   Standard mode.
 * @details The RTOS is running in secure state with no switch in non-secure
 *          state ever performed. This is also used on devices with no
 *          TrustZone support at all.
 */
#define PORT_KERNEL_MODE_NORMAL         0U
/**
 * @brief   Kernel running in secure state with TZ support.
 * @details The RTOS is running in secure state.
 */
#define PORT_KERNEL_MODE_HOST           1U
/**
 * @brief   Kernel running in non-secure state.
 * @details The RTOS is running in non-secure state with no access to secure
 *          resources.
 */
#define PORT_KERNEL_MODE_GUEST          2U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Kernel mode selection.
 */
#if !defined(PORT_KERNEL_MODE) || defined(__DOXYGEN__)
#define PORT_KERNEL_MODE                PORT_KERNEL_MODE_NORMAL
#endif

/**
 * @brief   Implements a syscall interface on SVCALL.
 */
#if !defined(PORT_USE_SYSCALL) || defined(__DOXYGEN__)
#define PORT_USE_SYSCALL                FALSE
#endif

/**
 * @brief   Number of MPU regions to be saved/restored during context switch.
 * @note    The first region is always region zero.
 * @note    The use of this option has an overhead of 8 bytes for each
 *          region for each thread.
 * @note    Allowed values are 0..8, zero means none.
 */
#if !defined(PORT_SWITCHED_REGIONS_NUMBER) || defined(__DOXYGEN__)
#define PORT_SWITCHED_REGIONS_NUMBER    0
#endif

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 * @note    In this port it is set to 64 because the idle thread does have
 *          a stack frame when compiling without optimizations. You may
 *          reduce this value to zero when compiling with optimizations.
 */
#if !defined(PORT_IDLE_THREAD_STACK_SIZE) || defined(__DOXYGEN__)
#define PORT_IDLE_THREAD_STACK_SIZE     64
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 * @note    This port requires no extra stack space for interrupt handling
 *          because switch is performed in an exception handler.
 */
#if !defined(PORT_INT_REQUIRED_STACK) || defined(__DOXYGEN__)
#define PORT_INT_REQUIRED_STACK         0
#endif

/**
 * @brief   Enables the use of the WFI instruction in the idle thread loop.
 */
#if !defined(PORT_ENABLE_WFI_IDLE)
#define PORT_ENABLE_WFI_IDLE            FALSE
#endif

/**
 * @brief   Number of upper priority levels reserved as fast interrupts.
 * @note    The default reserves no priority levels for fast interrupts.
 */
#if !defined(PORT_FAST_PRIORITIES)
#define PORT_FAST_PRIORITIES            0U
#endif

/**
 * @brief   FPU support in context switch.
 * @details Activating this option activates the FPU support in the kernel.
 */
#if !defined(CORTEX_USE_FPU)
#define CORTEX_USE_FPU                  CORTEX_HAS_FPU
#elif (CORTEX_USE_FPU == TRUE) && (CORTEX_HAS_FPU == FALSE)
/* This setting requires an FPU presence check in case it is externally
   redefined.*/
#error "the selected core does not have an FPU"
#endif

/**
 * @brief   Partial context switching.
 * @details This option skips saving FPU registers for those threads that
 *          do not use FPU. This can make context switch faster but can
 *          leak information among threads through uninitialized FPU
 *          registers, possible values:
 *          - 0: Always full and immediate switching (CONTROL_FPCA enforced).
 *          - 1: Activate lazy FPU registers stacking (CONTROL_FPCA enforced,
 *               FPU_FPCCR_LSPEN enabled).
 *          - 2: Uses short exception context when possible (FPU_FPCCR_ASPEN,
 *               FPU_FPCCR_LSPEN enabled).
 *          - 3: Uses short exception context when possible and also omits
 *               saving s16-s31 when it is not needed (FPU_FPCCR_ASPEN,
 *               FPU_FPCCR_LSPEN enabled and extra SW checks).
 */
#if !defined(PORT_USE_FPU_FAST_SWITCHING) || defined(__DOXYGEN__)
#define PORT_USE_FPU_FAST_SWITCHING     3
#endif

/**
 * @brief   NVIC PRIGROUP initialization expression.
 * @details The default assigns all available priority bits as preemption
 *          priority with no sub-priority.
 * @note    Changing this value is not recommended.
 */
#if !defined(PORT_PRIGROUP_INIT) || defined(__DOXYGEN__)
#define PORT_PRIGROUP_INIT              (7U - CORTEX_PRIORITY_BITS)
#endif

/**
 * @brief   Enables MPU static initialization.
 * @details The initialization is performed according to the various
 *          @p PORT_MPU_MAIRx_INIT, @p PORT_MPU_RBARx_INIT and
 *          @p PORT_MPU_RLARx_INIT settings.
 */
#if !defined(PORT_MPU_INITIALIZE) || defined(__DOXYGEN__)
#define PORT_MPU_INITIALIZE             TRUE
#endif

/**
 * @brief   MAIR0 register initialization.
 * @note    Default values of ATTR0...ATTR3 fields are those expected by the
 *          SB subsystem, do not change this setting if using SB.
 *          - ATTR0 is write-back cacheable memory.
 *          - ATTR1 is write-through cacheable memory.
 *          - ATTR2 is non-cacheable memory.
 *          - ATTR3 is device space.
 *          .
 */
#if !defined(PORT_MPU_MAIR0_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_MAIR0_INIT                                                 \
  (MPU_MAIR0_ATTR0(MPU_MAIR_MEM_OUTER_WB_NT_RWA     |                       \
                   MPU_MAIR_MEM_INNER_WB_NT_RWA)    |                       \
   MPU_MAIR0_ATTR1(MPU_MAIR_MEM_OUTER_WT_NT_RWA     |                       \
                   MPU_MAIR_MEM_INNER_WT_NT_RWA)    |                       \
   MPU_MAIR0_ATTR2(MPU_MAIR_MEM_OUTER_NC            |                       \
                   MPU_MAIR_MEM_INNER_NC)           |                       \
   MPU_MAIR0_ATTR3(MPU_MAIR_DEVICE_nGnRE))
#endif

/**
 * @brief   MAIR1 register initialization.
 */
#if !defined(PORT_MPU_MAIR1_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_MAIR1_INIT                                                 \
  (MPU_MAIR1_ATTR4(0U)                              |                       \
   MPU_MAIR1_ATTR5(0U)                              |                       \
   MPU_MAIR1_ATTR6(0U)                              |                       \
   MPU_MAIR1_ATTR7(0U))
#endif

/**
 * @brief   RBAR register initialization for region 0.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR0_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR0_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 0.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR0_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR0_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 1.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR1_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR1_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 1.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR1_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR1_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 2.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR2_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR2_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 2.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR2_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR2_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 3.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR3_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR3_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 3.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR3_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR3_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 4.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR4_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR4_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 4.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR4_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR4_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 5.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR5_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR5_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 5.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR5_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR5_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 6.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR6_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR6_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 6.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR6_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR6_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 7.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR7_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR7_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 7.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR7_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR7_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 8.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR8_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR8_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 8.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR8_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR8_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 9.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR9_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR9_INIT             0U
#endif

/**
 * @brief   RLAR register initialization for region 9.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR9_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR9_INIT             0U
#endif

/**
 * @brief   RBAR register initialization for region 10.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR10_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR10_INIT            0U
#endif

/**
 * @brief   RLAR register initialization for region 10.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR10_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR10_INIT            0U
#endif

/**
 * @brief   RBAR register initialization for region 11.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR11_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR11_INIT            0U
#endif

/**
 * @brief   RLAR register initialization for region 11.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR11_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR11_INIT            0U
#endif

/**
 * @brief   RBAR register initialization for region 12.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR12_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR12_INIT            0U
#endif

/**
 * @brief   RLAR register initialization for region 12.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR12_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR12_INIT            0U
#endif

/**
 * @brief   RBAR register initialization for region 13.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR13_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR13_INIT            0U
#endif

/**
 * @brief   RLAR register initialization for region 13.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR13_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR13_INIT            0U
#endif

/**
 * @brief   RBAR register initialization for region 14.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR14_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR14_INIT            0U
#endif

/**
 * @brief   RLAR register initialization for region 14.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR14_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR14_INIT            0U
#endif

/**
 * @brief   RBAR register initialization for region 15.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RBAR15_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RBAR15_INIT            0U
#endif

/**
 * @brief   RLAR register initialization for region 15.
 * @note    Memory regions must not overlap in ARMv8-M.
 */
#if !defined(PORT_MPU_RLAR15_INIT) || defined(__DOXYGEN__)
#define PORT_MPU_RLAR15_INIT            0U
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (PORT_SWITCHED_REGIONS_NUMBER < 0) || (PORT_SWITCHED_REGIONS_NUMBER > 8)
  #error "invalid PORT_SWITCHED_REGIONS_NUMBER value"
#endif

#if (PORT_USE_FPU_FAST_SWITCHING < 0) || (PORT_USE_FPU_FAST_SWITCHING > 3)
#error "invalid PORT_USE_FPU_FAST_SWITCHING value specified"
#endif

#if (PORT_KERNEL_MODE != PORT_KERNEL_MODE_NORMAL) &&                        \
    (PORT_KERNEL_MODE != PORT_KERNEL_MODE_HOST) &&                          \
    (PORT_KERNEL_MODE != PORT_KERNEL_MODE_GUEST)
#error "invalid PORT_KERNEL_MODE value specified"
#endif

#if PORT_KERNEL_MODE == PORT_KERNEL_MODE_HOST
#error "PORT_KERNEL_MODE_HOST not supported"
#endif

#if (PORT_USE_SYSCALL == TRUE) ||                                           \
    ((CORTEX_USE_FPU == TRUE) && (PORT_USE_FPU_FAST_SWITCHING >= 2)) ||     \
    defined(__DOXYGEN__)
/**
 * @brief   CONTROL as part of the saved thread context.
 * @note    Saving control is only required when:
 *          - PORT_USE_SYSCALL is enabled because support for unprivileged
 *            mode.
 *          - PORT_USE_FPU is enabled with @p PORT_USE_FPU_FAST_SWITCHING
 *            modes 2 or 3 because CONTROL.FPCA needs to be handled for
 *            each thread.
 */
#define PORT_SAVE_CONTROL               TRUE
#else
#define PORT_SAVE_CONTROL               FALSE
#endif

#if (PORT_USE_SYSCALL == TRUE) || (CH_DBG_ENABLE_STACK_CHECK == TRUE) ||    \
    defined(__DOXYGEN__)
/**
 * @brief   PSPLIM as part of the saved thread context.
 */
#define PORT_SAVE_PSPLIM                TRUE
#else
#define PORT_SAVE_PSPLIM                FALSE
#endif

/**
 * @name    Architecture
 * @{
 */
/**
 * @brief   Macro defining a generic ARM architecture.
 */
#define PORT_ARCHITECTURE_ARM

/**
 * @brief   Macro defining the specific ARM architecture.
 */
#define PORT_ARCHITECTURE_ARM_V8M_MAINLINE

/**
 * @brief   Name of the implemented architecture.
 */
#define PORT_ARCHITECTURE_NAME          "ARMv8-M Mainline"

#if (CORTEX_MODEL == 33) || defined(__DOXYGEN__)

#if !defined(CH_CUSTOMER_LIC_PORT_CM33)
#error "CH_CUSTOMER_LIC_PORT_CM33 not defined"
#endif

#if CH_CUSTOMER_LIC_PORT_CM33 == FALSE
#error "ChibiOS Cortex-M33 port not licensed"
#endif

/**
 * @brief   Name of the architecture variant.
 */
#if (CORTEX_USE_FPU == TRUE) || defined(__DOXYGEN__)
#define PORT_CORE_VARIANT_NAME          "Cortex-M33 with FPU"
#else
#define PORT_CORE_VARIANT_NAME          "Cortex-M33"
#endif

#elif (CORTEX_MODEL == 55) || defined(__DOXYGEN__)

#if !defined(CH_CUSTOMER_LIC_PORT_CM55)
#error "CH_CUSTOMER_LIC_PORT_CM55 not defined"
#endif

#if CH_CUSTOMER_LIC_PORT_CM55 == FALSE
#error "ChibiOS Cortex-M55 port not licensed"
#endif

/**
 * @brief   Name of the architecture variant.
 */
#if CORTEX_USE_FPU || defined(__DOXYGEN__)
#define PORT_CORE_VARIANT_NAME          "Cortex-M55 with FPU"
#else
#define PORT_CORE_VARIANT_NAME          "Cortex-M55"
#endif

#else
#error "unknown ARMv8-M core variant"
#endif

#if (PORT_KERNEL_MODE == PORT_KERNEL_MODE_NORMAL) || defined (__DOXYGEN__)
/**
 * @brief   Port-specific information string.
 */
#define PORT_INFO                       "Normal mode without TZ"
#elif PORT_KERNEL_MODE == PORT_KERNEL_MODE_GUEST
#define PORT_INFO                       "TZ guest mode"
#else
#error "invalid kernel security mode"
#endif
/** @} */

/**
 * @name    Priorities
 * @{
 */
/**
 * @brief   Priority level to priority mask conversion macro.
 */
#define CORTEX_PRIO_MASK(n)             ((n) << (8U - CORTEX_PRIORITY_BITS))

/**
 * @brief   Disabled value for BASEPRI register.
 */
#define CORTEX_BASEPRI_DISABLED         CORTEX_PRIO_MASK(0U)

/**
 * @brief   BASEPRI level within kernel lock.
 */
#define CORTEX_BASEPRI_KERNEL           CORTEX_PRIO_MASK(CORTEX_MAX_KERNEL_PRIORITY)

/**
 * @brief   Total priority levels.
 */
#define CORTEX_PRIORITY_LEVELS          (1U << CORTEX_PRIORITY_BITS)

/**
 * @brief   Minimum priority level.
 * @details This minimum priority level is calculated from the number of
 *          priority bits supported by the specific Cortex-Mx implementation.
 */
#define CORTEX_MINIMUM_PRIORITY         (CORTEX_PRIORITY_LEVELS - 1U)

/**
 * @brief   Maximum priority level.
 * @details The maximum allowed priority level is always zero.
 */
#define CORTEX_MAXIMUM_PRIORITY         0U

#if (PORT_KERNEL_MODE == PORT_KERNEL_MODE_NORMAL) || defined (__DOXYGEN__)
/**
 * @brief   SVCALL handler priority.
 */
#define CORTEX_PRIORITY_SVCALL          (CORTEX_MAXIMUM_PRIORITY +          \
                                         PORT_FAST_PRIORITIES)

/**
 * @brief   PENDSV handler priority.
 */
#define CORTEX_PRIORITY_PENDSV          (CORTEX_MINIMUM_PRIORITY)

/**
 * @brief   Maximum usable priority for normal ISRs.
 * @note    Must be lower than @p CORTEX_PRIORITY_SVCALL.
 */
#define CORTEX_MAX_KERNEL_PRIORITY      (CORTEX_PRIORITY_SVCALL + 1U)

/**
 * @brief   Minimum usable priority for normal ISRs.
 */
#define CORTEX_MIN_KERNEL_PRIORITY      (CORTEX_PRIORITY_PENDSV - 1U)

#elif PORT_KERNEL_MODE == PORT_KERNEL_MODE_GUEST
#define CORTEX_PRIORITY_SVCALL          (CORTEX_MAXIMUM_PRIORITY +          \
                                         PORT_FAST_PRIORITIES)
#define CORTEX_PRIORITY_PENDSV          (CORTEX_MINIMUM_PRIORITY & 0xFFFFFFFEU)
#define CORTEX_MAX_KERNEL_PRIORITY      ((CORTEX_PRIORITY_SVCALL | 1U) + 1U)
#define CORTEX_MIN_KERNEL_PRIORITY      ((CORTEX_PRIORITY_PENDSV - 1U) & 0xFFFFFFFEU)
#else
  /* Note, checked above.*/
#endif
/** @} */

#if (PORT_FAST_PRIORITIES < 0U) ||                                           \
    (PORT_FAST_PRIORITIES > (CORTEX_PRIORITY_LEVELS / 4U))
#error "invalid PORT_FAST_PRIORITIES value specified"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/**
 * @brief   Type of an MPU region registers structure.
 *
 */
typedef struct {
  uint32_t              rbar;
  uint32_t              rlar;
} port_mpureg_t;

/**
 * @brief   Integer-only external context.
 */
struct port_short_extctx {
  uint32_t              r0;
  uint32_t              r1;
  uint32_t              r2;
  uint32_t              r3;
  uint32_t              r12;
  uint32_t              lr_thd;
  uint32_t              pc;
  uint32_t              xpsr;
};

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 */
struct port_extctx {
  uint32_t              r0;
  uint32_t              r1;
  uint32_t              r2;
  uint32_t              r3;
  uint32_t              r12;
  uint32_t              lr_thd;
  uint32_t              pc;
  uint32_t              xpsr;
#if (CORTEX_USE_FPU == TRUE) || defined(__DOXYGEN__)
  uint32_t              s0;
  uint32_t              s1;
  uint32_t              s2;
  uint32_t              s3;
  uint32_t              s4;
  uint32_t              s5;
  uint32_t              s6;
  uint32_t              s7;
  uint32_t              s8;
  uint32_t              s9;
  uint32_t              s10;
  uint32_t              s11;
  uint32_t              s12;
  uint32_t              s13;
  uint32_t              s14;
  uint32_t              s15;
  uint32_t              fpscr;
  uint32_t              reserved;
#endif /* CORTEX_USE_FPU == TRUE */
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner context during a context
 *          switch.
 */
struct port_intctx {
  /* Integer and special  registers context.*/
  uint32_t              basepri;
  uint32_t              r4;
  uint32_t              r5;
  uint32_t              r6;
  uint32_t              r7;
  uint32_t              r8;
  uint32_t              r9;
  uint32_t              r10;
  uint32_t              r11;
  uint32_t              lr_exc;
  /* Special registers context.*/
#if (PORT_SAVE_PSPLIM == TRUE) || defined(__DOXYGEN__)
  uint32_t              splim;
#endif
#if (PORT_SAVE_CONTROL == TRUE) || defined(__DOXYGEN__)
  uint32_t              control;
#endif
#if (CORTEX_USE_FPU == TRUE) || defined(__DOXYGEN__)
  /* Floating point registers context.*/
  uint32_t              s16;
  uint32_t              s17;
  uint32_t              s18;
  uint32_t              s19;
  uint32_t              s20;
  uint32_t              s21;
  uint32_t              s22;
  uint32_t              s23;
  uint32_t              s24;
  uint32_t              s25;
  uint32_t              s26;
  uint32_t              s27;
  uint32_t              s28;
  uint32_t              s29;
  uint32_t              s30;
  uint32_t              s31;
#endif /* CORTEX_USE_FPU == TRUE */
};

/**
 * @brief   Platform dependent part of the @p thread_t structure.
 * @details In this port the structure just holds a pointer to the
 *          @p port_intctx structure representing the stack pointer
 *          at context switch time.
 */
struct port_context {
  struct port_extctx    *sp;
  struct port_intctx    regs;
#if (PORT_SWITCHED_REGIONS_NUMBER > 0) || defined(__DOXYGEN__)
  port_mpureg_t         regions[PORT_SWITCHED_REGIONS_NUMBER];
#endif
};

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Priority level verification macro.
 */
#define PORT_IRQ_IS_VALID_PRIORITY(n)                                       \
  (((n) >= 0U) && ((n) < CORTEX_PRIORITY_LEVELS))

/**
 * @brief   Priority level verification macro.
 */
#define PORT_IRQ_IS_VALID_KERNEL_PRIORITY(n)                                \
  (((n) >= CORTEX_MAX_KERNEL_PRIORITY) && ((n) <= CORTEX_MIN_KERNEL_PRIORITY))

/**
 * @brief   Optimized thread function declaration macro.
 */
#define PORT_THD_FUNCTION(tname, arg) void tname(void *arg)

/**
 * @brief   Exception return value for threads creation.
 * @note    Enforcing a long context when FPU is enabled else using a
 *          short context.
 */
#if (CORTEX_USE_FPU == TRUE) || defined(__DOXYGEN__)
  #define CORTEX_EXC_RETURN         0xFFFFFFACU
#else
  #define CORTEX_EXC_RETURN         0xFFFFFFBCU
#endif

/**
 * @brief   Initialization of CONTROL part of thread context.
 */
#if (PORT_SAVE_CONTROL == TRUE) || defined(__DOXYGEN__)
  #if (CORTEX_USE_FPU == TRUE) || defined(__DOXYGEN__)
    #define __PORT_SETUP_CONTEXT_CONTROL(tp)                                \
      (tp)->ctx.regs.control          = CONTROL_FPCA_Msk
  #else
    #define __PORT_SETUP_CONTEXT_CONTROL(tp)                                \
      (tp)->ctx.regs.control          = 0U
  #endif
#else
  #define __PORT_SETUP_CONTEXT_CONTROL(tp)
#endif

/**
 * @brief   Initialization of stack check part of thread context.
 */
#if (PORT_SAVE_PSPLIM == TRUE) || defined(__DOXYGEN__)
#define __PORT_SETUP_CONTEXT_SPLIM(tp, wbase)                               \
    (tp)->ctx.regs.splim = (uint32_t)(wbase)
#else
#define __PORT_SETUP_CONTEXT_SPLIM(tp, wbase)
#endif

/**
 * @brief   Initialization of FPU part of thread context.
 * @note    The value of FPDSCR is used, it is meant to be the default.
 */
#if (CORTEX_USE_FPU == TRUE) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_FPU(tp)                                      \
    (tp)->ctx.sp->fpscr               = FPU->FPDSCR
#else
  #define __PORT_SETUP_CONTEXT_FPU(tp)
#endif

/**
 * @brief   Initialization of MPU part of thread context.
 */
#if (PORT_SWITCHED_REGIONS_NUMBER == 0) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)

#elif (PORT_SWITCHED_REGIONS_NUMBER == 1) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 2) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 3) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U;                                       \
    (tp)->ctx.regions[2].rbar   = 0U;                                       \
    (tp)->ctx.regions[2].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 4) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U;                                       \
    (tp)->ctx.regions[2].rbar   = 0U;                                       \
    (tp)->ctx.regions[2].rlar   = 0U;                                       \
    (tp)->ctx.regions[3].rbar   = 0U;                                       \
    (tp)->ctx.regions[3].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 5) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U;                                       \
    (tp)->ctx.regions[2].rbar   = 0U;                                       \
    (tp)->ctx.regions[2].rlar   = 0U;                                       \
    (tp)->ctx.regions[3].rbar   = 0U;                                       \
    (tp)->ctx.regions[3].rlar   = 0U;                                       \
    (tp)->ctx.regions[4].rbar   = 0U;                                       \
    (tp)->ctx.regions[4].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 6) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U;                                       \
    (tp)->ctx.regions[2].rbar   = 0U;                                       \
    (tp)->ctx.regions[2].rlar   = 0U;                                       \
    (tp)->ctx.regions[3].rbar   = 0U;                                       \
    (tp)->ctx.regions[3].rlar   = 0U;                                       \
    (tp)->ctx.regions[4].rbar   = 0U;                                       \
    (tp)->ctx.regions[4].rlar   = 0U;                                       \
    (tp)->ctx.regions[5].rbar   = 0U;                                       \
    (tp)->ctx.regions[5].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 7) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U;                                       \
    (tp)->ctx.regions[2].rbar   = 0U;                                       \
    (tp)->ctx.regions[2].rlar   = 0U;                                       \
    (tp)->ctx.regions[3].rbar   = 0U;                                       \
    (tp)->ctx.regions[3].rlar   = 0U;                                       \
    (tp)->ctx.regions[4].rbar   = 0U;                                       \
    (tp)->ctx.regions[4].rlar   = 0U;                                       \
    (tp)->ctx.regions[5].rbar   = 0U;                                       \
    (tp)->ctx.regions[5].rlar   = 0U;                                       \
    (tp)->ctx.regions[6].rbar   = 0U;                                       \
    (tp)->ctx.regions[6].rlar   = 0U

#elif (PORT_SWITCHED_REGIONS_NUMBER == 8) || defined(__DOXYGEN__)
  #define __PORT_SETUP_CONTEXT_MPU(tp)                                      \
    (tp)->ctx.regions[0].rbar   = 0U;                                       \
    (tp)->ctx.regions[0].rlar   = 0U;                                       \
    (tp)->ctx.regions[1].rbar   = 0U;                                       \
    (tp)->ctx.regions[1].rlar   = 0U;                                       \
    (tp)->ctx.regions[2].rbar   = 0U;                                       \
    (tp)->ctx.regions[2].rlar   = 0U;                                       \
    (tp)->ctx.regions[3].rbar   = 0U;                                       \
    (tp)->ctx.regions[3].rlar   = 0U;                                       \
    (tp)->ctx.regions[4].rbar   = 0U;                                       \
    (tp)->ctx.regions[4].rlar   = 0U;                                       \
    (tp)->ctx.regions[5].rbar   = 0U;                                       \
    (tp)->ctx.regions[5].rlar   = 0U;                                       \
    (tp)->ctx.regions[6].rbar   = 0U;                                       \
    (tp)->ctx.regions[6].rlar   = 0U;                                       \
    (tp)->ctx.regions[7].rbar   = 0U;                                       \
    (tp)->ctx.regions[7].rlar   = 0U

#else
  /* Note, checked above.*/
#endif

/**
 * @brief   Platform dependent part of the thread creation API.
 */
#define PORT_SETUP_CONTEXT(tp, wbase, wtop, pf, arg) do {                   \
  (tp)->ctx.sp = (struct port_extctx *)(void *)                             \
                 ((uint8_t *)(wtop) - sizeof (struct port_extctx));         \
  (tp)->ctx.regs.basepri    = CORTEX_BASEPRI_KERNEL;                        \
  (tp)->ctx.regs.r4         = (uint32_t)(pf);                               \
  (tp)->ctx.regs.r5         = (uint32_t)(arg);                              \
  (tp)->ctx.regs.lr_exc     = (uint32_t)CORTEX_EXC_RETURN;                  \
  (tp)->ctx.sp->pc          = (uint32_t)__port_thread_start;                \
  (tp)->ctx.sp->xpsr        = (uint32_t)0x01000000;                         \
  __PORT_SETUP_CONTEXT_SPLIM(tp, wbase);                                    \
  __PORT_SETUP_CONTEXT_CONTROL(tp);                                         \
  __PORT_SETUP_CONTEXT_FPU(tp);                                             \
  __PORT_SETUP_CONTEXT_MPU(tp);                                             \
} while (false)

/**
 * @brief   Context switch area size.
 */
#define PORT_WA_CTX_SIZE    sizeof (struct port_extctx)

/**
 * @brief   Computes the thread working area global size.
 * @note    There is no need to perform alignments in this macro.
 */
#define PORT_WA_SIZE(n)     ((size_t)PORT_WA_CTX_SIZE +                     \
                             (size_t)(n) +                                  \
                             (size_t)PORT_INT_REQUIRED_STACK)

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#if 0
#define PORT_IRQ_EPILOGUE() do {                                            \
  port_lock_from_isr();                                                     \
  if (chSchIsPreemptionRequired()) {                                        \
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;                                     \
  }                                                                         \
  port_unlock_from_isr();                                                   \
} while (false)
#else
#define PORT_IRQ_EPILOGUE() do {                                            \
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;                                       \
} while (false)
#endif

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#ifdef __cplusplus
  #define PORT_IRQ_HANDLER(id) extern "C" void id(void)

#else
  #define PORT_IRQ_HANDLER(id) void id(void)
#endif

/**
 * @brief   Fast IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#ifdef __cplusplus
  #define PORT_FAST_IRQ_HANDLER(id) extern "C" void id(void)

#else
  #define PORT_FAST_IRQ_HANDLER(id) void id(void)
#endif

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
/*lint -e438 -e529 -e586 [4.3, 2.2] The keyword is required, parameters are
  referenced by the final asm and are used.*/
#define port_switch(ntp, otp) do {                                          \
  register thread_t *_ntp asm ("r0") = (ntp);                               \
  register thread_t *_otp asm ("r1") = (otp);                               \
  asm volatile ("svc     #0" : : "r" (_otp), "r" (_ntp) : "memory");        \
} while (false)

/**
 * @brief   Returns a word representing a critical section status.
 *
 * @return              The critical section status.
 */
#define port_get_lock_status() __port_get_irq_status()

/**
 * @brief   Determines if in a critical section.
 *
 * @param[in] sts       status word returned by @p port_get_lock_status()
 * @return              The current status.
 * @retval false        if running outside a critical section.
 * @retval true         if running within a critical section.
 */
#define port_is_locked(sts) !__port_irq_enabled(sts)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

#ifdef __cplusplus
extern "C" {
#endif
  void port_init(os_instance_t *oip);
  void __port_thread_start(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*lint -save -e718 -e746 [17.3] The MISRA parser cannot see the function
  declarations in CMSIS headers, CMSIS parsing is disabled in those headers
  because the whole thing is not MISRA compliant and it is not uder our
  control.*/

/**
 * @brief   Returns a word encoding the current interrupts status.
 *
 * @return              The interrupts status.
 */
__STATIC_FORCEINLINE syssts_t __port_get_irq_status(void) {

  return (syssts_t)__get_BASEPRI();
}

/**
 * @brief   Checks the interrupt status.
 *
 * @param[in] sts       the interrupt status word
 *
 * @return              The interrupt status.
 * @retval false        the word specified a disabled interrupts status.
 * @retval true         the word specified an enabled interrupts status.
 */
__STATIC_FORCEINLINE bool __port_irq_enabled(syssts_t sts) {

  return (bool)(sts == (syssts_t)CORTEX_BASEPRI_DISABLED);
}

/**
 * @brief   Determines the current execution context.
 *
 * @return              The execution context.
 * @retval false        not running in ISR mode.
 * @retval true         running in ISR mode.
 */
__STATIC_FORCEINLINE bool port_is_isr_context(void) {

  return (bool)((__get_IPSR() & 0x1FFU) != 0U);
}

/**
 * @brief   Kernel-lock action.
 * @details In this port this function raises the base priority to kernel
 *          level.
 */
__STATIC_FORCEINLINE void port_lock(void) {

  __set_BASEPRI(CORTEX_BASEPRI_KERNEL);
}

/**
 * @brief   Kernel-unlock action.
 * @details In this port this function lowers the base priority to user
 *          level.
 */
__STATIC_FORCEINLINE void port_unlock(void) {

  __set_BASEPRI(CORTEX_BASEPRI_DISABLED);
}

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @details In this port this function raises the base priority to kernel
 *          level.
 * @note    Same as @p port_lock() in this port.
 */
__STATIC_FORCEINLINE void port_lock_from_isr(void) {

  port_lock();
}

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @details In this port this function lowers the base priority to user
 *          level.
 * @note    Same as @p port_unlock() in this port.
 */
__STATIC_FORCEINLINE void port_unlock_from_isr(void) {

  port_unlock();
}

/**
 * @brief   Disables all the interrupt sources.
 * @note    In this port it disables all the interrupt sources by raising
 *          the priority mask to level 0.
 */
__STATIC_FORCEINLINE void port_disable(void) {

  __disable_irq();
}

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 * @note    Interrupt sources above kernel level remains enabled.
 * @note    In this port it raises/lowers the base priority to kernel level.
 */
__STATIC_FORCEINLINE void port_suspend(void) {

  __set_BASEPRI(CORTEX_BASEPRI_KERNEL);
  __enable_irq();
}

/**
 * @brief   Enables all the interrupt sources.
 * @note    In this port it lowers the base priority to user level.
 */
__STATIC_FORCEINLINE void port_enable(void) {

  __set_BASEPRI(CORTEX_BASEPRI_DISABLED);
  __enable_irq();
}

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    Implemented as an inlined @p WFI instruction.
 */
__STATIC_FORCEINLINE void port_wait_for_interrupt(void) {

#if PORT_ENABLE_WFI_IDLE == TRUE
  __WFI();
#endif
}

/**
 * @brief   Returns the current value of the realtime counter.
 *
 * @return              The realtime counter value.
 */
__STATIC_FORCEINLINE rtcnt_t port_rt_get_counter_value(void) {

  return DWT->CYCCNT;
}

/*lint -restore*/

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

#if CH_CFG_ST_TIMEDELTA > 0
#include "chcore_timer.h"
#endif /* CH_CFG_ST_TIMEDELTA > 0 */

#endif /* !defined(_FROM_ASM_) */

#endif /* CHCORE_H */

/** @} */
