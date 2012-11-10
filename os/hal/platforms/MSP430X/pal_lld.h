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
 * @file    MSP430/pal_lld.h
 * @brief   MSP430 Digital I/O low level driver header.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if HAL_USE_PAL || defined(__DOXYGEN__)

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
 * @brief   Simple MSP430 I/O port.
 */
struct msp430_port_simple_t {
  volatile uint8_t              in;
  volatile uint8_t              in_h;
  volatile uint8_t              out;
  volatile uint8_t              out_h;
  volatile uint8_t              dir;
  volatile uint8_t              dir_h;
  volatile uint8_t              ren;
  volatile uint8_t              ren_h;
  volatile uint8_t              ds;
  volatile uint8_t              ds_h;
#if defined(__MSP430_HAS_PORT1_R__)  || defined(__MSP430_HAS_PORT2_R__)  || defined(__MSP430_HAS_PORTA_R__) || \
    defined(__MSP430_HAS_PORT3_R__)  || defined(__MSP430_HAS_PORT4_R__)  || defined(__MSP430_HAS_PORTB_R__) || \
    defined(__MSP430_HAS_PORT5_R__)  || defined(__MSP430_HAS_PORT6_R__)  || defined(__MSP430_HAS_PORTC_R__) || \
    defined(__MSP430_HAS_PORT7_R__)  || defined(__MSP430_HAS_PORT8_R__)  || defined(__MSP430_HAS_PORTD_R__) || \
    defined(__MSP430_HAS_PORT9_R__)  || defined(__MSP430_HAS_PORT10_R__) || defined(__MSP430_HAS_PORTE_R__) || \
    defined(__MSP430_HAS_PORT11_R__) || defined(__MSP430_HAS_PORTF_R__)
  volatile uint8_t              sel;
  volatile uint8_t              sel_h;
#endif
};

/**
 * @brief   Full MSP430 I/O port.
 */
struct msp430_port_full_t {
  volatile uint8_t              in;
  volatile uint8_t              in_h;
  volatile uint8_t              out;
  volatile uint8_t              out_h;
  volatile uint8_t              dir;
  volatile uint8_t              dir_h;
  volatile uint8_t              ren;
  volatile uint8_t              ren_h;
  volatile uint8_t              ds;
  volatile uint8_t              ds_h;
#if defined(__MSP430_HAS_PORT1_R__)  || defined(__MSP430_HAS_PORT2_R__)  || defined(__MSP430_HAS_PORTA_R__) || \
    defined(__MSP430_HAS_PORT3_R__)  || defined(__MSP430_HAS_PORT4_R__)  || defined(__MSP430_HAS_PORTB_R__) || \
    defined(__MSP430_HAS_PORT5_R__)  || defined(__MSP430_HAS_PORT6_R__)  || defined(__MSP430_HAS_PORTC_R__) || \
    defined(__MSP430_HAS_PORT7_R__)  || defined(__MSP430_HAS_PORT8_R__)  || defined(__MSP430_HAS_PORTD_R__) || \
    defined(__MSP430_HAS_PORT9_R__)  || defined(__MSP430_HAS_PORT10_R__) || defined(__MSP430_HAS_PORTE_R__) || \
    defined(__MSP430_HAS_PORT11_R__) || defined(__MSP430_HAS_PORTF_R__)
  volatile uint8_t              sel;
  volatile uint8_t              sel_h;
#endif
#if defined(__MSP430_HAS_PORT1_R__) || defined(__MSP430_HAS_PORT2_R__) || defined(__MSP430_HAS_PORTA_R__)
  volatile uint8_t              pad[14];
  volatile uint8_t              ies;
  volatile uint8_t              ies_h;
  volatile uint8_t              ie;
  volatile uint8_t              ie_h;
  volatile uint8_t              ifg;
  volatile uint8_t              ifg_h;
#endif
};

/**
 * @brief   Simplified MSP430 I/O port representation.
 * @details This structure represents the common part of all the MSP430 I/O
 *          ports.
 */
struct msp430_port_common {
  volatile uint8_t              in;
  volatile uint8_t              in_h;
  volatile uint8_t              out;
  volatile uint8_t              out_h;
  volatile uint8_t              dir;
  volatile uint8_t              dir_h;
};

/**
 * @brief   Generic MSP430 I/O port.
 */
typedef union {
  struct msp430_port_common     iop_common;
  struct msp430_port_simple_t   iop_simple;
  struct msp430_port_full_t     iop_full;
} msp430_ioport_t;

/**
 * @brief   Setup registers common to all the MSP430 ports.
 */
typedef struct  {
  volatile uint8_t              out;
  volatile uint8_t              dir;
} msp430_dio_setup_t;

/**
 * @brief   MSP430 I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialize the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
#if defined(__MSP430_HAS_PORT1_R__) ||                                  \
    defined(__MSP430_HAS_PORTA_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 1 setup data.*/
  msp430_dio_setup_t    P1Data;
#endif
#if defined(__MSP430_HAS_PORT2_R__) ||                                  \
    defined(__MSP430_HAS_PORTA_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 2 setup data.*/
  msp430_dio_setup_t    P2Data;
#endif
#if defined(__MSP430_HAS_PORT3_R__) ||                                  \
    defined(__MSP430_HAS_PORTB_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 3 setup data.*/
  msp430_dio_setup_t    P3Data;
#endif
#if defined(__MSP430_HAS_PORT4_R__) ||                                  \
    defined(__MSP430_HAS_PORTB_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 4 setup data.*/
  msp430_dio_setup_t    P4Data;
#endif
#if defined(__MSP430_HAS_PORT5_R__) ||                                  \
    defined(__MSP430_HAS_PORTC_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 5 setup data.*/
  msp430_dio_setup_t    P5Data;
#endif
#if defined(__MSP430_HAS_PORT6_R__) ||                                  \
    defined(__MSP430_HAS_PORTC_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 6 setup data.*/
  msp430_dio_setup_t    P6Data;
#endif
#if defined(__MSP430_HAS_PORT7_R__) ||                                  \
    defined(__MSP430_HAS_PORTD_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 7 setup data.*/
  msp430_dio_setup_t    P7Data;
#endif  
#if defined(__MSP430_HAS_PORT8_R__) ||                                  \
    defined(__MSP430_HAS_PORTD_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 8 setup data.*/
  msp430_dio_setup_t    P8Data;
#endif    
#if defined(__MSP430_HAS_PORT9_R__) ||                                  \
    defined(__MSP430_HAS_PORTE_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 9 setup data.*/
  msp430_dio_setup_t    P9Data;
#endif   
#if defined(__MSP430_HAS_PORT10_R__) ||                                  \
    defined(__MSP430_HAS_PORTE_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 10 setup data.*/
  msp430_dio_setup_t    P10Data;
#endif    
#if defined(__MSP430_HAS_PORT11_R__) ||                                  \
    defined(__MSP430_HAS_PORTF_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 11 setup data.*/
  msp430_dio_setup_t    P11Data;
#endif       
#if defined(__MSP430_HAS_PORTJ_R__) ||                                  \
    defined(__DOXYGEN__)
  /** @brief Port 12 setup data.*/
  msp430_dio_setup_t    P12Data;
#endif     
} PALConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 8

/**
 * @brief   Whole port mask.
 * @details This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFF)

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint8_t ioportmask_t;

/**
 * @brief   Digital I/O modes.
 */
typedef uint16_t iomode_t;

/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef msp430_ioport_t *ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief   I/O port A identifier.
 * @details This port identifier is mapped on the MSP430 port 1 (P1).
 */
#if defined(__MSP430_HAS_PORT1_R__) ||                                  \
    defined(__MSP430_HAS_PORTA_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT1         ((ioportid_t)(PAIN_+0))
#endif

/**
 * @brief   I/O port A identifier.
 * @details This port identifier is mapped on the MSP430 port 2 (P2).
 */
#if defined(__MSP430_HAS_PORT2_R__) ||                                  \
    defined(__MSP430_HAS_PORTA_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT2         ((ioportid_t)(PAIN_+1))
#endif

/**
 * @brief   I/O port B identifier.
 * @details This port identifier is mapped on the MSP430 port 3 (P3).
 */
#if defined(__MSP430_HAS_PORT3_R__) ||                                  \
    defined(__MSP430_HAS_PORTB_R__) ||                                  \
        defined(__DOXYGEN__)
#define IOPORT3         ((ioportid_t)(PBIN_+0))
#endif

/**
 * @brief   I/O port B identifier.
 * @details This port identifier is mapped on the MSP430 port 4 (P4).
 */
#if defined(__MSP430_HAS_PORT4_R__) ||                                  \
    defined(__MSP430_HAS_PORTB_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT4         ((ioportid_t)(PBIN_+1))
#endif

/**
 * @brief   I/O port C identifier.
 * @details This port identifier is mapped on the MSP430 port 5 (P5).
 */
#if defined(__MSP430_HAS_PORT5_R__) ||                                  \
    defined(__MSP430_HAS_PORTC_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT5         ((ioportid_t)(PCIN_+0))
#endif

/**
 * @brief   I/O port C identifier.
 * @details This port identifier is mapped on the MSP430 port 6 (P6).
 */
#if defined(__MSP430_HAS_PORT6_R__) ||                                  \
    defined(__MSP430_HAS_PORTC_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT6         ((ioportid_t)(PCIN_+1))
#endif

/**
 * @brief   I/O port D identifier.
 * @details This port identifier is mapped on the MSP430 port 7 (P7).
 */
#if defined(__MSP430_HAS_PORT7_R__) ||                                  \
    defined(__MSP430_HAS_PORTD_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT7         ((ioportid_t)(PDIN_+0))
#endif

/**
 * @brief   I/O port D identifier.
 * @details This port identifier is mapped on the MSP430 port 8 (P8).
 */
#if defined(__MSP430_HAS_PORT8_R__) ||                                  \
    defined(__MSP430_HAS_PORTD_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT8         ((ioportid_t)(PDIN_+1))
#endif

/**
 * @brief   I/O port E identifier.
 * @details This port identifier is mapped on the MSP430 port 9 (P9).
 */
#if defined(__MSP430_HAS_PORT9_R__) ||                                  \
    defined(__MSP430_HAS_PORTE_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT9         ((ioportid_t)(PEIN_+0))
#endif

/**
 * @brief   I/O port E identifier.
 * @details This port identifier is mapped on the MSP430 port 10 (P10).
 */
#if defined(__MSP430_HAS_PORT10_R__) ||                                  \
    defined(__MSP430_HAS_PORTE_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT10        ((ioportid_t)(PEIN_+1))
#endif

/**
 * @brief   I/O port F identifier.
 * @details This port identifier is mapped on the MSP430 port 11 (P11).
 */
#if defined(__MSP430_HAS_PORT11_R__) ||                                  \
    defined(__MSP430_HAS_PORTF_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT11        ((ioportid_t)(PFIN_+0))
#endif

/**
 * @brief   I/O port J identifier.
 * @details This port identifier is mapped on the MSP430 port J (P12).
 */
#if defined(__MSP430_HAS_PORTJ_R__) ||                                  \
    defined(__DOXYGEN__)
#define IOPORT12        ((ioportid_t)(PJIN_+0))
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 * @details In MSP430 programs all the ports as input.
 *
 * @param[in] config the MSP430 ports configuration
 *
 * @notapi
 */
#define pal_lld_init(config) _pal_lld_init(config)

/**
 * @brief   Reads the physical I/O port states.
 * @details This function is implemented by reading the PxIN register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((port)->iop_common.in)

/**
 * @brief   Reads the output latch.
 * @details This function is implemented by reading the PxOUT register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
#define pal_lld_readlatch(port) ((port)->iop_common.out)

/**
 * @brief   Writes a bits mask on a I/O port.
 * @details This function is implemented by writing the PxOUT register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) ((port)->iop_common.out = (bits))

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as output as recommended by
 *          the MSP430x1xx Family User's Guide.
 * @note    This function does not alter the @p PxSEL registers. Alternate
 *          functions setup must be handled by device-specific code.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode)                      \
  _pal_lld_setgroupmode(port, mask << offset, mode)

extern const PALConfig pal_default_config;

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(const PALConfig *config);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
#ifdef __cplusplus
}
#endif

#endif /* _PAL_LLD_H_ */

#endif /* HAL_USE_PAL */

/** @} */
