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
 * @file    rt/include/chsafety.h
 * @brief   Functional safety support macros and structures.
 *
 * @addtogroup rt_safety
 * @{
 */

#ifndef CHSAFETY_H
#define CHSAFETY_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Masks of executable integrity checks.
 * @{
 */
#define CH_INTEGRITY_RLIST                  1U
#define CH_INTEGRITY_VTLIST                 2U
#define CH_INTEGRITY_REGISTRY               4U
#define CH_INTEGRITY_PORT                   8U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   Macro for data pointers validation.
 * @note    The default implementation is a @p NULL check and an alignment
 *          check but it is possible to provide stronger checks by defining:
 *          - @p CUSTOM_IS_VALID_DATA_POINTER() as a project-defined macro,
 *            this macros takes precedence over other implementations.
 *          - @p PORT_IS_VALID_DATA_POINTER() as a port-provided
 *            default implementation.
 */
#if defined(CUSTOM_IS_VALID_DATA_POINTER) || defined(__DOXYGEN__)
#define SFT_IS_VALID_DATA_POINTER(p)        CUSTOM_IS_VALID_DATA_POINTER()
#elif defined(PORT_IS_VALID_DATA_POINTER)
#define SFT_IS_VALID_DATA_POINTER(p)        PORT_IS_VALID_DATA_POINTER()
#else
#define SFT_IS_VALID_DATA_POINTER(p)                                        \
  (((p) != NULL) && (((unsigned)(p) & (PORT_NATURAL_ALIGN - 1U)) == 0U))
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Safety-related condition assertion.
 * @details If the condition check fails then the kernel panics with a
 *          message and halts.
 * @note    The condition is tested only if the specified check level is
 *          equal or greater to @p CH_CFG_HARDENING_LEVEL setting specified
 *          in @p chconf.h else the macro does nothing.
 * @note    Safety checks with levels from 0 to 2 are also activated when
 *          @p CH_DBG_ENABLE_ASSERTS is set to @p TRUE.
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] l         check level in range 0..3
 * @param[in] c         the condition to be verified to be true
 * @param[in] r         a remark string
 *
 * @api
 */
#if !defined(chSftAssert)
#define chSftAssert(l, c, r) do {                                           \
  /*lint -save -e506 -e774 [2.1, 14.3] Can be a constant by design.*/       \
  if (((l) >= CH_CFG_HARDENING_LEVEL) ||                                    \
      (((l) <= 2) && (CH_DBG_ENABLE_ASSERTS != FALSE))) {                   \
    if (unlikely(!(c))) {                                                   \
  /*lint -restore*/                                                         \
      CH_CFG_SAFETY_CHECK_HOOK(l, __func__);                                \
    }                                                                       \
  }                                                                         \
} while (false)
#endif /* !defined(chSftAssert) */
/** @} */

#if CH_CFG_HARDENING_LEVEL < 1
#define chSftCheckQueue(qp)
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#if CH_CFG_HARDENING_LEVEL >= 1
  void chSftCheckQueue(const void *p);
#endif
  void chSftIntegrityCheckI(unsigned testmask);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Validates a data pointer.
 * @details The purpose of this functionality is early detection of
 *          memory corruption by checking memory-fetched pointers
 *          before dereferencing.
 * @note    This check is enabled at hardening level 2 or higher.
 */
static inline void chSftValidateDataPointerX(const void *p) {

  chSftAssert(2, SFT_IS_VALID_DATA_POINTER(p), "invalid pointer");
}

#endif /* CHSAFETY_H */

/** @} */
