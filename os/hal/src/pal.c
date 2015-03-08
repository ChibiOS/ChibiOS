/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

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
 * @file    pal.c
 * @brief   I/O Ports Abstraction Layer code.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Read from an I/O bus.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p chSysLock() and
 *          @p chSysUnlock().
 * @note    The function internally uses the @p palReadGroup() macro. The use
 *          of this function is preferred when you value code size, readability
 *          and error checking over speed.
 * @note    The function can be called from any context.
 *
 * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
 * @return              The bus logical states.
 *
 * @special
 */
ioportmask_t palReadBus(IOBus *bus) {

  osalDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH));

  return palReadGroup(bus->portid, bus->mask, bus->offset);
}

/**
 * @brief   Write to an I/O bus.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p chSysLock() and
 *          @p chSysUnlock().
 * @note    The default implementation is non atomic and not necessarily
 *          optimal. Low level drivers may  optimize the function by using
 *          specific hardware or coding.
 * @note    The function can be called from any context.
 *
 * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
 * @param[in] bits      the bits to be written on the I/O bus. Values exceeding
 *                      the bus width are masked so most significant bits are
 *                      lost.
 *
 * @special
 */
void palWriteBus(IOBus *bus, ioportmask_t bits) {

  osalDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH));

  palWriteGroup(bus->portid, bus->mask, bus->offset, bits);
}

/**
 * @brief   Programs a bus with the specified mode.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p chSysLock() and
 *          @p chSysUnlock().
 * @note    The default implementation is non atomic and not necessarily
 *          optimal. Low level drivers may  optimize the function by using
 *          specific hardware or coding.
 * @note    The function can be called from any context.
 *
 * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
 * @param[in] mode      the mode
 *
 * @special
 */
void palSetBusMode(IOBus *bus, iomode_t mode) {

  osalDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH));

  palSetGroupMode(bus->portid, bus->mask, bus->offset, mode);
}

#endif /* HAL_USE_PAL == TRUE */

/** @} */
