/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file ports/ARM7-LPC214x/lpc214x_ssp.h
 * @brief LPC214x SSP driver macros and structures.
 * @addtogroup LPC214x_SSP
 * @{
 */

#ifndef _LPC214x_SSP_H_
#define _LPC214x_SSP_H_

/**
 * @brief SSP bus mutual exclusion control.
 * @details Configuration parameter, if set to @p TRUE enforces mutual
 * exclusion when invoking @p sspAcquireBus() and @p sspReleaseBus().
 * @note The internally used synchronization mechanism is a @p Semaphore.
 */
#if !defined(LPC214x_SSP_USE_MUTEX) || defined(__DOXYGEN__)
#define LPC214x_SSP_USE_MUTEX TRUE
#endif

#ifdef __cplusplus
}
#endif
  void ssp_init(void);
  void ssp_setup(int cpsr, int cr0, int cr1);
  void sspAcquireBus(void);
  void sspReleaseBus(void);
  void sspRW(uint8_t *in, uint8_t *out, size_t n);
#ifdef __cplusplus
}
#endif

#endif /* _LPC214x_SSP_H_*/

/** @} */
