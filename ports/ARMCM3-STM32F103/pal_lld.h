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
 * @file ports/ARMCM3-STM32F103/pal_lld.h
 * @brief STM32 GPIO low level driver
 * @addtogroup STM32F103_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

/*
 * Tricks required to make the TRUE/FALSE declaration inside the library
 * compatible.
 */
#ifndef __STM32F10x_MAP_H
#undef FALSE
#undef TRUE
#include "stm32f10x_map.h"
#define FALSE 0
#define TRUE (!FALSE)
#endif

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 16

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
typedef GPIO_TypeDef * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the STM32   */
/* firmware library.                                                         */
/*===========================================================================*/

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOA) || defined(__DOXYGEN__)
#define IOPORT_A        GPIOA
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOB) || defined(__DOXYGEN__)
#define IOPORT_B        GPIOB
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOC) || defined(__DOXYGEN__)
#define IOPORT_C        GPIOC
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOD) || defined(__DOXYGEN__)
#define IOPORT_D        GPIOD
#endif

/**
 * @brief GPIO port A identifier.
 */
#if defined(_GPIOE) || defined(__DOXYGEN__)
#define IOPORT_E        GPIOE
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, please put them in a file named ioports_lld.c if so.           */
/*===========================================================================*/

/**
 * @brief GPIO ports subsystem initialization.
 * @details Ports A-D enabled, AFIO enabled.
 */
#define pal_lld_init() {                                                \
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN |             \
                  RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN |             \
                  RCC_APB2ENR_IOPDEN;                                   \
}

/**
 * @brief Reads an I/O port.
 * @details This function is implemented by reading the GPIO IDR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @return the port bits
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readport(port) ((port)->IDR)

/**
 * @brief Reads the output latch.
 * @details This function is implemented by reading the GPIO oDR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @return The latched logical states.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readlatch(port) ((port)->ODR)

/**
 * @brief Writes on a I/O port.
 * @details This function is implemented by writing the GPIO ODR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writeport(port, bits) ((port)->ODR = (bits))

/**
 * @brief Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_setport(port, bits) ((port)->BSRR = (bits))

/**
 * @brief Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_clearport(port, bits) ((port)->BRR = (bits))

/**
 * @brief Writes a group of bits.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] offset the group bit offset within the port
 * @param[in] bits the bits to be written. Values exceeding the group width
 *            are masked.
 * @return The group logical states.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writegroup(port, mask, offset, bits) {                  \
  (port)->BSRR = ((~(bits) & (mask)) << (16 + (offset))) |              \
                 (((bits) & (mask)) << (offset));                       \
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

/**
 * @brief GPIO port setup.
 * @details This function initializes a GPIO port, note that this functionality
 *          is STM32 specific and non portable. It does not have a
 *          corresponding PAL API.
 */
#define pal_lld_stm32_setup(port, crh, crl) {                           \
  (port)->CRH = (crh);                                                  \
  (port)->CRL = (crl);                                                  \
}

#endif /* _PAL_LLD_H_ */

/** @} */
