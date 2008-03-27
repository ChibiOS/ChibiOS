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

#include <ch.hpp>
#include <evtimer.h>

#include <lpc214x.h>
#include <lpc214x_serial.h>

using namespace chibios_rt;

/*
 * LED blinking sequences.
 */
#define SLEEP           0
#define STOP            1
#define BITCLEAR        2
#define BITSET          3

typedef struct {
  uint8_t       action;
  uint32_t      value;
} bitop_t;

bitop_t LED1_sequence[] =
{
  {BITCLEAR, 0x00000400},
  {SLEEP,    200},
  {BITSET,   0x00000400},
  {SLEEP,    1800}
};

bitop_t LED2_sequence[] =
{
  {SLEEP,    1000},
  {BITCLEAR, 0x00000800},
  {SLEEP,    200},
  {BITSET,   0x00000800},
  {SLEEP,    800}
};

bitop_t LED3_sequence[] =
{
  {BITCLEAR, 0x80000000},
  {SLEEP,    200},
  {BITSET,   0x80000000},
  {SLEEP,    300}
};

/**
 * LED blinker thread class.
 */
class BlinkerThread : chibios_rt::BaseThread {
private:
  WorkingArea(wa, 64);
  bitop_t *base, *curr, *top;

protected:
  virtual msg_t Main(void) {

    while (TRUE) {
      switch(curr->action) {
      case SLEEP:
        Sleep(curr->value);
        break;
      case STOP:
        return 0;
      case BITCLEAR:
        IO0CLR = curr->value;
        break;
      case BITSET:
        IO0SET = curr->value;
        break;
      }
      if (++curr >= top)
        curr = base;
    }
  }

public:
  BlinkerThread(bitop_t *sequence, int n) : BaseThread(NORMALPRIO, 0, wa, sizeof wa) {

    base = curr = sequence;
    top = sequence + n;
  }
};

extern "C" {
  msg_t TestThread(void *p);
}

/*
 * Executed as event handler at 500mS intervals.
 */
static void TimerHandler(eventid_t id) {

  if (!(IO0PIN & 0x00018000))   // Both buttons
    TestThread(&COM1);
}

/*
 * Entry point, the interrupts are disabled on entry.
 */
int main(int argc, char **argv) {
  static const evhandler_t evhndl[] = {
    TimerHandler
  };
  static EvTimer evt;
  struct EventListener el0;

  System::Init();

  evtInit(&evt, 500);                   /* Initializes an event timer object.   */
  evtStart(&evt);                       /* Starts the event timer.              */
  chEvtRegister(&evt.et_es, &el0, 0);   /* Registers on the timer event source. */

  BlinkerThread blinker1(LED1_sequence, sizeof(LED1_sequence) / sizeof(bitop_t));
  BlinkerThread blinker2(LED2_sequence, sizeof(LED2_sequence) / sizeof(bitop_t));
  BlinkerThread blinker3(LED3_sequence, sizeof(LED3_sequence) / sizeof(bitop_t));

  while(1)
    Event::Wait(ALL_EVENTS, evhndl);

  return 0;
}
