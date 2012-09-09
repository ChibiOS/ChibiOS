/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief Console driver 1.
 */
BaseChannel CD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/


static size_t writes(void *ip, const uint8_t *bp, size_t n) {
  size_t ret;

  (void)ip;
  ret = fwrite(bp, 1, n, stdout);
  fflush(stdout);
  return ret;
}

static size_t reads(void *ip, uint8_t *bp, size_t n) {

  (void)ip;
  return fread(bp, 1, n, stdin);
}

static bool_t putwouldblock(void *ip) {

  (void)ip;
  return FALSE;
}

static bool_t getwouldblock(void *ip) {

  (void)ip;
  return TRUE;
}

static msg_t putt(void *ip, uint8_t b, systime_t time) {

  (void)ip;
  (void)time;
  fputc(b, stdout);
  fflush(stdout);
  return RDY_OK;
}

static msg_t gett(void *ip, systime_t time) {

  (void)ip;
  (void)time;
  return fgetc(stdin);
}

static size_t writet(void *ip, const uint8_t *bp, size_t n, systime_t time) {
  size_t ret;

  (void)ip;
  (void)time;
  ret = fwrite(bp, 1, n, stdout);
  fflush(stdout);
  return ret;
}

static size_t readt(void *ip, uint8_t *bp, size_t n, systime_t time) {

  (void)ip;
  (void)time;
  return fread(bp, 1, n, stdin);
}

static const struct BaseChannelVMT vmt = {
  writes, reads, putwouldblock, getwouldblock, putt, gett, writet, readt
};

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void conInit(void) {

  CD1.vmt = &vmt;
}

/** @} */
