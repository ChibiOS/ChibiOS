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
 * @file    STM8S/pal_lld.h
 * @brief   STM8S GPIO low level driver header.
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

#undef PAL_MODE_INPUT_PULLDOWN

/**
 * @brief   STM8S specific alternate push-pull slow output mode.
 */
#define PAL_MODE_OUTPUT_PUSHPULL_SLOW   16

/**
 * @brief   STM8S specific alternate open-drain slow output mode.
 */
#define PAL_MODE_OUTPUT_OPENDRAIN_SLOW  17

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief   Generic I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialized the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
#if defined(STM8S105) || defined(__DOXYGEN__)
  GPIO_TypeDef          P[7];
#elif defined(STM8S207) || defined(STM8S208)
  GPIO_TypeDef          P[9];
#else
  GPIO_TypeDef          P[6];
#endif
} PALConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 8

/**
 * @brief   Whole port mask.
 * @brief   This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFF)

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint8_t ioportmask_t;

/**
 * @brief   Digital I/O modes.
 */
typedef uint8_t iomode_t;

/**
 * @brief   Port Identifier.
 */
typedef GPIO_TypeDef *ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief   GPIO ports as a whole.
 */
#define IOPORTS         ((PALConfig *)0x5000)

/**
 * @brief   GPIO port A identifier.
 */
#define IOPORT1         GPIOA

/**
 * @brief   GPIO port B identifier.
 */
#define IOPORT2         GPIOB

/**
 * @brief   GPIO port C identifier.
 */
#define IOPORT3         GPIOC

/**
 * @brief   GPIO port D identifier.
 */
#define IOPORT4         GPIOD

/**
 * @brief   GPIO port E identifier.
 */
#define IOPORT5         GPIOE

/**
 * @brief   GPIO port F identifier.
 */
#define IOPORT6         GPIOF

#if defined(STM8S207) || defined(STM8S208) || defined(STM8S105) ||          \
    defined(__DOXYGEN__)
/**
 * @brief   GPIO port G identifier.
 */
#define IOPORT7         GPIOG
#endif

#if defined(STM8S207) || defined(STM8S208) || defined(__DOXYGEN__)
/**
 * @brief   GPIO port H identifier.
 */
#define IOPORT8         GPIOH

/**
 * @brief   GPIO port I identifier.
 */
#define IOPORT9         GPIOI
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 *
 * @param[in] config    architecture-dependent ports configuration
 *
 * @notapi
 */
#define pal_lld_init(config) (*IOPORTS = *(config))

/**
 * @brief   Reads the physical I/O port states.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((port)->IDR)

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
#define pal_lld_readlatch(port) ((port)->ODR)

/**
 * @brief   Writes a bits mask on a I/O port.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) ((port)->ODR = (bits))

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
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

extern ROMCONST PALConfig pal_default_config;

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */
