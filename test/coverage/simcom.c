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

/*
 * Win32 COM port simulator (on stdout).
 */

#include <chconf.h>

#include <windows.h>
#include <stdio.h>

#undef CDECL

#include <ch.h>

FullDuplexDriver COM1;

static uint8_t com_ib[1024];
static uint8_t com_ob[1024];

void init_simcom1(void) {

  chFDDInit(&COM1, com_ib, sizeof(com_ib), NULL, com_ob, sizeof(com_ob), NULL);
}

bool_t com1_conn_chkint(void) {

  return FALSE;
}

bool_t com1_in_chkint(void) {

  return FALSE;
}

bool_t com1_out_chkint(void) {
  msg_t n;
  bool_t rflag = FALSE;

  while (TRUE) {
    n = chFDDRequestDataI(&COM1);
    if (n < 0) {
      fflush(stdout);
      return rflag;
    }
    fputc(n, stdout);
    rflag = TRUE;
  }
}
