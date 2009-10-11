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
 * @file mem.h
 * @brief Low level memory manager macros and structures.
 * @addtogroup coremem
 * @{
 */

#ifndef _MEM_H_
#define _MEM_H_

#if CH_USE_COREMEM

/**
 * @brief Memory alignment type.
 */
typedef void *align_t;

/**
 * @brief Alignment mask constant.
 */
#define MEM_ALIGN_MASK      (sizeof(align_t) - 1)

/**
 * @brief Alignment helper macro.
 */
#define MEM_ALIGN_SIZE(p)   (((size_t)(p) + ALIGN_MASK) & ~ALIGN_MASK)

#ifdef __cplusplus
extern "C" {
#endif
  void mem_init(void);
  void *chMemAlloc(size_t size);
  void *chMemAllocI(size_t size);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_COREMEM */

#endif /* _MEM_H_ */

/** @} */
