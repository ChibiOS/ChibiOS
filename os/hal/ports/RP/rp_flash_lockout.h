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
 * @file    rp_flash_lockout.h
 * @brief   RP flash lockout (core-parking) API header.
 * @details When RP_EFL_USE_FLASH_LOCKOUT is TRUE the other core is parked
 *          in RAM before any flash program/erase/UID-read operation.
 *          The caller must route FIFO IRQ messages through
 *          @p rpFlashLockoutFifoMsg() from @p PORT_HANDLE_FIFO_MESSAGE
 *          in @p chconf.h.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#ifndef RP_FLASH_LOCKOUT_H
#define RP_FLASH_LOCKOUT_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   FIFO message requesting the remote core to park.
 */
#define RP_FIFO_FLASH_LOCKOUT_MSG           0xFFFFFFFDU

/**
 * @brief   FIFO acknowledgement sent by the parked core.
 */
#define RP_FIFO_FLASH_LOCKOUT_ACK           0xFFFFFFFCU

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Enables the flash lockout (core-parking) mechanism.
 * @note    Requires SMP mode with both cores running. Enable this in
 *          mcuconf.h and wire PORT_HANDLE_FIFO_MESSAGE in chconf.h.
 */
#if !defined(RP_EFL_USE_FLASH_LOCKOUT) || defined(__DOXYGEN__)
#define RP_EFL_USE_FLASH_LOCKOUT            FALSE
#endif

/**
 * @brief   Hardware spinlock number used to serialize flash lockout entry.
 * @note    Must not collide with PORT_SPINLOCK_NUMBER (kernel, default 31).
 *          Valid range is 0..31.
 */
#if !defined(RP_FLASH_LOCKOUT_SPINLOCK) || defined(__DOXYGEN__)
#define RP_FLASH_LOCKOUT_SPINLOCK           30U
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (RP_EFL_USE_FLASH_LOCKOUT == TRUE)
#if !defined(CH_CFG_SMP_MODE) || (CH_CFG_SMP_MODE != TRUE)
#error "RP_EFL_USE_FLASH_LOCKOUT requires CH_CFG_SMP_MODE == TRUE"
#endif
#if !defined(PORT_HANDLE_FIFO_MESSAGE)
#error "RP_EFL_USE_FLASH_LOCKOUT requires PORT_HANDLE_FIFO_MESSAGE"
#endif
#if (RP_FLASH_LOCKOUT_SPINLOCK > 31U)
#error "RP_FLASH_LOCKOUT_SPINLOCK out of range (0..31)"
#endif
#if defined(PORT_SPINLOCK_NUMBER) && \
    (RP_FLASH_LOCKOUT_SPINLOCK == PORT_SPINLOCK_NUMBER)
#error "RP_FLASH_LOCKOUT_SPINLOCK collides with PORT_SPINLOCK_NUMBER"
#endif
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__ASSEMBLER__)

#if (RP_EFL_USE_FLASH_LOCKOUT == TRUE) || defined(__DOXYGEN__)

#ifdef __cplusplus
extern "C" {
#endif
  void rpFlashLockoutInit(void);
  void rpFlashLockoutFifoMsg(unsigned msg);
  void rpFlashLockoutAcquire(void);
  void rpFlashLockoutRelease(void);
#ifdef __cplusplus
}
#endif

#else /* RP_EFL_USE_FLASH_LOCKOUT == FALSE */

static inline void rpFlashLockoutInit(void) {}
static inline void rpFlashLockoutFifoMsg(unsigned msg) { (void)msg; }
static inline void rpFlashLockoutAcquire(void) {}
static inline void rpFlashLockoutRelease(void) {}

#endif /* RP_EFL_USE_FLASH_LOCKOUT */

#endif /* !__ASSEMBLER__ */

#endif /* RP_FLASH_LOCKOUT_H */

/** @} */
