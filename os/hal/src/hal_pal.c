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
 * @file    hal_pal.c
 * @brief   I/O Ports Abstraction Layer code.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Read from an I/O bus.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The function internally uses the @p palReadGroup() macro. The use
 *          of this function is preferred when you value code size, readability
 *          and error checking over speed.
 * @note    The function can be called from any context.
 *
 * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
 * @return              The bus logical states.
 *
 * @special
 */
ioportmask_t palReadBus(IOBus *bus) {

  osalDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH));

  return palReadGroup(bus->portid, bus->mask, bus->offset);
}

/**
 * @brief   Write to an I/O bus.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The default implementation is non atomic and not necessarily
 *          optimal. Low level drivers may  optimize the function by using
 *          specific hardware or coding.
 * @note    The function can be called from any context.
 *
 * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
 * @param[in] bits      the bits to be written on the I/O bus. Values exceeding
 *                      the bus width are masked so most significant bits are
 *                      lost.
 *
 * @special
 */
void palWriteBus(IOBus *bus, ioportmask_t bits) {

  osalDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH));

  palWriteGroup(bus->portid, bus->mask, bus->offset, bits);
}

/**
 * @brief   Programs a bus with the specified mode.
 * @note    The operation is not guaranteed to be atomic on all the
 *          architectures, for atomicity and/or portability reasons you may
 *          need to enclose port I/O operations between @p osalSysLock() and
 *          @p osalSysUnlock().
 * @note    The default implementation is non atomic and not necessarily
 *          optimal. Low level drivers may  optimize the function by using
 *          specific hardware or coding.
 * @note    The function can be called from any context.
 *
 * @param[in] bus       the I/O bus, pointer to a @p IOBus structure
 * @param[in] mode      the mode
 *
 * @special
 */
void palSetBusMode(IOBus *bus, iomode_t mode) {

  osalDbgCheck((bus != NULL) && (bus->offset < PAL_IOPORTS_WIDTH));

  palSetGroupMode(bus->portid, bus->mask, bus->offset, mode);
}

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
 * @sclass
 */
msg_t palWaitPadTimeoutS(ioportid_t port, iopadid_t pad, systime_t timeout) {

  palevent_t *pep = pal_lld_get_pad_event(port, pad);
  return osalThreadEnqueueTimeoutS(&pep->threads, timeout);
}

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
 * @sclass
 */
msg_t palWaitLineTimeoutS(ioline_t line, systime_t timeout) {

  palevent_t *pep = pal_lld_get_line_event(line);
  return osalThreadEnqueueTimeoutS(&pep->threads, timeout);
}
#endif /* PAL_USE_WAIT */

#endif /* HAL_USE_PAL == TRUE */

/** @} */
