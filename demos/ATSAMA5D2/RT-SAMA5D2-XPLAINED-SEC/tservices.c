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

/**
 * @file    tservices.c
 * @brief   Trusted services application file.
 *
 * @addtogroup TSSI
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "chtssi.h"
#include "chprintf.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static THD_WORKING_AREA(waTsSimpleService, 1024);
static THD_FUNCTION(TsSimpleService, tsstate) {

  /* WARNING: do not put blocking call out of the cycle,
              i.e. no calls that suspend
              the current thread!.*/

  ts_state_t *svcp = tsstate;

  /* Start the request/process/response cycle.*/
  while (tssiWaitRequest(tsstate) == SMC_SVC_OK) {
    int i;
    chprintf((BaseSequentialStream*)&SD1,
        "TsSimpleService received a new request.\r\n");
    if (svcp->ts_datalen > 0) {
      *(TS_GET_DATA(svcp) + TS_GET_DATALEN(svcp) - 1) = '\0';
      chprintf((BaseSequentialStream*)&SD1,
          "My non secure 'alter ego' has a request.\r\n");
      chprintf((BaseSequentialStream*)&SD1,
          "She tells: '");
      chprintf((BaseSequentialStream*)&SD1, TS_GET_DATA(svcp));
      chprintf((BaseSequentialStream*)&SD1, "'\r\n");
    }
    for (i = 0; i < 100000; ++i)
      ;
    TS_SET_STATUS(svcp, i);
  }

  /* It never goes here.*/
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief     TSSI services table definition
 * @note      This table is filled by the user.
 */
TS_STATE_TABLE
TS_CONF_TABLE_BEGIN
  TS_CONF_TABLE_ENTRY("TsSimpleService", waTsSimpleService, TS_BASE_PRIO, TsSimpleService, TS_STATE(0))
TS_CONF_TABLE_END

/** @} */
