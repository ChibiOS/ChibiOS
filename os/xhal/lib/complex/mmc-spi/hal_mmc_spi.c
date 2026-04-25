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
/*
   Parts of this file have been contributed by Matthias Blaicher.
*/

/**
 * @file    hal_mmc_spi.c
 * @brief   MMC over SPI driver code.
 *
 * @addtogroup HAL_MMC_SPI
 * @{
 */

#include <stddef.h>
#include <string.h>

#include "hal_mmc_spi.h"

#if (MMC_SPI_USE_MODULE == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define MMC_SPI_R1_ERROR_MASK            0xFDFFE008U
#define MMC_SPI_CMD8_PATTERN             0x000001AAU

#define MMC_SPI_CMD_GO_IDLE_STATE        0U
#define MMC_SPI_CMD_INIT                 1U
#define MMC_SPI_CMD_SEND_IF_COND         8U
#define MMC_SPI_CMD_SEND_CSD             9U
#define MMC_SPI_CMD_SEND_CID             10U
#define MMC_SPI_CMD_STOP_TRANSMISSION    12U
#define MMC_SPI_CMD_SET_BLOCKLEN         16U
#define MMC_SPI_CMD_READ_MULTIPLE_BLOCK  18U
#define MMC_SPI_CMD_WRITE_MULTIPLE_BLOCK 25U
#define MMC_SPI_CMD_ERASE_RW_BLK_START   32U
#define MMC_SPI_CMD_ERASE_RW_BLK_END     33U
#define MMC_SPI_CMD_ERASE                38U
#define MMC_SPI_CMD_APP_OP_COND          41U
#define MMC_SPI_CMD_APP_CMD              55U
#define MMC_SPI_CMD_READ_OCR             58U

#define MMC_SPI_CSD_10_CSD_STRUCTURE_END   127U
#define MMC_SPI_CSD_10_CSD_STRUCTURE_START 126U
#define MMC_SPI_CSD_10_C_SIZE_END          73U
#define MMC_SPI_CSD_10_C_SIZE_START        62U
#define MMC_SPI_CSD_10_C_SIZE_MULT_END     49U
#define MMC_SPI_CSD_10_C_SIZE_MULT_START   47U
#define MMC_SPI_CSD_10_READ_BL_LEN_END     83U
#define MMC_SPI_CSD_10_READ_BL_LEN_START   80U
#define MMC_SPI_CSD_20_C_SIZE_END          69U
#define MMC_SPI_CSD_20_C_SIZE_START        48U

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Lookup table for CRC-7 (polynomial x^7 + x^3 + 1).
 */
static const uint8_t mmc_spi_crc7_lookup_table[256] = {
  0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f, 0x48, 0x41, 0x5a, 0x53,
  0x6c, 0x65, 0x7e, 0x77, 0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
  0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e, 0x32, 0x3b, 0x20, 0x29,
  0x16, 0x1f, 0x04, 0x0d, 0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
  0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14, 0x63, 0x6a, 0x71, 0x78,
  0x47, 0x4e, 0x55, 0x5c, 0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
  0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13, 0x7d, 0x74, 0x6f, 0x66,
  0x59, 0x50, 0x4b, 0x42, 0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
  0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69, 0x1e, 0x17, 0x0c, 0x05,
  0x3a, 0x33, 0x28, 0x21, 0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
  0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38, 0x41, 0x48, 0x53, 0x5a,
  0x65, 0x6c, 0x77, 0x7e, 0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
  0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67, 0x10, 0x19, 0x02, 0x0b,
  0x34, 0x3d, 0x26, 0x2f, 0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
  0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04, 0x6a, 0x63, 0x78, 0x71,
  0x4e, 0x47, 0x5c, 0x55, 0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
  0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a, 0x6d, 0x64, 0x7f, 0x76,
  0x49, 0x40, 0x5b, 0x52, 0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
  0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b, 0x17, 0x1e, 0x05, 0x0c,
  0x33, 0x3a, 0x21, 0x28, 0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
  0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31, 0x46, 0x4f, 0x54, 0x5d,
  0x62, 0x6b, 0x70, 0x79
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static uint32_t mmc_spi_get_slice(const uint32_t *data, uint32_t end,
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

static uint32_t mmc_spi_get_capacity(const uint32_t *csd) {
  uint32_t a, b, c;

  osalDbgCheck(csd != NULL);

  switch (mmc_spi_get_slice(csd, MMC_SPI_CSD_10_CSD_STRUCTURE_END,
                            MMC_SPI_CSD_10_CSD_STRUCTURE_START)) {
  case 0U:
    a = mmc_spi_get_slice(csd, MMC_SPI_CSD_10_C_SIZE_END,
                          MMC_SPI_CSD_10_C_SIZE_START);
    b = mmc_spi_get_slice(csd, MMC_SPI_CSD_10_C_SIZE_MULT_END,
                          MMC_SPI_CSD_10_C_SIZE_MULT_START);
    c = mmc_spi_get_slice(csd, MMC_SPI_CSD_10_READ_BL_LEN_END,
                          MMC_SPI_CSD_10_READ_BL_LEN_START);
    return ((a + 1U) << (b + 2U)) << (c - 9U);
  case 1U:
    return 1024U *
           (mmc_spi_get_slice(csd, MMC_SPI_CSD_20_C_SIZE_END,
                              MMC_SPI_CSD_20_C_SIZE_START) + 1U);
  default:
    break;
  }

  return 0U;
}

static uint8_t mmc_spi_crc7(uint8_t crc, const uint8_t *buffer, size_t len) {

  while (len > 0U) {
    crc = mmc_spi_crc7_lookup_table[(crc << 1U) ^ (*buffer++)];
    len--;
  }

  return crc;
}

static bool mmc_spi_apply_cfg(MMCSPIDriver *mmcp,
                              const hal_spi_config_t *config) {
  const hal_mmc_spi_config_t *mmccfg = mmcSpiGetConfigX(mmcp);

  if ((mmccfg == NULL) || (mmccfg->spip == NULL) || (config == NULL)) {
    return HAL_FAILED;
  }

  if (drvStart(mmccfg->spip, config) == HAL_RET_SUCCESS) {
    return HAL_SUCCESS;
  }

  if (drvSetCfgX(mmccfg->spip, config) == HAL_RET_SUCCESS) {
    return HAL_SUCCESS;
  }

  return HAL_FAILED;
}

static void mmc_spi_lock_bus(MMCSPIDriver *mmcp) {
#if (MMC_SPI_USE_MUTUAL_EXCLUSION == TRUE) && (HAL_USE_MUTUAL_EXCLUSION == TRUE)
  drvLock(mmcSpiGetConfigX(mmcp)->spip);
#else
  (void)mmcp;
#endif
}

static void mmc_spi_unlock_bus(MMCSPIDriver *mmcp) {
#if (MMC_SPI_USE_MUTUAL_EXCLUSION == TRUE) && (HAL_USE_MUTUAL_EXCLUSION == TRUE)
  drvUnlock(mmcSpiGetConfigX(mmcp)->spip);
#else
  (void)mmcp;
#endif
}

static bool mmc_spi_wait_idle(MMCSPIDriver *mmcp) {
  unsigned i;

  for (i = 0U; i < 16U; i++) {
    if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 1U, mmcp->buffer) !=
        HAL_RET_SUCCESS) {
      return HAL_FAILED;
    }
    if (mmcp->buffer[0] == 0xFFU) {
      return HAL_SUCCESS;
    }
  }

  i = 0U;
  do {
    if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 1U, mmcp->buffer) !=
        HAL_RET_SUCCESS) {
      return HAL_FAILED;
    }
    if (mmcp->buffer[0] == 0xFFU) {
      return HAL_SUCCESS;
    }

    osalThreadSleepMilliseconds(1);
  } while (++i < (unsigned)MMC_SPI_IDLE_TIMEOUT_MS);

  return HAL_FAILED;
}

static bool mmc_spi_send_hdr(MMCSPIDriver *mmcp, uint8_t cmd, uint32_t arg) {

  if (mmc_spi_wait_idle(mmcp) == HAL_FAILED) {
    return HAL_FAILED;
  }

  mmcp->buffer[0] = (uint8_t)(0x40U | cmd);
  mmcp->buffer[1] = (uint8_t)(arg >> 24U);
  mmcp->buffer[2] = (uint8_t)(arg >> 16U);
  mmcp->buffer[3] = (uint8_t)(arg >> 8U);
  mmcp->buffer[4] = (uint8_t)arg;
  mmcp->buffer[5] = ((mmc_spi_crc7(0U, mmcp->buffer, 5U) & 0x7FU) << 1U) |
                    0x01U;

  if (spiSend(mmcSpiGetConfigX(mmcp)->spip, 6U, mmcp->buffer) !=
      HAL_RET_SUCCESS) {
    return HAL_FAILED;
  }

  return HAL_SUCCESS;
}

static bool mmc_spi_recvr1(MMCSPIDriver *mmcp, uint8_t *r1p) {
  unsigned i;

  for (i = 0U; i < 9U; i++) {
    if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 1U, mmcp->buffer) !=
        HAL_RET_SUCCESS) {
      return HAL_FAILED;
    }
    *r1p = mmcp->buffer[0];
    if (*r1p != 0xFFU) {
      return HAL_SUCCESS;
    }
  }

  return HAL_FAILED;
}

static bool mmc_spi_recvr3(MMCSPIDriver *mmcp, uint8_t *r1p) {
  bool result;

  result = mmc_spi_recvr1(mmcp, r1p);
  if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 4U, mmcp->buffer) !=
      HAL_RET_SUCCESS) {
    return HAL_FAILED;
  }

  return result;
}

static bool mmc_spi_send_command_r1(MMCSPIDriver *mmcp, uint8_t cmd,
                                    uint32_t arg, uint8_t *r1p) {
  bool result;

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);
  result = mmc_spi_send_hdr(mmcp, cmd, arg);
  result = mmc_spi_recvr1(mmcp, r1p) || result;
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);

  return result;
}

static bool mmc_spi_send_command_r3(MMCSPIDriver *mmcp, uint8_t cmd,
                                    uint32_t arg, uint8_t *r1p) {
  bool result;

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);
  result = mmc_spi_send_hdr(mmcp, cmd, arg);
  result = mmc_spi_recvr3(mmcp, r1p) || result;
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);

  return result;
}

static bool mmc_spi_read_cxd(MMCSPIDriver *mmcp, uint8_t cmd, uint32_t cxd[4]) {
  unsigned i;
  uint8_t r1;
  uint8_t *bp;

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);

  if (mmc_spi_send_hdr(mmcp, cmd, 0U) ||
      mmc_spi_recvr1(mmcp, &r1) ||
      (r1 != 0x00U)) {
    spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
    return HAL_FAILED;
  }

  for (i = 0U; i < MMC_SPI_WAIT_DATA; i++) {
    if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 1U, mmcp->buffer) !=
        HAL_RET_SUCCESS) {
      spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
      return HAL_FAILED;
    }
    if (mmcp->buffer[0] == 0xFEU) {
      uint32_t *wp;

      if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 16U, mmcp->buffer) !=
          HAL_RET_SUCCESS) {
        spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
        return HAL_FAILED;
      }
      bp = mmcp->buffer;
      for (wp = &cxd[3]; wp >= cxd; wp--) {
        *wp = ((uint32_t)bp[0] << 24U) | ((uint32_t)bp[1] << 16U) |
              ((uint32_t)bp[2] << 8U)  | (uint32_t)bp[3];
        bp += 4;
      }

      (void)spiIgnore(mmcSpiGetConfigX(mmcp)->spip, 2U);
      spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
      return HAL_SUCCESS;
    }
  }

  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);

  return HAL_FAILED;
}

static bool mmc_spi_start_sequential_read(MMCSPIDriver *mmcp,
                                          uint32_t startblk) {
  uint8_t r1;
  bool error;

  if (mmcp->media_state != MMC_SPI_MEDIA_READY) {
    return HAL_FAILED;
  }

  mmcp->media_state = MMC_SPI_MEDIA_READING;

  if (mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->hscfg) != HAL_SUCCESS) {
    goto failed;
  }

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);

  if (mmcp->block_addresses) {
    error = mmc_spi_send_hdr(mmcp, MMC_SPI_CMD_READ_MULTIPLE_BLOCK, startblk);
  }
  else {
    error = mmc_spi_send_hdr(mmcp, MMC_SPI_CMD_READ_MULTIPLE_BLOCK,
                             startblk * MMC_SPI_BLOCK_SIZE);
  }

  if (error || mmc_spi_recvr1(mmcp, &r1) || (r1 != 0x00U)) {
    goto failed;
  }

  return HAL_SUCCESS;

failed:
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
  mmcp->media_state = MMC_SPI_MEDIA_READY;
  return HAL_FAILED;
}

static bool mmc_spi_sequential_read(MMCSPIDriver *mmcp, uint8_t *buffer) {
  unsigned i;

  if ((mmcp->media_state != MMC_SPI_MEDIA_READING) || (buffer == NULL)) {
    return HAL_FAILED;
  }

  for (i = 0U; i < MMC_SPI_WAIT_DATA; i++) {
    if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 1U, mmcp->buffer) !=
        HAL_RET_SUCCESS) {
      break;
    }
    if (mmcp->buffer[0] == 0xFEU) {
      if (spiReceive(mmcSpiGetConfigX(mmcp)->spip, MMC_SPI_BLOCK_SIZE,
                     buffer) != HAL_RET_SUCCESS) {
        break;
      }
      (void)spiIgnore(mmcSpiGetConfigX(mmcp)->spip, 2U);
      return HAL_SUCCESS;
    }
  }

  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
  mmcp->media_state = MMC_SPI_MEDIA_READY;

  return HAL_FAILED;
}

static bool mmc_spi_stop_sequential_read(MMCSPIDriver *mmcp) {
  static const uint8_t stopcmd[] = {
    (uint8_t)(0x40U | MMC_SPI_CMD_STOP_TRANSMISSION), 0U, 0U, 0U, 0U, 1U,
    0xFFU
  };
  uint8_t r1;

  if (mmcp->media_state != MMC_SPI_MEDIA_READING) {
    return HAL_FAILED;
  }

  if (spiSend(mmcSpiGetConfigX(mmcp)->spip, sizeof(stopcmd), stopcmd) !=
      HAL_RET_SUCCESS) {
    spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
    mmcp->media_state = MMC_SPI_MEDIA_READY;
    return HAL_FAILED;
  }

  (void)mmc_spi_recvr1(mmcp, &r1);
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
  mmcp->media_state = MMC_SPI_MEDIA_READY;

  return HAL_SUCCESS;
}

static bool mmc_spi_start_sequential_write(MMCSPIDriver *mmcp,
                                           uint32_t startblk) {
  uint8_t r1;
  bool error;

  if (mmcp->media_state != MMC_SPI_MEDIA_READY) {
    return HAL_FAILED;
  }

  mmcp->media_state = MMC_SPI_MEDIA_WRITING;

  if (mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->hscfg) != HAL_SUCCESS) {
    goto failed;
  }

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);

  if (mmcp->block_addresses) {
    error = mmc_spi_send_hdr(mmcp, MMC_SPI_CMD_WRITE_MULTIPLE_BLOCK, startblk);
  }
  else {
    error = mmc_spi_send_hdr(mmcp, MMC_SPI_CMD_WRITE_MULTIPLE_BLOCK,
                             startblk * MMC_SPI_BLOCK_SIZE);
  }

  if (error || mmc_spi_recvr1(mmcp, &r1) || (r1 != 0x00U)) {
    goto failed;
  }

  return HAL_SUCCESS;

failed:
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
  mmcp->media_state = MMC_SPI_MEDIA_READY;
  return HAL_FAILED;
}

static bool mmc_spi_sequential_write(MMCSPIDriver *mmcp,
                                     const uint8_t *buffer) {
  static const uint8_t start[] = {0xFFU, 0xFCU};

  if ((mmcp->media_state != MMC_SPI_MEDIA_WRITING) || (buffer == NULL)) {
    return HAL_FAILED;
  }

  if ((spiSend(mmcSpiGetConfigX(mmcp)->spip, sizeof(start), start) !=
       HAL_RET_SUCCESS) ||
      (spiSend(mmcSpiGetConfigX(mmcp)->spip, MMC_SPI_BLOCK_SIZE, buffer) !=
       HAL_RET_SUCCESS) ||
      (spiIgnore(mmcSpiGetConfigX(mmcp)->spip, 2U) != HAL_RET_SUCCESS) ||
      (spiReceive(mmcSpiGetConfigX(mmcp)->spip, 1U, mmcp->buffer) !=
       HAL_RET_SUCCESS)) {
    spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
    mmcp->media_state = MMC_SPI_MEDIA_READY;
    return HAL_FAILED;
  }

  if ((mmcp->buffer[0] & 0x1FU) == 0x05U) {
    return mmc_spi_wait_idle(mmcp);
  }

  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
  mmcp->media_state = MMC_SPI_MEDIA_READY;

  return HAL_FAILED;
}

static bool mmc_spi_stop_sequential_write(MMCSPIDriver *mmcp) {
  static const uint8_t stop[] = {0xFDU, 0xFFU};

  if (mmcp->media_state != MMC_SPI_MEDIA_WRITING) {
    return HAL_FAILED;
  }

  if (spiSend(mmcSpiGetConfigX(mmcp)->spip, sizeof(stop), stop) !=
      HAL_RET_SUCCESS) {
    spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
    mmcp->media_state = MMC_SPI_MEDIA_READY;
    return HAL_FAILED;
  }

  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);
  mmcp->media_state = MMC_SPI_MEDIA_READY;

  return HAL_SUCCESS;
}

static bool __mmcspi_blk_is_inserted_impl(void *ip) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiIsCardInserted(self);
}

static bool __mmcspi_blk_is_protected_impl(void *ip) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiIsWriteProtected(self);
}

static bool __mmcspi_blk_connect_impl(void *ip) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiConnect(self);
}

static bool __mmcspi_blk_disconnect_impl(void *ip) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiDisconnect(self);
}

static bool __mmcspi_blk_read_impl(void *ip, uint32_t startblk,
                                   uint8_t *buffer, uint32_t n) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiRead(self, startblk, buffer, n);
}

static bool __mmcspi_blk_write_impl(void *ip, uint32_t startblk,
                                    const uint8_t *buffer, uint32_t n) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiWrite(self, startblk, buffer, n);
}

static bool __mmcspi_blk_sync_impl(void *ip) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiSync(self);
}

static bool __mmcspi_blk_get_info_impl(void *ip, hal_blk_info_t *bdip) {
  MMCSPIDriver *self = oopIfGetOwner(MMCSPIDriver, ip);

  return mmcSpiGetInfo(self, bdip);
}

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

const struct hal_mmc_spi_driver_vmt __hal_mmc_spi_driver_vmt = {
  .dispose                  = __mmcspi_dispose_impl,
  .start                    = __mmcspi_start_impl,
  .stop                     = __mmcspi_stop_impl,
  .setcfg                   = __mmcspi_setcfg_impl,
  .selcfg                   = __mmcspi_selcfg_impl
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void *__mmcspi_objinit_impl(void *ip, const void *vmt) {
  MMCSPIDriver *self = (MMCSPIDriver *)ip;

  __drv_objinit_impl(self, vmt);

  {
    static const struct block_io_vmt mmc_spi_blk_vmt = {
      .instance_offset      = offsetof(MMCSPIDriver, blk),
      .is_inserted          = __mmcspi_blk_is_inserted_impl,
      .is_protected         = __mmcspi_blk_is_protected_impl,
      .connect              = __mmcspi_blk_connect_impl,
      .disconnect           = __mmcspi_blk_disconnect_impl,
      .read                 = __mmcspi_blk_read_impl,
      .write                = __mmcspi_blk_write_impl,
      .sync                 = __mmcspi_blk_sync_impl,
      .get_info             = __mmcspi_blk_get_info_impl
    };
    oopIfObjectInit(&self->blk, &mmc_spi_blk_vmt);
  }

  self->media_state = MMC_SPI_MEDIA_STOP;
  self->block_addresses = false;
  self->capacity = 0U;
  memset(self->cid, 0, sizeof(self->cid));
  memset(self->csd, 0, sizeof(self->csd));
  self->buffer = NULL;

  return self;
}

void __mmcspi_dispose_impl(void *ip) {
  MMCSPIDriver *self = (MMCSPIDriver *)ip;

  (void)self;
  __drv_dispose_impl(self);
}

msg_t __mmcspi_start_impl(void *ip, const void *config) {
  MMCSPIDriver *self = (MMCSPIDriver *)ip;

  if (config != NULL) {
    self->config = __mmcspi_setcfg_impl(self, config);
    if (self->config == NULL) {
      return HAL_RET_CONFIG_ERROR;
    }
  }

  if ((self->config == NULL) || (((const MMCSPIConfig *)self->config)->spip == NULL) ||
      (((const MMCSPIConfig *)self->config)->lscfg == NULL) ||
      (((const MMCSPIConfig *)self->config)->hscfg == NULL) ||
      (((const MMCSPIConfig *)self->config)->is_inserted == NULL) ||
      (self->buffer == NULL)) {
    return HAL_RET_CONFIG_ERROR;
  }

  self->media_state = MMC_SPI_MEDIA_ACTIVE;
  self->block_addresses = false;
  self->capacity = 0U;
  memset(self->cid, 0, sizeof(self->cid));
  memset(self->csd, 0, sizeof(self->csd));

  return HAL_RET_SUCCESS;
}

void __mmcspi_stop_impl(void *ip) {
  MMCSPIDriver *self = (MMCSPIDriver *)ip;

  self->media_state = MMC_SPI_MEDIA_STOP;
  self->block_addresses = false;
  self->capacity = 0U;
}

const void *__mmcspi_setcfg_impl(void *ip, const void *config) {
  MMCSPIDriver *self = (MMCSPIDriver *)ip;
  const MMCSPIConfig *mmccfg = (const MMCSPIConfig *)config;

  (void)self;

  if ((mmccfg == NULL) || (mmccfg->spip == NULL) || (mmccfg->lscfg == NULL) ||
      (mmccfg->hscfg == NULL) || (mmccfg->is_inserted == NULL)) {
    return NULL;
  }

  return config;
}

const void *__mmcspi_selcfg_impl(void *ip, unsigned cfgnum) {
  MMCSPIDriver *self = (MMCSPIDriver *)ip;

  (void)self;
  (void)cfgnum;

  return NULL;
}

bool mmcSpiConnect(MMCSPIDriver *mmcp) {
  unsigned i;
  uint8_t r1;
  bool success = HAL_FAILED;

  osalDbgCheck(mmcp != NULL);

  if ((drvGetStateX(mmcp) != HAL_DRV_STATE_READY) ||
      ((mmcp->media_state != MMC_SPI_MEDIA_ACTIVE) &&
       (mmcp->media_state != MMC_SPI_MEDIA_READY))) {
    return HAL_FAILED;
  }

  mmc_spi_lock_bus(mmcp);
  mmcp->media_state = MMC_SPI_MEDIA_CONNECTING;
  mmcp->block_addresses = false;

  if (mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->lscfg) != HAL_SUCCESS) {
    goto done;
  }

  if (spiIgnore(mmcSpiGetConfigX(mmcp)->spip, 16U) != HAL_RET_SUCCESS) {
    goto done;
  }

  i = 0U;
  while (true) {
    if ((mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_GO_IDLE_STATE, 0U, &r1) ==
         HAL_SUCCESS) && (r1 == 0x01U)) {
      break;
    }

    if (++i >= MMC_SPI_CMD0_RETRY) {
      goto done;
    }

    osalThreadSleepMilliseconds(10);
  }

  if (mmc_spi_send_command_r3(mmcp, MMC_SPI_CMD_SEND_IF_COND,
                              MMC_SPI_CMD8_PATTERN, &r1) == HAL_FAILED) {
    goto done;
  }

  if (r1 != 0x05U) {
    i = 0U;
    while (true) {
      if ((mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_APP_CMD, 0U, &r1) ==
           HAL_SUCCESS) && (r1 <= 0x01U) &&
          (mmc_spi_send_command_r3(mmcp, MMC_SPI_CMD_APP_OP_COND, 0x400001AAU,
                                   &r1) == HAL_SUCCESS) && (r1 == 0x00U)) {
        break;
      }

      if (++i >= MMC_SPI_ACMD41_RETRY) {
        goto done;
      }

      osalThreadSleepMilliseconds(10);
    }

    if (mmc_spi_send_command_r3(mmcp, MMC_SPI_CMD_READ_OCR, 0U, &r1) ==
        HAL_FAILED) {
      goto done;
    }

    if ((mmcp->buffer[0] & 0x40U) != 0U) {
      mmcp->block_addresses = true;
    }
  }

  i = 0U;
  while (true) {
    (void)mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_INIT, 0U, &r1);
    if (r1 == 0x00U) {
      break;
    }
    if ((r1 != 0x01U) || (++i >= MMC_SPI_CMD1_RETRY)) {
      goto done;
    }
    osalThreadSleepMilliseconds(10);
  }

  if ((mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->hscfg) != HAL_SUCCESS) ||
      (mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_SET_BLOCKLEN,
                               MMC_SPI_BLOCK_SIZE, &r1) == HAL_FAILED) ||
      (r1 != 0x00U) ||
      (mmc_spi_read_cxd(mmcp, MMC_SPI_CMD_SEND_CSD, mmcp->csd) ==
       HAL_FAILED)) {
    goto done;
  }

  mmcp->capacity = mmc_spi_get_capacity(mmcp->csd);
  if ((mmcp->capacity == 0U) ||
      (mmc_spi_read_cxd(mmcp, MMC_SPI_CMD_SEND_CID, mmcp->cid) ==
       HAL_FAILED)) {
    goto done;
  }

  success = HAL_SUCCESS;

done:
  mmcp->media_state = (success == HAL_SUCCESS) ? MMC_SPI_MEDIA_READY :
                                             MMC_SPI_MEDIA_ACTIVE;
  mmc_spi_unlock_bus(mmcp);

  return success;
}

bool mmcSpiDisconnect(MMCSPIDriver *mmcp) {
  bool result;

  osalDbgCheck(mmcp != NULL);

  if (drvGetStateX(mmcp) != HAL_DRV_STATE_READY) {
    return HAL_FAILED;
  }

  if (mmcp->media_state == MMC_SPI_MEDIA_ACTIVE) {
    return HAL_SUCCESS;
  }

  if (mmcp->media_state != MMC_SPI_MEDIA_READY) {
    return HAL_FAILED;
  }

  mmc_spi_lock_bus(mmcp);
  mmcp->media_state = MMC_SPI_MEDIA_DISCONNECTING;

  if (mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->hscfg) != HAL_SUCCESS) {
    mmcp->media_state = MMC_SPI_MEDIA_ACTIVE;
    mmc_spi_unlock_bus(mmcp);
    return HAL_FAILED;
  }

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);
  result = mmc_spi_wait_idle(mmcp);
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);

  mmcp->media_state = MMC_SPI_MEDIA_ACTIVE;
  mmc_spi_unlock_bus(mmcp);

  return result;
}

bool mmcSpiRead(MMCSPIDriver *mmcp, uint32_t startblk, uint8_t *buffer,
                uint32_t n) {
  bool err = HAL_FAILED;

  osalDbgCheck((mmcp != NULL) && (buffer != NULL));

  if ((drvGetStateX(mmcp) != HAL_DRV_STATE_READY) ||
      (mmcp->media_state != MMC_SPI_MEDIA_READY)) {
    return HAL_FAILED;
  }

  mmc_spi_lock_bus(mmcp);

  do {
    if (mmc_spi_start_sequential_read(mmcp, startblk) != HAL_SUCCESS) {
      break;
    }

    while (n > 0U) {
      if (mmc_spi_sequential_read(mmcp, buffer) != HAL_SUCCESS) {
        break;
      }
      buffer += MMC_SPI_BLOCK_SIZE;
      n--;
    }

    if ((n != 0U) || (mmc_spi_stop_sequential_read(mmcp) != HAL_SUCCESS)) {
      break;
    }

    err = HAL_SUCCESS;
  } while (false);

  if (mmcp->media_state == MMC_SPI_MEDIA_READING) {
    (void)mmc_spi_stop_sequential_read(mmcp);
  }

  mmc_spi_unlock_bus(mmcp);

  return err;
}

bool mmcSpiWrite(MMCSPIDriver *mmcp, uint32_t startblk, const uint8_t *buffer,
                 uint32_t n) {
  bool err = HAL_FAILED;

  osalDbgCheck((mmcp != NULL) && (buffer != NULL));

  if ((drvGetStateX(mmcp) != HAL_DRV_STATE_READY) ||
      (mmcp->media_state != MMC_SPI_MEDIA_READY)) {
    return HAL_FAILED;
  }

  mmc_spi_lock_bus(mmcp);

  do {
    if (mmc_spi_start_sequential_write(mmcp, startblk) != HAL_SUCCESS) {
      break;
    }

    while (n > 0U) {
      if (mmc_spi_sequential_write(mmcp, buffer) != HAL_SUCCESS) {
        break;
      }
      buffer += MMC_SPI_BLOCK_SIZE;
      n--;
    }

    if ((n != 0U) || (mmc_spi_stop_sequential_write(mmcp) != HAL_SUCCESS)) {
      break;
    }

    err = HAL_SUCCESS;
  } while (false);

  if (mmcp->media_state == MMC_SPI_MEDIA_WRITING) {
    (void)mmc_spi_stop_sequential_write(mmcp);
  }

  mmc_spi_unlock_bus(mmcp);

  return err;
}

bool mmcSpiSync(MMCSPIDriver *mmcp) {
  bool result;

  osalDbgCheck(mmcp != NULL);

  if ((drvGetStateX(mmcp) != HAL_DRV_STATE_READY) ||
      (mmcp->media_state != MMC_SPI_MEDIA_READY)) {
    return HAL_FAILED;
  }

  mmc_spi_lock_bus(mmcp);
  mmcp->media_state = MMC_SPI_MEDIA_SYNCING;

  if (mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->hscfg) != HAL_SUCCESS) {
    mmcp->media_state = MMC_SPI_MEDIA_READY;
    mmc_spi_unlock_bus(mmcp);
    return HAL_FAILED;
  }

  spiSelectX(mmcSpiGetConfigX(mmcp)->spip);
  result = mmc_spi_wait_idle(mmcp);
  spiUnselectX(mmcSpiGetConfigX(mmcp)->spip);

  mmcp->media_state = MMC_SPI_MEDIA_READY;
  mmc_spi_unlock_bus(mmcp);

  return result;
}

bool mmcSpiGetInfo(MMCSPIDriver *mmcp, hal_blk_info_t *bdip) {

  osalDbgCheck((mmcp != NULL) && (bdip != NULL));

  if ((drvGetStateX(mmcp) != HAL_DRV_STATE_READY) ||
      (mmcp->media_state != MMC_SPI_MEDIA_READY)) {
    return HAL_FAILED;
  }

  bdip->blk_num  = mmcp->capacity;
  bdip->blk_size = MMC_SPI_BLOCK_SIZE;

  return HAL_SUCCESS;
}

bool mmcSpiErase(MMCSPIDriver *mmcp, uint32_t startblk, uint32_t endblk) {
  uint8_t r1;
  bool result = HAL_FAILED;

  osalDbgCheck(mmcp != NULL);

  if ((drvGetStateX(mmcp) != HAL_DRV_STATE_READY) ||
      (mmcp->media_state != MMC_SPI_MEDIA_READY)) {
    return HAL_FAILED;
  }

  mmc_spi_lock_bus(mmcp);
  mmcp->media_state = MMC_SPI_MEDIA_WRITING;

  if (mmc_spi_apply_cfg(mmcp, mmcSpiGetConfigX(mmcp)->hscfg) != HAL_SUCCESS) {
    goto done;
  }

  if (!mmcp->block_addresses) {
    startblk *= MMC_SPI_BLOCK_SIZE;
    endblk   *= MMC_SPI_BLOCK_SIZE;
  }

  if ((mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_ERASE_RW_BLK_START, startblk,
                               &r1) == HAL_FAILED) || (r1 != 0x00U) ||
      (mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_ERASE_RW_BLK_END, endblk,
                               &r1) == HAL_FAILED) || (r1 != 0x00U) ||
      (mmc_spi_send_command_r1(mmcp, MMC_SPI_CMD_ERASE, 0U, &r1) ==
       HAL_FAILED) || (r1 != 0x00U)) {
    goto done;
  }

  result = HAL_SUCCESS;

done:
  mmcp->media_state = MMC_SPI_MEDIA_READY;
  mmc_spi_unlock_bus(mmcp);

  return result;
}

#endif /* MMC_SPI_USE_MODULE == TRUE */

/** @} */
