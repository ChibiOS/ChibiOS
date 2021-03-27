/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    GPIOv1/hal_pal_lld.h
 * @brief   RP PAL low level driver header.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef HAL_PAL_LLD_H
#define HAL_PAL_LLD_H

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

/* Specifies palInit() without parameter, required until all platforms will
   be updated to the new style.*/
#define PAL_NEW_INIT

#undef PAL_MODE_RESET
#undef PAL_MODE_UNCONNECTED
#undef PAL_MODE_INPUT
#undef PAL_MODE_INPUT_PULLUP
#undef PAL_MODE_INPUT_PULLDOWN
#undef PAL_MODE_INPUT_ANALOG
#undef PAL_MODE_OUTPUT_PUSHPULL
#undef PAL_MODE_OUTPUT_OPENDRAIN

/**
 * @name    RP-specific I/O mode flags
 * @{
 */
#define PAL_RP_MODE_INOVER_NOTINV           (0U << 16)
#define PAL_RP_MODE_INOVER_INV              (1U << 16)
#define PAL_RP_MODE_INOVER_DRVLOW           (2U << 16)
#define PAL_RP_MODE_INOVER_DRVHIGH          (3U << 16)

#define PAL_RP_MODE_OEOVER_DRVPERI          (0U << 12)
#define PAL_RP_MODE_OEOVER_DRVINVPERI       (1U << 12)
#define PAL_RP_MODE_OEOVER_DISABLE          (2U << 12)
#define PAL_RP_MODE_OEOVER_ENABLE           (3U << 12)

#define PAL_RP_MODE_OUTOVER_DRVPERI         (0U << 8)
#define PAL_RP_MODE_OUTOVER_DRVINVPERI      (1U << 8)
#define PAL_RP_MODE_OUTOVER_DRVLOW          (2U << 8)
#define PAL_RP_MODE_OUTOVER_DRVHIGH         (3U << 8)

#define PAL_RP_MODE_FUNCSEL(n)              ((n) << 0)

#define PAL_RP_MODE_OD                      (1U << (24 + 7))

#define PAL_RP_MODE_IE                      (1U << (24 + 6))

#define PAL_RP_MODE_DRIVE2                  (0U << (24 + 4))
#define PAL_RP_MODE_DRIVE4                  (1U << (24 + 4))
#define PAL_RP_MODE_DRIVE8                  (2U << (24 + 4))
#define PAL_RP_MODE_DRIVE12                 (3U << (24 + 4))

#define PAL_RP_MODE_PUE                     (1U << (24 + 3))

#define PAL_RP_MODE_PDE                     (1U << (24 + 2))

#define PAL_RP_MODE_SCHMITT                 (1U << (24 + 1))

#define PAL_RP_MODE_SLEWFAST                (1U << (24 + 0))

/**
 * @brief   Alternate function.
 *
 * @param[in] n         alternate function selector
 */
#define PAL_MODE_ALTERNATE(n)           (PAL_RP_MODE_IE                 |   \
                                         PAL_RP_MODE_OUTOVER_DRVPERI    |   \
                                         PAL_RP_MODE_FUNCSEL(n))
/** @} */

/**
 * @name    Standard I/O mode flags
 * @{
 */
/**
 * @brief   Implemented as input.
 */
#define PAL_MODE_RESET                  PAL_STM32_MODE_INPUT

/**
 * @brief   Implemented as input with pull-up.
 */
#define PAL_MODE_UNCONNECTED            PAL_MODE_INPUT_PULLUP

/**
 * @brief   Regular input high-Z pad.
 */
#define PAL_MODE_INPUT                  PAL_STM32_MODE_INPUT

/**
 * @brief   Input pad with weak pull up resistor.
 */
#define PAL_MODE_INPUT_PULLUP           (PAL_STM32_MODE_INPUT |             \
                                         PAL_STM32_PUPDR_PULLUP)

/**
 * @brief   Input pad with weak pull down resistor.
 */
#define PAL_MODE_INPUT_PULLDOWN         (PAL_STM32_MODE_INPUT |             \
                                         PAL_STM32_PUPDR_PULLDOWN)

/**
 * @brief   Analog input mode.
 */
#define PAL_MODE_INPUT_ANALOG           PAL_STM32_MODE_ANALOG

/**
 * @brief   Push-pull output pad.
 */
#define PAL_MODE_OUTPUT_PUSHPULL        (PAL_STM32_MODE_OUTPUT |            \
                                         PAL_STM32_OTYPE_PUSHPULL)

/**
 * @brief   Open-drain output pad.
 */
#define PAL_MODE_OUTPUT_OPENDRAIN       (PAL_STM32_MODE_OUTPUT |            \
                                         PAL_STM32_OTYPE_OPENDRAIN)
/** @} */

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @name    Port related definitions
 * @{
 */
/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 32

/**
 * @brief   Whole port mask.
 * @details This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFFFFFF)
/** @} */

/**
 * @name    Line handling macros
 * @{
 */
/**
 * @brief   Forms a line identifier.
 * @details A port/pad pair are encoded into an @p ioline_t type. The encoding
 *          of this type is platform-dependent.
 * @note    In this driver the pad number is encoded in the lower 4 bits of
 *          the GPIO address which are guaranteed to be zero.
 */
#define PAL_LINE(port, pad)             ((pad), (port))

/**
 * @brief   Decodes a port identifier from a line identifier.
 */
#define PAL_PORT(line)                  0U

/**
 * @brief   Decodes a pad identifier from a line identifier.
 */
#define PAL_PAD(line)                   (line)

/**
 * @brief   Value identifying an invalid line.
 */
#define PAL_NOLINE                      0U
/** @} */

/**
 * @brief   Type of digital I/O port sized unsigned integer.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Type of digital I/O modes.
 */
typedef uint32_t iomode_t;

/**
 * @brief   Type of an I/O line.
 */
typedef uint32_t ioline_t;

/**
 * @brief   Type of an event mode.
 */
typedef uint32_t ioeventmode_t;

/**
 * @brief   Type of a port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef uint32_t ioportid_t;

/**
 * @brief   Type of an pad identifier.
 */
typedef uint32_t iopadid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the STM32   */
/* firmware library.                                                         */
/*===========================================================================*/

/**
 * @brief   User port identifier.
 */
#define IOPORT1                         0U

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   GPIO ports subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init()                  __pal_lld_init()

/**
 * @brief   Reads the physical I/O port states.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) 0U

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
#define pal_lld_readlatch(port) 0U

/**
 * @brief   Writes a bits mask on a I/O port.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits)                                       \
  do {                                                                      \
    (void)port;                                                             \
    (void)bits;                                                             \
  } while (false)

/**
 * @brief   Sets a bits mask on a I/O port.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @notapi
 */
#define pal_lld_setport(port, bits)                                         \
  do {                                                                      \
    (void)port;                                                             \
    (void)bits;                                                             \
  } while (false)

/**
 * @brief   Clears a bits mask on a I/O port.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @notapi
 */
#define pal_lld_clearport(port, bits)                                       \
  do {                                                                      \
    (void)port;                                                             \
    (void)bits;                                                             \
  } while (false)

/**
 * @brief   Toggles a bits mask on a I/O port.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be XORed on the specified port
 *
 * @notapi
 */
#define pal_lld_toggleport(port, bits)                                      \
  do {                                                                      \
    (void)port;                                                             \
    (void)bits;                                                             \
  } while (false)

/**
 * @brief   Reads a group of bits.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @return              The group logical states.
 *
 * @notapi
 */
#define pal_lld_readgroup(port, mask, offset) 0U

/**
 * @brief   Writes a group of bits.
 * @note    The @ref PAL provides a default software implementation of this
 *          functionality, implement this function if can optimize it by using
 *          special hardware functionalities or special coding.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group width
 *                      are masked.
 *
 * @notapi
 */
#define pal_lld_writegroup(port, mask, offset, bits)                        \
  do {                                                                      \
    (void)port;                                                             \
    (void)mask;                                                             \
    (void)offset;                                                           \
    (void)bits;                                                             \
  } while (false)

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
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

/**
 * @brief   Reads an input pad logic state.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @return              The logic state.
 * @retval PAL_LOW      low logic state.
 * @retval PAL_HIGH     high logic state.
 *
 * @notapi
 */
#define pal_lld_readpad(port, pad)

/**
 * @brief   Writes a logical state on an output pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
#define pal_lld_writepad(port, pad, bit)

/**
 * @brief   Sets a pad logic state to @p PAL_HIGH.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_setpad(port, pad)

/**
 * @brief   Clears a pad logic state to @p PAL_LOW.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_clearpad(port, pad)

/**
 * @brief   Toggles a pad logic state.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_togglepad(port, pad)

/**
 * @brief   Pad mode setup.
 * @details This function programs a pad with the specified mode.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    Programming an unknown or unsupported mode is silently ignored.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad mode
 *
 * @notapi
 */
#define pal_lld_setpadmode(port, pad, mode)                                 \
  __pal_lld_pad_set_mode(port, pad, mode)

#ifdef __cplusplus
extern "C" {
#endif
  void __pal_lld_init(void);
  void __pal_lld_pad_set_mode(ioportid_t port, iopadid_t pad, iomode_t mode);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* HAL_PAL_LLD_H */

/** @} */
