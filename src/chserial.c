/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file chserial.c
 * @brief Serial Drivers code.
 * @addtogroup Serial
 * @{
 */

#include <ch.h>

#if CH_USE_SERIAL_FULLDUPLEX
/**
 * @brief Initializes a generic full duplex driver.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] sd pointer to a @p FullDuplexDriver structure
 * @param[in] ib pointer to a memory area allocated for the Input Queue buffer
 * @param[in] isize size of the Input Queue buffer
 * @param[in] inotify pointer to a callback function that is invoked when
 *                    some data is read from the Queue. The value can be
 *                    @p NULL.
 * @param[in] ob pointer to a memory area allocated for the Output Queue buffer
 * @param[in] osize size of the Output Queue buffer
 * @param[in] onotify pointer to a callback function that is invoked when
 *                    some data is written in the Queue. The value can be
 *                    @p NULL.
 */
void chFDDInit(FullDuplexDriver *sd,
               uint8_t *ib, size_t isize, qnotify_t inotify,
               uint8_t *ob, size_t osize, qnotify_t onotify) {

  chDbgCheck((sd != NULL) && (ib != NULL) && (ob != NULL) &&
             (isize > 0) && (osize > 0), "chFDDInit");

  chIQInit(&sd->sd_iqueue, ib, isize, inotify);
  chEvtInit(&sd->sd_ievent);
  chOQInit(&sd->sd_oqueue, ob, osize, onotify);
  chEvtInit(&sd->sd_oevent);
  chEvtInit(&sd->sd_sevent);
  sd->sd_flags = SD_NO_ERROR;
}

/**
 * @brief Handles incoming data.
 * @details This function must be called from the input interrupt service
 *          routine in order to enqueue incoming data and generate the
 *          related events.
 * @param[in] sd pointer to a @p FullDuplexDriver structure
 * @param[in] b the byte to be written in the driver's Input Queue
 */
void chFDDIncomingDataI(FullDuplexDriver *sd, uint8_t b) {

  if (chIQPutI(&sd->sd_iqueue, b) < Q_OK)
    chFDDAddFlagsI(sd, SD_OVERRUN_ERROR);
  else
    chEvtBroadcastI(&sd->sd_ievent);
}

/**
 * @brief Handles outgoing data.
 * @details Must be called from the output interrupt service routine in order
 *          to get the next byte to be transmitted.
 *
 * @param[in] sd pointer to a @p FullDuplexDriver structure
 * @return The byte value read from the driver's output queue.
 * @retval Q_EMPTY if the queue is empty (the lower driver usually disables
 *                 the interrupt source when this happens).
 */
msg_t chFDDRequestDataI(FullDuplexDriver *sd) {

  msg_t b = chOQGetI(&sd->sd_oqueue);
  if (b < Q_OK)
    chEvtBroadcastI(&sd->sd_oevent);
  return b;
}

/**
 * @brief Handles communication events/errors.
 * @details Must be called from the I/O interrupt service routine in order to
 *          notify I/O conditions as errors, signals change etc.
 *
 * @param[in] sd pointer to a @p FullDuplexDriver structure
 * @param[in] mask condition flags to be added to the mask
 */
void chFDDAddFlagsI(FullDuplexDriver *sd, dflags_t mask) {

  sd->sd_flags |= mask;
  chEvtBroadcastI(&sd->sd_sevent);
}

/**
 * @brief Returns and clears the errors mask associated to the driver.
 *
 * @param[in] sd pointer to a @p FullDuplexDriver structure
 * @return The condition flags modified since last time this function was
 *         invoked.
 */
dflags_t chFDDGetAndClearFlags(FullDuplexDriver *sd) {
  dflags_t mask;

  mask = sd->sd_flags;
  sd->sd_flags = SD_NO_ERROR;
  return mask;
}
#endif /* CH_USE_SERIAL_FULLDUPLEX */

#if CH_USE_SERIAL_HALFDUPLEX
/**
 * @brief Initializes a generic half duplex driver.
 * @details The HW dependent part of the initialization has to be performed
 *          outside, usually in the hardware initialization code.
 *
 * @param[out] sd pointer to a @p HalfDuplexDriver structure
 * @param[in] b pointer to a memory area allocated for the queue buffer
 * @param[in] size the buffer size
 * @param[in] inotify pointer to a callback function that is invoked when
 *                    some data is read from the queue. The value can be
 *                    @p NULL.
 * @param[in] onotify pointer to a callback function that is invoked when
 *                    some data is written in the queue. The value can be
 *                    @p NULL.
 */
void chHDDInit(HalfDuplexDriver *sd, uint8_t *b, size_t size,
               qnotify_t inotify, qnotify_t onotify) {

  chDbgCheck((sd != NULL) && (b != NULL) && (size > 0), "chHDDInit");

  chHDQInit(&sd->sd_queue, b, size, inotify, onotify);
  chEvtInit(&sd->sd_ievent);
  chEvtInit(&sd->sd_oevent);
  chEvtInit(&sd->sd_sevent);
  sd->sd_flags = SD_NO_ERROR;
}

/**
 * @brief Handles incoming data.
 * @details This function must be called from the input interrupt service
 *          routine in order to enqueue incoming data and generate the
 *          related events.
 * @param[in] sd pointer to a @p FullDuplexDriver structure
 * @param[in] b the byte to be written in the driver's input queue
 */
void chHDDIncomingDataI(HalfDuplexDriver *sd, uint8_t b) {

  if (chHDQPutReceiveI(&sd->sd_queue, b) < Q_OK)
    chHDDAddFlagsI(sd, SD_OVERRUN_ERROR);
  else
    chEvtBroadcastI(&sd->sd_ievent);
}

/**
 * @brief Handles outgoing data.
 * @details Must be called from the output interrupt service routine in order
 *          to get the next byte to be transmitted.
 *
 * @param[in] sd pointer to a @p HalfDuplexDriver structure
 * @return The byte value read from the driver's output queue.
 * @retval Q_EMPTY if the queue is empty (the lower driver usually disables
 *                 the interrupt source when this happens).
 */
msg_t chHDDRequestDataI(HalfDuplexDriver *sd) {

  msg_t b = chHDQGetTransmitI(&sd->sd_queue);
  if (b < Q_OK)
    chEvtBroadcastI(&sd->sd_oevent);
  return b;
}

/**
 * @brief Handles communication events/errors.
 * @details Must be called from the I/O interrupt service routine in order to
 *          notify I/O conditions as errors, signals change etc.
 *
 * @param[in] sd pointer to a @p HalfDuplexDriver structure
 * @param[in] mask condition flags to be added to the mask
 */
void chHDDAddFlagsI(HalfDuplexDriver *sd, dflags_t mask) {

  sd->sd_flags |= mask;
  chEvtBroadcastI(&sd->sd_sevent);
}

/**
 * @brief Returns and clears the errors mask associated to the driver.
 *
 * @param[in] sd pointer to a @p HalfDuplexDriver structure
 * @return The condition flags modified since last time this function was
 *         invoked.
 */
dflags_t chHDDGetAndClearFlags(HalfDuplexDriver *sd) {
  dflags_t mask;

  mask = sd->sd_flags;
  sd->sd_flags = SD_NO_ERROR;
  return mask;
}
#endif /* CH_USE_SERIAL_HALFDUPLEX */

/** @} */
