/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
#define PAL_RP_IOCTRL_INOVER_NOTINV         (0U << 16)
#define PAL_RP_IOCTRL_INOVER_INV            (1U << 16)
#define PAL_RP_IOCTRL_INOVER_DRVLOW         (2U << 16)
#define PAL_RP_IOCTRL_INOVER_DRVHIGH        (3U << 16)

#define PAL_RP_IOCTRL_OEOVER_DRVPERI        (0U << RP_GPIO_IOCTRL_OEOVER_Pos)
#define PAL_RP_IOCTRL_OEOVER_DRVINVPERI     (1U << RP_GPIO_IOCTRL_OEOVER_Pos)
#define PAL_RP_IOCTRL_OEOVER_DISABLE        (2U << RP_GPIO_IOCTRL_OEOVER_Pos)
#define PAL_RP_IOCTRL_OEOVER_ENABLE         (3U << RP_GPIO_IOCTRL_OEOVER_Pos)

#define PAL_RP_IOCTRL_OUTOVER_DRVPERI       (0U << RP_GPIO_IOCTRL_OUTOVER_Pos)
#define PAL_RP_IOCTRL_OUTOVER_DRVINVPERI    (1U << RP_GPIO_IOCTRL_OUTOVER_Pos)
#define PAL_RP_IOCTRL_OUTOVER_DRVLOW        (2U << RP_GPIO_IOCTRL_OUTOVER_Pos)
#define PAL_RP_IOCTRL_OUTOVER_DRVHIGH       (3U << RP_GPIO_IOCTRL_OUTOVER_Pos)

#define PAL_RP_IOCTRL_FUNCSEL(n)            ((n) << 0)
#define PAL_RP_IOCTRL_FUNCSEL_SPI           PAL_RP_IOCTRL_FUNCSEL(1U)
#define PAL_RP_IOCTRL_FUNCSEL_UART          PAL_RP_IOCTRL_FUNCSEL(2U)
#define PAL_RP_IOCTRL_FUNCSEL_I2C           PAL_RP_IOCTRL_FUNCSEL(3U)
#define PAL_RP_IOCTRL_FUNCSEL_PWM           PAL_RP_IOCTRL_FUNCSEL(4U)
#define PAL_RP_IOCTRL_FUNCSEL_SIO           PAL_RP_IOCTRL_FUNCSEL(5U)
#define PAL_RP_IOCTRL_FUNCSEL_PIO0          PAL_RP_IOCTRL_FUNCSEL(6U)
#define PAL_RP_IOCTRL_FUNCSEL_PIO1          PAL_RP_IOCTRL_FUNCSEL(7U)
#define PAL_RP_IOCTRL_FUNCSEL_NULL          PAL_RP_IOCTRL_FUNCSEL(31U)

#define PAL_RP_GPIO_OE                      (1U << (23 + 0))

#define PAL_RP_PAD_OD                       (1U << (24 + 7))

#define PAL_RP_PAD_IE                       (1U << (24 + 6))

#define PAL_RP_PAD_DRIVE2                   (0U << (24 + 4))
#define PAL_RP_PAD_DRIVE4                   (1U << (24 + 4))
#define PAL_RP_PAD_DRIVE8                   (2U << (24 + 4))
#define PAL_RP_PAD_DRIVE12                  (3U << (24 + 4))

#define PAL_RP_PAD_PUE                      (1U << (24 + 3))

#define PAL_RP_PAD_PDE                      (1U << (24 + 2))

#define PAL_RP_PAD_SCHMITT                  (1U << (24 + 1))

#define PAL_RP_PAD_SLEWFAST                 (1U << (24 + 0))
/** @} */

#include "rp_pal_lld.h"

/**
 * @name    RP-specific I/O event flags
 * @{
 */
#define RP_PAL_EVENT_MODE_LEVEL_LOW         4U
#define RP_PAL_EVENT_MODE_LEVEL_HIGH        8U
/** @} */

#if !defined(RP_PAL_EVENT_CORE_AFFINITY)
  /** @brief Core that handles all the PAL event interrupts. */
  #define RP_PAL_EVENT_CORE_AFFINITY        0
#endif

/**
 * @name    Alternate functions
 * @{
 * @param[in] n         alternate function selector
 */
#define PAL_MODE_ALTERNATE(n)               (PAL_RP_IOCTRL_FUNCSEL(n)   |   \
                                             PAL_RP_PAD_IE              |   \
                                             PAL_RP_PAD_SCHMITT)
#define PAL_MODE_ALTERNATE_SPI              (PAL_MODE_ALTERNATE(1U))
#define PAL_MODE_ALTERNATE_UART             (PAL_MODE_ALTERNATE(2U))
#define PAL_MODE_ALTERNATE_I2C              (PAL_MODE_ALTERNATE(3U))
#define PAL_MODE_ALTERNATE_PWM              (PAL_MODE_ALTERNATE(4U))
#define PAL_MODE_ALTERNATE_SIO              (PAL_MODE_ALTERNATE(5U))
#define PAL_MODE_ALTERNATE_PIO0             (PAL_MODE_ALTERNATE(6U))
#define PAL_MODE_ALTERNATE_PIO1             (PAL_MODE_ALTERNATE(7U))
/** @} */

/**
 * @name    Standard I/O mode flags
 * @{
 */
/**
 * @brief   Implemented as post-reset state.
 */
#define PAL_MODE_RESET                  (PAL_RP_IOCTRL_FUNCSEL_NULL     |   \
                                         PAL_RP_PAD_DRIVE4              |   \
                                         PAL_RP_PAD_IE                  |   \
                                         PAL_RP_PAD_SCHMITT)

/**
 * @brief   Implemented as input with pull-up.
 */
#define PAL_MODE_UNCONNECTED            (PAL_RP_IOCTRL_FUNCSEL_SIO      |   \
                                         PAL_RP_PAD_IE                  |   \
                                         PAL_RP_PAD_SCHMITT             |   \
                                         PAL_RP_PAD_PUE)

/**
 * @brief   Regular input high-Z pad.
 */
#define PAL_MODE_INPUT                  (PAL_RP_IOCTRL_FUNCSEL_SIO      |   \
                                         PAL_RP_PAD_IE                  |   \
                                         PAL_RP_PAD_SCHMITT)

/**
 * @brief   Input pad with weak pull up resistor.
 */
#define PAL_MODE_INPUT_PULLUP           (PAL_RP_IOCTRL_FUNCSEL_SIO      |   \
                                         PAL_RP_PAD_IE                  |   \
                                         PAL_RP_PAD_SCHMITT             |   \
                                         PAL_RP_PAD_PUE)

/**
 * @brief   Input pad with weak pull down resistor.
 * @note    On RP2350 revisions affected by erratum RP2350-E9, the internal
 *          pull-down may not behave as expected with the input buffer
 *          enabled. This driver does not apply a workaround automatically.
 */
#define PAL_MODE_INPUT_PULLDOWN         (PAL_RP_IOCTRL_FUNCSEL_SIO      |   \
                                         PAL_RP_PAD_IE                  |   \
                                         PAL_RP_PAD_SCHMITT             |   \
                                         PAL_RP_PAD_PDE)

/**
 * @brief   Analog input mode.
 */
#define PAL_MODE_INPUT_ANALOG           (PAL_RP_IOCTRL_FUNCSEL_NULL)

/**
 * @brief   Push-pull output pad.
 */
#define PAL_MODE_OUTPUT_PUSHPULL        (PAL_RP_IOCTRL_FUNCSEL_SIO      |   \
                                         PAL_RP_GPIO_OE                 |   \
                                         PAL_RP_PAD_IE)
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

#define RP_PAL_IOPORTS_COUNT            ((RP_GPIO_NUM_LINES + 31U) / 32U)

#if RP_GPIO_NUM_LINES >= 32U
  #define RP_PAL_PORT0_VALID_MASK       0xFFFFFFFFU
#else
  #define RP_PAL_PORT0_VALID_MASK       ((1U << RP_GPIO_NUM_LINES) - 1U)
#endif

#if RP_GPIO_NUM_LINES > 32U
  #define RP_PAL_PORT1_VALID_MASK       ((1U << (RP_GPIO_NUM_LINES - 32U)) - 1U)
  #define RP_PAL_VALID_MASK(port)                                               \
    ((uint32_t)(port) == 0U ? RP_PAL_PORT0_VALID_MASK : RP_PAL_PORT1_VALID_MASK)
#else
  #define RP_PAL_VALID_MASK(port)       RP_PAL_PORT0_VALID_MASK
#endif

/**
 * @name    Line handling macros
 * @{
 */
/**
 * @brief   Forms a line identifier.
 * @details A port/pad pair are encoded into an @p ioline_t type. The encoding
 *          of this type is platform-dependent.
 * @note    In this driver a line is the absolute GPIO number obtained by
 *          adding the port base to the port-relative pad number.
 */
#define PAL_LINE(port, pad)                                                 \
  ((ioline_t)(((uint32_t)(port) << 5U) | (uint32_t)(pad)))

/**
 * @brief   Decodes a port identifier from a line identifier.
 */
#define PAL_PORT(line)                                                      \
  ((ioportid_t)((uint32_t)(line) >> 5U))

/**
 * @brief   Decodes a pad identifier from a line identifier.
 */
#define PAL_PAD(line)                                                       \
  ((iopadid_t)((uint32_t)(line) & 31U))

/**
 * @brief   Value identifying an invalid line.
 */
#define PAL_NOLINE                      0xFFFFFFFFU
/** @} */

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver exposes RP GPIO banks as PAL ports.                  */
/*===========================================================================*/

/**
 * @brief   RP GPIO low port identifier.
 */
#define IOPORT1                         0U

#if RP_GPIO_NUM_LINES > 32U || defined(__DOXYGEN__)
/**
 * @brief   RP GPIO high port identifier.
 */
#define IOPORT2                         1U
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   GPIO ports subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init()                  _pal_lld_init()

/**
 * @brief   Reads the physical I/O port states.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port)                                              \
  ((ioportmask_t)RP_PAL_SIO_REG(GPIO_IN, (port)))

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
#define pal_lld_readlatch(port)                                             \
  ((ioportmask_t)RP_PAL_SIO_REG(GPIO_OUT, (port)))

/**
 * @brief   Writes a bits mask on a I/O port.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
/* @note    On RP2350, IOPORT2 maps to SIO GPIO_HI_OUT which is shared with
 *          QSPI/USB outputs in bits 31:16. This is a raw full-register write;
 *          use palSetPort()/palClearPort()/palTogglePort() when non-PAL high
 *          bits must be preserved.
 */
#define pal_lld_writeport(port, bits)                                       \
  do {                                                                      \
    RP_PAL_SIO_REG(GPIO_OUT, (port)) = (uint32_t)(bits);                    \
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
    osalDbgAssert(((uint32_t)(bits) & ~(uint32_t)RP_PAL_VALID_MASK(port))   \
                  == 0U, "invalid port bits");                              \
    RP_PAL_SIO_REG(GPIO_OUT_SET, (port)) = (uint32_t)(bits);               \
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
    osalDbgAssert(((uint32_t)(bits) & ~(uint32_t)RP_PAL_VALID_MASK(port))   \
                  == 0U, "invalid port bits");                              \
    RP_PAL_SIO_REG(GPIO_OUT_CLR, (port)) = (uint32_t)(bits);               \
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
    osalDbgAssert(((uint32_t)(bits) & ~(uint32_t)RP_PAL_VALID_MASK(port))   \
                  == 0U, "invalid port bits");                              \
    RP_PAL_SIO_REG(GPIO_OUT_XOR, (port)) = (uint32_t)(bits);               \
  } while (false)

/**
 * @brief   Pads group mode setup.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode)                     \
  _pal_lld_setgroupmode(port, (mask) << (offset), mode)

#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE)

#define pal_lld_enablepadevent(port, pad, mode)                             \
  _pal_lld_enablelineevent(PAL_LINE(port, pad), mode)

#define pal_lld_disablepadevent(port, pad)                                  \
  _pal_lld_disablelineevent(PAL_LINE(port, pad))

#define pal_lld_ispadeventenabled(port, pad)                                \
  _pal_lld_ispadeventenabled(port, pad)

#if !defined(RP_IO_IRQ_BANK0_PRIORITY)
  /** @brief IRQ priority of the external pad events. */
  #define RP_IO_IRQ_BANK0_PRIORITY          2
#endif

#if !defined(__DOXYGEN__)
extern palevent_t _pal_events[RP_GPIO_NUM_LINES];
#endif

/**
 * @brief   Returns a PAL event structure associated to a line.
 *
 * @param[in] line      line identifier
 *
 * @notapi
 */
#define pal_lld_get_line_event(line)                                        \
  &_pal_events[line]

/**
 * @brief   Returns a PAL event structure associated to a pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_get_pad_event(port, pad)                                    \
  &_pal_events[PAL_LINE(port, pad)]

/**
 * @brief Force a trigger event on the line.
 * 
 * @param[in]         line identifier
 * 
 * @notapi
 */
#define pal_lld_forcelineevent(line)                                        \
  _pal_lld_forcelineevent(line)

/**
 * @brief Clear any previously forced trigger on the line.
 * 
 * @param[in]         line identifier
 * 
 * @notapi
 */
#define pal_lld_unforcelineevent(line)                                      \
  _pal_lld_unforcelineevent(line)

#endif /* (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE) */

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
  void _pal_lld_init(void);
#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE)
  bool _pal_lld_ispadeventenabled(ioportid_t port, iopadid_t pad);
  void _pal_lld_enablelineevent(ioline_t line, ioeventmode_t mode);
  void _pal_lld_disablelineevent(ioline_t line);
  void _pal_lld_forcelineevent(ioline_t line);
  void _pal_lld_unforcelineevent(ioline_t line);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* HAL_PAL_LLD_H */

/** @} */
