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
 * @file    ARMv8-M-ML-ALT/smp/rp2/chcoresmp.c
 * @brief   ARMv8-M-ML-ALT RP2 SMP code.
 *
 * @addtogroup ARMV8M_ML_ALT_CORE_SMP_RP2
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void port_local_halt(void) {
  const char *reason = "remote panic";

  port_disable();

  __trace_halt("remote panic");

  currcore->dbg.panic_msg = reason;

  CH_CFG_SYSTEM_HALT_HOOK(reason);

  while (true) {
  }
}

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   Single FIFO interrupt handler for both cores (RP2350).
 * @note    RP2350 uses a shared SIO_IRQ_FIFO (IRQ 25) unlike RP2040 which
 *          has separate IRQs per core.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorA4) {

  CH_IRQ_PROLOGUE();

  SIO->FIFO_ST = SIO_FIFO_ST_ROE | SIO_FIFO_ST_WOF;

  while ((SIO->FIFO_ST & SIO_FIFO_ST_VLD) != 0U) {
    uint32_t message = SIO->FIFO_RD;
    /* Core 1 checks for panic from Core 0.*/
    if ((message == PORT_FIFO_PANIC_MESSAGE) && (port_get_core_id() == 1U)) {
      port_local_halt();
    }
#if defined(PORT_HANDLE_FIFO_MESSAGE)
    if (message != PORT_FIFO_RESCHEDULE_MESSAGE) {
      PORT_HANDLE_FIFO_MESSAGE(port_get_core_id() ^ 1U, message);
    }
#else
    (void)message;
#endif
  }

  __SEV();

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   SMP-related port initialization.
 *
 * @param[in, out] oip  pointer to the @p os_instance_t structure
 */
void __port_smp_init(os_instance_t *oip) {

#if CH_CFG_ST_TIMEDELTA > 0
  /* Activating timer for this instance.*/
  port_timer_enable(oip);
#endif

#if CH_CFG_SMP_MODE == TRUE
  /* FIFO handler for this core. RP2350 uses a single shared IRQ 25.*/
  SIO->FIFO_ST = SIO_FIFO_ST_ROE | SIO_FIFO_ST_WOF;
  NVIC_SetPriority(SIO_IRQ_FIFOn, CORTEX_MINIMUM_PRIORITY);
  NVIC_EnableIRQ(SIO_IRQ_FIFOn);
#endif

  (void)oip;
}

/**
 * @brief   Takes the kernel spinlock.
 */
void __port_spinlock_take(void) {

  port_spinlock_take();
}

/**
 * @brief   Releases the kernel spinlock.
 */
void __port_spinlock_release(void) {

  port_spinlock_release();
}

/** @} */
