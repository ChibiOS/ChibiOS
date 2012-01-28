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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <stdlib.h>

#include "ch.h"
#include "hal.h"

float ff1(float par);

float ff2(float par1, float par2, float par3, float par4);

/*===========================================================================*/
/* Configurable settings.                                                    */
/*===========================================================================*/

#ifndef RANDOMIZE
#define RANDOMIZE       FALSE
#endif

#ifndef ITERATIONS
#define ITERATIONS      100
#endif

/*===========================================================================*/
/* Test related code.                                                        */
/*===========================================================================*/

static bool_t saturated;

/*
 * Test worker thread.
 */
static WORKING_AREA(waWorkerThread, 128);
static msg_t WorkerThread(void *arg) {

  (void)arg;

  while(1) {
    float f1, f2, f3, f4, f5;

    f1 = ff1(3.0f);
    f2 = ff1(4.0f);
    f3 = ff1(5.0f);
    f5 = f1 + f2 + f3;
    f4 = ff1(6.0f);
    f5 = ff2(f5, f4, f5, f4);
    if (f5 != 324.0f)
      chSysHalt();
  }
}

/*
 * Test periodic thread.
 */
static WORKING_AREA(waPeriodicThread, 128);
static msg_t PeriodicThread(void *arg) {

  (void)arg;

  while(1) {
    float f1, f2, f3, f4, f5;

    f1 = ff1(4.0f);
    f2 = ff1(5.0f);
    f3 = ff1(6.0f);
    f5 = f1 + f2 + f3;
    f4 = ff1(7.0f);
    f5 = ff2(f5, f4, f5, f4);
    if (f5 != 484.0f)
      chSysHalt();
    chThdSleepSeconds(1);
  }
}

/*
 * GPT2 callback.
 */
static void gpt2cb(GPTDriver *gptp) {
  float f1, f2, f3, f4, f5;

  (void)gptp;

  f1 = ff1(2.0f);
  f2 = ff1(3.0f);
  f3 = ff1(4.0f);
  f5 = f1 + f2 + f3;
  f4 = ff1(5.0f);
  f5 = ff2(f5, f4, f5, f4);
  if (f5 != 196.0f)
    chSysHalt();
}

/*
 * GPT3 callback.
 */
static void gpt3cb(GPTDriver *gptp) {
  float f1, f2, f3, f4, f5;

  (void)gptp;

  f1 = ff1(1.0f);
  f2 = ff1(2.0f);
  f3 = ff1(3.0f);
  f5 = f1 + f2 + f3;
  f4 = ff1(4.0f);
  f5 = ff2(f5, f4, f5, f4);
  if (f5 != 100.0f)
    chSysHalt();
}

/*
 * GPT2 configuration.
 */
static const GPTConfig gpt2cfg = {
  1000000,  /* 1MHz timer clock.*/
  gpt2cb    /* Timer callback.*/
};

/*
 * GPT3 configuration.
 */
static const GPTConfig gpt3cfg = {
  1000000,  /* 1MHz timer clock.*/
  gpt3cb    /* Timer callback.*/
};


/*===========================================================================*/
/* Generic demo code.                                                        */
/*===========================================================================*/

static void print(char *p) {

  while (*p) {
    chIOPut(&SD2, *p++);
  }
}

static void println(char *p) {

  while (*p) {
    chIOPut(&SD2, *p++);
  }
  chIOWriteTimeout(&SD2, (uint8_t *)"\r\n", 2, TIME_INFINITE);
}

static void printn(uint32_t n) {
  char buf[16], *p;

  if (!n)
    chIOPut(&SD2, '0');
  else {
    p = buf;
    while (n)
      *p++ = (n % 10) + '0', n /= 10;
    while (p > buf)
      chIOPut(&SD2, *--p);
  }
}

/*
 * Application entry point.
 */
int main(void) {
  unsigned i;
  gptcnt_t interval, threshold, worst;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Prepares the Serial driver 2 and GPT drivers 2 and 3.
   */
  sdStart(&SD2, NULL);          /* Default is 38400-8-N-1.*/
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
  gptStart(&GPTD2, &gpt2cfg);
  gptStart(&GPTD3, &gpt3cfg);

  /*
   * Initializes the worker threads.
   */
  chThdCreateStatic(waWorkerThread, sizeof waWorkerThread,
                    NORMALPRIO - 20, WorkerThread, NULL);
  chThdCreateStatic(waPeriodicThread, sizeof waPeriodicThread,
                    NORMALPRIO - 10, PeriodicThread, NULL);

  /*
   * Test procedure.
   */
  println("");
  println("*** ChibiOS/RT IRQ-STORM-FPU long duration test");
  println("***");
  print("*** Kernel:       ");
  println(CH_KERNEL_VERSION);
  print("*** Compiled:     ");
  println(__DATE__ " - " __TIME__);
#ifdef CH_COMPILER_NAME
  print("*** Compiler:     ");
  println(CH_COMPILER_NAME);
#endif
  print("*** Architecture: ");
  println(CH_ARCHITECTURE_NAME);
#ifdef CH_CORE_VARIANT_NAME
  print("*** Core Variant: ");
  println(CH_CORE_VARIANT_NAME);
#endif
#ifdef CH_PORT_INFO
  print("*** Port Info:    ");
  println(CH_PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  print("*** Platform:     ");
  println(PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  print("*** Test Board:   ");
  println(BOARD_NAME);
#endif
  println("***");
  print("*** System Clock: ");
  printn(STM32_SYSCLK);
  println("");
  print("*** Iterations:   ");
  printn(ITERATIONS);
  println("");
  print("*** Randomize:    ");
  printn(RANDOMIZE);
  println("");

  println("");
  worst = 0;
  for (i = 1; i <= ITERATIONS; i++){
    print("Iteration ");
    printn(i);
    println("");
    saturated = FALSE;
    threshold = 0;
    for (interval = 2000; interval >= 10; interval -= interval / 10) {
      gptStartContinuous(&GPTD2, interval - 1); /* Slightly out of phase.*/
      gptStartContinuous(&GPTD3, interval + 1); /* Slightly out of phase.*/
      chThdSleepMilliseconds(1000);
      gptStopTimer(&GPTD2);
      gptStopTimer(&GPTD3);
      if (!saturated)
        print(".");
      else {
        print("#");
        if (threshold == 0)
          threshold = interval;
      }
    }
    /* Gives the worker threads a chance to empty the mailboxes before next
       cycle.*/
    chThdSleepMilliseconds(20);
    println("");
    print("Saturated at ");
    printn(threshold);
    println(" uS");
    println("");
    if (threshold > worst)
      worst = threshold;
  }
  gptStopTimer(&GPTD2);
  gptStopTimer(&GPTD3);

  print("Worst case at ");
  printn(worst);
  println(" uS");
  println("");
  println("Test Complete");

  /*
   * Normal main() thread activity, nothing in this test.
   */
  while (TRUE) {
    chThdSleepMilliseconds(5000);
  }
}
