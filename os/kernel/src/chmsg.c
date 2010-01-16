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
 * @file chmsg.c
 * @brief Messages code.
 * @addtogroup messages
 * @{
 */

#include "ch.h"

#if CH_USE_MESSAGES

#if CH_USE_MESSAGES_PRIORITY
#define msg_insert(tp, qp) prio_insert(tp, qp)
#else
#define msg_insert(tp, qp) queue_insert(tp, qp)
#endif

/**
 * @brief Sends a message to the specified thread.
 * @details The sender is stopped until the receiver executes a
 *          @p chMsgRelease()after receiving the message.
 *
 * @param[in] tp the pointer to the thread
 * @param[in] msg the message
 * @return The return message from @p chMsgRelease().
 */
msg_t chMsgSend(Thread *tp, msg_t msg) {

  chDbgCheck(tp != NULL, "chMsgSend");

  chSysLock();
  msg_insert(currp, &tp->p_msgqueue);
  currp->p_msg = msg;
  currp->p_wtthdp = tp;
  if (tp->p_state == PRWTMSG)
    chSchReadyI(tp);
  chSchGoSleepS(PRSNDMSG);
  msg = currp->p_rdymsg;
  chSysUnlock();
  return msg;
}

/**
 * @brief Suspends the thread and waits for an incoming message.
 *
 * @return The pointer to the message structure. Note, it is always the
 *         message associated to the thread on the top of the messages queue.
 * @note You can assume that the data contained in the message is stable until
 *       you invoke @p chMsgRelease() because the sending thread is
 *       suspended until then.
 */
msg_t chMsgWait(void) {
  msg_t msg;

  chSysLock();
  if (!chMsgIsPendingI(currp))
    chSchGoSleepS(PRWTMSG);
  msg = chMsgGetI(currp);
  chSysUnlock();
  return msg;
}

/**
 * @brief Returns the next message in the queue.
 *
 * @return The pointer to the message structure. Note, it is always the
 *         message associated to the thread on the top of the messages queue.
 *         If the queue is empty then @p NULL is returned.
 * @note You can assume that the data pointed by the message is stable until
 *       you invoke @p chMsgRelease() because the sending thread is
 *       suspended until then. Always remember that the message data is not
 *       copied between the sender and the receiver, just a pointer is passed.
 */
msg_t chMsgGet(void) {
  msg_t msg;

  chSysLock();
  msg = chMsgIsPendingI(currp) ? chMsgGetI(currp) : (msg_t)NULL;
  chSysUnlock();
  return msg;
}

/**
 * @brief Releases the thread waiting on top of the messages queue.
 *
 * @param[in] msg the message returned to the message sender
 * @note You can call this function only if there is a message already in the
 *       queue else the result will be unpredictable (a crash most likely).
 *       Exiting from the @p chMsgWait() ensures you have at least one
 *       message in the queue so it is not a big deal.<br>
 *       The condition is only tested in debug mode in order to make this code
 *       as fast as possible.
 */
void chMsgRelease(msg_t msg) {

  chSysLock();
  chDbgAssert(chMsgIsPendingI(currp),
              "chMsgRelease(), #1",
              "no message pending");
  chSchWakeupS(fifo_remove(&currp->p_msgqueue), msg);
  chSysUnlock();
}

#endif /* CH_USE_MESSAGES */

/** @} */
