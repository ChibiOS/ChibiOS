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
 * @addtogroup Messages
 * @{
 */

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#ifdef CH_USE_MESSAGES

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

t_msg chMsgSend(Thread *tp, t_msg msg);
t_msg chMsgWait(void);
t_msg chMsgGet(void);
void chMsgRelease(t_msg msg);

#ifdef CH_USE_MESSAGES_EVENT
t_msg chMsgSendWithEvent(Thread *tp, t_msg msg, EventSource *esp);
#endif

#ifdef CH_USE_MESSAGES_TIMEOUT
t_msg chMsgSendTimeout(Thread *tp, t_msg msg, t_time time);
#endif

#endif /* CH_USE_MESSAGES */

#endif /* _MESSAGES_H_ */

/** @} */
