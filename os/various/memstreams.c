/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    memstreams.c
 * @brief   Memory streams code.
 *
 * @addtogroup memory_streams
 * @{
 */

#include <string.h>

#include "ch.h"
#include "memstreams.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static size_t writes(void *ip, const uint8_t *bp, size_t n) {
  MemoryStream *msp = ip;

  if (msp->size - msp->eos < n)
    n = msp->size - msp->eos;
  memcpy(msp->buffer + msp->eos, bp, n);
  msp->eos += n;
  return n;
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {
  MemoryStream *msp = ip;

  if (msp->eos - msp->offset < n)
    n = msp->eos - msp->offset;
  memcpy(bp, msp->buffer + msp->offset, n);
  msp->offset += n;
  return n;
}

static msg_t put(void *ip, uint8_t b) {
  MemoryStream *msp = ip;

  if (msp->size - msp->eos <= 0)
    return RDY_RESET;
  *(msp->buffer + msp->eos) = b;
  msp->eos += 1;
  return RDY_OK;
}

static msg_t get(void *ip) {
  uint8_t b;
  MemoryStream *msp = ip;

  if (msp->eos - msp->offset <= 0)
    return RDY_RESET;
  b = *(msp->buffer + msp->offset);
  msp->offset += 1;
  return b;
}

static const struct MemStreamVMT vmt = {writes, reads, put, get};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

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
void msObjectInit(MemoryStream *msp, uint8_t *buffer,
                  size_t size, size_t eos) {

  msp->vmt    = &vmt;
  msp->buffer = buffer;
  msp->size   = size;
  msp->eos    = eos;
  msp->offset = 0;
}

/** @} */
