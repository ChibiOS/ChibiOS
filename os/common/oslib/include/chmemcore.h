/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

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
 * @file    chmemcore.h
 * @brief   Core memory manager macros and structures.
 *
 * @addtogroup memcore
 * @{
 */

#ifndef CHMEMCORE_H
#define CHMEMCORE_H

#if (CH_CFG_USE_MEMCORE == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Managed RAM size.
 * @details Size of the RAM area to be managed by the OS. If set to zero
 *          then the whole available RAM is used. The core memory is made
 *          available to the heap allocator and/or can be used directly through
 *          the simplified core memory allocator.
 *
 * @note    In order to let the OS manage the whole RAM the linker script must
 *          provide the @p __heap_base__ and @p __heap_end__ symbols.
 * @note    Requires @p CH_CFG_USE_MEMCORE.
 */
#if !defined(CH_CFG_MEMCORE_SIZE) || defined(__DOXYGEN__)
#define CH_CFG_MEMCORE_SIZE                 0
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_CFG_MEMCORE_SIZE < 0
#error "invalid CH_CFG_MEMCORE_SIZE value specified"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Memory get function.
 */
typedef void *(*memgetfunc_t)(size_t size, unsigned align);

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void _core_init(void);
  void *chCoreAllocAlignedI(size_t size, unsigned align);
  void *chCoreAllocAligned(size_t size, unsigned align);
  size_t chCoreGetStatusX(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Allocates a memory block.
 * @details The allocated block is guaranteed to be properly aligned for a
 *          pointer data type.
 *
 * @param[in] size      the size of the block to be allocated.
 * @return              A pointer to the allocated memory block.
 * @retval NULL         allocation failed, core memory exhausted.
 *
 * @iclass
 */
static inline void *chCoreAllocI(size_t size) {

  return chCoreAllocAlignedI(size, PORT_NATURAL_ALIGN);
}

/**
 * @brief   Allocates a memory block.
 * @details The allocated block is guaranteed to be properly aligned for a
 *          pointer data type.
 *
 * @param[in] size      the size of the block to be allocated.
 * @return              A pointer to the allocated memory block.
 * @retval NULL         allocation failed, core memory exhausted.
 *
 * @api
 */
static inline void *chCoreAlloc(size_t size) {

  return chCoreAllocAligned(size, PORT_NATURAL_ALIGN);
}

#endif /* CH_CFG_USE_MEMCORE == TRUE */

#endif /* CHMEMCORE_H */

/** @} */
