/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
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

#include "hal.h"

#include "string.h" /* for memset() */

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/
/**
 * @brief   MMC switch mode.
 */
typedef enum {
  MMC_SWITCH_COMMAND_SET = 0,
  MMC_SWITCH_SET_BITS    = 1,
  MMC_SWITCH_CLEAR_BITS  = 2,
  MMC_SWITCH_WRITE_BYTE  = 3,
} mmc_switch_t;

/**
 * @brief   SDC switch mode.
 */
typedef enum {
  SD_SWITCH_CHECK = 0,
  SD_SWITCH_SET   = 1,
} sd_switch_t;

/**
 * @brief   SDC switch function.
 */
typedef enum {
  SD_SWITCH_FUNCTION_SPEED = 0,
  SD_SWITCH_FUNCTION_CMD_SYSTEM = 1,
  SD_SWITCH_FUNCTION_DRIVER_STRENGTH = 2,
  SD_SWITCH_FUNCTION_CURRENT_LIMIT = 3,
} sd_switch_function_t;

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Virtual methods table.
 */
static const struct SDCDriverVMT sdc_vmt = {
  (bool (*)(void *))sdc_lld_is_card_inserted,
  (bool (*)(void *))sdc_lld_is_write_protected,
  (bool (*)(void *))sdcConnect,
  (bool (*)(void *))sdcDisconnect,
  (bool (*)(void *, uint32_t, uint8_t *, uint32_t))sdcRead,
  (bool (*)(void *, uint32_t, const uint8_t *, uint32_t))sdcWrite,
  (bool (*)(void *))sdcSync,
  (bool (*)(void *, BlockDeviceInfo *))sdcGetInfo
};

/**
 * @brief     Temporal storage for different purposes (extended CSD, etc.).
 */
static uint8_t scratchpad[512];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
/**
 * @brief   Detects card mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool mode_detect(SDCDriver *sdcp) {
  uint32_t resp[1];

  /* V2.0 cards detection.*/
  if (!sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_IF_COND,
                                  MMCSD_CMD8_PATTERN, resp)) {
    sdcp->cardmode = SDC_MODE_CARDTYPE_SDV20;
    /* Voltage verification.*/
    if (((resp[0] >> 8) & 0xF) != 1)
      return HAL_FAILED;
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0, resp) ||
        MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
  }
  else {
    /* MMC or SD V1.1 detection.*/
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0, resp) ||
        MMCSD_R1_ERROR(resp[0]))
      sdcp->cardmode = SDC_MODE_CARDTYPE_MMC;
    else {
      sdcp->cardmode = SDC_MODE_CARDTYPE_SDV11;
    
      /* Reset error flag illegal command.*/
      sdc_lld_send_cmd_none(sdcp, MMCSD_CMD_GO_IDLE_STATE, 0);
    }
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Init procedure for MMC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool mmc_init(SDCDriver *sdcp) {
  uint32_t ocr;
  unsigned i;
  uint32_t resp[1];

  ocr = 0xC0FF8000;
  i = 0;
  while (true) {
    if (sdc_lld_send_cmd_short(sdcp, MMCSD_CMD_INIT, ocr, resp))
      return HAL_FAILED;
    if ((resp[0] & 0x80000000) != 0) {
      if (resp[0] & 0x40000000)
        sdcp->cardmode |= SDC_MODE_HIGH_CAPACITY;
      break;
    }
    if (++i >= SDC_INIT_RETRY)
      return HAL_FAILED;
    osalThreadSleep(OSAL_MS2ST(10));
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Init procedure for SDC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_init(SDCDriver *sdcp) {
  unsigned i;
  uint32_t ocr;
  uint32_t resp[1];

  if ((sdcp->cardmode &  SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_SDV20)
    ocr = 0xC0100000;
  else
    ocr = 0x80100000;

  i = 0;
  while (true) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0, resp) ||
      MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
    if (sdc_lld_send_cmd_short(sdcp, MMCSD_CMD_APP_OP_COND, ocr, resp))
      return HAL_FAILED;
    if ((resp[0] & 0x80000000) != 0) {
      if (resp[0] & 0x40000000)
        sdcp->cardmode |= SDC_MODE_HIGH_CAPACITY;
      break;
    }
    if (++i >= SDC_INIT_RETRY)
      return HAL_FAILED;
    osalThreadSleep(OSAL_MS2ST(10));
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Constructs CMD6 argument for MMC.
 *
 * @param[in] access    EXT_CSD access mode
 * @param[in] index     EXT_CSD byte number
 * @param[in] value     value to be written in target field
 * @param[in] cmd_set   switch current command set
 *
 * @return              CMD6 argument.
 *
 * @notapi
 */
static uint32_t mmc_cmd6_construct(mmc_switch_t access, uint8_t index,
                                   uint8_t value, uint8_t cmd_set) {

  osalDbgAssert((index <= 191), "This field is not writable");
  osalDbgAssert((cmd_set < 8), "This field has only 3 bits");

  return (access << 24) | (index << 16) | (value << 8) | cmd_set;
}

/**
 * @brief   Constructs CMD6 argument for SDC.
 *
 * @param[in] mode      switch/test mode
 * @param[in] function  function number to be switched
 * @param[in] value     value to be written in target function
 *
 * @return              CMD6 argument.
 *
 * @notapi
 */
uint32_t sdc_cmd6_construct(sd_switch_t mode, sd_switch_function_t function,
                           uint8_t value) {
  uint32_t ret = 0xFFFFFF;

  osalDbgAssert((value < 16), "This field has only 4 bits");

  ret &= ~(0xF << (function * 4));
  ret |= value << (function * 4);
  return ret | (mode << 31);
}

/**
 * @brief   Extracts information from CMD6 answer.
 *
 * @param[in] function  function number to be switched
 * @param[in] buf       buffer with answer
 *
 * @return              extracted answer.
 *
 * @notapi
 */
static uint16_t sdc_cmd6_extract_info(sd_switch_function_t function,
                                     const uint8_t *buf) {

  size_t start = 12 - function * 2;

  return (buf[start] << 8) | buf[start+1];
}

/**
 * @brief   Checks status after switching using CMD6.
 *
 * @param[in] function  function number to be switched
 * @param[in] buf       buffer with answer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_cmd6_check_status(sd_switch_function_t function,
                                 const uint8_t *buf) {

  uint32_t tmp;
  uint8_t status;

  tmp = (buf[14] << 16) | (buf[15] << 8) | buf[16];
  status = (tmp >> (function * 4)) & 0xF;
  if (0xF != status)
    return HAL_SUCCESS;
  else
    return HAL_FAILED;
}

/**
 * @brief   Reads supported bus clock and switch SDC to appropriate mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] clk      pointer to clock enum
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_detect_bus_clk(SDCDriver *sdcp, sdcbusclk_t *clk) {
  uint32_t cmdarg;

  *clk = SDC_CLK_25MHz; /* safe default */

  cmdarg = 0;
  memset(scratchpad, 0x55, sizeof(scratchpad));
  if (sdc_lld_read_special(sdcp, scratchpad, 64, MMCSD_CMD_SWITCH, cmdarg))
    return HAL_FAILED;

  if ((sdc_cmd6_extract_info(SD_SWITCH_FUNCTION_SPEED, scratchpad) & 2) == 2) {
    cmdarg = sdc_cmd6_construct(SD_SWITCH_SET, SD_SWITCH_FUNCTION_SPEED, 1);
    memset(scratchpad, 0x55, sizeof(scratchpad));
    if (sdc_lld_read_special(sdcp, scratchpad, 64, MMCSD_CMD_SWITCH, cmdarg))
      return HAL_FAILED;
    if (HAL_SUCCESS == sdc_cmd6_check_status(SD_SWITCH_FUNCTION_SPEED, scratchpad))
      *clk = SDC_CLK_50MHz;
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Reads supported bus clock and switch MMC to appropriate mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] clk      pointer to clock enum
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static sdcbusclk_t mmc_detect_bus_clk(SDCDriver *sdcp, sdcbusclk_t *clk) {
  uint32_t cmdarg;
  uint32_t resp[1];

  cmdarg = mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 185, 1, 0);
  if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SWITCH, cmdarg, resp) ||
                                 MMCSD_R1_ERROR(resp[0]))
    *clk = SDC_CLK_25MHz;
  else
    *clk = SDC_CLK_50MHz;

  return HAL_SUCCESS;
}

/**
 * @brief   Reads supported bus clock and switch card to appropriate mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] clk      pointer to clock enum
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool detect_bus_clk(SDCDriver *sdcp, sdcbusclk_t *clk) {

  if (SDC_MODE_CARDTYPE_MMC == (sdcp->cardmode & SDC_MODE_CARDTYPE_MASK))
    return mmc_detect_bus_clk(sdcp, clk);
  else
    return sdc_detect_bus_clk(sdcp, clk);
}

/**
 * @brief   Sets bus width for SDC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_set_bus_width(SDCDriver *sdcp) {
  uint32_t resp[1];

  if (SDC_MODE_1BIT == sdcp->config->bus_width) {
    /* Nothing to do. Bus is already in 1bit mode.*/
    return HAL_SUCCESS;
  }
  else if (SDC_MODE_4BIT == sdcp->config->bus_width) {
    sdc_lld_set_bus_mode(sdcp, SDC_MODE_4BIT);
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, sdcp->rca, resp) ||
        MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SET_BUS_WIDTH, 2, resp) ||
        MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
  }
  else {
    /* SD card does not support 8bit bus.*/
    return HAL_FAILED;
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Sets bus width for MMC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool mmc_set_bus_width(SDCDriver *sdcp) {
  uint32_t resp[1];
  uint32_t cmdarg = mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 183, 0, 0);

  switch(sdcp->config->bus_width){
  case SDC_MODE_1BIT:
    /* Nothing to do. Bus is already in 1bit mode.*/
    return HAL_SUCCESS;
    break;
  case SDC_MODE_4BIT:
    cmdarg = mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 183, 1, 0);
    break;
  case SDC_MODE_8BIT:
    cmdarg = mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 183, 2, 0);
    break;
  }

  sdc_lld_set_bus_mode(sdcp, sdcp->config->bus_width);
  if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SWITCH, cmdarg, resp) ||
      MMCSD_R1_ERROR(resp[0])) {
    return HAL_FAILED;
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Wait for the card to complete pending operations.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool _sdc_wait_for_transfer_state(SDCDriver *sdcp) {
  uint32_t resp[1];

  while (TRUE) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_STATUS,
                                   sdcp->rca, resp) ||
        MMCSD_R1_ERROR(resp[0]))
      return HAL_FAILED;
    switch (MMCSD_R1_STS(resp[0])) {
    case MMCSD_STS_TRAN:
      return HAL_SUCCESS;
    case MMCSD_STS_DATA:
    case MMCSD_STS_RCV:
    case MMCSD_STS_PRG:
#if SDC_NICE_WAITING
      osalThreadSleep(OSAL_MS2ST(1));
#endif
      continue;
    default:
      /* The card should have been initialized so any other state is not
         valid and is reported as an error.*/
      return HAL_FAILED;
    }
  }
  /* If something going too wrong.*/
  return HAL_FAILED;
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

  sdcp->vmt      = &sdc_vmt;
  sdcp->state    = BLK_STOP;
  sdcp->errors   = SDC_NO_ERROR;
  sdcp->config   = NULL;
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

  osalDbgCheck(sdcp != NULL);

  osalSysLock();
  osalDbgAssert((sdcp->state == BLK_STOP) || (sdcp->state == BLK_ACTIVE),
                "invalid state");
  sdcp->config = config;
  sdc_lld_start(sdcp);
  sdcp->state = BLK_ACTIVE;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @api
 */
void sdcStop(SDCDriver *sdcp) {

  osalDbgCheck(sdcp != NULL);

  osalSysLock();
  osalDbgAssert((sdcp->state == BLK_STOP) || (sdcp->state == BLK_ACTIVE),
                "invalid state");
  sdc_lld_stop(sdcp);
  sdcp->state = BLK_STOP;
  osalSysUnlock();
}

/**
 * @brief   Performs the initialization procedure on the inserted card.
 * @details This function should be invoked when a card is inserted and
 *          brings the driver in the @p BLK_READY state where it is possible
 *          to perform read and write operations.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @api
 */
bool sdcConnect(SDCDriver *sdcp) {
  uint32_t resp[1];
  sdcbusclk_t clk;

  osalDbgCheck(sdcp != NULL);
  osalDbgAssert((sdcp->state == BLK_ACTIVE) || (sdcp->state == BLK_READY),
                "invalid state");

  /* Connection procedure in progress.*/
  sdcp->state = BLK_CONNECTING;

  /* Card clock initialization.*/
  sdc_lld_start_clk(sdcp);

  /* Enforces the initial card state.*/
  sdc_lld_send_cmd_none(sdcp, MMCSD_CMD_GO_IDLE_STATE, 0);

  /* Detect card type.*/
  if (HAL_FAILED == mode_detect(sdcp))
    goto failed;

  /* Perform specific initialization procedure.*/
  if ((sdcp->cardmode &  SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_MMC) {
    if (HAL_FAILED == mmc_init(sdcp))
      goto failed;
  }
  else {
    if (HAL_FAILED == sdc_init(sdcp))
      goto failed;
  }

  /* Reads CID.*/
  if (sdc_lld_send_cmd_long_crc(sdcp, MMCSD_CMD_ALL_SEND_CID, 0, sdcp->cid))
    goto failed;

  /* Asks for the RCA.*/
  if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_RELATIVE_ADDR,
                                 0, &sdcp->rca))
    goto failed;

  /* Reads CSD.*/
  if (sdc_lld_send_cmd_long_crc(sdcp, MMCSD_CMD_SEND_CSD,
                                sdcp->rca, sdcp->csd))
    goto failed;

  /* Selects the card for operations.*/
  if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEL_DESEL_CARD,
                                 sdcp->rca, resp))
    goto failed;

  /* Switches to high speed.*/
  if (HAL_SUCCESS != detect_bus_clk(sdcp, &clk))
    goto failed;
  sdc_lld_set_data_clk(sdcp, clk);

  /* Reads extended CSD if needed.*/
  if (SDC_MODE_CARDTYPE_MMC == (sdcp->cardmode & SDC_MODE_CARDTYPE_MASK) &&
          mmcsdGetSlice(sdcp->csd, MMCSD_CSD_MMC_CSD_STRUCTURE_SLICE) > 1) {
    if(sdc_lld_read_special(sdcp, scratchpad, 512, MMCSD_CMD_SEND_EXT_CSD, 0))
      goto failed;
  }

  /* Block length fixed at 512 bytes.*/
  if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SET_BLOCKLEN,
                                 MMCSD_BLOCK_SIZE, resp) ||
      MMCSD_R1_ERROR(resp[0]))
    goto failed;

  /* Determine capacity.*/
  if (SDC_MODE_CARDTYPE_MMC == (sdcp->cardmode & SDC_MODE_CARDTYPE_MASK))
    sdcp->capacity = mmcsdGetCapacityMMC(sdcp->csd, scratchpad);
  else
    sdcp->capacity = mmcsdGetCapacity(sdcp->csd);

  if (sdcp->capacity == 0)
    goto failed;

  /* Switches to wide bus mode.*/
  switch (sdcp->cardmode & SDC_MODE_CARDTYPE_MASK) {
  case SDC_MODE_CARDTYPE_SDV11:
  case SDC_MODE_CARDTYPE_SDV20:
    if (HAL_FAILED == sdc_set_bus_width(sdcp))
      goto failed;
    break;
  case SDC_MODE_CARDTYPE_MMC:
    if (HAL_FAILED == mmc_set_bus_width(sdcp))
      goto failed;
    break;
  }

  /* Initialization complete.*/
  sdcp->state = BLK_READY;
  return HAL_SUCCESS;

  /* Connection failed, state reset to BLK_ACTIVE.*/
failed:
  sdc_lld_stop_clk(sdcp);
  sdcp->state = BLK_ACTIVE;
  return HAL_FAILED;
}

/**
 * @brief   Brings the driver in a state safe for card removal.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @api
 */
bool sdcDisconnect(SDCDriver *sdcp) {

  osalDbgCheck(sdcp != NULL);

  osalSysLock();
  osalDbgAssert((sdcp->state == BLK_ACTIVE) || (sdcp->state == BLK_READY),
                "invalid state");
  if (sdcp->state == BLK_ACTIVE) {
    osalSysUnlock();
    return HAL_SUCCESS;
  }
  sdcp->state = BLK_DISCONNECTING;
  osalSysUnlock();

  /* Waits for eventual pending operations completion.*/
  if (_sdc_wait_for_transfer_state(sdcp)) {
    sdc_lld_stop_clk(sdcp);
    sdcp->state = BLK_ACTIVE;
    return HAL_FAILED;
  }

  /* Card clock stopped.*/
  sdc_lld_stop_clk(sdcp);
  sdcp->state = BLK_ACTIVE;
  return HAL_SUCCESS;
}

/**
 * @brief   Reads one or more blocks.
 * @pre     The driver must be in the @p BLK_READY state after a successful
 *          sdcConnect() invocation.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] n         number of blocks to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @api
 */
bool sdcRead(SDCDriver *sdcp, uint32_t startblk, uint8_t *buf, uint32_t n) {
  bool status;

  osalDbgCheck((sdcp != NULL) && (buf != NULL) && (n > 0));
  osalDbgAssert(sdcp->state == BLK_READY, "invalid state");

  if ((startblk + n - 1) > sdcp->capacity){
    sdcp->errors |= SDC_OVERFLOW_ERROR;
    return HAL_FAILED;
  }

  /* Read operation in progress.*/
  sdcp->state = BLK_READING;

  status = sdc_lld_read(sdcp, startblk, buf, n);

  /* Read operation finished.*/
  sdcp->state = BLK_READY;
  return status;
}

/**
 * @brief   Writes one or more blocks.
 * @pre     The driver must be in the @p BLK_READY state after a successful
 *          sdcConnect() invocation.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] n         number of blocks to write
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @api
 */
bool sdcWrite(SDCDriver *sdcp, uint32_t startblk,
              const uint8_t *buf, uint32_t n) {
  bool status;

  osalDbgCheck((sdcp != NULL) && (buf != NULL) && (n > 0));
  osalDbgAssert(sdcp->state == BLK_READY, "invalid state");

  if ((startblk + n - 1) > sdcp->capacity){
    sdcp->errors |= SDC_OVERFLOW_ERROR;
    return HAL_FAILED;
  }

  /* Write operation in progress.*/
  sdcp->state = BLK_WRITING;

  status = sdc_lld_write(sdcp, startblk, buf, n);

  /* Write operation finished.*/
  sdcp->state = BLK_READY;
  return status;
}

/**
 * @brief   Returns the errors mask associated to the previous operation.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @return              The errors mask.
 *
 * @api
 */
sdcflags_t sdcGetAndClearErrors(SDCDriver *sdcp) {
  sdcflags_t flags;

  osalDbgCheck(sdcp != NULL);
  osalDbgAssert(sdcp->state == BLK_READY, "invalid state");

  osalSysLock();
  flags = sdcp->errors;
  sdcp->errors = SDC_NO_ERROR;
  osalSysUnlock();
  return flags;
}

/**
 * @brief   Waits for card idle condition.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  the operation succeeded.
 * @retval HAL_FAILED   the operation failed.
 *
 * @api
 */
bool sdcSync(SDCDriver *sdcp) {
  bool result;

  osalDbgCheck(sdcp != NULL);

  if (sdcp->state != BLK_READY)
    return HAL_FAILED;

  /* Synchronization operation in progress.*/
  sdcp->state = BLK_SYNCING;

  result = sdc_lld_sync(sdcp);

  /* Synchronization operation finished.*/
  sdcp->state = BLK_READY;
  return result;
}

/**
 * @brief   Returns the media info.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] bdip     pointer to a @p BlockDeviceInfo structure
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  the operation succeeded.
 * @retval HAL_FAILED   the operation failed.
 *
 * @api
 */
bool sdcGetInfo(SDCDriver *sdcp, BlockDeviceInfo *bdip) {

  osalDbgCheck((sdcp != NULL) && (bdip != NULL));

  if (sdcp->state != BLK_READY)
    return HAL_FAILED;

  bdip->blk_num = sdcp->capacity;
  bdip->blk_size = MMCSD_BLOCK_SIZE;

  return HAL_SUCCESS;
}

/**
 * @brief   Erases the supplied blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  starting block number
 * @param[in] endblk    ending block number
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  the operation succeeded.
 * @retval HAL_FAILED   the operation failed.
 *
 * @api
 */
bool sdcErase(SDCDriver *sdcp, uint32_t startblk, uint32_t endblk) {
  uint32_t resp[1];

  osalDbgCheck((sdcp != NULL));
  osalDbgAssert(sdcp->state == BLK_READY, "invalid state");

  /* Erase operation in progress.*/
  sdcp->state = BLK_WRITING;

  /* Handling command differences between HC and normal cards.*/
  if (!(sdcp->cardmode & SDC_MODE_HIGH_CAPACITY)) {
    startblk *= MMCSD_BLOCK_SIZE;
    endblk *= MMCSD_BLOCK_SIZE;
  }

  _sdc_wait_for_transfer_state(sdcp);

  if ((sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_ERASE_RW_BLK_START,
                                  startblk, resp) != HAL_SUCCESS) ||
      MMCSD_R1_ERROR(resp[0]))
    goto failed;

  if ((sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_ERASE_RW_BLK_END,
                                  endblk, resp) != HAL_SUCCESS) ||
      MMCSD_R1_ERROR(resp[0]))
    goto failed;

  if ((sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_ERASE,
                                  0, resp) != HAL_SUCCESS) ||
      MMCSD_R1_ERROR(resp[0]))
    goto failed;

  /* Quick sleep to allow it to transition to programming or receiving state */
  /* TODO: ??????????????????????????? */

  /* Wait for it to return to transfer state to indicate it has finished erasing */
  _sdc_wait_for_transfer_state(sdcp);

  sdcp->state = BLK_READY;
  return HAL_SUCCESS;

failed:
  sdcp->state = BLK_READY;
  return HAL_FAILED;
}

#endif /* HAL_USE_SDC */

/** @} */

