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
 * @file ports/ARM7-AT91SAM7X/pal_lld.h
 * @brief AT91SAM7X PIO low level driver
 * @addtogroup AT91SAM7X_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#ifndef AT91SAM7X256_H
#include "at91lib/AT91SAM7X256.h"
#endif

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 32

/**
 * @brief Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef AT91PS_PIO ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief PIO port A identifier.
 */
#define IOPORT_A        AT91C_BASE_PIOA

/**
 * @brief PIO port B identifier.
 */
#define IOPORT_B        AT91C_BASE_PIOB

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in a file named pal_lld.c.               */
/*===========================================================================*/

/**
 * @brief Low level PAL subsystem initialization.
 * @details Clocks are enabled and both PIO ports are reset to a known state
 *          and all pads are enabled a digital I/Os.
 *
 * @note The other PIO registers are not modified, the PIOs are supposed to
 *       have just been reset. The PIO_PSR is cleared because its status
 *       after the reset is not very clear in the documentation.
 */
#define pal_lld_init() {                                                \
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA) | (1 << AT91C_ID_PIOB); \
  AT91C_BASE_PIOA->PIO_PER  = 0xFFFFFFFF;                               \
  AT91C_BASE_PIOB->PIO_PER  = 0xFFFFFFFF;                               \
}

/**
 * @brief Reads the physical I/O port states.
 * @details This function is implemented by reading the PIO_PDSR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @return The port bits.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readport(port) ((port)->PIO_PDSR)

/**
 * @brief Reads the output latch.
 * @details This function is implemented by reading the PIO_ODSR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @return The latched logical states.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readlatch(port) ((port)->PIO_ODSR)

/**
 * @brief Writes a bits mask on a I/O port.
 * @details This function is implemented by writing the PIO_ODSR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writeport(port, bits) {                                 \
  (port)->PIO_ODSR = (bits);                                            \
}

/**
 * @brief Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the PIO_SODR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_setport(port, bits) {                                   \
  (port)->PIO_SODR = (bits);                                            \
}


/**
 * @brief Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the PIO_CODR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_clearport(port, bits) {                                 \
  (port)->PIO_CODR = (bits);                                            \
}

/**
 * @brief Writes a group of bits.
 * @details This function is implemented by writing the PIO_OWER, PIO_ODSR and
 *          PIO_OWDR registers, the implementation is not atomic because the
 *          multiple accesses.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] offset the group bit offset within the port
 * @param[in] bits the bits to be written. Values exceeding the group width
 *            are masked.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writegroup(port, mask, offset, bits) {                  \
  (port)->PIO_OWER = (mask) << (offset);                                \
  (port)->PIO_ODSR = (bits) << (offset);                                \
  (port)->PIO_OWDR = (mask) << (offset);                                \
}

/**
 * @brief Writes a logical state on an output pad.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 * @param[out] bit the logical value, the value must be @p 0 or @p 1
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writepad(port, pad, bit) pal_lld_writegroup(port, 1, pad, bit)

#endif /* _PAL_LLD_H_ */

/** @} */
