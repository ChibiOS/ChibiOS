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
 * @file    chmem.h
 * @brief   Memory alignment macros and structures.
 *
 * @addtogroup mem
 * @{
 */

#ifndef CHALIGN_H
#define CHALIGN_H

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

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Memory alignment support macros
 */
/**
 * @brief   Alignment mask constant.
 *
 * @param[in] a         alignment, must be a power of two
 */
#define MEM_ALIGN_MASK(a)       ((size_t)(a) - 1U)

/**
 * @brief   Aligns to the previous aligned memory address.
 *
 * @param[in] p         variable to be aligned
 * @param[in] a         alignment, must be a power of two
 */
#define MEM_ALIGN_PREV(p, a)    ((size_t)(p) & ~MEM_ALIGN_MASK(a))

/**
 * @brief   Aligns to the new aligned memory address.
 *
 * @param[in] p         variable to be aligned
 * @param[in] a         alignment, must be a power of two
 */
#define MEM_ALIGN_NEXT(p, a)    MEM_ALIGN_PREV((size_t)(p) +                \
                                               MEM_ALIGN_MASK(a), (a))

/**
 * @brief   Returns whatever a pointer or memory size is aligned.
 *
 * @param[in] p         variable to be aligned
 * @param[in] a         alignment, must be a power of two
 */
#define MEM_IS_ALIGNED(p, a)    (((size_t)(p) & MEM_ALIGN_MASK(a)) == 0U)

/**
 * @brief   Returns whatever a constant is a valid alignment.
 * @details Valid alignments are powers of two.
 *
 * @param[in] a         alignment to be checked, must be a constant
 */
#define MEM_IS_VALID_ALIGNMENT(a)                                           \
  (((size_t)(a) != 0U) && (((size_t)(a) & ((size_t)(a) - 1U)) == 0U))
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

#endif /* CHALIGN_H */

/** @} */
