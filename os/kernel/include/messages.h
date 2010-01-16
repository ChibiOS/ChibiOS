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
 * @file messages.h
 * @brief Messages macros and structures.
 * @addtogroup messages
 * @{
 */

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#if CH_USE_MESSAGES

/**
 * Evaluates to TRUE if the thread has pending messages.
 */
#define chMsgIsPendingI(tp) \
        ((tp)->p_msgqueue.p_next != (Thread *)&(tp)->p_msgqueue)

/**
 * Returns the first message in the queue.
 */
#define chMsgGetI(tp) \
        ((tp)->p_msgqueue.p_next->p_msg)

#ifdef __cplusplus
extern "C" {
#endif
  msg_t chMsgSend(Thread *tp, msg_t msg);
  msg_t chMsgWait(void);
  msg_t chMsgGet(void);
  void chMsgRelease(msg_t msg);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_MESSAGES */

#endif /* _MESSAGES_H_ */

/** @} */
