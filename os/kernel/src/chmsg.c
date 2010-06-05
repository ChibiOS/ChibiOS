/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    chmsg.c
 * @brief   Messages code.
 *
 * @addtogroup messages
 * @details Synchronous inter-thread messages APIs and services.
 *          <h2>Operation Mode</h2>
 *          Synchronous messages are an easy to use and fast IPC mechanism,
 *          threads can both act as message servers and/or message clients,
 *          the mechanism allows data to be carried in both directions. Note
 *          that messages are not copied between the client and server threads
 *          but just a pointer passed so the exchange is very time
 *          efficient.<br>
 *          Messages are usually processed in FIFO order but it is possible to
 *          process them in priority order by enabling the
 *          @p CH_USE_MESSAGES_PRIORITY option in @p chconf.h.<br>
 *          Applications do not need to allocate buffers for synchronous
 *          message queues, the mechanism just requires two extra pointers in
 *          the @p Thread structure (the message queue header).<br>
 *          In order to use the Messages APIs the @p CH_USE_MESSAGES option
 *          must be enabled in @p chconf.h.
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
 * @brief   Sends a message to the specified thread.
 * @details The sender is stopped until the receiver executes a
 *          @p chMsgRelease()after receiving the message.
 *
 * @param[in] tp        the pointer to the thread
 * @param[in] msg       the message
 * @return              The answer message from @p chMsgRelease().
 */
msg_t chMsgSend(Thread *tp, msg_t msg) {
  Thread *ctp = currp;

  chDbgCheck(tp != NULL, "chMsgSend");

  chSysLock();
  ctp->p_msg = msg;
  ctp->p_u.wtobjp = &tp->p_msgqueue;
  msg_insert(ctp, &tp->p_msgqueue);
  if (tp->p_state == THD_STATE_WTMSG)
    chSchReadyI(tp);
  chSchGoSleepS(THD_STATE_SNDMSG);
  msg = ctp->p_u.rdymsg;
  chSysUnlock();
  return msg;
}

/**
 * @brief   Suspends the thread and waits for an incoming message.
 * @note    You can assume that the data contained in the message is stable
 *          until you invoke @p chMsgRelease() because the sending thread is
 *          suspended until then.
 *
 * @return              The pointer to the message structure. Note, it is
 *                      always the message associated to the thread on the
 *                      top of the messages queue.
 */
msg_t chMsgWait(void) {
  msg_t msg;

  chSysLock();
  if (!chMsgIsPendingI(currp))
    chSchGoSleepS(THD_STATE_WTMSG);
#if defined(CH_ARCHITECTURE_STM8)
  msg = chMsgGetI((volatile Thread *)currp); /* Temporary hack.*/
#else
  msg = chMsgGetI(currp);
#endif
  chSysUnlock();
  return msg;
}

/**
 * @brief   Returns the next message in the queue.
 * @note    You can assume that the data pointed by the message is stable until
 *          you invoke @p chMsgRelease() because the sending thread is
 *          suspended until then. Always remember that the message data is not
 *          copied between the sender and the receiver, just a pointer is
 *          passed.
 *
 * @return              The pointer to the message structure. Note, it is
 *                      always the message associated to the thread on the
 *                      top of the messages queue.
 * @retval NULL         if the queue is empty.
 */
msg_t chMsgGet(void) {
  msg_t msg;

  chSysLock();
  msg = chMsgIsPendingI(currp) ? chMsgGetI(currp) : (msg_t)NULL;
  chSysUnlock();
  return msg;
}

/**
 * @brief   Releases the thread waiting on top of the messages queue.
 * @note    You can call this function only if there is a message already in
 *          the queue else the result will be unpredictable (a crash most likely).
 *          Exiting from the @p chMsgWait() ensures you have at least one
 *          message in the queue so it is not a big deal.<br>
 *          The condition is only tested in debug mode in order to make this
 *          code as fast as possible.
 *
 * @param[in] msg       the message returned to the message sender
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
