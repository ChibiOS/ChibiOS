/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    ARMv6-M/smp/rp2/chcoresmp.h
 * @brief   ARMv6-M RP2 SMP macros and structures.
 *
 * @addtogroup ARMV6M_CORE_SMP_RP2
 * @{
 */

#ifndef CHCORESMP_H
#define CHCORESMP_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of SMP cores.
 */
#define PORT_CORES_NUMBER               2

/* RP2040 supports a realtime counter via TIMER0 unit, overriding the
   default in chcore.h.*/
#undef PORT_SUPPORTS_RT
#define PORT_SUPPORTS_RT                TRUE

/**
 * @name    IPC FIFO messages
 * @{
 */
#define PORT_FIFO_RESCHEDULE_MESSAGE    0xFFFFFFFFU
#define PORT_FIFO_PANIC_MESSAGE         0xFFFFFFFEU
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Spinlock to be used by the port layer.
 */
#if !defined(PORT_SPINLOCK_NUMBER)
#define PORT_SPINLOCK_NUMBER            31
#endif

/**
 * @brief   Preferential local and coherent BSS section for core zero.
 */
#if !defined(PORT_MEM_LOCAL_COHERENT_BSS0)
#define PORT_MEM_LOCAL_COHERENT_BSS0    CC_SECTION(".ram4_clear.core0")
#endif

/**
 * @brief   Preferential local and coherent BSS section for core one.
 */
#if !defined(PORT_MEM_LOCAL_COHERENT_BSS1)
#define PORT_MEM_LOCAL_COHERENT_BSS1    CC_SECTION(".ram5_clear.core1")
#endif

/**
 * @brief   Preferential local BSS section for core zero.
 */
#if !defined(PORT_MEM_LOCAL_BSS0)
#define PORT_MEM_LOCAL_BSS0             CC_SECTION(".ram4_clear.core0")
#endif

/**
 * @brief   Preferential local BSS section for core one.
 */
#if !defined(PORT_MEM_LOCAL_BSS1)
#define PORT_MEM_LOCAL_BSS1             CC_SECTION(".ram5_clear.core1")
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (PORT_SPINLOCK_NUMBER < 0) || (PORT_SPINLOCK_NUMBER > 31)
  #error "invalid PORT_SPINLOCK_NUMBER value"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Panic notification.
 * @note    It is sent without polling for FIFO space because the other side
 *          could be unable to empty the FIFO after a catastrophic error.
 */
#define PORT_SYSTEM_HALT_HOOK() do {                                        \
    SIO->FIFO_WR = PORT_FIFO_PANIC_MESSAGE;                                 \
  } while (false)

/**
 * @brief   SMP-related port initialization.
 * @note    The port checks on presence of this macro so this
 *          must be a macro.
 *
 * @param[in, out] oip  pointer to the @p os_instance_t structure
 */
#define port_smp_init(oip) __port_smp_init(oip)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void __port_smp_init(os_instance_t *oip);
  void __port_spinlock_take(void);
  void __port_spinlock_release(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Triggers an inter-core notification.
 *
 * @param[in] oip       pointer to the @p os_instance_t structure
 */
__STATIC_INLINE void port_notify_instance(os_instance_t *oip) {

  (void)oip;

  /* Sending a reschedule order to the other core if there is space in
     the FIFO.*/
  if ((SIO->FIFO_ST & SIO_FIFO_ST_RDY) != 0U) {
    SIO->FIFO_WR = PORT_FIFO_RESCHEDULE_MESSAGE;
  }
}

/**
 * @brief   Takes the kernel spinlock.
 */
__STATIC_INLINE void port_spinlock_take(void) {

  while (SIO->SPINLOCK[PORT_SPINLOCK_NUMBER] == 0U) {
  }
  __DMB();
}

/**
 * @brief   Releases the kernel spinlock.
 */
__STATIC_INLINE void port_spinlock_release(void) {

  __DMB();
  SIO->SPINLOCK[PORT_SPINLOCK_NUMBER] = (uint32_t)SIO;
}

/**
 * @brief   Returns the current value of the realtime counter.
 * @note    In this port the RT counter is the same counter used for
 *          system time in tick-less mode, resolution is always 1uS.
 *
 * @return              The realtime counter value.
 */
__STATIC_FORCEINLINE rtcnt_t port_rt_get_counter_value(void) {

  return TIMER0->TIMERAWL;
}

/**
 * @brief   Returns a core index.
 * @return              The core identifier from 0 to @p PORT_CORES_NUMBER - 1.
 */
__STATIC_INLINE core_id_t port_get_core_id(void) {

  return SIO->CPUID;
}

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* CHCORESMP_H */

/** @} */
