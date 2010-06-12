/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    memstreams.c
 * @brief   Memory streams code.
 *
 * @addtogroup memory_streams
 * @{
 */

#include <string.h>

#include "ch.h"
#include "memstreams.h"

/*
 * @brief   Write virtual method implementation.
 *
 * @param[in] ip        pointer to a @p MemoryStream object
 * @param[in] bp        pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The number of bytes transferred. The return value can
 *                      be less than the specified number of bytes if the
 *                      stream reaches a physical end of file and cannot be
 *                      extended.
 */
static size_t writes(void *ip, const uint8_t *bp, size_t n) {
  MemoryStream *msp = ip;

  if (msp->size - msp->eos < n)
    n = msp->size - msp->eos;
  memcpy(msp->buffer + msp->eos, bp, n);
  msp->eos += n;
  return n;
}

/*
 * @brief   Read virtual method implementation.
 *
 * @param[in] ip        pointer to a @p MemoryStream object
 * @param[out] bp       pointer to the data buffer
 * @param[in] n         the maximum amount of data to be transferred
 * @return              The number of bytes transferred. The return value can
 *                      be less than the specified number of bytes if the
 *                      stream reaches the end of the available data.
 */
static size_t reads(void *ip, uint8_t *bp, size_t n) {
  MemoryStream *msp = ip;

  if (msp->eos - msp->offset < n)
    n = msp->eos - msp->offset;
  memcpy(bp, msp->buffer + msp->offset, n);
  msp->offset += n;
  return n;
}

static const struct MemStreamVMT vmt = {writes, reads};

/**
 * @brief   Memory stream object initialization.
 *
 * @param[out] msp      pointer to the @p MemoryStream object to be initialized
 * @param[in] buffer    pointer to the memory buffer for the memory stream
 * @param[in] size      total size of the memory stream buffer
 * @param[in] eos       initial End Of Stream offset. Normally you need to
 *                      put this to zero for RAM buffers or equal to @p size
 *                      for ROM streams.
 */
void msObjectInit(MemoryStream *msp, uint8_t *buffer, size_t size, size_t eos) {

  msp->vmt    = &vmt;
  msp->buffer = buffer;
  msp->size   = size;
  msp->eos    = eos;
  msp->offset = 0;
}

/** @} */
