/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <ch.h>
#include <evtimer.h>
#include <avr_serial.h>

#include <avr/io.h>

#include "board.h"
#include "lcd.h"

void hwinit(void);

static WORKING_AREA(waThread1, 32);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    if (!(PINA & PORTA_BUTTON2))
      PORTA ^= PORTA_RELAY;
    chThdSleepMilliseconds(1000);
  }
  return 0;
}

static void TimerHandler(eventid_t id) {
  msg_t TestThread(void *p);

  if (!(PINA & PORTA_BUTTON1))
    TestThread(&SER2);
}

int main(int argc, char **argv) {
  static EvTimer evt;
  static evhandler_t handlers[1] = {
    TimerHandler
  };
  static EventListener el0;

  hwinit();

  /*
   * The main() function becomes a thread here then the interrupts are
   * enabled and ChibiOS/RT goes live.
   */
  chSysInit();

  /*
   * This initialization requires the OS already active because it uses delay
   * APIs inside.
   */
  lcdInit();
  lcdCmd(LCD_CLEAR);
  lcdPuts(LCD_LINE1, "   ChibiOS/RT   ");
  lcdPuts(LCD_LINE2, "  Hello World!  ");

  /*
   * Event Timer initialization.
   */
  evtInit(&evt, MS2ST(500));            /* Initializes an event timer object.   */
  evtStart(&evt);                       /* Starts the event timer.              */
  chEvtRegister(&evt.et_es, &el0, 0);   /* Registers on the timer event source. */

  /*
   * Starts the LED blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  while(TRUE)
    chEvtDispatch(handlers, chEvtWaitOne(ALL_EVENTS));

  return 0;
}
