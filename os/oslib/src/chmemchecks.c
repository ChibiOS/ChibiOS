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
 * @file    oslib/src/chmemchecks.c
 * @brief   Memory areas and pointers validation code.
 *
 * @addtogroup oslib_memareas
 * @details Memory areas and pointers validation utilities.
 * @note    Always available.
 * @note    Compatible with RT and NIL.
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

#if (CH_CFG_USE_MEMCHECKS == TRUE) || defined(__DOXYGEN__)

/**
 * @brief   Default writable memory areas.
 * @details By default all memory is writable, user must provide its own
 *          writable areas array for the device in use.
 * @note    The array is terminated by an end marker (base=-1).
 */
CC_WEAK memory_area_t __ch_mem_writable_areas[] = {
  {(uint8_t *)0,  0U},      /* Whole space is writable. */
  {(uint8_t *)-1, 0U},
};

/**
 * @brief   Default readable memory areas.
 * @details By default all memory is readable, user must provide its own
 *          readable areas array for the device in use.
 * @note    The array is terminated by an end marker (base=-1).
 */
CC_WEAK memory_area_t __ch_mem_readable_areas[] = {
  {(uint8_t *)0,  0U},      /* Whole space is readable. */
  {(uint8_t *)-1, 0U},
};

#endif /* CH_CFG_USE_MEMCHECKS == TRUE */

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Memory area check.
 * @details Checks if specified area belongs to one of the specified areas.
 *
 * @param[in] map       array of valid areas terminated with an end
 *                      marker (base=-1)
 * @param[in] p         pointer to the area to be checked
 * @param[in] size      size of the area to be checked
 * @return              The test result.
 * @retval false        if the area is entirely contained within one of the
 *                      specified areas.
 * @retval true         if the area check failed.
 *
 * @xclass
 */
bool chMemIsAreaContainedX(const memory_area_t areas[],
                           const void *p,
                           size_t size) {
  const memory_area_t *map = &areas[0];

  chDbgCheck(p != NULL);

  /* Scanning the array of the valid areas for a mismatch.*/
  while (map->base != (uint8_t *)-1) {
    if (chMemIsAreaWithinX(map, p, size)) {
      return true;
    }
    map++;
  }

  return false;
}

#if (CH_CFG_USE_MEMCHECKS == TRUE) || defined(__DOXYGEN__)

/**
 * @brief   Memory writable area check.
 * @details Checks if specified pointer belongs to one of the system-defined
 *          writable areas and is aligned as specified.
 * @note    This function is only effective if @p CH_CFG_SYS_WRITABLE_REGIONS
 *          is defined, if it is not defined then just the alignment of
 *          the pointer is checked.
 * @note    @p __ch_mem_writable_areas must be the name of a global
 *          @p memory_area_t array terminated with an end marker (-1, 0).
 *
 * @param[in] p         pointer to the area to be checked
 * @param[in] align     required pointer alignment to be checked, must be
 *                      a power of two
 * @return              The test result.
 * @retval false        if the area is entirely contained within one of the
 *                      system-defined writable areas.
 * @retval true         if the area check failed.
 *
 * @xclass
 */
bool chMemIsAreaWritableX(const void *p,
                          size_t size,
                          unsigned align) {

  chDbgCheck((align & (align - 1U)) == 0U);

  if (!MEM_IS_ALIGNED(p, align)) {
    return true;
  }

  return chMemIsAreaContainedX(__ch_mem_writable_areas, p, size);
}

/**
 * @brief   Memory readable area check.
 * @details Checks if specified pointer belongs to one of the system-defined
 *          readable areas and is aligned as specified.
 * @note    This function is only effective if @p CH_CFG_SYS_READABLE_REGIONS
 *          is defined, if it is not defined then just the alignment of
 *          the pointer is checked.
 * @note    @p __ch_mem_readable_areas must be the name of a global
 *          @p memory_area_t array terminated with an end marker (-1, 0).
 *
 * @param[in] p         pointer to the area to be checked
 * @param[in] align     required pointer alignment to be checked, must be
 *                      a power of two
 * @return              The test result.
 * @retval false        if the area is entirely contained within one of the
 *                      system-defined readable areas.
 * @retval true         if the area check failed.
 *
 * @xclass
 */
bool chMemIsAreaReadableX(const void *p,
                          size_t size,
                          unsigned align) {

  chDbgCheck((align & (align - 1U)) == 0U);

  if (!MEM_IS_ALIGNED(p, align)) {
    return true;
  }

  return chMemIsAreaContainedX(__ch_mem_readable_areas, p, size);
}

#endif /* CH_CFG_USE_MEMCHECKS == TRUE */

/** @} */
