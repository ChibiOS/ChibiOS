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
#include "tservices.h"
#include "proxies/tssockstub.h"
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

  BaseSequentialStream *ssp = (BaseSequentialStream*)&SD1;
  ts_state_t *svcp = tsstate;

  /* Start the 'wait request / process / response' cycle.*/
  for (;/* ever */;) {
    int i;

    /* Wait a service request.*/
    msg_t r = tssiWaitRequest(tsstate);

    /* Check if status is ko. It could not happen.*/
    if (r != SMC_SVC_OK) {
      chprintf(ssp, "Unexpected wait request error.\r\n");
      continue;
    }

    /* Process the request.*/
    chprintf(ssp, "r = %d, TsSimpleService received a new request.\r\n", r);
    if (svcp->ts_datalen > 0) {
      *(TS_GET_DATA(svcp) + TS_GET_DATALEN(svcp) - 1) = '\0';
      chprintf(ssp, "My non secure 'alter ego' has a request.\r\n");
      chprintf(ssp, "She tells: '");
      chprintf(ssp, TS_GET_DATA(svcp));
      chprintf(ssp, "'\r\n");
    }
    for (i = 0; i < 100000; ++i)
      ;

    /* Set the response.*/
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
  TS_CONF_TABLE_ENTRY("TsStubsService", waTsStubsService, TS_BASE_PRIO+1, TsStubsService, TS_STATE(1))
TS_CONF_TABLE_END

/** @} */
