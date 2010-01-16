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
 * @file templates/pal_lld.h
 * @brief PAL subsystem low level driver header template.
 * @addtogroup PAL_LLD
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

/**
 * @brief Generic I/O ports static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialized the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 *
 * @note This structure content is architecture dependent. The nome should be
 *       changed to include the architecture name following this pattern:<br>
 *       - [ARCH][CELL]Config.
 *       .
 *       As example:<br>
 *       - MSP430DIOConfig.
 *       .
 */
typedef struct {

} GenericConfig;

/**
 * @brief Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 32

/**
 * @brief Whole port mask.
 * @brief This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFFFFFF)

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
typedef uint32_t ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/*===========================================================================*/

/**
 * @brief First I/O port identifier.
 * @details Low level drivers can define multiple ports, it is suggested to
 *          use this naming convention.
 */
#define IOPORT1         0

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief Low level PAL subsystem initialization.
 *
 * @param[in] config the architecture-dependent ports configuration
 */
#define pal_lld_init(config)

/**
 * @brief Reads the physical I/O port states.
 *
 * @param[in] port the port identifier
 * @return The port bits.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readport(port)

/**
 * @brief Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 *
 * @param[in] port the port identifier
 * @return The latched logical states.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_readlatch(port)

/**
 * @brief Writes a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be written on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 */
#define pal_lld_writeport(port, bits)

/**
 * @brief Sets a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be ORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_setport(port, bits)

/**
 * @brief Clears a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be cleared on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_clearport(port, bits)

/**
 * @brief Toggles a bits mask on a I/O port.
 *
 * @param[in] port the port identifier
 * @param[in] bits the bits to be XORed on the specified port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_toggleport(port, bits)

/**
 * @brief Reads a group of bits.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] offset the group bit offset within the port
 * @return The group logical states.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_readgroup(port, mask, offset)

/**
 * @brief Writes a group of bits.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] offset the group bit offset within the port
 * @param[in] bits the bits to be written. Values exceeding the group width
 *            are masked.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_writegroup(port, mask, offset, bits)

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
 * @note Programming an unknown or unsupported mode is silently ignored.
 */
#define pal_lld_setgroupmode(port, mask, mode)

/**
 * @brief Reads a logical state from an I/O pad.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 * @return The logical state.
 * @retval 0 low logical state.
 * @retval 1 high logical state.
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_readpad(port, pad)

/**
 * @brief Writes a logical state on an output pad.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 * @param[out] bit the logical value, the value must be @p 0 or @p 1
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_writepad(port, pad, bit)

/**
 * @brief Sets a pad logical state to @p 1.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_setpad(port, pad)

/**
 * @brief Clears a pad logical state to @p 0.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_clearpad(port, pad)

/**
 * @brief Toggles a pad logical state.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 */
#define pal_lld_togglepad(port, pad)

/**
 * @brief Pad mode setup.
 * @details This function programs a pad with the specified mode.
 *
 * @param[in] port the port identifier
 * @param[in] pad the pad number within the port
 * @param[in] mode the mode
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note The @ref PAL provides a default software implementation of this
 *       functionality, implement this function if can optimize it by using
 *       special hardware functionalities or special coding.
 * @note Programming an unknown or unsupported mode is silently ignored.
 */
#define pal_lld_setpadmode(port, pad, mode)

#endif /* CH_HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */
