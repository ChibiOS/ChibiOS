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

#ifndef _STM8_H_
#define _STM8_H_

/*
 * Supported platforms.
 */
#define PLATFORM_STM8S208RB     1
#define PLATFORM_STM8AF51AA     2

#ifndef STM8_PLATFORM
#error "STM8 platform not defined"
#endif

#if STM8_PLATFORM == PLATFORM_STM8S208RB
#include "STM8/STM8S208RB.h"
#elif STM8_PLATFORM == PLATFORM_STM8AF51AA
#include "STM8/STM8AF51AA.h"
#else
#error "unsupported or invalid STM8 platform"
#endif

#endif /* _STM8_H_ */
