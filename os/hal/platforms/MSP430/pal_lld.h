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
 * @file MSP430/pal_lld.h
 * @brief MSP430 Digital I/O low level driver header.
 * @addtogroup MSP430_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if CH_HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

#undef PAL_MODE_INPUT_PULLUP
#undef PAL_MODE_INPUT_PULLDOWN
#undef PAL_MODE_INPUT_ANALOG
#undef PAL_MODE_OUTPUT_OPENDRAIN

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief Simplified MSP430 I/O port representation.
 * @details This structure represents the common part of all the MSP430 I/O
 *          ports.
 */
struct port_common_t {
  ioregister_t  in;
  ioregister_t  out;
  ioregister_t  dir;
};

/**
 * @brief Generic MSP430 I/O port.
 */
union __ioport {
  struct port_common_t iop_common;
  struct port_simple_t iop_simple;
  struct port_full_t iop_full;
};

/**
 * @brief Setup registers common to all the MSP430 ports.
 */
typedef struct  {
  ioregister_t  out;
  ioregister_t  dir;
} msp430_dio_setup_t;

/**
 * @brief MSP430 I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialize the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
#if defined(__MSP430_HAS_PORT1__) ||                                    \
    defined(__MSP430_HAS_PORT1_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 1 setup data.*/
  msp430_dio_setup_t    P1Data;
#endif
#if defined(__MSP430_HAS_PORT2__) ||                                    \
    defined(__MSP430_HAS_PORT2_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 2 setup data.*/
  msp430_dio_setup_t    P2Data;
#endif
#if defined(__MSP430_HAS_PORT3__) ||                                    \
    defined(__MSP430_HAS_PORT3_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 3 setup data.*/
  msp430_dio_setup_t    P3Data;
#endif
#if defined(__MSP430_HAS_PORT4__) ||                                    \
    defined(__MSP430_HAS_PORT4_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 4 setup data.*/
  msp430_dio_setup_t    P4Data;
#endif
#if defined(__MSP430_HAS_PORT5__) ||                                    \
    defined(__MSP430_HAS_PORT5_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 5 setup data.*/
  msp430_dio_setup_t    P5Data;
#endif
#if defined(__MSP430_HAS_PORT6__) ||                                    \
    defined(__MSP430_HAS_PORT6_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 6 setup data.*/
  msp430_dio_setup_t    P6Data;
#endif
} PALConfig;

/**
 * @brief Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 8

/**
 * @brief Whole port mask.
 * @brief This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFF)

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
#define IOPORT1         ((ioportid_t)0x0020)
#endif

/**
 * @brief I/O port B identifier.
 * @details This port identifier is mapped on the MSP430 port 2 (P2).
 */
#if defined(__MSP430_HAS_PORT2__) ||                                    \
    defined(__MSP430_HAS_PORT2_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT2         ((ioportid_t)0x0028)
#endif

/**
 * @brief I/O port C identifier.
 * @details This port identifier is mapped on the MSP430 port 3 (P3).
 */
#if defined(__MSP430_HAS_PORT3__) ||                                    \
    defined(__MSP430_HAS_PORT3_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT3         ((ioportid_t)0x0018)
#endif

/**
 * @brief I/O port D identifier.
 * @details This port identifier is mapped on the MSP430 port 4 (P4).
 */
#if defined(__MSP430_HAS_PORT4__) ||                                    \
    defined(__MSP430_HAS_PORT4_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT4         ((ioportid_t)0x001c)
#endif

/**
 * @brief I/O port E identifier.
 * @details This port identifier is mapped on the MSP430 port 5 (P5).
 */
#if defined(__MSP430_HAS_PORT5__) ||                                    \
    defined(__MSP430_HAS_PORT5_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT5         ((ioportid_t)0x0030)
#endif

/**
 * @brief I/O port F identifier.
 * @details This port identifier is mapped on the MSP430 port 6 (P6).
 */
#if defined(__MSP430_HAS_PORT6__) ||                                    \
    defined(__MSP430_HAS_PORT6_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT6         ((ioportid_t)0x0034)
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in a file named pal_lld.c.               */
/*===========================================================================*/

/**
 * @brief Low level PAL subsystem initialization.
 * @details In MSP430 programs all the ports as input.
 *
 * @param[in] config the MSP430 ports configuration
 */
#define pal_lld_init(config) _pal_lld_init(config)

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
 * @brief Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] mode the mode
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note @p PAL_MODE_UNCONNECTED is implemented as output as recommended by
 *       the MSP430x1xx Family User's Guide.
 * @note This function does not alter the @p PxSEL registers. Alternate
 *       functions setup must be handled by device-specific code.
 */
#define pal_lld_setgroupmode(port, mask, mode) \
  _pal_lld_setgroupmode(port, mask, mode)

extern const PALConfig pal_default_config;

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(const PALConfig *config);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             uint_fast8_t mode);
#ifdef __cplusplus
}
#endif

#endif /* _PAL_LLD_H_ */

#endif /* CH_HAL_USE_PAL */

/** @} */
