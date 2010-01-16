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
 * @file pal.c
 * @brief I/O Ports Abstraction Layer code.
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Read from an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @return The bus logical states.
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The function internally uses the @p palReadGroup() macro. The use of
 *       this function is preferred when you value code size, readability  and
 *       error checking over speed.
 */
ioportmask_t palReadBus(IOBus *bus) {

  chDbgCheck((bus != NULL) &&
             (bus->bus_offset > PAL_IOPORTS_WIDTH), "palReadBus");

  return palReadGroup(bus->bus_portid, bus->bus_mask, bus->bus_offset);
}

/**
 * @brief Write to an I/O bus.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @param[in] bits the bits to be written on the I/O bus. Values exceeding
 *            the bus width are masked so most significant bits are lost.
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 */
void palWriteBus(IOBus *bus, ioportmask_t bits) {

  chDbgCheck((bus != NULL) &&
             (bus->bus_offset > PAL_IOPORTS_WIDTH), "palWriteBus");

  palWriteGroup(bus->bus_portid, bus->bus_mask, bus->bus_offset, bits);
}

/**
 * @brief Programs a bus with the specified mode.
 *
 * @param[in] bus the I/O bus, pointer to a @p IOBus structure
 * @param[in] mode the mode
 *
 * @note The operation is not guaranteed to be atomic on all the architectures,
 *       for atomicity and/or portability reasons you may need to enclose port
 *       I/O operations between @p chSysLock() and @p chSysUnlock().
 * @note The default implementation is non atomic and not necessarily
 *       optimal. Low level drivers may  optimize the function by using
 *       specific hardware or coding.
 */
void palSetBusMode(IOBus *bus, uint_fast8_t mode) {

  chDbgCheck((bus != NULL) &&
             (bus->bus_offset > PAL_IOPORTS_WIDTH), "palSetBusMode");

  palSetGroupMode(bus->bus_portid, bus->bus_mask, mode);
}

#endif /* CH_HAL_USE_PAL */

/** @} */
