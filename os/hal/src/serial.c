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
 * @file serial.c
 * @brief Serial Driver code.
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*
 * Interface implementation, the following functions just invoke the equivalent
 * queue-level function or macro.
 */

static size_t writes(void *ip, const uint8_t *bp, size_t n) {

  return chOQWriteTimeout(&((SerialDriver *)ip)->sd.oqueue, bp,
                          n, TIME_INFINITE);
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {

  return chIQReadTimeout(&((SerialDriver *)ip)->sd.iqueue, bp,
                         n, TIME_INFINITE);
}

static bool_t putwouldblock(void *ip) {

  return chOQIsFull(&((SerialDriver *)ip)->sd.oqueue);
}

static bool_t getwouldblock(void *ip) {

  return chIQIsEmpty(&((SerialDriver *)ip)->sd.iqueue);
}

static msg_t putt(void *ip, uint8_t b, systime_t timeout) {

  return chOQPutTimeout(&((SerialDriver *)ip)->sd.oqueue, b, timeout);
}

static msg_t gett(void *ip, systime_t timeout) {

  return chIQGetTimeout(&((SerialDriver *)ip)->sd.iqueue, timeout);
}

static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {

  return chOQWriteTimeout(&((SerialDriver *)ip)->sd.oqueue, bp, n, time);
}

static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {

  return chIQReadTimeout(&((SerialDriver *)ip)->sd.iqueue, bp, n, time);
}

static const struct SerialDriverVMT vmt = {
  {writes, reads},
  {putwouldblock, getwouldblock, putt, gett, writet, readt},
  {}
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Serial Driver initialization.
 */
void sdInit(void) {

  sd_lld_init();
}

/**
 * @brief Initializes a generic full duplex driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] sdp pointer to a @p SerialDriver structure
 * @param[in] inotify pointer to a callback function that is invoked when
 *                    some data is read from the Queue. The value can be
 *                    @p NULL.
 * @param[in] onotify pointer to a callback function that is invoked when
 *                    some data is written in the Queue. The value can be
 *                    @p NULL.
 */
void sdObjectInit(SerialDriver *sdp, qnotify_t inotify, qnotify_t onotify) {

  sdp->vmt = &vmt;
  chEvtInit(&sdp->bac.ievent);
  chEvtInit(&sdp->bac.oevent);
  chEvtInit(&sdp->sd.sevent);
  sdp->sd.state = SD_STOP;
  sdp->sd.flags = SD_NO_ERROR;
  chIQInit(&sdp->sd.iqueue, sdp->sd.ib, SERIAL_BUFFERS_SIZE, inotify);
  chOQInit(&sdp->sd.oqueue, sdp->sd.ob, SERIAL_BUFFERS_SIZE, onotify);
}

/**
 * @brief Configures and starts the driver.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 * @param[in] config the architecture-dependent serial driver configuration.
 *                   If this parameter is set to @p NULL then a default
 *                   configuration is used.
 */
void sdStart(SerialDriver *sdp, const SerialConfig *config) {

  chDbgCheck(sdp != NULL, "sdStart");

  chSysLock();
  chDbgAssert((sdp->sd.state == SD_STOP) || (sdp->sd.state == SD_READY),
              "sdStart(), #1",
              "invalid state");
  sdp->sd.config = config;
  sd_lld_start(sdp);
  sdp->sd.state = SD_READY;
  chSysUnlock();
}

/**
 * @brief Stops the driver.
 * @details Any thread waiting on the driver's queues will be awakened with
 *          the message @p Q_RESET.
 *
 * @param[in] sdp pointer to a @p SerialDrive object
 */
void sdStop(SerialDriver *sdp) {

  chDbgCheck(sdp != NULL, "sdStop");

  chSysLock();
  chDbgAssert((sdp->sd.state == SD_STOP) || (sdp->sd.state == SD_READY),
              "sdStop(), #1",
              "invalid state");
  sd_lld_stop(sdp);
  sdp->sd.state = SD_STOP;
  chOQResetI(&sdp->sd.oqueue);
  chIQResetI(&sdp->sd.iqueue);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief Handles incoming data.
 * @details This function must be called from the input interrupt service
 *          routine in order to enqueue incoming data and generate the
 *          related events.
 * @note The incoming data event is only generated when the input queue
 *       becomes non-empty.
 * @note In order to gain some performance it is suggested to not use
 *       this function directly but copy this code directly into the
 *       interrupt service routine.
 *
 * @param[in] sdp pointer to a @p SerialDriver structure
 * @param[in] b the byte to be written in the driver's Input Queue
 */
void sdIncomingDataI(SerialDriver *sdp, uint8_t b) {

  chDbgCheck(sdp != NULL, "sdIncomingDataI");

  if (chIQIsEmpty(&sdp->sd.iqueue))
    chEvtBroadcastI(&sdp->bac.ievent);
  if (chIQPutI(&sdp->sd.iqueue, b) < Q_OK)
    sdAddFlagsI(sdp, SD_OVERRUN_ERROR);
}

/**
 * @brief Handles outgoing data.
 * @details Must be called from the output interrupt service routine in order
 *          to get the next byte to be transmitted.
 * @note In order to gain some performance it is suggested to not use
 *       this function directly but copy this code directly into the
 *       interrupt service routine.
 *
 * @param[in] sdp pointer to a @p SerialDriver structure
 * @return The byte value read from the driver's output queue.
 * @retval Q_EMPTY if the queue is empty (the lower driver usually disables
 *                 the interrupt source when this happens).
 */
msg_t sdRequestDataI(SerialDriver *sdp) {

  chDbgCheck(sdp != NULL, "sdRequestDataI");

  msg_t b = chOQGetI(&sdp->sd.oqueue);
  if (b < Q_OK)
    chEvtBroadcastI(&sdp->bac.oevent);
  return b;
}

/**
 * @brief Handles communication events/errors.
 * @details Must be called from the I/O interrupt service routine in order to
 *          notify I/O conditions as errors, signals change etc.
 *
 * @param[in] sdp pointer to a @p SerialDriver structure
 * @param[in] mask condition flags to be added to the mask
 */
void sdAddFlagsI(SerialDriver *sdp, sdflags_t mask) {

  chDbgCheck(sdp != NULL, "sdAddFlagsI");

  sdp->sd.flags |= mask;
  chEvtBroadcastI(&sdp->sd.sevent);
}

/**
 * @brief Returns and clears the errors mask associated to the driver.
 *
 * @param[in] sdp pointer to a @p SerialDriver structure
 * @return The condition flags modified since last time this function was
 *         invoked.
 */
sdflags_t sdGetAndClearFlags(SerialDriver *sdp) {
  sdflags_t mask;

  chDbgCheck(sdp != NULL, "sdGetAndClearFlags");

  chSysLock();
  mask = sdp->sd.flags;
  sdp->sd.flags = SD_NO_ERROR;
  chSysUnlock();
  return mask;
}

#endif /* CH_HAL_USE_SERIAL */

/** @} */
