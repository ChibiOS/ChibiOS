/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 *          Messages are scalar data types of type @p msg_t that are guaranteed
 *          to be size compatible with data pointers. Note that on some
 *          architectures function pointers can be larger that @p msg_t.<br>
 *          Messages are usually processed in FIFO order but it is possible to
 *          process them in priority order by enabling the
 *          @p CH_USE_MESSAGES_PRIORITY option in @p chconf.h.<br>
 * @pre     In order to use the message APIs the @p CH_USE_MESSAGES option
 *          must be enabled in @p chconf.h.
 * @post    Enabling messages requires 6-12 (depending on the architecture)
 *          extra bytes in the @p Thread structure.
 * @{
 */

#include "ch.h"

#if CH_USE_MESSAGES || defined(__DOXYGEN__)

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
 *
 * @api
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
 * @post    After receiving a message the function @p chMsgRelease() must be
 *          invoked in order to acknowledge the reception and send the answer.
 * @note    If the message is a pointer then you can assume that the data
 *          pointed by the message is stable until you invoke @p chMsgRelease()
 *          because the sending thread is suspended until then.
 *
 * @return              The message.
 *
 * @api
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
 * @post    After receiving a message the function @p chMsgRelease() must be
 *          invoked in order to acknowledge the reception and send the answer.
 * @note    If the message is a pointer then you can assume that the data
 *          pointed by the message is stable until you invoke @p chMsgRelease()
 *          because the sending thread is suspended until then.
 *
 * @return              The message.
 * @retval 0            if the queue is empty.
 *
 * @api
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
 * @pre     Invoke this function only after a message has been received
 *          using @p chMsgWait() or @p chMsgGet().
 *
 * @param[in] msg       the message returned to the message sender
 *
 * @api
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
