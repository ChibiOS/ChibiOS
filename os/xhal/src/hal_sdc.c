/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_sdc.c
 * @brief       Generated SDC Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SDC
 * @{
 */

#include "string.h"
#include "hal.h"

#if (HAL_USE_SDC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define MMCSD_BLOCK_SIZE                    512U

#define MMCSD_R1_ERROR_MASK                 0xFDFFE008U

#define MMCSD_CMD8_PATTERN                  0x000001AAU

#define MMCSD_STS_TRAN                      4U

#define MMCSD_CMD_GO_IDLE_STATE             0U

#define MMCSD_CMD_INIT                      1U

#define MMCSD_CMD_ALL_SEND_CID              2U

#define MMCSD_CMD_SEND_RELATIVE_ADDR        3U

#define MMCSD_CMD_SET_BUS_WIDTH             6U

#define MMCSD_CMD_SWITCH                    6U

#define MMCSD_CMD_SEL_DESEL_CARD            7U

#define MMCSD_CMD_SEND_IF_COND              8U

#define MMCSD_CMD_SEND_EXT_CSD              8U

#define MMCSD_CMD_SEND_CSD                  9U

#define MMCSD_CMD_STOP_TRANSMISSION         12U

#define MMCSD_CMD_SEND_STATUS               13U

#define MMCSD_CMD_SET_BLOCKLEN              16U

#define MMCSD_CMD_READ_SINGLE_BLOCK         17U

#define MMCSD_CMD_READ_MULTIPLE_BLOCK       18U

#define MMCSD_CMD_WRITE_BLOCK               24U

#define MMCSD_CMD_WRITE_MULTIPLE_BLOCK      25U

#define MMCSD_CMD_ERASE_RW_BLK_START        32U

#define MMCSD_CMD_ERASE_RW_BLK_END          33U

#define MMCSD_CMD_ERASE                     38U

#define MMCSD_CMD_APP_OP_COND               41U

#define MMCSD_CMD_APP_CMD                   55U

#define MMCSD_CMD_READ_OCR                  58U

#define MMCSD_CSD_MMC_CSD_STRUCTURE_END     127U

#define MMCSD_CSD_MMC_CSD_STRUCTURE_START   126U

#define MMCSD_CSD_10_CSD_STRUCTURE_END      127U

#define MMCSD_CSD_10_CSD_STRUCTURE_START    126U

#define MMCSD_CSD_10_C_SIZE_END             73U

#define MMCSD_CSD_10_C_SIZE_START           62U

#define MMCSD_CSD_10_C_SIZE_MULT_END        49U

#define MMCSD_CSD_10_C_SIZE_MULT_START      47U

#define MMCSD_CSD_10_READ_BL_LEN_END        83U

#define MMCSD_CSD_10_READ_BL_LEN_START      80U

#define MMCSD_CSD_20_C_SIZE_END             69U

#define MMCSD_CSD_20_C_SIZE_START           48U

#define MMCSD_ACMD6_BUS_WIDTH_4             2U

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/**
 * @brief       MMC switch mode.
 */
typedef enum {
  MMC_SWITCH_COMMAND_SET = 0,
  MMC_SWITCH_SET_BITS    = 1,
  MMC_SWITCH_CLEAR_BITS  = 2,
  MMC_SWITCH_WRITE_BYTE  = 3
} mmc_switch_t;

/**
 * @brief       SD switch mode.
 */
typedef enum {
  SD_SWITCH_CHECK = 0,
  SD_SWITCH_SET   = 1
} sd_switch_t;

/**
 * @brief       SD switch function selector.
 */
typedef enum {
  SD_SWITCH_FUNCTION_SPEED            = 0,
  SD_SWITCH_FUNCTION_CMD_SYSTEM       = 1,
  SD_SWITCH_FUNCTION_DRIVER_STRENGTH  = 2,
  SD_SWITCH_FUNCTION_CURRENT_LIMIT    = 3
} sd_switch_function_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief       Checks if a response contains R1 error bits.
 *
 * @param[in]     resp          Response word.
 * @return                      The error check result.
 */
static bool sdc_r1_error(uint32_t resp) {
  return ((resp & MMCSD_R1_ERROR_MASK) != 0U);
}

/**
 * @brief       Extracts a bit slice from a CSD/CID image.
 *
 * @param[in]     data          Input words.
 * @param[in]     end           End bit.
 * @param[in]     start         Start bit.
 * @return                      The selected bit slice.
 */
static uint32_t sdc_get_slice(const uint32_t *data, uint32_t end,
                              uint32_t start) {
  unsigned startidx, endidx, startoff;
  uint32_t endmask;

  osalDbgCheck((data != NULL) && (end >= start) && ((end - start) < 32U));

  startidx = start / 32U;
  startoff = start % 32U;
  endidx   = end / 32U;
  endmask  = ((uint32_t)1U << ((end % 32U) + 1U)) - 1U;

  if (startidx < endidx) {
    return (data[startidx] >> startoff) |
           ((data[endidx] & endmask) << (32U - startoff));
  }

  return (data[startidx] & endmask) >> startoff;
}

/**
 * @brief       Extracts the card capacity from a CSD record.
 *
 * @param[in]     csd           CSD record.
 * @return                      The card capacity in logical blocks.
 */
static uint32_t sdc_get_capacity(const uint32_t *csd) {
  uint32_t a, b, c;

  osalDbgCheck(csd != NULL);

  switch (sdc_get_slice(csd, MMCSD_CSD_10_CSD_STRUCTURE_END,
                        MMCSD_CSD_10_CSD_STRUCTURE_START)) {
    case 0U:
      a = sdc_get_slice(csd, MMCSD_CSD_10_C_SIZE_END,
                        MMCSD_CSD_10_C_SIZE_START);
      b = sdc_get_slice(csd, MMCSD_CSD_10_C_SIZE_MULT_END,
                        MMCSD_CSD_10_C_SIZE_MULT_START);
      c = sdc_get_slice(csd, MMCSD_CSD_10_READ_BL_LEN_END,
                        MMCSD_CSD_10_READ_BL_LEN_START);
      return ((a + 1U) << (b + 2U)) << (c - 9U);
    case 1U:
      return 1024U * (sdc_get_slice(csd, MMCSD_CSD_20_C_SIZE_END,
                                    MMCSD_CSD_20_C_SIZE_START) + 1U);
    default:
      break;
  }

  return 0U;
}

/**
 * @brief       Extracts the card capacity from an MMC EXT_CSD record.
 *
 * @param[in]     ext_csd       EXT_CSD record.
 * @return                      The card capacity in logical blocks.
 */
static uint32_t sdc_get_capacity_ext(const uint8_t *ext_csd) {
  osalDbgCheck(ext_csd != NULL);

  return ((uint32_t)ext_csd[215] << 24U) +
         ((uint32_t)ext_csd[214] << 16U) +
         ((uint32_t)ext_csd[213] << 8U)  +
         (uint32_t)ext_csd[212];
}

/**
 * @brief       Detects the inserted card type.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 */
static bool sdc_mode_detect(hal_sdc_driver_c *sdcp) {
  uint32_t resp[1];

  if (!sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_IF_COND,
                                  MMCSD_CMD8_PATTERN, resp)) {
    sdcp->cardmode = SDC_MODE_CARDTYPE_SDV20;
    if (((resp[0] >> 8U) & 0xFU) != 1U) {
      return HAL_FAILED;
    }
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0U, resp) ||
        sdc_r1_error(resp[0])) {
      return HAL_FAILED;
    }
  }
  else {
      if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0U, resp) ||
          sdc_r1_error(resp[0])) {
      sdcp->cardmode = SDC_MODE_CARDTYPE_MMC;
    }
    else {
      sdcp->cardmode = SDC_MODE_CARDTYPE_SDV11;
      sdc_lld_send_cmd_none(sdcp, MMCSD_CMD_GO_IDLE_STATE, 0U);
    }
  }

  return HAL_SUCCESS;
}

/**
 * @brief       Performs MMC-specific card initialization.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 */
static bool sdc_mmc_init(hal_sdc_driver_c *sdcp) {
  uint32_t ocr = 0xC0FF8000U;
  unsigned i = 0U;
  uint32_t resp[1];

  while (true) {
    if (sdc_lld_send_cmd_short(sdcp, MMCSD_CMD_INIT, ocr, resp)) {
      return HAL_FAILED;
    }
    if ((resp[0] & 0x80000000U) != 0U) {
      if ((resp[0] & 0x40000000U) != 0U) {
        sdcp->cardmode |= SDC_MODE_HIGH_CAPACITY;
      }
      break;
    }
    if (++i >= (unsigned)SDC_INIT_RETRY) {
      return HAL_FAILED;
    }
    osalThreadSleepMilliseconds(10);
  }

  return HAL_SUCCESS;
}

/**
 * @brief       Performs SD-specific card initialization.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 */
static bool sdc_sd_init(hal_sdc_driver_c *sdcp) {
  unsigned i = 0U;
  uint32_t ocr;
  uint32_t resp[1];

  if ((sdcp->cardmode & SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_SDV20) {
    ocr = SDC_INIT_OCR_V20;
  }
  else {
    ocr = SDC_INIT_OCR;
  }

  while (true) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, 0U, resp) ||
        sdc_r1_error(resp[0])) {
      return HAL_FAILED;
    }
      if (sdc_lld_send_cmd_short(sdcp, MMCSD_CMD_APP_OP_COND, ocr, resp)) {
      return HAL_FAILED;
    }
    if ((resp[0] & 0x80000000U) != 0U) {
      if ((resp[0] & 0x40000000U) != 0U) {
        sdcp->cardmode |= SDC_MODE_HIGH_CAPACITY;
      }
      break;
    }
    if (++i >= (unsigned)SDC_INIT_RETRY) {
      return HAL_FAILED;
    }
    osalThreadSleepMilliseconds(10);
  }

  return HAL_SUCCESS;
}

/**
 * @brief       Constructs an MMC CMD6 argument.
 *
 * @param[in]     access        Access mode.
 * @param[in]     idx           Field index.
 * @param[in]     value         Field value.
 * @param[in]     cmd_set       Command set.
 * @return                      The CMD6 argument.
 */
static uint32_t sdc_mmc_cmd6_construct(mmc_switch_t access, uint32_t idx,
                                       uint32_t value, uint32_t cmd_set) {
  osalDbgAssert(idx <= 191U, "field is not writable");
  osalDbgAssert(cmd_set < 8U, "field has only 3 bits");

  return ((uint32_t)access << 24U) | (idx << 16U) | (value << 8U) | cmd_set;
}

/**
 * @brief       Constructs an SD CMD6 argument.
 *
 * @param[in]     mode          Switch mode.
 * @param[in]     function      Switch function.
 * @param[in]     value         Requested value.
 * @return                      The CMD6 argument.
 */
static uint32_t sdc_sd_cmd6_construct(sd_switch_t mode,
                                      sd_switch_function_t function,
                                      uint32_t value) {
  uint32_t ret = 0xFFFFFFU;

  osalDbgAssert(value < 16U, "field has only 4 bits");

  ret &= ~((uint32_t)0xFU << ((uint32_t)function * 4U));
  ret |= value << ((uint32_t)function * 4U);

  return ret | ((uint32_t)mode << 31U);
}

/**
 * @brief       Extracts information from an SD CMD6 reply.
 *
 * @param[in]     function      Switch function.
 * @param[in]     buf           Reply buffer.
 * @return                      The extracted information.
 */
static uint16_t sdc_sd_cmd6_extract_info(sd_switch_function_t function,
                                         const uint8_t *buf) {
  unsigned start = 12U - ((unsigned)function * 2U);

  return ((uint16_t)buf[start] << 8U) | (uint16_t)buf[start + 1U];
}

/**
 * @brief       Checks the status bits in an SD CMD6 reply.
 *
 * @param[in]     function      Switch function.
 * @param[in]     buf           Reply buffer.
 * @return                      The operation status.
 */
static bool sdc_sd_cmd6_check_status(sd_switch_function_t function,
                                     const uint8_t *buf) {
  uint32_t tmp;
  uint32_t status;

  tmp = ((uint32_t)buf[14] << 16U) |
        ((uint32_t)buf[15] << 8U)  |
        (uint32_t)buf[16];
  status = (tmp >> ((uint32_t)function * 4U)) & 0xFU;

  if (status != 0xFU) {
    return HAL_SUCCESS;
  }

  return HAL_FAILED;
}

/**
 * @brief       Detects and selects the best supported data clock.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @param[out]    clk           Selected bus clock.
 * @return                      The operation status.
 */
static bool sdc_detect_bus_clk(hal_sdc_driver_c *sdcp, sdcbusclk_t *clk) {
  uint32_t cmdarg;
  uint16_t info;
  uint32_t resp[1];
  uint8_t *buf = sdcp->buf;

  if ((sdcp->cardmode & SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_MMC) {
    cmdarg = sdc_mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 185U, 1U, 0U);
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SWITCH, cmdarg, resp) ||
        sdc_r1_error(resp[0])) {
      return HAL_FAILED;
    }
    *clk = SDC_CLK_50MHz;
    return HAL_SUCCESS;
  }

  cmdarg = sdc_sd_cmd6_construct(SD_SWITCH_CHECK, SD_SWITCH_FUNCTION_SPEED, 1U);
      if (sdc_lld_read_special(sdcp, buf, 64U, MMCSD_CMD_SWITCH, cmdarg)) {
    return HAL_FAILED;
  }

  info = sdc_sd_cmd6_extract_info(SD_SWITCH_FUNCTION_SPEED, buf);
  if ((info & 0x0200U) != 0U) {
    cmdarg = sdc_sd_cmd6_construct(SD_SWITCH_SET, SD_SWITCH_FUNCTION_SPEED, 1U);
    if (sdc_lld_read_special(sdcp, buf, 64U, MMCSD_CMD_SWITCH, cmdarg)) {
      return HAL_FAILED;
    }
    if (sdc_sd_cmd6_check_status(SD_SWITCH_FUNCTION_SPEED, buf)) {
      return HAL_FAILED;
    }
    *clk = SDC_CLK_50MHz;
  }
  else {
    *clk = SDC_CLK_25MHz;
  }

  return HAL_SUCCESS;
}

/**
 * @brief       Switches an SD card to the configured bus width.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 */
static bool sdc_set_bus_width(hal_sdc_driver_c *sdcp) {
  uint32_t arg;
  uint32_t resp[1];

  switch (((const hal_sdc_config_t *)sdcp->config)->bus_width) {
    case SDC_MODE_4BIT:
      arg = MMCSD_ACMD6_BUS_WIDTH_4;
      break;
    case SDC_MODE_1BIT:
      return HAL_SUCCESS;
    default:
      return HAL_FAILED;
  }

    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_APP_CMD, sdcp->rca, resp) ||
        sdc_r1_error(resp[0])) {
    return HAL_FAILED;
  }

    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SET_BUS_WIDTH, arg, resp) ||
        sdc_r1_error(resp[0])) {
    return HAL_FAILED;
  }

  sdc_lld_set_bus_mode(sdcp, ((const hal_sdc_config_t *)sdcp->config)->bus_width);

  return HAL_SUCCESS;
}

/**
 * @brief       Switches an MMC card to the configured bus width.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 */
static bool sdc_mmc_set_bus_width(hal_sdc_driver_c *sdcp) {
  uint32_t cmdarg;
  uint32_t resp[1];

  switch (((const hal_sdc_config_t *)sdcp->config)->bus_width) {
    case SDC_MODE_1BIT:
      return HAL_SUCCESS;
    case SDC_MODE_4BIT:
      cmdarg = sdc_mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 183U, 1U, 0U);
      break;
    case SDC_MODE_8BIT:
      cmdarg = sdc_mmc_cmd6_construct(MMC_SWITCH_WRITE_BYTE, 183U, 2U, 0U);
      break;
    default:
      return HAL_FAILED;
  }

    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SWITCH, cmdarg, resp) ||
        sdc_r1_error(resp[0])) {
    return HAL_FAILED;
  }

  sdc_lld_set_bus_mode(sdcp, ((const hal_sdc_config_t *)sdcp->config)->bus_width);

  return HAL_SUCCESS;
}

/**
 * @brief       Waits for the card transfer state.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 */
static bool _sdc_wait_for_transfer_state(hal_sdc_driver_c *sdcp) {
  uint32_t resp[1];

  while (true) {
    if (sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_SEND_STATUS, sdcp->rca,
                                   resp)) {
      return HAL_FAILED;
    }
    if (sdc_r1_error(resp[0])) {
      return HAL_FAILED;
    }
    if (((resp[0] >> 9U) & 0xFU) == MMCSD_STS_TRAN) {
      return HAL_SUCCESS;
    }
#if SDC_NICE_WAITING == TRUE
    osalThreadSleepMilliseconds(1);
#endif
  }
}

/**
 * @brief       Waits for the card transfer state ignoring CRC in the status
 *              command.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The operation status.
 */
static bool _sdc_wait_for_transfer_state_nocrc(hal_sdc_driver_c *sdcp) {
  uint32_t resp[1];

  while (true) {
    if (sdc_lld_send_cmd_short(sdcp, MMCSD_CMD_SEND_STATUS, sdcp->rca, resp)) {
      return HAL_FAILED;
    }
    if (sdc_r1_error(resp[0])) {
      return HAL_FAILED;
    }
    if (((resp[0] >> 9U) & 0xFU) == MMCSD_STS_TRAN) {
      return HAL_SUCCESS;
    }
#if SDC_NICE_WAITING == TRUE
    osalThreadSleepMilliseconds(1);
#endif
  }
}

#if (SDC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Performs a synchronous aligned block read.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @param[in]     startblk      Initial block number.
 * @param[out]    buf           Pointer to the read buffer.
 * @param[in]     n             Number of blocks to read.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 */
static bool sdc_sync_read_aligned(hal_sdc_driver_c *sdcp, uint32_t startblk,
                                  uint8_t *buf, uint32_t n) {
  if (sdcStartRead(sdcp, startblk, buf, n) != HAL_RET_SUCCESS) {
    return HAL_FAILED;
  }

  if (sdcSynchronize(sdcp, TIME_INFINITE) != MSG_OK) {
    return HAL_FAILED;
  }

  return HAL_SUCCESS;
}

/**
 * @brief       Performs a synchronous aligned block write.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @param[in]     startblk      Initial block number.
 * @param[in]     buf           Pointer to the write buffer.
 * @param[in]     n             Number of blocks to write.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 */
static bool sdc_sync_write_aligned(hal_sdc_driver_c *sdcp, uint32_t startblk,
                                   const uint8_t *buf, uint32_t n) {
  if (sdcStartWrite(sdcp, startblk, buf, n) != HAL_RET_SUCCESS) {
    return HAL_FAILED;
  }

  if (sdcSynchronize(sdcp, TIME_INFINITE) != MSG_OK) {
    return HAL_FAILED;
  }

  return HAL_SUCCESS;
}
#endif /* SDC_USE_SYNCHRONIZATION == TRUE */

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       SDC Driver initialization.
 *
 * @init
 */
void sdcInit(void) {
  sdc_lld_init();
}

/*===========================================================================*/
/* Module class "hal_sdc_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of hal_sdc_driver_c
 * @{
 */
/**
 * @brief       Implementation of interface method @p blkIsInserted().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @return                      The media state.
 */
static bool __sdc_blk_is_inserted_impl(void *ip) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);

  return sdcIsCardInserted(self);
}

/**
 * @brief       Implementation of interface method @p blkIsWriteProtected().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @return                      The media state.
 */
static bool __sdc_blk_is_protected_impl(void *ip) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);

  return sdcIsWriteProtected(self);
}

/**
 * @brief       Implementation of interface method @p blkConnect().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @return                      The operation status.
 */
static bool __sdc_blk_connect_impl(void *ip) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);

  return sdcConnect(self);
}

/**
 * @brief       Implementation of interface method @p blkDisconnect().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @return                      The operation status.
 */
static bool __sdc_blk_disconnect_impl(void *ip) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);

  return sdcDisconnect(self);
}

/**
 * @brief       Implementation of interface method @p blkRead().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @param         startblk      Initial block to read.
 * @param         buffer        Pointer to the read buffer.
 * @param         n             Number of blocks to read.
 * @return                      The operation status.
 */
static bool __sdc_blk_read_impl(void *ip, uint32_t startblk, uint8_t *buffer,
                                uint32_t n) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);
#if SDC_USE_SYNCHRONIZATION == TRUE
  return sdcRead(self, startblk, buffer, n);
#else
  (void)startblk;
  (void)buffer;
  (void)n;

  return HAL_FAILED;
#endif
}

/**
 * @brief       Implementation of interface method @p blkWrite().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @param         startblk      Initial block to write.
 * @param         buffer        Pointer to the write buffer.
 * @param         n             Number of blocks to write.
 * @return                      The operation status.
 */
static bool __sdc_blk_write_impl(void *ip, uint32_t startblk,
                                 const uint8_t *buffer, uint32_t n) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);
#if SDC_USE_SYNCHRONIZATION == TRUE
  return sdcWrite(self, startblk, buffer, n);
#else
  (void)startblk;
  (void)buffer;
  (void)n;

  return HAL_FAILED;
#endif
}

/**
 * @brief       Implementation of interface method @p blkSync().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @return                      The operation status.
 */
static bool __sdc_blk_sync_impl(void *ip) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);

  return sdcSync(self);
}

/**
 * @brief       Implementation of interface method @p blkGetInfo().
 *
 * @param[in,out] ip            Pointer to the @p block_io_i class interface.
 * @param         bdip          Device information buffer pointer.
 * @return                      The operation status.
 */
static bool __sdc_blk_get_info_impl(void *ip, hal_blk_info_t *bdip) {
  hal_sdc_driver_c *self = oopIfGetOwner(hal_sdc_driver_c, ip);

  return sdcGetInfo(self, bdip);
}
/** @} */

/**
 * @name        Methods implementations of hal_sdc_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_sdc_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__sdc_objinit_impl(void *ip, const void *vmt) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization of interface block_io_i.*/
  {
    static const struct block_io_vmt sdc_blk_vmt = {
      .instance_offset      = offsetof(hal_sdc_driver_c, blk),
      .is_inserted          = __sdc_blk_is_inserted_impl,
      .is_protected         = __sdc_blk_is_protected_impl,
      .connect              = __sdc_blk_connect_impl,
      .disconnect           = __sdc_blk_disconnect_impl,
      .read                 = __sdc_blk_read_impl,
      .write                = __sdc_blk_write_impl,
      .sync                 = __sdc_blk_sync_impl,
      .get_info             = __sdc_blk_get_info_impl
    };
    oopIfObjectInit(&self->blk, &sdc_blk_vmt);
  }

  /* Initialization code.*/
  self->errors = SDC_NO_ERROR;
  self->cardmode = 0U;
  self->rca = 0U;
  self->capacity = 0U;
  memset(self->cid, 0, sizeof(self->cid));
  memset(self->csd, 0, sizeof(self->csd));
#if SDC_USE_SYNCHRONIZATION == TRUE
  self->sync_transfer = NULL;
#endif
#if defined(SDC_DRIVER_EXT_INIT_HOOK)
  SDC_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance to be
 *                              disposed.
 */
void __sdc_dispose_impl(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     config        Driver configuration or @p NULL.
 * @return                      The operation status.
 */
msg_t __sdc_start_impl(void *ip, const void *config) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  msg_t msg;

  if (config != NULL) {
    self->config = __sdc_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  msg = sdc_lld_start(self);
  if (msg == HAL_RET_SUCCESS) {
    self->state = HAL_DRV_STATE_ACTIVE;
  }
  else {
    self->config = NULL;
    self->errors = SDC_NO_ERROR;
    self->cardmode = 0U;
    self->rca = 0U;
    self->capacity = 0U;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 */
void __sdc_stop_impl(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  sdc_lld_stop(self);
  self->errors = SDC_NO_ERROR;
  self->cardmode = 0U;
  self->rca = 0U;
  self->capacity = 0U;
#if SDC_USE_SYNCHRONIZATION == TRUE
  osalThreadResumeI(&self->sync_transfer, MSG_RESET);
#endif
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__sdc_setcfg_impl(void *ip, const void *config) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  return (const void *)sdc_lld_setcfg(self, (const hal_sdc_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__sdc_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  return (const void *)sdc_lld_selcfg(self, cfgnum);
}

/**
 * @brief       Override of method @p __cbdrv_on_cb_set().
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param         cb            Callback function to be associated. Passing @p
 *                              NULL disables the existing callback, if any.
 */
void __sdc_oncbset_impl(void *ip, drv_cb_t cb) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  sdc_lld_set_callback(self, cb);
}
/** @} */

/**
 * @brief       VMT structure of SDC driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_sdc_driver_vmt __hal_sdc_driver_vmt = {
  .dispose                  = __sdc_dispose_impl,
  .start                    = __sdc_start_impl,
  .stop                     = __sdc_stop_impl,
  .setcfg                   = __sdc_setcfg_impl,
  .selcfg                   = __sdc_selcfg_impl,
  .oncbset                  = __sdc_oncbset_impl
};

/**
 * @name        Regular methods of hal_sdc_driver_c
 * @{
 */
/**
 * @brief       Performs the card initialization procedure.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcConnect(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  uint32_t resp[1];
  sdcbusclk_t clk = SDC_CLK_25MHz;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_READY),
                "invalid state");

  self->errors = SDC_NO_ERROR;
  self->state = SDC_CONNECTING;

  sdc_lld_start_clk(self);
  sdc_lld_send_cmd_none(self, MMCSD_CMD_GO_IDLE_STATE, 0U);

  if (sdc_mode_detect(self)) {
    goto failed;
  }

  if ((self->cardmode & SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_MMC) {
    if (sdc_mmc_init(self)) {
      goto failed;
    }
  }
  else {
    if (sdc_sd_init(self)) {
      goto failed;
    }
  }

  if (sdc_lld_send_cmd_long_crc(self, MMCSD_CMD_ALL_SEND_CID, 0U, self->cid)) {
    goto failed;
  }

  if (sdc_lld_send_cmd_short_crc(self, MMCSD_CMD_SEND_RELATIVE_ADDR, 0U,
                                 &self->rca)) {
    goto failed;
  }

  if (sdc_lld_send_cmd_long_crc(self, MMCSD_CMD_SEND_CSD, self->rca,
                                self->csd)) {
    goto failed;
  }

  if (sdc_lld_send_cmd_short_crc(self, MMCSD_CMD_SEL_DESEL_CARD, self->rca,
                                 resp)) {
    goto failed;
  }

  if (sdc_detect_bus_clk(self, &clk)) {
    goto failed;
  }
  sdc_lld_set_data_clk(self, clk);

  if (_sdc_wait_for_transfer_state_nocrc(self)) {
    goto failed;
  }

  if ((self->cardmode & SDC_MODE_CARDTYPE_MASK) == SDC_MODE_CARDTYPE_MMC) {
    if (sdc_get_slice(self->csd, MMCSD_CSD_MMC_CSD_STRUCTURE_END,
                      MMCSD_CSD_MMC_CSD_STRUCTURE_START) > 1U) {
      uint8_t *ext_csd = self->buf;

      if (sdc_lld_read_special(self, ext_csd, 512U, MMCSD_CMD_SEND_EXT_CSD, 0U)) {
        goto failed;
      }
      self->capacity = sdc_get_capacity_ext(ext_csd);
    }
    else {
      self->capacity = sdc_get_capacity(self->csd);
    }
  }
  else {
    self->capacity = sdc_get_capacity(self->csd);
  }

  if (sdc_lld_send_cmd_short_crc(self, MMCSD_CMD_SET_BLOCKLEN,
                                 MMCSD_BLOCK_SIZE, resp) ||
      sdc_r1_error(resp[0])) {
    goto failed;
  }

  switch (self->cardmode & SDC_MODE_CARDTYPE_MASK) {
    case SDC_MODE_CARDTYPE_SDV11:
    case SDC_MODE_CARDTYPE_SDV20:
      if (sdc_set_bus_width(self)) {
        goto failed;
      }
      break;
    case SDC_MODE_CARDTYPE_MMC:
      if (sdc_mmc_set_bus_width(self)) {
        goto failed;
      }
      break;
    default:
      goto failed;
  }

  self->state = HAL_DRV_STATE_READY;
  return HAL_SUCCESS;

  failed:
  sdc_lld_stop_clk(self);
  self->state = HAL_DRV_STATE_ACTIVE;
  return HAL_FAILED;
}

/**
 * @brief       Brings the driver in a state safe for card removal.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcDisconnect(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_READY),
                "invalid state");

  if (self->state == HAL_DRV_STATE_ACTIVE) {
    return HAL_SUCCESS;
  }

  self->state = SDC_DISCONNECTING;
  if (_sdc_wait_for_transfer_state(self)) {
    sdc_lld_stop_clk(self);
    self->state = HAL_DRV_STATE_ACTIVE;
    return HAL_FAILED;
  }

  sdc_lld_stop_clk(self);
  self->state = HAL_DRV_STATE_ACTIVE;
  return HAL_SUCCESS;
}

/**
 * @brief       Starts an asynchronous block read operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     startblk      Initial block number.
 * @param[out]    buf           Pointer to the read buffer.
 * @param[in]     n             Number of blocks to read.
 * @return                      The operation status.
 *
 * @api
 */
msg_t sdcStartRead(void *ip, uint32_t startblk, uint8_t *buf, uint32_t n) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (buf != NULL) && (n > 0U));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if ((startblk >= self->capacity) || (n > (self->capacity - startblk))) {
    self->errors |= SDC_OVERFLOW_ERROR;
    return HAL_RET_CONFIG_ERROR;
  }

  self->errors = SDC_NO_ERROR;
  self->state = SDC_READING;
  msg = sdc_lld_start_read(self, startblk, buf, n);
  if (msg != HAL_RET_SUCCESS) {
    self->state = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Starts an asynchronous block write operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     startblk      Initial block number.
 * @param[in]     buf           Pointer to the write buffer.
 * @param[in]     n             Number of blocks to write.
 * @return                      The operation status.
 *
 * @api
 */
msg_t sdcStartWrite(void *ip, uint32_t startblk, const uint8_t *buf,
                    uint32_t n) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (buf != NULL) && (n > 0U));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  if ((startblk >= self->capacity) || (n > (self->capacity - startblk))) {
    self->errors |= SDC_OVERFLOW_ERROR;
    return HAL_RET_CONFIG_ERROR;
  }

  self->errors = SDC_NO_ERROR;
  self->state = SDC_WRITING;
  msg = sdc_lld_start_write(self, startblk, buf, n);
  if (msg != HAL_RET_SUCCESS) {
    self->state = HAL_DRV_STATE_READY;
  }

  return msg;
}

/**
 * @brief       Stops an ongoing data transfer.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @return                      The operation status.
 *
 * @api
 */
msg_t sdcStopTransfer(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  msg_t msg = HAL_RET_SUCCESS;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == SDC_READING) ||
                (self->state == SDC_WRITING),
                "invalid state");

  if ((self->state == SDC_READING) || (self->state == SDC_WRITING)) {
    osalSysLock();
    msg = sdc_lld_stop_transfer(self);
    self->state = HAL_DRV_STATE_READY;
#if SDC_USE_SYNCHRONIZATION == TRUE
    osalThreadResumeI(&self->sync_transfer, MSG_RESET);
#endif
    osalOsRescheduleS();
    osalSysUnlock();
  }

  return msg;
}

#if (SDC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Waits for transfer completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 *
 * @sclass
 */
msg_t sdcSynchronizeS(void *ip, sysinterval_t timeout) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == SDC_READING) ||
                (self->state == SDC_WRITING),
                "invalid state");

  if ((self->state == SDC_READING) || (self->state == SDC_WRITING)) {
    msg = osalThreadSuspendTimeoutS(&self->sync_transfer, timeout);
    if (msg == MSG_TIMEOUT) {
      (void)sdc_lld_stop_transfer(self);
      self->state = HAL_DRV_STATE_READY;
    }
  }
  else {
    msg = MSG_OK;
  }

  return msg;
}

/**
 * @brief       Waits for transfer completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 *
 * @api
 */
msg_t sdcSynchronize(void *ip, sysinterval_t timeout) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = sdcSynchronizeS(self, timeout);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Reads one or more logical blocks.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     startblk      Initial block number.
 * @param[out]    buf           Pointer to the read buffer.
 * @param[in]     n             Number of blocks to read.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcRead(void *ip, uint32_t startblk, uint8_t *buf, uint32_t n) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  osalDbgCheck((self != NULL) && (buf != NULL) && (n > 0U));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  if ((startblk >= self->capacity) || (n > (self->capacity - startblk))) {
    self->errors |= SDC_OVERFLOW_ERROR;
    return HAL_FAILED;
  }

  self->errors = SDC_NO_ERROR;
#if STM32_SDC_SDMMC_UNALIGNED_SUPPORT
  if ((((uintptr_t)buf) & 3U) != 0U) {
    while (n > 0U) {
      if (sdc_sync_read_aligned(self, startblk, self->buf, 1U)) {
        return HAL_FAILED;
      }
      memcpy(buf, self->buf, MMCSD_BLOCK_SIZE);
      buf += MMCSD_BLOCK_SIZE;
      startblk++;
      n--;
    }
    return HAL_SUCCESS;
  }
#else
  osalDbgAssert((((uintptr_t)buf & 3U) == 0U), "unaligned buffer");
#endif

  return sdc_sync_read_aligned(self, startblk, buf, n);
}

/**
 * @brief       Writes one or more logical blocks.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     startblk      Initial block number.
 * @param[in]     buf           Pointer to the write buffer.
 * @param[in]     n             Number of blocks to write.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcWrite(void *ip, uint32_t startblk, const uint8_t *buf, uint32_t n) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  osalDbgCheck((self != NULL) && (buf != NULL) && (n > 0U));
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  if ((startblk >= self->capacity) || (n > (self->capacity - startblk))) {
    self->errors |= SDC_OVERFLOW_ERROR;
    return HAL_FAILED;
  }

  self->errors = SDC_NO_ERROR;
#if STM32_SDC_SDMMC_UNALIGNED_SUPPORT
  if ((((uintptr_t)buf) & 3U) != 0U) {
    while (n > 0U) {
      memcpy(self->buf, buf, MMCSD_BLOCK_SIZE);
      if (sdc_sync_write_aligned(self, startblk, self->buf, 1U)) {
        return HAL_FAILED;
      }
      buf += MMCSD_BLOCK_SIZE;
      startblk++;
      n--;
    }
    return HAL_SUCCESS;
  }
#else
  osalDbgAssert((((uintptr_t)buf & 3U) == 0U), "unaligned buffer");
#endif

  return sdc_sync_write_aligned(self, startblk, buf, n);
}
#endif /* SDC_USE_SYNCHRONIZATION == TRUE */

/**
 * @brief       Gets and clears the cached driver errors.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @return                      The selected pending error flags.
 *
 * @api
 */
sdcflags_t sdcGetAndClearErrors(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  sdcflags_t flags;
  syssts_t sts;

  osalDbgCheck(self != NULL);

  sts = osalSysGetStatusAndLockX();
  flags = self->errors;
  self->errors = SDC_NO_ERROR;
  osalSysRestoreStatusX(sts);

  return flags;
}

/**
 * @brief       Waits for the card to become idle.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcSync(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  bool result;

  osalDbgCheck(self != NULL);
  if (self->state != HAL_DRV_STATE_READY) {
    return HAL_FAILED;
  }

  self->state = SDC_SYNCING;
  result = sdc_lld_sync(self);
  self->state = HAL_DRV_STATE_READY;

  return result;
}

/**
 * @brief       Returns media information.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[out]    bdip          Media information buffer.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcGetInfo(void *ip, hal_blk_info_t *bdip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  osalDbgCheck((self != NULL) && (bdip != NULL));

  if (self->state != HAL_DRV_STATE_READY) {
    return HAL_FAILED;
  }

  bdip->blk_num = self->capacity;
  bdip->blk_size = MMCSD_BLOCK_SIZE;

  return HAL_SUCCESS;
}

/**
 * @brief       Erases a range of logical blocks.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     startblk      Starting block.
 * @param[in]     endblk        Ending block.
 * @return                      The operation status.
 * @retval false                Operation succeeded.
 * @retval true                 Operation failed.
 *
 * @api
 */
bool sdcErase(void *ip, uint32_t startblk, uint32_t endblk) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  uint32_t resp[1];
  uint32_t start;
  uint32_t end;

  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "invalid state");

  if ((startblk >= self->capacity) || (endblk >= self->capacity) ||
      (endblk < startblk)) {
    self->errors |= SDC_OVERFLOW_ERROR;
    return HAL_FAILED;
  }

  self->state = SDC_WRITING;

  start = startblk;
  end = endblk;
  if ((self->cardmode & SDC_MODE_HIGH_CAPACITY) == 0U) {
    start *= MMCSD_BLOCK_SIZE;
    end *= MMCSD_BLOCK_SIZE;
  }

  if (_sdc_wait_for_transfer_state(self)) {
    goto failed;
  }

  if (sdc_lld_send_cmd_short_crc(self, MMCSD_CMD_ERASE_RW_BLK_START, start,
                                 resp) || sdc_r1_error(resp[0])) {
    goto failed;
  }

  if (sdc_lld_send_cmd_short_crc(self, MMCSD_CMD_ERASE_RW_BLK_END, end,
                                 resp) || sdc_r1_error(resp[0])) {
    goto failed;
  }

  if (sdc_lld_send_cmd_short_crc(self, MMCSD_CMD_ERASE, 0U, resp) ||
      sdc_r1_error(resp[0])) {
    goto failed;
  }

  if (_sdc_wait_for_transfer_state(self)) {
    goto failed;
  }

  self->state = HAL_DRV_STATE_READY;
  return HAL_SUCCESS;

  failed:
  self->state = HAL_DRV_STATE_READY;
  return HAL_FAILED;
}
/** @} */

#endif /* HAL_USE_SDC == TRUE */

/** @} */
