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
 * @file    tsioblksstub.c
 * @brief   IOBlks stub for trusted services implementing the
 *          Reliance Edge api.
 *
 */

#include "ch.h"
#include "chobjfifos.h"
#include "chtssi.h"
#include "tsproxystubs.h"
#include "tsioblksstub.h"
#include <redfs.h>
#include <redvolume.h>
#include <string.h>
#include <ctype.h>

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
static stub_ctx_t stub_ctx;
const VOLCONF gaRedVolConf[REDCONF_VOLUME_COUNT] = {
    {
        512,              /* ulSectorSize.*/
        IOBLKS_PART_SIZE, /* ullSectorCount.*/
        0U,               /* ullSectorOffset.*/
        false,            /* fAtomicSectorWrite.*/
        1024U,            /* ulInodeCount.*/
        2U,               /* bBlockIoRetries.*/
        ""
    }
};

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief     The IOBlks stub service.
 */
THD_WORKING_AREA(waTsIOBlksStubsService, 1024);
THD_FUNCTION(TsIOBlksStubsService, tsstatep) {
  stub_ctx.event_flag = EVT_F_IOBLK_NEW_OP;
  TsStubService((ts_state_t *)tsstatep, &stub_ctx);
}

/**
 * @name    The IOBlks api, in the Reliance Edge fashion.
 * @{
 */

/**
 * @brief   Open the block device.
 */
REDSTATUS RedOsBDevOpen(uint8_t bVolNum, BDEVOPENMODE mode) {
  (void)mode;

  if (bVolNum != 0)
    return -RED_EINVAL;
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = IOBLKS_OP_OPEN;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)bVolNum;
  op->op_p[1].dir = OP_PRMDIR_NONE;
  op->op_p[1].val = (uint32_t)mode;
  return (REDSTATUS)callRemote(op);
}

/**
 * @brief   Close the block device.
 */
REDSTATUS RedOsBDevClose(uint8_t bVolNum) {
  if (bVolNum != 0)
    return -RED_EINVAL;
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = IOBLKS_OP_CLOSE;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)bVolNum;
  return (REDSTATUS)callRemote(op);
}

/**
 * @brief   Read from the block device.
 */
REDSTATUS RedOsBDevRead(uint8_t bVolNum, uint64_t ullSectorStart, uint32_t ulSectorCount, void *pBuffer) {
  if ((bVolNum != 0) ||
      (ullSectorStart >= gaRedVolConf[bVolNum].ullSectorCount) ||
      ((gaRedVolConf[bVolNum].ullSectorCount - ullSectorStart) < ulSectorCount) ||
      (pBuffer == NULL))
    return -RED_EINVAL;
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = IOBLKS_OP_READ;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)bVolNum;
  op->op_p[1].dir = OP_PRMDIR_NONE;
  op->op_p[1].val = (uint32_t)ullSectorStart;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)ulSectorCount;
  op->op_p[3].dir = OP_PRMDIR_OUT;
  op->op_p[3].val = (uint32_t)pBuffer;
  op->op_p[3].size = (uint32_t)(ulSectorCount * IOBLKS_SECT_SIZE);
  return (int)callRemote(op);
}

/**
 * @brief   Write to the block device.
 */
REDSTATUS RedOsBDevWrite(uint8_t bVolNum, uint64_t ullSectorStart, uint32_t ulSectorCount, const void *pBuffer) {
  if ((bVolNum != 0) ||
      (ullSectorStart >= gaRedVolConf[bVolNum].ullSectorCount) ||
      ((gaRedVolConf[bVolNum].ullSectorCount - ullSectorStart) < ulSectorCount) ||
      (pBuffer == NULL))
    return -RED_EINVAL;
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = IOBLKS_OP_WRITE;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)bVolNum;
  op->op_p[1].dir = OP_PRMDIR_NONE;
  op->op_p[1].val = (uint32_t)ullSectorStart;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)ulSectorCount;
  op->op_p[3].dir = OP_PRMDIR_IN;
  op->op_p[3].val = (uint32_t)pBuffer;
  op->op_p[3].size = (uint32_t)(ulSectorCount * IOBLKS_SECT_SIZE);
  return (int)callRemote(op);
}

/**
 * @brief   Flush the block device.
 */
REDSTATUS RedOsBDevFlush(uint8_t bVolNum) {
  if (bVolNum != 0)
    return -RED_EINVAL;
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = IOBLKS_OP_FLUSH;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)bVolNum;
  return (REDSTATUS)callRemote(op);
}

/** @} */

