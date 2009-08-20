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
 * @file serial.c
 * @brief Serial Driver code.
 * @addtogroup SERIAL
 * @{
 */

#include <ch.h>
#include <serial.h>

/*
 * Interface implementation, the following functions just invoke the equivalent
 * queue-level function or macro.
 */
static bool_t putwouldblock(void *ip) {

  return chOQIsFull(&((SerialDriver *)ip)->d2.oqueue);
}

static bool_t getwouldblock(void *ip) {

  return chIQIsEmpty(&((SerialDriver *)ip)->d2.iqueue);
}

static msg_t put(void *ip, uint8_t b, systime_t timeout) {

  return chOQPutTimeout(&((SerialDriver *)ip)->d2.oqueue, b, timeout);
}

static msg_t get(void *ip, systime_t timeout) {

  return chIQGetTimeout(&((SerialDriver *)ip)->d2.iqueue, timeout);
}

static size_t write(void *ip, uint8_t *buffer, size_t n) {

  return chOQWrite(&((SerialDriver *)ip)->d2.oqueue, buffer, n);
}

static size_t read(void *ip, uint8_t *buffer, size_t n) {

  return chIQRead(&((SerialDriver *)ip)->d2.iqueue, buffer, n);
}

static const struct SerialDriverVMT vmt = {
  {putwouldblock, getwouldblock, put, get},
  {write, read},
  {}
};

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
  chEvtInit(&sdp->d1.ievent);
  chEvtInit(&sdp->d1.oevent);
  chEvtInit(&sdp->d2.sevent);
  sdp->d2.flags = SD_NO_ERROR;
  chIQInit(&sdp->d2.iqueue, sdp->d2.ib, SERIAL_BUFFERS_SIZE, inotify);
  chOQInit(&sdp->d2.oqueue, sdp->d2.ob, SERIAL_BUFFERS_SIZE, onotify);
}

/**
 * @brief Configures and starts the driver.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 * @param[in] config the architecture-dependent serial driver configuration.
 *                   If this parameter is set to @p NULL then a default
 *                   configuration is used.
 */
void sdStart(SerialDriver *sdp, const SerialDriverConfig *config) {

  chSysLock();
  sd_lld_start(sdp, config);
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

  chSysLock();
  sd_lld_stop(sdp);
  chOQResetI(&sdp->d2.oqueue);
  chIQResetI(&sdp->d2.iqueue);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief Handles incoming data.
 * @details This function must be called from the input interrupt service
 *          routine in order to enqueue incoming data and generate the
 *          related events.
 * @param[in] sd pointer to a @p SerialDriver structure
 * @param[in] b the byte to be written in the driver's Input Queue
 */
void sdIncomingDataI(SerialDriver *sd, uint8_t b) {

  if (chIQPutI(&sd->d2.iqueue, b) < Q_OK)
    sdAddFlagsI(sd, SD_OVERRUN_ERROR);
  else
    chEvtBroadcastI(&sd->d1.ievent);
}

/**
 * @brief Handles outgoing data.
 * @details Must be called from the output interrupt service routine in order
 *          to get the next byte to be transmitted.
 *
 * @param[in] sd pointer to a @p SerialDriver structure
 * @return The byte value read from the driver's output queue.
 * @retval Q_EMPTY if the queue is empty (the lower driver usually disables
 *                 the interrupt source when this happens).
 */
msg_t sdRequestDataI(SerialDriver *sd) {

  msg_t b = chOQGetI(&sd->d2.oqueue);
  if (b < Q_OK)
    chEvtBroadcastI(&sd->d1.oevent);
  return b;
}

/**
 * @brief Handles communication events/errors.
 * @details Must be called from the I/O interrupt service routine in order to
 *          notify I/O conditions as errors, signals change etc.
 *
 * @param[in] sd pointer to a @p SerialDriver structure
 * @param[in] mask condition flags to be added to the mask
 */
void sdAddFlagsI(SerialDriver *sd, sdflags_t mask) {

  sd->d2.flags |= mask;
  chEvtBroadcastI(&sd->d2.sevent);
}

/**
 * @brief Returns and clears the errors mask associated to the driver.
 *
 * @param[in] sd pointer to a @p SerialDriver structure
 * @return The condition flags modified since last time this function was
 *         invoked.
 */
sdflags_t sdGetAndClearFlags(SerialDriver *sd) {
  sdflags_t mask;

  mask = sd->d2.flags;
  sd->d2.flags = SD_NO_ERROR;
  return mask;
}

/** @} */
