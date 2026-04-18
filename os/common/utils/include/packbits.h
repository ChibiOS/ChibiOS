/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    packbits.h
 * @brief   PackBits codec header file.
 *
 * @addtogroup UTILS_PACKBITS
 * @{
 */

#ifndef PACKBITS_H
#define PACKBITS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Maximum size of a single PackBits literal run.
 */
#define PACKBITS_LITERAL_MAX             128U

/**
 * @brief   Maximum size of a single PackBits repeated run.
 */
#define PACKBITS_REPEAT_MAX              128U

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

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  size_t packbits_encode_bound(size_t src_size);
  bool packbits_encode(const uint8_t *src, size_t src_size,
                       uint8_t *dst, size_t dst_size,
                       size_t *encoded_sizep);
  bool packbits_decode_slice(const uint8_t *src, size_t src_size,
                             size_t dst_offset,
                             uint8_t *dst, size_t dst_size,
                             size_t decoded_size);
  bool packbits_decode(const uint8_t *src, size_t src_size,
                       uint8_t *dst, size_t dst_size);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* PACKBITS_H */

/** @} */
