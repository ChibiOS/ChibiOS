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

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Test related code.                                                        */
/*===========================================================================*/

#define NUM_THREADS     4
#define MAILBOX_SIZE    2

#define MSG_SEND_LEFT   0
#define MSG_SEND_RIGHT  1

static bool_t saturated;

/*
 * Mailboxes and buffers.
 */
static Mailbox mb[NUM_THREADS];
static msg_t b[NUM_THREADS][MAILBOX_SIZE];

/*
 * Test worker threads.
 */
static WORKING_AREA(waWorkerThread[NUM_THREADS], 128);
static msg_t WorkerThread(void *arg) {
  static volatile uint32_t x = 0;
  static uint32_t cnt = 0;
  uint32_t me = (uint32_t)arg;
  uint32_t target;
  uint32_t i;
  msg_t msg;

  /* Work loop.*/
  while (TRUE) {
    /* Waiting for a message.*/
   chMBFetch(&mb[me], &msg, TIME_INFINITE);

    /* Pseudo-random delay.*/
    for (i = 0; i < (me >> 4); i++)
      x++;

    /* Deciding in which direction to re-send the message.*/
    if (msg == MSG_SEND_LEFT)
      target = me - 1;
    else
      target = me + 1;

    if (target < NUM_THREADS) {
      /* If this thread is not at the end of a chain re-sending the message,
         note this check works because the variable target is unsigned.*/
      msg = chMBPost(&mb[target], msg, TIME_IMMEDIATE);
      if (msg != RDY_OK)
        saturated = TRUE;
    }
    else {
      /* Provides a visual feedback about the system.*/
      if (++cnt >= 500) {
        cnt = 0;
        palTogglePad(GPIOC, GPIOC_LED);
      }
    }
  }
}

/*
 * GPT1 callback.
 */
static void gpt1cb(GPTDriver *gptp) {
  msg_t msg;

  (void)gptp;
  chSysLockFromIsr();
  msg = chMBPostI(&mb[0], MSG_SEND_RIGHT);
  if (msg != RDY_OK)
    saturated = TRUE;
  chSysUnlockFromIsr();
}

/*
 * GPT2 callback.
 */
static void gpt2cb(GPTDriver *gptp) {
  msg_t msg;

  (void)gptp;
  chSysLockFromIsr();
  msg = chMBPostI(&mb[NUM_THREADS - 1], MSG_SEND_LEFT);
  if (msg != RDY_OK)
    saturated = TRUE;
  chSysUnlockFromIsr();
}

/*
 * GPT1 configuration.
 */
static const GPTConfig gpt1cfg = {
  1000000,  /* 1MHz timer clock.*/
  gpt1cb    /* Timer callback.*/
};

/*
 * GPT2 configuration.
 */
static const GPTConfig gpt2cfg = {
  1000000,  /* 1MHz timer clock.*/
  gpt2cb    /* Timer callback.*/
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
  gptcnt_t interval;
  gptcnt_t threshold;

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
   * Prepares the Serial driver 2 and GPT drivers 1 and 2.
   */
  sdStart(&SD2, NULL);          /* Default is 38400-8-N-1.*/
  gptStart(&GPTD1, &gpt1cfg);
  gptStart(&GPTD2, &gpt2cfg);

  /*
   * Initializes the mailboxes and creates the worker threads.
   */
  for (i = 0; i < NUM_THREADS; i++) {
    chMBInit(&mb[i], b[i], MAILBOX_SIZE);
    chThdCreateStatic(waWorkerThread[i], sizeof waWorkerThread[i],
                      NORMALPRIO - 20, WorkerThread, (void *)i);
  }

  /*
   * Test procedure.
   */
  println("");
  println("*** ChibiOS/RT IRQ-STORM long duration test");
  println("***");
  print("*** Kernel:       ");
 println(CH_KERNEL_VERSION);
#ifdef __GNUC__
  print("*** GCC Version:  ");
  println(__VERSION__);
#endif
  print("*** Architecture: ");
  println(CH_ARCHITECTURE_NAME);
#ifdef CH_CORE_VARIANT_NAME
  print("*** Core Variant: ");
  println(CH_CORE_VARIANT_NAME);
#endif
#ifdef PLATFORM_NAME
  print("*** Platform:     ");
  println(PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  print("*** Test Board:   ");
  println(BOARD_NAME);
#endif
  print("*** SYSCLK:       ");
  printn(STM32_SYSCLK);
  println("");

  println("");
  for (i = 1; i <= 100; i++){
    print("Iteration ");
    printn(i);
    println("");
    saturated = FALSE;
    threshold = 0;
    for (interval = 2000; interval >= 20; interval -= interval / 10) {
      gptStartContinuous(&GPTD1, interval - 1); /* Slightly out of phase.*/
      gptStartContinuous(&GPTD2, interval + 1); /* Slightly out of phase.*/
      chThdSleepMilliseconds(1000);
      gptStopTimer(&GPTD1);
      gptStopTimer(&GPTD2);
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
    print("\r\nSaturated at ");
    printn(threshold);
    print(" uS\r\n\n");
  }
  gptStopTimer(&GPTD1);
  gptStopTimer(&GPTD2);

  println("\r\nTest Complete");

  /*
   * Normal main() thread activity, nothing in this test.
   */
  while (TRUE) {
    chThdSleepMilliseconds(5000);
  }
  return 0;
}
