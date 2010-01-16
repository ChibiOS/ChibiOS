/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file memcore.h
 * @brief Core memory manager macros and structures.
 * @addtogroup memcore
 * @{
 */

#ifndef _MEMCORE_H_
#define _MEMCORE_H_

/**
 * @brief Memory alignment type.
 */
typedef void *align_t;

/**
 * @brief Memory get function.
 *
 * @note This type must be assignment compatible with the @p chMemAlloc()
 *       function.
 */
typedef void *(*memgetfunc_t)(size_t size);

/**
 * @brief Alignment mask constant.
 */
#define MEM_ALIGN_MASK      (sizeof(align_t) - 1)

/**
 * @brief Alignment helper macro.
 */
#define MEM_ALIGN_SIZE(p)   (((size_t)(p) + MEM_ALIGN_MASK) & ~MEM_ALIGN_MASK)

/**
 * @brief Returns whatever a pointer or memory size is aligned to
 *        the type @p align_t.
 */
#define MEM_IS_ALIGNED(p)   (((size_t)(p) & MEM_ALIGN_MASK) == 0)

#if CH_USE_MEMCORE

#ifdef __cplusplus
extern "C" {
#endif
  void core_init(void);
  void *chCoreAlloc(size_t size);
  void *chCoreAllocI(size_t size);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_MEMCORE */

#endif /* _MEMCORE_H_ */

/** @} */
