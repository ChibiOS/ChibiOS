/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    serial.c
 * @brief   Serial Driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

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

  return chOQWriteTimeout(&((SerialDriver *)ip)->oqueue, bp,
                          n, TIME_INFINITE);
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {

  return chIQReadTimeout(&((SerialDriver *)ip)->iqueue, bp,
                         n, TIME_INFINITE);
}

static bool_t putwouldblock(void *ip) {

  return chOQIsFullI(&((SerialDriver *)ip)->oqueue);
}

static bool_t getwouldblock(void *ip) {

  return chIQIsEmptyI(&((SerialDriver *)ip)->iqueue);
}

static msg_t putt(void *ip, uint8_t b, systime_t timeout) {

  return chOQPutTimeout(&((SerialDriver *)ip)->oqueue, b, timeout);
}

static msg_t gett(void *ip, systime_t timeout) {

  return chIQGetTimeout(&((SerialDriver *)ip)->iqueue, timeout);
}

static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {

  return chOQWriteTimeout(&((SerialDriver *)ip)->oqueue, bp, n, time);
}

static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {

  return chIQReadTimeout(&((SerialDriver *)ip)->iqueue, bp, n, time);
}

static ioflags_t getflags(void *ip) {
  _ch_get_and_clear_flags_impl(ip);
}

static const struct SerialDriverVMT vmt = {
  writes, reads, putwouldblock, getwouldblock,
  putt, gett, writet, readt,
  getflags
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Serial Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sdInit(void) {

  sd_lld_init();
}

/**
 * @brief   Initializes a generic full duplex driver object.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] sdp      pointer to a @p SerialDriver structure
 * @param[in] inotify   pointer to a callback function that is invoked when
 *                      some data is read from the Queue. The value can be
 *                      @p NULL.
 * @param[in] onotify   pointer to a callback function that is invoked when
 *                      some data is written in the Queue. The value can be
 *                      @p NULL.
 *
 * @init
 */
void sdObjectInit(SerialDriver *sdp, qnotify_t inotify, qnotify_t onotify) {

  sdp->vmt = &vmt;
  chEvtInit(&sdp->event);
  sdp->flags = IO_NO_ERROR;
  sdp->state = SD_STOP;
  chIQInit(&sdp->iqueue, sdp->ib, SERIAL_BUFFERS_SIZE, inotify);
  chOQInit(&sdp->oqueue, sdp->ob, SERIAL_BUFFERS_SIZE, onotify);
}

/**
 * @brief   Configures and starts the driver.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @api
 */
void sdStart(SerialDriver *sdp, const SerialConfig *config) {

  chDbgCheck(sdp != NULL, "sdStart");

  chSysLock();
  chDbgAssert((sdp->state == SD_STOP) || (sdp->state == SD_READY),
              "sdStart(), #1",
              "invalid state");
  sd_lld_start(sdp, config);
  sdp->state = SD_READY;
  chSysUnlock();
}

/**
 * @brief   Stops the driver.
 * @details Any thread waiting on the driver's queues will be awakened with
 *          the message @p Q_RESET.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @api
 */
void sdStop(SerialDriver *sdp) {

  chDbgCheck(sdp != NULL, "sdStop");

  chSysLock();
  chDbgAssert((sdp->state == SD_STOP) || (sdp->state == SD_READY),
              "sdStop(), #1",
              "invalid state");
  sd_lld_stop(sdp);
  sdp->state = SD_STOP;
  chOQResetI(&sdp->oqueue);
  chIQResetI(&sdp->iqueue);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief   Handles incoming data.
 * @details This function must be called from the input interrupt service
 *          routine in order to enqueue incoming data and generate the
 *          related events.
 * @note    The incoming data event is only generated when the input queue
 *          becomes non-empty.
 * @note    In order to gain some performance it is suggested to not use
 *          this function directly but copy this code directly into the
 *          interrupt service routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver structure
 * @param[in] b         the byte to be written in the driver's Input Queue
 *
 * @iclass
 */
void sdIncomingDataI(SerialDriver *sdp, uint8_t b) {

  chDbgCheckClassI();
  chDbgCheck(sdp != NULL, "sdIncomingDataI");

  if (chIQIsEmptyI(&sdp->iqueue))
    chIOAddFlagsI(sdp, IO_INPUT_AVAILABLE);
  if (chIQPutI(&sdp->iqueue, b) < Q_OK)
    chIOAddFlagsI(sdp, SD_OVERRUN_ERROR);
}

/**
 * @brief   Handles outgoing data.
 * @details Must be called from the output interrupt service routine in order
 *          to get the next byte to be transmitted.
 * @note    In order to gain some performance it is suggested to not use
 *          this function directly but copy this code directly into the
 *          interrupt service routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver structure
 * @return              The byte value read from the driver's output queue.
 * @retval Q_EMPTY      if the queue is empty (the lower driver usually
 *                      disables the interrupt source when this happens).
 *
 * @iclass
 */
msg_t sdRequestDataI(SerialDriver *sdp) {
  msg_t  b;

  chDbgCheckClassI();
  chDbgCheck(sdp != NULL, "sdRequestDataI");

  b = chOQGetI(&sdp->oqueue);
  if (b < Q_OK)
    chIOAddFlagsI(sdp, IO_OUTPUT_EMPTY);
  return b;
}

#endif /* HAL_USE_SERIAL */

/** @} */
