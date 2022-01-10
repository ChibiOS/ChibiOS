/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "vfs.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

event_source_t sdmon_inserted_event, sdmon_removed_event;
bool sdmon_ready;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static virtual_timer_t tmr;
static unsigned cnt;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Insertion monitor timer callback function.
 *
 * @param[in] p         pointer to the @p BaseBlockDevice object
 *
 * @notapi
 */
static void tmrfunc(virtual_timer_t *vtp, void *p) {
  BaseBlockDevice *bbdp = p;

  chSysLockFromISR();

  if (cnt > 0) {
    if (blkIsInserted(bbdp)) {
      if (--cnt == 0) {
        chEvtBroadcastI(&sdmon_inserted_event);
      }
    }
    else
      cnt = POLLING_INTERVAL;
  }
  else {
    if (!blkIsInserted(bbdp)) {
      cnt = POLLING_INTERVAL;
      chEvtBroadcastI(&sdmon_removed_event);
    }
  }
  chVTSetI(vtp, TIME_MS2I(POLLING_DELAY), tmrfunc, bbdp);

  chSysUnlockFromISR();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Polling monitor start.
 *
 * @notapi
 */
void sdmonInit(void) {
  static event_listener_t el0, el1;

  /* Activates the  SDC driver using default configuration.*/
  sdcStart(&SDCD1, NULL);

  chEvtObjectInit(&sdmon_inserted_event);
  chEvtObjectInit(&sdmon_removed_event);

  chEvtRegister(&sdmon_inserted_event, &el0, (eventid_t)0);
  chEvtRegister(&sdmon_removed_event, &el1, (eventid_t)1);

  sdmon_ready = false;
  cnt = POLLING_INTERVAL;
  chVTSet(&tmr, TIME_MS2I(POLLING_DELAY), tmrfunc, (void *)&SDCD1);
}

/*
 * Card insertion event.
 */
void sdmonInsertHandler(eventid_t id) {
  msg_t err;

  (void)id;

#if HAL_USE_SDC
  if (sdcConnect(&SDCD1)) {
    return;
  }
#else
  if (mmcConnect(&MMCD1)) {
    return;
  }
#endif

  err = drvFatFSMount("0:", 1);
  if (CH_RET_IS_ERROR(err)) {
#if HAL_USE_SDC
    sdcDisconnect(&SDCD1);
#else
  if (mmcDisconnect(&MMCD1)) {
#endif
   return;
  }
  sdmon_ready = true;
}

/*
 * Card removal event.
 */
void sdmonRemoveHandler(eventid_t id) {

  (void)id;

#if HAL_USE_SDC
  sdcDisconnect(&SDCD1);
#else
  mmcDisconnect(&MMCD1);
#endif
  sdmon_ready = false;
}
