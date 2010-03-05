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
 * @file    STM8/pal_lld.h
 * @brief   STM8 GPIO low level driver header.
 *
 * @addtogroup STM8_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if CH_HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

typedef struct {
  volatile uint8_t      ODR;
  volatile uint8_t      IDR;
  volatile uint8_t      DDR;
  volatile uint8_t      CR1;
  volatile uint8_t      CR2;
} gpio_t;

/**
 * @brief   Generic I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialized the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
  gpio_t                P[9];
} STM8GPIOConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 32

/**
 * @brief   Whole port mask.
 * @brief   This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFFFFFF)

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Port Identifier.
 */
typedef gpio_t *ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief   GPIO ports as a whole.
 */
#define IOPORTS         ((STM8GPIOConfig *)0x5000)

/**
 * @brief   GPIO port A identifier.
 */
#define IOPORT1         ((gpio_t *)0x5000)

/**
 * @brief   GPIO port B identifier.
 */
#define IOPORT2         ((gpio_t *)0x5005)

/**
 * @brief   GPIO port C identifier.
 */
#define IOPORT3         ((gpio_t *)0x500A)

/**
 * @brief   GPIO port D identifier.
 */
#define IOPORT4         ((gpio_t *)0x500F)

/**
 * @brief   GPIO port E identifier.
 */
#define IOPORT5         ((gpio_t *)0x5014)

/**
 * @brief   GPIO port F identifier.
 */
#define IOPORT6         ((gpio_t *)0x5019)

/**
 * @brief   GPIO port G identifier.
 */
#define IOPORT7         ((gpio_t *)0x501E)

/**
 * @brief   GPIO port H identifier.
 */
#define IOPORT8         ((gpio_t *)0x5023)

/**
 * @brief   GPIO port I identifier.
 */
#define IOPORT9         ((gpio_t *)0x5028)

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 *
 * @param[in] config    architecture-dependent ports configuration
 */
#define pal_lld_init(config)    *IOPORTS = *(config)

/**
 * @brief   Reads the physical I/O port states.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 */
#define pal_lld_readport(port) ((port)->IDR)

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 */
#define pal_lld_readlatch(port) ((port)->ODR)

/**
 * @brief   Writes a bits mask on a I/O port.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 */
#define pal_lld_writeport(port, bits) ((port)->ODR = (bits))


/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] mode      group mode
 */
#define pal_lld_setgroupmode(port, mask, mode) ((void)(mode))

extern const STM8GPIOConfig pal_default_config;

#endif /* CH_HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */
