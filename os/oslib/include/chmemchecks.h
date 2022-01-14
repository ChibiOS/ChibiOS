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
 * @brief   Type of a memory area.
 */
typedef struct {
  /**
   * @brief   Memory area base.
   * @note    Value -1 is reserved as end-on-array marker.
   */
  uint8_t                       *base;
  /**
   * @brief   Memory area size.
   * @note    Value 0 represents the whole address space and is only valid
   *          when @p base is also zero.
   */
  size_t                        size;
} memory_area_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const memory_area_t __ch_mem_writable_areas[];
extern const memory_area_t __ch_mem_readable_areas[];
extern const memory_area_t __ch_mem_executable_areas[];
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if CH_CFG_USE_MEMCHECKS == TRUE
  size_t chMemIsStringWithinX(const memory_area_t *map,
                              const char *s,
                              size_t max);
  size_t chMemIsPointersArrayWithinX(const memory_area_t *map,
                                     const void *pp[],
                                     size_t max);
  bool chMemIsSpaceContainedX(const memory_area_t areas[],
                              const void *p,
                              size_t size);
  bool chMemIsSpaceWritableX(void *p,
                             size_t size,
                             unsigned align);
  bool chMemIsSpaceReadableX(const void *p,
                             size_t size,
                             unsigned align);
  bool chMemIsAddressExecutableX(const void *p);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Memory space inclusion check.
 * @details Checks if specified space belongs to the specified memory area.
 *
 * @param[in] map       pointer to a @p memory_area_t structure
 * @param[in] p         pointer to the memory space to be checked
 * @param[in] size      size of the memory space to be checked, zero is
 *                      considered the whole address space
 * @return              The test result.
 * @retval true         if the memory space is entirely contained.
 * @retval false        if the memory space is not entirely contained.
 *
 * @xclass
 */
static inline bool chMemIsSpaceWithinX(const memory_area_t *map,
                                       const void *p,
                                       size_t size) {
  const uint8_t *mem_base = (const uint8_t *)map->base;
  const uint8_t *mem_end  = mem_base + map->size - (size_t)1;
  const uint8_t *base     = (const uint8_t *)p;
  const uint8_t *end      = base + size - (size_t)1;

  chDbgAssert((mem_base <= mem_end) && (base <= end), "invalid memory area");

  return (bool)((base <= end) && (base >= mem_base) && (end <= mem_end));
}

/**
 * @brief   Memory area inclusion check.
 * @details Checks if specified space belongs to the specified memory area.
 *
 * @param[in] map1      pointer to the container @p memory_area_t structure
 * @param[in] map2      pointer to the contained @p memory_area_t structure
 *                      considered the whole address space
 * @return              The test result.
 * @retval true         if the memory space is entirely contained.
 * @retval false        if the memory space is not entirely contained.
 *
 * @xclass
 */
static inline bool chMemIsAreaWithinX(const memory_area_t *map1,
                                      const memory_area_t *map2) {
  const uint8_t *mem_base = (const uint8_t *)map1->base;
  const uint8_t *mem_end  = mem_base + map1->size - (size_t)1;
  const uint8_t *base     = (const uint8_t *)map2->base;
  const uint8_t *end      = base + map2->size - (size_t)1;

  chDbgAssert((mem_base <= mem_end) && (base <= end), "invalid memory area");

  return (bool)((base <= end) && (base >= mem_base) && (end <= mem_end));
}

/**
 * @brief   Memory space intersection check.
 * @details Checks if specified memory space intersects the specified memory
 *          area.
 *
 * @param[in] map       pointer to a @p memory_area_t structure
 * @param[in] p         pointer to the memory space to be checked
 * @param[in] size      size of the memory space to be checked, zero is
 *                      considered the whole address space
 * @return              The test result.
 * @retval true         if the memory space is intersecting.
 * @retval false        if the memory space is not intersecting.
 *
 * @xclass
 */
static inline bool chMemIsSpaceIntersectingX(const memory_area_t *map,
                                             const void *p,
                                             size_t size) {
  const uint8_t *mem_base = (const uint8_t *)map->base;
  const uint8_t *mem_end  = mem_base + map->size - (size_t)1;
  const uint8_t *base     = (const uint8_t *)p;
  const uint8_t *end      = base + size - (size_t)1;

  chDbgAssert((mem_base <= mem_end) && (base <= end), "invalid memory area");

  return (bool)(((base >= mem_base) && (base <= mem_end)) ||
                ((end  >= mem_base) && (end  <= mem_end)));
}

/**
 * @brief   Memory area intersection check.
 * @details Checks if specified memory space intersects the specified memory
 *          area.
 *
 * @param[in] map1      pointer to a @p memory_area_t structure
 * @param[in] map2      pointer to a @p memory_area_t structure
 *                      considered the whole address space
 * @return              The test result.
 * @retval true         if the memory areas are intersecting.
 * @retval false        if the memory areas are not intersecting.
 *
 * @xclass
 */
static inline bool chMemIsAreaIntersectingX(const memory_area_t *map1,
                                            const memory_area_t *map2) {
  const uint8_t *mem_base = (const uint8_t *)map1->base;
  const uint8_t *mem_end  = mem_base + map1->size - (size_t)1;
  const uint8_t *base     = (const uint8_t *)map2->base;
  const uint8_t *end      = base + map2->size - (size_t)1;

  chDbgAssert((mem_base <= mem_end) && (base <= end), "invalid memory area");

  return (bool)(((base >= mem_base) && (base <= mem_end)) ||
                ((end  >= mem_base) && (end  <= mem_end)));
}

#if CH_CFG_USE_MEMCHECKS == FALSE
/* Stub implementations for when the functionality is disabled, areas are
   always reported as valid.*/
bool chMemIsAreaWritableX(const void *p,
                          size_t size,
                          unsigned align) {

  (void)p;
  (void)size;
  (void)align;

  return true;
}

bool chMemIsAreaReadableX(const void *p,
                          size_t size,
                          unsigned align) {

  (void)p;
  (void)size;
  (void)align;

  return true;
}

bool chMemIsAddressExecutableX(const void *p) {

  (void)p;

  return true;
}
#endif

#endif /* CHMEMCHECKS_H */

/** @} */
