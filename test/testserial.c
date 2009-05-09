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

#include <ch.h>

#include "test.h"

/**
 * @page test_serial Serial Drivers test
 *
 * <h2>Description</h2>
 * This module implements the test sequence for the @ref Serial subsystem.
 * The tests are performed on a loopback software serial driver where a
 * dedicated thread echoes back in the input queue the data read from the
 * output queue at a fixed rate.
 *
 * <h2>Objective</h2>
 * Objective of the test module is to cover 100% of the @ref Serial code
 * as a necessary step in order to assess its maturity level.<br>
 * Note that the @ref Serial subsystem depends on the @ref Semaphores and
 * @ref Events subsystems that have to met their testing objectives as well.
 *
 * <h2>Preconditions</h2>
 * The module requires the following kernel options:
 * - @p CH_USE_SERIAL_FULLDUPLEX (and dependent options)
 * .
 * In case some of the required options are not enabled then some or all tests
 * may be skipped.
 *
 * @file testserial.c
 * @brief Kernel Serial Driver test source file
 * @file testserial.h
 * @brief Kernel Serial Driver test header file
 */

#if CH_USE_SERIAL_FULLDUPLEX

#endif /* CH_USE_SERIAL_FULLDUPLEX */

/*
 * Test sequence for queues pattern.
 */
const struct testcase * const patternserial[] = {
#if CH_USE_SERIAL_FULLDUPLEX
//  &testserial1,
#endif
  NULL
};
