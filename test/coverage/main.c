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

#include <windows.h>
#include <string.h>
#include <stdio.h>

#include <ch.h>
#include <test.h>

extern FullDuplexDriver COM1;

/*
 * Simulator main.
 */
int main(int argc, char *argv[]) {
  msg_t result;

  chSysInit();
  result = TestThread(&COM1);
  chThdSleepMilliseconds(1); /* Gives time to flush COM1 output queue */
  fflush(stdout);
  if (result)
    exit(1);
  else
    exit(0);
}
