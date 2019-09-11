/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/sbapi.h
 * @brief   ARMv7-M sandbox user API macros and structures.
 *
 * @addtogroup ARMV7M_SANDBOX_USERAPI
 * @{
 */

#ifndef SBUSER_H
#define SBUSER_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of system time counter.
 */
typedef uint32_t sb_systime_t;

/**
 * @brief   Type of a message.
 */
typedef uint32_t sb_msg_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name   SVC instruction wrappers.
 *
 * @{
 */
#define __syscall0(x)                                                       \
  asm volatile ("svc " #x : : : "memory")

#define __syscall0r(x)                                                      \
  register uint32_t r0 asm ("r0");                                          \
  asm volatile ("svc " #x : "=r" (r0) : : "memory")

#define __syscall1r(x, p1)                                                  \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0) : "memory")

#define __syscall2r(x, p1, p2)                                              \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0), "r" (r1) : "memory")

#define __syscall3r(x, p1, p2, p3)                                          \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  register uint32_t r2 asm ("r2") = (uint32_t)(p3);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0), "r" (r1),                 \
                                        "r" (r2) : "memory")

#define __syscall4r(x, p1, p2, p3, p4)                                      \
  register uint32_t r0 asm ("r0") = (uint32_t)(p1);                         \
  register uint32_t r1 asm ("r1") = (uint32_t)(p2);                         \
  register uint32_t r2 asm ("r2") = (uint32_t)(p3);                         \
  register uint32_t r3 asm ("r3") = (uint32_t)(p4);                         \
  asm volatile ("svc " #x : "=r" (r0) : "r" (r0), "r" (r1),                 \
                                        "r" (r2), "r" (r3) : "memory")
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

static inline void sbExit(sb_msg_t msg) {

  __syscall1r(1, msg);
}

static inline sb_systime_t sbGetSystemTime(void) {

  __syscall0r(2);
  return (sb_systime_t)r0;
}

static inline uint32_t sbGetFrequency(void) {

  __syscall0r(3);
  return (uint32_t)r0;
}

static inline void sbSleepMilliseconds(uint32_t milliseconds) {

  __syscall1r(4, milliseconds);
}

static inline void sbSleepUntil(sb_systime_t start, sb_systime_t end) {

  __syscall2r(5, start, end);
}

#endif /* SBUSER_H */

/** @} */
