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
 * @file    tsioblksskel.c
 * @brief   IOBlocks skeleton daemon for trusted clients.
 *
 */

#include "ch.h"
#include "hal.h"
#include "chobjfifos.h"
#include "tsclient.h"
#include "tsdaemonskels.h"
#include "tsioblksskel.h"
#include "dummyredconf.h"
#include "rederrno.h"
#include "redostypes.h"
#include "redosserv.h"
#include "sama_sdmmc_lld.h"
#include "ch_sdmmc_device.h"
#include "ch_sdmmc_cmds.h"
#include "ch_sdmmc_sdio.h"
#include "ch_sdmmc_sd.h"
#include "ch_sdmmc_mmc.h"
#include "ch_sdmmc_reledge.h"
#include <string.h>

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
static skel_ctx_t skel_ctx;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @name    IOBlocks API skeletons, in reliance edge fashion.
 * @{
 */

/**
 * @brief int red_open(uint8_t bVolNum, int mode)
 */
static void red_open(skel_req_t *skreqp) {
  REDSTATUS     result = 0;
  uint32_t      ulTries;
  eSDMMC_RC     cs;
  SdmmcDriver  *sdmmcp = NULL;
  uint8_t       bVolNum;
  int           mode;

  bVolNum = (uint8_t)skreqp->stub_op_p[0];
  mode = (int)skreqp->stub_op_p[1];
  if (!sdmmcGetInstance(bVolNum, &sdmmcp))
    result = -RED_EINVAL;
  else {
    for (ulTries = 0U; ulTries < 20U; ulTries++) {
      cs = sd_mmc_test_unit_ready(sdmmcp);
      if ((cs == SDMMC_OK) && (cs != SDMMC_BUSY)){
          break;
      }
      chThdSleepMilliseconds(5);
    }

    if (cs == SDMMC_OK) {
      if (mode != BDEV_O_RDONLY)
        if (sd_mmc_is_write_protected(sdmmcp))
          result = -RED_EROFS;
    } else
      result = -RED_EIO;
  }

  /* report the result and copy the 'out' parameters.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief int red_close(uint8_t bVolNum)
 */
static void red_close(skel_req_t *skreqp) {
  int result;

  result = 0;

  /* report the result.*/
  returnToRemote(skreqp, result);
}


/**
 * @brief int red_read(uint8_t bVolNum, uint32_t ulSectorStart, uint32_t ulSectorCount, void *pBuffer)
 */
static void red_read(skel_req_t *skreqp) {
  REDSTATUS result = 0;
  uint8_t   bVolNum;
  uint32_t  ulSectorStart;
  uint32_t  ulSectorCount;
  void *mem;
  size_t len;

  bVolNum = (uint8_t)skreqp->stub_op_p[0];
  ulSectorStart = (uint32_t)skreqp->stub_op_p[1] + IOBLKS_PART_OFFS;
  ulSectorCount = (uint32_t)skreqp->stub_op_p[2];
  len = ulSectorCount * IOBLKS_SECT_SIZE;

  /* Allocate the space for the receive buffer.*/
  mem = chHeapAlloc(NULL, len);
  if (NULL == mem) {
    result = -RED_ENOMEM;
    len = 0;
  } else {
    SdmmcDriver *sdmmcp = NULL;
    eSDMMC_RC cs;

    if (!sdmmcGetInstance(bVolNum, &sdmmcp)) {
      result = -RED_EINVAL;
      len = 0;
    } else {
      cs = SD_ReadBlocks(sdmmcp, ulSectorStart, mem, ulSectorCount);
      if(cs != SDMMC_OK) {
        result = -RED_EIO;
        len = 0;
      }
    }
  }
  skreqp->stub_op_p_sz[3] = len;
  skreqp->stub_op_p[3] = (uint32_t)mem;

  /* report the result and copy 'out' parameter mem.*/
  returnToRemote(skreqp, result);
  if (NULL != mem)
    chHeapFree(mem);
}

/**
 * @brief int red_write(uint8_t bVolNum, uint32_t ulSectorStart, uint32_t ulSectorCount, const void *pBuffer)
 */
static void red_write(skel_req_t *skreqp) {
  REDSTATUS result = 0;
  uint8_t   bVolNum;
  uint32_t  ulSectorStart;
  uint32_t  ulSectorCount;
  void     *dataptr;
  size_t    size;

  bVolNum = (uint8_t)skreqp->stub_op_p[0];
  ulSectorStart = (uint32_t)skreqp->stub_op_p[1] + IOBLKS_PART_OFFS;
  ulSectorCount = (uint32_t)skreqp->stub_op_p[2];
  size = ulSectorCount * IOBLKS_SECT_SIZE;

  /* Allocate the space for the send buffer.*/
  dataptr = chHeapAlloc(NULL, size);
  if (NULL == dataptr) {
    result = -RED_ENOMEM;
  } else {
    SdmmcDriver *sdmmcp = NULL;
    eSDMMC_RC cs;

    skreqp->stub_op_p[3] = (uint32_t)dataptr;

    /* call the stub service in order to copy the
       'in' parameter dataptr.*/
    paramsInFromRemote(skreqp);

    if (!sdmmcGetInstance(bVolNum, &sdmmcp)) {
      result = -RED_EINVAL;
    } else {
      cs = SD_WriteBlocks(sdmmcp, ulSectorStart, dataptr, ulSectorCount);
      if (cs != SDMMC_OK)
        result = -RED_EIO;
    }

    chHeapFree(dataptr);
  }

  /* report the result.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief red_flush(uint8_t bVolNum);
 */
static void red_flush(skel_req_t *skreqp) {
  REDSTATUS     result = 0;
  uint8_t       bVolNum;
  eSDMMC_RC     cs;
  SdmmcDriver  *sdmmcp = NULL;

  bVolNum = (uint8_t)skreqp->stub_op_p[0];
  if (!sdmmcGetInstance(bVolNum, &sdmmcp))
    result = -RED_EINVAL;
  else {
    cs = sd_mmc_test_unit_ready(sdmmcp);
    if(cs != SDMMC_OK)
      result = -RED_EIO;
  }

  /* Report the result.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief   IOBlocks Daemon. Dispatch a request to the corresponding
 *          local method.
 */
static THD_FUNCTION(TsIOBlksSkelDaemon, arg) {
  objects_fifo_t *ofp = arg;
  skel_req_t *skreqp;

  for (;/* ever */;) {
    chFifoReceiveObjectTimeout(ofp, (void **)&skreqp,
        TIME_INFINITE);
    switch (skreqp->stub_op_code) {
    case IOBLKS_OP_OPEN:
      red_open(skreqp);
      break;
    case IOBLKS_OP_CLOSE:
      red_close(skreqp);
      break;
    case IOBLKS_OP_READ:
      red_read(skreqp);
      break;
    case IOBLKS_OP_WRITE:
      red_write(skreqp);
      break;
    case IOBLKS_OP_FLUSH:
      red_flush(skreqp);
      break;
    default:
      break;
    }
  }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Init the IOBlocks skeleton daemon objects and create the
 *          corresponding threads.
 */
void tsIOBlksSkelInit(void) {
  int i;
  skel_ctx_t *scp;

  scp = &skel_ctx;
  chFifoObjectInit(&scp->skel_req_fifo, sizeof (skel_req_t), N_MAX_SKEL_REQS,
      sizeof (uint8_t), scp->skel_reqs, scp->skel_req_msgs);
  scp->skel_eventflag = EVT_F_IOBLK_NEW_OP;
  scp->stub_svc_name = IOBLKS_SVC_NAME;

  for (i = 0; i < N_IOBLKSKEL_THD; ++i)
    chThdCreateFromHeap(NULL, 2048, "TsIOBlksSkelDaemonWrk", NORMALPRIO,
        TsIOBlksSkelDaemon, &scp->skel_req_fifo);
  chThdCreateFromHeap(NULL, 2048, "TsIOBlksSkelDaemon", NORMALPRIO,
      TsSkelsDaemon, scp);
}

/** @} */
