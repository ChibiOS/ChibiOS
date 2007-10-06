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

static BYTE8 waThread1[UserStackSize(32)];

static t_msg Thread1(void *arg) {

  while (TRUE) {
    chThdSleep(800);
  }
  return 0;
}

int main(int argc, char **argv) {

  chSysInit();
  chThdCreate(NORMALPRIO, 0, waThread1, sizeof(waThread1), Thread1, NULL);
  chSysPause();
  return 0;
}
