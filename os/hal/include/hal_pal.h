/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    hal_pal.h
 * @brief   I/O Ports Abstraction Layer macros, types and structures.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef HAL_PAL_H
#define HAL_PAL_H

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Pads mode constants
 * @{
 */
/**
 * @brief   After reset state.
 * @details The state itself is not specified and is architecture dependent,
 *          it is guaranteed to be equal to the after-reset state. It is
 *          usually an input state.
 */
#define PAL_MODE_RESET                  0U

/**
 * @brief   Safe state for <b>unconnected</b> pads.
 * @details The state itself is not specified and is architecture dependent,
 *          it may be mapped on @p PAL_MODE_INPUT_PULLUP,
 *          @p PAL_MODE_INPUT_PULLDOWN or @p PAL_MODE_OUTPUT_PUSHPULL for
 *          example.
 */
#define PAL_MODE_UNCONNECTED            1U

/**
 * @brief   Regular input high-Z pad.
 */
#define PAL_MODE_INPUT                  2U

/**
 * @brief   Input pad with weak pull up resistor.
 */
#define PAL_MODE_INPUT_PULLUP           3U

/**
 * @brief   Input pad with weak pull down resistor.
 */
#define PAL_MODE_INPUT_PULLDOWN         4U

/**
 * @brief   Analog input mode.
 */
#define PAL_MODE_INPUT_ANALOG           5U

/**
 * @brief   Push-pull output pad.
 */
#define PAL_MODE_OUTPUT_PUSHPULL        6U

/**
 * @brief   Open-drain output pad.
 */
#define PAL_MODE_OUTPUT_OPENDRAIN       7U
/** @} */

/**
 * @name    Logic level constants
 * @{
 */
/**
 * @brief   Logical low state.
 */
#define PAL_LOW                         0U

/**
 * @brief   Logical high state.
 */
#define PAL_HIGH                        1U
/** @} */

/**
 * @name    PAL event modes
 * @{
 */
#define PAL_EVENT_MODE_EDGES_MASK   3U  /**< @brief Mask of edges field.    */
#define PAL_EVENT_MODE_DISABLED     0U  /**< @brief Channel disabled.       */
#define PAL_EVENT_MODE_RISING_EDGE  1U  /**< @brief Rising edge callback.   */
#define PAL_EVENT_MODE_FALLING_EDGE 2U  /**< @brief Falling edge callback.  */
#define PAL_EVENT_MODE_BOTH_EDGES   3U  /**< @brief Both edges callback.    */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    PAL configuration options
 * @{
 */
/**
 * @brief   Enables synchronous APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(PAL_USE_WAIT) || defined(__DOXYGEN__)
#define PAL_USE_WAIT                TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a PAL event callback.
 */
typedef void (*palcallback_t)(void *arg);

/**
 * @brief   Type of a PAL event record.
 */
typedef struct {
#if PAL_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief   Threads queued for an event.
   */
  threads_queue_t       threads;
#endif
  /**
   * @brief   Event callback.
   */
  palcallback_t         cb;
  /**
   * @brief   Event callback argument.
   */
  void                  *arg;
} palevent_t;

#include "hal_pal_lld.h"

/**
 * @brief   I/O bus descriptor.
 * @details This structure describes a group of contiguous digital I/O lines
 *          that have to be handled as bus.
 * @note    I/O operations on a bus do not affect I/O lines on the same port but
 *          not belonging to the bus.
 */
typedef struct {
  /**
   * @brief Port identifier.
   */
  ioportid_t            portid;
  /**
   * @brief Bus mask aligned to port bit 0.
   * @note  The bus mask implicitly define the bus width. A logic AND is
   *        performed on the bus data.
   */
  ioportmask_t          mask;
  /**
   * @brief Offset, within the port, of the least significant bit of the bus.
   */
  uint_fast8_t          offset;
} IOBus;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Port bit helper macro.
 * @details This macro calculates the mask of a bit within a port.
 *
 * @param[in] n         bit position within the port
 * @return              The bit mask.
 */
#if !defined(PAL_PORT_BIT) || defined(__DOXYGEN__)
#define PAL_PORT_BIT(n) ((ioportmask_t)(1U << (n)))
#endif

/**
 * @brief   Bits group mask helper.
 * @details This macro calculates the mask of a bits group.
 *
 * @param[in] width         group width
 * @return                  The group mask.
 */
#if !defined(PAL_GROUP_MASK) || defined(__DOXYGEN__)
#define PAL_GROUP_MASK(width) ((ioportmask_t)(1U << (width)) - 1U)
#endif

/**
 * @brief   Data part of a static I/O bus initializer.
 * @details This macro should be used when statically initializing an I/O bus
 *          that is part of a bigger structure.
 *
 * @param[in] name      name of the IOBus variable
 * @param[in] port      I/O port descriptor
 * @param[in] width     bus width in bits
 * @param[in] offset     bus bit offset within the port
 */
#define _IOBUS_DATA(name, port, width, offset)                              \
  {port, PAL_GROUP_MASK(width), offset}

/**
 * @brief   Static I/O bus initializer.
 *
 * @param[in] name      name of the IOBus variable
 * @param[in] port      I/O port descriptor
 * @param[in] width     bus width in bits
 * @param[in] offset    bus bit offset within the port
 */
#define IOBUS_DECL(name, port, width, offset)                               \
  IOBus name = _IOBUS_DATA(name, port, width, offset)

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief   Common ISR code.
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] e         event index
 *
 * @notapi
 */
#if PAL_USE_WAIT || defined(__DOXYGEN__)
#define _pal_isr_code(e) do {                                               \
  if (_pal_events[e].cb != NULL) {                                          \
    _pal_events[e].cb(_pal_events[e].arg);                                  \
  }                                                                         \
  osalSysLockFromISR();                                                     \
  osalThreadDequeueAllI(&_pal_events[e].threads, MSG_OK);                   \
  osalSysUnlockFromISR();                                                   \
} while (false)
#else
#define _pal_isr_code(e) do {                                               \
  if (_pal_events[e].cb != NULL) {                                          \
    _pal_events[e].cb(_pal_events[e].arg);                                  \
  }                                                                         \
} while (false)
#endif

/**
 * @brief   PAL event setup.
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] e         event index
 * @param[in] c         callback pointer
 * @param[in] a         callback argument
 *
 * @notapi
 */
#if PAL_USE_WAIT || defined(__DOXYGEN__)
#define _pal_set_event(e, c, a) {                                           \
  osalThreadQueueObjectInit(&_pal_events[e].threads);                       \
  _pal_events[e].cb = c;                                                    \
  _pal_events[e].arg = a;                                                   \
}
#else
#define _pal_set_event(e, c, a) {                                           \
  _pal_events[e].cb = c;                                                    \
  _pal_events[e].arg = a;                                                   \
}
#endif

/**
 * @brief   PAL event clear.
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] e         event index
 *
 * @notapi
 */
#if PAL_USE_WAIT || defined(__DOXYGEN__)
#define _pal_clear_event(e) {                                               \
  osalThreadDequeueAllI(&_pal_events[e].threads, MSG_RESET);                \
  _pal_events[e].cb = NULL;                                                 \
  _pal_events[e].arg = NULL;                                                \
}
#else
#define _pal_clear_event(e) {                                               \
  _pal_events[e].cb = NULL;                                                 \
  _pal_events[e].arg = NULL;                                                \
}
#endif
/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   PAL subsystem initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @param[in] config pointer to an architecture specific configuration
 *            structure. This structure is defined in the low level driver
 *            header.
 *
 * @init
 */
#define palInit(config) pal_lld_init(config)

/**
 * @brief   Reads the physical I/O port states.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @return              The port logic states.
 *
 * @special
 */
#if !defined(pal_lld_readport) || defined(__DOXYGEN__)
#define palReadPort(port) ((void)(port), 0U)
#else
#define palReadPort(port) pal_lld_readport(port)
#endif

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @return              The latched logic states.
 *
 * @special
 */
#if !defined(pal_lld_readlatch) || defined(__DOXYGEN__)
#define palReadLatch(port) ((void)(port), 0U)
#else
#define palReadLatch(port) pal_lld_readlatch(port)
#endif

/**
 * @brief   Writes a bits mask on a I/O port.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @special
 */
#if !defined(pal_lld_writeport) || defined(__DOXYGEN__)
#define palWritePort(port, bits) ((void)(port), (void)(bits))
#else
#define palWritePort(port, bits) pal_lld_writeport(port, bits)
#endif

/**
 * @brief   Sets a bits mask on a I/O port.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @special
 */
#if !defined(pal_lld_setport) || defined(__DOXYGEN__)
#define palSetPort(port, bits)                                              \
  palWritePort(port, palReadLatch(port) | (bits))
#else
#define palSetPort(port, bits) pal_lld_setport(port, bits)
#endif

/**
 * @brief   Clears a bits mask on a I/O port.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures,  for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @special
 */
#if !defined(pal_lld_clearport) || defined(__DOXYGEN__)
#define palClearPort(port, bits)                                            \
  palWritePort(port, palReadLatch(port) & ~(bits))
#else
#define palClearPort(port, bits) pal_lld_clearport(port, bits)
#endif

/**
 * @brief   Toggles a bits mask on a I/O port.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be XORed on the specified port
 *
 * @special
 */
#if !defined(pal_lld_toggleport) || defined(__DOXYGEN__)
#define palTogglePort(port, bits)                                           \
  palWritePort(port, palReadLatch(port) ^ (bits))
#else
#define palTogglePort(port, bits) pal_lld_toggleport(port, bits)
#endif

/**
 * @brief   Reads a group of bits.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask, a logic AND is performed on the input
 *                      data
 * @param[in] offset    group bit offset within the port
 * @return              The group logic states.
 *
 * @special
 */
#if !defined(pal_lld_readgroup) || defined(__DOXYGEN__)
#define palReadGroup(port, mask, offset)                                    \
  ((palReadPort(port) >> (offset)) & (mask))
#else
#define palReadGroup(port, mask, offset) pal_lld_readgroup(port, mask, offset)
#endif

/**
 * @brief   Writes a group of bits.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask, a logic AND is performed on the
 *                      output  data
 * @param[in] offset    group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group
 *                      width are masked.
 *
 * @special
 */
#if !defined(pal_lld_writegroup) || defined(__DOXYGEN__)
#define palWriteGroup(port, mask, offset, bits)                             \
  palWritePort(port, (palReadLatch(port) & ~((mask) << (offset))) |         \
                     (((bits) & (mask)) << (offset)))
#else
#define palWriteGroup(port, mask, offset, bits)                             \
  pal_lld_writegroup(port, mask, offset, bits)
#endif

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    Programming an unknown or unsupported mode is silently ignored.
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @special
 */
#if !defined(pal_lld_setgroupmode) || defined(__DOXYGEN__)
#define palSetGroupMode(port, mask, offset, mode)
#else
#define palSetGroupMode(port, mask, offset, mode)                           \
  pal_lld_setgroupmode(port, mask, offset, mode)
#endif

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
 * @special
 */
#if !defined(pal_lld_readpad) || defined(__DOXYGEN__)
#define palReadPad(port, pad) ((palReadPort(port) >> (pad)) & 1U)
#else
#define palReadPad(port, pad) pal_lld_readpad(port, pad)
#endif

/**
 * @brief   Writes a logic state on an output pad.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logic value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @special
 */
#if !defined(pal_lld_writepad) || defined(__DOXYGEN__)
#define palWritePad(port, pad, bit)                                         \
  palWritePort(port, (palReadLatch(port) & ~PAL_PORT_BIT(pad)) |            \
                     (((bit) & 1U) << pad))
#else
#define palWritePad(port, pad, bit) pal_lld_writepad(port, pad, bit)
#endif

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
 * @special
 */
#if !defined(pal_lld_setpad) || defined(__DOXYGEN__)
#define palSetPad(port, pad) palSetPort(port, PAL_PORT_BIT(pad))
#else
#define palSetPad(port, pad) pal_lld_setpad(port, pad)
#endif

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
 * @special
 */
#if !defined(pal_lld_clearpad) || defined(__DOXYGEN__)
#define palClearPad(port, pad) palClearPort(port, PAL_PORT_BIT(pad))
#else
#define palClearPad(port, pad) pal_lld_clearpad(port, pad)
#endif

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
 * @special
 */
#if !defined(pal_lld_togglepad) || defined(__DOXYGEN__)
#define palTogglePad(port, pad) palTogglePort(port, PAL_PORT_BIT(pad))
#else
#define palTogglePad(port, pad) pal_lld_togglepad(port, pad)
#endif

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
 * @special
 */
#if !defined(pal_lld_setpadmode) || defined(__DOXYGEN__)
#define palSetPadMode(port, pad, mode)                                      \
  palSetGroupMode(port, PAL_PORT_BIT(pad), 0U, mode)
#else
#define palSetPadMode(port, pad, mode) pal_lld_setpadmode(port, pad, mode)
#endif

/**
 * @brief   Pad event enable.
 * @details This function programs an event callback in the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 * 
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad event mode
 * @param[in] callback  event callback function
 * @param[in] arg       callback argument
 * 
 * @iclass
 */
#if !defined(pal_lld_enablepadevent) || defined(__DOXYGEN__)
#define palEnablePadEventI(port, pad, mode, callback, arg)
#else
#define palEnablePadEventI(port, pad, mode, callback, arg)                  \
  pal_lld_enablepadevent(port, pad, mode, callback, arg)
#endif

/**
 * @brief   Pad event disable.
 * @details This function disables previously programmed event callbacks.
 * 
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * 
 * @iclass
 */
#if !defined(pal_lld_disablepadevent) || defined(__DOXYGEN__)
#define palDisablePadEventI(port, pad)
#else
#define PadDisablepalEventI(port, pad)                                      \
  pal_lld_disablepadevent(port, pad)
#endif

/**
 * @brief   Pad event enable.
 * @details This function programs an event callback in the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad event mode
 * @param[in] callback  event callback function
 * @param[in] arg       callback argument
 *
 * @api
 */
#define palEnablePadEvent(port, pad, mode, callback, arg)                   \
  do {                                                                      \
    osalSysLock();                                                          \
    palEnablePadEventI(port, pad, mode, callback, arg);                     \
    osalSysUnlock();                                                        \
  } while (false)

/**
 * @brief   Pad event disable.
 * @details This function disables previously programmed event callbacks.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @api
 */
#define palDisablePadEvent(port, pad)                                       \
  do {                                                                      \
    osalSysLock();                                                          \
    palisablePadDEventI(port, pad);                                         \
    osalSysUnlock();                                                        \
  } while (false)

/**
 * @brief   Reads an input line logic state.
 * @note    The function can be called from any context.
 *
 * @param[in] line      line identifier
 * @return              The logic state.
 * @retval PAL_LOW      low logic state.
 * @retval PAL_HIGH     high logic state.
 *
 * @special
 */
#if !defined(pal_lld_readline) || defined(__DOXYGEN__)
#define palReadLine(line) palReadPad(PAL_PORT(line), PAL_PAD(line))
#else
#define palReadLine(line) pal_lld_readline(line)
#endif

/**
 * @brief   Writes a logic state on an output line.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] line      line identifier
 * @param[in] bit       logic value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @special
 */
#if !defined(pal_lld_writeline) || defined(__DOXYGEN__)
#define palWriteLine(line, bit) palWritePad(PAL_PORT(line), PAL_PAD(line), bit)
#else
#define palWriteLine(line, bit) pal_lld_writeline(line, bit)
#endif

/**
 * @brief   Sets a line logic state to @p PAL_HIGH.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] line      line identifier
 *
 * @special
 */
#if !defined(pal_lld_setline) || defined(__DOXYGEN__)
#define palSetLine(line) palSetPad(PAL_PORT(line), PAL_PAD(line))
#else
#define palSetLine(line) pal_lld_setline(line)
#endif

/**
 * @brief   Clears a line logic state to @p PAL_LOW.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] line      line identifier
 *
 * @special
 */
#if !defined(pal_lld_clearline) || defined(__DOXYGEN__)
#define palClearLine(line) palClearPad(PAL_PORT(line), PAL_PAD(line))
#else
#define palClearLine(line) pal_lld_clearline(line)
#endif

/**
 * @brief   Toggles a line logic state.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] line      line identifier
 *
 * @special
 */
#if !defined(pal_lld_toggleline) || defined(__DOXYGEN__)
#define palToggleLine(line) palTogglePad(PAL_PORT(line), PAL_PAD(line))
#else
#define palToggleLine(line) pal_lld_toggleline(line)
#endif

/**
 * @brief   Line mode setup.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function can be called from any context.
 *
 * @param[in] line      line identifier
 * @param[in] mode      pad mode
 *
 * @special
 */
#if !defined(pal_lld_setlinemode) || defined(__DOXYGEN__)
#define palSetLineMode(line, mode)                                          \
  palSetPadMode(PAL_PORT(line), PAL_PAD(line), mode)
#else
#define palSetLineMode(line, mode) pal_lld_setlinemode(line, mode)
#endif

/**
 * @brief   Line event enable.
 *
 * @param[in] line      line identifier
 * @param[in] mode      line event mode
 * @param[in] callback  event callback function
 * @param[in] arg       callback argument
 *
 * @iclass
 */
#if !defined(pal_lld_enablelineevent) || defined(__DOXYGEN__)
#define palEnableLineEventI(line, mode, callback, arg)                      \
  palEnablePadEventI(PAL_PORT(line), PAL_PAD(line), mode, callback, arg)
#else
#define palEnableLineEventI(line, mode, callback, arg)                      \
    pal_lld_enablelineevent(line, mode, callback, arg)
#endif

/**
 * @brief   Line event disable.
 *
 * @param[in] line      line identifier
 *
 * @iclass
 */
#if !defined(pal_lld_disablelineevent) || defined(__DOXYGEN__)
#define palDisableLineEventI(line)                                          \
  palDisablePadEventI(PAL_PORT(line), PAL_PAD(line))
#else
#define palDisableLineEventI(line) pal_lld_disablelineevent(line)
#endif

/**
 * @brief   Line event enable.
 *
 * @param[in] line      line identifier
 * @param[in] mode      line event mode
 * @param[in] callback  event callback function
 * @param[in] arg       callback argument
 *
 * @api
 */
#define palEnableLineEvent(line, mode, callback, arg)                       \
  do {                                                                      \
    osalSysLock();                                                          \
    palEnableLineEventI(line, mode, callback, arg);                         \
    osalSysUnlock();                                                        \
  } while (false)

/**
 * @brief   Line event disable.
 *
 * @param[in] line      line identifier
 *
 * @api
 */
#define paDisableLineEvent(line)                                            \
  do {                                                                      \
    osalSysLock();                                                          \
    palDisableLineEventI(line);                                             \
    osalSysUnlock();                                                        \
  } while (false)

#if PAL_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Waits for an edge on the specified port/pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @returns             The operation state.
 * @retval MSG_OK       if an edge has been detected.
 * @retval MSG_TIMEOUT  if a timeout occurred before an edge cound be detected.
 * @retval MSG_RESET    if the event has been disabled while the thread was
 *                      waiting for an edge.
 *
 * @api
 */
#define palWaitPadTimeout(port, pad, timeout)                               \
  do {                                                                      \
    osalSysLock();                                                          \
    palWaitPadTimeoutS(port, pad, timeout);                                 \
    osalSysUnlock();                                                        \
  } while (false)


/**
 * @brief   Waits for an edge on the specified line.
 *
 * @param[in] line      line identifier
 * @param[in] timeout   operation timeout
 * @returns             The operation state.
 * @retval MSG_OK       if an edge has been detected.
 * @retval MSG_TIMEOUT  if a timeout occurred before an edge cound be detected.
 * @retval MSG_RESET    if the event has been disabled while the thread was
 *                      waiting for an edge.
 *
 * @api
 */
#define palWaitLineTimeout(line, timeout)                                   \
  do {                                                                      \
    osalSysLock();                                                          \
    palWaitLineTimeoutS(line, timeout);                                     \
    osalSysUnlock();                                                        \
  } while (false)

#endif /* PAL_USE_WAIT */

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  ioportmask_t palReadBus(IOBus *bus);
  void palWriteBus(IOBus *bus, ioportmask_t bits);
  void palSetBusMode(IOBus *bus, iomode_t mode);
#if PAL_USE_WAIT || defined(__DOXYGEN__)
  msg_t palWaitPadTimeoutS(ioportid_t port, iopadid_t pad, systime_t timeout);
  msg_t palWaitLineTimeoutS(ioline_t line, systime_t timeout);
#endif /* PAL_USE_WAIT */
#ifdef __cplusplus
}
#endif

#endif /* HAL_PAL_H */

#endif /* HAL_USE_PAL == TRUE */

/** @} */
