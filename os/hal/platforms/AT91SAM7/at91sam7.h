/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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

#ifndef _AT91SAM7_H_
#define _AT91SAM7_H_

/*
 * Supported platforms.
 */
#define SAM7S64         0
#define SAM7S128        1
#define SAM7S256        2
#define SAM7S512        3
#define SAM7X128        4
#define SAM7X256        5
#define SAM7X512        6

#ifndef SAM7_PLATFORM
#error "SAM7 platform not defined"
#endif

#if SAM7_PLATFORM == SAM7S64
#include "at91lib/AT91SAM7S64.h"
#elif SAM7_PLATFORM == SAM7S128
#include "at91lib/AT91SAM7S128.h"
#elif SAM7_PLATFORM == SAM7S256
#include "at91lib/AT91SAM7S256.h"
#elif SAM7_PLATFORM == SAM7S512
#include "at91lib/AT91SAM7S512.h"
#elif SAM7_PLATFORM == SAM7X128
#include "at91lib/AT91SAM7X128.h"
#elif SAM7_PLATFORM == SAM7X256
#include "at91lib/AT91SAM7X256.h"
#elif SAM7_PLATFORM == SAM7X512
#include "at91lib/AT91SAM7X512.h"
#else
#error "SAM7 platform not supported"
#endif

#endif /* _AT91SAM7_H_ */
