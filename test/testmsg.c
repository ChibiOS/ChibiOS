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

#include "ch.h"
#include "test.h"

/**
 * @page test_msg Messages test
 *
 * File: @ref testmsg.c
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref messages subsystem.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref messages
 * subsystem code.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_MESSAGES
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * <h2>Test Cases</h2>
 * - @subpage test_msg_001
 * .
 * @file testmsg.c
 * @brief Messages test source file
 * @file testmsg.h
 * @brief Messages header file
 */

#if CH_USE_MESSAGES || defined(__DOXYGEN__)

/**
 * @page test_msg_001 Messages Server loop
 *
 * <h2>Description</h2>
 * A thread is spawned that sends four messages back to the tester thread.<br>
 * The test expect to receive the messages in the correct sequence and to
 * not find a fifth message waiting.
 */

static msg_t thread(void *p) {

  chMsgSend(p, 'A');
  chMsgSend(p, 'B');
  chMsgSend(p, 'C');
  return 0;
}

static void msg1_execute(void) {
  Thread *tp;
  msg_t msg;

  /*
   * Testing the whole messages loop.
   */
  threads[0] = chThdCreateStatic(wa[0], WA_SIZE, chThdGetPriority() + 1,
                                 thread, chThdSelf());
  tp = chMsgWait();
  msg = chMsgGet(tp);
  chMsgRelease(tp, msg);
  test_emit_token(msg);
  tp = chMsgWait();
  msg = chMsgGet(tp);
  chMsgRelease(tp, msg);
  test_emit_token(msg);
  tp = chMsgWait();
  msg = chMsgGet(tp);
  chMsgRelease(tp, msg);
  test_emit_token(msg);
  test_assert_sequence(1, "ABC");
}

ROMCONST struct testcase testmsg1 = {
  "Messages, loop",
  NULL,
  NULL,
  msg1_execute
};

#endif /* CH_USE_MESSAGES */

/**
 * @brief   Test sequence for messages.
 */
ROMCONST struct testcase * ROMCONST patternmsg[] = {
#if CH_USE_MESSAGES || defined(__DOXYGEN__)
  &testmsg1,
#endif
  NULL
};
