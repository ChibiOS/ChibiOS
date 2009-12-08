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
 * @file console.c
 * @brief Simulator console driver code.
 * @{
 */

#include <stdio.h>

#include "ch.h"
#include "console.h"

/**
 * @brief Console driver 1.
 */
BaseChannel CD1;

/*
 * Interface implementation, the following functions just invoke the equivalent
 * queue-level function or macro.
 */
static bool_t putwouldblock(void *ip) {

  (void)ip;
  return FALSE;
}

static bool_t getwouldblock(void *ip) {

  (void)ip;
  return FALSE; /******************************/
}

static msg_t put(void *ip, uint8_t b, systime_t timeout) {

  (void)ip;
  (void)timeout;
  fputc(b, stdout);
  fflush(stdout);
  return RDY_OK;
}

static msg_t get(void *ip, systime_t timeout) {

  (void)ip;
  (void)timeout;
  return 0;
}

static const struct BaseChannelVMT vmt = {
  {putwouldblock, getwouldblock, put, get}
};

void conInit(void) {

  CD1.vmt = &vmt;
}

/** @} */
