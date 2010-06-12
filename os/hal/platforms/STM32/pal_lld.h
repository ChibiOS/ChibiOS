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
 * @file    STM32/pal_lld.h
 * @brief   STM32 GPIO low level driver header.
 *
 * @addtogroup STM32_PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if CH_HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

/**
 * @brief   STM32 specific alternate push-pull output mode.
 */
#define PAL_MODE_STM32_ALTERNATE_PUSHPULL   16

/**
 * @brief   STM32 specific alternate open-drain output mode.
 */
#define PAL_MODE_STM32_ALTERNATE_OPENDRAIN  17

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief   GPIO port setup info.
 */
typedef struct {
  /** Initial value for ODR register.*/
  uint32_t      odr;
  /** Initial value for CRL register.*/
  uint32_t      crl;
  /** Initial value for CRH register.*/
  uint32_t      crh;
} stm32_gpio_setup_t;

/**
 * @brief   STM32 GPIO static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialize the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
  /** @brief Port A setup data.*/
  stm32_gpio_setup_t    PAData;
  /** @brief Port B setup data.*/
  stm32_gpio_setup_t    PBData;
  /** @brief Port C setup data.*/
  stm32_gpio_setup_t    PCData;
  /** @brief Port D setup data.*/
  stm32_gpio_setup_t    PDData;
#if !defined(STM32F10X_LD) || defined(__DOXYGEN__)
  /** @brief Port E setup data.*/
  stm32_gpio_setup_t    PEData;
#endif
#if defined(STM32F10X_HD) || defined(__DOXYGEN__)
  /** @brief Port F setup data.*/
  stm32_gpio_setup_t    PFData;
  /** @brief Port G setup data.*/
  stm32_gpio_setup_t    PGData;
#endif
} PALConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 16

/**
 * @brief   Whole port mask.
 * @brief   This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFF)

/**
 * @brief Digital I/O port sized unsigned type.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Port Identifier.
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
#if !defined(STM32F10X_LD) || defined(__DOXYGEN__)
#define IOPORT5         GPIOE
#endif

/**
 * @brief   GPIO port F identifier.
 */
#if defined(STM32F10X_HD) || defined(__DOXYGEN__)
#define IOPORT6         GPIOF

/**
 * @brief   GPIO port G identifier.
 */
#define IOPORT7         GPIOG
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, please put them in a file named ioports_lld.c if so.           */
/*===========================================================================*/

/**
 * @brief   GPIO ports subsystem initialization.
 */
#define pal_lld_init(config) _pal_lld_init(config)

/**
 * @brief   Reads an I/O port.
 * @details This function is implemented by reading the GPIO IDR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      the port identifier
 * @return              The port bits.
 */
#define pal_lld_readport(port) ((port)->IDR)

/**
 * @brief   Reads the output latch.
 * @details This function is implemented by reading the GPIO ODR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      the port identifier
 * @return              The latched logical states.
 */
#define pal_lld_readlatch(port) ((port)->ODR)

/**
 * @brief   Writes on a I/O port.
 * @details This function is implemented by writing the GPIO ODR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      the port identifier
 * @param[in] bits      the bits to be written on the specified port
 */
#define pal_lld_writeport(port, bits) ((port)->ODR = (bits))

/**
 * @brief   Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      the port identifier
 * @param[in] bits      the bits to be ORed on the specified port
 */
#define pal_lld_setport(port, bits) ((port)->BSRR = (bits))

/**
 * @brief   Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BRR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      the port identifier
 * @param[in] bits      the bits to be cleared on the specified port
 */
#define pal_lld_clearport(port, bits) ((port)->BRR = (bits))

/**
 * @brief   Writes a group of bits.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] offset    the group bit offset within the port
 * @param[in] bits      the bits to be written. Values exceeding the group
 *                      width are masked.
 */
#define pal_lld_writegroup(port, mask, offset, bits) {                      \
  (port)->BSRR = ((~(bits) & (mask)) << (16 + (offset))) |                  \
                 (((bits) & (mask)) << (offset));                           \
}

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 */
#define pal_lld_setgroupmode(port, mask, mode)                              \
  _pal_lld_setgroupmode(port, mask, mode)

/**
 * @brief   Writes a logical state on an output pad.
 * @note    This function is not meant to be invoked directly by the
 *          application code.
 * @note    Writing on pads programmed as pull-up or pull-down has the side
 *          effect to modify the resistor setting because the output latched
 *          data is used for the resistor selection.
 *
 * @param[in] port      the port identifier
 * @param[in] pad       the pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 */
#define pal_lld_writepad(port, pad, bit) pal_lld_writegroup(port, 1, pad, bit)

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

#endif /* CH_HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */
