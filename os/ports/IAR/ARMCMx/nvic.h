/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    IAR/ARMCMx/nvic.h
 * @brief   Cortex-Mx NVIC support macros and structures.
 *
 * @addtogroup IAR_ARMCMx_NVIC
 * @{
 */

#ifndef _NVIC_H_
#define _NVIC_H_

/*
 * System vector constants for @p NVICSetSystemHandlerPriority().
 */
#define HANDLER_MEM_MANAGE      0       /**< MEM MANAGE vector id.          */
#define HANDLER_BUS_FAULT       1       /**< BUS FAULT vector id.           */
#define HANDLER_USAGE_FAULT     2       /**< USAGE FAULT vector id.         */
#define HANDLER_RESERVED_3      3
#define HANDLER_RESERVED_4      4
#define HANDLER_RESERVED_5      5
#define HANDLER_RESERVED_6      6
#define HANDLER_SVCALL          7       /**< SVCALL vector id.              */
#define HANDLER_DEBUG_MONITOR   8       /**< DEBUG MONITOR vector id.       */
#define HANDLER_RESERVED_9      9
#define HANDLER_PENDSV          10      /**< PENDSV vector id.              */
#define HANDLER_SYSTICK         11      /**< SYS TCK vector id.             */

typedef volatile uint8_t IOREG8;        /**< 8 bits I/O register type.      */
typedef volatile uint32_t IOREG32;      /**< 32 bits I/O register type.     */

/**
 * @brief NVIC ITCR register.
 */
#define NVIC_ITCR               (*((IOREG32 *)0xE000E004))

/**
 * @brief NVIC STIR register.
 */
#define NVIC_STIR               (*((IOREG32 *)0xE000EF00))

/**
 * @brief Structure representing the SYSTICK I/O space.
 */
typedef struct {
  IOREG32       CSR;
  IOREG32       RVR;
  IOREG32       CVR;
  IOREG32       CBVR;
} CM3_ST;

/**
 * @brief SYSTICK peripheral base address.
 */
#define STBase                  ((CM3_ST *)0xE000E010)
#define ST_CSR                  (STBase->CSR)
#define ST_RVR                  (STBase->RVR)
#define ST_CVR                  (STBase->CVR)
#define ST_CBVR                 (STBase->CBVR)

#define CSR_ENABLE_MASK         (0x1 << 0)
#define ENABLE_OFF_BITS         (0 << 0)
#define ENABLE_ON_BITS          (1 << 0)
#define CSR_TICKINT_MASK        (0x1 << 1)
#define   TICKINT_DISABLED_BITS (0 << 1)
#define   TICKINT_ENABLED_BITS  (1 << 1)
#define CSR_CLKSOURCE_MASK      (0x1 << 2)
#define   CLKSOURCE_EXT_BITS    (0 << 2)
#define   CLKSOURCE_CORE_BITS   (1 << 2)
#define CSR_COUNTFLAG_MASK      (0x1 << 16)

#define RVR_RELOAD_MASK         (0xFFFFFF << 0)

#define CVR_CURRENT_MASK        (0xFFFFFF << 0)

#define CBVR_TENMS_MASK         (0xFFFFFF << 0)
#define CBVR_SKEW_MASK          (0x1 << 30)
#define CBVR_NOREF_MASK         (0x1 << 31)

/**
 * @brief Structure representing the NVIC I/O space.
 */
typedef struct {
  IOREG32       ISER[8];
  IOREG32       unused1[24];
  IOREG32       ICER[8];
  IOREG32       unused2[24];
  IOREG32       ISPR[8];
  IOREG32       unused3[24];
  IOREG32       ICPR[8];
  IOREG32       unused4[24];
  IOREG32       IABR[8];
  IOREG32       unused5[56];
  IOREG32       IPR[60];
} CM3_NVIC;

/**
 * @brief NVIC peripheral base address.
 */
#define NVICBase                ((CM3_NVIC *)0xE000E100)
#define NVIC_ISER(n)            (NVICBase->ISER[n])
#define NVIC_ICER(n)            (NVICBase->ICER[n])
#define NVIC_ISPR(n)            (NVICBase->ISPR[n])
#define NVIC_ICPR(n)            (NVICBase->ICPR[n])
#define NVIC_IABR(n)            (NVICBase->IABR[n])
#define NVIC_IPR(n)             (NVICBase->IPR[n])

/**
 * @brief Structure representing the System Control Block I/O space.
 */
typedef struct {
  IOREG32       CPUID;
  IOREG32       ICSR;
  IOREG32       VTOR;
  IOREG32       AIRCR;
  IOREG32       SCR;
  IOREG32       CCR;
  IOREG32       SHPR[3];
  IOREG32       SHCSR;
  IOREG32       CFSR;
  IOREG32       HFSR;
  IOREG32       DFSR;
  IOREG32       MMFAR;
  IOREG32       BFAR;
  IOREG32       AFSR;
} CM3_SCB;

/**
 * @brief SCB peripheral base address.
 */
#define SCBBase                 ((CM3_SCB *)0xE000ED00)
#define SCB_CPUID               (SCBBase->CPUID)
#define SCB_ICSR                (SCBBase->ICSR)
#define SCB_VTOR                (SCBBase->VTOR)
#define SCB_AIRCR               (SCBBase->AIRCR)
#define SCB_SCR                 (SCBBase->SCR)
#define SCB_CCR                 (SCBBase->CCR)
#define SCB_SHPR(n)             (SCBBase->SHPR[n])
#define SCB_SHCSR               (SCBBase->SHCSR)
#define SCB_CFSR                (SCBBase->CFSR)
#define SCB_HFSR                (SCBBase->HFSR)
#define SCB_DFSR                (SCBBase->DFSR)
#define SCB_MMFAR               (SCBBase->MMFAR)
#define SCB_BFAR                (SCBBase->BFAR)
#define SCB_AFSR                (SCBBase->AFSR)

#define ICSR_VECTACTIVE_MASK    (0x1FF << 0)
#define ICSR_RETTOBASE          (0x1 << 11)
#define ICSR_VECTPENDING_MASK   (0x1FF << 12)
#define ICSR_ISRPENDING         (0x1 << 22)
#define ICSR_ISRPREEMPT         (0x1 << 23)
#define ICSR_PENDSTCLR          (0x1 << 25)
#define ICSR_PENDSTSET          (0x1 << 26)
#define ICSR_PENDSVCLR          (0x1 << 27)
#define ICSR_PENDSVSET          (0x1 << 28)
#define ICSR_NMIPENDSET         (0x1 << 31)

#define AIRCR_VECTKEY           0x05FA0000
#define AIRCR_PRIGROUP_MASK     (0x7 << 8)
#define AIRCR_PRIGROUP(n)       ((n) << 8)

#ifdef __cplusplus
extern "C" {
#endif
  void NVICEnableVector(uint32_t n, uint32_t prio);
  void NVICDisableVector(uint32_t n);
  void NVICSetSystemHandlerPriority(uint32_t handler, uint32_t prio);
#ifdef __cplusplus
}
#endif

#endif /* _NVIC_H_ */

/** @} */
