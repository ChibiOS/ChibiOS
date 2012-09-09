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

#include "ch.hpp"
#include "hal.h"
#include "test.h"
#include "evtimer.h"

using namespace chibios_rt;

/*
 * LED blink sequences.
 * NOTE: Sequences must always be terminated by a GOTO instruction.
 * NOTE: The sequencer language could be easily improved but this is outside
 *       the scope of this demo.
 */
#define SLEEP           0
#define GOTO            1
#define STOP            2
#define BITCLEAR        3
#define BITSET          4

typedef struct {
  uint8_t       action;
  uint32_t      value;
} seqop_t;

// Flashing sequence for LED1.
static const seqop_t LED1_sequence[] =
{
  {BITCLEAR, PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    200},
  {BITSET,   PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    800},
  {BITCLEAR, PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    400},
  {BITSET,   PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    600},
  {BITCLEAR, PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    600},
  {BITSET,   PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    400},
  {BITCLEAR, PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    800},
  {BITSET,   PAL_PORT_BIT(GPIOC_LED)},
  {SLEEP,    200},
  {GOTO,     0}
};

/*
 * Sequencer thread class. It can drive LEDs or other output pins.
 * Any sequencer is just an instance of this class, all the details are
 * totally encapsulated and hidden to the application level.
 */
class SequencerThread : public EnhancedThread<128> {
private:
  const seqop_t *base, *curr;                   // Thread local variables.

protected:
  virtual msg_t Main(void) {
    while (true) {
      switch(curr->action) {
      case SLEEP:
        Sleep(curr->value);
        break;
      case GOTO:
        curr = &base[curr->value];
        continue;
      case STOP:
        return 0;
      case BITCLEAR:
        palClearPort(GPIOC, curr->value);
        break;
      case BITSET:
        palSetPort(GPIOC, curr->value);
        break;
      }
      curr++;
    }
  }

public:
  SequencerThread(const seqop_t *sequence) : EnhancedThread<128>("sequencer") {

    base = curr = sequence;
  }
};

/*
 * Tester thread class. This thread executes the test suite.
 */
class TesterThread : public EnhancedThread<256> {

protected:
  virtual msg_t Main(void) {

    return TestThread(&SD2);
  }

public:
  TesterThread(void) : EnhancedThread<256>("tester") {
  }
};

/*
 * Executed as an event handler at 500mS intervals.
 */
static void TimerHandler(eventid_t id) {

  (void)id;
  if (palReadPad(GPIOA, GPIOA_BUTTON)) {
    TesterThread tester;
    tester.Wait();
  };
}

/*
 * Application entry point.
 */
int main(void) {
  static const evhandler_t evhndl[] = {
    TimerHandler
  };
  static EvTimer evt;
  struct EventListener el0;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::Init();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  evtInit(&evt, 500);                   // Initializes an event timer.
  evtStart(&evt);                       // Starts the event timer.
  chEvtRegister(&evt.et_es, &el0, 0);   // Registers a listener on the source.

  /*
   * Starts several instances of the SequencerThread class, each one operating
   * on a different LED.
   */
  SequencerThread blinker1(LED1_sequence);

  /*
   * Serves timer events.
   */
  while (true)
    Event::Dispatch(evhndl, Event::WaitOne(ALL_EVENTS));

  return 0;
}
