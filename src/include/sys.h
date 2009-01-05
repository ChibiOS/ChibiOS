/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @addtogroup Core
 * @{
 */

#ifndef _SYS_H_
#define _SYS_H_

#if defined(CH_USE_REENTRANT_LOCKS) || defined(_DOXYGEN_)
/**
 * Enters the ChibiOS/RT system mutual exclusion zone.
 * @note The use of system mutual exclusion zone is not recommended in
 *       the user code, it is a better idea to use the semaphores or mutexes
 *       instead.
 * @note The code of this API is always inlined regardless the
 *       @p CH_OPTIMIZE_SPEED setting. This function is meant to be used in
 *       places where the performance is always preferred.
 * @see CH_USE_NESTED_LOCKS
 */
#define chSysLockInline() {                                             \
  if (currp->p_locks == 0) {                                            \
    currp->p_locks++;                                                   \
    sys_disable();                                                      \
  }                                                                     \
}

/**
 * Leaves the ChibiOS/RT system mutual exclusion zone.
 * @note The use of system mutual exclusion zone is not recommended in
 *       the user code, it is a better idea to use the semaphores or mutexes
 *       instead.
 * @note The code of this API is always inlined regardless the
 *       @p CH_OPTIMIZE_SPEED setting. This function is meant to be used in
 *       places where the performance is always preferred.
 * @see CH_USE_NESTED_LOCKS
 */
#define chSysUnlockInline() {                                           \
  if (--currp->p_locks == 0)                                            \
    sys_enable();                                                       \
}
#else /* defined(CH_USE_REENTRANT_LOCKS) || defined(_DOXYGEN_) */
#define chSysLockInline() sys_disable()
#define chSysUnlockInline() sys_enable()
#endif /* !defined(CH_USE_REENTRANT_LOCKS) && !defined(_DOXYGEN_) */

#if defined(CH_OPTIMIZE_SPEED)
#define chSysLock() chSysLockInline()
#define chSysUnlock chSysUnlockInline()
#endif /* defined(CH_OPTIMIZE_SPEED) */

#ifdef __cplusplus
extern "C" {
#endif
  void chSysInit(void);
  void chSysTimerHandlerI(void);
#ifdef __cplusplus
}
#endif

#endif /* _SYS_H_ */

/** @} */
