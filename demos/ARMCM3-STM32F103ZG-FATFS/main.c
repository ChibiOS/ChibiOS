/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
#include "test.h"

/*===========================================================================*/
/* Card insertion monitor.                                                   */
/*===========================================================================*/

#define SDC_POLLING_INTERVAL            10
#define SDC_POLLING_DELAY               10

/**
 * @brief   Card monitor timer.
 */
static VirtualTimer tmr;

/**
 * @brief   Debounce counter.
 */
static unsigned cnt;

/**
 * @brief   Card event sources.
 */
static EventSource inserted_event, removed_event;

/**
 * @brief   Inserion monitor function.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
bool_t sdc_lld_is_card_inserted(SDCDriver *sdcp) {

  return !palReadPad(GPIOF, GPIOF_SD_DETECT);
}

/**
 * @brief   Protection detection.
 * @note    Not supported.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
bool_t sdc_lld_is_write_protected(SDCDriver *sdcp) {

  return FALSE;
}

/**
 * @brief   Inserion monitor timer callback function.
 *
 * @param[in] p         pointer to the @p SDCDriver object
 *
 * @notapi
 */
static void tmrfunc(void *p) {
  SDCDriver *sdcp = p;

  if (cnt > 0) {
    if (sdcIsCardInserted(sdcp)) {
      if (--cnt == 0) {
        chEvtBroadcastI(&inserted_event);
      }
    }
    else
      cnt = SDC_POLLING_INTERVAL;
  }
  else {
    if (!sdcIsCardInserted(sdcp)) {
      cnt = SDC_POLLING_INTERVAL;
      chEvtBroadcastI(&removed_event);
    }
  }
  chVTSetI(&tmr, MS2ST(SDC_POLLING_DELAY), tmrfunc, sdcp);
}

/**
 * @brief   Polling monitor start.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
static void tmr_init(SDCDriver *sdcp) {

  chEvtInit(&inserted_event);
  chEvtInit(&removed_event);
  chSysLock();
  cnt = SDC_POLLING_INTERVAL;
  chVTSetI(&tmr, MS2ST(SDC_POLLING_DELAY), tmrfunc, sdcp);
  chSysUnlock();
}

/*===========================================================================*/
/* Main and generic code.                                                    */
/*===========================================================================*/

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  while (TRUE) {
    palClearPad(GPIOF, GPIOF_LED4);
    palSetPad(GPIOF, GPIOF_LED1);
    chThdSleepMilliseconds(250);
    palClearPad(GPIOF, GPIOF_LED1);
    palSetPad(GPIOF, GPIOF_LED2);
    chThdSleepMilliseconds(250);
    palClearPad(GPIOF, GPIOF_LED2);
    palSetPad(GPIOF, GPIOF_LED3);
    chThdSleepMilliseconds(250);
    palClearPad(GPIOF, GPIOF_LED3);
    palSetPad(GPIOF, GPIOF_LED4);
    chThdSleepMilliseconds(250);
  }
}

/*
 * Application entry point.
 */
int main(void) {

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
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  /*
   * Activates the card insertion monitor.
   */
  tmr_init(&SDCD1);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (TRUE) {
    if (!palReadPad(GPIOG, GPIOG_USER_BUTTON))
      TestThread(&SD1);
    chThdSleepMilliseconds(500);
  }
}
