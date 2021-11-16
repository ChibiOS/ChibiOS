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
 * @file    oslib/include/chmemchecks.h
 * @brief   Memory areas and pointers validation macros and structures.
 *
 * @addtogroup oslib_memareas
 * @{
 */

#ifndef CHMEMCHECKS_H
#define CHMEMCHECKS_H

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
 * @brief   Type of a memory region.
 */
typedef struct {
  /**
   * @brief   Memory region base.
   * @note    Zero if not used.
   */
  uint8_t                       *base;
  /**
   * @brief   Memory region end (inclusive).
   * @note    Zero if not used.
   */
  uint8_t                       *end;
} memory_region_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#if CH_CFG_USE_MEMCHECKS == TRUE
  bool chMemIsAreaContainedX(const memory_region_t *mrp,
                             const void *base,
                             size_t size);
  bool chMemIsAreaWritableX(const void *p,
                            size_t size,
                            unsigned align);
  bool chMemIsAreaReadableX(const void *p,
                            size_t size,
                            unsigned align);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#if CH_CFG_USE_MEMCHECKS == FALSE
/* Stub implementations for when the functionality is disabled, areas are
   always reported as valid.*/
static inline bool chMemIsAreaContainedX(const memory_region_t *mrp,
                                         const void *base,
                                         size_t size) {

  (void)mrp;
  (void)base;
  (void)size;

  return false;
}

bool chMemIsAreaWritableX(const void *p,
                          size_t size,
                          unsigned align) {

  (void)p;
  (void)size;
  (void)align;

  return false;
}

bool chMemIsAreaReadableX(const void *p,
                          size_t size,
                          unsigned align) {

  (void)p;
  (void)size;
  (void)align;

  return false;
}
#endif

#endif /* CHMEMCHECKS_H */

/** @} */
