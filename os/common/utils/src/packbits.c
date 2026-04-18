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
 * @file    packbits.c
 * @brief   PackBits codec code.
 *
 * @addtogroup UTILS_PACKBITS
 * @{
 */

#include <string.h>

#include "packbits.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static size_t get_repeat_size(const uint8_t *src, size_t src_size) {
  size_t n;

  n = 1U;
  while ((n < src_size) && (n < PACKBITS_REPEAT_MAX) && (src[n] == src[0])) {
    n++;
  }

  return n;
}

static bool put_byte(uint8_t *dst, size_t dst_size, size_t *outposp,
                     uint8_t value) {
  if (*outposp >= dst_size) {
    return false;
  }

  dst[*outposp] = value;
  (*outposp)++;

  return true;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Returns the worst-case encoded size for a source buffer.
 *
 * @param[in] src_size          Source size in bytes.
 * @return                      Worst-case encoded size.
 * @retval 0                    Size overflow.
 */
size_t packbits_encode_bound(size_t src_size) {
  size_t extra;

  if (src_size == 0U) {
    return 0U;
  }

  extra = (src_size + PACKBITS_LITERAL_MAX - 1U) / PACKBITS_LITERAL_MAX;
  if (src_size > (SIZE_MAX - extra)) {
    return 0U;
  }

  return src_size + extra;
}

/**
 * @brief   Encodes a buffer using the PackBits format.
 *
 * @param[in] src               Source buffer.
 * @param[in] src_size          Source size.
 * @param[out] dst              Destination buffer.
 * @param[in] dst_size          Destination buffer size.
 * @param[out] encoded_sizep    Written encoded size.
 * @return                      The operation result.
 * @retval true                 Encoding completed.
 * @retval false                Invalid arguments or destination overflow.
 */
bool packbits_encode(const uint8_t *src, size_t src_size,
                     uint8_t *dst, size_t dst_size,
                     size_t *encoded_sizep) {
  size_t inpos;
  size_t outpos;

  if (encoded_sizep == NULL) {
    return false;
  }

  *encoded_sizep = 0U;

  if (src_size == 0U) {
    return true;
  }

  if ((src == NULL) || (dst == NULL)) {
    return false;
  }

  inpos = 0U;
  outpos = 0U;
  while (inpos < src_size) {
    size_t repeat_size;

    repeat_size = get_repeat_size(&src[inpos], src_size - inpos);
    if (repeat_size >= 3U) {
      if (!put_byte(dst, dst_size, &outpos, (uint8_t)(257U - repeat_size)) ||
          !put_byte(dst, dst_size, &outpos, src[inpos])) {
        return false;
      }

      inpos += repeat_size;
    }
    else {
      size_t literal_start;
      size_t literal_size;

      literal_start = inpos;
      literal_size = 0U;
      while ((inpos < src_size) && (literal_size < PACKBITS_LITERAL_MAX)) {
        repeat_size = get_repeat_size(&src[inpos], src_size - inpos);
        if (repeat_size >= 3U) {
          break;
        }

        inpos++;
        literal_size++;
      }

      if (!put_byte(dst, dst_size, &outpos, (uint8_t)(literal_size - 1U))) {
        return false;
      }

      if (outpos > dst_size - literal_size) {
        return false;
      }
      memcpy(&dst[outpos], &src[literal_start], literal_size);
      outpos += literal_size;
    }
  }

  *encoded_sizep = outpos;

  return true;
}

/**
 * @brief   Decodes a PackBits stream into a fixed-size output slice.
 *
 * @param[in] src               Encoded buffer.
 * @param[in] src_size          Encoded buffer size.
 * @param[in] dst_offset        Offset of the requested slice in the decoded
 *                              stream.
 * @param[out] dst              Destination buffer.
 * @param[in] dst_size          Requested slice size.
 * @param[in] decoded_size      Expected full decoded size.
 * @return                      The operation result.
 * @retval true                 Decoding completed.
 * @retval false                Malformed stream or destination mismatch.
 */
bool packbits_decode_slice(const uint8_t *src, size_t src_size,
                           size_t dst_offset,
                           uint8_t *dst, size_t dst_size,
                           size_t decoded_size) {
  size_t inpos;
  size_t outpos;
  size_t copied;

  if (dst_offset > decoded_size) {
    return false;
  }
  if (dst_size > decoded_size - dst_offset) {
    return false;
  }
  if (decoded_size == 0U) {
    return (src_size == 0U) && (dst_size == 0U);
  }

  if ((src == NULL) || ((dst == NULL) && (dst_size > 0U))) {
    return false;
  }

  inpos = 0U;
  outpos = 0U;
  copied = 0U;
  while (inpos < src_size) {
    uint8_t control;

    control = src[inpos++];
    if (control <= 127U) {
      size_t literal_size;
      size_t take_from;
      size_t take_to;

      literal_size = (size_t)control + 1U;
      if ((src_size - inpos < literal_size) ||
          (outpos > decoded_size - literal_size)) {
        return false;
      }

      take_from = dst_offset;
      if (take_from < outpos) {
        take_from = outpos;
      }
      take_to = dst_offset + dst_size;
      if (take_to > outpos + literal_size) {
        take_to = outpos + literal_size;
      }
      if (take_from < take_to) {
        size_t copy_size;

        copy_size = take_to - take_from;
        memcpy(&dst[copied], &src[inpos + (take_from - outpos)], copy_size);
        copied += copy_size;
      }

      inpos += literal_size;
      outpos += literal_size;
    }
    else if (control >= 129U) {
      size_t repeat_size;
      uint8_t value;
      size_t take_from;
      size_t take_to;

      repeat_size = 257U - (size_t)control;
      if ((inpos >= src_size) || (outpos > decoded_size - repeat_size)) {
        return false;
      }

      value = src[inpos++];

      take_from = dst_offset;
      if (take_from < outpos) {
        take_from = outpos;
      }
      take_to = dst_offset + dst_size;
      if (take_to > outpos + repeat_size) {
        take_to = outpos + repeat_size;
      }
      if (take_from < take_to) {
        size_t copy_size;

        copy_size = take_to - take_from;
        memset(&dst[copied], value, copy_size);
        copied += copy_size;
      }

      outpos += repeat_size;
    }
    else {
      continue;
    }
  }

  return (outpos == decoded_size) && (copied == dst_size);
}

/**
 * @brief   Decodes a PackBits stream into a fixed-size output buffer.
 *
 * @param[in] src               Encoded buffer.
 * @param[in] src_size          Encoded buffer size.
 * @param[out] dst              Destination buffer.
 * @param[in] dst_size          Destination size, also the expected decoded
 *                              size.
 * @return                      The operation result.
 * @retval true                 Decoding completed.
 * @retval false                Malformed stream or destination mismatch.
 */
bool packbits_decode(const uint8_t *src, size_t src_size,
                     uint8_t *dst, size_t dst_size) {

  return packbits_decode_slice(src, src_size, 0U, dst, dst_size, dst_size);
}

/** @} */
