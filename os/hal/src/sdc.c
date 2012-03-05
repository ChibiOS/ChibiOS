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

/**
 * @file    sdc.c
 * @brief   SDC Driver code.
 *
 * @addtogroup SDC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Get slice with data from uint32_t[4] array.
 *
 * @notapi
 */
static uint32_t _sdc_get_slice(uint32_t *data, int8_t end, int8_t start) {
  uint32_t word = 0;
  uint32_t mask = 0;

  chDbgCheck(((start >=0) && (end >=0) && (end >= start)), "sdc_get_slice");

  while ((start - 32 * word) > 31){
    word++;
    data++;
  }

  end   -= 32 * word;
  start -= 32 * word;

  if (end < 31){
    /* Value lays in one word.*/
    mask = (1 << (end - start + 1)) - 1;
    return (*data >> start) & mask;
  }
  else{
    /* Value spread on separate words.*/
    uint32_t lsb, msb;
    lsb = *data >> start;
    data++;
    mask = (1 << (end - 32 + 1)) - 1;
    msb = *data & mask;
    msb = msb << (32 - start);
    return (msb | lsb);
  }
}

/**
 * @brief   Wait for the card to complete pending operations.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval SDC_SUCCESS  operation succeeded.
 * @retval SDC_FAILED   operation failed.
 *
 * @notapi
 */
bool_t _sdc_wait_for_transfer_state(SDCDriver *sdcp) {
  uint32_t resp[1];

  while (TRUE) {
    if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_SEND_STATUS,
                                   sdcp->rca, resp) ||
        SDC_R1_ERROR(resp[0]))
      return SDC_FAILED;
    switch (SDC_R1_STS(resp[0])) {
    case SDC_STS_TRAN:
      return SDC_SUCCESS;
    case SDC_STS_DATA:
    case SDC_STS_RCV:
    case SDC_STS_PRG:
#if SDC_NICE_WAITING
      chThdSleepMilliseconds(1);
#endif
      continue;
    default:
      /* The card should have been initialized so any other state is not
         valid and is reported as an error.*/
      return SDC_FAILED;
    }
  }
  /* If something going too wrong.*/
  return SDC_FAILED;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   SDC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sdcInit(void) {

  sdc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p SDCDriver structure.
 *
 * @param[out] sdcp     pointer to the @p SDCDriver object
 *
 * @init
 */
void sdcObjectInit(SDCDriver *sdcp) {

  sdcp->state  = SDC_STOP;
  sdcp->errors = SDC_NO_ERROR;
  sdcp->config = NULL;
  sdcp->capacity = 0;
}

/**
 * @brief   Configures and activates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] config    pointer to the @p SDCConfig object, can be @p NULL if
 *                      the driver supports a default configuration or
 *                      requires no configuration
 *
 * @api
 */
void sdcStart(SDCDriver *sdcp, const SDCConfig *config) {

  chDbgCheck(sdcp != NULL, "sdcStart");

  chSysLock();
  chDbgAssert((sdcp->state == SDC_STOP) || (sdcp->state == SDC_READY),
              "sdcStart(), #1", "invalid state");
  sdcp->config = config;
  sdc_lld_start(sdcp);
  sdcp->state = SDC_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @api
 */
void sdcStop(SDCDriver *sdcp) {

  chDbgCheck(sdcp != NULL, "sdcStop");

  chSysLock();
  chDbgAssert((sdcp->state == SDC_STOP) || (sdcp->state == SDC_READY),
              "sdcStop(), #1", "invalid state");
  sdc_lld_stop(sdcp);
  sdcp->state = SDC_STOP;
  chSysUnlock();
}

/**
 * @brief   Performs the initialization procedure on the inserted card.
 * @details This function should be invoked when a card is inserted and
 *          brings the driver in the @p SDC_ACTIVE state where it is possible
 *          to perform read and write operations.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval SDC_SUCCESS  operation succeeded.
 * @retval SDC_FAILED   operation failed.
 *
 * @api
 */
bool_t sdcConnect(SDCDriver *sdcp) {
  uint32_t resp[1];

  chDbgCheck(sdcp != NULL, "sdcConnect");

  chSysLock();
  chDbgAssert((sdcp->state == SDC_READY) || (sdcp->state == SDC_ACTIVE),
              "mmcConnect(), #1", "invalid state");
  sdcp->state = SDC_CONNECTING;
  chSysUnlock();

  /* Card clock initialization.*/
  sdc_lld_start_clk(sdcp);

  /* Enforces the initial card state.*/
  sdc_lld_send_cmd_none(sdcp, SDC_CMD_GO_IDLE_STATE, 0);

  /* V2.0 cards detection.*/
  if (!sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_SEND_IF_COND,
                                  SDC_CMD8_PATTERN, resp)) {
    sdcp->cardmode = SDC_MODE_CARDTYPE_SDV20;
    /* Voltage verification.*/
    if (((resp[0] >> 8) & 0xF) != 1)
      goto failed;
    if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_APP_CMD, 0, resp) ||
        SDC_R1_ERROR(resp[0]))
      goto failed;
  }
  else {
#if SDC_MMC_SUPPORT
    /* MMC or SD V1.1 detection.*/
    if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_APP_CMD, 0, resp) ||
        SDC_R1_ERROR(resp[0]))
      sdcp->cardmode = SDC_MODE_CARDTYPE_MMC;
    else
#endif /* SDC_MMC_SUPPORT */
      sdcp->cardmode = SDC_MODE_CARDTYPE_SDV11;
  }

#if SDC_MMC_SUPPORT
    if ((sdcp->cardmode &  SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_MMC) {
    /* TODO: MMC initialization.*/
    return TRUE;
  }
  else
#endif /* SDC_MMC_SUPPORT */
  {
    unsigned i;
    uint32_t ocr;

    /* SD initialization.*/
    if ((sdcp->cardmode &  SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_SDV20)
      ocr = 0xC0100000;
    else
      ocr = 0x80100000;

    /* SD-type initialization. */
    i = 0;
    while (TRUE) {
      if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_APP_CMD, 0, resp) ||
        SDC_R1_ERROR(resp[0]))
        goto failed;
      if (sdc_lld_send_cmd_short(sdcp, SDC_CMD_APP_OP_COND, ocr, resp))
        goto failed;
      if ((resp[0] & 0x80000000) != 0) {
        if (resp[0] & 0x40000000)
          sdcp->cardmode |= SDC_MODE_HIGH_CAPACITY;
        break;
      }
      if (++i >= SDC_INIT_RETRY)
        goto failed;
      chThdSleepMilliseconds(10);
    }
  }

  /* Reads CID.*/
  if (sdc_lld_send_cmd_long_crc(sdcp, SDC_CMD_ALL_SEND_CID, 0, sdcp->cid))
    goto failed;

  /* Asks for the RCA.*/
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_SEND_RELATIVE_ADDR,
                                 0, &sdcp->rca))
    goto failed;

  /* Reads CSD.*/
  if (sdc_lld_send_cmd_long_crc(sdcp, SDC_CMD_SEND_CSD, sdcp->rca, sdcp->csd))
    goto failed;

  /* Switches to high speed.*/
  sdc_lld_set_data_clk(sdcp);

  /* Selects the card for operations.*/
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_SEL_DESEL_CARD,
                                 sdcp->rca, resp))
    goto failed;

  /* Block length fixed at 512 bytes.*/
  if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_SET_BLOCKLEN,
                                 SDC_BLOCK_SIZE, resp) ||
      SDC_R1_ERROR(resp[0]))
    goto failed;

  /* Switches to wide bus mode.*/
  switch (sdcp->cardmode & SDC_MODE_CARDTYPE_MASK) {
  case SDC_MODE_CARDTYPE_SDV11:
  case SDC_MODE_CARDTYPE_SDV20:
    sdc_lld_set_bus_mode(sdcp, SDC_MODE_4BIT);
    if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_APP_CMD, sdcp->rca, resp) ||
        SDC_R1_ERROR(resp[0]))
      goto failed;
    if (sdc_lld_send_cmd_short_crc(sdcp, SDC_CMD_SET_BUS_WIDTH, 2, resp) ||
        SDC_R1_ERROR(resp[0]))
      goto failed;
    break;
  }

  /* Determine capacity.*/
  switch (sdcp->csd[3] >> 30) {
  uint32_t a;
  uint8_t  b, c;
  case 0:
    /* CSD version 1.0 */
    a = _sdc_get_slice(sdcp->csd, SDC_CSD_10_C_SIZE_SLICE);
    b = _sdc_get_slice(sdcp->csd, SDC_CSD_10_C_SIZE_MULT_SLICE);
    c = _sdc_get_slice(sdcp->csd, SDC_CSD_10_READ_BL_LEN_SLICE);
    sdcp->capacity = ((a + 1) << (b + 2) << c) / 512;
    break;
  case 1:
    /* CSD version 2.0 */
    a = _sdc_get_slice(sdcp->csd, SDC_CSD_20_C_SIZE_SLICE);
    sdcp->capacity = 1024 * (a + 1);
    break;
  }
  if (sdcp->capacity == 0)
    goto failed;

  /* Initialization complete.*/
  sdcp->state = SDC_ACTIVE;
  return SDC_SUCCESS;

  /* Initialization failed.*/
failed:
  sdc_lld_stop_clk(sdcp);
  sdcp->state = SDC_READY;
  return SDC_FAILED;
}

/**
 * @brief   Brings the driver in a state safe for card removal.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval SDC_SUCCESS  operation succeeded.
 * @retval SDC_FAILED   operation failed.
 *
 * @api
 */
bool_t sdcDisconnect(SDCDriver *sdcp) {

  chDbgCheck(sdcp != NULL, "sdcDisconnect");

  chSysLock();
  chDbgAssert(sdcp->state == SDC_ACTIVE,
              "sdcDisconnect(), #1", "invalid state");
  if (sdcp->state == SDC_READY) {
    chSysUnlock();
    return SDC_SUCCESS;
  }
  sdcp->state = SDC_DISCONNECTING;
  chSysUnlock();

  /* Waits for eventual pending operations completion.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return SDC_FAILED;

  /* Card clock stopped.*/
  sdc_lld_stop_clk(sdcp);

  sdcp->state = SDC_READY;
  return SDC_SUCCESS;
}

/**
 * @brief   Reads one or more blocks.
 * @pre     The driver must be in the @p SDC_ACTIVE state after a successful
 *          sdcConnect() invocation.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] n         number of blocks to read
 *
 * @return              The operation status.
 * @retval SDC_SUCCESS  operation succeeded.
 * @retval SDC_FAILED   operation failed.
 *
 * @api
 */
bool_t sdcRead(SDCDriver *sdcp, uint32_t startblk,
               uint8_t *buf, uint32_t n) {
  bool_t status;

  chDbgCheck((sdcp != NULL) && (buf != NULL) && (n > 0), "sdcRead");

  if ((startblk + n - 1) > sdcp->capacity){
    sdcp->errors |= SDC_OVERFLOW_ERROR;
    return SDC_FAILED;
  }

  chSysLock();
  chDbgAssert(sdcp->state == SDC_ACTIVE, "sdcRead(), #1", "invalid state");
  sdcp->state = SDC_READING;
  chSysUnlock();

  status = sdc_lld_read(sdcp, startblk, buf, n);
  sdcp->state = SDC_ACTIVE;
  return status;
}

/**
 * @brief   Writes one or more blocks.
 * @pre     The driver must be in the @p SDC_ACTIVE state after a successful
 *          sdcConnect() invocation.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] n         number of blocks to write
 *
 * @return              The operation status.
 * @retval SDC_SUCCESS  operation succeeded.
 * @retval SDC_FAILED   operation failed.
 *
 * @api
 */
bool_t sdcWrite(SDCDriver *sdcp, uint32_t startblk,
                const uint8_t *buf, uint32_t n) {
  bool_t status;

  chDbgCheck((sdcp != NULL) && (buf != NULL) && (n > 0), "sdcWrite");

  if ((startblk + n - 1) > sdcp->capacity){
    sdcp->errors |= SDC_OVERFLOW_ERROR;
    return SDC_FAILED;
  }

  chSysLock();
  chDbgAssert(sdcp->state == SDC_ACTIVE, "sdcWrite(), #1", "invalid state");
  sdcp->state = SDC_WRITING;
  chSysUnlock();

  status = sdc_lld_write(sdcp, startblk, buf, n);
  sdcp->state = SDC_ACTIVE;
  return status;
}

#endif /* HAL_USE_SDC */

/** @} */
