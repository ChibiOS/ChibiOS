/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    rp_flash_lockout.c
 * @brief   RP flash lockout (core-parking) implementation.
 * @details Parks the other core in RAM while flash program/erase operations
 *          are in progress. Uses the inter-core FIFO for the handshake.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#include "hal.h"
#include "rp_flash_lockout.h"

#if (RP_EFL_USE_FLASH_LOCKOUT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Macro to place functions in RAM.
 */
#define RAMFUNC __attribute__((noinline, section(".ramtext")))

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Lockout active flag, shared between cores.
 */
static volatile bool lockout_active;

/**
 * @brief   Acknowledgement wait flag, local to the initiating core.
 */
static volatile bool lockout_waiting_ack;

/**
 * @brief   Acknowledgement received flag, local to the initiating core.
 */
static volatile bool lockout_ack_received;

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the flash lockout module.
 * @details Releases the hardware spinlock to a known state. This is
 *          necessary because SIO spinlocks are not cleared by a debug
 *          (SWD) reset — only by a full power-on reset.
 * @note    Must be called once before any lockout acquire, typically
 *          from efl_lld_init().
 */
void rpFlashLockoutInit(void) {

  lockout_active = false;
  SIO->SPINLOCK[RP_FLASH_LOCKOUT_SPINLOCK] = 0U;
}

/**
 * @brief   FIFO message handler called from the FIFO ISR on the remote core.
 * @details If the message is a lockout request, this function acknowledges
 *          and spins in RAM until the lockout is released.
 * @note    This function MUST be in RAM.
 *
 * @param[in] msg       the FIFO message received
 */
RAMFUNC void rpFlashLockoutFifoMsg(unsigned msg) {
  unsigned primask;

  if ((msg == RP_FIFO_FLASH_LOCKOUT_ACK) && lockout_waiting_ack) {
    lockout_ack_received = true;
    __DMB();
    __SEV();
    return;
  }

  if (msg != RP_FIFO_FLASH_LOCKOUT_MSG) {
    return;
  }

  /* Save and disable interrupts on this core. */
  primask = __get_PRIMASK();
  __disable_irq();

  /* Acknowledge the lockout request. */
  SIO->FIFO_WR = RP_FIFO_FLASH_LOCKOUT_ACK;
  __SEV();

  /* Spin in RAM until released. */
  while (lockout_active) {
    __WFE();
  }

  /* Restore interrupt state. */
  __set_PRIMASK(primask);
}

/**
 * @brief   Acquires the flash lockout, parking the other core.
 * @details Sends a lockout request via the inter-core FIFO and waits for
 *          the acknowledgement. The other core's FIFO ISR will call
 *          @p rpFlashLockoutFifoMsg() which parks it in RAM.
 * @note    This function MUST be in RAM.
 */
RAMFUNC void rpFlashLockoutAcquire(void) {
  /* Serialize entry: only one core may attempt lockout at a time.
   * The losing core spins here with interrupts enabled, so its FIFO ISR
   * can still service a lockout request from the winning core. */
  while (SIO->SPINLOCK[RP_FLASH_LOCKOUT_SPINLOCK] == 0U) {
  }

  /* Signal the lockout is active before sending the message. */
  lockout_ack_received = false;
  lockout_waiting_ack = true;
  lockout_active = true;
  __DMB();

  /* Send lockout request to the other core, waiting for FIFO space. */
  while ((SIO->FIFO_ST & SIO_FIFO_ST_RDY) == 0U) {
    __WFE();
  }
  SIO->FIFO_WR = RP_FIFO_FLASH_LOCKOUT_MSG;
  __SEV();

  /* Wait until the FIFO ISR receives the acknowledgement. */
  while (!lockout_ack_received) {
    __WFE();
  }

  lockout_waiting_ack = false;
  __DMB();
}

/**
 * @brief   Releases the flash lockout, waking the parked core.
 * @note    This function MUST be in RAM.
 */
RAMFUNC void rpFlashLockoutRelease(void) {
  /* Clear the lockout flag. */
  lockout_active = false;
  __DMB();

  /* Wake the parked core. */
  __SEV();

  /* Release the hardware spinlock so the other core can acquire. */
  SIO->SPINLOCK[RP_FLASH_LOCKOUT_SPINLOCK] = 0U;
}

#endif /* RP_EFL_USE_FLASH_LOCKOUT == TRUE */

/** @} */
