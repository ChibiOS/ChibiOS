/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
#include <stdio.h>

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h"

#include "settings.h"
#include "cmd.h"

#define TEST_WA_SIZE        THD_WA_SIZE(256)

/*===========================================================================*/
/* Test suite command.                                                       */
/*===========================================================================*/
static void cmd_test(BaseChannel *chp, int argc, char *argv[]) {
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    shellPrintLine(chp, "Usage: test");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriority(),
                           TestThread, chp);
  if (tp == NULL) {
    shellPrintLine(chp, "out of memory");
    return;
  }
  chThdWait(tp);
}

/*===========================================================================*/
/* PWM command.                                                              */
/*===========================================================================*/
static void cmd_pwm(BaseChannel *chp, int argc, char *argv[]) {
  pwmchannel_t channel;

  (void)argv;
  if (argc != 2) {
    shellPrintLine(chp, "Usage: pwm <channel> on|off|<n>");
    return;
  }
  channel = atoi(argv[1]);
}

static const ShellCommand commands[] = {
  {"test", cmd_test},
  {"pwm", cmd_pwm},
  {NULL, NULL}
};

const ShellConfig shell_cfg = {
  (BaseChannel *)&SD2,
  commands
};
