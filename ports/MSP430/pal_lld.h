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
 * @file ports/MSP430/pal_lld.h
 * @brief MSP430 Digital I/O low level driver
 * @addtogroup MSP430_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#include <msp430x16x.h>

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief Generic MSP430 I/O port.
 */
union __ioport {
  struct {
    ioregister_t  in;
    ioregister_t  out;
    ioregister_t  dir;
  } iop_common;
  struct port_simple_t iop_simple;
  struct port_full_t iop_full;
};

/**
 * @brief Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 8

/**
 * @brief Digital I/O port sized unsigned type.
 */
typedef uint8_t ioportmask_t;

/**
 * @brief Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef union __ioport * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief I/O port A identifier.
 * @details This port identifier is mapped on the MSP430 port 1 (P1).
 */
#if defined(__MSP430_HAS_PORT1__) ||                                    \
    defined(__MSP430_HAS_PORT1_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT_A        ((ioportid_t)0x0020)
#endif

/**
 * @brief I/O port B identifier.
 * @details This port identifier is mapped on the MSP430 port 2 (P2).
 */
#if defined(__MSP430_HAS_PORT2__) ||                                    \
    defined(__MSP430_HAS_PORT2_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT_B        ((ioportid_t)0x0028)
#endif

/**
 * @brief I/O port C identifier.
 * @details This port identifier is mapped on the MSP430 port 3 (P3).
 */
#if defined(__MSP430_HAS_PORT3__) ||                                    \
    defined(__MSP430_HAS_PORT3_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT_C        ((ioportid_t)0x0018)
#endif

/**
 * @brief I/O port D identifier.
 * @details This port identifier is mapped on the MSP430 port 4 (P4).
 */
#if defined(__MSP430_HAS_PORT4__) ||                                    \
    defined(__MSP430_HAS_PORT4_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT_D        ((ioportid_t)0x001c)
#endif

/**
 * @brief I/O port E identifier.
 * @details This port identifier is mapped on the MSP430 port 5 (P5).
 */
#if defined(__MSP430_HAS_PORT5__) ||                                    \
    defined(__MSP430_HAS_PORT5_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT_E        ((ioportid_t)0x0030)
#endif

/**
 * @brief I/O port F identifier.
 * @details This port identifier is mapped on the MSP430 port 6 (P6).
 */
#if defined(__MSP430_HAS_PORT6__) ||                                    \
    defined(__MSP430_HAS_PORT6_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT_F        ((ioportid_t)0x0034)
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in a file named pal_lld.c.               */
/*===========================================================================*/

/**
 * @brief Low level PAL subsystem initialization.
 */
#define pal_lld_init()

/**
 * @brief Reads the physical I/O port states.
 * @details This function is implemented by reading the PxIN register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @return The port bits.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readport(port) ((port)->iop_common.in.reg_p)

/**
 * @brief Reads the output latch.
 * @details This function is implemented by reading the PxOUT register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @return The latched logical states.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readlatch(port) ((port)->iop_common.out.reg_p)

/**
 * @brief Writes a bits mask on a I/O port.
 * @details This function is implemented by writing the PxOUT register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writeport(port, bits) {                                 \
  (port)->iop_common.out.reg_p = (bits);                                \
}

/**
 * @brief Set pins direction.
 * @details This function programs the pins direction within a port.
 */
#define pal_lld_msp430_set_direction(port, dirmask) {                   \
  (port)->iop_common.dir.reg_p = (dirmask);                             \
}

#endif /* _PAL_LLD_H_ */

/** @} */
